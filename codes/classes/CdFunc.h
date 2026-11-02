//
// Created by ws on 2026/11/2.
//

#ifndef CODATAP_CDFUNC_H
#define CODATAP_CDFUNC_H
#include <functional>
#include <string>


class CommandArgument;

class CdFunc {
public:
    virtual std::string Run(CommandArgument) const;
};

class CdLambda : public CdFunc {
public:
    explicit CdLambda(std::function<std::string(CommandArgument)>);

    std::string Run(CommandArgument) const override;
    std::function<std::string(CommandArgument)> func;
};

class CdExec : public CdFunc {
public:
    explicit CdExec(std::string);

    std::string Run(CommandArgument) const override;
    std::string command;
};


#endif //CODATAP_CDFUNC_H
