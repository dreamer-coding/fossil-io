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

FOSSIL_TEST_CASE(c_test_parser_init) {
    // Initialize the parser with application name and version
    const char *app_name = "FossilApp";
    const char *version = "1.0.0";

    fossil_io_parser_init(app_name, version);

    // No direct assertions here, but ensure no crashes or errors
    ASSUME_ITS_TRUE(1); // Placeholder to indicate successful execution
}

FOSSIL_TEST_CASE(c_test_parser_add_command) {
    // Define a mock command
    fossil_io_cmd_t mock_cmd = {
        .name = "mock",
        .description = "Mock command for testing",
        .handler = NULL
    };

    // Add the command to the parser
    fossil_io_parser_add_command(&mock_cmd);

    // No direct assertions here, but ensure no crashes or errors
    ASSUME_ITS_TRUE(1); // Placeholder to indicate successful execution
}

FOSSIL_TEST_CASE(c_test_parser_parse_arguments) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--verbose", "--dry-run" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the flags are set correctly
    ASSUME_ITS_TRUE(fossil_io_parser_is_verbose());
    ASSUME_ITS_TRUE(fossil_io_parser_is_dry_run());
}

FOSSIL_TEST_CASE(c_test_parser_check_verbose_flag) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--verbose" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the verbose flag is enabled
    ASSUME_ITS_TRUE(fossil_io_parser_is_verbose());
}

FOSSIL_TEST_CASE(c_test_parser_check_dry_run_flag) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--dry-run" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the dry-run flag is enabled
    ASSUME_ITS_TRUE(fossil_io_parser_is_dry_run());
}

FOSSIL_TEST_CASE(c_test_parser_check_color_flag) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--color" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the color output flag is enabled
    ASSUME_ITS_TRUE(fossil_io_parser_use_color());
}

FOSSIL_TEST_CASE(c_test_parser_check_sanity_flag) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--sanity" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the sanity checks flag is enabled
    ASSUME_ITS_TRUE(fossil_io_parser_do_sanity());
}

FOSSIL_TEST_CASE(c_test_parser_check_info_flag) {
    // Mock command-line arguments
    const char *argv[] = { "fossil", "--this" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Verify the informational messages flag is enabled
    ASSUME_ITS_TRUE(fossil_io_parser_show_info());
}

FOSSIL_TEST_CASE(c_test_parser_invalid_arguments) {
    // Mock invalid command-line arguments
    const char *argv[] = { "fossil", "--invalid-flag" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse the arguments
    fossil_io_parser_parse(argc, (char **)argv);

    // Ensure no valid flags are set
    ASSUME_ITS_FALSE(fossil_io_parser_is_verbose());
    ASSUME_ITS_FALSE(fossil_io_parser_is_dry_run());
    ASSUME_ITS_FALSE(fossil_io_parser_use_color());
    ASSUME_ITS_FALSE(fossil_io_parser_do_sanity());
    ASSUME_ITS_FALSE(fossil_io_parser_show_info());
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_parser_test_cases) {
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_init);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_add_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_parse_arguments);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_check_verbose_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_check_dry_run_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_check_color_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_check_sanity_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_check_info_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_test_parser_invalid_arguments);

    FOSSIL_TEST_REGISTER(c_parser_suite);
} // end of group
