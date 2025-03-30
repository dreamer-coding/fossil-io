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
 #include <fossil/test/framework.h>

 #include "fossil/io/framework.h"
 
 // * * * * * * * * * * * * * * * * * * * * * * * *
 // * Fossil Logic Test Utilites
 // * * * * * * * * * * * * * * * * * * * * * * * *
 // Setup steps for things like test fixtures and
 // mock objects are set here.
 // * * * * * * * * * * * * * * * * * * * * * * * *
 
 // Define the test suite and add test cases
 FOSSIL_TEST_SUITE(cpp_output_suite);
 
 // Setup function for the test suite
 FOSSIL_SETUP(cpp_output_suite) {
     // Setup code here
 }
 
 // Teardown function for the test suite
 FOSSIL_TEARDOWN(cpp_output_suite) {
     // Teardown code here
 }
 
 // * * * * * * * * * * * * * * * * * * * * * * * *
 // * Fossil Logic Test Cases
 // * * * * * * * * * * * * * * * * * * * * * * * *
 // The test cases below are provided as samples, inspired
 // by the Meson build system's approach of using test cases
 // as samples for library usage.
 // * * * * * * * * * * * * * * * * * * * * * * * *
  
 FOSSIL_TEST_CASE(cpp_test_io_puts) {
     const char *input = "Hello, World!";
     fossil_io_puts(input);
     const char *output = fossil_io_get_last_output();
     ASSUME_ITS_EQUAL_CSTR(input, output);
 }
 
 FOSSIL_TEST_CASE(cpp_test_io_printf) {
     const char *format = "Hello, %s!";
     const char *name = "World";
     const char *expected = "Hello, World!";
     fossil_io_printf(format, name);
     const char *output = fossil_io_get_last_output();
     ASSUME_ITS_EQUAL_CSTR(expected, output);
 }
 
 FOSSIL_TEST_CASE(cpp_test_io_print_color) {
     const char *color = "red";
     const char *input = "Colored Text";
     fossil_io_print_color(color, "%s", input);
     const char *output = fossil_io_get_last_output();
     ASSUME_ITS_EQUAL_CSTR(input, output); // Assuming color is not part of the output string
 }
  
 // * * * * * * * * * * * * * * * * * * * * * * * *
 // * Fossil Logic Test Pool
 // * * * * * * * * * * * * * * * * * * * * * * * *
 
FOSSIL_TEST_GROUP(cpp_output_tests) {
    FOSSIL_TEST_ADD(cpp_output_suite, cpp_test_io_puts);
    FOSSIL_TEST_ADD(cpp_output_suite, cpp_test_io_printf);
    FOSSIL_TEST_ADD(cpp_output_suite, cpp_test_io_print_color);
 
    FOSSIL_TEST_REGISTER(cpp_output_suite);
}
