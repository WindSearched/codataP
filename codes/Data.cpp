//
// Created by ws on 2026/10/26.
//

#include "Data.h"

#include <fstream>
#include <curl/curl.h>

#include "Core.h"
#include "Sets.h"

namespace fs = std::filesystem;
using string = std::string;

Branch Data::branch = Branch("data")
    .AddBranch(Branch("current")
        .AddExecution([](const CommandArgument&) -> string {
            return Data::currentPath.string();
            })
        )
    .AddBranch(Branch("fileExist")
        .AddArgument("path")
        .AddExecution([](const CommandArgument &args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            bool b = IsFile(path);
            return b ? "Exists" : "Not exists";
        }))
    .AddBranch(Branch("directoryExist")
        .AddArgument("path")
        .AddExecution([](const CommandArgument &args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            bool b = IsFile(path);
            return b ? "Exists" : "Not exists";
        }))
    .AddBranch(Branch("read")
        .AddArgument("path")
        .AddExecution([](const CommandArgument &args)->string {
            string path;
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            string t = Read(path);
            return t;
        }))
    .AddBranch(Branch("write")
        .AddArgument("path")
        .AddArgument("content")
        .AddArgument("open")
        .AddExecution([](const CommandArgument &args)->string {
            string path, content;
            bool open = args.GetBool("open");
            if (!args.TryGet("path", path)) return "The argument path is not exists";
            if (!args.TryGet("content", content)) return "The argument content is not exists";

            bool b = Write(path, content);

            if (open) OpenTextDefault(path);

            return b ? "Success" : "Failure";
        }))
    .AddBranch(Branch("httpGet")
        .AddBranch(Branch("test")
            .AddExecution([](const CommandArgument&)-> string {
                return HttpGet("https://example.com/");
            })
            )
        .AddArgument("url")
        .AddExecution([](const CommandArgument &args) -> string {
            string url;
            if (!args.TryGet("url", url)) return "The argument 'url' is not exists";
            auto content = HttpGet(url);
            return content;
        })
    )
;
Branch Data::setsBranch = Branch("sets")
    .AddBranch(Branch("saveTube")
        .AddArgument(BranchArgument::boolean("arg"))
        .AddExecution([](const CommandArgument& args) -> string {
            bool a;

            if (args.TryGetBool("arg", a)) {
                Sets::sets["saveTube"] = a;
                return "Set " + Core::ToString(a) + " " + (a ? "success" : "failure");
            }
            auto n = Sets::sets["saveTube"];
            return n && n.as<bool>() ? "state: True" : "State: False";
        })
        )
;

std::filesystem::path Data::currentPath = fs::current_path();
std::filesystem::path Data::setsPath = currentPath / "sets.yml";
std::filesystem::path Data::pluginsPath = currentPath / "plugins";

std::filesystem::path Data::ToPath(const std::string &path) {
    return fs::path(path);
}

bool Data::IsFile(const std::filesystem::path &path) {
    return std::filesystem::is_regular_file(path);
}

bool Data::IsDirectory(const std::filesystem::path& path) {
    return fs::is_directory(path);
}

std::filesystem::path Data::GetProximatePath(const std::filesystem::path &path) {
    return fs::proximate(path);
}

bool Data::IsExecutable(const std::filesystem::path& p) {
    if (!fs::exists(p) || fs::is_directory(p)) {
        return false;
    }

    auto perms = fs::status(p).permissions();

    // 检查所有者、所属组或其他用户是否拥有执行权限
    return ((perms & fs::perms::owner_exec) != fs::perms::none ||
            (perms & fs::perms::group_exec) != fs::perms::none ||
            (perms & fs::perms::others_exec) != fs::perms::none);
}

bool Data::IsExtension(const std::filesystem::path& path, const std::string& extension) {
    return path.extension() == extension;
}

bool Data::IsExtension(const std::string& path, const std::string& extension) {
    auto p = fs::path(path);
    return p.extension() == extension;
}

std::string Data::Read(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return {};

    std::stringstream buffer;
    buffer << file.rdbuf();

    string content = buffer.str();
    return content;
}

bool Data::Write(const std::filesystem::path& path, const std::string& data) {
    if (path.has_parent_path())
        fs::create_directory(path.parent_path());

    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << data;

    return true;
}

void Data::CreateDirectory(const std::filesystem::path& path) {
    fs::create_directories(path);
}

bool Data::OpenTextDefault(const std::filesystem::path& path) {
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

#ifdef _Win32
#define popen _popen
#define pclose _pclose
#endif


std::string Data::ExecuteCommand(const std::string& command) {
    std::array<char, 128> buffer{};
    string result;

    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        return "popen() failed";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

size_t Data::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    auto* result = static_cast<std::string *>(userp);

    result->append(static_cast<char *>(contents), totalSize);

    return totalSize;
}

std::string Data::HttpGet(const std::string &url) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 超时时间
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // 跟随重定向
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            response.clear();
            std::cerr << "请求失败: "
                      << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

YAML::Node Sets::sets = Data::IsFile(Data::setsPath) ? YAML::LoadFile(Data::setsPath.string()) : YAML::Node();
