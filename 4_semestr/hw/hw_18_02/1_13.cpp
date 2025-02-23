#include <iostream>

struct mystr { 
    int a, b; 
    private:
        mystr(const mystr&);
}; 

int i = sizeof(mystr); 

int f(mystr s) { 
    return 0; 
} 

int main() {
    return 0;
}