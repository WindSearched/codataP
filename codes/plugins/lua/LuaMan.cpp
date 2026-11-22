//
// Created by ws on 2026/11/1.
//

#include "LuaMan.h"

#include <utility>

#include "../../Core.h"
#include "../../Data.h"
#include "../../Sets.h"
#define func lua.set_function

sol::state LuaMan::lua = sol::state();

Branch LuaMan::branch_ = Branch("lua")
    .AddBranch(Branch("path")
        .AddArgument("path")
        .AddExecution([](const CommandArgument& arg)-> string {
            string path;
            if (!arg.TryGet("path", path)) return "Failed";
            Read(std::filesystem::path(path));
            return "Success";
        })
        )
    .AddBranch(Branch("plg")
        .AddArgument("path")
        .AddExecution([](const CommandArgument& arg)-> string {
            string path;
            if (!arg.TryGet("path", path)) return "Failed";
            return Load(path) ? "Success" : "Failed, the path is not exist";
            })
        )
    .AddArgument("content")
    .AddExecution([](CommandArgument arg)-> string {
        string content;
        if (!arg.TryGet("content", content)) return "Failed";
        Read(content);
        return "Success";
    })
;

void LuaMan::Register() {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::string,
        sol::lib::table,
        sol::lib::math
    );
    func("Log",
        [](string msg){Core::mainWindow->LogLine(std::move(msg));});
    func("Cmd",
        [](string command){ Core::ExecuteCommand(std::move(command));});
}

bool LuaMan::Load(std::filesystem::path path) {
    if (!Data::IsFile(path)) return false;
    sol::table t = lua.script_file(path);

    sol::protected_function onStart = t["OnStart"];
    if (onStart.valid()) {
        onStart();
    }

    sol::protected_function onClose = t["OnClose"];
    if (onClose.valid()) {
        Core::OnStop.Add([onClose]() {
           onClose();
        });
    }

    return true;
}

void LuaMan::Read(string content) {
    lua.script(content);
}

bool LuaMan::Read(std::filesystem::path path) {
    if (!Data::IsFile(path)) return false;
    lua.script_file(path);
    return true;
}
