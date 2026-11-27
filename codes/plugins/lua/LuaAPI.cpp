//
// Created by ws on 2026/11/26.
//

#include "LuaAPI.h"

#include <utility>

#include "../../Data.h"

void LuaAPI::DataReg(sol::state &state) {
    // 创建 Lua 全局表 Data
        sol::table data = state.create_named_table("Data");

        // 文件路径
        data.set_function("ToPath",
            [](const std::string& path)
            {
                return Data::ToPath(path).string();
            });

        data.set_function("GetProximatePath",
            [](const std::string& path)
            {
                return Data::GetProximatePath(path).string();
            });

        // 文件判断
        data.set_function("IsFile",
            [](const std::string& path)
            {
                return Data::IsFile(path);
            });

        data.set_function("IsDirectory",
            [](const std::string& path)
            {
                return Data::IsDirectory(path);
            });

        data.set_function("IsExecutable",
            [](const std::string& path)
            {
                return Data::IsExecutable(path);
            });

        // 扩展名判断（两个重载）
        data.set_function("IsExtension",
            sol::overload(
                [](const std::string& path, const std::string& ext)
                {
                    return Data::IsExtension(
                        std::filesystem::path(path), ext
                    );
                },
                [](const std::string& path, const std::string& ext)
                {
                    return Data::IsExtension(path, ext);
                }
            )
        );

        // 文件读写
        data.set_function("Read",
            [](const std::string& path)
            {
                return Data::Read(path);
            });

        data.set_function("Write",
            [](const std::string& path, const std::string& content)
            {
                return Data::Write(path, content);
            });

        data.set_function("CreateDirectory",
            [](const std::string& path)
            {
                Data::CreateDirectory(path);
            });

        data.set_function("OpenTextDefault",
            [](const std::string& path)
            {
                return Data::OpenTextDefault(path);
            });

        // 命令执行
        data.set_function("ExecuteCommand",
            &Data::ExecuteCommand
        );

        // HTTP
        data.set_function("HttpGet",
            &Data::HttpGet
        );
}

void LuaAPI::BranchReg(sol::state &state) {
    // BranchArgument
    state.new_usertype<BranchArgument>(
        "BranchArgument",

        sol::constructors<
            BranchArgument(std::string)
        >(),

        "arg", &BranchArgument::arg,

        "SetSuggestion", &BranchArgument::SetSuggestion,
        "GetSuggestion", &BranchArgument::GetSuggestion,

        "boolean", &BranchArgument::boolean
    );

    // Branch
    state.new_usertype<Branch>(
        "Branch",

        sol::constructors<
            Branch(std::string)
        >(),

        // 字段
        "name", &Branch::name,
        "description", &Branch::description,
        "param", &Branch::param,
        "abbreviation", &Branch::abbreviation,

        // 添加子命令
        "AddBranch",
        sol::overload(
            [](Branch& self, Branch branch) -> Branch& {
                return self.AddBranch(std::move(branch));
            },
            [](Branch& self, std::string name) -> Branch& {
                return self.AddBranch(std::move(name));
            }
        ),

        "AddBranchReturnSelf",
        sol::overload(
            [](Branch& self, Branch branch) {
                return self.AddBranchReturnSelf(std::move(branch));
            },
            [](Branch& self, std::string name) {
                return self.AddBranchReturnSelf(std::move(name));
            }
        ),

        "AddBranches", [](Branch& self, sol::table branches) -> Branch& {
            std::vector<Branch> values;

            for (std::size_t i = 1; ; ++i)
            {
                sol::object obj = branches[i];

                if (obj == sol::nil)
                    break;

                values.push_back(obj.as<Branch>());
            }

            return self.AddBranches(std::move(values));
        },

        // 添加参数
        "AddArgument",
        sol::overload(
            [](Branch& self, BranchArgument argument) -> Branch& {
                return self.AddArgument(std::move(argument));
            },
            [](Branch& self, std::string name) -> Branch& {
                return self.AddArgument(std::move(name));
            }
        ),

        // 设置执行函数
        "AddExecution", &Branch::AddExecution,

        "SetParam", &Branch::SetParam,

        // Parse
        "Parse", [](Branch& self, sol::table split, CommandArgument& args) {
            return self.Parse(TableToStrings(split), args);
        },

        // Run
        "Run", &Branch::Run,

        // Command
        "Command", [](Branch& self, sol::table split) {
            return self.Command(TableToStrings(split));
        },

        // 获取子命令和参数
        "GetBranches", [](Branch& self, sol::this_state state) {
            return VectorToTable(
                sol::state_view(state),
                self.branches
            );
        },

        "GetArguments", [](Branch& self, sol::this_state state) {
            return VectorToTable(
                sol::state_view(state),
                self.args
            );
        },

        // 静态方法
        "ToString", sol::overload(
            [](Branch& branch) {
                return Branch::ToString(branch);
            },
            [](sol::table branches) {
                std::vector<Branch> values;

                for (std::size_t i = 1; ; ++i)
                {
                    sol::object obj = branches[i];

                    if (obj == sol::nil)
                        break;

                    values.push_back(obj.as<Branch>());
                }

                return Branch::ToString(values);
            }
        ),

        "FromString", [](const sol::state_view& lua, std::string text) {
            auto values = Branch::FromString(std::move(text));
            return VectorToTable(lua, values);
        }
    );
}

void LuaAPI::NewReg(sol::state &state) {
    sol::table n = state.create_named_table("New");
    n.set_function("Branch",
        [](const std::string& name) -> Branch {
        return Branch(name);
    });
}

std::vector<std::string> LuaAPI::TableToStrings(const sol::table &table) {
    std::vector<std::string> result;

    for (std::size_t i = 1; ; ++i)
    {
        sol::object obj = table[i];

        if (obj == sol::nil)
            break;

        result.push_back(obj.as<std::string>());
    }

    return result;
}
