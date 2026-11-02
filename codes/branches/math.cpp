//
// Created by ws on 2026/10/25.
//

#include "math.h"
#include <cmath>
#include <numeric>

// 辅助宏：注册一个二元 double 函数
#define REG_2FUNC(name, functor)                                    \
    .AddBranch(Branch(name)                                        \
        .AddArgument(BranchArgument("a"))                          \
        .AddArgument(BranchArgument("b"))                          \
        .AddExecution([](CommandArgument args) -> std::string {    \
            double a, b;                                            \
            if (!args.TryGetDouble("a", a)) return "Arg 'a' is not valid"; \
            if (!args.TryGetDouble("b", b)) return "Arg 'b' is not valid"; \
            return Core::ToString(functor(a, b));                  \
        }))

// 辅助宏：注册一个一元 double 函数
#define REG_1FUNC(name, functor)                                    \
    .AddBranch(Branch(name)                                        \
        .AddArgument(BranchArgument("a"))                          \
        .AddExecution([](CommandArgument args) -> std::string {    \
            double a;                                               \
            if (!args.TryGetDouble("a", a)) return "Arg 'a' is not valid"; \
            return Core::ToString(functor(a));                     \
        }))

Branch math::math_ = Branch("math")
    // ... 原有的 gcd、lcm 分支 ...
    REG_1FUNC("abs",   math::Abs)
    REG_1FUNC("floor", math::Floor)
    REG_1FUNC("ceil",  math::Ceil)
    REG_1FUNC("round", math::Round)
    REG_1FUNC("trunc", math::Trunc)

    REG_2FUNC("pow",   math::Pow)
    REG_2FUNC("root",  math::Root)
    REG_2FUNC("fmod",  math::Fmod)
    REG_2FUNC("hypot", math::Hypot)
    REG_2FUNC("min",   math::Min)
    REG_2FUNC("max",   math::Max)
    REG_2FUNC("atan2", math::Atan2)

    REG_1FUNC("sqrt",  math::Sqrt)
    REG_1FUNC("cbrt",  math::Cbrt)
    REG_1FUNC("sin",   math::Sin)
    REG_1FUNC("cos",   math::Cos)
    REG_1FUNC("tan",   math::Tan)
    REG_1FUNC("asin",  math::Asin)
    REG_1FUNC("acos",  math::Acos)
    REG_1FUNC("atan",  math::Atan)
    REG_1FUNC("exp",   math::Exp)
    REG_1FUNC("log",   math::Log)
    REG_1FUNC("log10", math::Log10)
    REG_1FUNC("log2",  math::Log2)

    REG_1FUNC("degtorad", math::DegToRad)
    REG_1FUNC("radtodeg", math::RadToDeg)

    // 整数专用
    .AddBranch(Branch("factorial")
        .AddArgument(BranchArgument("n"))
        .AddExecution([](CommandArgument args) -> std::string {
            int n;
            if (!args.TryGetInt("n", n)) return "Arg 'n' is not valid";
            return Core::ToString(math::Factorial(n));
        }))
    .AddBranch(Branch("isprime")
        .AddArgument(BranchArgument("n"))
        .AddExecution([](CommandArgument args) -> std::string {
            int n;
            if (!args.TryGetInt("n", n)) return "Arg 'n' is not valid";
            return math::IsPrime(n) ? "true" : "false";
        }))
    .AddBranch(Branch("fib")
        .AddArgument(BranchArgument("n"))
        .AddExecution([](CommandArgument args) -> std::string {
            int n;
            if (!args.TryGetInt("n", n)) return "Arg 'n' is not valid";
            return Core::ToString(math::Fib(n));
        }))
    .AddBranch(Branch("clamp")
        .AddArgument(BranchArgument("x"))
        .AddArgument(BranchArgument("lo"))
        .AddArgument(BranchArgument("hi"))
        .AddExecution([](CommandArgument args) -> std::string {
            double x, lo, hi;
            if (!args.TryGetDouble("x",  x))  return "Arg 'x' is not valid";
            if (!args.TryGetDouble("lo", lo)) return "Arg 'lo' is not valid";
            if (!args.TryGetDouble("hi", hi)) return "Arg 'hi' is not valid";
            return Core::ToString(math::Clamp(x, lo, hi));
        }))
    .AddBranch(Branch("sign")
        .AddArgument(BranchArgument("a"))
        .AddExecution([](CommandArgument args) -> std::string {
            double a;
            if (!args.TryGetDouble("a", a)) return "Arg 'a' is not valid";
            return Core::ToString(math::Sign(a));
        }))
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
// ---------------- 取整 / 取余 ----------------
double math::Floor(double x) { return std::floor(x); }
double math::Ceil(double x)  { return std::ceil(x); }
double math::Round(double x) { return std::round(x); }
double math::Trunc(double x) { return std::trunc(x); }
double math::Fmod(double a, double b) { return std::fmod(a, b); }

// ---------------- 三角函数（弧度） ----------------
double math::Sin(double x) { return std::sin(x); }
double math::Cos(double x) { return std::cos(x); }
double math::Tan(double x) { return std::tan(x); }
double math::Asin(double x) { return std::asin(x); }
double math::Acos(double x) { return std::acos(x); }
double math::Atan(double x) { return std::atan(x); }
double math::Atan2(double y, double x) { return std::atan2(y, x); }

// ---------------- 指数 / 对数 ----------------
double math::Exp(double x)   { return std::exp(x); }
double math::Log(double x)   { return std::log(x); }
double math::Log10(double x) { return std::log10(x); }
double math::Log2(double x)  { return std::log2(x); }

// ---------------- 其它 ----------------
double math::Cbrt(double x) { return std::cbrt(x); }
double math::Hypot(double a, double b) { return std::hypot(a, b); }

double math::Min(double a, double b) { return std::min(a, b); }
double math::Max(double a, double b) { return std::max(a, b); }

int math::AbsInt(int x)    { return std::abs(x); }
int math::MinInt(int a, int b) { return std::min(a, b); }
int math::MaxInt(int a, int b) { return std::max(a, b); }

// ---------------- 整数算法 ----------------
long long math::Factorial(int n) {
    if (n < 0) return 0;
    long long r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

bool math::IsPrime(long long n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (long long i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

long long math::Fib(int n) {
    if (n < 0) return 0;
    long long a = 0, b = 1;
    for (int i = 0; i < n; ++i) { long long t = a + b; a = b; b = t; }
    return a;
}

// ---------------- 角度 / 工具 ----------------
double math::DegToRad(double deg) { return deg * 3.14159265358979323846 / 180.0; }
double math::RadToDeg(double rad) { return rad * 180.0 / 3.14159265358979323846; }

double math::Clamp(double x, double lo, double hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}
int math::Sign(double x) {
    return (x > 0) - (x < 0);
}