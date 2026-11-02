//
// Created by ws on 2026/10/28.
//

#include "CdaPI.h"

#include "Center.h"
#include "codes/YmlManager.h"

Node CdaPI::GetYml(std::string path) {
    return YmlManager::Read(path);
}

void CdaPI::SetBranch(Branch &branch) {
    Center::SetBranch(branch);
}

void CdaPI::Log(std::string output) {
    Center::Log(output);
}
