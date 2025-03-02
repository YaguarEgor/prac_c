#include <iostream>

using namespace std;

//a

void f(double a, double b = 0);

void f(int a, string b);

int main() {
    f (1); 
    f ('+', '+'); 
    f (2.3); 
    f (3, "str"); 
    return 0;
}

//в

void h(int a, int b);

void h(const char* a, const char* b);

int main() {
    h (0, 1); 
    h (1, 0); 
    h (0, "m"); 
    h ("n", 0); 
    return 0;
}