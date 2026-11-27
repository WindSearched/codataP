//
// Created by ws on 2026/11/26.
//

#ifndef CODATAP_PLUGINSMAN_H
#define CODATAP_PLUGINSMAN_H
#include <filesystem>


class PluginsMan {
public:
    static bool Load(const std::filesystem::path& path);
};


#endif //CODATAP_PLUGINSMAN_H
