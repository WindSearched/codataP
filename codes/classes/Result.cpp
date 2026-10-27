//
// Created by ws on 2026/10/27.
//

#include "Result.h"

Result Result::isSuccess = Result{
    true,
    "Success"
};
Result Result::isFailed = Result{
    true,
    "Failed"
};

std::string Result::argNotExists(std::string arg) {
    return "The argument" + arg + "is not exists";
}
