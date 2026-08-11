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
#include "fossil/io/cube.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <errno.h>
#endif


/* ------------------------------------------------------------
 * Internal Constants
 * ------------------------------------------------------------ */

#define CUBE_DEFAULT_WIDTH   80
#define CUBE_DEFAULT_HEIGHT  24
#define CUBE_DEFAULT_BACKEND "ansi"

#define CUBE_OK              0
#define CUBE_ERROR          -1
#define CUBE_NOT_FOUND      -2
#define CUBE_INVALID        -3
#define CUBE_EXISTS         -4
#define CUBE_FULL           -5


/* ------------------------------------------------------------
 * Internal Objects
 * ------------------------------------------------------------ */

typedef struct
{
    char id[FOSSIL_IO_CUBE_MAX_ID];

    fossil_io_cube_action_callback_t callback;
    void *user_data;

} cube_action_t;


typedef struct
{
    char key[FOSSIL_IO_CUBE_MAX_ID];
    char action[FOSSIL_IO_CUBE_MAX_ID];

} cube_binding_t;


typedef struct
{
    char id[FOSSIL_IO_CUBE_MAX_ID];
    char label[FOSSIL_IO_CUBE_MAX_TEXT];

} cube_menu_item_t;


typedef struct
{
    char id[FOSSIL_IO_CUBE_MAX_ID];
    fossil_io_cube_style_t style;

} cube_style_entry_t;


/* ------------------------------------------------------------
 * Window
 * ------------------------------------------------------------ */

struct fossil_io_cube_window_t
{
    fossil_io_cube_t *cube;

    char id[FOSSIL_IO_CUBE_MAX_ID];
    char title[FOSSIL_IO_CUBE_MAX_TEXT];
    char style[FOSSIL_IO_CUBE_MAX_ID];

    fossil_io_cube_rect_t rect;

    int32_t visible;
    int32_t z_index;
};


/* ------------------------------------------------------------
 * Panel
 * ------------------------------------------------------------ */

struct fossil_io_cube_panel_t
{
    fossil_io_cube_t *cube;

    char id[FOSSIL_IO_CUBE_MAX_ID];

    fossil_io_cube_rect_t rect;

    fossil_io_cube_window_t **windows;
    size_t window_count;
    size_t window_capacity;
};


/* ------------------------------------------------------------
 * Widget
 * ------------------------------------------------------------ */

struct fossil_io_cube_widget_t
{
    fossil_io_cube_t *cube;

    char id[FOSSIL_IO_CUBE_MAX_ID];
    char type[FOSSIL_IO_CUBE_MAX_ID];

    char text[FOSSIL_IO_CUBE_MAX_TEXT];

    fossil_io_cube_rect_t rect;

    int32_t visible;
    int32_t enabled;
    int32_t focused;

    int32_t focus_index;

    char style[FOSSIL_IO_CUBE_MAX_ID];

    struct
    {
        char id[FOSSIL_IO_CUBE_MAX_ID];
        char value[FOSSIL_IO_CUBE_MAX_TEXT];

    } properties[64];

    size_t property_count;

    fossil_io_cube_event_callback_t event_callback;
    void *event_user_data;

    fossil_io_cube_render_callback_t render_callback;
    void *render_user_data;

    size_t input_cursor;
};


/* ------------------------------------------------------------
 * Menu
 * ------------------------------------------------------------ */

struct fossil_io_cube_menu_t
{
    fossil_io_cube_t *cube;

    char id[FOSSIL_IO_CUBE_MAX_ID];

    fossil_io_cube_rect_t rect;

    cube_menu_item_t items[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t item_count;

    size_t selected_index;

    char selected[FOSSIL_IO_CUBE_MAX_ID];

    char mode[FOSSIL_IO_CUBE_MAX_ID];
};


/* ------------------------------------------------------------
 * Layout
 * ------------------------------------------------------------ */

struct fossil_io_cube_layout_t
{
    fossil_io_cube_t *cube;

    char id[FOSSIL_IO_CUBE_MAX_ID];
    char type[FOSSIL_IO_CUBE_MAX_ID];

    fossil_io_cube_widget_t *
        widgets[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t widget_count;
};


/* ------------------------------------------------------------
 * Cube Context
 * ------------------------------------------------------------ */

struct fossil_io_cube_t
{
    fossil_io_cube_config_t config;

    fossil_io_cube_size_t terminal;

    fossil_io_cube_window_t *
        windows[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t window_count;

    fossil_io_cube_panel_t *
        panels[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t panel_count;

    fossil_io_cube_widget_t *
        widgets[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t widget_count;

    fossil_io_cube_menu_t *
        menus[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t menu_count;

    fossil_io_cube_layout_t *
        layouts[FOSSIL_IO_CUBE_MAX_CHILDREN];

    size_t layout_count;

    cube_action_t actions[FOSSIL_IO_CUBE_MAX_CHILDREN];
    size_t action_count;

    cube_binding_t bindings[FOSSIL_IO_CUBE_MAX_CHILDREN];
    size_t binding_count;

    cube_style_entry_t styles[FOSSIL_IO_CUBE_MAX_CHILDREN];
    size_t style_count;

    fossil_io_cube_widget_t *focused;

    char active[FOSSIL_IO_CUBE_MAX_ID];
    char status[FOSSIL_IO_CUBE_MAX_TEXT];

    char quit_reason[FOSSIL_IO_CUBE_MAX_TEXT];

    int32_t running;
    int32_t initialized;
    int32_t cursor_visible;

    int32_t cursor_x;
    int32_t cursor_y;

    int32_t frame_active;

    uint64_t frame_counter;
};


/* ------------------------------------------------------------
 * Internal Helpers
 * ------------------------------------------------------------ */

// static int cube_string_valid(const char *value)
// {
//     return value != NULL && value[0] != '\0';
// }


static int cube_string_copy(
    char *destination,
    size_t destination_size,
    const char *source
)
{
    size_t length;

    if (!destination || destination_size == 0)
        return CUBE_INVALID;

    if (!source)
    {
        destination[0] = '\0';
        return CUBE_OK;
    }

    length = strlen(source);

    if (length >= destination_size)
        return CUBE_ERROR;

    memcpy(destination, source, length + 1);

    return CUBE_OK;
}


static fossil_io_cube_window_t *
cube_find_window(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->window_count; ++i)
    {
        if (strcmp(cube->windows[i]->id, id) == 0)
            return cube->windows[i];
    }

    return NULL;
}


static fossil_io_cube_widget_t *
cube_find_widget(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->widget_count; ++i)
    {
        if (strcmp(cube->widgets[i]->id, id) == 0)
            return cube->widgets[i];
    }

    return NULL;
}


static fossil_io_cube_panel_t *
cube_find_panel(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->panel_count; ++i)
    {
        if (strcmp(cube->panels[i]->id, id) == 0)
            return cube->panels[i];
    }

    return NULL;
}


static fossil_io_cube_menu_t *
cube_find_menu(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->menu_count; ++i)
    {
        if (strcmp(cube->menus[i]->id, id) == 0)
            return cube->menus[i];
    }

    return NULL;
}


static fossil_io_cube_layout_t *
cube_find_layout(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->layout_count; ++i)
    {
        if (strcmp(cube->layouts[i]->id, id) == 0)
            return cube->layouts[i];
    }

    return NULL;
}


static cube_action_t *
cube_find_action(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return NULL;

    for (i = 0; i < cube->action_count; ++i)
    {
        if (strcmp(cube->actions[i].id, id) == 0)
            return &cube->actions[i];
    }

    return NULL;
}


static const char *
cube_style_attribute(
    const fossil_io_cube_style_t *style
)
{
    if (!style)
        return "";

    return style->attributes;
}


static void
cube_ansi_write(
    fossil_io_cube_t *cube,
    const char *text
)
{
    FILE *stream = stdout;

    (void)cube;

    if (text)
        fputs(text, stream);
}


static void
cube_ansi_flush(
    fossil_io_cube_t *cube
)
{
    (void)cube;
    fflush(stdout);
}


static void
cube_ansi_reset(
    fossil_io_cube_t *cube
)
{
    cube_ansi_write(cube, "\033[0m");
}


static void
cube_ansi_move(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y
)
{
    char buffer[64];

    snprintf(
        buffer,
        sizeof(buffer),
        "\033[%d;%dH",
        y + 1,
        x + 1
    );

    cube_ansi_write(cube, buffer);

    cube->cursor_x = x;
    cube->cursor_y = y;
}


static void
cube_ansi_clear(
    fossil_io_cube_t *cube
)
{
    cube_ansi_write(cube, "\033[2J\033[H");

    cube->cursor_x = 0;
    cube->cursor_y = 0;
}


static void
cube_apply_style(
    fossil_io_cube_t *cube,
    const char *style
)
{
    size_t i;

    if (!cube || !style)
        return;

    for (i = 0; i < cube->style_count; ++i)
    {
        if (strcmp(cube->styles[i].id, style) == 0)
        {
            char buffer[FOSSIL_IO_CUBE_MAX_TEXT + 16];

            snprintf(
                buffer,
                sizeof(buffer),
                "\033[%sm",
                cube_style_attribute(
                    &cube->styles[i].style
                )
            );

            cube_ansi_write(cube, buffer);
            return;
        }
    }

    /*
     * Permit raw ANSI SGR strings as an implementation-level
     * fallback when the caller has not registered a style.
     */
    if (isdigit((unsigned char)style[0]))
    {
        char buffer[64];

        snprintf(
            buffer,
            sizeof(buffer),
            "\033[%sm",
            style
        );

        cube_ansi_write(cube, buffer);
    }
}


static void
cube_draw_window(
    fossil_io_cube_window_t *window
)
{
    fossil_io_cube_t *cube;
    fossil_io_cube_rect_t r;
    int32_t x;
    int32_t y;

    if (!window || !window->visible)
        return;

    cube = window->cube;
    r = window->rect;

    if (r.width < 2 || r.height < 2)
        return;

    cube_apply_style(cube, window->style);

    cube_ansi_move(cube, r.x, r.y);

    cube_ansi_write(cube, "+");

    for (x = 1; x < r.width - 1; ++x)
        cube_ansi_write(cube, "-");

    cube_ansi_write(cube, "+");

    for (y = 1; y < r.height - 1; ++y)
    {
        cube_ansi_move(cube, r.x, r.y + y);
        cube_ansi_write(cube, "|");

        for (x = 1; x < r.width - 1; ++x)
            cube_ansi_write(cube, " ");

        cube_ansi_write(cube, "|");
    }

    cube_ansi_move(
        cube,
        r.x,
        r.y + r.height - 1
    );

    cube_ansi_write(cube, "+");

    for (x = 1; x < r.width - 1; ++x)
        cube_ansi_write(cube, "-");

    cube_ansi_write(cube, "+");

    if (window->title[0])
    {
        cube_ansi_move(
            cube,
            r.x + 2,
            r.y
        );

        cube_ansi_write(cube, " ");

        cube_ansi_write(cube, window->title);

        cube_ansi_write(cube, " ");
    }

    cube_ansi_reset(cube);
}


static void
cube_draw_widget(
    fossil_io_cube_widget_t *widget
)
{
    fossil_io_cube_t *cube;

    if (!widget || !widget->visible)
        return;

    cube = widget->cube;

    if (widget->render_callback)
    {
        widget->render_callback(
            widget,
            widget->render_user_data
        );

        return;
    }

    cube_apply_style(cube, widget->style);

    if (widget->rect.width > 0 &&
        widget->rect.height > 0)
    {
        cube_ansi_move(
            cube,
            widget->rect.x,
            widget->rect.y
        );

        cube_ansi_write(
            cube,
            widget->text
        );
    }

    cube_ansi_reset(cube);
}


/* ------------------------------------------------------------
 * Initialization
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_init(
    fossil_io_cube_t *cube,
    const fossil_io_cube_config_t *config
)
{
    if (!cube)
        return CUBE_INVALID;

    memset(cube, 0, sizeof(*cube));

    if (config)
        memcpy(
            &cube->config,
            config,
            sizeof(*config)
        );

    if (!cube->config.backend)
        cube->config.backend = CUBE_DEFAULT_BACKEND;

    cube->terminal.width = CUBE_DEFAULT_WIDTH;
    cube->terminal.height = CUBE_DEFAULT_HEIGHT;

    cube->cursor_visible = 1;
    cube->running = 1;
    cube->initialized = 1;

    fossil_io_cube_terminal_size(
        cube,
        &cube->terminal
    );

    fossil_io_cube_cursor_hide(cube);
    fossil_io_cube_clear(cube);

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Shutdown
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_shutdown(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    if (!cube->initialized)
        return CUBE_OK;

    fossil_io_cube_cursor_show(cube);
    cube_ansi_reset(cube);
    cube_ansi_flush(cube);

    cube->running = 0;
    cube->initialized = 0;

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Create / Destroy
 * ------------------------------------------------------------ */

fossil_io_cube_t *
fossil_io_cube_create(
    const fossil_io_cube_config_t *config
)
{
    fossil_io_cube_t *cube;

    cube = calloc(1, sizeof(*cube));

    if (!cube)
        return NULL;

    if (fossil_io_cube_init(cube, config) < 0)
    {
        free(cube);
        return NULL;
    }

    return cube;
}


void fossil_io_cube_destroy(
    fossil_io_cube_t *cube
)
{
    size_t i;

    if (!cube)
        return;

    fossil_io_cube_shutdown(cube);

    for (i = 0; i < cube->window_count; ++i)
        free(cube->windows[i]);

    for (i = 0; i < cube->panel_count; ++i)
    {
        free(cube->panels[i]->windows);
        free(cube->panels[i]);
    }

    for (i = 0; i < cube->widget_count; ++i)
        free(cube->widgets[i]);

    for (i = 0; i < cube->menu_count; ++i)
        free(cube->menus[i]);

    for (i = 0; i < cube->layout_count; ++i)
        free(cube->layouts[i]);

    free(cube);
}


/* ------------------------------------------------------------
 * Terminal
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_terminal_size(
    fossil_io_cube_t *cube,
    fossil_io_cube_size_t *size
)
{
    if (!cube || !size)
        return CUBE_INVALID;

#if defined(_WIN32)

    {
        CONSOLE_SCREEN_BUFFER_INFO info;

        HANDLE handle =
            GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle != INVALID_HANDLE_VALUE &&
            GetConsoleScreenBufferInfo(handle, &info))
        {
            size->width =
                info.srWindow.Right -
                info.srWindow.Left + 1;

            size->height =
                info.srWindow.Bottom -
                info.srWindow.Top + 1;

            cube->terminal = *size;

            return CUBE_OK;
        }
    }

#else

    {
        struct winsize ws;

        if (ioctl(
                STDOUT_FILENO,
                TIOCGWINSZ,
                &ws
            ) == 0)
        {
            if (ws.ws_col > 0)
                size->width = ws.ws_col;
            else
                size->width = CUBE_DEFAULT_WIDTH;

            if (ws.ws_row > 0)
                size->height = ws.ws_row;
            else
                size->height = CUBE_DEFAULT_HEIGHT;

            cube->terminal = *size;

            return CUBE_OK;
        }
    }

#endif

    *size = cube->terminal;

    return CUBE_OK;
}


int32_t fossil_io_cube_clear(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    cube_ansi_clear(cube);

    return CUBE_OK;
}


int32_t fossil_io_cube_cursor_move(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y
)
{
    if (!cube)
        return CUBE_INVALID;

    cube_ansi_move(cube, x, y);

    return CUBE_OK;
}


int32_t fossil_io_cube_cursor_show(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    cube_ansi_write(cube, "\033[?25h");

    cube->cursor_visible = 1;

    return CUBE_OK;
}


int32_t fossil_io_cube_cursor_hide(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    cube_ansi_write(cube, "\033[?25l");

    cube->cursor_visible = 0;

    return CUBE_OK;
}


int32_t fossil_io_cube_terminal_title(
    fossil_io_cube_t *cube,
    const char *title
)
{
    if (!cube || !title)
        return CUBE_INVALID;

    cube_ansi_write(cube, "\033]0;");
    cube_ansi_write(cube, title);
    cube_ansi_write(cube, "\007");

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Rendering
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_begin(
    fossil_io_cube_t *cube
)
{
    if (!cube || !cube->initialized)
        return CUBE_INVALID;

    cube->frame_active = 1;
    cube->frame_counter++;

    return CUBE_OK;
}


int32_t fossil_io_cube_render(
    fossil_io_cube_t *cube
)
{
    size_t i;

    if (!cube)
        return CUBE_INVALID;

    for (i = 0; i < cube->window_count; ++i)
        cube_draw_window(cube->windows[i]);

    for (i = 0; i < cube->widget_count; ++i)
        cube_draw_widget(cube->widgets[i]);

    if (cube->status[0])
    {
        cube_ansi_move(
            cube,
            0,
            cube->terminal.height - 1
        );

        cube_ansi_write(cube, cube->status);
    }

    return CUBE_OK;
}


int32_t fossil_io_cube_end(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    cube->frame_active = 0;

    cube_ansi_reset(cube);
    cube_ansi_flush(cube);

    return CUBE_OK;
}


int32_t fossil_io_cube_draw_text(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y,
    const char *text
)
{
    if (!cube || !text)
        return CUBE_INVALID;

    cube_ansi_move(cube, x, y);
    cube_ansi_write(cube, text);

    return CUBE_OK;
}


int32_t fossil_io_cube_draw_text_styled(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y,
    const char *text,
    const char *style
)
{
    if (!cube || !text)
        return CUBE_INVALID;

    cube_apply_style(cube, style);

    fossil_io_cube_draw_text(
        cube,
        x,
        y,
        text
    );

    cube_ansi_reset(cube);

    return CUBE_OK;
}


int32_t fossil_io_cube_draw_border(
    fossil_io_cube_t *cube,
    const fossil_io_cube_rect_t *rect,
    const char *style
)
{
    int32_t x;
    int32_t y;

    if (!cube || !rect)
        return CUBE_INVALID;

    if (rect->width < 2 || rect->height < 2)
        return CUBE_INVALID;

    cube_apply_style(cube, style);

    cube_ansi_move(
        cube,
        rect->x,
        rect->y
    );

    cube_ansi_write(cube, "+");

    for (x = 1; x < rect->width - 1; ++x)
        cube_ansi_write(cube, "-");

    cube_ansi_write(cube, "+");

    for (y = 1; y < rect->height - 1; ++y)
    {
        cube_ansi_move(
            cube,
            rect->x,
            rect->y + y
        );

        cube_ansi_write(cube, "|");

        cube_ansi_move(
            cube,
            rect->x + rect->width - 1,
            rect->y + y
        );

        cube_ansi_write(cube, "|");
    }

    cube_ansi_move(
        cube,
        rect->x,
        rect->y + rect->height - 1
    );

    cube_ansi_write(cube, "+");

    for (x = 1; x < rect->width - 1; ++x)
        cube_ansi_write(cube, "-");

    cube_ansi_write(cube, "+");

    cube_ansi_reset(cube);

    return CUBE_OK;
}


int32_t fossil_io_cube_fill(
    fossil_io_cube_t *cube,
    const fossil_io_cube_rect_t *rect,
    char character
)
{
    int32_t x;
    int32_t y;

    char buffer[2];

    if (!cube || !rect)
        return CUBE_INVALID;

    buffer[0] = character;
    buffer[1] = '\0';

    for (y = 0; y < rect->height; ++y)
    {
        cube_ansi_move(
            cube,
            rect->x,
            rect->y + y
        );

        for (x = 0; x < rect->width; ++x)
            cube_ansi_write(cube, buffer);
    }

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Windows
 * ------------------------------------------------------------ */

fossil_io_cube_window_t *
fossil_io_cube_window_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
)
{
    fossil_io_cube_window_t *window;

    if (!cube || !id || !rect)
        return NULL;

    if (!fossil_io_cube_validate_id(id))
        return NULL;

    if (cube_find_window(cube, id))
        return NULL;

    if (cube->window_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return NULL;

    window = calloc(1, sizeof(*window));

    if (!window)
        return NULL;

    window->cube = cube;
    window->rect = *rect;
    window->visible = 1;
    window->z_index =
        (int32_t)cube->window_count;

    cube_string_copy(
        window->id,
        sizeof(window->id),
        id
    );

    cube->windows[
        cube->window_count++
    ] = window;

    return window;
}


int32_t fossil_io_cube_window_destroy(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return CUBE_INVALID;

    for (i = 0; i < cube->window_count; ++i)
    {
        if (strcmp(cube->windows[i]->id, id) == 0)
        {
            free(cube->windows[i]);

            memmove(
                &cube->windows[i],
                &cube->windows[i + 1],
                (cube->window_count - i - 1) *
                    sizeof(cube->windows[0])
            );

            cube->window_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


fossil_io_cube_window_t *
fossil_io_cube_window_find(
    fossil_io_cube_t *cube,
    const char *id
)
{
    return cube_find_window(cube, id);
}


int32_t fossil_io_cube_window_set_rect(
    fossil_io_cube_window_t *window,
    const fossil_io_cube_rect_t *rect
)
{
    if (!window || !rect)
        return CUBE_INVALID;

    window->rect = *rect;

    return CUBE_OK;
}


int32_t fossil_io_cube_window_set_title(
    fossil_io_cube_window_t *window,
    const char *title
)
{
    if (!window || !title)
        return CUBE_INVALID;

    return cube_string_copy(
        window->title,
        sizeof(window->title),
        title
    );
}


int32_t fossil_io_cube_window_set_style(
    fossil_io_cube_window_t *window,
    const char *style
)
{
    if (!window || !style)
        return CUBE_INVALID;

    return cube_string_copy(
        window->style,
        sizeof(window->style),
        style
    );
}


int32_t fossil_io_cube_window_draw_text(
    fossil_io_cube_window_t *window,
    int32_t x,
    int32_t y,
    const char *text
)
{
    if (!window || !text)
        return CUBE_INVALID;

    return fossil_io_cube_draw_text(
        window->cube,
        window->rect.x + x,
        window->rect.y + y,
        text
    );
}


int32_t fossil_io_cube_window_clear(
    fossil_io_cube_window_t *window
)
{
    if (!window)
        return CUBE_INVALID;

    return fossil_io_cube_fill(
        window->cube,
        &window->rect,
        ' '
    );
}


int32_t fossil_io_cube_window_raise(
    fossil_io_cube_window_t *window
)
{
    if (!window)
        return CUBE_INVALID;

    window->z_index =
        (int32_t)window->cube->window_count;

    return CUBE_OK;
}


int32_t fossil_io_cube_window_lower(
    fossil_io_cube_window_t *window
)
{
    if (!window)
        return CUBE_INVALID;

    window->z_index = 0;

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Panels
 * ------------------------------------------------------------ */

fossil_io_cube_panel_t *
fossil_io_cube_panel_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
)
{
    fossil_io_cube_panel_t *panel;

    if (!cube || !id || !rect)
        return NULL;

    if (cube_find_panel(cube, id))
        return NULL;

    if (cube->panel_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return NULL;

    panel = calloc(1, sizeof(*panel));

    if (!panel)
        return NULL;

    panel->cube = cube;
    panel->rect = *rect;

    cube_string_copy(
        panel->id,
        sizeof(panel->id),
        id
    );

    cube->panels[
        cube->panel_count++
    ] = panel;

    return panel;
}


fossil_io_cube_panel_t *
fossil_io_cube_panel_find(
    fossil_io_cube_t *cube,
    const char *id
)
{
    return cube_find_panel(cube, id);
}


int32_t fossil_io_cube_panel_destroy(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return CUBE_INVALID;

    for (i = 0; i < cube->panel_count; ++i)
    {
        if (strcmp(cube->panels[i]->id, id) == 0)
        {
            free(cube->panels[i]->windows);
            free(cube->panels[i]);

            memmove(
                &cube->panels[i],
                &cube->panels[i + 1],
                (cube->panel_count - i - 1) *
                    sizeof(cube->panels[0])
            );

            cube->panel_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_panel_add_window(
    fossil_io_cube_panel_t *panel,
    fossil_io_cube_window_t *window
)
{
    fossil_io_cube_window_t **new_windows;

    if (!panel || !window)
        return CUBE_INVALID;

    if (panel->window_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    new_windows = realloc(
        panel->windows,
        (panel->window_count + 1) *
        sizeof(*new_windows)
    );

    if (!new_windows)
        return CUBE_ERROR;

    panel->windows = new_windows;

    panel->windows[
        panel->window_count++
    ] = window;

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Widgets
 * ------------------------------------------------------------ */

fossil_io_cube_widget_t *
fossil_io_cube_widget_create(
    fossil_io_cube_t *cube,
    const char *id,
    const char *type,
    const fossil_io_cube_rect_t *rect
)
{
    fossil_io_cube_widget_t *widget;

    if (!cube || !id || !type || !rect)
        return NULL;

    if (cube_find_widget(cube, id))
        return NULL;

    if (cube->widget_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return NULL;

    widget = calloc(1, sizeof(*widget));

    if (!widget)
        return NULL;

    widget->cube = cube;
    widget->rect = *rect;
    widget->visible = 1;
    widget->enabled = 1;
    widget->focus_index =
        (int32_t)cube->widget_count;

    cube_string_copy(
        widget->id,
        sizeof(widget->id),
        id
    );

    cube_string_copy(
        widget->type,
        sizeof(widget->type),
        type
    );

    cube->widgets[
        cube->widget_count++
    ] = widget;

    return widget;
}


fossil_io_cube_widget_t *
fossil_io_cube_widget_find(
    fossil_io_cube_t *cube,
    const char *id
)
{
    return cube_find_widget(cube, id);
}


int32_t fossil_io_cube_widget_destroy(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return CUBE_INVALID;

    for (i = 0; i < cube->widget_count; ++i)
    {
        if (strcmp(cube->widgets[i]->id, id) == 0)
        {
            if (cube->focused == cube->widgets[i])
                cube->focused = NULL;

            free(cube->widgets[i]);

            memmove(
                &cube->widgets[i],
                &cube->widgets[i + 1],
                (cube->widget_count - i - 1) *
                    sizeof(cube->widgets[0])
            );

            cube->widget_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_widget_set_text(
    fossil_io_cube_widget_t *widget,
    const char *text
)
{
    if (!widget || !text)
        return CUBE_INVALID;

    return cube_string_copy(
        widget->text,
        sizeof(widget->text),
        text
    );
}


const char *
fossil_io_cube_widget_get_text(
    fossil_io_cube_widget_t *widget
)
{
    if (!widget)
        return NULL;

    return widget->text;
}


int32_t fossil_io_cube_widget_set_rect(
    fossil_io_cube_widget_t *widget,
    const fossil_io_cube_rect_t *rect
)
{
    if (!widget || !rect)
        return CUBE_INVALID;

    widget->rect = *rect;

    return CUBE_OK;
}


int32_t fossil_io_cube_widget_set_property(
    fossil_io_cube_widget_t *widget,
    const char *property,
    const char *value
)
{
    size_t i;

    if (!widget || !property || !value)
        return CUBE_INVALID;

    for (i = 0; i < widget->property_count; ++i)
    {
        if (strcmp(
                widget->properties[i].id,
                property
            ) == 0)
        {
            return cube_string_copy(
                widget->properties[i].value,
                sizeof(widget->properties[i].value),
                value
            );
        }
    }

    if (widget->property_count >= 64)
        return CUBE_FULL;

    cube_string_copy(
        widget->properties[
            widget->property_count
        ].id,
        sizeof(widget->properties[0].id),
        property
    );

    cube_string_copy(
        widget->properties[
            widget->property_count
        ].value,
        sizeof(widget->properties[0].value),
        value
    );

    widget->property_count++;

    return CUBE_OK;
}


int32_t fossil_io_cube_widget_get_property(
    fossil_io_cube_widget_t *widget,
    const char *property,
    char *value,
    size_t size
)
{
    size_t i;

    if (!widget || !property || !value || size == 0)
        return CUBE_INVALID;

    for (i = 0; i < widget->property_count; ++i)
    {
        if (strcmp(
                widget->properties[i].id,
                property
            ) == 0)
        {
            return cube_string_copy(
                value,
                size,
                widget->properties[i].value
            );
        }
    }

    value[0] = '\0';

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_widget_set_event_callback(
    fossil_io_cube_widget_t *widget,
    fossil_io_cube_event_callback_t callback,
    void *user_data
)
{
    if (!widget)
        return CUBE_INVALID;

    widget->event_callback = callback;
    widget->event_user_data = user_data;

    return CUBE_OK;
}


int32_t fossil_io_cube_widget_set_render_callback(
    fossil_io_cube_widget_t *widget,
    fossil_io_cube_render_callback_t callback,
    void *user_data
)
{
    if (!widget)
        return CUBE_INVALID;

    widget->render_callback = callback;
    widget->render_user_data = user_data;

    return CUBE_OK;
}


int32_t fossil_io_cube_widget_focus(
    fossil_io_cube_t *cube,
    const char *id
)
{
    fossil_io_cube_widget_t *widget;

    if (!cube || !id)
        return CUBE_INVALID;

    widget = cube_find_widget(cube, id);

    if (!widget)
        return CUBE_NOT_FOUND;

    if (cube->focused)
        cube->focused->focused = 0;

    widget->focused = 1;
    cube->focused = widget;

    cube_string_copy(
        cube->active,
        sizeof(cube->active),
        widget->id
    );

    return CUBE_OK;
}


fossil_io_cube_widget_t *
fossil_io_cube_widget_focused(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return NULL;

    return cube->focused;
}


/* ------------------------------------------------------------
 * Menus
 * ------------------------------------------------------------ */

fossil_io_cube_menu_t *
fossil_io_cube_menu_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
)
{
    fossil_io_cube_menu_t *menu;

    if (!cube || !id || !rect)
        return NULL;

    if (cube_find_menu(cube, id))
        return NULL;

    if (cube->menu_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return NULL;

    menu = calloc(1, sizeof(*menu));

    if (!menu)
        return NULL;

    menu->cube = cube;
    menu->rect = *rect;

    cube_string_copy(
        menu->id,
        sizeof(menu->id),
        id
    );

    cube_string_copy(
        menu->mode,
        sizeof(menu->mode),
        "single"
    );

    cube->menus[
        cube->menu_count++
    ] = menu;

    return menu;
}


fossil_io_cube_menu_t *
fossil_io_cube_menu_find(
    fossil_io_cube_t *cube,
    const char *id
)
{
    return cube_find_menu(cube, id);
}


int32_t fossil_io_cube_menu_destroy(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return CUBE_INVALID;

    for (i = 0; i < cube->menu_count; ++i)
    {
        if (strcmp(cube->menus[i]->id, id) == 0)
        {
            free(cube->menus[i]);

            memmove(
                &cube->menus[i],
                &cube->menus[i + 1],
                (cube->menu_count - i - 1) *
                    sizeof(cube->menus[0])
            );

            cube->menu_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_menu_add_item(
    fossil_io_cube_menu_t *menu,
    const char *id,
    const char *label
)
{
    cube_menu_item_t *item;

    if (!menu || !id || !label)
        return CUBE_INVALID;

    if (menu->item_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    item = &menu->items[
        menu->item_count
    ];

    cube_string_copy(
        item->id,
        sizeof(item->id),
        id
    );

    cube_string_copy(
        item->label,
        sizeof(item->label),
        label
    );

    menu->item_count++;

    return CUBE_OK;
}


int32_t fossil_io_cube_menu_remove_item(
    fossil_io_cube_menu_t *menu,
    const char *id
)
{
    size_t i;

    if (!menu || !id)
        return CUBE_INVALID;

    for (i = 0; i < menu->item_count; ++i)
    {
        if (strcmp(menu->items[i].id, id) == 0)
        {
            memmove(
                &menu->items[i],
                &menu->items[i + 1],
                (menu->item_count - i - 1) *
                    sizeof(menu->items[0])
            );

            menu->item_count--;

            if (strcmp(menu->selected, id) == 0)
                menu->selected[0] = '\0';

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_menu_select(
    fossil_io_cube_menu_t *menu,
    const char *id
)
{
    size_t i;

    if (!menu || !id)
        return CUBE_INVALID;

    for (i = 0; i < menu->item_count; ++i)
    {
        if (strcmp(menu->items[i].id, id) == 0)
        {
            menu->selected_index = i;

            return cube_string_copy(
                menu->selected,
                sizeof(menu->selected),
                id
            );
        }
    }

    return CUBE_NOT_FOUND;
}


const char *
fossil_io_cube_menu_selected(
    fossil_io_cube_menu_t *menu
)
{
    if (!menu || !menu->selected[0])
        return NULL;

    return menu->selected;
}


int32_t fossil_io_cube_menu_set_mode(
    fossil_io_cube_menu_t *menu,
    const char *mode
)
{
    if (!menu || !mode)
        return CUBE_INVALID;

    return cube_string_copy(
        menu->mode,
        sizeof(menu->mode),
        mode
    );
}


int32_t fossil_io_cube_menu_move(
    fossil_io_cube_menu_t *menu,
    const char *direction
)
{
    size_t index;

    if (!menu || !direction)
        return CUBE_INVALID;

    if (menu->item_count == 0)
        return CUBE_NOT_FOUND;

    index = menu->selected_index;

    if (strcmp(direction, "up") == 0)
    {
        if (index > 0)
            index--;
    }
    else if (strcmp(direction, "down") == 0)
    {
        if (index + 1 < menu->item_count)
            index++;
    }
    else if (strcmp(direction, "first") == 0)
    {
        index = 0;
    }
    else if (strcmp(direction, "last") == 0)
    {
        index = menu->item_count - 1;
    }
    else if (strcmp(direction, "page_up") == 0)
    {
        if (index > 10)
            index -= 10;
        else
            index = 0;
    }
    else if (strcmp(direction, "page_down") == 0)
    {
        index += 10;

        if (index >= menu->item_count)
            index = menu->item_count - 1;
    }
    else
    {
        return CUBE_INVALID;
    }

    menu->selected_index = index;

    return fossil_io_cube_menu_select(
        menu,
        menu->items[index].id
    );
}


/* ------------------------------------------------------------
 * Layout
 * ------------------------------------------------------------ */

fossil_io_cube_layout_t *
fossil_io_cube_layout_create(
    fossil_io_cube_t *cube,
    const char *id,
    const char *type
)
{
    fossil_io_cube_layout_t *layout;

    if (!cube || !id || !type)
        return NULL;

    if (cube_find_layout(cube, id))
        return NULL;

    if (cube->layout_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return NULL;

    layout = calloc(1, sizeof(*layout));

    if (!layout)
        return NULL;

    layout->cube = cube;

    cube_string_copy(
        layout->id,
        sizeof(layout->id),
        id
    );

    cube_string_copy(
        layout->type,
        sizeof(layout->type),
        type
    );

    cube->layouts[
        cube->layout_count++
    ] = layout;

    return layout;
}


fossil_io_cube_layout_t *
fossil_io_cube_layout_find(
    fossil_io_cube_t *cube,
    const char *id
)
{
    return cube_find_layout(cube, id);
}


int32_t fossil_io_cube_layout_destroy(
    fossil_io_cube_t *cube,
    const char *id
)
{
    size_t i;

    if (!cube || !id)
        return CUBE_INVALID;

    for (i = 0; i < cube->layout_count; ++i)
    {
        if (strcmp(cube->layouts[i]->id, id) == 0)
        {
            free(cube->layouts[i]);

            memmove(
                &cube->layouts[i],
                &cube->layouts[i + 1],
                (cube->layout_count - i - 1) *
                    sizeof(cube->layouts[0])
            );

            cube->layout_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


int32_t fossil_io_cube_layout_add_widget(
    fossil_io_cube_layout_t *layout,
    fossil_io_cube_widget_t *widget
)
{
    if (!layout || !widget)
        return CUBE_INVALID;

    if (layout->widget_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    layout->widgets[
        layout->widget_count++
    ] = widget;

    return CUBE_OK;
}


int32_t fossil_io_cube_layout_apply(
    fossil_io_cube_layout_t *layout
)
{
    size_t i;
    size_t count;

    if (!layout)
        return CUBE_INVALID;

    count = layout->widget_count;

    if (count == 0)
        return CUBE_OK;

    if (strcmp(layout->type, "horizontal") == 0)
    {
        int32_t width =
            layout->cube->terminal.width /
            (int32_t)count;

        for (i = 0; i < count; ++i)
        {
            layout->widgets[i]->rect.x =
                (int32_t)i * width;

            layout->widgets[i]->rect.y = 0;
            layout->widgets[i]->rect.width = width;
            layout->widgets[i]->rect.height =
                layout->cube->terminal.height;
        }
    }
    else if (strcmp(layout->type, "vertical") == 0)
    {
        int32_t height =
            layout->cube->terminal.height /
            (int32_t)count;

        for (i = 0; i < count; ++i)
        {
            layout->widgets[i]->rect.x = 0;

            layout->widgets[i]->rect.y =
                (int32_t)i * height;

            layout->widgets[i]->rect.width =
                layout->cube->terminal.width;

            layout->widgets[i]->rect.height =
                height;
        }
    }
    else if (strcmp(layout->type, "center") == 0)
    {
        for (i = 0; i < count; ++i)
        {
            fossil_io_cube_widget_t *widget =
                layout->widgets[i];

            widget->rect.x =
                (layout->cube->terminal.width -
                 widget->rect.width) / 2;

            widget->rect.y =
                (layout->cube->terminal.height -
                 widget->rect.height) / 2;
        }
    }

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Actions
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_action_register(
    fossil_io_cube_t *cube,
    const char *id,
    fossil_io_cube_action_callback_t callback,
    void *user_data
)
{
    cube_action_t *action;

    if (!cube || !id || !callback)
        return CUBE_INVALID;

    if (cube_find_action(cube, id))
        return CUBE_EXISTS;

    if (cube->action_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    action = &cube->actions[
        cube->action_count++
    ];

    cube_string_copy(
        action->id,
        sizeof(action->id),
        id
    );

    action->callback = callback;
    action->user_data = user_data;

    return CUBE_OK;
}


int32_t fossil_io_cube_action_execute(
    fossil_io_cube_t *cube,
    const char *id
)
{
    cube_action_t *action;

    if (!cube || !id)
        return CUBE_INVALID;

    action = cube_find_action(cube, id);

    if (!action)
        return CUBE_NOT_FOUND;

    return action->callback(
        cube,
        id,
        action->user_data
    );
}


int32_t fossil_io_cube_action_bind(
    fossil_io_cube_t *cube,
    const char *key,
    const char *action
)
{
    size_t i;

    if (!cube || !key || !action)
        return CUBE_INVALID;

    if (!cube_find_action(cube, action))
        return CUBE_NOT_FOUND;

    for (i = 0; i < cube->binding_count; ++i)
    {
        if (strcmp(
                cube->bindings[i].key,
                key
            ) == 0)
        {
            cube_string_copy(
                cube->bindings[i].action,
                sizeof(cube->bindings[i].action),
                action
            );

            return CUBE_OK;
        }
    }

    if (cube->binding_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    cube_string_copy(
        cube->bindings[
            cube->binding_count
        ].key,
        sizeof(cube->bindings[0].key),
        key
    );

    cube_string_copy(
        cube->bindings[
            cube->binding_count
        ].action,
        sizeof(cube->bindings[0].action),
        action
    );

    cube->binding_count++;

    return CUBE_OK;
}


int32_t fossil_io_cube_action_unbind(
    fossil_io_cube_t *cube,
    const char *key
)
{
    size_t i;

    if (!cube || !key)
        return CUBE_INVALID;

    for (i = 0; i < cube->binding_count; ++i)
    {
        if (strcmp(
                cube->bindings[i].key,
                key
            ) == 0)
        {
            memmove(
                &cube->bindings[i],
                &cube->bindings[i + 1],
                (cube->binding_count - i - 1) *
                    sizeof(cube->bindings[0])
            );

            cube->binding_count--;

            return CUBE_OK;
        }
    }

    return CUBE_NOT_FOUND;
}


/* ------------------------------------------------------------
 * Input
 * ------------------------------------------------------------ */

#if !defined(_WIN32)

static struct termios cube_old_terminal;
static int cube_terminal_saved = 0;

static void cube_terminal_raw_enable(void)
{
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, &cube_old_terminal) != 0)
        return;

    raw = cube_old_terminal;

    raw.c_lflag &=
        (tcflag_t)~(ICANON | ECHO);

    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(
        STDIN_FILENO,
        TCSANOW,
        &raw
    );

    cube_terminal_saved = 1;
}


static void cube_terminal_raw_disable(void)
{
    if (!cube_terminal_saved)
        return;

    tcsetattr(
        STDIN_FILENO,
        TCSANOW,
        &cube_old_terminal
    );

    cube_terminal_saved = 0;
}

#endif


int32_t fossil_io_cube_input(
    fossil_io_cube_t *cube,
    fossil_io_cube_event_t *event
)
{
    int c;

    if (!cube || !event)
        return CUBE_INVALID;

    memset(
        event,
        0,
        sizeof(*event)
    );

#if defined(_WIN32)

    c = _getch();

#else

    cube_terminal_raw_enable();

    c = getchar();

#endif

    if (c == EOF)
        return CUBE_ERROR;

    cube_string_copy(
        event->type,
        sizeof(event->type),
        "key"
    );

    if (c == '\n' || c == '\r')
    {
        cube_string_copy(
            event->key,
            sizeof(event->key),
            "enter"
        );
    }
    else if (c == 27)
    {
        cube_string_copy(
            event->key,
            sizeof(event->key),
            "escape"
        );
    }
    else if (c == '\t')
    {
        cube_string_copy(
            event->key,
            sizeof(event->key),
            "tab"
        );
    }
    else if (c == 127 || c == 8)
    {
        cube_string_copy(
            event->key,
            sizeof(event->key),
            "backspace"
        );
    }
    else if (isprint((unsigned char)c))
    {
        event->text[0] = (char)c;
        event->text[1] = '\0';

        event->key[0] = (char)c;
        event->key[1] = '\0';

        cube_string_copy(
            event->type,
            sizeof(event->type),
            "text"
        );
    }
    else
    {
        event->key[0] = (char)c;
        event->key[1] = '\0';
    }

#if !defined(_WIN32)
    cube_terminal_raw_disable();
#endif

    return CUBE_OK;
}


int32_t fossil_io_cube_event(
    fossil_io_cube_t *cube,
    const fossil_io_cube_event_t *event
)
{
    size_t i;

    if (!cube || !event)
        return CUBE_INVALID;

    /*
     * Key bindings have priority.
     */
    if (event->key[0])
    {
        for (i = 0; i < cube->binding_count; ++i)
        {
            if (strcmp(
                    cube->bindings[i].key,
                    event->key
                ) == 0)
            {
                return fossil_io_cube_action_execute(
                    cube,
                    cube->bindings[i].action
                );
            }
        }
    }

    /*
     * Deliver events to the focused widget.
     */
    if (cube->focused &&
        cube->focused->enabled &&
        cube->focused->event_callback)
    {
        int32_t result =
            cube->focused->event_callback(
                cube->focused,
                event,
                cube->focused->event_user_data
            );

        if (result != 0)
            return result;
    }

    /*
     * Basic built-in focus handling.
     */
    if (strcmp(event->key, "tab") == 0)
    {
        return fossil_io_cube_focus_move(
            cube,
            "next"
        );
    }

    return CUBE_OK;
}


int32_t fossil_io_cube_poll(
    fossil_io_cube_t *cube
)
{
    fossil_io_cube_event_t event;

    if (!cube)
        return CUBE_INVALID;

    if (fossil_io_cube_input(
            cube,
            &event
        ) < 0)
    {
        return CUBE_ERROR;
    }

    return fossil_io_cube_event(
        cube,
        &event
    );
}


/* ------------------------------------------------------------
 * Main Loop
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_update(
    fossil_io_cube_t *cube
)
{
    int32_t result;

    if (!cube)
        return CUBE_INVALID;

    result = fossil_io_cube_poll(cube);

    if (result < 0)
        return result;

    result = fossil_io_cube_begin(cube);

    if (result < 0)
        return result;

    result = fossil_io_cube_render(cube);

    if (result < 0)
        return result;

    return fossil_io_cube_end(cube);
}


int32_t fossil_io_cube_run(
    fossil_io_cube_t *cube
)
{
    int32_t result;

    if (!cube)
        return CUBE_INVALID;

    cube->running = 1;

    while (cube->running)
    {
        result = fossil_io_cube_update(cube);

        if (result < 0)
        {
            cube->running = 0;
            return result;
        }
    }

    return CUBE_OK;
}


int32_t fossil_io_cube_quit(
    fossil_io_cube_t *cube,
    const char *reason
)
{
    if (!cube)
        return CUBE_INVALID;

    cube->running = 0;

    if (reason)
    {
        cube_string_copy(
            cube->quit_reason,
            sizeof(cube->quit_reason),
            reason
        );
    }

    return CUBE_OK;
}


int32_t fossil_io_cube_running(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return CUBE_INVALID;

    return cube->running ? 1 : 0;
}


/* ------------------------------------------------------------
 * Focus
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_focus_move(
    fossil_io_cube_t *cube,
    const char *direction
)
{
    size_t current = 0;
    size_t i;
    size_t target;

    if (!cube || !direction)
        return CUBE_INVALID;

    if (cube->widget_count == 0)
        return CUBE_NOT_FOUND;

    if (cube->focused)
    {
        for (i = 0; i < cube->widget_count; ++i)
        {
            if (cube->widgets[i] == cube->focused)
            {
                current = i;
                break;
            }
        }
    }

    target = current;

    if (strcmp(direction, "next") == 0)
    {
        target =
            (current + 1) %
            cube->widget_count;
    }
    else if (strcmp(direction, "previous") == 0)
    {
        if (current == 0)
            target = cube->widget_count - 1;
        else
            target = current - 1;
    }
    else if (strcmp(direction, "first") == 0)
    {
        target = 0;
    }
    else if (strcmp(direction, "last") == 0)
    {
        target = cube->widget_count - 1;
    }
    else
    {
        return CUBE_INVALID;
    }

    return fossil_io_cube_widget_focus(
        cube,
        cube->widgets[target]->id
    );
}


int32_t fossil_io_cube_focus_order(
    fossil_io_cube_widget_t *widget,
    int32_t order
)
{
    if (!widget)
        return CUBE_INVALID;

    widget->focus_index = order;

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Scrolling
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_scroll(
    fossil_io_cube_widget_t *widget,
    const char *direction,
    int32_t amount
)
{
    char value[64];

    if (!widget || !direction)
        return CUBE_INVALID;

    snprintf(
        value,
        sizeof(value),
        "%d",
        amount
    );

    fossil_io_cube_widget_set_property(
        widget,
        "scroll_direction",
        direction
    );

    fossil_io_cube_widget_set_property(
        widget,
        "scroll_amount",
        value
    );

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Styling
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_style_register(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_style_t *style
)
{
    size_t i;

    if (!cube || !id || !style)
        return CUBE_INVALID;

    for (i = 0; i < cube->style_count; ++i)
    {
        if (strcmp(
                cube->styles[i].id,
                id
            ) == 0)
        {
            cube->styles[i].style = *style;
            return CUBE_OK;
        }
    }

    if (cube->style_count >=
        FOSSIL_IO_CUBE_MAX_CHILDREN)
        return CUBE_FULL;

    cube_string_copy(
        cube->styles[
            cube->style_count
        ].id,
        sizeof(cube->styles[0].id),
        id
    );

    cube->styles[
        cube->style_count
    ].style = *style;

    cube->style_count++;

    return CUBE_OK;
}


int32_t fossil_io_cube_style_apply(
    fossil_io_cube_widget_t *widget,
    const char *style
)
{
    if (!widget || !style)
        return CUBE_INVALID;

    return cube_string_copy(
        widget->style,
        sizeof(widget->style),
        style
    );
}


int32_t fossil_io_cube_window_style(
    fossil_io_cube_window_t *window,
    const char *style
)
{
    return fossil_io_cube_window_set_style(
        window,
        style
    );
}


/* ------------------------------------------------------------
 * Text Input
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_input_set(
    fossil_io_cube_widget_t *widget,
    const char *text
)
{
    int32_t result;

    if (!widget || !text)
        return CUBE_INVALID;

    result = fossil_io_cube_widget_set_text(
        widget,
        text
    );

    if (result == CUBE_OK)
        widget->input_cursor = strlen(text);

    return result;
}


int32_t fossil_io_cube_input_get(
    fossil_io_cube_widget_t *widget,
    char *buffer,
    size_t size
)
{
    if (!widget || !buffer || size == 0)
        return CUBE_INVALID;

    return cube_string_copy(
        buffer,
        size,
        widget->text
    );
}


int32_t fossil_io_cube_input_cursor(
    fossil_io_cube_widget_t *widget,
    size_t position
)
{
    if (!widget)
        return CUBE_INVALID;

    if (position > strlen(widget->text))
        return CUBE_INVALID;

    widget->input_cursor = position;

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Message / Dialog
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_message(
    fossil_io_cube_t *cube,
    const char *title,
    const char *message,
    const char *style
)
{
    fossil_io_cube_rect_t rect;

    if (!cube || !title || !message)
        return CUBE_INVALID;

    rect.width = 60;
    rect.height = 7;

    rect.x =
        (cube->terminal.width - rect.width) / 2;

    rect.y =
        (cube->terminal.height - rect.height) / 2;

    fossil_io_cube_draw_border(
        cube,
        &rect,
        style
    );

    fossil_io_cube_draw_text(
        cube,
        rect.x + 2,
        rect.y + 1,
        title
    );

    fossil_io_cube_draw_text(
        cube,
        rect.x + 2,
        rect.y + 3,
        message
    );

    cube_ansi_flush(cube);

    return CUBE_OK;
}


int32_t fossil_io_cube_confirm(
    fossil_io_cube_t *cube,
    const char *title,
    const char *message,
    char *result,
    size_t result_size
)
{
    fossil_io_cube_event_t event;

    if (!cube ||
        !title ||
        !message ||
        !result ||
        result_size == 0)
    {
        return CUBE_INVALID;
    }

    fossil_io_cube_message(
        cube,
        title,
        message,
        NULL
    );

    fossil_io_cube_draw_text(
        cube,
        2,
        cube->terminal.height - 2,
        "[y]es  [n]o  [c]ancel"
    );

    cube_ansi_flush(cube);

    if (fossil_io_cube_input(
            cube,
            &event
        ) < 0)
    {
        return CUBE_ERROR;
    }

    if (event.key[0] == 'y' ||
        event.key[0] == 'Y')
    {
        cube_string_copy(
            result,
            result_size,
            "yes"
        );
    }
    else if (event.key[0] == 'n' ||
             event.key[0] == 'N')
    {
        cube_string_copy(
            result,
            result_size,
            "no"
        );
    }
    else
    {
        cube_string_copy(
            result,
            result_size,
            "cancel"
        );
    }

    return CUBE_OK;
}


/* ------------------------------------------------------------
 * Status / Diagnostics
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_status(
    fossil_io_cube_t *cube,
    const char *text
)
{
    if (!cube || !text)
        return CUBE_INVALID;

    return cube_string_copy(
        cube->status,
        sizeof(cube->status),
        text
    );
}


const char *
fossil_io_cube_status_get(
    fossil_io_cube_t *cube
)
{
    if (!cube)
        return NULL;

    return cube->status;
}


const char *
fossil_io_cube_active(
    fossil_io_cube_t *cube
)
{
    if (!cube || !cube->active[0])
        return NULL;

    return cube->active;
}


int32_t fossil_io_cube_activate(
    fossil_io_cube_t *cube,
    const char *id
)
{
    if (!cube || !id)
        return CUBE_INVALID;

    if (cube_find_widget(cube, id) ||
        cube_find_window(cube, id) ||
        cube_find_panel(cube, id) ||
        cube_find_menu(cube, id) ||
        cube_find_layout(cube, id))
    {
        return cube_string_copy(
            cube->active,
            sizeof(cube->active),
            id
        );
    }

    return CUBE_NOT_FOUND;
}


/* ------------------------------------------------------------
 * Utility
 * ------------------------------------------------------------ */

int32_t fossil_io_cube_validate_id(
    const char *id
)
{
    size_t length;

    if (!id || id[0] == '\0')
        return 0;

    length = strlen(id);

    if (length >= FOSSIL_IO_CUBE_MAX_ID)
        return 0;

    return 1;
}


int32_t fossil_io_cube_option_valid(
    const char *value,
    const char *options[]
)
{
    size_t i;

    if (!value || !options)
        return 0;

    for (i = 0; options[i] != NULL; ++i)
    {
        if (strcmp(value, options[i]) == 0)
            return 1;
    }

    return 0;
}
