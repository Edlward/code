#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;

int main(int argc, char **argv)
{
    vector<int> array;
    int itmp;

    cout << "enter:";
    while(cin >> itmp)
    {
        array.push_back(itmp);
        cout << itmp << endl;
        cout << "enter an integer:" ;
    }

    auto result = find(array.begin(), array.end(), 1);
    cout << endl << "result: " << *result;

    auto num = count(array.begin(), array.end(), 1);
    cout << endl << "num 1: " << num;

    auto sum = accumulate(array.begin(), array.end(), 0);
    cout << endl << "sum: " << sum;

    // what is this
    cout << endl << typeid(result).name();
    cout << endl << typeid(array.begin()).name();

    cout << endl;

}