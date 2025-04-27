#include <iostream>

using namespace std;

int f(int a = 0) { 
    cout << "1";
    return a; 
}  

int f(double a) { 
    cout << "2";
    return a; 
} 

int main() {  
    short int s;  
    int i;  
    bool b;  
    enum e {A, B, C};  
    float f1 = 1.0f; 
    f();     //1 на 1 шаге
    f(s);    //1 на 2.2 шаге 
    f(f1);   //2 на 2.2 шаге 
    f(b);    //1 на 2.2 шаге 
    f(A);    //1 на 2.2 шаге 
}