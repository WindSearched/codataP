#pragma once
#include <string>
#include <iostream>
#include <map>

#include "SType.h"
#include "../../CdPlugin/CdaPI.h"

#if defined(_WIN32)
    #include <windows.h>
    using PluginHandle = HMODULE;
#else
#include <dlfcn.h>
using PluginHandle = void*;
#endif

class PluginLoader {
private:
    PluginHandle handle = nullptr;

public:
    static void LoadPlugins();
    static std::map<SType, CdaPI*> plugins;

    PluginLoader() = default;
    ~PluginLoader() { unload(); }

    // 跨平台加载：会自动根据系统自动补全后缀，如输入 "my_plugin"
    bool load(const std::string& plugin_name) {
        std::string full_path = plugin_name;
#if defined(_WIN32)
        full_path += ".dll";
        handle = LoadLibraryA(full_path.c_str());
#else
        full_path = "./lib" + full_path + ".so";
        handle = dlopen(full_path.c_str(), RTLD_LAZY);
#endif
        if (!handle) {
            std::cerr << "加载插件失败: " << full_path << std::endl;
            return false;
        }
        return true;
    }

    // 跨平台获取函数指针
    template<typename T>
    T get_symbol(const std::string& symbol_name) {
        if (!handle) return nullptr;
#if defined(_WIN32)
        return reinterpret_cast<T>(GetProcAddress(handle, symbol_name.c_str()));
#else
        return reinterpret_cast<T>(dlsym(handle, symbol_name.c_str()));
#endif
    }

    void unload() {
        if (handle) {
#if defined(_WIN32)
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
            handle = nullptr;
        }
    }
};
