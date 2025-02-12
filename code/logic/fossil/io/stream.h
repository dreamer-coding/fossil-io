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
#ifndef FOSSIL_IO_STREAM_H
#define FOSSIL_IO_STREAM_H

#include <stdio.h>
#include <stdint.h>

#define FOSSIL_STDIN  stdin
#define FOSSIL_STDOUT stdout
#define FOSSIL_STDERR stderr

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Structure representing a file stream.
 */
typedef struct {
    FILE *file;                                       // Pointer to the FILE structure for the stream
    char filename[500]; // Array to store the filename
} fossil_fstream_t;

/**
 * Reopen a stream with a new file.
 *
 * This function reopens a stream with a new file.
 *
 * @param stream   Pointer to the fossil_fstream_t structure to reopen.
 * @param filename The name of the file to reopen.
 * @param mode     The mode in which to reopen the file.
 * @param file     Pointer to the FILE structure to reopen.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file);

/**
 * Open a stream for file operations.
 *
 * This function opens a file stream, allowing read or write operations on the specified file.
 *
 * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
 * @param filename The name of the file to be opened.
 * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_open(fossil_fstream_t *stream, const char *filename, const char *mode);

/**
 * Close an open stream.
 *
 * This function closes a previously opened stream, releasing associated resources.
 *
 * @param stream Pointer to the fossil_fstream_t structure to be closed.
 */
void fossil_fstream_close(fossil_fstream_t *stream);

/**
 * Check if a stream is open.
 *
 * This function checks if a stream is open.
 *
 * @param stream Pointer to the fossil_fstream_t structure to check.
 * @return       1 if the stream is open, 0 if not.
 */
int32_t fossil_fstream_is_open(const fossil_fstream_t *stream);

/**
 * Read data from an open stream.
 *
 * This function reads data from an open stream into a buffer.
 *
 * @param stream Pointer to the fossil_fstream_t structure from which to read.
 * @param buffer Pointer to the buffer to store the read data.
 * @param size   Size of each element to be read.
 * @param count  Number of elements to read.
 * @return       The total number of elements successfully read.
 */
size_t fossil_fstream_read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count);

/**
 * Write data to an open stream.
 *
 * This function writes data from a buffer to an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to which to write.
 * @param buffer Pointer to the buffer containing the data to be written.
 * @param size   Size of each element to be written.
 * @param count  Number of elements to write.
 * @return       The total number of elements successfully written.
 */
size_t fossil_fstream_write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count);

/**
 * Append data to the end of an open stream.
 *
 * This function appends data from a buffer to the end of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to which to append.
 * @param buffer Pointer to the buffer containing the data to be appended.
 * @param size   Size of each element to be appended.
 * @param count  Number of elements to append.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_append(fossil_fstream_t *stream, const void *buffer, size_t size, int32_t count);

/**
 * Seek to a specified position in an open stream.
 *
 * This function moves the file pointer associated with the stream to a new position.
 *
 * @param stream Pointer to the fossil_fstream_t structure to seek.
 * @param offset The offset from the specified origin.
 * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_seek(fossil_fstream_t *stream, int64_t offset, int32_t origin);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the position of.
 * @return       The current position of the file pointer.
 */
int32_t fossil_fstream_tell(fossil_fstream_t *stream);

/**
 * Save an open stream to a new file.
 *
 * This function saves the contents of an open stream to a new file.
 *
 * @param stream       Pointer to the fossil_fstream_t structure to be saved.
 * @param new_filename The name of the new file to save to.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_fstream_save(fossil_fstream_t *stream, const char *new_filename);

/**
 * Copy a file from the source to the destination.
 *
 * This function copies a file from a source file to a destination file.
 *
 * @param source_filename      The name of the source file.
 * @param destination_filename The name of the destination file.
 * @return                     0 on success, non-zero on failure.
 */
int32_t fossil_fstream_copy(const char *source_filename, const char *destination_filename);

/**
 * Remove a file stream.
 *
 * This function removes a file stream.
 *
 * @param filename The name of the file to remove.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_remove(const char *filename);

/**
 * Rename a file or directory.
 *
 * This function renames a file or directory.
 *
 * @param old_filename The current name of the file or directory.
 * @param new_filename The new name to assign to the file or directory.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_fstream_rename(const char *old_filename, const char *new_filename);

/**
 * Flush the contents of an open stream.
 *
 * This function flushes the contents of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to flush.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_flush(fossil_fstream_t *stream);

/**
 * Seek to the beginning of an open stream.
 *
 * This function moves the file pointer associated with the stream to the beginning.
 *
 * @param stream Pointer to the fossil_fstream_t structure to seek.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_setpos(fossil_fstream_t *stream, int32_t pos);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the position of.
 * @param pos    Pointer to store the current position of the file pointer.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_getpos(fossil_fstream_t *stream, int32_t *pos);

/**
 * Rotate a file stream.
 *
 * This function rotates a file stream.
 * 
 * @param filename The name of the file to rotate.
 * @param n        The number of rotations to perform.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_rotate(const char *filename, int32_t n);

/**
 * Create a backup of a file with a specified backup suffix.
 *
 * This function creates a backup of a file with the given suffix.
 *
 * @param filename      The name of the file to create a backup for.
 * @param backup_suffix The suffix to be appended to the backup file.
 * @return              0 on success, non-zero on failure.
 */
int32_t fossil_fstream_backup(const char *filename, const char *backup_suffix);

/**
 * Check if a file exists.
 *
 * This function checks if a file exists.
 *
 * @param filename The name of the file to check for existence.
 * @return         1 if the file exists, 0 if not.
 */
int32_t fossil_fstream_file_exists(const char *filename);

/**
 * Get the size of an open stream.
 *
 * This function retrieves the size of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the size of.
 * @return       The size of the open stream.
 */
int32_t fossil_fstream_get_size(fossil_fstream_t *stream);

/**
 * Delete a file.
 *
 * This function deletes a file.
 *
 * @param filename The name of the file to be deleted.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_delete(const char *filename);

/**
 * Get the type of a file stream.
 *
 * This function retrieves the type of a file stream.
 *
 * @param filename The name of the file to get the type of.
 * @return         The type of the file stream.
 */
int fossil_fstream_get_type(const char *filename);

/**
 * Check if a file is readable.
 *
 * This function checks if a file has read permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if readable, 0 otherwise.
 */
int32_t fossil_fstream_is_readable(const char *filename);

/**
 * Check if a file is writable.
 *
 * This function checks if a file has write permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if writable, 0 otherwise.
 */
int32_t fossil_fstream_is_writable(const char *filename);

/**
 * Check if a file is executable.
 *
 * This function checks if a file has execute permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if executable, 0 otherwise.
 */
int32_t fossil_fstream_is_executable(const char *filename);

/**
 * Set file permissions.
 *
 * This function sets the permissions for a file.
 *
 * @param filename The name of the file to set permissions for.
 * @param mode     The permissions to set (POSIX: chmod-style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_set_permissions(const char *filename, int32_t mode);

/**
 * Get file permissions.
 *
 * This function retrieves the permissions of a file.
 *
 * @param filename The name of the file to retrieve permissions for.
 * @param mode     Pointer to store the retrieved permissions (POSIX style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_get_permissions(const char *filename, int32_t *mode);

#ifdef __cplusplus
}
#include <string>

/**
 * C++ API namespace for Fossil Logic source code.
 */
namespace fossil {
    /**
     * IO namespace for io related code.
     */
    namespace io {

        /**
         * Class for file stream functions.
         */
        class Stream {
        public:

            /**
             * Reopen a stream with a new file.
             *
             * This function reopens a stream with a new file.
             *
             * @param stream   Pointer to the fossil_fstream_t structure to reopen.
             * @param filename The name of the file to reopen.
             * @param mode     The mode in which to reopen the file.
             * @param file     Pointer to the FILE structure to reopen.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file) {
                return fossil_fstream_freopen(stream, filename, mode, file);
            }

            /**
             * Reopen a stream with a new file.
             *
             * This function reopens a stream with a new file.
             *
             * @param stream   Pointer to the fossil_fstream_t structure to reopen.
             * @param filename The name of the file to reopen.
             * @param mode     The mode in which to reopen the file.
             * @param file     Pointer to the FILE structure to reopen.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t freopen(fossil_fstream_t *stream, const std::string &filename, const std::string &mode, FILE *file) {
                return fossil_fstream_freopen(stream, filename.c_str(), mode.c_str(), file);
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             *
             * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t open(fossil_fstream_t *stream, const char *filename, const char *mode) {
                return fossil_fstream_open(stream, filename, mode);
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             *
             * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t open(fossil_fstream_t *stream, const std::string &filename, const std::string &mode) {
                return fossil_fstream_open(stream, filename.c_str(), mode.c_str());
            }

            /**
             * Close an open stream.
             *
             * This function closes a previously opened stream, releasing associated resources.
             *
             * @param stream Pointer to the fossil_fstream_t structure to be closed.
             */
            static void close(fossil_fstream_t *stream) {
                fossil_fstream_close(stream);
            }

            /**
             * Check if a stream is open.
             *
             * This function checks if a stream is open.
             *
             * @param stream Pointer to the fossil_fstream_t structure to check.
             * @return       1 if the stream is open, 0 if not.
             */
            static int32_t is_open(const fossil_fstream_t *stream) {
                return fossil_fstream_is_open(stream);
            }

            /**
             * Read data from an open stream.
             *
             * This function reads data from an open stream into a buffer.
             *
             * @param stream Pointer to the fossil_fstream_t structure from which to read.
             * @param buffer Pointer to the buffer to store the read data.
             * @param size   Size of each element to be read.
             * @param count  Number of elements to read.
             * @return       The total number of elements successfully read.
             */
            static size_t read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count) {
                return fossil_fstream_read(stream, buffer, size, count);
            }

            /**
             * Write data to an open stream.
             *
             * This function writes data from a buffer to an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to which to write.
             * @param buffer Pointer to the buffer containing the data to be written.
             * @param size   Size of each element to be written.
             * @param count  Number of elements to write.
             * @return       The total number of elements successfully written.
             */
            static size_t write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count) {
                return fossil_fstream_write(stream, buffer, size, count);
            }

            /**
             * Append data to the end of an open stream.
             *
             * This function appends data from a buffer to the end of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to which to append.
             * @param buffer Pointer to the buffer containing the data to be appended.
             * @param size   Size of each element to be appended.
             * @param count  Number of elements to append.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t append(fossil_fstream_t *stream, const void *buffer, size_t size, int32_t count) {
                return fossil_fstream_append(stream, buffer, size, count);
            }

            /**
             * Seek to a specified position in an open stream.
             *
             * This function moves the file pointer associated with the stream to a new position.
             *
             * @param stream Pointer to the fossil_fstream_t structure to seek.
             * @param offset The offset from the specified origin.
             * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
             * @return       0 on success, non-zero on failure.
             */
            static int32_t seek(fossil_fstream_t *stream, int64_t offset, int32_t origin) {
                return fossil_fstream_seek(stream, offset, origin);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the position of.
             * @return       The current position of the file pointer.
             */
            static int32_t tell(fossil_fstream_t *stream) {
                return fossil_fstream_tell(stream);
            }

            /**
             * Save an open stream to a new file.
             *
             * This function saves the contents of an open stream to a new file.
             *
             * @param stream       Pointer to the fossil_fstream_t structure to be saved.
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t save(fossil_fstream_t *stream, const char *new_filename) {
                return fossil_fstream_save(stream, new_filename);
            }

            /**
             * Save an open stream to a new file.
             *
             * This function saves the contents of an open stream to a new file.
             *
             * @param stream       Pointer to the fossil_fstream_t structure to be saved.
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t save(fossil_fstream_t *stream, const std::string &new_filename) {
                return fossil_fstream_save(stream, new_filename.c_str());
            }

            /**
             * Copy a file from the source to the destination.
             *
             * This function copies a file from a source file to a destination file.
             *
             * @param source_filename      The name of the source file.
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            static int32_t copy(const char *source_filename, const char *destination_filename) {
                return fossil_fstream_copy(source_filename, destination_filename);
            }

            /**
             * Copy a file from the source to the destination.
             *
             * This function copies a file from a source file to a destination file.
             *
             * @param source_filename      The name of the source file.
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            static int32_t copy(const std::string &source_filename, const std::string &destination_filename) {
                return fossil_fstream_copy(source_filename.c_str(), destination_filename.c_str());
            }

            /**
             * Remove a file stream.
             *
             * This function removes a file stream.
             *
             * @param filename The name of the file to remove.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t remove(const char *filename) {
                return fossil_fstream_remove(filename);
            }

            /**
             * Remove a file stream.
             *
             * This function removes a file stream.
             *
             * @param filename The name of the file to remove.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t remove(const std::string &filename) {
                return fossil_fstream_remove(filename.c_str());
            }

            /**
             * Rename a file or directory.
             *
             * This function renames a file or directory.
             *
             * @param old_filename The current name of the file or directory.
             * @param new_filename The new name to assign to the file or directory.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t rename(const char *old_filename, const char *new_filename) {
                return fossil_fstream_rename(old_filename, new_filename);
            }

            /**
             * Rename a file or directory.
             *
             * This function renames a file or directory.
             *
             * @param old_filename The current name of the file or directory.
             * @param new_filename The new name to assign to the file or directory.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t rename(const std::string &old_filename, const std::string &new_filename) {
                return fossil_fstream_rename(old_filename.c_str(), new_filename.c_str());
            }

            /**
             * Flush the contents of an open stream.
             *
             * This function flushes the contents of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to flush.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t flush(fossil_fstream_t *stream) {
                return fossil_fstream_flush(stream);
            }

            /**
             * Seek to the beginning of an open stream.
             *
             * This function moves the file pointer associated with the stream to the beginning.
             *
             * @param stream Pointer to the fossil_fstream_t structure to seek.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t setpos(fossil_fstream_t *stream, int32_t pos) {
                return fossil_fstream_setpos(stream, pos);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the position of.
             * @param pos    Pointer to store the current position of the file pointer.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t getpos(fossil_fstream_t *stream, int32_t *pos) {
                return fossil_fstream_getpos(stream, pos);
            }

            /**
             * Rotate a file stream.
             *
             * This function rotates a file stream.
             * 
             * @param filename The name of the file to rotate.
             * @param n        The number of rotations to perform.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t rotate(const char *filename, int32_t n) {
                return fossil_fstream_rotate(filename, n);
            }

            /**
             * Rotate a file stream.
             *
             * This function rotates a file stream.
             * 
             * @param filename The name of the file to rotate.
             * @param n        The number of rotations to perform.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t rotate(const std::string &filename, int32_t n) {
                return fossil_fstream_rotate(filename.c_str(), n);
            }

            /**
             * Create a backup of a file with a specified backup suffix.
             *
             * This function creates a backup of a file with the given suffix.
             *
             * @param filename      The name of the file to create a backup for.
             * @param backup_suffix The suffix to be appended to the backup file.
             * @return              0 on success, non-zero on failure.
             */
            static int32_t backup(const char *filename, const char *backup_suffix) {
                return fossil_fstream_backup(filename, backup_suffix);
            }

            /**
             * Create a backup of a file with a specified backup suffix.
             *
             * This function creates a backup of a file with the given suffix.
             *
             * @param filename      The name of the file to create a backup for.
             * @param backup_suffix The suffix to be appended to the backup file.
             * @return              0 on success, non-zero on failure.
             */
            static int32_t backup(const std::string &filename, const std::string &backup_suffix) {
                return fossil_fstream_backup(filename.c_str(), backup_suffix.c_str());
            }

            /**
             * Check if a file exists.
             *
             * This function checks if a file exists.
             *
             * @param filename The name of the file to check for existence.
             * @return         1 if the file exists, 0 if not.
             */
            static int32_t file_exists(const char *filename) {
                return fossil_fstream_file_exists(filename);
            }

            /**
             * Check if a file exists.
             *
             * This function checks if a file exists.
             *
             * @param filename The name of the file to check for existence.
             * @return         1 if the file exists, 0 if not.
             */
            static int32_t file_exists(const std::string &filename) {
                return fossil_fstream_file_exists(filename.c_str());
            }

            /**
             * Get the size of an open stream.
             *
             * This function retrieves the size of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the size of.
             * @return       The size of the open stream.
             */
            static int32_t get_size(fossil_fstream_t *stream) {
                return fossil_fstream_get_size(stream);
            }

            /**
             * Delete a file.
             *
             * This function deletes a file.
             *
             * @param filename The name of the file to be deleted.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t delete_file(const char *filename) {
                return fossil_fstream_delete(filename);
            }

            /**
             * Delete a file.
             *
             * This function deletes a file.
             *
             * @param filename The name of the file to be deleted.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t delete_file(const std::string &filename) {
                return fossil_fstream_delete(filename.c_str());
            }

            /**
             * Get the type of a file stream.
             *
             * This function retrieves the type of a file stream.
             *
             * @param filename The name of the file to get the type of.
             * @return         The type of the file stream.
             */
            static int get_type(const char *filename) {
                return fossil_fstream_get_type(filename);
            }

            /**
             * Get the type of a file stream.
             *
             * This function retrieves the type of a file stream.
             *
             * @param filename The name of the file to get the type of.
             * @return         The type of the file stream.
             */
            static int get_type(const std::string &filename) {
                return fossil_fstream_get_type(filename.c_str());
            }

            /**
             * Check if a file is readable.
             *
             * This function checks if a file has read permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if readable, 0 otherwise.
             */
            static int32_t is_readable(const char *filename) {
                return fossil_fstream_is_readable(filename);
            }

            /**
             * Check if a file is readable.
             *
             * This function checks if a file has read permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if readable, 0 otherwise.
             */
            static int32_t is_readable(const std::string &filename) {
                return fossil_fstream_is_readable(filename.c_str());
            }

            /**
             * Check if a file is writable.
             *
             * This function checks if a file has write permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if writable, 0 otherwise.
             */
            static int32_t is_writable(const char *filename) {
                return fossil_fstream_is_writable(filename);
            }

            /**
             * Check if a file is writable.
             *
             * This function checks if a file has write permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if writable, 0 otherwise.
             */
            static int32_t is_writable(const std::string &filename) {
                return fossil_fstream_is_writable(filename.c_str());
            }

            /**
             * Check if a file is executable.
             *
             * This function checks if a file has execute permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if executable, 0 otherwise.
             */
            static int32_t is_executable(const char *filename) {
                return fossil_fstream_is_executable(filename);
            }

            /**
             * Check if a file is executable.
             *
             * This function checks if a file has execute permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if executable, 0 otherwise.
             */
            static int32_t is_executable(const std::string &filename) {
                return fossil_fstream_is_executable(filename.c_str());
            }

            /**
             * Set file permissions.
             *
             * This function sets the permissions for a file.
             *
             * @param filename The name of the file to set permissions for.
             * @param mode     The permissions to set (POSIX: chmod-style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t set_permissions(const char *filename, int32_t mode) {
                return fossil_fstream_set_permissions(filename, mode);
            }

            /**
             * Set file permissions.
             *
             * This function sets the permissions for a file.
             *
             * @param filename The name of the file to set permissions for.
             * @param mode     The permissions to set (POSIX: chmod-style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t set_permissions(const std::string &filename, int32_t mode) {
                return fossil_fstream_set_permissions(filename.c_str(), mode);
            }

            /**
             * Get file permissions.
             *
             * This function retrieves the permissions of a file.
             *
             * @param filename The name of the file to retrieve permissions for.
             * @param mode     Pointer to store the retrieved permissions (POSIX style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t get_permissions(const char *filename, int32_t *mode) {
                return fossil_fstream_get_permissions(filename, mode);
            }

            /**
             * Get file permissions.
             *
             * This function retrieves the permissions of a file.
             *
             * @param filename The name of the file to retrieve permissions for.
             * @param mode     Pointer to store the retrieved permissions (POSIX style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t get_permissions(const std::string &filename, int32_t *mode) {
                return fossil_fstream_get_permissions(filename.c_str(), mode);
            }

        };

    } // namespace io

} // namespace fossil

#endif


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
    #include <Python.h>
#elif defined(__APPLE__)
    #include <Python.h>
#else
    #if __has_include(<python3.13/Python.h>)
        #include <python3.13/Python.h>
    #elif __has_include(<python3.12/Python.h>)
        #include <python3.12/Python.h>
    #elif __has_include(<python3.11/Python.h>)
        #include <python3.11/Python.h>
    #elif __has_include(<python3.10/Python.h>)
        #include <python3.10/Python.h>
    #elif __has_include(<python3.9/Python.h>)
        #include <python3.9/Python.h>
    #elif __has_include(<python3.8/Python.h>)
        #include <python3.8/Python.h>
    #else
        #error "Python 3.8 or later is required!"
    #endif
#endif


/**
 * Python wrapper for fossil_fstream_t structure.
 * This structure is used to create a Python object that wraps the fossil_fstream_t structure,
 * allowing it to be used in Python code.
 */
typedef struct {
    PyObject_HEAD
    fossil_fstream_t *stream;  // Pointer to the fossil_fstream_t structure
} PyFossilStream;

/**
 * Initialize a new PyFossilStream object.
 * This function is called when a new PyFossilStream object is created.
 *
 * @param self  Pointer to the PyFossilStream object to initialize.
 * @param args  Tuple of arguments passed to the constructor.
 * @param kwds  Dictionary of keyword arguments passed to the constructor.
 * @return      0 on success, -1 on failure.
 */
static int PyFossilStream_init(PyFossilStream *self, PyObject *args, PyObject *kwds) {
    self->stream = (fossil_fstream_t *)malloc(sizeof(fossil_fstream_t));
    if (!self->stream) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate fossil_fstream_t");
        return -1;
    }
    return 0;
}

/**
 * Deallocate a PyFossilStream object.
 * This function is called when a PyFossilStream object is deallocated.
 *
 * @param self  Pointer to the PyFossilStream object to deallocate.
 */
static void PyFossilStream_dealloc(PyFossilStream *self) {
    if (self->stream) {
        free(self->stream);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/**
 * Open a file stream.
 * This function is called from Python to open a file stream.
 *
 * @param self  Pointer to the PyFossilStream object.
 * @param args  Tuple of arguments passed from Python (filename and mode).
 * @return      PyLong object representing the result of the operation.
 */
static PyObject *PyFossilStream_open(PyFossilStream *self, PyObject *args) {
    const char *filename;
    const char *mode;
    if (!PyArg_ParseTuple(args, "ss", &filename, &mode)) {
        return NULL;
    }
    int32_t result = fossil_fstream_open(self->stream, filename, mode);
    return PyLong_FromLong(result);
}

/**
 * Close a file stream.
 * This function is called from Python to close a file stream.
 *
 * @param self  Pointer to the PyFossilStream object.
 * @return      Py_None.
 */
static PyObject *PyFossilStream_close(PyFossilStream *self) {
    fossil_fstream_close(self->stream);
    Py_RETURN_NONE;
}

/**
 * Read from a file stream.
 * This function is called from Python to read data from a file stream.
 *
 * @param self  Pointer to the PyFossilStream object.
 * @param args  Tuple of arguments passed from Python (size and count).
 * @return      PyBytes object containing the read data.
 */
static PyObject *PyFossilStream_read(PyFossilStream *self, PyObject *args) {
    size_t size, count;
    if (!PyArg_ParseTuple(args, "kk", &size, &count)) {
        return NULL;
    }
    void *buffer = malloc(size * count);
    if (!buffer) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate buffer");
        return NULL;
    }
    size_t result = fossil_fstream_read(self->stream, buffer, size, count);
    PyObject *py_result = PyBytes_FromStringAndSize((const char *)buffer, result * size);
    free(buffer);
    return py_result;
}

/**
 * Write to a file stream.
 * This function is called from Python to write data to a file stream.
 *
 * @param self  Pointer to the PyFossilStream object.
 * @param args  Tuple of arguments passed from Python (buffer, size, and count).
 * @return      PyLong object representing the result of the operation.
 */
static PyObject *PyFossilStream_write(PyFossilStream *self, PyObject *args) {
    const char *buffer;
    size_t size, count;
    if (!PyArg_ParseTuple(args, "y#kk", &buffer, &size, &count)) {
        return NULL;
    }
    size_t result = fossil_fstream_write(self->stream, buffer, size, count);
    return PyLong_FromLong(result);
}

/**
 * Define the methods of the PyFossilStream type.
 * This array contains the methods that can be called on a PyFossilStream object from Python.
 */
static PyMethodDef PyFossilStream_methods[] = {
    {"open", (PyCFunction)PyFossilStream_open, METH_VARARGS, "Open a file stream"},
    {"close", (PyCFunction)PyFossilStream_close, METH_NOARGS, "Close a file stream"},
    {"read", (PyCFunction)PyFossilStream_read, METH_VARARGS, "Read from a file stream"},
    {"write", (PyCFunction)PyFossilStream_write, METH_VARARGS, "Write to a file stream"},
    {NULL, NULL, 0, NULL}
};

/**
 * Define the PyFossilStream type.
 * This structure defines the PyFossilStream type, including its name, size, methods, and other properties.
 */
static PyTypeObject PyFossilStreamType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fossil.PyFossilStream",
    .tp_basicsize = sizeof(PyFossilStream),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)PyFossilStream_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Fossil file stream",
    .tp_methods = PyFossilStream_methods,
    .tp_init = (initproc)PyFossilStream_init,
    .tp_new = PyType_GenericNew,
};

/**
 * Define the module methods.
 * This array contains the methods that can be called on the module itself from Python.
 */
static PyMethodDef module_methods[] = {
    {NULL, NULL, 0, NULL}
};

/**
 * Define the module.
 * This structure defines the module, including its name, documentation, and methods.
 */
static struct PyModuleDef fossilmodule = {
    PyModuleDef_HEAD_INIT,
    "fossil",
    "Fossil file stream module",
    -1,
    module_methods
};

/**
 * Initialize the module.
 * This function is called when the module is imported in Python.
 *
 * @return  Pointer to the module object, or NULL on failure.
 */
PyMODINIT_FUNC PyInit_fossil(void) {
    PyObject *m;
    if (PyType_Ready(&PyFossilStreamType) < 0) {
        return NULL;
    }
    m = PyModule_Create(&fossilmodule);
    if (m == NULL) {
        return NULL;
    }
    Py_INCREF(&PyFossilStreamType);
    PyModule_AddObject(m, "PyFossilStream", (PyObject *)&PyFossilStreamType);
    return m;
}

#ifdef __cplusplus
}
#endif



#endif /* FOSSIL_IO_FRAMEWORK_H */
