//
// Created by ws on 2026/10/25.
//

#include "math.h"
#include <cmath>
#include <numeric>

Branch math::math_ = Branch("math")
        .AddBranch(Branch("gcd")
            .AddArgument(BranchArgument("a"))
            .AddArgument(BranchArgument("b"))
            .AddExecution(
                    [](CommandArgument args) -> std::string {
                        int a,b;
                        if (!args.TryGetInt("a", a)) return "Arg 'a' is not valid";
                        if (!args.TryGetInt("b", b)) return "Arg 'b' is not valid";

                        int c = GCD(a,b);
                        return Core::ToString(c);
                    }
                )
        )
        .AddBranch(Branch("lcm")
            .AddArgument(BranchArgument("a"))
            .AddArgument(BranchArgument("b"))
            .AddExecution(
                    [](CommandArgument args) -> std::string {
                        int a,b;
                        if (!args.TryGetInt("a", a)) return "Arg 'a' is not valid";
                        if (!args.TryGetInt("b", b)) return "Arg 'b' is not valid";

                        int c = LCM(a,b);
                        return Core::ToString(c);
                    }
                )
        )
    ;

double math::Abs(double x) {
    return std::abs(x);
}

double math::Pow(double a, double b) {
    return std::pow(a,b);
}

double math::Root(double n, double root) {
    return Pow(n, 1/root);
}

double math::Sqrt(float a) {
    return std::sqrt(a);
}

template<class... Args>
int math::LCM(Args... numbers) {
    std::vector<int> v{ numbers... };
    if (v.empty()) return 0;

    int res = v[0];
    for (std::size_t i = 1; i < v.size(); ++i) {
        res = std::lcm(res, v[i]);
        if (res == 1) return 1;
    }
    return res;
}

template<class... Args>
int math::GCD(Args... numbers) {
    std::vector<int> v{ numbers... };
    if (v.empty()) return 0;

    int res = v[0];
    for (std::size_t i = 1; i < v.size(); ++i) {
        res = std::gcd(res, v[i]);
        if (res == 1) return 1;
    }
    return res;
}
