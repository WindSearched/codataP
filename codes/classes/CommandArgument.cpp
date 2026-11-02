//
// Created by ws on 2026/11/2.
//

#include "CommandArgument.h"

#include <stdexcept>


// --------------------------------------------------
// CommandArgument
// --------------------------------------------------

std::string CommandArgument::Get(const std::string& key) const {
    auto it = args.find(key);

    if (it == args.end()) {
        throw std::out_of_range("Command argument not found: " + key);
    }

    return it->second;
}

bool CommandArgument::TryGet(
    const std::string& key,
    std::string& value) const {

    auto it = args.find(key);

    if (it == args.end()) {
        return false;
    }

    value = it->second;
    return true;
}

int CommandArgument::GetInt(const std::string& key) const {
    return std::stoi(Get(key));
}

bool CommandArgument::TryGetInt(
    const std::string& key,
    int& value) const {

    std::string text;

    if (!TryGet(key, text)) {
        return false;
    }

    try {
        std::size_t pos = 0;
        int result = std::stoi(text, &pos);

        if (pos != text.size()) {
            return false;
        }

        value = result;
        return true;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool CommandArgument::GetBool(const std::string& key) const {
    const std::string value = Get(key);

    return value == "true" ||
           value == "1" ||
           value == "y";
}

bool CommandArgument::TryGetBool(
    const std::string& key,
    bool& value) const {

    std::string text;

    if (!TryGet(key, text)) {
        return false;
    }

    if (text == "true" || text == "1" || text == "y") {
        value = true;
        return true;
    }

    if (text == "false" || text == "0" || text == "n") {
        value = false;
        return true;
    }

    return false;
}

std::string CommandArgument::ParamGet(int index) const {
    return Get("param" + std::to_string(index));
}

bool CommandArgument::TryParamGet(
    int index,
    std::string& value) const {

    return TryGet("param" + std::to_string(index), value);
}

std::vector<std::string> CommandArgument::GetList() const {
    std::vector<std::string> result;

    for (int i = 1; ; ++i) {
        std::string value;

        if (!TryParamGet(i, value)) {
            break;
        }

        result.push_back(value);
    }

    return result;
}

void CommandArgument::Set(
    const std::string& key,
    const std::string& value) {

    args[key] = value;
}

std::vector<std::string> CommandArgument::GetArgs() const {
    std::vector<std::string> result;
    result.reserve(args.size());

    for (const auto& item : args) {
        result.push_back(item.second);
    }

    return result;
}

std::size_t CommandArgument::Count() const {
    return args.size();
}
