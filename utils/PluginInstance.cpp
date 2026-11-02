//
// Created by ws on 2026/10/28.
//

#include "PluginInstance.h"

#include "../codes/Core.h"

void PluginInstance::log(std::string output) const {
    Core::mainWindow->LogLine(output);
}

void PluginInstance::setBranch(Branch &branch) const {
    Core::branch.AddBranch(branch);
}
