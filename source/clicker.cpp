#include "clicker.h"

#include "settings.h"
#include "main_window.h"

#include <thread>
#include <chrono>
#include <Windows.h>

#pragma comment(lib, "WinMM.lib")


using std::string;
using std::thread;


bool clicker::program = true;
bool is_active_L = false, is_active_R = false;

bool left::status = false;
bool left::skip_Next_Down = false, left::skip_Next_Up = false;

bool right::status = false;
bool right::skip_Next_Down = false, right::skip_Next_Up = false;

thread left_Clicker_Thread, right_Clicker_Thread, msg_loop_Thread;
HHOOK mouse_Hook, keyboard_Hook;
DWORD msg_loop_ID;


static void send_Click(const string& button) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    if (button == "left") {
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        left::skip_Next_Down = true;
    }

    if (button == "right") {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        right::skip_Next_Down = true;
    }

    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT));


    input = { 0 };
    input.type = INPUT_MOUSE;

    if (button == "left") {
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        left::skip_Next_Up = true;
    }

    if (button == "right") {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        right::skip_Next_Up = true;
    }

    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT));
}

void clicker::stop() {
    is_active_L = false; is_active_R = false;

    left::status = false; right::status = false;

    left::skip_Next_Down = false; left::skip_Next_Up = false;
    right::skip_Next_Down = false; right::skip_Next_Up = false;
}

void clicker::left() {
    while (clicker::program == true) {
        while ((is_active_L == true && config::mouse_hold_L == false) ||
            (is_active_L == true && config::mouse_hold_L == true && left::status == true)) {

            thread click = thread(send_Click, "left");

            std::chrono::milliseconds wait(1000 / config::left_CPS);
            std::this_thread::sleep_for(wait);

            if (click.joinable()) click.join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void clicker::right() {
    while (clicker::program == true) {
        while ((is_active_R == true && config::mouse_hold_R == false) ||
            (is_active_R == true && config::mouse_hold_R == true && right::status == true)) {

            thread click = thread(send_Click, "right");

            std::chrono::milliseconds wait(1000 / config::right_CPS);
            std::this_thread::sleep_for(wait);

            if (click.joinable()) click.join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}


// Keyboard and mouse >>

static void change_toggle_key::from_Keyboard(const string& key) {
    if (change_Key == "left") {
        MainWindow::instance->update_key_label("left", key);
        MainWindow::instance->change_hold_Checkbox("left", true);
        config::toggle_Key_L = key;
        write_cfg(line_TK_L, key);
    }

    else if (change_Key == "right") {
        config::toggle_Key_R = key;
        MainWindow::instance->update_key_label("right", key);
        MainWindow::instance->change_hold_Checkbox("right", true);
        write_cfg(line_TK_R, key);
    }
}

static void change_toggle_key::from_Mouse(const WPARAM& wparam) {
    if (change_Key == "left") {
        switch (wparam) {

        case WM_MBUTTONDOWN:
            config::toggle_Key_L = "M Middle";
            break;

        case WM_XBUTTONDOWN:
            config::toggle_Key_L = "M Side";
            break;

        default:
            break;
        }
        MainWindow::instance->update_key_label("left", config::toggle_Key_L);
        MainWindow::instance->change_hold_Checkbox("left", false);
        write_cfg(line_TK_L, config::toggle_Key_L);
    }

    else if (change_Key == "right") {
        switch (wparam) {

        case WM_MBUTTONDOWN:
            config::toggle_Key_R = "M Middle";
            break;

        case WM_XBUTTONDOWN:
            config::toggle_Key_R = "M Side";
            break;

        default:
            break;
        }
        MainWindow::instance->update_key_label("right", config::toggle_Key_R);
        MainWindow::instance->change_hold_Checkbox("right", false);
        write_cfg(line_TK_R, config::toggle_Key_R);
    }
}


static void change_active_bool(const string& button) {
    if (button == "left") {
        if (is_active_L == false) {
            is_active_L = true;
            if (config::toggle_Sound == true) PlaySound(TEXT("resources/active.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
        else {
            is_active_L = false;
            if (config::toggle_Sound == true) PlaySound(TEXT("resources/deactive.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
    }

    else if (button == "right") {
        if (is_active_R == false) {
            is_active_R = true;
            if (config::toggle_Sound == true) PlaySound(TEXT("resources/active.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
        else {
            is_active_R = false;
            if (config::toggle_Sound == true) PlaySound(TEXT("resources/deactive.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
    }
    MainWindow::instance->update_status_label();
}

static void change_mouse_bool(const string& button, const string& status) {
    if (button == "left") {
        if (status == "down") {
            if (left::skip_Next_Down == true) left::skip_Next_Down = false;
            else left::status = true;
        }
        else {
            if (left::skip_Next_Up == true) left::skip_Next_Up = false;
            else left::status = false;
        }
    }
    else {
        if (status == "down") {
            if (right::skip_Next_Down == true) right::skip_Next_Down = false;
            else right::status = true;
        }
        else {
            if (right::skip_Next_Up == true) right::skip_Next_Up = false;
            else right::status = false;
        }
    }
}


static string get_Key_Name(const DWORD& scanCode) {
    char key_Name[512];
    GetKeyNameTextA(scanCode << 16, key_Name, sizeof(key_Name));
    return string(key_Name);
}

LRESULT CALLBACK keyboard_Proc(int ncode, WPARAM wparam, LPARAM lparam) {
    if (ncode >= 0 && (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lparam;
        string key = get_Key_Name(pKeyboard->scanCode);

        if (change_Key == "no" && config::toggle_Key_L == key && (config::mouse_Button == "left" || config::mouse_Button == "both")) {
            change_active_bool("left");
        }
        if (change_Key == "no" && config::toggle_Key_R == key && (config::mouse_Button == "right" || config::mouse_Button == "both")) {
            change_active_bool("right");
        }

        if (change_Key != "no") {
            change_toggle_key::from_Keyboard(key);
        }
    }
    return CallNextHookEx(keyboard_Hook, ncode, wparam, lparam);
}

LRESULT CALLBACK mouse_Proc(int ncode, WPARAM wparam, LPARAM lparam) {
    if (ncode >= 0) {
        if (config::mouse_Button == "left" || config::mouse_Button == "both") {
            if (config::mouse_hold_L == true && change_Key == "no") {
                if (wparam == WM_LBUTTONDOWN) {
                    change_mouse_bool("left", "down");
                }

                if (wparam == WM_LBUTTONUP) {
                    change_mouse_bool("left", "up");
                }
            }

            else if (config::mouse_hold_L == false && change_Key == "no") {
                if (config::toggle_Key_L == "M Middle") {
                    if (wparam == WM_MBUTTONDOWN) {
                        is_active_L = true;
                    }

                    if (wparam == WM_MBUTTONUP) {
                        is_active_L = false;
                    }
                    MainWindow::instance->update_status_label();
                }

                else if (config::toggle_Key_L == "M Side") {
                    if (wparam == WM_XBUTTONDOWN) {
                        is_active_L = true;
                    }

                    if (wparam == WM_XBUTTONUP) {
                        is_active_L = false;
                    }
                    MainWindow::instance->update_status_label();
                }
            }
        }

        if (config::mouse_Button == "right" || config::mouse_Button == "both") {
            if (config::mouse_hold_R == true && change_Key == "no") {
                if (wparam == WM_RBUTTONDOWN) {
                    change_mouse_bool("right", "down");
                }

                if (wparam == WM_RBUTTONUP) {
                    change_mouse_bool("right", "up");
                }
            }

            else if (config::mouse_hold_R == false && change_Key == "no") {
                if (config::toggle_Key_R == "M Middle") {
                    if (wparam == WM_MBUTTONDOWN) {
                        is_active_R = true;
                    }

                    if (wparam == WM_MBUTTONUP) {
                        is_active_R = false;
                    }
                    MainWindow::instance->update_status_label();
                }

                else if (config::toggle_Key_R == "M Side") {
                    if (wparam == WM_XBUTTONDOWN) {
                        is_active_R = true;
                    }

                    if (wparam == WM_XBUTTONUP) {
                        is_active_R = false;
                    }
                    MainWindow::instance->update_status_label();
                }
            }
        }

        if (change_Key != "no" && (wparam == WM_MBUTTONDOWN || wparam == WM_XBUTTONDOWN)) change_toggle_key::from_Mouse(wparam);
    }
    return CallNextHookEx(mouse_Hook, ncode, wparam, lparam);
}

void handle_Hooks() {
    msg_loop_ID = GetCurrentThreadId();

    keyboard_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_Proc, NULL, 0);
    mouse_Hook = SetWindowsHookEx(WH_MOUSE_LL, mouse_Proc, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}