//
// Created by ws on 2026/11/2.
//

#include "CdFunc.h"

#include <algorithm>
#include <utility>
#include "../Branch.h"
#include "../Data.h"

std::string CdFunc::Run(CommandArgument) const {
    return "Ciallo ～(∠・ω< )⌒☆ Something is wrong😭";
}

CdLambda::CdLambda(std::function<std::string(CommandArgument)> function) {
    func = std::move(function);
}

std::string CdLambda::Run(CommandArgument args) const {
    if (func)
        return func(args);
    return "Something is wrong in CdLambda 😰";
}

CdExec::CdExec(std::string cmd) {
    command = std::move(cmd);
}

std::string CdExec::Run(CommandArgument command_argument) const {
    if (command.empty()) return "Something is wrong in DeExec 🔥";

    return Data::ExecuteCommand(command);
}

