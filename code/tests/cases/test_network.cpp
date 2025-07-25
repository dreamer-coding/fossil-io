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
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(cpp_network_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_network_suite) {
    // Setup code (if needed)
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_network_suite) {
    // Teardown code (if needed)
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_nstream_create_and_destroy) {
    const char *protocols[] = {"tcp", "udp", "raw", "icmp", "sctp", "http", "https", "ftp", "ssh", "dns", "ntp", "smtp", "pop3", "imap", "ldap", "mqtt"};
    const char *clients[] = {"mail-server", "server", "mail-client", "client", "mail-bot", "bot", "multicast", "broadcast"};

    for (size_t i = 0; i < sizeof(protocols) / sizeof(protocols[0]); i++) {
        for (size_t j = 0; j < sizeof(clients) / sizeof(clients[0]); j++) {
            fossil_nstream_t *stream = fossil_nstream_create(protocols[i], clients[j]);
            ASSUME_NOT_CNULL(stream);
            fossil_nstream_destroy(stream);
        }
    }
}

FOSSIL_TEST(cpp_test_nstream_connect_invalid_host) {
    fossil_nstream_t *stream = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(stream);

    // Attempt to connect to an invalid host
    ASSUME_ITS_EQUAL_I32(-1, fossil_nstream_connect(stream, "invalid_host", 12345));
    fossil_nstream_destroy(stream);
}

#ifndef _WIN32
FOSSIL_TEST(cpp_test_nstream_listen_and_accept) {
    fossil_nstream_t *server = fossil_nstream_create("tcp", "server");
    ASSUME_NOT_CNULL(server);

    // Set SO_REUSEADDR to avoid bind fail on rapid test reruns
    fossil_nstream_set_reuseaddr(server, 1);

    // Use a unique port for this test to avoid bind conflicts
    const int test_port = 12346;

    // Start listening on a local port
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_listen(server, "127.0.0.1", test_port));

    // Simulate a client connecting
    fossil_nstream_t *client = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(client);
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_connect(client, "127.0.0.1", test_port));

    // Accept the client connection
    fossil_nstream_t *accepted_client = fossil_nstream_accept(server);
    ASSUME_NOT_CNULL(accepted_client);

    // Cleanup
    fossil_nstream_destroy(client);
    fossil_nstream_destroy(accepted_client);
    fossil_nstream_destroy(server);
}

FOSSIL_TEST(cpp_test_nstream_send_and_receive) {
    fossil_nstream_t *server = fossil_nstream_create("tcp", "server");
    ASSUME_NOT_CNULL(server);

    // Set SO_REUSEADDR to avoid bind fail on rapid test reruns
    fossil_nstream_set_reuseaddr(server, 1);

    // Use a unique port for this test to avoid bind conflicts
    const int test_port = 12347;

    // Start listening on a local port
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_listen(server, "127.0.0.1", test_port));

    // Simulate a client connecting
    fossil_nstream_t *client = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(client);
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_connect(client, "127.0.0.1", test_port));

    // Accept the client connection
    fossil_nstream_t *accepted_client = fossil_nstream_accept(server);
    ASSUME_NOT_CNULL(accepted_client);

    // Send and receive data
    const char *message = "Hello, Fossil!";
    ASSUME_ITS_EQUAL_I32(strlen(message), fossil_nstream_send(client, message, strlen(message)));

    char buffer[1024] = {0};
    ASSUME_ITS_EQUAL_I32(strlen(message), fossil_nstream_recv(accepted_client, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR(message, buffer);

    // Cleanup
    fossil_nstream_destroy(client);
    fossil_nstream_destroy(accepted_client);
    fossil_nstream_destroy(server);
}
#endif

FOSSIL_TEST(cpp_test_nstream_protocols) {
    const char *protocols[] = {"tcp", "udp", "raw", "icmp", "sctp", "http", "https", "ftp", "ssh", "dns", "ntp", "smtp", "pop3", "imap", "ldap", "mqtt"};

    for (size_t i = 0; i < sizeof(protocols) / sizeof(protocols[0]); i++) {
        fossil_nstream_t *stream = fossil_nstream_create(protocols[i], "client");
        ASSUME_NOT_CNULL(stream);
        fossil_nstream_destroy(stream);
    }
}

FOSSIL_TEST(cpp_test_nstream_client_types) {
    const char *clients[] = {"mail-server", "server", "mail-client", "client", "mail-bot", "bot", "multicast", "broadcast"};

    for (size_t i = 0; i < sizeof(clients) / sizeof(clients[0]); i++) {
        fossil_nstream_t *stream = fossil_nstream_create("tcp", clients[i]);
        ASSUME_NOT_CNULL(stream);
        fossil_nstream_destroy(stream);
    }
}

FOSSIL_TEST(cpp_test_nstream_class_create_and_destroy) {
    using namespace fossil::io;

    const std::string protocols[] = {"tcp", "udp", "raw", "icmp", "sctp", "http", "https", "ftp", "ssh", "dns", "ntp", "smtp", "pop3", "imap", "ldap", "mqtt"};
    const std::string clients[] = {"mail-server", "server", "mail-client", "client", "mail-bot", "bot", "multicast", "broadcast"};

    for (const auto &protocol : protocols) {
        for (const auto &client : clients) {
            NStream stream(protocol, client);
        }
    }
}

FOSSIL_TEST(cpp_test_nstream_class_connect_invalid_host) {
    using namespace fossil::io;

    try {
        NStream stream("tcp", "client");
        stream.connect("invalid_host", 12345);
        ASSUME_ITS_FALSE("Expected exception for invalid host");
    } catch (const std::runtime_error &e) {
        ASSUME_ITS_TRUE("Caught expected exception");
    }
}

#ifndef _WIN32
FOSSIL_TEST(cpp_test_nstream_class_listen_and_accept) {
    using namespace fossil::io;

    NStream server("tcp", "server");
    server.listen("127.0.0.1", 12345);

    NStream client("tcp", "client");
    client.connect("127.0.0.1", 12345);

    NStream *accepted_client = server.accept();
    ASSUME_NOT_CNULL(accepted_client);

    delete accepted_client;
}

FOSSIL_TEST(cpp_test_nstream_class_send_and_receive) {
    using namespace fossil::io;

    NStream server("tcp", "server");
    server.listen("127.0.0.1", 12345);

    NStream client("tcp", "client");
    client.connect("127.0.0.1", 12345);

    NStream *accepted_client = server.accept();
    ASSUME_NOT_CNULL(accepted_client);

    const std::string message = "Hello, Fossil!";
    client.send(message.c_str(), message.size());

    char buffer[1024] = {0};
    ssize_t bytes_received = accepted_client->recv(buffer, sizeof(buffer));
    ASSUME_ITS_EQUAL_I32(message.size(), bytes_received);
    ASSUME_ITS_EQUAL_CSTR(message.c_str(), buffer);

    delete accepted_client;
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_network_tests) {
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_create_and_destroy);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_connect_invalid_host);
    #ifndef _WIN32
        FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_listen_and_accept);
        FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_send_and_receive);
    #endif
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_protocols);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_client_types);

    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_create_and_destroy);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_connect_invalid_host);
    #ifndef _WIN32
        FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_listen_and_accept);
        FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_send_and_receive);
    #endif

    FOSSIL_TEST_REGISTER(cpp_network_suite);
}
