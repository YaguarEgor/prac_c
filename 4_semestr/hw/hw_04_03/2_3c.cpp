#include <iostream>

using namespace std;

int f(int a = 1) {
    return a;
}  

int f(long double a = 5.0) {
    return a;
} 

int main () { 
    short int s;  
    int i;  
    bool b;  
    float f1 = 1.0f;  
    double d = 2.0; 
    f(s);   //1 на 2 шаге
    f(i);   //1 на 1 шаге
    f(b);   //1 на 2 или 3 шаге  
    f(f1);   //Err, так как обе функции подходят на 3 шаге
    f(d);   //Err, double -> long double не расширение, а обычное преобразование
}    
  