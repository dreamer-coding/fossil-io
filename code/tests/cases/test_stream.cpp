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
FOSSIL_TEST_SUITE(cpp_stream_suite);
fossil_fstream_t cpp_stream;

// Setup function for the test suite
FOSSIL_SETUP(cpp_stream_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_stream_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));
    fossil_fstream_read(&cpp_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_let_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename1, "w"));
    fossil_fstream_close(&cpp_stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename2, "w"));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));

    // Seek to the end of the file
    fossil_fstream_seek(&cpp_stream, 0, SEEK_END);

    // Get the current position
    long position = fossil_fstream_tell(&cpp_stream);

    ASSUME_ITS_TRUE(position > 0);

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil_fstream_get_type(filename));  // Regular file
}

FOSSIL_TEST_CASE(cpp_test_stream_is_readable) {
    const char *filename = "testfile_readable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is readable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_readable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_is_writable) {
    const char *filename = "testfile_writable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is writable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_is_executable) {
    const char *filename = "testfile_executable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is executable
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_is_executable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_set_permissions) {
    const char *filename = "testfile_permissions.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename, 0644));

    // Check if the file is readable and writable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_readable(filename));
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_get_permissions) {
    const char *filename = "testfile_get_permissions.txt";
    int32_t mode;

    // // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename, 0644));

    // Get file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_get_permissions(filename, &mode));
}

FOSSIL_TEST_CASE(cpp_test_stream_get_modification_time) {
    const char *filename = "testfile_modification_time.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Get the modification time
    int64_t mod_time = fossil_fstream_get_modified_time(filename);
    ASSUME_ITS_TRUE(mod_time > 0);
    ASSUME_ITS_TRUE(mod_time < 10000000000);  // Check if the time is reasonable
}

FOSSIL_TEST_CASE(cpp_test_stream_get_creation_time) {
    const char *filename = "testfile_creation_time.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Get the creation time
    int64_t creation_time = fossil_fstream_get_creation_time(filename);
    ASSUME_ITS_TRUE(creation_time > 0);
}

FOSSIL_TEST_CASE(cpp_test_stream_flush_file) {
    const char *filename = "testfile_flush.txt";
    const char *content = "This is a test.";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);

    // Flush the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_flush(&cpp_stream));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_setpos_and_getpos) {
    const char *filename = "testfile_setpos_getpos.txt";
    const char *content = "This is a test.";
    int32_t pos;

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));

    // Set the file position
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_setpos(&cpp_stream, 5));

    // Get the file position
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_getpos(&cpp_stream, &pos));
    ASSUME_ITS_EQUAL_I32(5, pos);

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_truncate_file) {
    const char *filename = "testfile_truncate.txt";
    const char *content = "This is a test.";

    // Create the file and write content
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Truncate the file to a smaller size
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_truncate(filename, 4));

    // Read the truncated content
    char buffer[1024] = {0};
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));
    fossil_fstream_read(&cpp_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&cpp_stream);

    // Verify the truncated content
    ASSUME_ITS_EQUAL_CSTR("This", buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_lock_and_unlock_file) {
    const char *filename = "testfile_lock.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));

    // Lock the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_lock(&cpp_stream));

    // Unlock the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_unlock(&cpp_stream));

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_syncpp_file) {
    const char *filename = "testfile_sync.txt";
    const char *content = "This is a test.";

    // Create the file and write content
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);

    // Sync the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_sync(&cpp_stream));

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_set_buffering) {
    const char *filename = "testfile_buffering.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));

    // Enable buffering
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_buffering(&cpp_stream, 1));

    // Disable buffering
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_buffering(&cpp_stream, 0));

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_set_custom_buffer) {
    const char *filename = "testfile_custom_buffer.txt";
    char custom_buffer[1024];

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));

    // Set a custom buffer
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_buffer(&cpp_stream, custom_buffer, sizeof(custom_buffer)));

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_set_autosync) {
    const char *filename = "testfile_autosync.txt";
    const char *content = "This is a test.";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));

    // Enable auto-sync
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_autosync(&cpp_stream, 1));

    // Write content
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);

    // Disable auto-sync
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_autosync(&cpp_stream, 0));

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_remove_file) {
    const char *filename = "testfile_remove.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Remove the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_remove(filename));

    // Check if the file does not exist
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_file_exists(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_delete_file) {
    const char *filename = "testfile_delete.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Delete the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_delete(filename));

    // Check if the file does not exist
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_file_exists(filename));
}



// Test cases for the Stream class
FOSSIL_TEST_CASE(cpp_test_stream_class_open_and_close) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_open_close.txt";

    // Open the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));

    // Check if the file is open
    ASSUME_ITS_TRUE(stream.is_open());

    // Close the file
    stream.close();

    // Check if the file is closed
    ASSUME_ITS_FALSE(stream.is_open());
}

FOSSIL_TEST_CASE(cpp_test_stream_class_write_and_read) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_write_read.txt";
    const std::string content = "This is a test.";
    char buffer[1024] = {0};

    // Write data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));
    ASSUME_ITS_EQUAL_I32(content.size(), stream.write(content.c_str(), 1, content.size()));
    stream.close();

    // Read data from the file
    ASSUME_ITS_TRUE(stream.open(filename, "r"));
    ASSUME_ITS_EQUAL_I32(content.size(), stream.read(buffer, 1, content.size()));
    stream.close();

    // Verify the content
    ASSUME_ITS_EQUAL_CSTR(content.c_str(), buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_seek_and_tell) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_seek_tell.txt";
    const std::string content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));
    ASSUME_ITS_EQUAL_I32(content.size(), stream.write(content.c_str(), 1, content.size()));
    stream.close();

    // Open the file and seek
    ASSUME_ITS_TRUE(stream.open(filename, "r"));
    ASSUME_ITS_TRUE(stream.seek(5, SEEK_SET));

    // Get the current position
    ASSUME_ITS_EQUAL_I32(5, stream.tell());
    stream.close();
}

FOSSIL_TEST_CASE(cpp_test_stream_class_truncate) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_truncate.txt";
    const std::string content = "This is a test.";
    char buffer[1024] = {0};

    // Write data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));
    ASSUME_ITS_EQUAL_I32(content.size(), stream.write(content.c_str(), 1, content.size()));
    stream.close();

    // Truncate the file
    ASSUME_ITS_TRUE(stream.truncate(4));

    // Read the truncated content
    ASSUME_ITS_TRUE(stream.open(filename, "r"));
    ASSUME_ITS_EQUAL_I32(4, stream.read(buffer, 1, sizeof(buffer)));
    stream.close();

    // Verify the truncated content
    ASSUME_ITS_EQUAL_CSTR("This", buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_append_mode) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_append.txt";
    const std::string content1 = "This is a test.";
    const std::string content2 = " Appending data.";
    char buffer[1024] = {0};

    // Write data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));
    ASSUME_ITS_EQUAL_I32(content1.size(), stream.write(content1.c_str(), 1, content1.size()));
    stream.close();

    // Append data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "a"));
    ASSUME_ITS_EQUAL_I32(content2.size(), stream.write(content2.c_str(), 1, content2.size()));
    stream.close();

    // Read the appended content
    ASSUME_ITS_TRUE(stream.open(filename, "r"));
    ASSUME_ITS_EQUAL_I32(content1.size() + content2.size(), stream.read(buffer, 1, sizeof(buffer)));
    stream.close();

    // Verify the appended content
    ASSUME_ITS_EQUAL_CSTR("This is a test. Appending data.", buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_binary_mode) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_binary.txt";
    const char binary_data[] = {0x01, 0x02, 0x03, 0x04};
    char buffer[4] = {0};

    // Write binary data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "wb"));
    ASSUME_ITS_EQUAL_I32(sizeof(binary_data), stream.write(binary_data, 1, sizeof(binary_data)));
    stream.close();

    // Read binary data from the file
    ASSUME_ITS_TRUE(stream.open(filename, "rb"));
    ASSUME_ITS_EQUAL_I32(sizeof(binary_data), stream.read(buffer, 1, sizeof(buffer)));
    stream.close();
}

FOSSIL_TEST_CASE(cpp_test_stream_class_file_size) {
    fossil::io::Stream stream;
    const std::string filename = "testfile_class_size.txt";
    const std::string content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_TRUE(stream.open(filename, "w"));
    ASSUME_ITS_EQUAL_I32(content.size(), stream.write(content.c_str(), 1, content.size()));
    stream.close();
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_file_tests) {
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_write_and_read_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_open_and_close_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_multiple_files);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_seek_and_tell);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_type);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_readable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_writable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_executable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_modification_time);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_creation_time);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_flush_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_setpos_and_getpos);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_truncate_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_lock_and_unlock_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_syncpp_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_buffering);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_custom_buffer);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_autosync);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_remove_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_delete_file);

    // Test cases for the Stream class
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_open_and_close);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_write_and_read);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_seek_and_tell);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_append_mode);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_binary_mode);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_file_size);

    FOSSIL_TEST_REGISTER(cpp_stream_suite);
}
