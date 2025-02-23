#include <iostream>
#include <cstring>

using namespace std;

class Book {
    // char* name;
    // char* author;
    string name;
    string author;
    int published_at;
public:
    Book() {
        this->name = "None";
    };

    // Book(const char* name, const char* author, int published_at) {
    //     this -> name = new char[strlen(name)+1];
    //     strcpy(this->name, name);
    //     this -> author = new char[strlen(author)+1];
    //     strcpy(this->author, author);
    //     this -> published_at = published_at;
    // };

    Book(const string name, const string author, int published_at) {
        this -> name = name;
        this -> author = author;
        this -> published_at = published_at;
    };

    string get_name() const{
        return this -> name;       
    }
    string get_author() const {
        return this -> author;
    }
    int get_published_at() const {
        return this -> published_at;
    }
};

class HomeLibrary {
    Book* books;
    int size_of_library;
    int len_of_mas;

public:
    HomeLibrary(){
        this->books = new Book[1];
        this->len_of_mas = 1;
        this->size_of_library = 0;
    };

    void add_book(string name, string author, int published_at = 2025) {
        if (size_of_library == len_of_mas) {
            Book* new_books = new Book[len_of_mas*2];
            memcpy(new_books, this->books, sizeof(this->books)); 
            delete this->books;
            this->books = new_books;
            len_of_mas *= 2;
        }
        books[size_of_library++] = Book(name, author, published_at);
    }

    Book* get_book_by_author(string value) {
        Book* cur_books = new Book[this->size_of_library];
        int cur_number = 0;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_author() == value) {
                cur_books[i] = this->books[i];
            }
        }
        return cur_books;
    }

    Book* get_book_by_name(char* value) {
        Book* cur_books = new Book[this->size_of_library];
        int cur_number = 0;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_author() == value) {
                cur_books[i] = this->books[i];
            }
        }
        return cur_books;
    }

    Book* get_book_by_published_at(int value) {
        Book* cur_books = new Book[this->size_of_library];
        int cur_number = 0;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_published_at() == value) {
                cur_books[i] = this->books[i];
            }
        }
        return cur_books;
    }

    void get_commands() {
        cout << "Добро пожаловать в Домашнюю библиотеку! Сейчас она у вас пустая,"
        " но Вы можете это исправить. Итак, давайте перейдем к командам, чтобы менять библиотеку:" << endl << " "
        "1) Добавить книгу. Вас попросят ввести название и автора книги, год публикации необязателен (по умолчанию - 2025)" << endl << " "
        "2) Поиск книги по автору. Введите автора и получите все его книги в Вашей библиотеке." << endl << " "
        "3) Поиск книги по названию. Ввведите название и получите всн книги с таким названием." << endl << " "
        "4) Поиск книги по дате публикации. Введите дату и получите все книги этого года выпуска в Вашей библиотеке." << endl << endl << ""
        "Чтобы ввести одну их этих команд, напишите номер команды (без закрывающей скобки) и следуйте дальнейшим подсказкам" << endl;
    }
};

int main() {
    int num_cmd;
    string name, author;
    int date;
    HomeLibrary library;
    library.get_commands();
    while (1) {
        cout << "Введите номер команды: ";
        cin >> num_cmd;
        switch (num_cmd) {
            case 1:
                cout << "Как называется новая книга? ";
                getline(cin, name);
                cout << "Как зовут ее автора? ";
                getline(cin, author);
                cout << "Год публикации? ";
                cin >> date;
                library.add_book(name, author, date);
        }
        
    }
    return 0;
}