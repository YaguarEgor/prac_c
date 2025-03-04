#include <iostream>
#include <cstring>
#include <variant>

using namespace std;

class Book {
    string name;
    string author;
    int published_at;
public:
    Book() {};

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

    variant<string, int> get_field(int field) {
        switch (field) {
            case 1:
                return this->name;
                break;
            case 2:
                return this->author;
                break;
            case 3:
                return this->published_at;
                break;
        }
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
        int num = 0;
        cout << "Все книги, автора которых зовут " << value << ":" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_author() == value) {
                num++;
                cout << "Книга '" << this->books[i].get_name() << "', год выхода: " << this->books[i].get_published_at() << endl;
            }
        }
        if (num == 0) {
            cout << "Таких книг пока нет" << endl;
        }
    }

    void get_book_by_name(string value) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        int num = 0;
        cout << "Все книги, которые называются " << value << ":" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_name() == value) {
                num ++;
                cout << "Книга автора " << this->books[i].get_author() << ", год выхода: " << this->books[i].get_published_at() << endl;
            }
        }
        if (num == 0) {
            cout << "Таких книг пока нет" << endl;
        }
    }

    void get_book_by_published_at(int value) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        int num = 0;
        cout << "Все книги, которые были выпущены в " << value << " году:" << endl;
        for (int i = 0; i < this->size_of_library; i++) {
            if (this->books[i].get_published_at() == value) {
                num ++;
                cout << "Книга '" << this->books[i].get_name() << "', автор: " << this->books[i].get_author() << endl;
            }
        }
        if (num == 0) {
            cout << "Таких книг пока нет" << endl;
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
        "3) Поиск книги по названию. Введите название и получите всн книги с таким названием" << endl << " "
        "4) Поиск книги по дате публикации. Введите дату и получите все книги этого года выпуска в Вашей библиотеке" << endl << " "
        "5) Удаление книги по названию. Введите название книги, чтобы удалить ее из Вашей библиотеки" << endl << " "
        "6) Отсортировать книги по полю. Нужно будет выбрать поле для сортировки и порядок" << endl << " "
        "7) Выйти из библиотеки. Завершит взаимодействие с Домашней библиотекой" << endl << endl << ""
        "Чтобы ввести одну их этих команд, напишите номер команды (без закрывающей скобки) и следуйте дальнейшим подсказкам" << endl;
    }

    void delete_book(string title) {
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        int num_for_delete = 0;
        for (int i = 0; i < this->size_of_library; ++i) {
            if (this->books[i].get_name() == title) {
                ++num_for_delete;
            }
        }
        if (num_for_delete == 0) {
            cout << "В Вашей библиотеке нет книг с таким названием" << endl;
            return;
        }
        Book* new_books = new Book[this->size_of_library-num_for_delete];
        int new_ind = 0;
        for (int i = 0; i < this->size_of_library; ++i) {
            if (this->books[i].get_name() != title) {
                new_books[new_ind++] = this->books[i];
            }
        }
        this->size_of_library-=num_for_delete;
        delete[] books;
        books = new_books; 
    }

    void sort_library(int field) {
        if (field > 3 || field < 1) {
            cout << "Выберите один из трех вариантов" << endl;
        }
        if (this->size_of_library == 0) {
            cout << "В Вашей библиотеке пока нет книг" << endl;
            return;
        }
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
              if (this->books[j].get_field(field) > this->books[j + 1].get_field(field)) {
                Book b = this->books[j]; 
                this->books[j] = this->books[j + 1];
                this->books[j + 1] = b; 
              }
            }
        }
        get_all_books();
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
                cout << "Введите название книги, которую будем удалять: ";
                cin.ignore();
                getline(cin, title);
                library.delete_book(title);
                break;
            case 6:
                break;
            case 7:
                return 0;
            default:
                cout << "Такой команды нет" << endl;
                break;
        }
        
    }
}
