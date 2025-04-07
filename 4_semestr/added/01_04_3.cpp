#include <iostream>

struct B {
    B() {std::cout << "Constr";}
    B(const B &) {
        std::cout << "Copy";
    }
    B(B && b) {
        std::cout << "Move constr";
    }
    ~B() {
        std::cout << "Destr";
    }
};

B f() {
    return B();
}

int main() {
    try {
        B b;
        //b = f();
        f();
    }
    catch (...) {
        return 0;
    }
    return 0;
}