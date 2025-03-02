#include <iostream>

using namespace std;

class Cls { 
    int i; 
public: 
    //такое добавление надо сделать:
    //Cls& operator= (const Cls&) = delete; 
    Cls() { i = 1; } 
}; 

void f(Cls * p, Cls * q) { 
    *p = *q; 
} 