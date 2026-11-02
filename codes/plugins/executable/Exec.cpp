//
// Created by ws on 2026/11/2.
//

#include "Exec.h"

#include "../../Core.h"
#include "../../Data.h"

bool Exec::Load(std::filesystem::path path) {
    if (!Data::IsExecutable(path)) return false;

    std::string startPut = Data::ExecuteCommand(path.string() + " start");
    Core::mainWindow->LogLine(startPut);

    return true;
}
