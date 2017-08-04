#include <algorithm>
#include <iostream>

using namespace std;

auto lamb1 = [](const int v1, const int v2)->int 
{
    return v1+v2;
};

int main(int argc, char **argv)
{
    int a = 0; 
    a = lamb1(1,2);
    cout << a << endl;

    auto lamb2 = [a](const int v1)->int
    {
        return a+v1;
    };

    int b = lamb2(1);
    cout << b << endl;
}