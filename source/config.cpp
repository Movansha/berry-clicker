#include "config.h"

#include "include/json.hpp"

#include <ShlObj.h>
#include <filesystem>

#include <fstream>

//----------

using nlohmann::json;

using std::ifstream;
using std::ofstream;

using std::wstring;

json config_json;

//----------

//----------

static wstring get_path_AppData() {
    PWSTR temp = NULL;

    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &temp);

    return wstring(temp);
}

static wstring mkdir_in_AppData(const wstring& path) {
    wstring folder = get_path_AppData() + L"\\" + path;

    if (!std::filesystem::exists(folder)) std::filesystem::create_directories(folder);

    return folder;
}

static wstring config_file = mkdir_in_AppData(L"Movansha\\Berry Clicker") + L"\\config.json";

//----------

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
