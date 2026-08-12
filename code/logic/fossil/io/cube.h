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
#ifndef FOSSIL_IO_CUBE_H
#define FOSSIL_IO_CUBE_H

#include <stddef.h>
#include <stdint.h>
#include "output.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ------------------------------------------------------------
 * Core Limits
 * ------------------------------------------------------------ */

#define FOSSIL_IO_CUBE_MAX_ID       128
#define FOSSIL_IO_CUBE_MAX_TEXT     4096
#define FOSSIL_IO_CUBE_MAX_CHILDREN 256
#define FOSSIL_IO_CUBE_MAX_COLUMNS  256
#define FOSSIL_IO_CUBE_MAX_ROWS     256

/* ------------------------------------------------------------
 * Forward Declarations
 * ------------------------------------------------------------ */

typedef struct fossil_io_cube_t fossil_io_cube_t;
typedef struct fossil_io_cube_window_t fossil_io_cube_window_t;
typedef struct fossil_io_cube_widget_t fossil_io_cube_widget_t;
typedef struct fossil_io_cube_menu_t fossil_io_cube_menu_t;
typedef struct fossil_io_cube_panel_t fossil_io_cube_panel_t;
typedef struct fossil_io_cube_layout_t fossil_io_cube_layout_t;

/* ------------------------------------------------------------
 * Geometry
 * ------------------------------------------------------------ */

/**
 * @brief Describes a terminal position.
 */

typedef struct
{
    int32_t x;
    int32_t y;
} fossil_io_cube_position_t;

/**
 * @brief Describes terminal dimensions.
 */

typedef struct
{
    int32_t width;
    int32_t height;
} fossil_io_cube_size_t;

/**
 * @brief Describes a rectangular terminal region.
 */

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} fossil_io_cube_rect_t;

/* ------------------------------------------------------------
 * Color / Style
 * ------------------------------------------------------------ */

/**
 * @brief Describes foreground/background terminal colors.
 *
 * Color names are represented by strings so applications may use
 * standard Fossil IO color names as well as terminal-specific names.
 */

typedef struct
{
    char foreground[FOSSIL_IO_CUBE_MAX_ID];
    char background[FOSSIL_IO_CUBE_MAX_ID];
} fossil_io_cube_color_t;

/**
 * @brief Describes terminal text styling.
 *
 * Attribute names are represented as a string list rather than an enum.
 *
 * Examples:
 *
 *   "bold"
 *   "underline"
 *   "italic"
 *   "reverse"
 *   "bold,underline"
 */

typedef struct
{
    char attributes[FOSSIL_IO_CUBE_MAX_TEXT];
} fossil_io_cube_style_t;

/* ------------------------------------------------------------
 * Cell
 * ------------------------------------------------------------ */

/**
 * @brief Represents a single terminal cell.
 *
 * Cube internally renders terminal interfaces as a grid of cells.
 */

typedef struct
{
    char character;
    fossil_io_cube_color_t color;
    fossil_io_cube_style_t style;
} fossil_io_cube_cell_t;

/* ------------------------------------------------------------
 * Input Event
 * ------------------------------------------------------------ */

/**
 * @brief Represents an input event.
 *
 * Event identifiers are strings.
 *
 * Examples:
 *
 *   "key"
 *   "text"
 *   "mouse"
 *   "resize"
 *   "focus"
 *   "quit"
 *
 * Key identifiers may include:
 *
 *   "up"
 *   "down"
 *   "left"
 *   "right"
 *   "enter"
 *   "escape"
 *   "backspace"
 *   "tab"
 *   "delete"
 *   "home"
 *   "end"
 *   "page_up"
 *   "page_down"
 */

typedef struct
{
    char type[FOSSIL_IO_CUBE_MAX_ID];
    char key[FOSSIL_IO_CUBE_MAX_ID];
    char text[FOSSIL_IO_CUBE_MAX_TEXT];
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} fossil_io_cube_event_t;

/* ------------------------------------------------------------
 * Callback Types
 * ------------------------------------------------------------ */

/**
 * @brief Widget event callback.
 */

typedef int (*fossil_io_cube_event_callback_t)(
    fossil_io_cube_widget_t *widget,
    const fossil_io_cube_event_t *event,
    void *user_data
);

/**
 * @brief Widget rendering callback.
 */

typedef int (*fossil_io_cube_render_callback_t)(
    fossil_io_cube_widget_t *widget,
    void *user_data
);

/**
 * @brief Action callback.
 */
typedef int (*fossil_io_cube_action_callback_t)(
    fossil_io_cube_t *cube,
    const char *action_id,
    void *user_data
);

/* ------------------------------------------------------------
 * Cube Configuration
 * ------------------------------------------------------------ */

/**
 * @brief Configuration used when creating a Cube context.
 *
 * All selectable behavior is represented using string identifiers.
 *
 * Examples:
 *
 *   backend = "ansi"
 *   input   = "terminal"
 *   layout  = "absolute"
 *   theme   = "default"
 */

typedef struct
{
    const char *backend;
    const char *input;
    const char *layout;
    const char *theme;
    fossil_io_filesys_file_t *input_stream;
    fossil_io_filesys_file_t *output_stream;
} fossil_io_cube_config_t;

/* ------------------------------------------------------------
 * Cube Context
 * ------------------------------------------------------------ */

/**
 * @brief Initialize a Cube terminal context.
 *
 * @param cube Cube context to initialize.
 * @param config Configuration to apply.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_init(
    fossil_io_cube_t *cube,
    const fossil_io_cube_config_t *config
);

/**
 * @brief Shut down a Cube terminal context.
 *
 * Restores terminal state and releases resources owned by Cube.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_shutdown(
    fossil_io_cube_t *cube
);

/**
 * @brief Create a new Cube context.
 *
 * @param config Configuration to apply.
 *
 * @return Newly allocated Cube context, or NULL on failure.
 */

fossil_io_cube_t *fossil_io_cube_create(
    const fossil_io_cube_config_t *config
);

/**
 * @brief Destroy a Cube context.
 *
 * @param cube Cube context.
 */

void fossil_io_cube_destroy(
    fossil_io_cube_t *cube
);

/* ------------------------------------------------------------
 * Terminal
 * ------------------------------------------------------------ */

/**
 * @brief Query the current terminal dimensions.
 *
 * @param cube Cube context.
 * @param size Output terminal size.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_terminal_size(
    fossil_io_cube_t *cube,
    fossil_io_cube_size_t *size
);

/**
 * @brief Clear the terminal.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_clear(
    fossil_io_cube_t *cube
);

/**
 * @brief Move the terminal cursor.
 *
 * @param cube Cube context.
 * @param x Horizontal position.
 * @param y Vertical position.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_cursor_move(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y
);

/**
 * @brief Show the terminal cursor.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_cursor_show(
    fossil_io_cube_t *cube
);

/**
 * @brief Hide the terminal cursor.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_cursor_hide(
    fossil_io_cube_t *cube
);

/**
 * @brief Set the terminal title.
 *
 * @param cube Cube context.
 * @param title Terminal title.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_terminal_title(
    fossil_io_cube_t *cube,
    const char *title
);

/* ------------------------------------------------------------
 * Rendering
 * ------------------------------------------------------------ */

/**
 * @brief Begin a rendering frame.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_begin(
    fossil_io_cube_t *cube
);

/**
 * @brief Render the current interface.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_render(
    fossil_io_cube_t *cube
);

/**
 * @brief Finish and flush a rendering frame.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_end(
    fossil_io_cube_t *cube
);

/**
 * @brief Render a string at a terminal position.
 *
 * @param cube Cube context.
 * @param x Horizontal position.
 * @param y Vertical position.
 * @param text Text to render.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_draw_text(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y,
    const char *text
);

/**
 * @brief Render styled text at a terminal position.
 *
 * @param cube Cube context.
 * @param x Horizontal position.
 * @param y Vertical position.
 * @param text Text to render.
 * @param style Style identifier string.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_draw_text_styled(
    fossil_io_cube_t *cube,
    int32_t x,
    int32_t y,
    const char *text,
    const char *style
);

/**
 * @brief Draw a rectangular border.
 *
 * @param cube Cube context.
 * @param rect Rectangle to draw.
 * @param style Border style identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_draw_border(
    fossil_io_cube_t *cube,
    const fossil_io_cube_rect_t *rect,
    const char *style
);

/**
 * @brief Fill a rectangular region.
 *
 * @param cube Cube context.
 * @param rect Rectangle to fill.
 * @param character Fill character.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_fill(
    fossil_io_cube_t *cube,
    const fossil_io_cube_rect_t *rect,
    char character
);

/* ------------------------------------------------------------
 * Windows
 * ------------------------------------------------------------ */

/**
 * @brief Create a named window.
 *
 * @param cube Cube context.
 * @param id String identifier for the window.
 * @param rect Window rectangle.
 *
 * @return Window pointer, or NULL on failure.
 */

fossil_io_cube_window_t *fossil_io_cube_window_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Destroy a window.
 *
 * @param cube Cube context.
 * @param id Window identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_destroy(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Find a window by string identifier.
 *
 * @param cube Cube context.
 * @param id Window identifier.
 *
 * @return Window pointer, or NULL if not found.
 */

fossil_io_cube_window_t *fossil_io_cube_window_find(
    fossil_io_cube_t *cube`
    const char *id
);

/**
 * @brief Set a window rectangle.
 *
 * @param window Window.
 * @param rect New rectangle.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_set_rect(
    fossil_io_cube_window_t *window,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Set the window title.
 *
 * @param window Window.
 * @param title Window title.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_set_title(
    fossil_io_cube_window_t *window,
    const char *title
);

/**
 * @brief Set the window style.
 *
 * @param window Window.
 * @param style Style identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_set_style(
    fossil_io_cube_window_t *window,
    const char *style
);

/**
 * @brief Draw a string inside a window.
 *
 * @param window Window.
 * @param x Relative horizontal position.
 * @param y Relative vertical position.
 * @param text Text to draw.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_draw_text(
    fossil_io_cube_window_t *window,
    int32_t x,
    int32_t y,
    const char *text
);

/**
 * @brief Clear a window.
 *
 * @param window Window.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_clear(
    fossil_io_cube_window_t *window
);

/**
 * @brief Raise a window above other windows.
 *
 * @param window Window.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_raise(
    fossil_io_cube_window_t *window
);

/**
 * @brief Lower a window below other windows.
 *
 * @param window Window.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_lower(
    fossil_io_cube_window_t *window
);

/* ------------------------------------------------------------
 * Panels
 * ------------------------------------------------------------ */

/**
 * @brief Create a named panel.
 *
 * Panels provide a higher-level grouping mechanism over windows.
 *
 * @param cube Cube context.
 * @param id Panel identifier.
 * @param rect Panel rectangle.
 *
 * @return Panel pointer, or NULL on failure.
 */

fossil_io_cube_panel_t *fossil_io_cube_panel_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Find a panel by identifier.
 *
 * @param cube Cube context.
 * @param id Panel identifier.
 *
 * @return Panel pointer, or NULL if not found.
 */

fossil_io_cube_panel_t *fossil_io_cube_panel_find(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Destroy a panel.
 *
 * @param cube Cube context.
 * @param id Panel identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_panel_destroy(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Add a window to a panel.
 *
 * @param panel Panel.
 * @param window Window.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_panel_add_window(
    fossil_io_cube_panel_t *panel,
    fossil_io_cube_window_t *window
);

/* ------------------------------------------------------------
 * Widgets
 * ------------------------------------------------------------ */

/**
 * @brief Create a generic widget.
 *
 * @param cube Cube context.
 * @param id Widget identifier.
 * @param type Widget type identifier.
 * @param rect Widget rectangle.
 *
 * @return Widget pointer, or NULL on failure.
 */

fossil_io_cube_widget_t *fossil_io_cube_widget_create(
    fossil_io_cube_t *cube,
    const char *id,
    const char *type,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Find a widget by string identifier.
 *
 * @param cube Cube context.
 * @param id Widget identifier.
 *
 * @return Widget pointer, or NULL if not found.
 */

fossil_io_cube_widget_t *fossil_io_cube_widget_find(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Destroy a widget.
 *
 * @param cube Cube context.
 * @param id Widget identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_destroy(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Set widget text.
 *
 * @param widget Widget.
 * @param text Widget text.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_set_text(
    fossil_io_cube_widget_t *widget,
    const char *text
);

/**
 * @brief Get widget text.
 *
 * @param widget Widget.
 *
 * @return Widget text, or NULL on failure.
 */

const char *fossil_io_cube_widget_get_text(
    fossil_io_cube_widget_t *widget
);

/**
 * @brief Set widget rectangle.
 *
 * @param widget Widget.
 * @param rect Widget rectangle.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_set_rect(
    fossil_io_cube_widget_t *widget,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Set a widget property.
 *
 * Properties are identified by strings.
 *
 * Examples:
 *
 *   "align"
 *   "padding"
 *   "border"
 *   "readonly"
 *   "wrap"
 *   "scroll"
 *
 * @param widget Widget.
 * @param property Property identifier.
 * @param value Property value.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_set_property(
    fossil_io_cube_widget_t *widget,
    const char *property,
    const char *value
);

/**
 * @brief Get a widget property.
 *
 * @param widget Widget.
 * @param property Property identifier.
 * @param value Output buffer.
 * @param size Output buffer size.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_get_property(
    fossil_io_cube_widget_t *widget,
    const char *property,
    char *value,
    size_t size
);

/**
 * @brief Set a widget event callback.
 *
 * @param widget Widget.
 * @param callback Callback function.
 * @param user_data User data.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_set_event_callback(
    fossil_io_cube_widget_t *widget,
    fossil_io_cube_event_callback_t callback,
    void *user_data
);

/**
 * @brief Set a widget rendering callback.
 *
 * @param widget Widget.
 * @param callback Callback function.
 * @param user_data User data.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_set_render_callback(
    fossil_io_cube_widget_t *widget,
    fossil_io_cube_render_callback_t callback,
    void *user_data
);

/**
 * @brief Focus a widget.
 *
 * @param cube Cube context.
 * @param id Widget identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_widget_focus(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Get the currently focused widget.
 *
 * @param cube Cube context.
 *
 * @return Focused widget, or NULL if none is focused.
 */

fossil_io_cube_widget_t *fossil_io_cube_widget_focused(
    fossil_io_cube_t *cube
);

/* ------------------------------------------------------------
 * Menus
 * ------------------------------------------------------------ */

/**
 * @brief Create a named menu.
 *
 * @param cube Cube context.
 * @param id Menu identifier.
 * @param rect Menu rectangle.
 *
 * @return Menu pointer, or NULL on failure.
 */

fossil_io_cube_menu_t *fossil_io_cube_menu_create(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_rect_t *rect
);

/**
 * @brief Find a menu by identifier.
 *
 * @param cube Cube context.
 * @param id Menu identifier.
 *
 * @return Menu pointer, or NULL if not found.
 */

fossil_io_cube_menu_t *fossil_io_cube_menu_find(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Destroy a menu.
 *
 * @param cube Cube context.
 * @param id Menu identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_destroy(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Add an item to a menu.
 *
 * The item identifier is a string and becomes the canonical selection
 * returned by the menu.
 *
 * @param menu Menu.
 * @param id Item identifier.
 * @param label Display label.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_add_item(
    fossil_io_cube_menu_t *menu,
    const char *id,
    const char *label
);

/**
 * @brief Remove an item from a menu.
 *
 * @param menu Menu.
 * @param id Item identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_remove_item(
    fossil_io_cube_menu_t *menu,
    const char *id
);

/**
 * @brief Select a menu item by string identifier.
 *
 * @param menu Menu.
 * @param id Item identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_select(
    fossil_io_cube_menu_t *menu,
    const char *id
);

/**
 * @brief Retrieve the selected menu item identifier.
 *
 * @param menu Menu.
 *
 * @return Selected item identifier, or NULL if no item is selected.
 */

const char *fossil_io_cube_menu_selected(
    fossil_io_cube_menu_t *menu
);

/**
 * @brief Set the menu selection strategy.
 *
 * Examples:
 *
 *   "single"
 *   "multiple"
 *   "radio"
 *   "checkbox"
 *
 * @param menu Menu.
 * @param mode Selection mode identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_set_mode(
    fossil_io_cube_menu_t *menu,
    const char *mode
);

/**
 * @brief Move menu selection.
 *
 * Examples:
 *
 *   "up"
 *   "down"
 *   "first"
 *   "last"
 *   "page_up"
 *   "page_down"
 *
 * @param menu Menu.
 * @param direction Direction identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_menu_move(
    fossil_io_cube_menu_t *menu,
    const char *direction
);

/* ------------------------------------------------------------
 * Layout
 * ------------------------------------------------------------ */

/**
 * @brief Create a named layout.
 *
 * @param cube Cube context.
 * @param id Layout identifier.
 * @param type Layout type identifier.
 *
 * Examples:
 *
 *   "absolute"
 *   "horizontal"
 *   "vertical"
 *   "grid"
 *   "stack"
 *   "center"
 *
 * @return Layout pointer, or NULL on failure.
 */

fossil_io_cube_layout_t *fossil_io_cube_layout_create(
    fossil_io_cube_t *cube,
    const char *id,
    const char *type
);

/**
 * @brief Find a layout by identifier.
 *
 * @param cube Cube context.
 * @param id Layout identifier.
 *
 * @return Layout pointer, or NULL if not found.
 */

fossil_io_cube_layout_t *fossil_io_cube_layout_find(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Destroy a layout.
 *
 * @param cube Cube context.
 * @param id Layout identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_layout_destroy(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Add a widget to a layout.
 *
 * @param layout Layout.
 * @param widget Widget.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_layout_add_widget(
    fossil_io_cube_layout_t *layout,
    fossil_io_cube_widget_t *widget
);

/**
 * @brief Recalculate a layout.
 *
 * @param layout Layout.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_layout_apply(
    fossil_io_cube_layout_t *layout
);

/* ------------------------------------------------------------
 * Actions
 * ------------------------------------------------------------ */

/**
 * @brief Register an application action.
 *
 * Actions are selected by string identifiers.
 *
 * Examples:
 *
 *   "save"
 *   "quit"
 *   "open"
 *   "refresh"
 *   "help"
 *
 * @param cube Cube context.
 * @param id Action identifier.
 * @param callback Action callback.
 * @param user_data User data.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_action_register(
    fossil_io_cube_t *cube,
    const char *id,
    fossil_io_cube_action_callback_t callback,
    void *user_data
);

/**
 * @brief Execute an action by identifier.
 *
 * @param cube Cube context.
 * @param id Action identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_action_execute(
    fossil_io_cube_t *cube,
    const char *id
);

/**
 * @brief Bind a key to an action.
 *
 * Examples:
 *
 *   key   = "ctrl+s"
 *   action = "save"
 *
 *   key   = "escape"
 *   action = "quit"
 *
 * @param cube Cube context.
 * @param key Key identifier.
 * @param action Action identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_action_bind(
    fossil_io_cube_t *cube,
    const char *key,
    const char *action
);

/**
 * @brief Remove a key binding.
 *
 * @param cube Cube context.
 * @param key Key identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_action_unbind(
    fossil_io_cube_t *cube,
    const char *key
);

/* ------------------------------------------------------------
 * Input
 * ------------------------------------------------------------ */

/**
 * @brief Read the next terminal event.
 *
 * @param cube Cube context.
 * @param event Output event.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_input(
    fossil_io_cube_t *cube,
    fossil_io_cube_event_t *event
);

/**
 * @brief Process an input event.
 *
 * Dispatches the event to the appropriate widget, action, menu,
 * or application callback.
 *
 * @param cube Cube context.
 * @param event Input event.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_event(
    fossil_io_cube_t *cube,
    const fossil_io_cube_event_t *event
);

/**
 * @brief Read and process one terminal event.
 *
 * This combines input acquisition and event dispatch.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_poll(
    fossil_io_cube_t *cube
);

/* ------------------------------------------------------------
 * Main Loop
 * ------------------------------------------------------------ */

/**
 * @brief Run one iteration of the Cube interface.
 *
 * Performs input processing and rendering.
 *
 * @param cube Cube context.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_update(
    fossil_io_cube_t *cube
);

/**
 * @brief Run the Cube application loop.
 *
 * The loop continues until an application action or event terminates
 * the interface.
 *
 * @param cube Cube context.
 *
 * @return 0 on normal termination, negative value on failure.
 */

int32_t fossil_io_cube_run(
    fossil_io_cube_t *cube
);

/**
 * @brief Request termination of the Cube application loop.
 *
 * @param cube Cube context.
 * @param reason String describing the termination reason.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_quit(
    fossil_io_cube_t *cube,
    const char *reason
);

/**
 * @brief Determine whether the Cube application is still running.
 *
 * @param cube Cube context.
 *
 * @return 1 if running, 0 if stopped, negative value on error.
 */

int32_t fossil_io_cube_running(
    fossil_io_cube_t *cube
);

/* ------------------------------------------------------------
 * Focus
 * ------------------------------------------------------------ */

/**
 * @brief Move focus between widgets.
 *
 * Examples:
 *
 *   "next"
 *   "previous"
 *   "first"
 *   "last"
 *
 * @param cube Cube context.
 * @param direction Direction identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_focus_move(
    fossil_io_cube_t *cube,
    const char *direction
);

/**
 * @brief Set the focus order for a widget.
 *
 * @param widget Widget.
 * @param order Focus order.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_focus_order(
    fossil_io_cube_widget_t *widget,
    int32_t order
);

/* ------------------------------------------------------------
 * Scrolling
 * ------------------------------------------------------------ */

/**
 * @brief Scroll a widget.
 *
 * Examples:
 *
 *   "up"
 *   "down"
 *   "left"
 *   "right"
 *   "page_up"
 *   "page_down"
 *   "top"
 *   "bottom"
 *
 * @param widget Widget.
 * @param direction Direction identifier.
 * @param amount Scroll amount.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_scroll(
    fossil_io_cube_widget_t *widget,
    const char *direction,
    int32_t amount
);

/* ------------------------------------------------------------
 * Styling
 * ------------------------------------------------------------ */

/**
 * @brief Register a named style.
 *
 * @param cube Cube context.
 * @param id Style identifier.
 * @param style Style description.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_style_register(
    fossil_io_cube_t *cube,
    const char *id,
    const fossil_io_cube_style_t *style
);

/**
 * @brief Apply a named style to a widget.
 *
 * @param widget Widget.
 * @param style Style identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_style_apply(
    fossil_io_cube_widget_t *widget,
    const char *style
);

/**
 * @brief Apply a named style to a window.
 *
 * @param window Window.
 * @param style Style identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_window_style(
    fossil_io_cube_window_t *window,
    const char *style
);

/* ------------------------------------------------------------
 * Text Input
 * ------------------------------------------------------------ */

/**
 * @brief Set the contents of a text input widget.
 *
 * @param widget Widget.
 * @param text Text.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_input_set(
    fossil_io_cube_widget_t *widget,
    const char *text
);

/**
 * @brief Retrieve text from a text input widget.
 *
 * @param widget Widget.
 * @param buffer Output buffer.
 * @param size Output buffer size.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_input_get(
    fossil_io_cube_widget_t *widget,
    char *buffer,
    size_t size
);

/**
 * @brief Set the cursor position inside a text input widget.
 *
 * @param widget Widget.
 * @param position Text position.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_input_cursor(
    fossil_io_cube_widget_t *widget,
    size_t position
);

/* ------------------------------------------------------------
 * Message / Dialog Support
 * ------------------------------------------------------------ */

/**
 * @brief Display a message dialog.
 *
 * @param cube Cube context.
 * @param title Dialog title.
 * @param message Dialog message.
 * @param style Dialog style identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_message(
    fossil_io_cube_t *cube,
    const char *title,
    const char *message,
    const char *style
);

/**
 * @brief Display a confirmation dialog.
 *
 * The selected result is returned through the supplied string buffer.
 *
 * Example results:
 *
 *   "yes"
 *   "no"
 *   "cancel"
 *
 * @param cube Cube context.
 * @param title Dialog title.
 * @param message Dialog message.
 * @param result Output result buffer.
 * @param result_size Output buffer size.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_confirm(
    fossil_io_cube_t *cube,
    const char *title,
    const char *message,
    char *result,
    size_t result_size
);

/* ------------------------------------------------------------
 * Status / Diagnostics
 * ------------------------------------------------------------ */

/**
 * @brief Set the status line.
 *
 * @param cube Cube context.
 * @param text Status text.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_status(
    fossil_io_cube_t *cube,
    const char *text
);

/**
 * @brief Get the current status line.
 *
 * @param cube Cube context.
 *
 * @return Status text, or NULL on failure.
 */

const char *fossil_io_cube_status_get(
    fossil_io_cube_t *cube
);

/**
 * @brief Get the currently active object.
 *
 * Returns the identifier of the currently focused or active object.
 *
 * @param cube Cube context.
 *
 * @return Active object identifier, or NULL if none exists.
 */

const char *fossil_io_cube_active(
    fossil_io_cube_t *cube
);

/**
 * @brief Set the active object by identifier.
 *
 * @param cube Cube context.
 * @param id Object identifier.
 *
 * @return 0 on success, negative value on failure.
 */

int32_t fossil_io_cube_activate(
    fossil_io_cube_t *cube,
    const char *id
);

/* ------------------------------------------------------------
 * Utility
 * ------------------------------------------------------------ */

/**
 * @brief Check whether a Cube identifier is valid.
 *
 * Cube identifiers must be non-empty and must fit within the Cube
 * identifier limit.
 *
 * @param id Identifier to validate.
 *
 * @return 1 if valid, 0 otherwise.
 */

int32_t fossil_io_cube_validate_id(
    const char *id
);

/**
 * @brief Resolve a string option.
 *
 * This is intended for internal/application-level string option
 * validation without requiring public enums.
 *
 * @param value Option value.
 * @param options Null-terminated array of valid option strings.
 *
 * @return 1 if the option exists, 0 otherwise.
 */

int32_t fossil_io_cube_option_valid(
    const char *value,
    const char *options[]);

#ifdef __cplusplus
}

#include <string>

namespace fossil::io

{

class Cube

{

public:

    Cube() :

        cube_(nullptr)

    {

    }

    explicit Cube(

        const fossil_io_cube_config_t &config

    ) :

        cube_(fossil_io_cube_create(&config))

    {

    }

    ~Cube()

    {

        if (cube_)

        {

            fossil_io_cube_destroy(cube_);

            cube_ = nullptr;

        }

    }

    Cube(const Cube &) = delete;

    Cube &operator=(const Cube &) = delete;

    int32_t init(

        const fossil_io_cube_config_t &config

    )

    {

        return fossil_io_cube_init(

            cube_,

            &config

        );

    }

    int32_t shutdown()

    {

        return fossil_io_cube_shutdown(cube_);

    }

    int32_t terminal_size(

        fossil_io_cube_size_t &size

    )

    {

        return fossil_io_cube_terminal_size(

            cube_,

            &size

        );

    }

    int32_t clear()

    {

        return fossil_io_cube_clear(cube_);

    }

    int32_t cursor_move(

        int32_t x,

        int32_t y

    )

    {

        return fossil_io_cube_cursor_move(

            cube_,

            x,

            y

        );

    }

    int32_t cursor_show()

    {

        return fossil_io_cube_cursor_show(cube_);

    }

    int32_t cursor_hide()

    {

        return fossil_io_cube_cursor_hide(cube_);

    }

    int32_t terminal_title(

        const std::string &title

    )

    {

        return fossil_io_cube_terminal_title(

            cube_,

            title.c_str()

        );

    }

    int32_t begin()

    {

        return fossil_io_cube_begin(cube_);

    }

    int32_t render()

    {

        return fossil_io_cube_render(cube_);

    }

    int32_t end()

    {

        return fossil_io_cube_end(cube_);

    }

    int32_t draw_text(

        int32_t x,

        int32_t y,

        const std::string &text

    )

    {

        return fossil_io_cube_draw_text(

            cube_,

            x,

            y,

            text.c_str()

        );

    }

    int32_t draw_text_styled(

        int32_t x,

        int32_t y,

        const std::string &text,

        const std::string &style

    )

    {

        return fossil_io_cube_draw_text_styled(

            cube_,

            x,

            y,

            text.c_str(),

            style.c_str()

        );

    }

    int32_t draw_border(

        const fossil_io_cube_rect_t &rect,

        const std::string &style

    )

    {

        return fossil_io_cube_draw_border(

            cube_,

            &rect,

            style.c_str()

        );

    }

    int32_t fill(

        const fossil_io_cube_rect_t &rect,

        char character

    )

    {

        return fossil_io_cube_fill(

            cube_,

            &rect,

            character

        );

    }

    fossil_io_cube_window_t *window_create(

        const std::string &id,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_window_create(

            cube_,

            id.c_str(),

            &rect

        );

    }

    int32_t window_destroy(

        const std::string &id

    )

    {

        return fossil_io_cube_window_destroy(

            cube_,

            id.c_str()

        );

    }

    fossil_io_cube_window_t *window_find(

        const std::string &id

    )

    {

        return fossil_io_cube_window_find(

            cube_,

            id.c_str()

        );

    }

    int32_t window_set_rect(

        fossil_io_cube_window_t *window,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_window_set_rect(

            window,

            &rect

        );

    }

    int32_t window_set_title(

        fossil_io_cube_window_t *window,

        const std::string &title

    )

    {

        return fossil_io_cube_window_set_title(

            window,

            title.c_str()

        );

    }

    int32_t window_set_style(

        fossil_io_cube_window_t *window,

        const std::string &style

    )

    {

        return fossil_io_cube_window_set_style(

            window,

            style.c_str()

        );

    }

    int32_t window_draw_text(

        fossil_io_cube_window_t *window,

        int32_t x,

        int32_t y,

        const std::string &text

    )

    {

        return fossil_io_cube_window_draw_text(

            window,

            x,

            y,

            text.c_str()

        );

    }

    int32_t window_clear(

        fossil_io_cube_window_t *window

    )

    {

        return fossil_io_cube_window_clear(window);

    }

    int32_t window_raise(

        fossil_io_cube_window_t *window

    )

    {

        return fossil_io_cube_window_raise(window);

    }

    int32_t window_lower(

        fossil_io_cube_window_t *window

    )

    {

        return fossil_io_cube_window_lower(window);

    }

    fossil_io_cube_panel_t *panel_create(

        const std::string &id,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_panel_create(

            cube_,

            id.c_str(),

            &rect

        );

    }

    fossil_io_cube_panel_t *panel_find(

        const std::string &id

    )

    {

        return fossil_io_cube_panel_find(

            cube_,

            id.c_str()

        );

    }

    int32_t panel_destroy(

        const std::string &id

    )

    {

        return fossil_io_cube_panel_destroy(

            cube_,

            id.c_str()

        );

    }

    int32_t panel_add_window(

        fossil_io_cube_panel_t *panel,

        fossil_io_cube_window_t *window

    )

    {

        return fossil_io_cube_panel_add_window(

            panel,

            window

        );

    }

    fossil_io_cube_widget_t *widget_create(

        const std::string &id,

        const std::string &type,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_widget_create(

            cube_,

            id.c_str(),

            type.c_str(),

            &rect

        );

    }

    fossil_io_cube_widget_t *widget_find(

        const std::string &id

    )

    {

        return fossil_io_cube_widget_find(

            cube_,

            id.c_str()

        );

    }

    int32_t widget_destroy(

        const std::string &id

    )

    {

        return fossil_io_cube_widget_destroy(

            cube_,

            id.c_str()

        );

    }

    int32_t widget_set_text(

        fossil_io_cube_widget_t *widget,

        const std::string &text

    )

    {

        return fossil_io_cube_widget_set_text(

            widget,

            text.c_str()

        );

    }

    std::string widget_get_text(

        fossil_io_cube_widget_t *widget

    )

    {

        const char *text =

            fossil_io_cube_widget_get_text(widget);

        return text ? std::string(text) : std::string();

    }

    int32_t widget_set_rect(

        fossil_io_cube_widget_t *widget,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_widget_set_rect(

            widget,

            &rect

        );

    }

    int32_t widget_set_property(

        fossil_io_cube_widget_t *widget,

        const std::string &property,

        const std::string &value

    )

    {

        return fossil_io_cube_widget_set_property(

            widget,

            property.c_str(),

            value.c_str()

        );

    }

    int32_t widget_get_property(

        fossil_io_cube_widget_t *widget,

        const std::string &property,

        std::string &value

    )

    {

        char buffer[FOSSIL_IO_CUBE_MAX_TEXT];

        int32_t result =

            fossil_io_cube_widget_get_property(

                widget,

                property.c_str(),

                buffer,

                sizeof(buffer)

            );

        if (result == 0)

            value = buffer;

        return result;

    }

    int32_t widget_set_event_callback(

        fossil_io_cube_widget_t *widget,

        fossil_io_cube_event_callback_t callback,

        void *user_data

    )

    {

        return fossil_io_cube_widget_set_event_callback(

            widget,

            callback,

            user_data

        );

    }

    int32_t widget_set_render_callback(

        fossil_io_cube_widget_t *widget,

        fossil_io_cube_render_callback_t callback,

        void *user_data

    )

    {

        return fossil_io_cube_widget_set_render_callback(

            widget,

            callback,

            user_data

        );

    }

    int32_t widget_focus(

        const std::string &id

    )

    {

        return fossil_io_cube_widget_focus(

            cube_,

            id.c_str()

        );

    }

    fossil_io_cube_widget_t *widget_focused()

    {

        return fossil_io_cube_widget_focused(cube_);

    }

    fossil_io_cube_menu_t *menu_create(

        const std::string &id,

        const fossil_io_cube_rect_t &rect

    )

    {

        return fossil_io_cube_menu_create(

            cube_,

            id.c_str(),

            &rect

        );

    }

    fossil_io_cube_menu_t *menu_find(

        const std::string &id

    )

    {

        return fossil_io_cube_menu_find(

            cube_,

            id.c_str()

        );

    }

    int32_t menu_destroy(

        const std::string &id

    )

    {

        return fossil_io_cube_menu_destroy(

            cube_,

            id.c_str()

        );

    }

    int32_t menu_add_item(

        fossil_io_cube_menu_t *menu,

        const std::string &id,

        const std::string &label

    )

    {

        return fossil_io_cube_menu_add_item(

            menu,

            id.c_str(),

            label.c_str()

        );

    }

    int32_t menu_remove_item(

        fossil_io_cube_menu_t *menu,

        const std::string &id

    )

    {

        return fossil_io_cube_menu_remove_item(

            menu,

            id.c_str()

        );

    }

    int32_t menu_select(

        fossil_io_cube_menu_t *menu,

        const std::string &id

    )

    {

        return fossil_io_cube_menu_select(

            menu,

            id.c_str()

        );

    }

    std::string menu_selected(

        fossil_io_cube_menu_t *menu

    )

    {

        const char *id =

            fossil_io_cube_menu_selected(menu);

        return id ? std::string(id) : std::string();

    }

    int32_t menu_set_mode(

        fossil_io_cube_menu_t *menu,

        const std::string &mode

    )

    {

        return fossil_io_cube_menu_set_mode(

            menu,

            mode.c_str()

        );

    }

    int32_t menu_move(

        fossil_io_cube_menu_t *menu,

        const std::string &direction

    )

    {

        return fossil_io_cube_menu_move(

            menu,

            direction.c_str()

        );

    }

    fossil_io_cube_layout_t *layout_create(

        const std::string &id,

        const std::string &type

    )

    {

        return fossil_io_cube_layout_create(

            cube_,

            id.c_str(),

            type.c_str()

        );

    }

    fossil_io_cube_layout_t *layout_find(

        const std::string &id

    )

    {

        return fossil_io_cube_layout_find(

            cube_,

            id.c_str()

        );

    }

    int32_t layout_destroy(

        const std::string &id

    )

    {

        return fossil_io_cube_layout_destroy(

            cube_,

            id.c_str()

        );

    }

    int32_t layout_add_widget(

        fossil_io_cube_layout_t *layout,

        fossil_io_cube_widget_t *widget

    )

    {

        return fossil_io_cube_layout_add_widget(

            layout,

            widget

        );

    }

    int32_t layout_apply(

        fossil_io_cube_layout_t *layout

    )

    {

        return fossil_io_cube_layout_apply(layout);

    }

    int32_t action_register(

        const std::string &id,

        fossil_io_cube_action_callback_t callback,

        void *user_data

    )

    {

        return fossil_io_cube_action_register(

            cube_,

            id.c_str(),

            callback,

            user_data

        );

    }

    int32_t action_execute(

        const std::string &id

    )

    {

        return fossil_io_cube_action_execute(

            cube_,

            id.c_str()

        );

    }

    int32_t action_bind(

        const std::string &key,

        const std::string &action

    )

    {

        return fossil_io_cube_action_bind(

            cube_,

            key.c_str(),

            action.c_str()

        );

    }

    int32_t action_unbind(

        const std::string &key

    )

    {

        return fossil_io_cube_action_unbind(

            cube_,

            key.c_str()

        );

    }

    int32_t input(

        fossil_io_cube_event_t &event

    )

    {

        return fossil_io_cube_input(

            cube_,

            &event

        );

    }

    int32_t event(

        const fossil_io_cube_event_t &event

    )

    {

        return fossil_io_cube_event(

            cube_,

            &event

        );

    }

    int32_t poll()

    {

        return fossil_io_cube_poll(cube_);

    }

    int32_t update()

    {

        return fossil_io_cube_update(cube_);

    }

    int32_t run()

    {

        return fossil_io_cube_run(cube_);

    }

    int32_t quit(

        const std::string &reason

    )

    {

        return fossil_io_cube_quit(

            cube_,

            reason.c_str()

        );

    }

    int32_t running()

    {

        return fossil_io_cube_running(cube_);

    }

    int32_t focus_move(

        const std::string &direction

    )

    {

        return fossil_io_cube_focus_move(

            cube_,

            direction.c_str()

        );

    }

    int32_t focus_order(

        fossil_io_cube_widget_t *widget,

        int32_t order

    )

    {

        return fossil_io_cube_focus_order(

            widget,

            order

        );

    }

    int32_t scroll(

        fossil_io_cube_widget_t *widget,

        const std::string &direction,

        int32_t amount

    )

    {

        return fossil_io_cube_scroll(

            widget,

            direction.c_str(),

            amount

        );

    }

    int32_t style_register(

        const std::string &id,

        const fossil_io_cube_style_t &style

    )

    {

        return fossil_io_cube_style_register(

            cube_,

            id.c_str(),

            &style

        );

    }

    int32_t style_apply(

        fossil_io_cube_widget_t *widget,

        const std::string &style

    )

    {

        return fossil_io_cube_style_apply(

            widget,

            style.c_str()

        );

    }

    int32_t window_style(

        fossil_io_cube_window_t *window,

        const std::string &style

    )

    {

        return fossil_io_cube_window_style(

            window,

            style.c_str()

        );

    }

    int32_t input_set(

        fossil_io_cube_widget_t *widget,

        const std::string &text

    )

    {

        return fossil_io_cube_input_set(

            widget,

            text.c_str()

        );

    }

    int32_t input_get(

        fossil_io_cube_widget_t *widget,

        std::string &text

    )

    {

        char buffer[FOSSIL_IO_CUBE_MAX_TEXT];

        int32_t result =

            fossil_io_cube_input_get(

                widget,

                buffer,

                sizeof(buffer)

            );

        if (result == 0)

            text = buffer;

        return result;

    }

    int32_t input_cursor(

        fossil_io_cube_widget_t *widget,

        size_t position

    )

    {

        return fossil_io_cube_input_cursor(

            widget,

            position

        );

    }

    int32_t message(

        const std::string &title,

        const std::string &message,

        const std::string &style

    )

    {

        return fossil_io_cube_message(

            cube_,

            title.c_str(),

            message.c_str(),

            style.c_str()

        );

    }

    int32_t confirm(

        const std::string &title,

        const std::string &message,

        std::string &result

    )

    {

        char buffer[FOSSIL_IO_CUBE_MAX_ID];

        int32_t status =

            fossil_io_cube_confirm(

                cube_,

                title.c_str(),

                message.c_str(),

                buffer,

                sizeof(buffer)

            );

        if (status == 0)

            result = buffer;

        return status;

    }

    int32_t status(

        const std::string &text

    )

    {

        return fossil_io_cube_status(

            cube_,

            text.c_str()

        );

    }

    std::string status_get() const

    {

        const char *text =

            fossil_io_cube_status_get(cube_);

        return text ? std::string(text) : std::string();

    }

    std::string active() const

    {

        const char *id =

            fossil_io_cube_active(cube_);

        return id ? std::string(id) : std::string();

    }

    int32_t activate(

        const std::string &id

    )

    {

        return fossil_io_cube_activate(

            cube_,

            id.c_str()

        );

    }

    static int32_t validate_id(

        const std::string &id

    )

    {

        return fossil_io_cube_validate_id(

            id.c_str()

        );

    }

    static int32_t option_valid(

        const std::string &value,

        const char *options[]

    )

    {

        return fossil_io_cube_option_valid(

            value.c_str(),

            options

        );

    }

    fossil_io_cube_t *native_handle()

    {

        return cube_;

    }

private:

    fossil_io_cube_t *cube_;

};

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
