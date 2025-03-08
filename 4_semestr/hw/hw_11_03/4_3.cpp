#include <iostream>

using namespace std;

 
class T { 
public:  
    virtual  int f (int x) { 
        cout  << "T::f" << endl;    
        return 0; 
    } 
    void g () { 
        f (1);  
        cout  << "T::g" << endl;  
    } 
    virtual void h () {  
        g (); 
        cout  << "T::h" << endl;  
    }  
}; 
    
    
class S: public T {  
public:  
    int f (double y){  
        cout  << "S::f" << endl;     
        return 2;  
    } 
    virtual void g () { 
        f (1);     
        cout  << "S::g" << endl; 
    }  
    virtual void h () { 
        g();   
        cout  << "S::h" << endl;   
    } 
}; 
int main(){ 
    T t;  S s;  T *p = &s;   
    p -> f (1.5);   //T::f
    p -> g ();      //T::f \n T::g
    p -> h ();      //S::f \n S::g \n S::h
}