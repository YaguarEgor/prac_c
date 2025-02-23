#include <iostream>
using namespace std; //используем пространство имен

//ошибка заключалась в том, что поля изначально приватные

class Cl {
    public: //вот и исправление
        int x, y;
}; //точку с запятой забыли
   
int main () {
    Cl c;
    c.y = 10;
    c.x = c.y++;
    cout << c.x << c.y << endl;
    return 0;
}

//в результате будет выведено 1011