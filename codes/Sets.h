//
// Created by ws on 2026/10/27.
//

#ifndef CODATAP_SETS_H
#define CODATAP_SETS_H
#include <yaml-cpp/binary.h>
#include <yaml-cpp/node/node.h>

using Node = YAML::Node;
using string = std::string;

class Sets {
    public:
    static YAML::Node sets;

    static std::string GetUser();
    static void SetUser(string user);
    static std::string GetPointPath();
    static void SetPointPath(const string& pointPath);
    static std::string GetLineHead();
};


#endif //CODATAP_SETS_H
