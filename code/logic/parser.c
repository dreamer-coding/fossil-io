/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/io/parser.h"
#include "fossil/io/output.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

static const char *g_app_name = "app";
static const char *g_version = "0.0.0";
static const char *g_description = "A command-line parser for Fossil Logic applications.";
static fossil_io_cmd_t *g_cmds = NULL;
static int g_cmd_count = 0;

// Built-in flags
static int dry_run = 0, verbose = 0, sanity = 0, show_this = 0;


static int levenshtein_distance(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    int **dp = malloc((la + 1) * sizeof(int *));
    for (int i = 0; i <= la; i++) {
        dp[i] = malloc((lb + 1) * sizeof(int));
        for (int j = 0; j <= lb; j++) {
            dp[i][j] = (i == 0) ? j : (j == 0) ? i : 0;
        }
    }
    for (int i = 1; i <= la; i++) {
        for (int j = 1; j <= lb; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = 1 + fmin(fmin(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
        }
    }
    int result = dp[la][lb];
    for (int i = 0; i <= la; i++) {
        free(dp[i]);
    }
    free(dp);
    return result;
}

static const char* suggest_closest(const char *input, const char **options, int option_count) {
    int best = 999, idx = -1;
    for (int i = 0; i < option_count; i++) {
        int d = levenshtein_distance(input, options[i]);
        if (d < best) { best = d; idx = i; }
    }
    return (best <= 3 && idx >= 0) ? options[idx] : NULL;
}

static const char* suggest_command(const char *input) {
    const char *commands[g_cmd_count];
    for (int i = 0; i < g_cmd_count; i++) {
        commands[i] = g_cmds[i].name;
    }
    return suggest_closest(input, commands, g_cmd_count);
}

static const char* suggest_subcommand(const char *input, const fossil_io_cmd_t *cmd) {
    if (!cmd || cmd->subcommand_count == 0) return NULL;
    const char *subcommands[cmd->subcommand_count];
    for (int i = 0; i < cmd->subcommand_count; i++) {
        subcommands[i] = cmd->subcommands[i].name;
    }
    return suggest_closest(input, subcommands, cmd->subcommand_count);
}

static const char* suggest_flag(const char *input, const fossil_io_cmd_t *cmd) {
    if (!cmd || cmd->flag_count == 0) return NULL;
    const char *flags[cmd->flag_count];
    for (int i = 0; i < cmd->flag_count; i++) {
        flags[i] = cmd->flags[i].name;
    }
    return suggest_closest(input, flags, cmd->flag_count);
}

static const char* suggest_builtin_flag(const char *input) {
    const char *builtin_flags[] = {"--dry-run", "--verbose", "--sanity", "--this", "--version", "--help"};
    int builtin_flag_count = sizeof(builtin_flags) / sizeof(builtin_flags[0]);
    return suggest_closest(input, builtin_flags, builtin_flag_count);
}

static void print_this(void) {
    fossil_io_printf("{blue,bold}Application:{cyan,italic} %s{reset}\n", g_app_name);
    fossil_io_printf("{blue,bold}Version    :{cyan,italic} v%s{reset}\n", g_version);
    fossil_io_printf("{blue,bold}Description:{cyan,italic} %s{reset}\n", g_description);
}

static void print_help(const fossil_io_cmd_t *cmd) {
    fossil_io_printf("{blue,bold}Usage:{cyan,italic} %s [command] [subcommand] [flags]{reset}\n", g_app_name);

    fossil_io_printf("{blue,bold}Built-in:{cyan,italic}\n");
    fossil_io_printf("  --help\tShow help for commands or subcommands\n");
    fossil_io_printf("  --version\tShow version information\n");
    fossil_io_printf("  --dry-run\tSimulate execution without side effects\n");
    fossil_io_printf("  --verbose\tEnable verbose output\n");
    fossil_io_printf("  --sanity\tPerform internal sanity checks\n");
    fossil_io_printf("  --this\tDisplay application metadata\n");
    fossil_io_printf("  --flag=<name>\tShow help for a specific flag\n");
    fossil_io_printf("{reset}");

    if (cmd) {
        fossil_io_printf("{blue,bold}Command:{reset}\n");
        fossil_io_printf("{blue,bold}>{cyan,italic} %s - %s{reset}\n", cmd->name, cmd->description);

        if (cmd->subcommand_count > 0) {
            fossil_io_printf("{blue,bold}Subcommands:{reset}\n");
            for (int i = 0; i < cmd->subcommand_count; i++) {
                fossil_io_printf("{cyan,italic}  %s\t%s{reset}\n", cmd->subcommands[i].name, cmd->subcommands[i].description);
            }
        }

        if (cmd->flag_count > 0) {
            fossil_io_printf("{blue,bold}Flags:{reset}\n");
            for (int i = 0; i < cmd->flag_count; i++) {
                fossil_io_printf("{cyan,italic}  --%s\t%s{reset}\n", cmd->flags[i].name, cmd->flags[i].description);
            }
        }
    } else {
        if (g_cmd_count > 0) {
            fossil_io_printf("{blue,bold}Commands:{reset}\n");
            for (int i = 0; i < g_cmd_count; i++) {
                fossil_io_printf("{cyan,italic}  %s\t%s{reset}\n", g_cmds[i].name, g_cmds[i].description);
            }
        }
    }
}

static int is_flag(const char *arg) {
    return strncmp(arg, "--", 2) == 0;
}

static void set_builtin_flag(const char *arg) {
    if (!strcmp(arg, "--dry-run")) dry_run = 1;
    else if (!strcmp(arg, "--verbose")) verbose = 1;
    else if (!strcmp(arg, "--sanity")) sanity = 1;
    else if (!strcmp(arg, "--this")) {
        show_this = 1;
        print_this();
        exit(0);
    }
    else if (!strcmp(arg, "--version")) {
        fossil_io_printf("{cyan,italic}%s v%s{reset}\n", g_app_name, g_version);
        exit(0);
    }
    else if (!strcmp(arg, "--help")) {
        print_help(g_cmds);
        exit(0);
    }
    else if (!strncmp(arg, "--flag=", 7)) {
        const char *name = arg + 7;
        for (int c = 0; c < g_cmd_count; c++) {
            for (int f = 0; f < g_cmds[c].flag_count; f++) {
                if (!strcmp(name, g_cmds[c].flags[f].name)) {
                    fossil_io_printf("{cyan,italic}--%s: %s{reset}\n", name, g_cmds[c].flags[f].description);
                    exit(0);
                }
            }
        }
        fossil_io_printf("{red,bold}No such flag:{red,italic} %s{reset}\n", name);
        exit(1);
    } else {
        fossil_io_printf("{red,bold}Unknown built-in flag:{red,italic} %s{reset}\n", arg);
        fossil_io_printf("{yellow,bold}Use --help for more information.{reset}\n");
        exit(1);
    }
}

static void parse_value(const char *val, fossil_io_flag_t *flag) {
    if (!flag || !flag->value) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL flag or uninitialized value in parse_value.{reset}\n");
        exit(EXIT_FAILURE);
    }

    switch (flag->type) {
        case FOSSIL_IO_TYPE_BOOL:
            *(int*)flag->value = 1;
            break;
        case FOSSIL_IO_TYPE_INT:
            *(int*)flag->value = atoi(val);
            break;
        case FOSSIL_IO_TYPE_FLOAT:
            *(float*)flag->value = (float)atof(val);
            break;
        case FOSSIL_IO_TYPE_STRING:
            *(const char**)flag->value = val;
            break;
        case FOSSIL_IO_TYPE_ARRAY:
            *(const char**)flag->value = val;
            break;
        case FOSSIL_IO_TYPE_FEATURE:
            *(int*)flag->value = 1;
            break;
        case FOSSIL_IO_TYPE_COMBO:
            if (!strcmp(val, "auto")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_AUTO;
            else if (!strcmp(val, "enable")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_ENABLE;
            else if (!strcmp(val, "disable")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_DISABLE;
            else {
                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Invalid value for combo type: %s.{reset}\n", val);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Unknown flag type in parse_value.{reset}\n");
            exit(EXIT_FAILURE);
    }
}

void fossil_io_parser_init(const char *app_name, const char *version, const char *description) {
    if (app_name) g_app_name = app_name;
    if (version) g_version = version;
    if (description) g_description = description;
}

void fossil_io_parser_add_command(fossil_io_cmd_t *cmd) {
    if (!cmd || !cmd->name || !cmd->description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Invalid or NULL command passed to fossil_io_parser_add_command.{reset}\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < g_cmd_count; ++i) {
        if (strcmp(g_cmds[i].name, cmd->name) == 0) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Command '%s' already exists.{reset}\n", cmd->name);
            exit(EXIT_FAILURE);
        }
    }

    fossil_io_cmd_t *new_cmds = realloc(g_cmds, sizeof(fossil_io_cmd_t) * (g_cmd_count + 1));
    if (!new_cmds) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed in fossil_io_parser_add_command.{reset}\n");
        exit(EXIT_FAILURE);
    }

    g_cmds = new_cmds; // Update g_cmds only after successful allocation
    memset(&g_cmds[g_cmd_count], 0, sizeof(fossil_io_cmd_t)); // Ensure the new command is zero-initialized
    g_cmds[g_cmd_count++] = *cmd;
}

void fossil_io_parser_add_subcommand(fossil_io_cmd_t *parent_cmd, fossil_io_cmd_t *sub_cmd) {
    if (!parent_cmd || !sub_cmd || !sub_cmd->name || !sub_cmd->description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL or invalid parent command or subcommand passed to fossil_io_parser_add_subcommand.{reset}\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < parent_cmd->subcommand_count; ++i) {
        if (strcmp(parent_cmd->subcommands[i].name, sub_cmd->name) == 0) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Subcommand '%s' already exists under command '%s'.{reset}\n", sub_cmd->name, parent_cmd->name);
            exit(EXIT_FAILURE);
        }
    }

    fossil_io_cmd_t *new_subcommands = realloc(parent_cmd->subcommands, sizeof(fossil_io_cmd_t) * (parent_cmd->subcommand_count + 1));
    if (!new_subcommands) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed in fossil_io_parser_add_subcommand.{reset}\n");
        exit(EXIT_FAILURE);
    }

    parent_cmd->subcommands = new_subcommands; // Update subcommands only after successful allocation
    memset(&parent_cmd->subcommands[parent_cmd->subcommand_count], 0, sizeof(fossil_io_cmd_t)); // Zero-initialize
    parent_cmd->subcommands[parent_cmd->subcommand_count++] = *sub_cmd;
}

void fossil_io_parser_add_flag(fossil_io_cmd_t *cmd, fossil_io_flag_t *flag) {
    if (!cmd || !flag || !flag->name || !flag->description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL or invalid command or flag passed to fossil_io_parser_add_flag.{reset}\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < cmd->flag_count; ++i) {
        if (strcmp(cmd->flags[i].name, flag->name) == 0) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Flag '--%s' already exists for command '%s'.{reset}\n", flag->name, cmd->name);
            exit(EXIT_FAILURE);
        }
    }

    fossil_io_flag_t *new_flags = realloc(cmd->flags, sizeof(fossil_io_flag_t) * (cmd->flag_count + 1));
    if (!new_flags) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed in fossil_io_parser_add_flag.{reset}\n");
        exit(EXIT_FAILURE);
    }

    cmd->flags = new_flags;
    memset(&cmd->flags[cmd->flag_count], 0, sizeof(fossil_io_flag_t)); // Zero-initialize
    cmd->flags[cmd->flag_count++] = *flag;
}

int fossil_io_parser_has_command(const char *cmd_name) {
    if (!cmd_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL command name passed to fossil_io_parser_has_command.{reset}\n");
        return 0;
    }

    for (int i = 0; i < g_cmd_count; ++i) {
        if (strcmp(g_cmds[i].name, cmd_name) == 0) {
            return 1; // Command found
        }
    }
    return 0; // Command not found
}

int fossil_io_parser_has_subcommand(fossil_io_cmd_t *parent_cmd, const char *sub_cmd_name) {
    if (!parent_cmd || !sub_cmd_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL parent command or subcommand name passed to fossil_io_parser_has_subcommand.{reset}\n");
        return 0;
    }

    for (int i = 0; i < parent_cmd->subcommand_count; ++i) {
        if (strcmp(parent_cmd->subcommands[i].name, sub_cmd_name) == 0) {
            return 1; // Subcommand found
        }
    }
    return 0; // Subcommand not found
}

int fossil_io_parser_has_flag(fossil_io_cmd_t *cmd, const char *flag_name) {
    if (!cmd || !flag_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL command or flag name passed to fossil_io_parser_has_flag.{reset}\n");
        return 0;
    }

    for (int i = 0; i < cmd->flag_count; ++i) {
        if (strcmp(cmd->flags[i].name, flag_name) == 0) {
            return 1; // Flag found
        }
    }
    return 0; // Flag not found
}

int fossil_io_parser_has_builtin_flag(const char *flag_name) {
    if (!flag_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL flag name passed to fossil_io_parser_has_builtin_flag.{reset}\n");
        return 0;
    }

    const char *builtin_flags[] = {"--dry-run", "--verbose", "--sanity", "--this", "--version", "--help"};
    int builtin_flag_count = sizeof(builtin_flags) / sizeof(builtin_flags[0]);

    for (int i = 0; i < builtin_flag_count; ++i) {
        if (strcmp(builtin_flags[i], flag_name) == 0) {
            return 1; // Built-in flag found
        }
    }
    return 0; // Built-in flag not found
}

int fossil_io_parser_has_any_flag(const char *flag_name) {
    if (!flag_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL flag name passed to fossil_io_parser_has_any_flag.{reset}\n");
        return 0;
    }

    // Check built-in flags
    if (fossil_io_parser_has_builtin_flag(flag_name)) {
        return 1; // Built-in flag found
    }

    // Check command-specific flags
    for (int c = 0; c < g_cmd_count; ++c) {
        if (fossil_io_parser_has_flag(&g_cmds[c], flag_name)) {
            return 1; // Command-specific flag found
        }
    }

    return 0; // No flag found
}

void fossil_io_parser_parse(int argc, char **argv) {
    if (argc < 2) {
        print_help(g_cmds);
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
        if (is_flag(argv[i])) {
            int flag_handled = 0;

            // Check built-in flags
            set_builtin_flag(argv[i]);
            flag_handled = 1;

            // Check command-specific flags
            for (int c = 0; c < g_cmd_count; ++c) {
                fossil_io_cmd_t *cmd = &g_cmds[c];
                for (int f = 0; f < cmd->flag_count; ++f) {
                    if (!strcmp(argv[i] + 2, cmd->flags[f].name)) {
                        flag_handled = 1;
                        if (cmd->flags[f].type == FOSSIL_IO_TYPE_BOOL) {
                            *(int*)cmd->flags[f].value = 1;
                        } else if (i + 1 < argc && !is_flag(argv[i + 1])) {
                            parse_value(argv[++i], &cmd->flags[f]);
                        }
                        break;
                    }
                }
            }

            if (!flag_handled) {
                fossil_io_printf("{red,bold}Unknown flag:{red,italic} %s{reset}\n", argv[i]);
                const char *suggest = suggest_builtin_flag(argv[i]);
                if (suggest) {
                    fossil_io_printf("{yellow,bold}Did you mean:{yellow,italic} %s?{reset}\n", suggest);
                }
                fossil_io_printf("{yellow,bold}Use --help for more information.{reset}\n");
                print_help(g_cmds);
                exit(1);
            }
            continue;
        }

        int command_found = 0;
        for (int c = 0; c < g_cmd_count; ++c) {
            fossil_io_cmd_t *cmd = &g_cmds[c];
            if (!strcmp(argv[i], cmd->name)) {
                command_found = 1;
                int j = i + 1;

                while (j < argc) {
                    if (is_flag(argv[j])) {
                        int flag_found = 0;
                        for (int f = 0; f < cmd->flag_count; ++f) {
                            if (!strcmp(argv[j] + 2, cmd->flags[f].name)) {
                                flag_found = 1;
                                if (cmd->flags[f].type == FOSSIL_IO_TYPE_BOOL) {
                                    *(int*)cmd->flags[f].value = 1;
                                } else if (j + 1 < argc && !is_flag(argv[j + 1])) {
                                    parse_value(argv[++j], &cmd->flags[f]);
                                }
                                break;
                            }
                        }
                        if (!flag_found) {
                            fossil_io_printf("{red,bold}Unknown flag:{red,italic} %s{reset}\n", argv[j]);
                            const char *suggest = suggest_flag(argv[j] + 2, cmd);
                            if (suggest) {
                                fossil_io_printf("{yellow,bold}Did you mean:{yellow,italic} --%s?{reset}\n", suggest);
                            }
                            fossil_io_printf("{yellow,bold}Use --help for more information.{reset}\n");
                            print_help(cmd);
                            exit(1);
                        }
                    } else {
                        int subcommand_found = 0;
                        for (int s = 0; s < cmd->subcommand_count; ++s) {
                            if (!strcmp(argv[j], cmd->subcommands[s].name)) {
                                subcommand_found = 1;
                                if (cmd->subcommands[s].handler) {
                                    cmd->subcommands[s].handler(&cmd->subcommands[s]);
                                }
                                return;
                            }
                        }
                        if (!subcommand_found) {
                            fossil_io_printf("{red,bold}Unknown subcommand:{red,italic} %s{reset}\n", argv[j]);
                            const char *suggest = suggest_subcommand(argv[j], cmd);
                            if (suggest) {
                                fossil_io_printf("{yellow,bold}Did you mean:{yellow,italic} %s?{reset}\n", suggest);
                            }
                            fossil_io_printf("{yellow,bold}Use --help for more information.{reset}\n");
                            print_help(cmd);
                            exit(1);
                        }
                    }
                    j++;
                }

                if (cmd->handler) {
                    cmd->handler(cmd);
                }
                return;
            }
        }

        if (!command_found) {
            fossil_io_printf("{red,bold}Unknown command:{red,italic} %s{reset}\n", argv[i]);
            const char *suggest = suggest_command(argv[i]);
            if (suggest) {
                fossil_io_printf("{yellow,bold}Did you mean:{yellow,italic} %s?{reset}\n", suggest);
            } else {
                fossil_io_printf("{yellow,bold}Use --help for more information.{reset}\n");
            }
            print_help(g_cmds);
            exit(1);
        }
    }
}

// Built-in flag accessors
int fossil_io_parser_is_dry_run(void) {
    return dry_run;
}

int fossil_io_parser_is_verbose(void) {
    return verbose;
}

int fossil_io_parser_do_sanity(void) {
    return sanity;
}

int fossil_io_parser_show_info(void) {
    return show_this;
}
