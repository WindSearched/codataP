//
// Created by ws on 2026/11/1.
//

#ifndef CODATAP_LUAMAN_H
#define CODATAP_LUAMAN_H

#include <filesystem>
#include <sol/sol.hpp>

#include "../../Branch.h"

//lua manager
class LuaMan {
public:
    static sol::state lua;
    static Branch branch_;

    static void Register();
    static bool Load(std::filesystem::path path);
    static void Read(std::string content);
    static bool Read(std::filesystem::path path);
};


#endif //CODATAP_LUAMAN_H
