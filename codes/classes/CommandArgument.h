//
// Created by ws on 2026/11/2.
//

#ifndef CODATAP_COMMANDARGUMENT_H
#define CODATAP_COMMANDARGUMENT_H
#include <string>
#include <unordered_map>
#include <vector>


class CommandArgument {
public:
    std::unordered_map<std::string, std::string> args;

    std::string Get(const std::string& key) const;
    bool TryGet(const std::string& key, std::string& value) const;

    int GetInt(const std::string& key) const;
    bool TryGetInt(const std::string& key, int& value) const;

    bool GetBool(const std::string& key) const;
    bool TryGetBool(const std::string& key, bool& value) const;

    bool TryGetDouble(const std::string& key, double& value) const;

    std::string ParamGet(int index) const;
    bool TryParamGet(int index, std::string& value) const;

    std::vector<std::string> GetList() const;

    void Set(const std::string& key, const std::string& value);

    std::vector<std::string> GetArgs() const;

    std::size_t Count() const;
};

#endif //CODATAP_COMMANDARGUMENT_H
