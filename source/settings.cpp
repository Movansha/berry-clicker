#include "settings.h"

#include <fstream>
#include <vector>
#include <iostream>


using std::ifstream; using std::ofstream;
using std::vector;
using std::stoi; using std::stod;


str config::toggle_Key_L, config::toggle_Key_R;
double config::left_CPS, config::right_CPS;
bool config::mouse_hold_L, config::mouse_hold_R;

str config::mouse_Button;
bool config::toggle_Sound;

int line_TK_L = 0, line_TK_R = 1, line_L_CPS = 2, line_R_CPS = 3, line_MH_L = 4, line_MH_R = 5, line_MB = 6, line_TS = 7;


void set_values(str TK_L, str TK_R, double L_CPS, double R_CPS, bool MH_L, bool MH_R, str MB, bool TS) {
    config::toggle_Key_L = TK_L;
    config::toggle_Key_R = TK_R;

    config::left_CPS = L_CPS;
    config::right_CPS = R_CPS;

    config::mouse_hold_L = MH_L;
    config::mouse_hold_R = MH_R;

    config::mouse_Button = MB;
    config::toggle_Sound = TS;
}

void default_values() {
    ofstream file("settings.cfg");

    vector<str> lines = { "R", "R", "15", "20", "1", "1", "left", "0" };
    set_values("R", "R", 15, 20, true, true, "left", false);

    for (const str line : lines) {
        file << line << "\n";
    }

    file.close();
}


void setup_cfg() {
    try {
        vector<str> lines;
        str temp;
        ifstream cfg_file("settings.cfg");

        if (!cfg_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        while (std::getline(cfg_file, temp)) {
            lines.push_back(temp);
        }

        cfg_file.close();

        set_values(lines[line_TK_L], lines[line_TK_R],
            stod(lines[line_L_CPS]), stod(lines[line_R_CPS]),
            stoi(lines[line_MH_L]), stoi(lines[line_MH_R]),
            lines[line_MB], stoi(lines[line_TS]));
    }

    catch (...) {
        default_values();
    }
}

void write_cfg(int line, str text) {
    try {
        vector<str> lines;
        str temp;
        ifstream input_file("settings.cfg");

        if (!input_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        while (std::getline(input_file, temp)) {
            lines.push_back(temp);
        }

        input_file.close();


        lines[line] = text;
        ofstream output_file("settings.cfg");

        if (!output_file.is_open()) throw std::ios_base::failure("settings.cfg file couldn't open");

        for (const str line : lines) {
            output_file << line << "\n";
        }

        output_file.close();
    }
    catch (...) {
        default_values();
    }
}