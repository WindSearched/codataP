//
// Created by ws on 2026/10/27.
//

#ifndef CODATAP_RESULT_H
#define CODATAP_RESULT_H
#include <string>


struct Result {
    bool success = true;
    std::string output;

    Result();
    Result(bool success, std::string output);

    static Result isSuccess, isFailed;
    static std::string argNotExists(std::string arg);
};


#endif //CODATAP_RESULT_H
