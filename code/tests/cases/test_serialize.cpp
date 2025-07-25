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
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(cpp_serialize_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_serialize_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_serialize_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_io_serialize_create) {
    fossil_io_serialize_buffer_t buf;
    int result = fossil_io_serialize_create(&buf, 1024);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_expand) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_expand(&buf, 512);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_i8) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_i8(&buf, 127);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_i16) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_i16(&buf, 32767);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_i32) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_i32(&buf, 2147483647);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_i64) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_i64(&buf, 9223372036854775807LL);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_u8) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_u8(&buf, 255);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_u16) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_u16(&buf, 65535);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_u32) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_u32(&buf, 4294967295U);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_u64) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_u64(&buf, 18446744073709551615ULL);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_cstr) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_cstr(&buf, "test string");
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_bool) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_serialize_bool(&buf, 1);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_i8) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_i8(&buf, 127);
    size_t offset = 0;
    int8_t value;
    int result = fossil_io_deserialize_i8(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(127, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_i16) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_i16(&buf, 32767);
    size_t offset = 0;
    int16_t value;
    int result = fossil_io_deserialize_i16(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(32767, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_i32) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_i32(&buf, 2147483647);
    size_t offset = 0;
    int32_t value;
    int result = fossil_io_deserialize_i32(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(2147483647, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_i64) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_i64(&buf, 9223372036854775807LL);
    size_t offset = 0;
    int64_t value;
    int result = fossil_io_deserialize_i64(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(9223372036854775807LL, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_u8) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_u8(&buf, 255);
    size_t offset = 0;
    uint8_t value;
    int result = fossil_io_deserialize_u8(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(255, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_u16) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_u16(&buf, 65535);
    size_t offset = 0;
    uint16_t value;
    int result = fossil_io_deserialize_u16(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(65535, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_u32) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_u32(&buf, 4294967295U);
    size_t offset = 0;
    uint32_t value;
    int result = fossil_io_deserialize_u32(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(4294967295U, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_u64) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_u64(&buf, 18446744073709551615ULL);
    size_t offset = 0;
    uint64_t value;
    int result = fossil_io_deserialize_u64(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(18446744073709551615ULL, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_cstr) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_cstr(&buf, "test string");
    size_t offset = 0;
    char out[20];
    int result = fossil_io_deserialize_cstr(&buf, &offset, out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_CSTR("test string", out);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_deserialize_bool) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_bool(&buf, 1);
    size_t offset = 0;
    int value;
    int result = fossil_io_deserialize_bool(&buf, &offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(1, value);
    fossil_io_serialize_destroy(&buf);
}

FOSSIL_TEST(cpp_test_io_serialize_to_file) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_cstr(&buf, "test string");
    int result = fossil_io_serialize_to_file(&buf, "test_file.bin");
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_serialize_destroy(&buf);
    remove("test_file.bin");
}

FOSSIL_TEST(cpp_test_io_deserialize_from_file) {
    fossil_io_serialize_buffer_t buf;
    fossil_io_serialize_create(&buf, 1024);
    fossil_io_serialize_cstr(&buf, "test string");
    fossil_io_serialize_to_file(&buf, "test_file.bin");
    fossil_io_serialize_destroy(&buf);

    fossil_io_serialize_create(&buf, 1024);
    int result = fossil_io_deserialize_from_file(&buf, "test_file.bin");
    ASSUME_ITS_EQUAL_I32(0, result);
    size_t offset = 0;
    char out[20];
    fossil_io_deserialize_cstr(&buf, &offset, out, sizeof(out));
    ASSUME_ITS_EQUAL_CSTR("test string", out);
    fossil_io_serialize_destroy(&buf);
    remove("test_file.bin");
}

FOSSIL_TEST(cpp_test_serialize_class_expand) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.expand(512);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_i8) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_i8(127);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_i16) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_i16(32767);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_i32) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_i32(2147483647);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_i64) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_i64(9223372036854775807LL);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_u8) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_u8(255);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_u16) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_u16(65535);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_u32) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_u32(4294967295U);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_u64) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_u64(18446744073709551615ULL);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_cstr) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_cstr("test string");
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_serialize_class_bool) {
    fossil::io::Serialize serializer(1024);
    int result = serializer.serialize_bool(1);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_deserialize_class_i8) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_i8(127);
    size_t offset = 0;
    int8_t value;
    int result = serializer.deserialize_i8(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(127, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_i16) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_i16(32767);
    size_t offset = 0;
    int16_t value;
    int result = serializer.deserialize_i16(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(32767, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_i32) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_i32(2147483647);
    size_t offset = 0;
    int32_t value;
    int result = serializer.deserialize_i32(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(2147483647, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_i64) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_i64(9223372036854775807LL);
    size_t offset = 0;
    int64_t value;
    int result = serializer.deserialize_i64(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(9223372036854775807LL, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_u8) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_u8(255);
    size_t offset = 0;
    uint8_t value;
    int result = serializer.deserialize_u8(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(255, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_u16) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_u16(65535);
    size_t offset = 0;
    uint16_t value;
    int result = serializer.deserialize_u16(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(65535, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_u32) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_u32(4294967295U);
    size_t offset = 0;
    uint32_t value;
    int result = serializer.deserialize_u32(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(4294967295U, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_u64) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_u64(18446744073709551615ULL);
    size_t offset = 0;
    uint64_t value;
    int result = serializer.deserialize_u64(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(18446744073709551615ULL, value);
}

FOSSIL_TEST(cpp_test_deserialize_class_cstr) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_cstr("test string");
    size_t offset = 0;
    char out[20];
    int result = serializer.deserialize_cstr(&offset, out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_CSTR("test string", out);
}

FOSSIL_TEST(cpp_test_deserialize_class_bool) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_bool(1);
    size_t offset = 0;
    int value;
    int result = serializer.deserialize_bool(&offset, &value);
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(1, value);
}

FOSSIL_TEST(cpp_test_serialize_class_to_file) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_cstr("test string");
    int result = serializer.serialize_to_file("test_file.bin");
    ASSUME_ITS_EQUAL_I32(0, result);
    remove("test_file.bin");
}

FOSSIL_TEST(cpp_test_deserialize_class_from_file) {
    fossil::io::Serialize serializer(1024);
    serializer.serialize_cstr("test string");
    serializer.serialize_to_file("test_file.bin");

    fossil::io::Serialize deserializer(1024);
    int result = deserializer.deserialize_from_file("test_file.bin");
    ASSUME_ITS_EQUAL_I32(0, result);
    size_t offset = 0;
    char out[20];
    deserializer.deserialize_cstr(&offset, out, sizeof(out));
    ASSUME_ITS_EQUAL_CSTR("test string", out);
    remove("test_file.bin");
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_serialize_tests) {
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_create);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_expand);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_i8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_i16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_i32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_i64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_u8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_u16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_u32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_u64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_cstr);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_bool);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_i8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_i16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_i32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_i64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_u8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_u16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_u32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_u64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_cstr);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_bool);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_serialize_to_file);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_io_deserialize_from_file);

    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_expand);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_i8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_i16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_i32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_i64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_u8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_u16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_u32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_u64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_cstr);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_bool);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_i8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_i16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_i32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_i64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_u8);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_u16);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_u32);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_u64);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_cstr);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_bool);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_serialize_class_to_file);
    FOSSIL_TEST_ADD(cpp_serialize_suite, cpp_test_deserialize_class_from_file);

    FOSSIL_TEST_REGISTER(cpp_serialize_suite);
}
