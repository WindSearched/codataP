//
// Created by ws on 2026/10/28.
//

#include "PluginLoader.h"

#include "../Data.h"
#include "../../CdPlugin/CdaPI.h"

namespace fs = std::filesystem;
typedef CdaPI* (*CDaFunc)();

std::map<SType, CdaPI*> PluginLoader::plugins;

void PluginLoader::LoadPlugins() {
    PluginLoader loader;

    if (!Data::IsDirectory(Data::pluginsPath)) {
        Data::CreateDirectory(Data::pluginsPath);
        return;
    }

    for (auto& file : fs::directory_iterator(Data::pluginsPath)) {
        if (file.is_regular_file()) {
            std::string name = file.path().stem().string();//the pure name without extension
            // 统一传入不带前缀和后缀的插件名
            if (loader.load(name)) {
                // 统一获取函数指针
                auto func = loader.get_symbol<CDaFunc>("cdFunc");
                if (func) {
                    CdaPI* plugin = func();

                    plugin->onStart();
                    plugins[SType(plugin->getName(), plugin->getAuthor())] = plugin;


                }
            }
        }
    }

}
