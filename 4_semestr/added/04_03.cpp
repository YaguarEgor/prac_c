#include <iostream>

using namespace std;

class Single {
    Single() {};
    Single& operator= (const Single&) = delete;

public:
    static Single& getObj() {
        static Single obj;
        return obj;
    }

    void print_something() {
        static int count = 0;
        count++;
        cout << count << endl;
    }

};

int main() {
    Single& obj = Single::getObj();
    obj.print_something();
    Single& obj2 = Single::getObj();
    obj2.print_something();
    return 0;
}