//
// Created by ws on 2026/11/2.
//

#include "CdFunc.h"

#include <algorithm>
#include <utility>
#include "../Branch.h"
#include "../Data.h"
#include "../Sets.h"
#include "../plugins/lua/LuaMan.h"

std::string CdFunc::Run(CommandArgument) const {
    return "Ciallo ～(∠・ω< )⌒☆ Something is wrong😭";
}

CdLambda::CdLambda(std::function<std::string(CommandArgument)> function) {
    func = std::move(function);
}

std::string CdLambda::Run(CommandArgument args) const {
    if (func)
        return func(args);
    return "Something is wrong in CdLambda 😰";
}

CdExec::CdExec(std::string cmd) {
    command = std::move(cmd);
}

std::string CdExec::Run(CommandArgument args) const {
    if (command.empty()) return "Something is wrong in DeExec 🔥";

    string s = "";
    for (auto& a : args.args) {
        s += " " + a.second;
    }

    return Data::ExecuteCommand(command + s);
}

CdLuaFunc::CdLuaFunc(std::vector<std::string> loc) {
    location = std::move(loc);
}

std::string CdLuaFunc::Run(CommandArgument command_argument) const {
    auto f = LuaMan::GetFunction(location);
    if (f.valid()) {
        return f(command_argument);
    }
    else
        return "Lua-branch the location is not exist";
}
