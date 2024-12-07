#pragma once

#include <string>
#include <thread>
#include <Windows.h>

using str = std::string;
using std::thread;


extern bool is_active_L, is_active_R;

extern thread left_Clicker_Thread, right_Clicker_Thread, msg_loop_Thread;
extern HHOOK keyboard_Hook, mouse_Hook;
extern DWORD msg_loop_ID;

namespace left {
    extern bool status;
    extern bool skip_Next_Down;
    extern bool skip_Next_Up;
}

namespace right {
    extern bool status;
    extern bool skip_Next_Down;
    extern bool skip_Next_Up;
}

namespace clicker {
    extern bool program;

    void stop();
    void left();
    void right();
}

namespace change_toggle_key {
    void from_Keyboard(str key);
    void from_Mouse(WPARAM wparam);
}

LRESULT CALLBACK keyboard_Proc(int ncode, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK mouse_Proc(int ncode, WPARAM wparam, LPARAM lparam);
void handle_Hooks();