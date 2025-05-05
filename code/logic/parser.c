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

static const char *g_app_name = "app";
static const char *g_version = "0.0.0";
static fossil_io_cmd_t *g_cmds = NULL;
static int g_cmd_count = 0;

extern char *_custom_strdup(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

// Built-in flags
static int dry_run = 0, verbose = 0, color = 0, sanity = 0, show_this = 0;

static void print_help(const fossil_io_cmd_t *cmd) {
    fossil_io_printf("{blue,bold}Usage:{cyan,italic} %s [command] [flags]{reset}\n", g_app_name);
    if (cmd) {
        fossil_io_printf("{blue,bold}Command:{cyan,italic} %s - %s{reset}\n", cmd->name, cmd->description);
        for (int i = 0; i < cmd->flag_count; i++)
            fossil_io_printf("{cyan,italic}  --%s\t%s{reset}\n", cmd->flags[i].name, cmd->flags[i].description);
        for (int i = 0; i < cmd->subcommand_count; i++)
            fossil_io_printf("{cyan,italic}  %s\t%s{reset}\n", cmd->subcommands[i].name, cmd->subcommands[i].description);
    } else {
        for (int i = 0; i < g_cmd_count; i++)
            fossil_io_printf("{cyan,italic}  %s\t%s{reset}\n", g_cmds[i].name, g_cmds[i].description);
    }
    fossil_io_puts("{blue,bold}Built-in flags:{cyan,italic} --help, --version, --dry-run, --this, --sanity, --color, --verbose{reset}");
}

static int is_flag(const char *arg) {
    return strncmp(arg, "--", 2) == 0;
}

static void set_builtin_flag(const char *arg) {
    if (!strcmp(arg, "--dry-run")) dry_run = 1;
    else if (!strcmp(arg, "--verbose")) verbose = 1;
    else if (!strcmp(arg, "--color")) color = 1;
    else if (!strcmp(arg, "--sanity")) sanity = 1;
    else if (!strcmp(arg, "--this")) show_this = 1;
    else if (!strcmp(arg, "--version")) { fossil_io_printf("{cyan,italic}%s v%s{reset}\n", g_app_name, g_version); exit(0); }
    else if (!strcmp(arg, "--help")) { print_help(NULL); exit(0); }
}

static void parse_value(const char *val, fossil_io_flag_t *flag) {
    switch (flag->type) {
        case FOSSIL_IO_TYPE_BOOL: *(int*)flag->value = 1; break;
        case FOSSIL_IO_TYPE_INT: *(int*)flag->value = atoi(val); break;
        case FOSSIL_IO_TYPE_FLOAT: *(float*)flag->value = (float)atof(val); break;
        case FOSSIL_IO_TYPE_STRING: *(const char**)flag->value = val; break;
        case FOSSIL_IO_TYPE_ARRAY: *(const char**)flag->value = val; break;
        case FOSSIL_IO_TYPE_FEATURE: *(int*)flag->value = 1; break;
        case FOSSIL_IO_TYPE_COMBO:
            if (!strcmp(val, "auto")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_AUTO;
            else if (!strcmp(val, "enable")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_ENABLE;
            else if (!strcmp(val, "disable")) *(fossil_io_combo_t*)flag->value = FOSSIL_IO_COMBO_DISABLE;
            break;
    }
}

void fossil_io_parser_init(const char *app_name, const char *version) {
    g_app_name = app_name;
    g_version = version;
}

void fossil_io_parser_add_command(fossil_io_cmd_t *cmd) {
    g_cmds = realloc(g_cmds, sizeof(fossil_io_cmd_t) * (g_cmd_count + 1));
    g_cmds[g_cmd_count++] = *cmd;
}

void fossil_io_parser_parse(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (is_flag(argv[i])) {
            set_builtin_flag(argv[i]);
            continue;
        }

        for (int c = 0; c < g_cmd_count; ++c) {
            fossil_io_cmd_t *cmd = &g_cmds[c];
            if (!strcmp(argv[i], cmd->name)) {
                int j = i + 1;
                while (j < argc) {
                    if (is_flag(argv[j])) {
                        for (int f = 0; f < cmd->flag_count; ++f) {
                            if (!strcmp(argv[j] + 2, cmd->flags[f].name)) {
                                if (cmd->flags[f].type == FOSSIL_IO_TYPE_BOOL) {
                                    *(int*)cmd->flags[f].value = 1;
                                } else if (j + 1 < argc && !is_flag(argv[j + 1])) {
                                    parse_value(argv[++j], &cmd->flags[f]);
                                }
                            }
                        }
                    } else {
                        for (int s = 0; s < cmd->subcommand_count; ++s) {
                            if (!strcmp(argv[j], cmd->subcommands[s].name)) {
                                if (cmd->subcommands[s].handler)
                                    cmd->subcommands[s].handler(&cmd->subcommands[s]);
                                return;
                            }
                        }
                    }
                    j++;
                }

                if (cmd->handler) cmd->handler(cmd);
                return;
            }
        }

        fossil_io_printf("{red,bold}Unknown command:{red,italic} %s{reset}\n", argv[i]);
        print_help(NULL);
        exit(1);
    }
}

// Built-in flag accessors
int fossil_io_parser_is_dry_run(void) {
    return dry_run;
}

int fossil_io_parser_is_verbose(void) {
    return verbose;
}

int fossil_io_parser_use_color(void) {
    return color;
}

int fossil_io_parser_do_sanity(void) {
    return sanity;
}

int fossil_io_parser_show_info(void) {
    return show_this;
}
