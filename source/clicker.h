#pragma once

#include <string>

#include <thread>
#include <Windows.h>

using std::string;
using std::thread;

//----------

extern thread left_clicker_Thread;
extern thread right_clicker_Thread;
extern thread msg_loop_Thread;

extern HHOOK keyboard_hook;
extern HHOOK mouse_hook;

extern DWORD msg_loop_ID;

//----------

extern bool is_active_L;
extern bool is_active_R;

namespace left {
    extern bool is_active_mouse;
    extern bool skip_next_down;
    extern bool skip_next_up;
}

namespace right {
    extern bool is_active_mouse;
    extern bool skip_next_down;
    extern bool skip_next_up;
}

namespace clicker {
    extern bool program;

    void left();
    void right();
    void stop();
}

//----------

namespace change_toggle_keys {
    static void from_keyboard(const string& key);
    static void from_mouse(const WPARAM& wparam, const LPARAM& lparam);
}

LRESULT CALLBACK keyboard_Proc(int ncode, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK mouse_Proc(int ncode, WPARAM wparam, LPARAM lparam);

void handle_Hooks();
