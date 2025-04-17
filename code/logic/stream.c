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
#include "fossil/io/stream.h"
#include "fossil/io/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

typedef enum {
    FOSSIL_BUFFER_SMALL  = 100,
    FOSSIL_BUFFER_MEDIUM = 500,
    FOSSIL_BUFFER_LARGE  = 1000,
    FOSSIL_BUFFER_HUGE   = 5000,
    FOSSIL_BUFFER_GIANT  = 10000
} fossil_limit_t;

typedef struct {
    const char *keyword;
    const char *mode;
} fossil_fstream_mode_entry_t;

static const fossil_fstream_mode_entry_t fossil_fstream_mode_table[] = {
    // Classic C modes (standard fopen strings)
    { "r",     "r"   }, { "rb",    "rb"  },
    { "w",     "w"   }, { "wb",    "wb"  },
    { "a",     "a"   }, { "ab",    "ab"  },
    { "r+",    "r+"  }, { "rb+",   "r+b" }, { "r+b", "r+b" },
    { "w+",    "w+"  }, { "wb+",   "w+b" }, { "w+b", "w+b" },
    { "a+",    "a+"  }, { "ab+",   "a+b" }, { "a+b", "a+b" },

    // Extended readable modes
    { "read",          "r"   },
    { "readb",         "rb"  },
    { "write",         "w"   },
    { "writeb",        "wb"  },
    { "append",        "a"   },
    { "appendb",       "ab"  },
    { "read+write",    "r+"  },
    { "read+writeb",   "r+b" },
    { "write+read",    "w+"  },
    { "write+readb",   "w+b" },
    { "append+read",   "a+"  },
    { "append+readb",  "a+b" },
    { "read+t",        "rt"  },
    { "write+t",       "wt"  },
    { "read+write+t",  "r+t" },

    // Optional end-of-table sentinel
    { NULL, NULL }
};

static const char *fossil_fstream_mode_from_keyword(const char *keyword) {
    if (keyword == NULL) return NULL;
    for (int i = 0; fossil_fstream_mode_table[i].keyword != NULL; i++) {
        if (strcmp(keyword, fossil_fstream_mode_table[i].keyword) == 0) {
            return fossil_fstream_mode_table[i].mode;
        }
    }
    return NULL;
}

// Open a stream for file operations
int32_t fossil_fstream_open(fossil_fstream_t *stream, const char *filename, const char *mode) {
    if (stream == NULL || filename == NULL || mode == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (strlen(filename) >= FOSSIL_BUFFER_MEDIUM) {
        fprintf(stderr, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    stream->file = fopen(filename, fossil_fstream_mode_from_keyword(mode));
    if (stream->file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    strncpy(stream->filename, filename, FOSSIL_BUFFER_MEDIUM);

    return FOSSIL_ERROR_OK;
}

// Close an open stream
void fossil_fstream_close(fossil_fstream_t *stream) {
    if (stream != NULL && stream->file != NULL) {
        if (fclose(stream->file) != 0) {
            fprintf(stderr, "Error: Failed to close file - %s\n", stream->filename);
        }
        stream->file = NULL;
    }
}

int32_t fossil_fstream_freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file) {
    if (stream == NULL || filename == NULL || mode == NULL || file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    FILE *new_file = freopen(filename, fossil_fstream_mode_from_keyword(mode), file);
    if (new_file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    stream->file = new_file;
    strncpy(stream->filename, filename, FOSSIL_BUFFER_MEDIUM);

    return FOSSIL_ERROR_OK;
}

// Read data from an open stream
size_t fossil_fstream_read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    size_t bytes_read = fread(buffer, size, count, stream->file);

    if (bytes_read == 0 && ferror(stream->file)) {
        fprintf(stderr, "Error: File corruption\n");
        return FOSSIL_ERROR_FILE_CORRUPTION;
    }

    return bytes_read;
}

// Write data to an open stream
size_t fossil_fstream_write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    size_t bytes_written = fwrite(buffer, size, count, stream->file);

    if (bytes_written == 0 && ferror(stream->file)) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return bytes_written;
}

// Append data to the end of an open stream
int32_t fossil_fstream_append(fossil_fstream_t *stream, const void * restrict buffer, size_t size, int32_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    fseek(stream->file, 0, SEEK_END);
    int32_t result = fwrite(buffer, size, count, stream->file);

    if (result != count) {
        fprintf(stderr, "Error: File not found\n");
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return FOSSIL_ERROR_OK;
}

// Seek to a specified position in an open stream
int32_t fossil_fstream_seek(fossil_fstream_t *stream, int64_t offset, int32_t origin) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    int32_t result = fseek(stream->file, offset, origin);

    if (result != 0) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return FOSSIL_ERROR_OK;
}

// Get the current position of the file pointer in an open stream
int32_t fossil_fstream_tell(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    long position = ftell(stream->file);

    if (position == -1L && ferror(stream->file)) {
        fprintf(stderr, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }

    return (int32_t)position;
}

// Save an open stream to a new file
int32_t fossil_fstream_save(fossil_fstream_t *stream, const char *new_filename) {
    if (stream == NULL || stream->file == NULL || new_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (strlen(new_filename) >= FOSSIL_BUFFER_MEDIUM) {
        fprintf(stderr, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    fclose(stream->file);

    if (rename(stream->filename, new_filename) != 0) {
        fprintf(stderr, "Error: Failed to save %s\n", new_filename);
        return FOSSIL_ERROR_NULL_POINTER;
    }

    // Reopen the file with the new name
    int32_t result = fossil_fstream_open(stream, new_filename, "r");

    if (result != FOSSIL_ERROR_OK) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }
    return result;
}

// Copy a file from the source to the destination
int32_t fossil_fstream_copy(const char *source_filename, const char *destination_filename) {
    if (source_filename == NULL || destination_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", source_filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    FILE *destination_file = fopen(destination_filename, "wb");
    if (destination_file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", destination_filename);
        fclose(source_file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    char buffer[FOSSIL_BUFFER_MEDIUM];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, FOSSIL_BUFFER_MEDIUM, source_file)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, destination_file);
        if (bytesWritten != bytesRead) {
            fprintf(stderr, "Error: File not found\n");
            fclose(source_file);
            fclose(destination_file);
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        }
    }

    fclose(source_file);
    fclose(destination_file);

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_remove(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (remove(filename) == 0) {
        return FOSSIL_ERROR_OK;  // File removed successfully
    }

    fprintf(stderr, "Error: IO error when removing file %s\n", filename);
    return FOSSIL_ERROR_IO;
}

int32_t fossil_fstream_rename(const char *old_filename, const char *new_filename) {
    if (old_filename == NULL || new_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (rename(old_filename, new_filename) != 0) {
        fprintf(stderr, "Error: Failed to rename file %s\n", old_filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_flush(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (fflush(stream->file) != 0) {
        fprintf(stderr, "Error: Failed to flush file\n");
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_setpos(fossil_fstream_t *stream, int32_t pos) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (fseek(stream->file, pos, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Failed to set file position\n");
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_getpos(fossil_fstream_t *stream, int32_t *pos) {
    if (stream == NULL || stream->file == NULL || pos == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    *pos = ftell(stream->file);
    if (*pos == -1L && ferror(stream->file)) {
        fprintf(stderr, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_rotate(const char *filename, int32_t n) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    char old_filename[FOSSIL_BUFFER_MEDIUM];
    char new_filename[FOSSIL_BUFFER_MEDIUM];

    for (int32_t i = n; i > 0; i--) {
        if (i == 1) {
            snprintf(old_filename, FOSSIL_BUFFER_MEDIUM, "%s", filename);
        } else {
            snprintf(old_filename, FOSSIL_BUFFER_MEDIUM, "%s.%d", filename, i - 1);
        }

        snprintf(new_filename, FOSSIL_BUFFER_MEDIUM, "%s.%d", filename, i);
        if (fossil_fstream_rename(old_filename, new_filename) != FOSSIL_ERROR_OK) {
            fprintf(stderr, "Error: Failed to rotate file %s\n", filename);
            return FOSSIL_ERROR_IO;
        }
    }

    return FOSSIL_ERROR_OK;
}

// Create a backup of a file with a specified backup suffix
int32_t fossil_fstream_backup(const char *filename, const char *backup_suffix) {
    if (filename == NULL || backup_suffix == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    char backup_filename[FOSSIL_BUFFER_MEDIUM + 10];  // Length of backup_suffix + maximum integer length
    snprintf(backup_filename, FOSSIL_BUFFER_MEDIUM + 10, "%s%s", filename, backup_suffix);

    if (fossil_fstream_copy(filename, backup_filename) != FOSSIL_ERROR_OK) {
        fprintf(stderr, "Error: Failed to create backup for %s\n", filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

// Check if a file exists
int32_t fossil_fstream_file_exists(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;  // File exists
    }
    return FOSSIL_ERROR_OK;  // File does not exist
}

// Get the size of an open stream
int32_t fossil_fstream_get_size(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    fseek(stream->file, 0, SEEK_END);
    long size = ftell(stream->file);
    rewind(stream->file);

    if (size == -1L && ferror(stream->file)) {
        fprintf(stderr, "Error: IO error from getting file size\n");
        return FOSSIL_ERROR_OK;
    }

    return (int32_t)size;
}

// Delete a file
int32_t fossil_fstream_delete(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (remove(filename) == 0) {
        return FOSSIL_ERROR_OK;  // File deleted successfully
    }

    fprintf(stderr, "Error: IO error when deleting file %s\n", filename);
    return FOSSIL_ERROR_IO;
}

// Detect file type (Regular file, Directory, Symbolic link)
int fossil_fstream_get_type(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    DWORD attributes = GetFileAttributesA(filename);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return -1;
    }

    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        return 1;  // Directory
    }

    if (attributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        return 3;  // Symbolic Link (or Junction)
    }

    return 2;  // Regular File
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode)) return 1;  // Directory
    if (S_ISREG(file_stat.st_mode)) return 2;  // Regular file
    if (S_ISLNK(file_stat.st_mode)) return 3;  // Symbolic link

    return 0;  // Unknown
#endif
}

int32_t fossil_fstream_is_open(const fossil_fstream_t *stream) {
    return stream != NULL && stream->file != NULL;
}

int32_t fossil_fstream_is_readable(const char *filename) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    return (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    return (access(filename, R_OK) == 0) ? 1 : 0;
#endif
}

int32_t fossil_fstream_is_writable(const char *filename) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES || (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        return 0;
    }
    return !(attrs & FILE_ATTRIBUTE_READONLY);
#else
    return (access(filename, W_OK) == 0) ? 1 : 0;
#endif
}

int32_t fossil_fstream_is_executable(const char *filename) {
#ifdef _WIN32
    // On Windows, executables typically have extensions like .exe, .bat, .cmd
    const char *ext = strrchr(filename, '.');
    return (ext && (_stricmp(ext, ".exe") == 0 || _stricmp(ext, ".bat") == 0 || _stricmp(ext, ".cmd") == 0)) ? 1 : 0;
#else
    return (access(filename, X_OK) == 0) ? 1 : 0;
#endif
}

int32_t fossil_fstream_set_permissions(const char *filename, int32_t mode) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return -1; // File not found or other error
    }

    if (mode & _S_IWRITE) {
        attrs &= ~FILE_ATTRIBUTE_READONLY; // Remove readonly
    } else {
        attrs |= FILE_ATTRIBUTE_READONLY; // Add readonly
    }

    return (SetFileAttributesA(filename, attrs) != 0) ? 0 : -1;
#else
    return chmod(filename, mode);
#endif
}

int32_t fossil_fstream_get_permissions(const char *filename, int32_t *mode) {
    if (!mode) {
        return -1; // Null pointer error
    }

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return -1; // File not found or other error
    }

    *mode = _S_IREAD;
    if (!(attrs & FILE_ATTRIBUTE_READONLY)) {
        *mode |= _S_IWRITE;
    }
    return 0;
#else
    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1; // File not found or error
    }
    *mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO); // User, Group, Other permissions
    return 0;
#endif
}

int64_t fossil_fstream_get_modified_time(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }
#ifdef _WIN32
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return -1;
    }

    FILETIME ft;
    if (!GetFileTime(file_handle, NULL, NULL, &ft)) {
        CloseHandle(file_handle);
        fprintf(stderr, "Error: IO error from getting file time\n");
        return -1;
    }
    CloseHandle(file_handle);

    return ((int64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return -1;
    }

    return (int64_t)file_stat.st_mtime;
#endif
}

int64_t fossil_fstream_get_creation_time(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }
#ifdef _WIN32
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return -1;
    }

    FILETIME ft;
    if (!GetFileTime(file_handle, &ft, NULL, NULL)) {
        CloseHandle(file_handle);
        fprintf(stderr, "Error: IO error from getting file time\n");
        return -1;
    }
    CloseHandle(file_handle);

    return ((int64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return -1;
    }

    return (int64_t)file_stat.st_ctime;
#endif
}

int32_t fossil_fstream_truncate(const char *filename, int64_t size) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }
#ifdef _WIN32
    HANDLE file_handle = CreateFileA(filename, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    if (SetFilePointer(file_handle, (DWORD)(size & 0xFFFFFFFF), (LONG *)&size, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        CloseHandle(file_handle);
        fprintf(stderr, "Error: IO error from truncating file\n");
        return FOSSIL_ERROR_IO;
    }

    if (!SetEndOfFile(file_handle)) {
        CloseHandle(file_handle);
        fprintf(stderr, "Error: IO error from truncating file\n");
        return FOSSIL_ERROR_IO;
    }
    CloseHandle(file_handle);
#else
    int fd = open(filename, O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    if (size < 0) {
        close(fd);
        fprintf(stderr, "Error: Invalid buffer size\n");
        return FOSSIL_ERROR_IO;
    }

    // Alternative to ftruncate: Use lseek and write
    if (lseek(fd, size, SEEK_SET) == -1) {
        close(fd);
        fprintf(stderr, "Error: IO error from seeking file\n");
        return FOSSIL_ERROR_IO;
    }

    if (write(fd, "", 1) != 1) { // Write a single null byte to extend the file
        close(fd);
        fprintf(stderr, "Error: IO error from writing to file\n");
        return FOSSIL_ERROR_IO;
    }

    close(fd);
#endif
    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_lock(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }
#ifdef _WIN32
    HANDLE file_handle = (HANDLE)_get_osfhandle(_fileno(stream->file));
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: IO error from locking file\n");
        return FOSSIL_ERROR_IO;
    }

    OVERLAPPED overlapped = {0};
    if (!LockFile(file_handle, 0, 0, MAXDWORD, MAXDWORD)) {
        fprintf(stderr, "Error: IO error from locking file\n");
        return FOSSIL_ERROR_IO;
    }
#else
    int fd = open(stream->filename, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Error: IO error from opening file for locking\n");
        return FOSSIL_ERROR_IO;
    }

    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK; // Write lock
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // Lock the whole file

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        fprintf(stderr, "Error: IO error from locking file\n");
        close(fd);
        return FOSSIL_ERROR_IO;
    }

    close(fd); // Close the file descriptor after locking
#endif
    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_unlock(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }
#ifdef _WIN32
    HANDLE file_handle = (HANDLE)_get_osfhandle(_fileno(stream->file));
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: IO error from unlocking file\n");
        return FOSSIL_ERROR_IO;
    }

    OVERLAPPED overlapped = {0};
    if (!UnlockFile(file_handle, 0, 0, MAXDWORD, MAXDWORD)) {
        fprintf(stderr, "Error: IO error from unlocking file\n");
        return FOSSIL_ERROR_IO;
    }
#else
    int fd;
    if ((fd = open(stream->filename, O_RDWR)) == -1) {
        fprintf(stderr, "Error: IO error from opening file for unlocking\n");
        return FOSSIL_ERROR_IO;
    }

    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_UNLCK; // Unlock
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // Unlock the whole file

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        fprintf(stderr, "Error: IO error from unlocking file\n");
        close(fd);
        return FOSSIL_ERROR_IO;
    }

    close(fd); // Close the file descriptor after unlocking
#endif
    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_sync(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }
#ifdef _WIN32
    HANDLE file_handle = (HANDLE)_get_osfhandle(_fileno(stream->file));
    if (file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: IO error from syncing file\n");
        return FOSSIL_ERROR_IO;
    }

    if (!FlushFileBuffers(file_handle)) {
        fprintf(stderr, "Error: IO error from syncing file\n");
        return FOSSIL_ERROR_IO;
    }
#else
    int fd;
    if ((fd = open(stream->filename, O_RDWR)) == -1) {
        fprintf(stderr, "Error: IO error from opening file for syncing\n");
        return FOSSIL_ERROR_IO;
    }

    if (fsync(fd) == -1) {
        fprintf(stderr, "Error: IO error from syncing file\n");
        close(fd);
        return FOSSIL_ERROR_IO;
    }

    close(fd);
#endif
    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_set_buffering(fossil_fstream_t *stream, int32_t enabled) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (enabled) {
        setvbuf(stream->file, NULL, _IOFBF, FOSSIL_BUFFER_MEDIUM);
    } else {
        setvbuf(stream->file, NULL, _IONBF, 0);
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_set_buffer(fossil_fstream_t *stream, void *buffer, size_t size) {
    if (stream == NULL || stream->file == NULL || buffer == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (setvbuf(stream->file, buffer, _IOFBF, size) != 0) {
        fprintf(stderr, "Error: IO error from setting buffer\n");
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_set_autosync(fossil_fstream_t *stream, int32_t enabled) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (enabled) {
        setvbuf(stream->file, NULL, _IOLBF, 0);
    } else {
        setvbuf(stream->file, NULL, _IOFBF, FOSSIL_BUFFER_MEDIUM);
    }

    return FOSSIL_ERROR_OK;
}
