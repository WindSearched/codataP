//
// Created by ws on 2026/10/24.
//

#include "tmain.h"

#include <functional>
#include <iostream>
#include <unordered_map>

using string = std::string;
using func = std::function<int(string[])>;

std::string runCmd(const std::string& cmd, int& exitCode) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen failed");

    std::string out;
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), pipe)) > 0)
        out.append(buf, n);

    int status = pclose(pipe);
    if (status == -1) throw std::runtime_error("pclose failed");

    // 解析退出码
#if defined(_WIN32)
    exitCode = status;
#else
    if (WIFEXITED(status)) exitCode = WEXITSTATUS(status);
    else                   exitCode = -1;
#endif
    return out;
}

std::unordered_map<string, func> registry{
    {"test", [](string args[])-> int{
        std::cout << "testing" << std::endl;
        return 0;
    }},
    {"help", [](string args[]) ->int {
        for (auto& a : registry) {
            std::cout << a.first << std::endl;
        }
        return 0;
    }}
};

int main(int argc, char* argv[])
{
    string command = argv[2];
    string args[argc];
    for (int i = 3 ; i < argc ; i++) {
        args[i - 3] = argv[i];
    }

    auto f = registry.find(command);
    if (f == registry.end()) {
        std::cout << "unknown command: " << command << std::endl;
    }
    return f->second(args);

}