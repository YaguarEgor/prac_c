#include <iostream>

int x = 0; 

void f(int a, int b) { x = a+b; } 

class A { 
    int x; 
public: 
    void f() { x = 2; } 
    void f(char a1, char b1) { x = a1-b1; } 
}; 
 
class B: public A { 
public: 
    void f(int a) { ::x = a; } 
    void g () { 
        //f();  //B::f, too few arguments
        f(0);  //B::f
        //f(5.3 , 1); //B::f, too few arguments
        //x = 1;  //A::x, no access
    } 
}; 
 
int main () { 
    B b;  //A(), B()
    //f(2); //::f, too few arguments
    f(3, 'a'); //::f 
    return 0; 
} 
 