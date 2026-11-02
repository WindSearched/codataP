//
// Created by ws on 2026/10/28.
//

#include "Center.h"

CenterInstance * Center::cti = nullptr;

void Center::Log(std::string output) {
    cti->log(output);
}

void Center::SetBranch(Branch &branch) {
    cti->setBranch(branch);
}
