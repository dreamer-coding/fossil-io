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

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Structure representing a file stream.
     */
    typedef struct
    {
        FILE *file;         // Pointer to the FILE structure for the stream
        char filename[500]; // Array to store the filename
    } fossil_fstream_t;

    extern fossil_fstream_t *FOSSIL_STDIN;
    extern fossil_fstream_t *FOSSIL_STDOUT;
    extern fossil_fstream_t *FOSSIL_STDERR;

#define FOSSIL_STDIN (FOSSIL_STDIN)
#define FOSSIL_STDOUT (FOSSIL_STDOUT)
#define FOSSIL_STDERR (FOSSIL_STDERR)

    /**
     * Reopen a stream with a new file.
     *
     * This function reopens a stream with a new file. It supports both Fossil Logic
     * file modes (e.g "read", "write", "append", etc.) and classic file modes (e.g.,
     * "r", "w", "a", "rb", "wb", etc.).
     *
     * @param stream   Pointer to the fossil_fstream_t structure to reopen.
     * @param filename The name of the file to reopen.
     * @param mode     The mode in which to reopen the file. Supports Fossil Logic
     *                 file modes and classic file modes.
     * @param file     Pointer to the FILE structure to reopen.
     * @return         0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file);

    /**
     * Open a stream for file operations.
     *
     * This function opens a file stream, allowing read or write operations on the specified file.
     * It supports both Fossil Logic file modes (e.g "read", "write", "append", etc.) and classic
     * file modes (e.g., "r", "w", "a", "rb", "wb", etc.).
     *
     * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
     * @param filename The name of the file to be opened.
     * @param mode     The mode in which to open the file. Supports Fossil Logic
     *                 file modes and classic file modes.
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

    /**
     * Get the last modified time of a file.
     *
     * This function retrieves the last modified time of a file.
     *
     * @param filename The name of the file.
     * @return         The last modified time as a UNIX timestamp, or -1 on failure.
     */
    int64_t fossil_fstream_get_modified_time(const char *filename);

    /**
     * Get the creation time of a file.
     *
     * This function retrieves the creation time of a file.
     *
     * @param filename The name of the file.
     * @return         The creation time as a UNIX timestamp, or -1 on failure.
     */
    int64_t fossil_fstream_get_creation_time(const char *filename);

    /**
     * Truncate a file to a specified size.
     *
     * This function truncates the file to a given size.
     *
     * @param filename The name of the file to truncate.
     * @param size     The size to truncate the file to.
     * @return         0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_truncate(const char *filename, int64_t size);

    /**
     * Lock a file for exclusive access.
     *
     * This function locks a file to prevent concurrent access.
     *
     * @param stream Pointer to the fossil_fstream_t structure representing the file.
     * @return       0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_lock(fossil_fstream_t *stream);

    /**
     * Unlock a previously locked file.
     *
     * This function unlocks a file that was locked for exclusive access.
     *
     * @param stream Pointer to the fossil_fstream_t structure representing the file.
     * @return       0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_unlock(fossil_fstream_t *stream);

    /**
     * Sync file contents to disk.
     *
     * This function forces a flush of file contents to disk for durability.
     *
     * @param stream Pointer to the fossil_fstream_t structure to sync.
     * @return       0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_sync(fossil_fstream_t *stream);

    /**
     * Enable or disable buffering for a stream.
     *
     * This function allows buffering to be enabled or disabled for an open stream.
     * Disabling buffering may be useful for real-time or low-latency requirements.
     *
     * @param stream    Pointer to the fossil_fstream_t structure.
     * @param enabled   1 to enable buffering, 0 to disable.
     * @return          0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_set_buffering(fossil_fstream_t *stream, int32_t enabled);

    /**
     * Set a custom buffer for a stream.
     *
     * This function sets a user-defined buffer for a stream. The buffer must remain valid
     * for the duration of the stream's use or until reset.
     *
     * @param stream    Pointer to the fossil_fstream_t structure.
     * @param buffer    Pointer to the buffer to use.
     * @param size      Size of the buffer in bytes.
     * @return          0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_set_buffer(fossil_fstream_t *stream, void *buffer, size_t size);

    /**
     * Enable auto-sync mode.
     *
     * This function configures a stream to automatically flush after each write operation,
     * ensuring all data is written to disk immediately.
     *
     * @param stream    Pointer to the fossil_fstream_t structure.
     * @param enabled   1 to enable auto-sync, 0 to disable.
     * @return          0 on success, non-zero on failure.
     */
    int32_t fossil_fstream_set_autosync(fossil_fstream_t *stream, int32_t enabled);

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
        private:
            fossil_fstream_t _stream; // Internal C stream structure
            std::string _filename;    // STL string for filename

        public:
            /**
             * Constructor: Initializes the stream and filename.
             */
            Stream() : _filename("") {
                _stream.file = nullptr;
            }

            /**
             * Destructor: Ensures the stream is closed if open.
             */
            ~Stream() {
                if (fossil_fstream_is_open(&_stream)) {
                    fossil_fstream_close(&_stream);
                }
            }

            /**
             * Open a file stream.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             * It supports both Fossil Logic file modes (e.g "read", "write", "append", etc.) and classic
             * file modes (e.g., "r", "w", "a", "rb", "wb", etc.).
             *
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file. Supports Fossil Logic
             *                 file modes and classic file modes.
             * @return         True if the file was opened successfully, false otherwise.
             */
            bool freopen(const std::string &filename, const std::string &mode) {
                return fossil_fstream_freopen(&_stream, filename.c_str(), mode.c_str(), _stream.file) == 0;
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             * It supports both Fossil Logic file modes (e.g "read", "write", "append", etc.) and classic
             * file modes (e.g., "r", "w", "a", "rb", "wb", etc.).
             *
             * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file. Supports Fossil Logic
             *                 file modes and classic file modes.
             * @return         0 on success, non-zero on failure.
             */
            bool open(const std::string &filename, const std::string &mode) {
                _filename = filename;
                return fossil_fstream_open(&_stream, _filename.c_str(), mode.c_str()) == 0;
            }

            /**
             * Close the file stream.
             *
             * This function closes the file stream and clears the associated filename.
             */
            void close() {
                fossil_fstream_close(&_stream);
                _filename.clear();
            }

            /**
             * Check if the file stream is open.
             *
             * @return True if the stream is open, false otherwise.
             */
            bool is_open() const {
                return fossil_fstream_is_open(&_stream) == 1;
            }

            /**
             * Read data from the file stream.
             *
             * @param buffer Pointer to the buffer to store the read data.
             * @param size   Size of each element to read.
             * @param count  Number of elements to read.
             * @return       The total number of elements successfully read.
             */
            size_t read(void *buffer, size_t size, size_t count) {
                return fossil_fstream_read(&_stream, buffer, size, count);
            }

            /**
             * Write data to the file stream.
             *
             * @param buffer Pointer to the buffer containing the data to write.
             * @param size   Size of each element to write.
             * @param count  Number of elements to write.
             * @return       The total number of elements successfully written.
             */
            size_t write(const void *buffer, size_t size, size_t count) {
                return fossil_fstream_write(&_stream, buffer, size, count);
            }

            /**
             * Append data to the end of the file stream.
             *
             * @param buffer Pointer to the buffer containing the data to append.
             * @param size   Size of each element to append.
             * @param count  Number of elements to append.
             * @return       0 on success, non-zero on failure.
             */
            int32_t append(const void *buffer, size_t size, int32_t count) {
                return fossil_fstream_append(&_stream, buffer, size, count);
            }

            /**
             * Seek to a specific position in the file stream.
             *
             * @param offset The offset from the specified origin.
             * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
             * @return       True if the seek operation was successful, false otherwise.
             */
            bool seek(int64_t offset, int32_t origin) {
                return fossil_fstream_seek(&_stream, offset, origin) == 0;
            }

            /**
             * Get the current position in the file stream.
             *
             * @return The current position of the file pointer.
             */
            int32_t tell() {
                return fossil_fstream_tell(const_cast<fossil_fstream_t*>(&_stream));
            }

            /**
             * Save the file stream to a new file.
             *
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            bool save(const std::string &new_filename) {
                return fossil_fstream_save(&_stream, new_filename.c_str()) == 0;
            }

            /**
             * Copy the file stream to a new file.
             *
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            bool copy(const std::string &destination_filename) {
                return fossil_fstream_copy(_filename.c_str(), destination_filename.c_str()) == 0;
            }

            /**
             * Remove the file stream.
             *
             * @return 0 on success, non-zero on failure.
             */
            bool remove() {
                return fossil_fstream_remove(_filename.c_str()) == 0;
            }

            /**
             * Rename the file stream.
             *
             * @param new_filename The new name for the file.
             * @return             0 on success, non-zero on failure.
             */
            bool rename(const std::string &new_filename) {
                return fossil_fstream_rename(_filename.c_str(), new_filename.c_str()) == 0;
            }

            /**
             * Flush the file stream.
             *
             * This function flushes the contents of the stream to ensure all data is written to disk.
             *
             * @return True if the flush operation was successful, false otherwise.
             */
            bool flush() {
                return fossil_fstream_flush(&_stream) == 0;
            }

            /**
             * Set the file position in the stream.
             *
             * @param pos The position to set in the stream.
             * @return    0 on success, non-zero on failure.
             */
            bool setpos(int32_t pos) {
                return fossil_fstream_setpos(&_stream, pos) == 0;
            }

            /**
             * Get the current position in the file stream.
             *
             * @param pos Pointer to store the current position of the file pointer.
             * @return    0 on success, non-zero on failure.
             */
            bool getpos(int32_t *pos) {
                return fossil_fstream_getpos(&_stream, pos) == 0;
            }

            /**
             * Rotate the file stream.
             *
             * @param n The number of rotations to perform.
             * @return 0 on success, non-zero on failure.
             */
            bool rotate(int32_t n) {
                return fossil_fstream_rotate(_filename.c_str(), n) == 0;
            }

            /**
             * Create a backup of the file stream.
             *
             * @param backup_suffix The suffix to be appended to the backup file.
             * @return             0 on success, non-zero on failure.
             */
            bool backup(const std::string &backup_suffix) {
                return fossil_fstream_backup(_filename.c_str(), backup_suffix.c_str()) == 0;
            }

            /**
             * Check if the file stream exists.
             *
             * @return 1 if the file exists, 0 otherwise.
             */
            bool file_exists() const {
                return fossil_fstream_file_exists(_filename.c_str()) == 1;
            }

            /**
             * Get the size of the file stream.
             *
             * @return The size of the file stream.
             */
            int32_t get_size() {
                return fossil_fstream_get_size(&_stream);
            }

            /**
             * Delete the file stream.
             *
             * @return 0 on success, non-zero on failure.
             */
            bool delete_file() {
                return fossil_fstream_delete(_filename.c_str()) == 0;
            }

            /**
             * Get the type of the file stream.
             *
             * @return The type of the file stream.
             */
            int get_type() const {
                return fossil_fstream_get_type(_filename.c_str());
            }

            /**
             * Check if the file stream is readable.
             *
             * @return 1 if the file is readable, 0 otherwise.
             */
            bool is_readable() const {
                return fossil_fstream_is_readable(_filename.c_str()) == 1;
            }

            /**
             * Check if the file stream is writable.
             *
             * @return 1 if the file is writable, 0 otherwise.
             */
            bool is_writable() const {
                return fossil_fstream_is_writable(_filename.c_str()) == 1;
            }

            /**
             * Check if the file stream is executable.
             *
             * @return 1 if the file is executable, 0 otherwise.
             */
            bool is_executable() const {
                return fossil_fstream_is_executable(_filename.c_str()) == 1;
            }

            /**
             * Set the file permissions for the file stream.
             *
             * @param mode The permissions to set (POSIX: chmod-style).
             * @return     0 on success, non-zero on failure.
             */
            bool set_permissions(int32_t mode) {
                return fossil_fstream_set_permissions(_filename.c_str(), mode) == 0;
            }

            /**
             * Get the file permissions for the file stream.
             *
             * @param mode Pointer to store the retrieved permissions (POSIX style).
             * @return     0 on success, non-zero on failure.
             */
            bool get_permissions(int32_t *mode) {
                return fossil_fstream_get_permissions(_filename.c_str(), mode) == 0;
            }

            /**
             * Get the last modified time of the file stream.
             *
             * @return The last modified time as a UNIX timestamp, or -1 on failure.
             */
            int64_t get_modified_time() const {
                return fossil_fstream_get_modified_time(_filename.c_str());
            }

            /**
             * Get the creation time of the file stream.
             *
             * @return The creation time as a UNIX timestamp, or -1 on failure.
             */
            int64_t get_creation_time() const {
                return fossil_fstream_get_creation_time(_filename.c_str());
            }

            /**
             * Truncate the file stream to a specified size.
             *
             * @param size The size to truncate the file to.
             * @return     0 on success, non-zero on failure.
             */
            bool truncate(int64_t size) {
                return fossil_fstream_truncate(_filename.c_str(), size) == 0;
            }

            /**
             * Lock the file stream for exclusive access.
             *
             * @return 0 on success, non-zero on failure.
             */
            bool lock() {
                return fossil_fstream_lock(&_stream) == 0;
            }

            /**
             * Unlock the previously locked file stream.
             *
             * @return 0 on success, non-zero on failure.
             */
            bool unlock() {
                return fossil_fstream_unlock(&_stream) == 0;
            }

            /**
             * Sync the file stream contents to disk.
             *
             * @return 0 on success, non-zero on failure.
             */
            bool sync() {
                return fossil_fstream_sync(&_stream) == 0;
            }

            /**
             * Enable or disable buffering for the file stream.
             *
             * @param enabled 1 to enable buffering, 0 to disable.
             * @return        0 on success, non-zero on failure.
             */
            bool set_buffering(int32_t enabled) {
                return fossil_fstream_set_buffering(&_stream, enabled) == 0;
            }

            /**
             * Set a custom buffer for the file stream.
             *
             * @param buffer Pointer to the buffer to use.
             * @param size   Size of the buffer in bytes.
             * @return       0 on success, non-zero on failure.
             */
            bool set_buffer(void *buffer, size_t size) {
                return fossil_fstream_set_buffer(&_stream, buffer, size) == 0;
            }

            /**
             * Enable or disable auto-sync mode for the file stream.
             *
             * @param enabled 1 to enable auto-sync, 0 to disable.
             * @return        0 on success, non-zero on failure.
             */
            bool set_autosync(int32_t enabled) {
                return fossil_fstream_set_autosync(&_stream, enabled) == 0;
            }

            /**
             * Get the internal C stream structure.
             *
             * @return Pointer to the internal C stream structure.
             */
            fossil_fstream_t *get_stream() {
                return &_stream;
            }

            /**
             * Get the filename associated with the file stream.
             *
             * @return The filename as a C-style string.
             */
            const char *get_filename_cstr() const {
                return _filename.c_str();
            }

            /**
             * Get the filename associated with the file stream.
             *
             * @return The filename as a string.
             */
            std::string get_filename() const {
                return _filename;
            }

        }; // class Stream

    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
