//
// Created by ws on 2026/10/26.
//

#include "Data.h"

#include <fstream>

#include "Core.h"

namespace fs = std::filesystem;
using string = std::string;

Branch Data::branch = Branch("data")
    .AddBranch(Branch("current")
        .AddExecution([](CommandArgument) -> string {
            return Data::currentPath.string();
            })
        )
    .AddBranch(Branch("fileExist")
        .AddArgument("path")
        .AddExecution([](CommandArgument args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            bool b = IsFile(path);
            return b ? "Exists" : "Not exists";
        }))
    .AddBranch(Branch("directoryExist")
        .AddArgument("path")
        .AddExecution([](CommandArgument args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            bool b = IsFile(path);
            return b ? "Exists" : "Not exists";
        }))
    .AddBranch(Branch("read")
        .AddArgument("path")
        .AddExecution([](CommandArgument args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            string t = Read(path);
            return t;
        }))
    .AddBranch(Branch("write")
        .AddArgument("path")
        .AddArgument("content")
        .AddArgument("open")
        .AddExecution([](CommandArgument args)->string {
            string path, content;
            bool open = args.GetBool("open");
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            if (!args.TryGet("content", content)) return "The argument content is not exists";

            bool b = Write(path, content);

            if (open) OpenTextDefault(path);

            return b ? "Success" : "Failure";
        }))
;
Branch Data::setsBranch = Branch("sets")
    .AddBranch(Branch("saveTube")
        .AddArgument("arg")
        .AddExecution([](CommandArgument args) -> string {
            bool a;

            if (args.TryGetBool("arg", a)) {
                Data::sets["saveTube"] = a;
                return "Set " + Core::ToString(a) + " " + (a ? "success" : "failure");
            }
            auto n = Data::sets["saveTube"];
            if (n)
                n = n;
            return n && n.as<bool>() ? "state: True" : "State: False";
        })
        )
;

std::filesystem::path Data::currentPath = fs::current_path();
std::filesystem::path Data::setsPath = currentPath / "sets.yml";
YAML::Node Data::sets = IsFile(setsPath) ? YAML::LoadFile(setsPath) : YAML::Node();

std::filesystem::path Data::ToPath(std::string path) {
    return fs::path(path);
}

bool Data::IsFile(std::filesystem::path path) {
    return std::filesystem::is_regular_file(path);
}

bool Data::IsDirectory(std::filesystem::path path) {
    return fs::is_directory(path);
}

std::string Data::Read(std::filesystem::path path) {
    std::ifstream file(path);
    if (!file.is_open()) return {};

    std::stringstream buffer;
    buffer << file.rdbuf();

    string content = buffer.str();
    return content;
}

bool Data::Write(std::filesystem::path path, std::string data) {
    if (path.has_parent_path())
        fs::create_directory(path.parent_path());

    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << data;

    return true;
}

bool Data::OpenTextDefault(std::filesystem::path path) {
    if (!fs::exists(path)) {
        return false;
    }

    // 1. 强烈建议转换为绝对路径，避免系统找不到文件
    std::string path_str = fs::absolute(path).string();
    std::string command;

    // 2. 根据操作系统自动拼接不同的系统内置命令
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    // Windows 系统: start "" "路径"
    // 注意：start 后面加一个空双引号 "" 是为了防止路径中带空格时被 start 误认作窗口标题
    command = "start \"\" \"" + path_str + "\"";
#elif defined(__APPLE__)
    // macOS 系统: open "路径"
    command = "open \"" + path_str + "\"";
#else
    // Linux / Ubuntu 等系统: xdg-open "路径"
    command = "xdg-open \"" + path_str + "\"";
#endif

    // 3. 执行系统命令
    std::system(command.c_str());

    return true;
}
