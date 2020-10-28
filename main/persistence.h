#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include <INIReader.h>

using namespace std;

class Persistence{
public:

    void load(string name)
    {
    INIReader reader(name);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load 'test.ini'\n";
    }
    
    std::cout << "Config loaded from 'test.ini': version="
              << reader.GetInteger("protocol", "version", -1) << ", name="
              << reader.Get("user", "name", "UNKNOWN") << ", email="
              << reader.Get("user", "email", "UNKNOWN") << ", pi="
              << reader.GetReal("user", "pi", -1) << ", active="
              << reader.GetBoolean("user", "active", true) << "\n";
    }
};


#endif