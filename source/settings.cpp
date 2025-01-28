#include "settings.h"

#include <ShlObj.h>
#include <filesystem>
#include <fstream>

#include <vector>
#include <iostream>


using std::ifstream; using std::ofstream;
using std::stoi; using std::stod;

using std::string;
using std::vector;


string config::toggle_Key_L, config::toggle_Key_R;
int config::left_CPS, config::right_CPS;
bool config::mouse_hold_L, config::mouse_hold_R;

string config::mouse_Button;
bool config::toggle_Sound;

int line_TK_L = 0, line_TK_R = 1, line_L_CPS = 2, line_R_CPS = 3, line_MH_L = 4, line_MH_R = 5, line_MB = 6, line_TS = 7;


static string get_AppData_path() {
    PWSTR temp_path = NULL;
    string output;

    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &temp_path) == S_OK) {
        std::wstring path = std::wstring(temp_path);

        size_t converted = 0;
        size_t buffer_size = path.length() + 1;
        char* buffer = new char[buffer_size];

        const wchar_t* w_path = path.c_str();
        errno_t err = wcstombs_s(&converted, buffer, buffer_size, w_path, path.length());
        
        if (err == 0) output = string(buffer);
        else output = "";

        delete[] buffer;
        CoTaskMemFree(temp_path);
    }

    return output;
}

static string get_config_path() {
    string config_folder = get_AppData_path() + "\\Movansha\\Berry Clicker";

    if (!std::filesystem::exists(config_folder)) {
        if (!std::filesystem::create_directories(config_folder)) return get_AppData_path();
    }

    return config_folder;
}

static string settings_cfg = get_config_path() + "\\settings.cfg";


static void set_values(string TK_L, string TK_R, int L_CPS, int R_CPS, bool MH_L, bool MH_R, string MB, bool TS) {
    config::toggle_Key_L = TK_L;
    config::toggle_Key_R = TK_R;

    config::left_CPS = L_CPS;
    config::right_CPS = R_CPS;

    config::mouse_hold_L = MH_L;
    config::mouse_hold_R = MH_R;

    config::mouse_Button = MB;
    config::toggle_Sound = TS;
}

static void default_values() {
    ofstream file(settings_cfg);

    vector<string> lines = { "R", "R", "15", "20", "1", "1", "left", "1" };
    set_values("R", "R", 15, 20, true, true, "left", true);

    for (const string line : lines) {
        file << line << "\n";
    }

    file.close();
}


void setup_cfg() {
    try {
        vector<string> lines;
        string temp;
        ifstream cfg_file(settings_cfg);

        if (!cfg_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        while (std::getline(cfg_file, temp)) {
            lines.push_back(temp);
        }

        cfg_file.close();

        set_values(lines[line_TK_L], lines[line_TK_R],
            stoi(lines[line_L_CPS]), stoi(lines[line_R_CPS]),
            stoi(lines[line_MH_L]), stoi(lines[line_MH_R]),
            lines[line_MB], stoi(lines[line_TS]));
    }

    catch (...) {
        default_values();
    }
}

void write_cfg(const int& line, const string& text) {
    try {
        vector<string> lines;
        string temp;
        ifstream input_file(settings_cfg);

        if (!input_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        while (std::getline(input_file, temp)) {
            lines.push_back(temp);
        }

        input_file.close();


        lines[line] = text;
        ofstream output_file(settings_cfg);

        if (!output_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        for (const string line : lines) {
            output_file << line << "\n";
        }

        output_file.close();
    }
    catch (...) {
        default_values();
    }
}