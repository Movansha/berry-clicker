#pragma once

#include <string>

using std::string;


namespace config {
	extern string toggle_Key_L, toggle_Key_R;
	extern int left_CPS, right_CPS;
	extern bool mouse_hold_L, mouse_hold_R;

	extern string mouse_Button;
	extern bool toggle_Sound;
}

extern int line_TK_L, line_TK_R, line_L_CPS, line_R_CPS, line_MH_L, line_MH_R, line_MB, line_TS;

void setup_cfg();
void write_cfg(const int& line, const string& text);
