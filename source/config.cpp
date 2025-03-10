#include "include/json.hpp"

#include "config.h"

#include <ShlObj.h>
#include <filesystem>
#include <fstream>

#include <string>

//----------

using nlohmann::json;

using std::ifstream;
using std::ofstream;

using std::string;

json config_json;

//----------

//----------

static string get_path_AppData() {
    PWSTR temp = NULL;
    string output;

    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &temp) == S_OK) {
        std::wstring path = std::wstring(temp);

        size_t converted = 0;
        size_t buffer_size = path.length() + 1;
        char* buffer = new char[buffer_size];

        const wchar_t* w_path = path.c_str();
        errno_t err = wcstombs_s(&converted, buffer, buffer_size, w_path, path.length());

        if (err == 0) output = string(buffer) + "\\";
        else output = "";

        delete[] buffer;
        CoTaskMemFree(temp);
    }

    return output;
}

static string get_config_folder() {
    string config_folder = get_path_AppData() + "Movansha\\Berry Clicker\\";

    if (!std::filesystem::exists(config_folder)) std::filesystem::create_directories(config_folder);

    return config_folder;
}

//----------

static string config_file = get_config_folder() + "config.json";

static void default_values() {
    config_json = {
        {"toggle_key_left", "R"},
        {"toggle_key_right", "R"},
        {"left_cps", 15},
        {"right_cps", 20},
        {"mouse_hold_left", true},
        {"mouse_hold_right", true},
        {"mouse_button", "left"},
        {"toggle_sound", true}
    };

    ofstream file(config_file);
    file << config_json.dump(4);
    file.close();
}

void setup_json() {
    try {
        ifstream file(config_file);
        file >> config_json;
        file.close();
    }

    catch (...) {
        default_values();
    }
}

bool write_json() {
    try {
        std::ofstream file(config_file);
        file << config_json.dump(4);
        file.close();

        return true;
    }
    catch (...) {
        default_values();

        return false;
    }
}
