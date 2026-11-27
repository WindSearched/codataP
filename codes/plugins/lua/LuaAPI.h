//
// Created by ws on 2026/11/26.
//

#ifndef CODATAP_LUAAPI_H
#define CODATAP_LUAAPI_H
#include <sol/state.hpp>


class LuaAPI {
    public:
    static void DataReg(sol::state &state);
    static void BranchReg(sol::state &state);
    static void NewReg(sol::state &state);

    static std::vector<std::string> TableToStrings(const sol::table& table);
    template<typename T>
    static sol::table VectorToTable
        (sol::state_view lua, const std::vector<T>& values);
};

template<typename T>
sol::table LuaAPI::VectorToTable(sol::state_view lua, const std::vector<T> &values) {
    sol::table result = lua.create_table();

    for (const auto& value : values)
        result.add(value);

    return result;
}


#endif //CODATAP_LUAAPI_H
