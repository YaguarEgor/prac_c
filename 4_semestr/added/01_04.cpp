#include <iostream>

class A {
    static int x;
public:
    static int f() {
        return ++x;
    }
};

int A::x = 1;

int main() {
    A a;
    A::f();
    A b;
    std::cout << A::f() << a.f() << b.f() << std::endl;
    return 0;
}