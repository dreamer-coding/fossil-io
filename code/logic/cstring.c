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
#include "fossil/io/cstring.h"
#include <ctype.h>
#include <stdlib.h>

// ============================================================================
// C String Functions
// ============================================================================

cstring fossil_io_cstring_create(const char *init) {
    if (!init) return NULL;
    size_t length = strlen(init);
    cstring str = (cstring)malloc(length + 1);
    if (str) {
        strcpy(str, init);
    }
    return str;
}

void fossil_io_cstring_free(cstring str) {
    if (str) {
        free(str);
    }
}

cstring fossil_io_cstring_copy(ccstring str) {
    if (!str) return NULL;
    return fossil_io_cstring_create(str);
}

cstring fossil_io_cstring_dup(ccstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring new_str = (cstring)malloc(length + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

cstring fossil_io_cstring_concat(ccstring s1, ccstring s2) {
    if (!s1 || !s2) return NULL;
    size_t length1 = strlen(s1);
    size_t length2 = strlen(s2);
    cstring str = (cstring)malloc(length1 + length2 + 1);
    if (str) {
        strcpy(str, s1);
        strcpy(str + length1, s2);
    }
    return str;
}

size_t fossil_io_cstring_length(ccstring str) {
    if (!str) return 0;
    return strlen(str);
}

int fossil_io_cstring_compare(ccstring s1, ccstring s2) {
    if (!s1 || !s2) return -1;
    return strcmp(s1, s2);
}

void fossil_io_cstring_trim(cstring str) {
    if (!str) return;
    size_t length = strlen(str);
    size_t start = 0;
    size_t end = length - 1;
    while (start < length && isspace(str[start])) {
        start++;
    }
    while (end > start && isspace(str[end])) {
        end--;
    }
    size_t count = end - start + 1;
    if (start > 0) {
        memmove(str, str + start, count);
    }
    str[count] = '\0';
}

cstring *fossil_io_cstring_split(ccstring str, char delimiter, size_t *count) {
    if (!str || !count) return NULL;
    size_t length = strlen(str);
    size_t num_delimiters = 0;
    for (size_t i = 0; i < length; i++) {
        if (str[i] == delimiter) {
            num_delimiters++;
        }
    }
    *count = num_delimiters + 1;
    cstring *result = (cstring*)malloc(*count * sizeof(cstring));
    if (result) {
        size_t start = 0;
        size_t index = 0;
        for (size_t i = 0; i < length; i++) {
            if (str[i] == delimiter) {
                size_t count = i - start;
                result[index] = (cstring)malloc(count + 1);
                if (result[index]) {
                    memcpy(result[index], str + start, count);
                    result[index][count] = '\0';
                    start = i + 1;
                    index++;
                } else {
                    // Free previously allocated memory on error
                    for (size_t j = 0; j < index; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
            }
        }
        size_t count = length - start;
        result[index] = (cstring)malloc(count + 1);
        if (result[index]) {
            memcpy(result[index], str + start, count);
            result[index][count] = '\0';
        } else {
            // Free previously allocated memory on error
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
    }
    return result;
}

cstring fossil_io_cstring_replace(ccstring str, ccstring old, ccstring new_str) {
    if (!str || !old || !new_str) return NULL;
    size_t old_length = strlen(old);
    size_t new_length = strlen(new_str);
    size_t count = 0;
    size_t index = 0;
    size_t length = strlen(str);
    while (index < length) {
        if (strstr(str + index, old) == str + index) {
            count++;
            index += old_length;
        } else {
            index++;
        }
    }
    cstring result = (cstring)malloc(length + count * (new_length - old_length) + 1);
    if (result) {
        index = 0;
        size_t start = 0;
        while (index < length) {
            if (strstr(str + index, old) == str + index) {
                strcpy(result + start, new_str);
                start += new_length;
                index += old_length;
            } else {
                result[start] = str[index];
                start++;
                index++;
            }
        }
        result[start] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_to_upper(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = toupper(str[i]);
        }
        result[length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_to_lower(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = tolower(str[i]);
        }
        result[length] = '\0';
    }
    return result;
}

int fossil_io_cstring_starts_with(ccstring str, ccstring prefix) {
    if (!str || !prefix) return 0;
    size_t str_length = strlen(str);
    size_t prefix_length = strlen(prefix);
    if (prefix_length > str_length) {
        return 0;
    }
    return strncmp(str, prefix, prefix_length) == 0;
}

int fossil_io_cstring_ends_with(ccstring str, ccstring suffix) {
    if (!str || !suffix) return 0;
    size_t str_length = strlen(str);
    size_t suffix_length = strlen(suffix);
    if (suffix_length > str_length) {
        return 0;
    }
    return strncmp(str + str_length - suffix_length, suffix, suffix_length) == 0;
}

cstring fossil_io_cstring_substring(ccstring str, size_t start, size_t length) {
    if (!str) return NULL;
    size_t str_length = strlen(str);
    if (start >= str_length) {
        return NULL;
    }
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        size_t count = str_length - start;
        if (length < count) {
            count = length;
        }
        memcpy(result, str + start, count);
        result[count] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_reverse(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = str[length - i - 1];
        }
        result[length] = '\0';
    }
    return result;
}

int fossil_io_cstring_contains(ccstring str, ccstring substr) {
    if (!str || !substr) return 0;
    return strstr(str, substr) != NULL;
}

cstring fossil_io_cstring_repeat(ccstring str, size_t count) {
    if (!str || count == 0) return NULL;
    size_t length = strlen(str);
    size_t new_length = length * count;
    cstring result = (cstring)malloc(new_length + 1);
    if (result) {
        for (size_t i = 0; i < count; i++) {
            memcpy(result + i * length, str, length);
        }
        result[new_length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_strip(ccstring str, char ch) {
    if (!str) return NULL;
    size_t length = strlen(str);
    size_t start = 0;
    size_t end = length - 1;
    while (start < length && str[start] == ch) {
        start++;
    }
    while (end > start && str[end] == ch) {
        end--;
    }
    size_t count = end - start + 1;
    cstring result = (cstring)malloc(count + 1);
    if (result) {
        memcpy(result, str + start, count);
        result[count] = '\0';
    }
    return result;
}

size_t fossil_io_cstring_count(ccstring str, ccstring substr) {
    if (!str || !substr) return 0;
    size_t count = 0;
    size_t length = strlen(substr);
    while ((str = strstr(str, substr)) != NULL) {
        count++;
        str += length;
    }
    return count;
}

cstring fossil_io_cstring_pad_left(ccstring str, size_t total_length, char pad_char) {
    if (!str || total_length == 0) return NULL;
    size_t length = strlen(str);
    if (length >= total_length) {
        return fossil_io_cstring_copy(str);
    }
    size_t pad_length = total_length - length;
    cstring result = (cstring)malloc(total_length + 1);
    if (result) {
        memset(result, pad_char, pad_length);
        strcpy(result + pad_length, str);
        result[total_length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_pad_right(ccstring str, size_t total_length, char pad_char) {
    if (!str || total_length == 0) return NULL;
    size_t length = strlen(str);
    if (length >= total_length) {
        return fossil_io_cstring_copy(str);
    }
    size_t pad_length = total_length - length;
    cstring result = (cstring)malloc(total_length + 1);
    if (result) {
        strcpy(result, str);
        memset(result + length, pad_char, pad_length);
        result[total_length] = '\0';
    }
    return result;
}

// ============================================================================
// String Stream Functions
// ============================================================================

fossil_io_cstring_stream* fossil_io_cstring_stream_create(size_t initial_size) {
    if (initial_size == 0) return NULL;
    fossil_io_cstring_stream *stream = (fossil_io_cstring_stream*)malloc(sizeof(fossil_io_cstring_stream));
    if (stream) {
        stream->buffer = (char*)malloc(initial_size);
        if (stream->buffer) {
            stream->buffer[0] = '\0';
            stream->length = 0;
            stream->capacity = initial_size;
        } else {
            free(stream);
            stream = NULL;
        }
    }
    return stream;
}

void fossil_io_cstring_stream_free(fossil_io_cstring_stream *stream) {
    if (stream) {
        free(stream->buffer);
        free(stream);
    }
}

void fossil_io_cstring_stream_write(fossil_io_cstring_stream *stream, ccstring str) {
    if (!stream || !str) return;
    size_t length = strlen(str);
    size_t new_length = stream->length + length;
    if (new_length > stream->capacity) {
        size_t new_capacity = stream->capacity * 2;
        if (new_capacity < new_length) {
            new_capacity = new_length;
        }
        char *new_buffer = (char*)realloc(stream->buffer, new_capacity);
        if (new_buffer) {
            stream->buffer = new_buffer;
            stream->capacity = new_capacity;
        } else {
            return;
        }
    }
    memcpy(stream->buffer + stream->length, str, length);
    stream->length = new_length;
    stream->buffer[stream->length] = '\0';
}

ccstring fossil_io_cstring_stream_read(fossil_io_cstring_stream *stream) {
    if (!stream) return NULL;
    return stream->buffer;
}
