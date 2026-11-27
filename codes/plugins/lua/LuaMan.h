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
    static sol::table table;
    static Branch branch_;

    static void Register();
    static bool Load(std::filesystem::path path);
    static void Read(std::string content);
    static bool Read(std::filesystem::path path);

    static sol::protected_function GetFunction(std::vector<std::string>);

    template<typename T>
    static std::vector<T> ToVector(sol::table table) {
        std::vector<T> vec;
        for (int i = 1; i <= table.size(); i++) {
            vec.push_back(table[i].get<T>());
        }
        return vec;
    }
};


#endif //CODATAP_LUAMAN_H
