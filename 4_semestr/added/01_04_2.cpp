#include <iostream>

using namespace std;

struct A {
    operator int() {return 1;}
};

void f(int a, double b) {
    cout << 1;
}

void f(double a, char b) {
    cout << 2;
}

int main() {
    A a;
    f(1, a);
}