//
// Created by ws on 2026/10/28.
//

#ifndef CODATA_CENTER_H
#define CODATA_CENTER_H
#include <string>

#include "codes/Branch.h"

class CenterInstance {
public:
    virtual void log(std::string output) const = 0;
    virtual void setBranch(Branch & branch) const = 0;
};

class Center {
public:
    static CenterInstance* cti;

    static void Log(std::string output);
    static void SetBranch(Branch& branch);
};

#endif //CODATA_CENTER_H
