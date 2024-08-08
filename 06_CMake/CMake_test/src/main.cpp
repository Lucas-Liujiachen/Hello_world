#include <iostream>
#include "math_functions.h"

using namespace std;

int main(int, char **)
{
    int a = 4, b = 10;
    auto result1 = add(a, b);
    auto result2 = sub(a, b);
    auto result3 = multi(a, b);
    auto result4 = divv(a, b);
    cout << "result1 is " << result1 << endl;
    cout << "result2 is " << result2 << endl;
    cout << "result3 is " << result3 << endl;
    cout << "result4 is " << result4 << endl;
    return 0;
}
