#include "config.h"

#include "clicker.h"
#include "mainwindow.h"

#include <string>

#include <chrono>
#include <thread>
#include <Windows.h>

#pragma comment (lib, "WinMM.lib")

using std::string;
using std::thread;

//----------

thread left_clicker_Thread;
thread right_clicker_Thread;
thread msg_loop_Thread;

HHOOK keyboard_hook;
HHOOK mouse_hook;

DWORD msg_loop_ID;

//----------

bool clicker::program = true;

bool is_active_L = false;
bool is_active_R = false;

bool left::is_active_mouse = false;
bool left::skip_next_down = false;
bool left::skip_next_up = false;

bool right::is_active_mouse = false;
bool right::skip_next_down = false;
bool right::skip_next_up = false;


//----------

static void send_click(const string& button) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    if (button == "left") {
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        left::skip_next_down = true;
    }

    else if (button == "right") {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        right::skip_next_down = true;
    }

    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT));


    input = { 0 };
    input.type = INPUT_MOUSE;

    if (button == "left") {
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        left::skip_next_up = true;
    }

    else if (button == "right") {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        right::skip_next_up = true;
    }

    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT));
}


//----------

void clicker::left() {
    while (clicker::program == true) {
        while (
            (is_active_L == true && config_json["mouse_hold_left"] == false)
            ||
            (is_active_L == true && config_json["mouse_hold_left"] == true && left::is_active_mouse == true)) {

            thread click = thread(send_click, "left");

            std::chrono::milliseconds wait(1000 / config_json["left_cps"]);
            std::this_thread::sleep_for(wait);

            if (click.joinable()) click.join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void clicker::right() {
    while (clicker::program == true) {
        while (
            (is_active_R == true && config_json["mouse_hold_right"] == false)
            ||
            (is_active_R == true && config_json["mouse_hold_right"] == true && right::is_active_mouse == true)) {

            thread click = thread(send_click, "right");

            std::chrono::milliseconds wait(1000 / config_json["right_cps"]);
            std::this_thread::sleep_for(wait);

            if (click.joinable()) click.join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void clicker::stop() {
    is_active_L = false;
    is_active_R = false;

    left::is_active_mouse = false;
    right::is_active_mouse = false;

    left::skip_next_down = false;
    left::skip_next_up = false;

    right::skip_next_down = false;
    right::skip_next_up = false;
}

//----------

static void change_toggle_keys::from_keyboard(const string& key) {
    if (change_key == "left") {
        change_key = "false";

        config_json["toggle_key_left"] = key;

        if (!write_json()) {
            emit MainWindow::instance->load_cfg_into_ui();
            return;
        }

        emit MainWindow::instance->update_key_label("left", key);
        emit MainWindow::instance->change_checkbox("left", true);
    }

    else if (change_key == "right") {
        change_key = "false";

        config_json["toggle_key_right"] = key;

        if (!write_json()) {
            emit MainWindow::instance->load_cfg_into_ui();
            return;
        }

        emit MainWindow::instance->update_key_label("right", key);
        emit MainWindow::instance->change_checkbox("right", true);
    }
}

static void change_toggle_keys::from_mouse(const WPARAM& wparam, const LPARAM& lparam) {
    if (change_key == "left") {
        change_key = "false";

        if (wparam == WM_MBUTTONDOWN) {
            config_json["toggle_key_left"] = "M. Middle";
        }

        else if (wparam == WM_XBUTTONDOWN) {
            PMSLLHOOKSTRUCT pMouseStruct = PMSLLHOOKSTRUCT(lparam);
            WORD xButton = HIWORD(pMouseStruct->mouseData);

            string side_button = (xButton == XBUTTON1) ? "M. Back" : "M. Forward";

            config_json["toggle_key_left"] = side_button;
        }

        if (!write_json()) {
            emit MainWindow::instance->load_cfg_into_ui();
            return;
        }

        emit MainWindow::instance->update_key_label("left", config_json["toggle_key_left"]);
        emit MainWindow::instance->change_checkbox("left", false);
    }

    else if (change_key == "right") {
        change_key = "false";

        if (wparam == WM_MBUTTONDOWN) {
            config_json["toggle_key_right"] = "M. Middle";
        }

        else if (wparam == WM_XBUTTONDOWN) {
            PMSLLHOOKSTRUCT pMouseStruct = PMSLLHOOKSTRUCT(lparam);
            WORD xButton = HIWORD(pMouseStruct->mouseData);

            string side_button = (xButton == XBUTTON1) ? "M. Back" : "M. Forward";

            config_json["toggle_key_right"] = side_button;
        }

        if (!write_json()) {
            emit MainWindow::instance->load_cfg_into_ui();
            return;
        }

        emit MainWindow::instance->update_key_label("right", config_json["toggle_key_right"]);
        emit MainWindow::instance->change_checkbox("right", false);
    }
}

//----------

static void change_active_bool(const string& mouse_button) {
    if (mouse_button == "left") {
        if (is_active_L == false) {
            is_active_L = true;
            if (config_json["toggle_sound"] == true) PlaySound(TEXT("resources/active.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
        else {
            is_active_L = false;
            if (config_json["toggle_sound"] == true) PlaySound(TEXT("resources/deactive.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
    }

    else if (mouse_button == "right") {
        if (is_active_R == false) {
            is_active_R = true;
            if (config_json["toggle_sound"] == true) PlaySound(TEXT("resources/active.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
        else {
            is_active_R = false;
            if (config_json["toggle_sound"] == true) PlaySound(TEXT("resources/deactive.wav"), NULL, SND_PURGE | SND_FILENAME | SND_ASYNC);
        }
    }

    emit MainWindow::instance->update_status_label();
}

static void change_mouse_bool(const string& mouse_button, const string& status) {
    if (mouse_button == "left") {
        if (status == "down") {
            if (left::skip_next_down == true) left::skip_next_down = false;
            else left::is_active_mouse = true;;
        }

        else if (status == "up") {
            if (left::skip_next_up == true) left::skip_next_up = false;
            else left::is_active_mouse = false;
        }
    }

    else if (mouse_button == "right") {
        if (status == "down") {
            if (right::skip_next_down == true) right::skip_next_down = false;
            else right::is_active_mouse = true;
        }

        else if (status == "up") {
            if (right::skip_next_up == true) right::skip_next_up = false;
            else right::is_active_mouse = false;
        }
    }
}

//----------

static string get_key_name(const DWORD& scanCode, const DWORD& flags) {
    wchar_t key_name[512];
    GetKeyNameTextW((scanCode << 16) | (flags << 24), key_name, sizeof(key_name) / sizeof(wchar_t));

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, key_name, -1, nullptr, 0, nullptr, nullptr);

    string string_key_name(size_needed - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, key_name, -1, &string_key_name[0], size_needed, nullptr, nullptr);

    return string_key_name;
}

LRESULT CALLBACK keyboard_Proc(int ncode, WPARAM wparam, LPARAM lparam) {
    if (ncode >= 0 && (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lparam;
        string key = get_key_name(pKeyboard->scanCode, pKeyboard->flags);

        if (change_key == "false") {
            if (config_json["toggle_key_left"] == key && (config_json["mouse_button"] == "left" || config_json["mouse_button"] == "both")) {
                change_active_bool("left");
            }

            if (config_json["toggle_key_right"] == key && (config_json["mouse_button"] == "right" || config_json["mouse_button"] == "both")) {
                change_active_bool("right");
            }
        }

        if (change_key != "false") {
            change_toggle_keys::from_keyboard(key);
        }
    }

    return CallNextHookEx(keyboard_hook, ncode, wparam, lparam);
}

//----------

LRESULT CALLBACK mouse_Proc(int ncode, WPARAM wparam, LPARAM lparam) {
    if (ncode >= 0) {
        if (config_json["mouse_button"] == "left" || config_json["mouse_button"] == "both") {
            if (config_json["mouse_hold_left"] == true && change_key == "false") {
                if (wparam == WM_LBUTTONDOWN) {
                    change_mouse_bool("left", "down");
                }

                if (wparam == WM_LBUTTONUP) {
                    change_mouse_bool("left", "up");
                }
            }

            else if (config_json["mouse_hold_left"] == false && change_key == "false") {
                if (config_json["toggle_key_left"] == "M. Middle") {
                    if (wparam == WM_MBUTTONDOWN) {
                        is_active_L = true;
                    }

                    if (wparam == WM_MBUTTONUP) {
                        is_active_L = false;
                    }
                    emit MainWindow::instance->update_status_label();
                }

                else if (config_json["toggle_key_left"] == "M. Back" || "M. Forward") {
                    PMSLLHOOKSTRUCT pMouseStruct = PMSLLHOOKSTRUCT(lparam);
                    WORD xButton = HIWORD(pMouseStruct->mouseData);

                    string side_button = (xButton == XBUTTON1) ? "M. Back" : "M. Forward";

                    if (wparam == WM_XBUTTONDOWN && config_json["toggle_key_left"] == side_button) {
                        is_active_L = true;
                    }

                    if (wparam == WM_XBUTTONUP && config_json["toggle_key_left"] == side_button) {
                        is_active_L = false;
                    }
                    emit MainWindow::instance->update_status_label();
                }
            }
        }

        if (config_json["mouse_button"] == "right" || config_json["mouse_button"] == "both") {
            if (config_json["mouse_hold_right"] == true && change_key == "false") {
                if (wparam == WM_RBUTTONDOWN) {
                    change_mouse_bool("right", "down");
                }

                if (wparam == WM_RBUTTONUP) {
                    change_mouse_bool("right", "up");
                }
            }

            else if (config_json["mouse_hold_right"] == false && change_key == "false") {
                if (config_json["toggle_key_right"] == "M. Middle") {
                    if (wparam == WM_MBUTTONDOWN) {
                        is_active_R = true;
                    }

                    if (wparam == WM_MBUTTONUP) {
                        is_active_R = false;
                    }
                    emit MainWindow::instance->update_status_label();
                }

                else if (config_json["toggle_key_right"] == "M. Back" || "M. Forward") {
                    PMSLLHOOKSTRUCT pMouseStruct = PMSLLHOOKSTRUCT(lparam);
                    WORD xButton = HIWORD(pMouseStruct->mouseData);

                    string side_button = (xButton == XBUTTON1) ? "M. Back" : "M. Forward";

                    if (wparam == WM_XBUTTONDOWN && config_json["toggle_key_right"] == side_button) {
                        is_active_R = true;
                    }

                    if (wparam == WM_XBUTTONUP && config_json["toggle_key_right"] == side_button) {
                        is_active_R = false;
                    }
                    emit MainWindow::instance->update_status_label();
                }
            }
        }

        if (change_key != "false" && (wparam == WM_MBUTTONDOWN || wparam == WM_XBUTTONDOWN)) change_toggle_keys::from_mouse(wparam, lparam);
    }
    return CallNextHookEx(mouse_hook, ncode, wparam, lparam);
}

//----------

void handle_Hooks() {
    msg_loop_ID = GetCurrentThreadId();

    keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_Proc, NULL, 0);
    mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, mouse_Proc, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
