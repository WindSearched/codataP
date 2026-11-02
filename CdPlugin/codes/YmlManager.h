//
// Created by ws on 2026/10/28.
//

#ifndef CODATA_YMLMANAGER_H
#define CODATA_YMLMANAGER_H
#include <filesystem>
#include <map>
#include <vector>
#include <yaml-cpp/node/node.h>

#include "../../codes/Sets.h"

class YmlManager {
public:
    static void OnClose();

    static std::map<std::filesystem::path, Node> dict;

    static Node Load(const std::filesystem::path &path);
    static void Save(const Node* node, std::filesystem::path path);
    static Node Read(std::filesystem::path);
};


#endif //CODATA_YMLMANAGER_H
