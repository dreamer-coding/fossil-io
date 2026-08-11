/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * Cube TUI test suite.
 * -----------------------------------------------------------------------------
 */

#include <fossil/maip/framework.h>
#include <string.h>

#include "fossil/io/framework.h"
#include "fossil/io/cube.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(c_cube_suite);

FOSSIL_SETUP(c_cube_suite)
{
}

FOSSIL_TEARDOWN(c_cube_suite)
{
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Cube Configuration / Lifecycle
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_create_destroy)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube = fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_init_shutdown)
{
    fossil_io_cube_t *cube = NULL;

    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    /*
     * This test assumes the Cube implementation permits a caller
     * to provide storage for the opaque context. If Cube remains
     * fully opaque, use fossil_io_cube_create() instead.
     */
    cube = fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_shutdown(cube)
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Terminal
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_terminal_size)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_size_t size;

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_terminal_size(cube, &size)
    );

    ASSUME_ITS_TRUE(size.width >= 0);
    ASSUME_ITS_TRUE(size.height >= 0);

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_clear)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cube_clear(cube));

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_cursor)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_cursor_move(cube, 0, 0)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_cursor_hide(cube)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_cursor_show(cube)
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_terminal_title)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_terminal_title(
            cube,
            "Fossil Cube Test"
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Rendering
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_render_frame)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_begin(cube)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_render(cube)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_end(cube)
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_draw_text)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_draw_text(
            cube,
            0,
            0,
            "Fossil Cube"
        )
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_draw_text_styled)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_draw_text_styled(
            cube,
            0,
            0,
            "Fossil Cube",
            "bold"
        )
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_draw_border)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        20,
        5
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_draw_border(
            cube,
            &rect,
            "single"
        )
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_fill)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        10,
        3
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_fill(
            cube,
            &rect,
            ' '
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Windows
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_window_create_find_destroy)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        1,
        1,
        30,
        10
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_window_t *window =
        fossil_io_cube_window_create(
            cube,
            "main",
            &rect
        );

    ASSUME_ITS_TRUE(window != NULL);

    ASSUME_ITS_TRUE(
        fossil_io_cube_window_find(
            cube,
            "main"
        ) == window
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_window_set_title(
            window,
            "Main Window"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_window_draw_text(
            window,
            1,
            1,
            "Hello Cube"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_window_clear(window)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_window_destroy(
            cube,
            "main"
        )
    );

    ASSUME_ITS_TRUE(
        fossil_io_cube_window_find(
            cube,
            "main"
        ) == NULL
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Panels
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_panel)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        40,
        15
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_panel_t *panel =
        fossil_io_cube_panel_create(
            cube,
            "main_panel",
            &rect
        );

    ASSUME_ITS_TRUE(panel != NULL);

    fossil_io_cube_window_t *window =
        fossil_io_cube_window_create(
            cube,
            "panel_window",
            &rect
        );

    ASSUME_ITS_TRUE(window != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_panel_add_window(
            panel,
            window
        )
    );

    ASSUME_ITS_TRUE(
        fossil_io_cube_panel_find(
            cube,
            "main_panel"
        ) == panel
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_panel_destroy(
            cube,
            "main_panel"
        )
    );

    fossil_io_cube_window_destroy(
        cube,
        "panel_window"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Widgets
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_widget)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        1,
        1,
        20,
        5
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "label",
            "label",
            &rect
        );

    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_set_text(
            widget,
            "Hello Cube"
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "Hello Cube",
        fossil_io_cube_widget_get_text(widget)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_set_property(
            widget,
            "align",
            "center"
        )
    );

    char property[128];

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_get_property(
            widget,
            "align",
            property,
            sizeof(property)
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "center",
        property
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_focus(
            cube,
            "label"
        )
    );

    ASSUME_ITS_TRUE(
        fossil_io_cube_widget_focused(cube) == widget
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_destroy(
            cube,
            "label"
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Menus
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_menu)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        30,
        10
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_menu_t *menu =
        fossil_io_cube_menu_create(
            cube,
            "file_menu",
            &rect
        );

    ASSUME_ITS_TRUE(menu != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_add_item(
            menu,
            "open",
            "Open"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_add_item(
            menu,
            "save",
            "Save"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_set_mode(
            menu,
            "single"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_select(
            menu,
            "open"
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "open",
        fossil_io_cube_menu_selected(menu)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_move(
            menu,
            "down"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_remove_item(
            menu,
            "open"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_menu_destroy(
            cube,
            "file_menu"
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Layout
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_layout)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        40,
        10
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    fossil_io_cube_layout_t *layout =
        fossil_io_cube_layout_create(
            cube,
            "main_layout",
            "vertical"
        );

    ASSUME_ITS_TRUE(layout != NULL);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "widget",
            "label",
            &rect
        );

    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_layout_add_widget(
            layout,
            widget
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_layout_apply(layout)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_layout_destroy(
            cube,
            "main_layout"
        )
    );

    fossil_io_cube_widget_destroy(
        cube,
        "widget"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Actions
// * * * * * * * * * * * * * * * * * * * * * * * *

static int cube_test_action(
    fossil_io_cube_t *cube,
    const char *action_id,
    void *user_data
)
{
    (void)cube;
    (void)action_id;

    int *called = (int *)user_data;

    if (called)
    {
        (*called)++;
    }

    return 0;
}

FOSSIL_TEST(c_test_cube_actions)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    int called = 0;

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_action_register(
            cube,
            "save",
            cube_test_action,
            &called
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_action_execute(
            cube,
            "save"
        )
    );

    ASSUME_ITS_EQUAL_I32(1, called);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_action_bind(
            cube,
            "ctrl+s",
            "save"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_action_unbind(
            cube,
            "ctrl+s"
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Input / Events
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_event)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_event_t event;

    memset(&event, 0, sizeof(event));

    strcpy(event.type, "key");
    strcpy(event.key, "enter");

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_event(
            cube,
            &event
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Main Loop / Focus
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_quit_running)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_quit(
            cube,
            "test"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_running(cube)
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_focus)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        20,
        5
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_widget_t *first =
        fossil_io_cube_widget_create(
            cube,
            "first",
            "button",
            &rect
        );

    fossil_io_cube_widget_t *second =
        fossil_io_cube_widget_create(
            cube,
            "second",
            "button",
            &rect
        );

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_TRUE(first != NULL);
    ASSUME_ITS_TRUE(second != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_focus_order(first, 0)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_focus_order(second, 1)
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_widget_focus(
            cube,
            "first"
        )
    );

    ASSUME_ITS_TRUE(
        fossil_io_cube_widget_focused(cube) == first
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_focus_move(
            cube,
            "next"
        )
    );

    ASSUME_ITS_TRUE(
        fossil_io_cube_widget_focused(cube) == second
    );

    fossil_io_cube_widget_destroy(cube, "first");
    fossil_io_cube_widget_destroy(cube, "second");
    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Scrolling
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_scroll)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        40,
        10
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "viewer",
            "text",
            &rect
        );

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_scroll(
            widget,
            "down",
            1
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_scroll(
            widget,
            "top",
            0
        )
    );

    fossil_io_cube_widget_destroy(
        cube,
        "viewer"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Styling
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_style)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        20,
        5
    };

    fossil_io_cube_style_t style;

    memset(&style, 0, sizeof(style));

    strcpy(
        style.attributes,
        "bold,underline"
    );

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "label",
            "label",
            &rect
        );

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_style_register(
            cube,
            "emphasis",
            &style
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_style_apply(
            widget,
            "emphasis"
        )
    );

    fossil_io_cube_widget_destroy(
        cube,
        "label"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Text Input
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_text_input)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        40,
        5
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "input",
            "text_input",
            &rect
        );

    char buffer[128];

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_input_set(
            widget,
            "Fossil Logic"
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_input_get(
            widget,
            buffer,
            sizeof(buffer)
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "Fossil Logic",
        buffer
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_input_cursor(
            widget,
            6
        )
    );

    fossil_io_cube_widget_destroy(
        cube,
        "input"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Message / Dialog
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_message)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_message(
            cube,
            "Information",
            "Cube test message",
            "default"
        )
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Status / Activation
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_status)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    ASSUME_ITS_TRUE(cube != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_status(
            cube,
            "Ready"
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "Ready",
        fossil_io_cube_status_get(cube)
    );

    fossil_io_cube_destroy(cube);
}

FOSSIL_TEST(c_test_cube_activate)
{
    fossil_io_cube_config_t config = {
        "ansi",
        "terminal",
        "absolute",
        "default",
        NULL,
        NULL
    };

    fossil_io_cube_rect_t rect = {
        0,
        0,
        20,
        5
    };

    fossil_io_cube_t *cube =
        fossil_io_cube_create(&config);

    fossil_io_cube_widget_t *widget =
        fossil_io_cube_widget_create(
            cube,
            "main",
            "panel",
            &rect
        );

    ASSUME_ITS_TRUE(cube != NULL);
    ASSUME_ITS_TRUE(widget != NULL);

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_activate(
            cube,
            "main"
        )
    );

    ASSUME_ITS_EQUAL_CSTR(
        "main",
        fossil_io_cube_active(cube)
    );

    fossil_io_cube_widget_destroy(
        cube,
        "main"
    );

    fossil_io_cube_destroy(cube);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Utility
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cube_validate_id)
{
    ASSUME_ITS_EQUAL_I32(
        1,
        fossil_io_cube_validate_id("cube")
    );

    ASSUME_ITS_EQUAL_I32(
        1,
        fossil_io_cube_validate_id("main_window")
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_validate_id("")
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_validate_id(NULL)
    );
}

FOSSIL_TEST(c_test_cube_option_valid)
{
    const char *options[] = {
        "ansi",
        "terminal",
        "software",
        NULL
    };

    ASSUME_ITS_EQUAL_I32(
        1,
        fossil_io_cube_option_valid(
            "ansi",
            options
        )
    );

    ASSUME_ITS_EQUAL_I32(
        1,
        fossil_io_cube_option_valid(
            "terminal",
            options
        )
    );

    ASSUME_ITS_EQUAL_I32(
        0,
        fossil_io_cube_option_valid(
            "invalid",
            options
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_cube_tests)
{
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_create_destroy);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_init_shutdown);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_terminal_size);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_clear);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_cursor);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_terminal_title);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_render_frame);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_draw_text);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_draw_text_styled);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_draw_border);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_fill);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_window_create_find_destroy);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_panel);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_widget);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_menu);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_layout);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_actions);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_event);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_quit_running);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_focus);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_scroll);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_style);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_text_input);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_message);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_status);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_activate);

    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_validate_id);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_option_valid);

    FOSSIL_ADD_SUITE(c_cube_suite);
}
