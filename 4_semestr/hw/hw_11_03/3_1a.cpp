#include <iostream>

int x = 0; 
void f (int a, int b) {
    x = a+b;
} 
 
class A { 
    int x; 
public: 
    void f (){x = 2;} 
}; 
 
class B: public A { 
public: 
    void f (int a){::x = a;} 
    void g (); 
}; 
 
void B::g() { 
    //f();  //вызывается функция B::f, поэтому ошибка
    f(1);  //B::f
    //f(5 , 1); //вызывается функция B::f, поэтому ошибка
    //x = 2;  //нет доступа к private члену родительского класса
} 
 
B ret(B & x, B & y) {  
    return x;  
} 
 
int main () { 
    B b;  //A(), then B()
    //f(5);   //::f, но для нее слишком мало аргументов
    f('+', 6);   //::f
    b = ret(b,  b);  //::ret, copy_const, operator=, ~B(), ~A()
    return 0;  //~B(), ~A()
}