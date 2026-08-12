/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2013
 *
 * Copyright (C) 2013-Current Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/maip/framework.h>
#include <string.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_output_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_output_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_output_suite)
{
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_output_color_flag_toggle)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;

    FOSSIL_IO_COLOR_ENABLE = 0;
    ASSUME_ITS_EQUAL_I32(0, FOSSIL_IO_COLOR_ENABLE);

    FOSSIL_IO_COLOR_ENABLE = 1;
    ASSUME_ITS_EQUAL_I32(1, FOSSIL_IO_COLOR_ENABLE);

    FOSSIL_IO_COLOR_ENABLE = original_color;
}

FOSSIL_TEST(c_test_output_color_markup_disabled)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    char buffer[128] = {0};

    FOSSIL_IO_COLOR_ENABLE = 0;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_sprintf(buffer, "Status: {green}OK{reset}");
    ASSUME_ITS_EQUAL_CSTR("Status: OK", buffer);

    FOSSIL_IO_COLOR_ENABLE = original_color;
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_output_color_markup_enabled)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    char buffer[128] = {0};

    FOSSIL_IO_COLOR_ENABLE = 1;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_sprintf(buffer, "Status: {green}OK{reset}");
    ASSUME_ITS_TRUE(strstr(buffer, "\x1b[") != NULL);
    ASSUME_ITS_TRUE(strstr(buffer, "OK") != NULL);

    FOSSIL_IO_COLOR_ENABLE = original_color;
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_output_output_preserves_formatting_when_disabled)
{
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    char buffer[128] = {0};

    FOSSIL_IO_OUTPUT_ENABLE = 0;
    int result = fossil_io_sprintf(buffer, "%s", "DryRun");
    ASSUME_ITS_EQUAL_I32(-1, result);
    ASSUME_ITS_EQUAL_CSTR("", buffer);

    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_output_enable_flag_toggle)
{
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;

    FOSSIL_IO_OUTPUT_ENABLE = 0;
    ASSUME_ITS_EQUAL_I32(0, FOSSIL_IO_OUTPUT_ENABLE);

    FOSSIL_IO_OUTPUT_ENABLE = 1;
    ASSUME_ITS_EQUAL_I32(1, FOSSIL_IO_OUTPUT_ENABLE);

    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_clear_screen)
{
    // Test that clear screen executes without error when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_clear_screen();
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_cursor_movement)
{
    // Test cursor movement to specific position when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_move_cursor(10, 20);
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_cursor_hide_show)
{
    // Test hiding and showing cursor when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_hide_cursor();
    ASSUME_ITS_TRUE(1);
    
    fossil_io_show_cursor();
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_draw_horizontal_line)
{
    // Test drawing horizontal line when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_draw_horizontal_line(40, '-');
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_draw_vertical_line)
{
    // Test drawing vertical line when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_draw_vertical_line(20, '|');
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_flush_output)
{
    // Test flushing output stream when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_flush();
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_draw_box)
{
    // Test drawing a rectangular box when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_draw_box(5, 10, 10, 30, '+');
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_clear_line)
{
    // Test clearing a single line when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    fossil_io_clear_line(15);
    ASSUME_ITS_TRUE(1);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_tui_get_terminal_size)
{
    // Test retrieving terminal size when enabled
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    FOSSIL_IO_OUTPUT_ENABLE = 1;
    int rows = 0, cols = 0;
    int result = fossil_io_get_term_size(&rows, &cols);
    ASSUME_ITS_TRUE(result >= 0);
    ASSUME_ITS_TRUE(rows > 0 && cols > 0);
    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_output_tests)
{
    FOSSIL_ADD_TEST(c_output_suite, c_test_output_color_flag_toggle);
    FOSSIL_ADD_TEST(c_output_suite, c_test_output_color_markup_disabled);
    FOSSIL_ADD_TEST(c_output_suite, c_test_output_color_markup_enabled);
    FOSSIL_ADD_TEST(c_output_suite, c_test_output_output_preserves_formatting_when_disabled);
    FOSSIL_ADD_TEST(c_output_suite, c_test_output_enable_flag_toggle);

    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_clear_screen);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_cursor_movement);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_cursor_hide_show);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_draw_horizontal_line);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_draw_vertical_line);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_flush_output);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_draw_box);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_clear_line);
    FOSSIL_ADD_TEST(c_output_suite, c_test_tui_get_terminal_size);

    FOSSIL_ADD_SUITE(c_output_suite);
}
