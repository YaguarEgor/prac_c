#include <iostream>

using namespace std;

class A {
    int a;
public:
    A(int a): a(a) {};
    A& operator *= (const A& other) {
        this -> a *= other.a;
        return *this;
    }
    string get() {
        return to_string(a) + " ";
    }
};

int main () { 
    A a1 (5),  a2 = 3; 
    a1  *=  10; 
    a2  *=   a1  *=  2; 
    cout << a1.get() << a2.get() << endl; 
    return 0; 
   } 