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
    static Branch math_ ;

    static double Abs(double x);
    static double Pow(double a, double b);
    static double Root(double n, double root);
    static double Sqrt(float a);

    template<class ... Args>
    static int LCM(Args... numbers);

    template<class ... Args>
    static int GCD(Args... numbers);
};

