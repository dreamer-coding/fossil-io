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
FOSSIL_TEST_SUITE(c_input_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_input_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_input_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_io_gets_from_stream) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_no_offensive) {
    char input[] = "This is a clean sentence.\n";
    char expected[] = "This is a clean sentence.";
    char buffer[256];

    FILE *stream = tmpfile();
    fwrite(input, 1, strlen(input), stream);
    rewind(stream);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), stream);
    fclose(stream);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_with_punctuation) {
    char input[] = "This is a test with punctuation, and special characters!\n";
    char expected[] = "This is a test with punctuation, and special characters!";
    char buffer[256];

    FILE *stream = tmpfile();
    fwrite(input, 1, strlen(input), stream);
    rewind(stream);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), stream);
    fclose(stream);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_empty_input) {
    const char *input_data = "\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_only_whitespace) {
    const char *input_data = "   \n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_long_input) {
    const char *input_data = "This is a very long input string that exceeds the buffer size\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("This is a very long", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_gets_from_stream_ex) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    int error_code = 0;
    char *result = fossil_io_gets_from_stream_ex(buf, sizeof(buf), input_stream, &error_code);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_gets_utf8) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_utf8(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_int_valid) {
    const char *input = "12345";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_I32(12345, output);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_int_invalid) {
    const char *input = "123abc";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_float_invalid) {
    const char *input = "123.abc";
    float output;
    int result = fossil_io_validate_is_float(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_alnum_valid) {
    const char *input = "abc123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_alnum_invalid) {
    const char *input = "abc 123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_email_valid) {
    const char *input = "test@example.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_email_invalid) {
    const char *input = "test@com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_length_valid) {
    const char *input = "short";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_is_length_invalid) {
    const char *input = "this is a very long string";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(c_test_io_validate_read_secure_line_valid) {
    char input_data[] = "SecureInput";
    char buffer[256];
    size_t buffer_size = sizeof(buffer);

    // Mocking user input
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    int result = fossil_io_validate_read_secure_line(buffer, buffer_size);

    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_CSTR(input_data, buffer);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_validate_read_secure_line_invalid) {
    char buffer[10]; // Small buffer to test overflow handling
    size_t buffer_size = sizeof(buffer);

    int result = fossil_io_validate_read_secure_line(buffer, buffer_size);

    ASSUME_ITS_FALSE(result); // The function should fail due to insufficient buffer size
}

FOSSIL_TEST_CASE(c_test_io_display_menu) {
    const char *choices[] = {"Choice 1", "Choice 2", "Choice 3"};
    int num_choices = 3;
    const char *prompt = "Please select an option:";

    // Here, we will mock user input. Let's assume user selects choice 1 (index 0)
    int selected_choice = fossil_io_display_menu(prompt, choices, num_choices);

    ASSUME_ITS_EQUAL_I32(0, selected_choice); // User should select the first choice
}

FOSSIL_TEST_CASE(c_test_io_read_password_valid) {
    char buffer[256];
    size_t size = sizeof(buffer);

    // Mock password input
    const char *password = "Password123";
    FILE *input_stream = tmpfile();
    fwrite(password, 1, strlen(password), input_stream);
    rewind(input_stream);

    int result = fossil_io_read_password(buffer, size);

    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_CSTR(password, buffer);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_read_password_invalid) {
    char buffer[10]; // Small buffer to test overflow handling
    size_t size = sizeof(buffer);

    int result = fossil_io_read_password(buffer, size);

    ASSUME_ITS_FALSE(result); // Password input should fail due to small buffer size
}

FOSSIL_TEST_CASE(c_test_io_read_multiline_input_valid) {
    char input_data[] = "This is line 1.\nThis is line 2.";
    char buffer[256];
    size_t size = sizeof(buffer);

    // Mock multiline input
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    int result = fossil_io_read_multiline_input(buffer, size);

    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_CSTR(input_data, buffer);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_read_multiline_input_invalid) {
    char buffer[10]; // Small buffer to test overflow handling
    size_t size = sizeof(buffer);

    int result = fossil_io_read_multiline_input(buffer, size);

    ASSUME_ITS_FALSE(result); // Multiline input should fail due to small buffer size
}

FOSSIL_TEST_CASE(c_test_io_getch) {
    char input_char = 'A';

    // Mock user input
    FILE *input_stream = tmpfile();
    fwrite(&input_char, 1, sizeof(input_char), input_stream);
    rewind(input_stream);

    char result = fossil_io_getch();

    ASSUME_ITS_EQUAL_CCHAR(input_char, result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(c_test_io_show_progress) {
    // This test assumes the progress bar function doesn't return anything.
    // We'll just check that the progress is shown correctly.

    // Test with various progress values (0%, 50%, 100%)
    fossil_io_show_progress(0);
    fossil_io_show_progress(50);
    fossil_io_show_progress(100);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_input_tests) {
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_no_offensive);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_with_punctuation);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_empty_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_only_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_long_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_ex);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_utf8);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_int_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_int_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_float_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_alnum_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_alnum_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_length_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_length_invalid);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_read_secure_line_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_read_secure_line_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_display_menu);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_read_password_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_read_password_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_read_multiline_input_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_read_multiline_input_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_getch);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_show_progress);
    
    FOSSIL_TEST_REGISTER(c_input_suite);
}
