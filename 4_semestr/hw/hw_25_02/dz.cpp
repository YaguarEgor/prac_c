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
    Book() {};

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
    int size_of_library;
    Book* books;
    
public:
    HomeLibrary(): size_of_library(0), books(nullptr) {};

    void add_book(string name, string author, int published_at) {
        Book* new_books = new Book[size_of_library+1];
        for (int i = 0; i < size_of_library; i++) {
            new_books[i] = books[i];
        }
        new_books[size_of_library++] = Book(name, author, published_at);
        delete[] books;
        books = new_books;
    }

    void get_book_by_author(string value) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        cout << "Все книги, автора которых зовут " << value << ":" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_author() == value) {
                cout << "Книга '" << this->books[i].get_name() << "', год выхода: " << this->books[i].get_published_at() << endl;
            }
        }
    }

    void get_book_by_name(string value) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        cout << "Все книги, которые назваются " << value << ":" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_author() == value) {
                cout << "Книга автора " << this->books[i].get_author() << ", год выхода: " << this->books[i].get_published_at() << endl;
            }
        }
    }

    void get_book_by_published_at(int value) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        cout << "Все книги, которые были выпущены в " << value << " году:" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_published_at() == value) {
                cout << "Книга '" << this->books[i].get_name() << "', автор: " << this->books[i].get_author() << endl;
            }
        }
    }

    void get_all_books() {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        for (int i = 0; i < this->size_of_library; ++i) {
            cout << "Книга №" << i+1 << ":" << endl << "  Название: " << this->books[i].get_name() << endl << "  " 
            "Автор: " << this->books[i].get_author() << endl << "  Год выпуска: " << this->books[i].get_published_at() << endl;
        }
    }

    void get_commands() {
        cout << "Добро пожаловать в Домашнюю библиотеку! Сейчас она у вас пустая,"
        " но Вы можете это исправить. Итак, давайте перейдем к командам, чтобы менять библиотеку:" << endl << " "
        "0) Вывести все книги в Вашей библиотеке" << endl << " "
        "1) Добавить книгу. Вас попросят ввести название и автора книги, год публикации необязателен (по умолчанию - 2025)" << endl << " "
        "2) Поиск книги по автору. Введите автора и получите все его книги в Вашей библиотеке" << endl << " "
        "3) Поиск книги по названию. Ввведите название и получите всн книги с таким названием" << endl << " "
        "4) Поиск книги по дате публикации. Введите дату и получите все книги этого года выпуска в Вашей библиотеке" << endl << ""
        "5) Выйти из библиотеки. Завершит взаимодействие с Домашней библиотекой" << endl << endl << ""
        "Чтобы ввести одну их этих команд, напишите номер команды (без закрывающей скобки) и следуйте дальнейшим подсказкам" << endl;
    }

    ~HomeLibrary() {
        delete[] this->books;
    }
};

int main() {
    int num_cmd;
    string title, author;
    int date;
    HomeLibrary library;
    library.get_commands();
    while (1) {
        cout << "Введите номер команды: ";
        cin >> num_cmd;
        switch (num_cmd) {
            case 0:
                library.get_all_books();
                break;
            case 1:
                cout << "Как называется новая книга? ";
                cin.ignore();
                getline(cin, title);
                cout << "Как зовут ее автора? ";
                getline(cin, author);
                cout << "Год публикации? ";
                cin >> date;
                if (date > 2025) {
                    date = 2025;
                }
                library.add_book(title, author, date);
                break;
            case 2:
                cout << "Введите имя автора, книги которого будем искать: ";
                cin.ignore();
                getline(cin, author);
                library.get_book_by_author(author);
                break;
            case 3:
                cout << "Введите название книги, которую будем искать: ";
                cin.ignore();
                getline(cin, title);
                library.get_book_by_name(title);
                break;
            case 4:
                cout << "Введите год публикации книги, по которому будем искать: ";
                cin >> date;
                library.get_book_by_published_at(date);
                break;
            case 5:
                return 0;
            default:
                cout << "Такой команды нет" << endl;
                break;
        }
        
    }
}
