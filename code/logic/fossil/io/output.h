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
#ifndef FOSSIL_IO_OUTPUT_H
#define FOSSIL_IO_OUTPUT_H

#include <stdarg.h>
#include "stream.h"

// Define color codes for output
#define FOSSIL_IO_COLOR_RESET       "\033[0m"
#define FOSSIL_IO_COLOR_RED         "\033[31m"
#define FOSSIL_IO_COLOR_GREEN       "\033[32m"
#define FOSSIL_IO_COLOR_YELLOW      "\033[33m"
#define FOSSIL_IO_COLOR_BLUE        "\033[34m"
#define FOSSIL_IO_COLOR_MAGENTA     "\033[35m"
#define FOSSIL_IO_COLOR_CYAN        "\033[36m"
#define FOSSIL_IO_COLOR_WHITE       "\033[37m"

// Bright colors
#define FOSSIL_IO_COLOR_BRIGHT_RED   "\033[91m"
#define FOSSIL_IO_COLOR_BRIGHT_GREEN "\033[92m"
#define FOSSIL_IO_COLOR_BRIGHT_YELLOW "\033[93m"
#define FOSSIL_IO_COLOR_BRIGHT_BLUE  "\033[94m"
#define FOSSIL_IO_COLOR_BRIGHT_MAGENTA "\033[95m"
#define FOSSIL_IO_COLOR_BRIGHT_CYAN  "\033[96m"
#define FOSSIL_IO_COLOR_BRIGHT_WHITE "\033[97m"

// Define text attributes
#define FOSSIL_IO_ATTR_BOLD         "\033[1m"
#define FOSSIL_IO_ATTR_UNDERLINE    "\033[4m"
#define FOSSIL_IO_ATTR_REVERSED     "\033[7m"
#define FOSSIL_IO_ATTR_BLINK        "\033[5m"
#define FOSSIL_IO_ATTR_HIDDEN       "\033[8m"
#define FOSSIL_IO_ATTR_NORMAL       "\033[22m" // For reverting to normal text

// Additional attributes
#define FOSSIL_IO_ATTR_ITALIC       "\033[3m"
#define FOSSIL_IO_ATTR_STRIKETHROUGH "\033[9m"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * This code provides a robust set of functions for formatting and manipulating terminal output, 
 * allowing developers to apply color, text attributes (like bold, underline, etc.), and cursor positioning 
 * to improve the visual appeal and functionality of text-based applications. The functionality is primarily 
 * based on ANSI escape sequences, which are widely supported in most terminal environments (such as Linux terminals, 
 * macOS Terminal, and Windows terminals that support ANSI escape codes).
 *
 * The core concept behind this system is a simple markup language that allows developers to specify formatting 
 * instructions within the text output. These instructions are enclosed in curly braces `{}` within the format string 
 * and are processed dynamically to change the appearance of the text. The markup supports various styles, such as:
 * 
 * 1. **Colors** - The code includes a set of predefined color codes for changing the foreground color of text. 
 *    Supported colors include basic colors (e.g., red, green, blue) and bright colors (e.g., bright red, bright green, etc.).
 *    The colors are implemented using ANSI escape sequences and can be easily extended to support more colors if needed.
 *
 * 2. **Attributes** - Text attributes can be applied to the text to change its appearance. These attributes include:
 *    - **Bold** (for making text bold)
 *    - **Underline** (for adding an underline to the text)
 *    - **Reversed** (for inverting the colors of the text and background)
 *    - **Blink** (for making text blink)
 *    - **Hidden** (for hiding the text)
 *    - **Normal** (for reverting text back to its normal form, removing any attributes)
 *
 * 3. **Positioning** - The code introduces a flexible way to manipulate the position of the text in the terminal using 
 *    named positions such as `top`, `bottom`, `left`, and `right`. These positions allow for text to be dynamically 
 *    placed at specific locations on the terminal screen, enhancing the user experience for applications requiring 
 *    more control over text layout and movement. The positions are marked with `pos:` followed by the desired 
 *    position name (e.g., `{pos:top}` or `{pos:left}`).
 * 
 * 4. **Flexibility and Extendability** - The markup language allows for the use of multiple color and attribute 
 *    specifications in a single string. The color and attribute specifications can be combined, for instance, 
 *    `{red,bold}` for red and bold text, or `{green,underline}` for green and underlined text. This allows for 
 *    fine-grained control over the text output. The system is flexible enough to be extended with more attributes, 
 *    colors, and positioning options as required.
 *
 * 5. **Implementation Details** - The function `fossil_io_print_with_attributes` processes the format string 
 *    provided to it, looking for `{}` markers. When it encounters a `{}`, it checks if the enclosed string specifies 
 *    a color, attribute, or position, and then calls the respective helper functions (`fossil_io_apply_color` and 
 *    `fossil_io_apply_attribute`) to modify the terminal output accordingly. If a position marker is found (e.g., 
 *    `{pos:top}`), it adjusts the cursor position in the terminal to the specified location. The code uses standard 
 *    C string manipulation functions like `strchr`, `strncpy`, and `vsnprintf` to process the format string and apply 
 *    the requested changes to the output.
 * 
 * In summary, this system provides a highly customizable and intuitive way to format terminal text with colors, 
 * attributes, and positions, making it ideal for developers who want to build visually rich and interactive 
 * command-line interfaces. The markup-based approach is simple to use and can be easily extended to meet the 
 * needs of more complex applications.
 */

/**
 * Prints a string to the output.
 * 
 * This function outputs the provided string `str` to the terminal or console. It is a simple utility function
 * that can be used for printing plain text to the screen. The string is printed as-is, with no formatting or
 * color modifications applied.
 *
 * @param str The string to be printed. This should be a null-terminated string.
 */
void fossil_io_puts(const char *str);

/** 
 * Prints a formatted string to the output.
 *
 * This function allows for formatted output, similar to `printf`. It takes a format string that can include
 * format specifiers (e.g., `%d`, `%s`, `%f`), and the additional arguments provided will be formatted accordingly.
 * The function uses a variable argument list (`...`) to handle a wide variety of format specifiers and argument types.
 * 
 * The format string can also include custom formatting markers enclosed in curly braces `{}`, such as `{red}` for
 * color or `{bold}` for text attributes, which will be processed and applied to the output.
 *
 * Example usage:
 * ```c
 * fossil_io_printf("Hello, %s! Your score is %d\n", "Alice", 95);
 * ```
 *
 * @param format The format string, which contains the text to be printed, along with format specifiers.
 * @param ... The additional arguments to be formatted. These arguments are inserted into the format string
 *            in the order they appear, based on the format specifiers.
 */
void fossil_io_printf(const char *format, ...);

/**
 * Prints a character to the output.
 * 
 * This function is a basic utility to print a single character to the output. It is especially useful when you
 * need to print individual characters rather than strings or formatted text.
 *
 * Example usage:
 * ```c
 * fossil_io_putchar('A');
 * ```
 *
 * @param c The character to be printed. This should be a single character.
 */
void fossil_io_putchar(char c);

/**
 * Prints a string to the specified output stream.
 * 
 * This function is similar to `fossil_io_puts`, but instead of printing to the standard output, it allows you
 * to specify an output stream (like a file or a custom output stream). This can be useful when writing to files
 * or other output destinations.
 *
 * Example usage:
 * ```c
 * FILE *file = fopen("output.txt", "w");
 * fossil_io_fputs(file, "Hello, File Output!\n");
 * fclose(file);
 * ```
 *
 * @param stream The output stream where the string should be printed. This should be a valid pointer to a `FILE` object.
 * @param str The string to be printed. This should be a null-terminated string.
 */
void fossil_io_fputs(fossil_fstream_t *stream, const char *str);

/**
 * Prints a formatted string to the specified output stream.
 * 
 * This function is similar to `fossil_io_printf`, but instead of printing to the standard output, it allows you
 * to specify an output stream. The format string can include format specifiers and custom formatting markers, just
 * like `fossil_io_printf`. This can be useful when writing formatted text to files or other output destinations.
 *
 * Example usage:
 * ```c
 * FILE *file = fopen("output.txt", "w");
 * fossil_io_fprintf(file, "Hello, %s! Your score is %d\n", "Alice", 95);
 * fclose(file);
 * ```
 *
 * @param stream The output stream where the formatted string should be printed. This should be a valid pointer to a `FILE` object.
 * @param format The format string, which contains the text to be printed, along with format specifiers.
 * @param ... The additional arguments to be formatted. These arguments are inserted into the format string
 *            in the order they appear, based on the format specifiers.
 */
void fossil_io_fprintf(fossil_fstream_t *stream, const char *format, ...);

/**
 * Prints a character to the specified output stream.
 * 
 * This function is similar to `fossil_io_putchar`, but it allows you to specify an output stream, such as a file.
 * It can be useful for printing individual characters to custom output destinations like files.
 *
 * Example usage:
 * ```c
 * FILE *file = fopen("output.txt", "w");
 * fossil_io_fputchar(file, 'A');
 * fclose(file);
 * ```
 *
 * @param stream The output stream where the character should be printed. This should be a valid pointer to a `FILE` object.
 * @param c The character to be printed. This should be a single character.
 */
void fossil_io_fputchar(fossil_fstream_t *stream, char c);

/**
 * Prints a character to the standard output.
 * 
 * This function is a simple wrapper around `putchar` and prints a single character to the standard output (usually the terminal).
 * It is often used when you need to print individual characters in the terminal without needing to specify a stream.
 *
 * Example usage:
 * ```c
 * fossil_io_putc('A');
 * ```
 *
 * @param c The character to be printed. This should be a single character.
 */
void fossil_io_putc(char c);

#ifdef __cplusplus
}
/**
 * C++ wrapper for the output functions.
 */
namespace fossil {
    /**
     * Namespace for input/output operations.
     */
    namespace io {
        /**
         * Class for output operations.
         */
        /**
         * Class for output operations.
         */
        class Output {
        public:
            /**
             * Prints a string to the output.
             *
             * @param str The string to be printed.
             */
            static void puts(const char *str) {
                fossil_io_puts(str);
            }

            /**
             * Prints a formatted string to the output.
             *
             * @param format The format string.
             * @param ... The additional arguments to be formatted.
             */
            static void printf(const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_printf(format, args);
                va_end(args);
            }

            /**
             * Prints a character to the output.
             *
             * @param c The character to be printed.
             */
            static void putchar(char c) {
                fossil_io_putchar(c);
            }

            /**
             * Prints a string to the specified output stream.
             *
             * @param stream The output stream where the string should be printed.
             * @param str The string to be printed.
             */
            static void fputs(fossil_fstream_t *stream, const char *str) {
                fossil_io_fputs(stream, str);
            }

            /**
             * Prints a formatted string to the specified output stream.
             *
             * @param stream The output stream where the formatted string should be printed.
             * @param format The format string.
             * @param ... The additional arguments to be formatted.
             */
            static void fprintf(fossil_fstream_t *stream, const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_fprintf(stream, format, args);
                va_end(args);
            }

            /**
             * Prints a character to the specified output stream.
             *
             * @param stream The output stream where the character should be printed.
             * @param c The character to be printed.
             */
            static void fputchar(fossil_fstream_t *stream, char c) {
                fossil_io_fputchar(stream, c);
            }

            /**
             * Prints a character to the standard output.
             *
             * @param c The character to be printed.
             */
            static void putc(char c) {
                fossil_io_putc(c);
            }
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
