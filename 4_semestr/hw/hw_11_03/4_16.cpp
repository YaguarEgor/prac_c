#include <iostream>

using namespace std;

struct S { 
    static double d; 
    virtual  S &  g () { 
        cout << "g ( ) from S" << endl; 
    } 
}; 

struct T: S { 
    virtual  T & g ( ) { 
        cout << "g ( ) from T" << endl; 
    } 
    
}; 

double S::d = 1.5; 
int main () {  
    T t;   
    S s, *ps = &t; 
    ps  -> g ();   //g() from T
    s.d = 5; 
    t.d = 7; 
    cout << s.d << ' ' << t.d << ' ' <<  S::d << endl;  //7 7 7
    return 0; 
} 
   