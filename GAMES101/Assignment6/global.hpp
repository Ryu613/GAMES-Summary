#pragma once
#include <iostream>
#include <cmath>
#include <random>

#undef M_PI
#define M_PI 3.141592653589793f

// 指EPSILON变量在别的地方定义，告诉编译器去别处找EPSILON等于多少
extern const float  EPSILON;
// numeric_limits取每种类型的极值，因为这个极值可能跟平台有关，不同平台上，数据类型的极值不一样
const float kInfinity = std::numeric_limits<float>::max();

// inline适用于短小的，重复频繁调用的函数
// inline 避免频繁消耗栈空间,inline编译时会换成具体的表达式，而不是函数调用
// inline是否真正内联取决于编译器
// inline一般放在头文件中，要放在函数的定义里，只放在函数的声明里没用
inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

// 解一元二次方程,abc为系数，x0x1为从小到大排列的两个解
inline  bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

// 获取随机浮点数
inline float get_random_float()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.f, 1.f); // distribution in range [1, 6]

    return dist(rng);
}

// 更新进度条
inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};
