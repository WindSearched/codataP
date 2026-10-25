//
// Created by ws on 2026/10/23.
//

#ifndef CODATAP_MATH_H
#define CODATAP_MATH_H
#include "../Branch.h"

#endif //CODATAP_MATH_H

class math {
    public:
    Branch math_ = Branch("math")
        .AddBranch(Branch("gcd")
            .AddArgument(BranchArgument("a"))
            .AddArgument(BranchArgument("b"))
            .AddExecution([](const CommandArgument& args) {
                return "hi";
            })
        )
    ;

    static double Abs(double x);
    static double Pow(double a, double b);
    static double Root(double n, double root);
    static double Sqrt(float a);
};

