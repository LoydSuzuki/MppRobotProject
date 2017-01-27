//
//  CubicSpline.hpp
//  SimpleExample
//
//  Created by TATSUHIKO SUZUKI M++ on 2017/01/12.
//
//

#ifndef CubicSpline_h
#define CubicSpline_h

#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;

class CubicSpline{
public:
    CubicSpline(const vector<double> &y);
    double Calc(double t);
    
private:
    vector<double> a_;
    vector<double> b_;
    vector<double> c_;
    vector<double> d_;
    vector<double> w_;
    
    void InitParameter(const vector<double> &y);
};


#endif /* CubicSpline_hpp */
