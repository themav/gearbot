/* This function loads the bot's parameters from file.
 * Created by MK on 11/01/2022
 */

#include "config_store.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>

config_store::config_store(const string &file_name) {
  std::ifstream config_file{file_name};
  if (config_file.bad() || !config_file.is_open()) {
    config_file.close();
    throw std::invalid_argument{"config_store.cpp: Unable to read from config file."};
  }
  _values = new std::map<string, string>;
  std::string line;
  while (std::getline(config_file, line)) {
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    if (line.empty() || line[0] == '#')
      continue;
    auto key = line.substr(0, line.find('='));
    auto value = line.substr(line.find('=') + 1, line.size());
    _values->insert(std::pair<string, string>(key, value));
  }
}

// Returns the a reference to the string containing the value given the key.
const std::string& config_store::get_value(const string &_key) {
  auto value = _values->find(_key);
  if (value == _values->end())
    throw std::out_of_range{"config_store.cpp: Unknown key specified to get_value."};
  return value->second; // Get the value from the key,value pair.
}

config_store::~config_store() { delete _values; }

config_store::config_store(const config_store &old) {
  this->_values = new std::map<string, string>(*old._values);
}
