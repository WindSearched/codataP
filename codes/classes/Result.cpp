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

std::string Result::argNotExists(const std::string& arg) {
    return "The argument" + arg + "is not exists";
}

Result::Result() : success(true), output("") {
}

// 两个参数的构造函数
Result::Result(bool success, std::string output) : success(success), output(std::move(output)) {
}