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
#ifndef FOSSIL_IO_PARSER_H
#define FOSSIL_IO_PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FOSSIL_IO_TYPE_BOOL,
    FOSSIL_IO_TYPE_INT,
    FOSSIL_IO_TYPE_FLOAT,
    FOSSIL_IO_TYPE_STRING,
    FOSSIL_IO_TYPE_ARRAY,
    FOSSIL_IO_TYPE_FEATURE,
    FOSSIL_IO_TYPE_COMBO  // auto, enable, disable
} fossil_io_type_t;

typedef enum {
    FOSSIL_IO_COMBO_AUTO,
    FOSSIL_IO_COMBO_ENABLE,
    FOSSIL_IO_COMBO_DISABLE
} fossil_io_combo_t;

typedef struct {
    const char *name;
    const char *description;
    fossil_io_type_t type;
    void *value;
} fossil_io_flag_t;

typedef struct fossil_io_cmd fossil_io_cmd_t;

typedef void (*fossil_io_cmd_handler_t)(fossil_io_cmd_t *cmd);

struct fossil_io_cmd {
    const char *name;
    const char *description;
    fossil_io_flag_t *flags;
    int flag_count;
    fossil_io_cmd_t *subcommands;
    int subcommand_count;
    fossil_io_cmd_handler_t handler;
};

// ==================================================================
// Functions
// ==================================================================

/**
 * @brief Initializes the fossil I/O parser with application-specific details.
 *
 * This function sets up the parser by providing the application name and version.
 * It must be called before using any other parser-related functions.
 *
 * @param app_name The name of the application.
 * @param version The version of the application.
 * @param discription A brief description of the application.
 */
void fossil_io_parser_init(const char *app_name, const char *version, const char *discription);

/**
 * @brief Adds a command to the fossil I/O parser.
 *
 * This function registers a new command that the parser can recognize and handle.
 * Commands must be added before calling the parse function.
 *
 * @param cmd A pointer to the command structure to be added.
 */
void fossil_io_parser_add_command(fossil_io_cmd_t *cmd);

/**
 * @brief Adds a subcommand to a parent command.
 *
 * This function registers a subcommand under a specified parent command.
 * Subcommands must be added before calling the parse function.
 *
 * @param parent_cmd A pointer to the parent command structure.
 * @param sub_cmd A pointer to the subcommand structure to be added.
 */
void fossil_io_parser_add_subcommand(fossil_io_cmd_t *parent_cmd, fossil_io_cmd_t *sub_cmd);

/**
 * @brief Checks if a command exists in the parser.
 *
 * This function determines whether a command with the specified name
 * has been registered in the parser.
 *
 * @param cmd_name The name of the command to check.
 * @return Non-zero if the command exists, otherwise 0.
 */
int fossil_io_parser_has_command(const char *cmd_name);

/**
 * @brief Checks if a subcommand exists under a parent command.
 *
 * This function determines whether a subcommand with the specified name
 * has been registered under the given parent command.
 *
 * @param parent_cmd A pointer to the parent command structure.
 * @param sub_cmd_name The name of the subcommand to check.
 * @return Non-zero if the subcommand exists, otherwise 0.
 */
int fossil_io_parser_has_subcommand(fossil_io_cmd_t *parent_cmd, const char *sub_cmd_name);

/**
 * @brief Checks if a flag exists in a command.
 *
 * This function determines whether a flag with the specified name
 * has been registered in the given command.
 *
 * @param cmd A pointer to the command structure.
 * @param flag_name The name of the flag to check.
 * @return Non-zero if the flag exists, otherwise 0.
 */
int fossil_io_parser_has_flag(fossil_io_cmd_t *cmd, const char *flag_name);

/**
 * @brief Checks if a built-in flag exists.
 *
 * This function determines whether a built-in flag with the specified name
 * is recognized by the parser.
 *
 * @param flag_name The name of the flag to check.
 * @return Non-zero if the built-in flag exists, otherwise 0.
 */
int fossil_io_parser_has_builtin_flag(const char *flag_name);

/**
 * @brief Checks if any flag exists in the parser.
 *
 * This function determines whether a flag with the specified name
 * has been registered in any command or as a built-in flag.
 *
 * @param flag_name The name of the flag to check.
 * @return Non-zero if the flag exists, otherwise 0.
 */
int fossil_io_parser_has_any_flag(const char *flag_name);

/**
 * @brief Adds a flag to a command.
 *
 * This function registers a flag that can be used with a specific command.
 * Flags must be added before calling the parse function.
 *
 * @param cmd A pointer to the command structure to which the flag will be added.
 * @param flag A pointer to the flag structure to be added.
 */
void fossil_io_parser_add_flag(fossil_io_cmd_t *cmd, fossil_io_flag_t *flag);

/**
 * @brief Parses the command-line arguments.
 *
 * This function processes the arguments passed to the application and matches
 * them against the registered commands and built-in flags.
 *
 * @param argc The argument count, typically passed from the main function.
 * @param argv The argument vector, typically passed from the main function.
 */
void fossil_io_parser_parse(int argc, char **argv);

/**
 * @brief Checks if the dry-run flag is enabled.
 *
 * This accessor function determines whether the dry-run mode is active,
 * which typically means no changes will be applied.
 *
 * @return Non-zero if dry-run mode is enabled, otherwise 0.
 */
int fossil_io_parser_is_dry_run(void);

/**
 * @brief Checks if verbose mode is enabled.
 *
 * This accessor function determines whether verbose output is enabled,
 * which typically provides additional details during execution.
 *
 * @return Non-zero if verbose mode is enabled, otherwise 0.
 */
int fossil_io_parser_is_verbose(void);

/**
 * @brief Checks if sanity checks are enabled.
 *
 * This accessor function determines whether the parser should perform
 * additional sanity checks during execution.
 *
 * @return Non-zero if sanity checks are enabled, otherwise 0.
 */
int fossil_io_parser_do_sanity(void);

/**
 * @brief Checks if informational messages should be displayed.
 *
 * This accessor function determines whether the parser should display
 * informational messages during execution.
 *
 * @return Non-zero if informational messages are enabled, otherwise 0.
 */
int fossil_io_parser_show_info(void);

#ifdef __cplusplus
}

/**
 * C++ wrapper for the Parser API.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        /**
         * Parser API for sanitizing strings.
         */
        class Parser {
        public:
            /**
             * Initializes the parser with application-specific details.
             *
             * @param app_name The name of the application.
             * @param version The version of the application.
             * @param discription A brief description of the application.
             */
            static void init(const char *app_name, const char *version, const char *discription) {
                fossil_io_parser_init(app_name, version, discription);
            }

            /**
             * Adds a command to the parser.
             *
             * @param cmd A pointer to the command structure to be added.
             */
            static void add_command(fossil_io_cmd_t *cmd) {
                fossil_io_parser_add_command(cmd);
            }

            /**
             * Adds a subcommand to a parent command.
             *
             * @param parent_cmd A pointer to the parent command structure.
             * @param sub_cmd A pointer to the subcommand structure to be added.
             */
            static void add_subcommand(fossil_io_cmd_t *parent_cmd, fossil_io_cmd_t *sub_cmd) {
                fossil_io_parser_add_subcommand(parent_cmd, sub_cmd);
            }

            /**
             * Adds a flag to a command.
             *
             * @param cmd A pointer to the command structure to which the flag will be added.
             * @param flag A pointer to the flag structure to be added.
             */
            static void add_flag(fossil_io_cmd_t *cmd, fossil_io_flag_t *flag) {
                fossil_io_parser_add_flag(cmd, flag);
            }

            /**
             * Parses command-line arguments.
             *
             * @param argc The argument count.
             * @param argv The argument vector.
             */
            static void parse(int argc, char **argv) {
                fossil_io_parser_parse(argc, argv);
            }

            /**
             * Checks if dry-run mode is enabled.
             *
             * @return Non-zero if dry-run mode is enabled, otherwise 0.
             */
            static int is_dry_run() {
                return fossil_io_parser_is_dry_run();
            }

            /**
             * Checks if verbose mode is enabled.
             *
             * @return Non-zero if verbose mode is enabled, otherwise 0.
             */
            static int is_verbose() {
                return fossil_io_parser_is_verbose();
            }

            /**
             * Checks if sanity checks are enabled.
             *
             * @return Non-zero if sanity checks are enabled, otherwise 0.
             */
            static int do_sanity() {
                return fossil_io_parser_do_sanity();
            }

            /**
             * Checks if informational messages should be displayed.
             *
             * @return Non-zero if informational messages are enabled, otherwise 0.
             */
            static int show_info() {
                return fossil_io_parser_show_info();
            }
        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
