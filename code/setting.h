#ifndef SETTING_H
#define SETTING_H

#include "json.hpp"
#include <fstream>
#include <iostream>

//#define DEBUG_MODE

// for convenience
using json = nlohmann::json;

class Setting
{
public:
    int port;
    std::string dir;
    json user;
    int threadNumber;
    static Setting* getInstance();
private:
    void init();
    static Setting* me;
    Setting();
    ~Setting();
};

#endif // SETTING_H
