#pragma once

#define iOCT_BUTTON_LIST \
    X(OCT_KEY_0,                "0") \
    X(OCT_KEY_1,                "1") \
    X(OCT_KEY_2,                "2") \
    X(OCT_KEY_3,                "3") \
    X(OCT_KEY_4,                "4") \
    X(OCT_KEY_5,                "5") \
    X(OCT_KEY_6,                "6") \
    X(OCT_KEY_7,                "7") \
    X(OCT_KEY_8,                "8") \
    X(OCT_KEY_9,                "9") \
    X(OCT_KEY_A,                "A") \
    X(OCT_KEY_B,                "B") \
    X(OCT_KEY_C,                "C") \
    X(OCT_KEY_D,                "D") \
    X(OCT_KEY_E,                "E") \
    X(OCT_KEY_F,                "F") \
    X(OCT_KEY_G,                "G") \
    X(OCT_KEY_H,                "H") \
    X(OCT_KEY_I,                "I") \
    X(OCT_KEY_J,                "J") \
    X(OCT_KEY_K,                "K") \
    X(OCT_KEY_L,                "L") \
    X(OCT_KEY_M,                "M") \
    X(OCT_KEY_N,                "N") \
    X(OCT_KEY_O,                "O") \
    X(OCT_KEY_P,                "P") \
    X(OCT_KEY_Q,                "Q") \
    X(OCT_KEY_R,                "R") \
    X(OCT_KEY_S,                "S") \
    X(OCT_KEY_T,                "T") \
    X(OCT_KEY_U,                "U") \
    X(OCT_KEY_V,                "V") \
    X(OCT_KEY_W,                "W") \
    X(OCT_KEY_X,                "X") \
    X(OCT_KEY_Y,                "Y") \
    X(OCT_KEY_Z,                "Z") \
    \
    X(OCT_KEY_F1,               "F1") \
    X(OCT_KEY_F2,               "F2") \
    X(OCT_KEY_F3,               "F3") \
    X(OCT_KEY_F4,               "F4") \
    X(OCT_KEY_F5,               "F5") \
    X(OCT_KEY_F6,               "F6") \
    X(OCT_KEY_F7,               "F7") \
    X(OCT_KEY_F8,               "F8") \
    X(OCT_KEY_F9,               "F9") \
    X(OCT_KEY_F10,              "F10") \
    X(OCT_KEY_F11,              "F11") \
    X(OCT_KEY_F12,              "F12") \
    \
    X(OCT_KEY_SPACE,            " ") \
    X(OCT_KEY_SEMICOLON,        ";") \
    X(OCT_KEY_APOSTROPHE,       "'") \
    X(OCT_KEY_COMMA,            ",") \
    X(OCT_KEY_PERIOD,           ".") \
    X(OCT_KEY_SLASH,            "/") \
    X(OCT_KEY_BACKSLASH,        "\\") \
    X(OCT_KEY_MINUS,            "-") \
    X(OCT_KEY_EQUAL,            "=") \
    X(OCT_KEY_GRAVE,            "`") \
    X(OCT_KEY_LEFT_BRACKET,     "[") \
    X(OCT_KEY_RIGHT_BRACKET,    "]") \
    \
    X(OCT_KEY_BACKSPACE,        "Backspace") \
    X(OCT_KEY_DELETE,           "Delete") \
    X(OCT_KEY_INSERT,           "Insert") \
    X(OCT_KEY_TAB,              "Tab") \
    X(OCT_KEY_ENTER,            "Enter") \
    X(OCT_KEY_ESCAPE,           "Escape") \
    X(OCT_KEY_CAPS_LOCK,        "Caps Lock") \
    X(OCT_KEY_NUM_LOCK,         "Num Lock") \
    X(OCT_KEY_SCROLL_LOCK,      "Scroll Lock") \
    X(OCT_KEY_FN_LOCK,          "FN Lock") \
    X(OCT_KEY_PRINT_SCREEN,     "Print Screen") \
    X(OCT_KEY_HOME,             "Home") \
    X(OCT_KEY_END,              "End") \
    X(OCT_KEY_PAGE_UP,          "Page Up") \
    X(OCT_KEY_PAGE_DOWN,        "Page Down") \
    X(OCT_KEY_LEFT,             "Left") \
    X(OCT_KEY_RIGHT,            "Right") \
    X(OCT_KEY_UP,               "Up") \
    X(OCT_KEY_DOWN,             "Down") \
    \
    X(OCT_KEY_SHIFT,            "Shift") \
    X(OCT_KEY_CTRL,             "Ctrl") \
    X(OCT_KEY_ALT,              "Alt") \
    X(OCT_KEY_FN,               "Fn") \
    X(OCT_KEY_META,             "Meta") \
    X(OCT_KEY_MENU,             "Menu") \
    \
    X(OCT_MOUSE_LEFT,           "Left click") \
    X(OCT_MOUSE_RIGHT,          "Right click") \
    X(OCT_MOUSE_MIDDLE,         "Middle click") \
    X(OCT_MOUSE_4,              "Mouse 4") \
    X(OCT_MOUSE_5,              "Mouse 5") \
    X(OCT_MOUSE_6,              "Mouse 6") \
    X(OCT_MOUSE_7,              "Mouse 7") \
    X(OCT_MOUSE_8,              "Mouse 8") \
    X(OCT_MOUSE_9,              "Mouse 9") \
    X(OCT_MOUSE_10,             "Mouse 10") \
    X(OCT_MOUSE_11,             "Mouse 11") \
    X(OCT_MOUSE_12,             "Mouse 12") \
    X(OCT_MOUSE_SCROLL_UP,     "Scroll up") \
    X(OCT_MOUSE_SCROLL_DOWN,   "Scroll down") \



typedef enum OCT_BUTTONS {
#define X(keycode, print) keycode,
    iOCT_BUTTON_LIST
#undef X
    OCT_BUTTONS_TOTAL
} OCT_BUTTON;

static const int OCT_BUTTONS_ARRAY[] = {
#define X(keyCode, print) keyCode,
    iOCT_BUTTON_LIST
#undef X
};

static const char* OCT_BUTTONS_NAMES[] = {
#define X(keyCode, print) print,
    iOCT_BUTTON_LIST
#undef X
};
