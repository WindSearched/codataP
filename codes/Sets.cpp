//
// Created by ws on 2026/10/27.
//

#include "Sets.h"

#include "Data.h"

YAML::Node Sets::sets = Data::IsFile(Data::setsPath) ? YAML::LoadFile(Data::setsPath) : YAML::Node();


std::string Sets::GetUser() {
    Node n = sets["user"];
    return n ? n.as<string>() : "codata";
}

void Sets::SetUser(string user) {
    sets["user"] = user;
}

std::string Sets::GetPointPath() {
    Node n = sets["pointPath"];
    auto p = n ? n.as<string>() : Data::currentPath.string();
    return Data::GetProximatePath(p);
}

void Sets::SetPointPath(const string& pointPath) {
    sets["pointPath"] = pointPath;
}

std::string Sets::GetLineHead() {
    return GetUser() + ":" + GetPointPath() + ">";
}
