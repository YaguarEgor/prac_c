#include <iostream>

//ошибка в использовании explicit и отсутствии конструктора по умолчанию

struct A {
    int n;
    A (double d) { n = d;}
    A() {};
};

int main () {
    A a, b (1), c = 1.5;
    std::cout << b.n << c.n << std::endl;
    return 0;
}

//будет выведено 11