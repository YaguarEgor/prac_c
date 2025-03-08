#include <iostream>

using namespace std;

class Transport {

protected:
    string* passengers;
    int number_of_passengers;
    string brand;

public:
    Transport(): passengers(nullptr), number_of_passengers(0), brand("None") {};
    Transport(string brand): passengers(nullptr), number_of_passengers(0), brand(brand) {};

    Transport(const Transport& other) {
        this->number_of_passengers = other.number_of_passengers;
        if (this->number_of_passengers == 0) {
            this->passengers = nullptr;
            return;
        }
        this->passengers = new string[this->number_of_passengers];
        for (int i = 0; i < this->number_of_passengers; ++i) {
            this->passengers[i] = other.passengers[i];
        }
        this->brand = other.brand;
    }

    int get_num_of_pas() const {
        return number_of_passengers;
    }

    void PrintPassengers() const {
        if (number_of_passengers == 0) {
            cout << "The transport is empty" << endl;
            return;
        }
        cout << "My transport has brand " << brand << endl;
        for (int i = 0; i < number_of_passengers; ++i) {
            cout << "Passenger number " << i+1 << ": " << passengers[i] << endl;
        }
    };

    Transport& operator=(const Transport& other) {
        this->brand = other.brand;
        if (this->passengers) {
            delete[] this->passengers;
        }
        this->number_of_passengers = other.number_of_passengers;
        if (this->number_of_passengers == 0) {
            this->passengers = nullptr;
            return *this;
        }
        delete[] this->passengers;
        this->passengers = new string[this->number_of_passengers];
        for (int i = 0; i < this->number_of_passengers; ++i) {
            this->passengers[i] = other.passengers[i];
        }
        return *this;
    }

    string& operator[](int index) const{
        if (index >= this->number_of_passengers) {
            throw out_of_range("There isn't a passenger with this index");
        }
        return this->passengers[index];
    };

    friend ostream& operator<<(ostream& os, const Transport& t) {
        t.print(os);
        return os;
    }

    virtual string GetBrand() const = 0;
    virtual void operator()(string name) = 0;
    virtual void print(ostream& out) const = 0;
    virtual ~Transport() {
        delete[] passengers;
    }; 
};

class Car: public Transport {
public:
    static const int max_num_of_pas = 4;
    Car() {};
    Car(string brand): Transport(brand) {};

    Car(const Car& other): Transport(other) {};

    virtual void print(ostream& out) const {
        out << "Transport: Car, brand: " << this->brand << ", number of passengers: " << this->number_of_passengers << endl;
    };

    virtual string GetBrand() const{
        return "Brand of this Car is " + brand + "\n";
    };

    virtual void operator()(string name) {
        if (this->number_of_passengers == max_num_of_pas) {
            cout << "The Car is full" << endl;
            return;
        }
        this->number_of_passengers++;
        string* new_mas = new string[this->number_of_passengers];
        for (int i = 0; i < this->number_of_passengers-1; i++) {
            new_mas[i] = this->passengers[i];
        }
        if (this->passengers) {
            delete[] this->passengers;
        }
        this->passengers = new_mas;
        this->passengers[this->number_of_passengers-1] = name;
    };

    virtual ~Car() {
        cout << "I am destroying car" << endl;
    }; 
};

class Bus: public Transport {
public:
    static const int max_num_of_pas = 10;
    Bus() {};
    Bus(string brand): Transport(brand) {};

    virtual void print(ostream& out) const {
        out << "Transport: Bus, brand: " << this->brand << ", number of passengers: " << this->number_of_passengers << endl;
    };

    virtual string GetBrand() const{
        return "Brand of this Bus is " + brand + "\n";
    };

    virtual void operator()(string name) {
        if (this->number_of_passengers == max_num_of_pas) {
            cout << "The Bus is full" << endl;
            return;
        }
        this->number_of_passengers++;
        string* new_mas = new string[this->number_of_passengers];
        for (int i = 0; i < this->number_of_passengers-1; i++) {
            new_mas[i] = this->passengers[i];
        }
        if (this->passengers) {
            delete[] this->passengers;
        }
        this->passengers = new_mas;
        this->passengers[this->number_of_passengers-1] = name;
    };

    virtual ~Bus() {
        cout << "I am destroying bus" << endl;
    }; 
};

int main() {
    Transport* transports[5];
    
    transports[0] = new Car();
    transports[1] = new Car("Nissan");
    transports[2] = new Bus();
    transports[3] = new Bus("Gazel");
    transports[4] = new Car();
    *transports[4] = *transports[1];

    for (int i = 0; i < 5; i++) {
        (*transports[i])("Passenger 1");
    }
    for (int i = 0; i < 4; i++) {
        (*transports[i])("Passenger 2");
    }

    for (int i = 0; i < 4; i++) {
        (*transports[0])("New Passenger");
    }

    cout << "First: " << endl;
    transports[1]->PrintPassengers();
    cout << "Second: " << endl;
    transports[4]->PrintPassengers();
    
    cout << endl;

    for (int i = 1; i < 4; i++) {
        cout << transports[i]->GetBrand();
    }

    for (int i = 0; i < 5; i++) {
        cout << *transports[i];
    }

    for (int i = 0; i < 5; ++i) {
        delete transports[i];
    }


    return 0;
}