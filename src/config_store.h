//Created by MK on 11/01/2022

#pragma once
#ifndef JIBBLESBOT_LOAD_CONFIG_H
#define JIBBLESBOT_LOAD_CONFIG_H
#include <string>
#include <map>
using std::string;
class config_store {
    public:
    const string& get_value(const string& _key);

    config_store(const string& file_name);
    ~config_store();
    config_store(const config_store& old);

    private:
    std::map<string, string> *_values;
};

#endif