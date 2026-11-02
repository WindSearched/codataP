//
// Created by ws on 2026/10/28.
//

#ifndef CODATA_STYPE_H
#define CODATA_STYPE_H
#include <string>

#endif //CODATA_STYPE_H

struct SType {
    std::string name;
    std::string tag;

    SType(std::string name, std::string tag) {
        this->name = name;
        this->tag = tag;
    };

    bool operator<(const SType& other) const {
        if (this->name != other.name) {
            return this->name < other.name; // 先按 name 字母序排序
        }
        return this->tag < other.tag;       // name 相同再按 tag 排序
    }
};