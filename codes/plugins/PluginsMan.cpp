//
// Created by ws on 2026/11/26.
//

#include "PluginsMan.h"

#include "../Data.h"
#include "executable/Exec.h"
#include "lua/LuaMan.h"

bool PluginsMan::Load(const std::filesystem::path& path) {
    if (!Data::IsDirectory(path)) return false;

    for (const auto& file : std::filesystem::directory_iterator(path)) {
        const auto& filePath = file.path();

        std::string extension = filePath.extension();
        if (extension == ".lua") {
            if (!LuaMan::Load(filePath)) return false;
            continue;
        }

        if (Data::IsExecutable(filePath)) {
            Exec::Load(filePath);
            continue;
        }
    }
    return true;
}
