//
// Created by ws on 2026/10/23.
//

#ifndef CODATAP_MATH_H
#define CODATAP_MATH_H
#include "../Branch.h"
#include "../Core.h"

#endif //CODATAP_MATH_H

class math {
public:
    static Branch math_;

    // 已有
    static double Abs(double x);
    static double Pow(double a, double b);
    static double Root(double n, double root);
    static double Sqrt(float a);

    template<class ... Args>
    static int LCM(Args... numbers);
    template<class ... Args>
    static int GCD(Args... numbers);

    // ---- 新增 ----
    static double Floor(double x);
    static double Ceil(double x);
    static double Round(double x);
    static double Trunc(double x);
    static double Fmod(double a, double b);

    static double Sin(double x);      // 弧度
    static double Cos(double x);
    static double Tan(double x);
    static double Asin(double x);
    static double Acos(double x);
    static double Atan(double x);
    static double Atan2(double y, double x);

    static double Exp(double x);
    static double Log(double x);      // 自然对数
    static double Log10(double x);
    static double Log2(double x);

    static double Cbrt(double x);
    static double Hypot(double a, double b);

    static double Min(double a, double b);
    static double Max(double a, double b);

    static int AbsInt(int x);
    static int MinInt(int a, int b);
    static int MaxInt(int a, int b);

    static long long Factorial(int n);   // n!
    static bool IsPrime(long long n);
    static long long Fib(int n);         // 斐波那契

    static double DegToRad(double deg);
    static double RadToDeg(double rad);

    static double Clamp(double x, double lo, double hi);
    static int Sign(double x);
};

