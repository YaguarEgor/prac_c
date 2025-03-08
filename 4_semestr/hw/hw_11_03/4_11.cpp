struct A { 
    int i; 
    virtual void f() = 0; 
    virtual ~A() {} 
}; 
 
int g(A a) { 
    return a.i * 5; 
}

//передается объект абстрактного класса, что невозможно. Нужно передавать его по ссылке