#pragma once
#include "include/json.hpp"

using nlohmann::json;

//----------

extern json config_json;

void setup_json();
bool write_json();
