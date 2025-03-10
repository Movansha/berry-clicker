#pragma once
#include "include/json.hpp"

#include <string>

using nlohmann::json;
using std::string;

//----------

extern json config_json;

void setup_json();
bool write_json();
