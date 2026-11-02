//
// Created by ws on 2026/10/28.
//

#include "../codes/YmlManager.h"

#include <fstream>
#include <yaml-cpp/node/parse.h>

struct SType;
std::map<std::filesystem::path, Node> YmlManager::dict;

void YmlManager::OnClose() {
    for (const auto& i : dict) {
        Save(&i.second, i.first);
    }
}

Node YmlManager::Load(const std::filesystem::path &path) {
    auto n = std::filesystem::is_regular_file(path) ? YAML::LoadFile(path) : YAML::Node();
    return n;
}

void YmlManager::Save(const Node *node, std::filesystem::path path) {
    std::ofstream fout(path);
    fout << node;
    fout.close();
}

Node YmlManager::Read(std::filesystem::path path) {
    auto p = dict.find(path);
    if (p == dict.end()) {
        return dict[path] = Load(path);
    }
    else {
        return p->second;
    }

}
