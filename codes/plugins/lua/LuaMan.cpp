//
// Created by ws on 2026/11/1.
//

#include "LuaMan.h"

#include <utility>

#include "LuaAPI.h"
#include "../../Core.h"
#include "../../Data.h"
#include "../../Sets.h"
#define func lua.set_function

sol::state LuaMan::lua;
sol::table LuaMan::table = lua.create_table();

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

    LuaAPI::DataReg(lua);
    LuaAPI::NewReg(lua);
    LuaAPI::BranchReg(lua);
}

bool LuaMan::Load(std::filesystem::path path)
{
    if (!Data::IsFile(path))
        return false;

    try
    {
        sol::protected_function_result result =
            lua.safe_script_file(path.string());

        if (!result.valid())
        {
            sol::error err = result;
            Core::mainWindow->LogLine(
                std::string("[Lua Error] ") + err.what()
            );
            return false;
        }

        sol::table t = result;
        string s = t["name"];
        table[s] = t;

        sol::object startObj = t["OnStart"];

        if (startObj.is<sol::protected_function>())
        {
            sol::protected_function onStart =
                startObj.as<sol::protected_function>();

            auto startResult = onStart();

            if (!startResult.valid())
            {
                sol::error err = startResult;
                Core::mainWindow->LogLine(err.what());
                return false;
            }
        }

        // 获取 OnClose
        sol::object closeObj = t["OnClose"];

        if (closeObj.is<sol::protected_function>())
        {
            sol::protected_function onClose =
                closeObj.as<sol::protected_function>();

            Core::OnStop.Add([onClose]()
            {
                auto result = onClose();

                if (!result.valid())
                {
                    sol::error err = result;
                    Core::mainWindow->LogLine(err.what());
                    return false;
                }
            });
        }

        sol::object branchesObj = t["RegBranches"];

        if (branchesObj.is<sol::protected_function>())
        {
            sol::protected_function branches =
                branchesObj.as<sol::protected_function>();

            auto result = branches();
            if (!result.valid()){
                sol::error err = result;
                Core::mainWindow->LogLine(err.what());
                return false;
            }

            // 检查是否真的返回了值
            if (result.return_count() == 0)
            {
                Core::mainWindow->LogLine("RegBranches returned nothing!");
                return false;
            }

            // 检查返回值类型
            sol::object obj = result.get<sol::object>();

            std::vector<Branch> bs;
            if (obj.get_type() == sol::type::table)
            {
                sol::table a = obj.as<sol::table>();
                bs = ToVector<Branch>(a);
            }
            else if (obj.get_type() == sol::type::userdata) {
                if (obj.is<std::vector<Branch>>()) {
                    bs = obj.as<std::vector<Branch>>();
                }
            }
            else {
                Core::mainWindow->LogLine("The type is not supported " +
                    sol::type_name(lua, obj.get_type()));
                return false;
            }

            Core::branch.AddBranches(bs);
        }

        return true;
    }
    catch (const sol::error& e)
    {
        Core::mainWindow->LogLine(
            std::string("[Lua Error] ") + e.what()
        );
        return false;
    }
}

void LuaMan::Read(string content) {
    lua.script(content);
}

bool LuaMan::Read(std::filesystem::path path) {
    if (!Data::IsFile(path)) return false;
    lua.script_file(path);
    return true;
}

sol::protected_function LuaMan::GetFunction(std::vector<std::string> location) {
    sol::table t = table;
    for (const auto& l : location) {
        t = t[l];
        if (!t.valid())
            return nullptr;
    }
    return t;
}
