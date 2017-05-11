#include <iostream>

using namespace std;

// 声明一些可以在编译时期获得的值

// A constexpr specifier used in a function or static member variable
//  (since C++17) declaration implies inline.

int main(int argc, char **argv)
{
    // A constexpr specifier used in an object declaration implies const
    constexpr int a = 1;
    cout << a << endl;
    
    // a = 2;   //error ，const变量不能被赋值
    cout << a << endl;
}