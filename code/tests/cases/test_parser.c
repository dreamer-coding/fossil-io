/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 07/01/2024
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/test/framework.h>
#include <fossil/io/framework.h>


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_TEST_SUITE(c_parser_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_parser_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_parser_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_parser_add_command) {
    fossil_io_cmd_t cmd = { .name = "test_command" };

    // Add a command to the parser
    fossil_io_parser_add_command(&cmd);

    // Verify the command exists
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_command("test_command"));
}

FOSSIL_TEST_CASE(c_test_parser_add_subcommand) {
    fossil_io_cmd_t parent_cmd = { .name = "parent_command" };
    fossil_io_cmd_t sub_cmd = { .name = "sub_command" };

    // Add the parent command
    fossil_io_parser_add_command(&parent_cmd);

    // Add a subcommand to the parent command
    fossil_io_parser_add_subcommand(&parent_cmd, &sub_cmd);

    // Verify the subcommand exists under the parent command
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_subcommand(&parent_cmd, "sub_command"));
}

FOSSIL_TEST_CASE(c_test_parser_add_flag) {
    fossil_io_cmd_t cmd = { .name = "test_command" };
    fossil_io_flag_t flag = { .name = "test_flag" };

    // Add the command
    fossil_io_parser_add_command(&cmd);

    // Add a flag to the command
    fossil_io_parser_add_flag(&cmd, &flag);

    // Verify the flag exists in the command
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_flag(&cmd, "test_flag"));
}

FOSSIL_TEST_CASE(c_test_parser_has_builtin_flag) {
    const char *builtin_flag = "--help";

    // Verify the built-in flag exists
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_builtin_flag(builtin_flag));
}

FOSSIL_TEST_CASE(c_test_parser_has_any_flag) {
    fossil_io_cmd_t cmd = { .name = "test_command" };
    fossil_io_flag_t flag = { .name = "test_flag" };

    // Add the command and flag
    fossil_io_parser_add_command(&cmd);
    fossil_io_parser_add_flag(&cmd, &flag);

    // Verify the flag exists in the parser
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_any_flag("test_flag"));
}

FOSSIL_TEST_CASE(c_test_parser_parse_arguments) {
    fossil_io_cmd_t cmd = { .name = "test_command" };
    fossil_io_flag_t flag = { .name = "--test_flag" };

    // Add the command and flag
    fossil_io_parser_add_command(&cmd);
    fossil_io_parser_add_flag(&cmd, &flag);

    // Simulate command-line arguments
    const char *argv[] = { "app", "test_command", "--test_flag" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the command and flag were recognized
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_command("test_command"));
    ASSUME_ITS_EQUAL_I32(1, fossil_io_parser_has_flag(&cmd, "--test_flag"));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_parser_test_cases) {
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_add_command);
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_add_subcommand);
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_add_flag);
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_has_builtin_flag);
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_has_any_flag);
    // FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_parse_arguments);

    FOSSIL_TEST_REGISTER(c_parser_suite);
} // end of group
