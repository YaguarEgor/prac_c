#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cctype>
#include <utility>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <ostream>
#include <stack>


using namespace std;

enum type_of_lex {
    LEX_NULL,      

    LEX_IF, LEX_ELSE, LEX_AND, LEX_REAL, LEX_STRING, LEX_DO, LEX_GOTO, LEX_FALSE, LEX_INT, LEX_BOOL,                         
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_BREAK, LEX_WRITE, LEX_WHILE,                    

    LEX_FIN,                                                                                    

    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, 
    LEX_MOD,    
    LEX_LEQ, LEX_NEQ, LEX_GEQ,    
    LEX_LFIG, LEX_RFIG,   
    
    LEX_NUM,                                                                                    
    LEX_STR,                                                                                    
    LEX_ID,        
    LEX_REAL_CONST,                                                                             

    POLIZ_LABEL,                                                                                
    POLIZ_ADDRESS,                                                                              
    POLIZ_GO,                                                                                   
    POLIZ_FGO                                                                                  
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    double d_val;
    int line_lex;
    string s_lex;
public:
    explicit Lex ( type_of_lex t = LEX_NULL, int v = 0, int line = 0, string str = ""): 
                                t_lex (t), v_lex (v), d_val(0.0), line_lex(line), s_lex(move(str)) { }
    
    Lex(type_of_lex t, double d, int ln): t_lex(t), v_lex(0), d_val(d), line_lex(ln) {}
    
    type_of_lex get_type () const { 
        return t_lex; 
    }

    int get_value () const { 
        return v_lex; 
    }

    double get_dval() const {
        return d_val;
    }

    friend ostream &operator<<(ostream &s, Lex l );
};

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
    bool is_label;
    
public:
    Ident(): declare(false), assign(false) {} 

    bool operator==(const string &s) const {
        return name == s; 
    }

    explicit Ident(const string n): declare(false), assign(false) {
        name = move(n);
    }

    string get_name() const { 
        return name; 
    }

    bool get_declare() const { 
        return declare; 
    }

    void put_declare() { 
        declare = true; 
    }

    type_of_lex get_type() const { 
        return type;
    }

    void put_type(type_of_lex t) { 
        type = t; 
    }

    bool get_assign() const { 
        return assign; 
    }

    void put_assign() { 
        assign = true; 
    }

    int get_value() const { 
        return value; 
    }

    void put_value(int v) { 
        value = v; 
    }

    bool get_is_label() const { 
        return is_label; 
    }

    void put_is_label() { 
        is_label = true; 
    }
};

vector<Ident> TID;
 
int put (const string &buf) {
    vector<Ident>::iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

class Scanner {
    FILE * fp;
    int   c;

    static int look (const string& buf, const char **list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }

    void gc () {
        c = fgetc(fp);
    }

public:
    static const char *TW[], *TD[];
    static int line_counter;
    explicit Scanner(const char *program) {
        fp = fopen(program, "r");
        if (!fp) {
            throw "can`t open file";
        }
    }
    Lex get_lex();

    ~Scanner() { if (fp) fclose(fp); } 
};

const char *Scanner::TW[] = {
    "", "if", "else", "and", "real", "string", "do",
    "goto","false","int","bool","not","or","program","read",
    "true", "break", "write", "while", NULL
};

const char *Scanner::TD[] = {
    "", ";", ",", ":", "=", "(", ")", "==",
    "<", ">", "+", "-", "*", "/", "%", "<=", "!=", ">=", 
    "{", "}", NULL
};

int count = 0;
static int str_counter = 0;

Lex Scanner::get_lex() {
    enum state { H, IDENT, NUMB, REAL, COM, ALE, STR };
    string buf;
    state  CS = H;
    bool dot_seen = false, exp_seen = false;

    while (true) {
        gc();
        switch (CS) {
            case H:
                if (c == ' ' || c == '\t' || c == '\r') {
                    ;
                } else if (c == '\n') {
                    ++line_counter;
                } else if (isalpha(c)) {
                    buf.push_back(static_cast<char>(c)); CS = IDENT;
                } else if (isdigit(c)) {
                    buf.push_back(static_cast<char>(c)); CS = NUMB;
                } else if (c == EOF) {
                    return Lex(LEX_FIN, 0, line_counter);
                } else if (c == '=' || c == '<' || c == '>' || c == '!') {
                    buf.push_back(static_cast<char>(c)); CS = ALE;
                } else if (c == '"') {
                    CS = STR;
                } else if (c == '/') {
                    gc();
                    if (c == '*') CS = COM; else {
                        ungetc(c, fp); buf.push_back('/');
                        int j = look(buf, TD);
                        if (j) return Lex(static_cast<type_of_lex>(j + LEX_FIN), j, line_counter);
                        throw static_cast<char>(buf[0]);
                    }
                } else {
                    buf.push_back(static_cast<char>(c));
                    int j = look(buf, TD);
                    if (j) return Lex(static_cast<type_of_lex>(j + LEX_FIN), j, line_counter);
                    throw static_cast<char>(c);
                }
                break;

            case IDENT:
                if (isalnum(c)) buf.push_back(static_cast<char>(c));
                else {
                    ungetc(c, fp);
                    int j = look(buf, TW);
                    if (j) return Lex(static_cast<type_of_lex>(j), j, line_counter);
                    return Lex(LEX_ID, put(buf), line_counter);
                }
                break;

            case NUMB:
                if (isdigit(c)) buf.push_back(static_cast<char>(c));
                else if (c == '.' || c == 'e' || c == 'E') {
                    buf.push_back(static_cast<char>(c));
                    dot_seen = (c == '.'); exp_seen = (c == 'e' || c == 'E');
                    CS = REAL;
                } else if (isalpha(c)) throw "wrong identifier";
                else {
                    ungetc(c, fp);
                    return Lex(LEX_NUM, stoi(buf), line_counter);
                }
                break;

            case REAL:
                if (isdigit(c)) buf.push_back(static_cast<char>(c));
                else if (c == '.' && !dot_seen) { buf.push_back('.'); dot_seen = true; }
                else if ((c == 'e' || c == 'E') && !exp_seen) {
                    buf.push_back(static_cast<char>(c)); exp_seen = true; gc();
                    if (c == '+' || c == '-') buf.push_back(static_cast<char>(c)); else ungetc(c, fp);
                } else {
                    ungetc(c, fp);
                    return Lex(LEX_REAL_CONST, stod(buf), line_counter);
                }
                break;

            case COM:
                if (c == EOF) throw "unclosed comment";
                else if (c == '\n') ++line_counter;
                else if (c == '*') { gc(); if (c == '/') CS = H; else ungetc(c, fp); }
                break;

            case ALE:
                if (c == '=') buf.push_back('='); else ungetc(c, fp);
                return Lex(static_cast<type_of_lex>(look(buf, TD) + LEX_FIN), look(buf, TD), line_counter);

            case STR:
                if (c != '"') { if (c == '\n') ++line_counter; buf.push_back(static_cast<char>(c)); }
                else { ++str_counter; return Lex(LEX_STR, str_counter, line_counter, buf); }
                break;
        }
    }
}

ostream &operator<<(ostream &s, Lex lex_to_print) {

    string t;
    if (lex_to_print.t_lex < LEX_FIN)
        t = Scanner::TW[lex_to_print.t_lex];
    else if (lex_to_print.t_lex <= LEX_RFIG)
        t = Scanner::TD[lex_to_print.t_lex - LEX_FIN];
    else if (lex_to_print.t_lex == LEX_NUM)
        t = "NUMB";
    else if (lex_to_print.t_lex == LEX_STR)
        t = "STR";
    else if (lex_to_print.t_lex == LEX_REAL_CONST)
        t = "REAL_CONST";
    else if (lex_to_print.t_lex == LEX_ID)
        t = TID[lex_to_print.v_lex].get_name();
    else
        throw lex_to_print;
    s << "In line " << lex_to_print.line_lex 
      << " ( type " << t 
      << " , index " << lex_to_print.t_lex 
      << " , counter " << lex_to_print.v_lex << " );" << endl;
    cout << endl;
    return s;
}

int Scanner::line_counter = 1;

template<class Stack, class T>
void from_st(Stack &st, T &i) {
    i = st.top();
    st.pop();
}

class Parser {
    private:
        Scanner            scan;               // лексический анализатор
        Lex                curr_lex;           // текущая лексема
        type_of_lex        c_type;             // тип текущей лексемы
        int                c_val;              // значение/индекс текущей лексемы
        stack<int>         st_int;             // стек целочисленных значений (не используется здесь)
        stack<type_of_lex> st_lex;             // стек типов при проверке выражений
    
        void gl();                              // считать следующую лексему
        void P();                               // запуск анализа программы
        void D(type_of_lex t);                  // обработка раздела объявлений
        void S();                               // обработка оператора
        void E();                               // разбор выражения E
        void E1();                              // разбор E1
        void T();                               // разбор T
        void F();                               // разбор F
    
        void eq_type();                         // проверка корректности присваивания
        void check_op();                        // проверка бинарных операций
        void check_not();                       // проверка оператора not
        void eq_bool();                         // проверка булевого выражения в условии
        void check_id();                        // проверка объявления идентификатора
        void check_id_in_read();                // проверка идентификатора для read
        void check_label();
    
    public:
        explicit Parser(const char *program);   // конструктор, принимает имя файла
        void analyze();                         // точка входа синтаксического анализа
    };
    
    
    
    // Конструктор: инициализация Scanner и чтение первой лексемы
    Parser::Parser(const char *program)
      : scan(program) {
        gl();
    }
    
    // gl(): загрузить следующую лексему из Scanner
    void Parser::gl() {
        curr_lex = scan.get_lex();
        c_type   = curr_lex.get_type();
        c_val    = curr_lex.get_value();
    }
    
    // analyze(): общий поток синтаксического анализа
    void Parser::analyze() {
        // Ожидаем ключевое слово program
        if (c_type != LEX_PROGRAM) throw curr_lex;
        gl(); // съели program
    
        // Ожидаем '{'
        if (c_type != LEX_LFIG) throw curr_lex;
        gl(); // съели '{'
    
        // Объявления: пока встречается int, bool, real, string
        while (c_type == LEX_INT || c_type == LEX_BOOL ||
               c_type == LEX_REAL || c_type == LEX_STRING) {
            D(c_type);                          // съесть тип и идентификаторы
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            gl();                                // съесть ';'
        }
    
        while (c_type != LEX_RFIG) {
            S();
            if (c_type == LEX_SEMICOLON) {
                gl(); // съесть ';'
            }
        }
        gl();
    
        // Конец файла
        if (c_type != LEX_FIN) throw curr_lex;
        cout << "Yes!!!" << endl;
    }
    
    // D(type): раздел объявлений переменных
    void Parser::D(type_of_lex t) {
        gl();                                  // съесть токен типа
        if (c_type != LEX_ID) throw curr_lex;
        do {
            int id = c_val;
            TID[id].put_declare();             // пометить как объявленное
            TID[id].put_type(t);               // сохранить тип переменной
    
            gl(); // съесть идентификатор
            if (c_type == LEX_ASSIGN) {
                gl(); // съесть '='
                E();  // разобрать выражение-инициализатор
    
                // проверить, что тип инициализатора совпадает с объявленным
                if (st_lex.top() != t) throw "initializer type mismatch";
                st_lex.pop();
            }
        } while (c_type == LEX_COMMA && (gl(), true));
    }
    
    // S(): разбор операторов
    void Parser::S() {
        if (c_type == LEX_IF) {
            gl(); E(); eq_bool();
            S();
            if (c_type == LEX_ELSE) { gl(); S(); }
        }
        else if (c_type == LEX_WHILE) {
            gl();             // съели 'while'
            E(); eq_bool();   // условие
            // если есть 'do', то съём его (необязательно в примере)
            if (c_type == LEX_DO) gl();
            // дальше S() разберёт либо одиночный stmt, либо блок {...}
            S();
        }
        else if (c_type == LEX_READ) {
            gl();              // 'read'
            if (c_type != LEX_LPAREN) throw curr_lex;
            gl();              // '('
            if (c_type != LEX_ID) throw curr_lex;
            check_id_in_read();
            gl();              // ID
            if (c_type != LEX_RPAREN) throw curr_lex;
            gl();              // ')'
        }
        else if (c_type == LEX_WRITE) {
            gl();              // 'write'
            if (c_type != LEX_LPAREN) throw curr_lex;
            gl();              // '('
            do {
                E();
                st_lex.pop();  // убираем тип аргумента
            } while (c_type == LEX_COMMA && (gl(), true));
            if (c_type != LEX_RPAREN) throw curr_lex;
            gl();              // ')'
        }
        else if (c_type == LEX_LFIG) {
            // блок { S; S; … }
            gl();                       // съели '{'
            while (c_type != LEX_RFIG) {
                S();
                if (c_type == LEX_SEMICOLON)
                    gl();               // съели ';'
            }
            gl();     // съели '}'
        }
        else if (c_type == LEX_ID) {
            // Проверяем, является ли это меткой (ID : )
            Lex saved_lex = curr_lex; // Сохраняем текущую лексему
            int id = c_val;
            gl(); // Съели ID
            if (c_type == LEX_COLON) {
                // Это метка
                TID[id].put_is_label(); // Отмечаем как метку
                gl(); // Съели ':'
                // После метки может следовать оператор
                if (c_type != LEX_RFIG && c_type != LEX_SEMICOLON) {
                    S(); // Разбираем следующий оператор
                }
            } else {
                // Это не метка, а присваивание
                int id2 = c_val;
                c_val = id;
                check_id(); // Проверяем, что ID объявлен
                c_val = id2;
                if (c_type != LEX_ASSIGN) throw curr_lex;
                do {
                    gl();  // съели '='
                    E();   // RHS
                    eq_type();
                    
                } while (c_type == LEX_ASSIGN);
            }
        }
        else if (c_type == LEX_GOTO) {
            gl(); // Съели 'goto'
            if (c_type != LEX_ID) throw curr_lex;
            check_label(); // Проверяем, что это метка
            gl(); // Съели ID
            // После goto label ожидается ;
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            // Не съедаем ; здесь, так как это сделает вызывающий код
        }
        else {
            throw curr_lex;
        }
    }
    
    
    // P ::= E1 [ relop E1 ]
    void Parser::P() {
        E1();
        if (c_type == LEX_EQ  || c_type == LEX_NEQ ||
            c_type == LEX_LSS || c_type == LEX_GTR ||
            c_type == LEX_LEQ || c_type == LEX_GEQ) {
            st_lex.push(c_type);
            gl();       // съели relop
            E1();
            check_op(); // проверили пару E1 relop E1
        }
    }

    // E ::= P { or P }
    void Parser::E() {
        P();
        while (c_type == LEX_OR) {
            st_lex.push(c_type); 
            gl();       // съели 'or'
            P();
            check_op(); // проверили BOOL or BOOL
        }
    }

    // E1: сумма { + сумма | - сумма }
    void Parser::E1() {
        T();
        while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
            st_lex.push(c_type);
            gl();            // съели '+' или '-'
            T();
            check_op();      // проверили арифметику
        }
    }

    // T: терминал { * терминал | / терминал | and терминал }
    void Parser::T() {
        F();
        while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
            st_lex.push(c_type);
            gl();            // съели '*', '/' или 'and'
            F();
            check_op();      // проверили boolean and
        }
    }
    
    // F: атомарное выражение
    void Parser::F() {
        if (c_type == LEX_ID) {
            Lex saved_lex = curr_lex;
            int id = c_val;
            gl();
            if (c_type == LEX_ASSIGN) {
                c_val = id;
                // Присваивание как выражение, например, b=1
                check_id(); // Проверяем, что ID объявлен
                gl(); // Съели '='
                E(); // Разбираем правую часть
                eq_type(); // Проверяем типы, оставляем тип левой части на стеке
            } else {
                // Обычный идентификатор
                c_val = id;
                check_id(); // Проверяем, что ID объявлен
            }
        }
        else if (c_type == LEX_NUM) {
            st_lex.push(LEX_INT);
            gl();
        }
        else if (c_type == LEX_STR) {
            st_lex.push(LEX_STRING);
            gl();
        }
        else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
            st_lex.push(LEX_BOOL);
            gl();
        }
        else if (c_type == LEX_REAL_CONST) { 
            st_lex.push(LEX_REAL);  
            gl(); 
        }
        else if (c_type == LEX_NOT) {
            gl();
            F();
            check_not();
        }
        else if (c_type == LEX_LPAREN) {
            gl();
            E();
            if (c_type != LEX_RPAREN) throw curr_lex;
            gl();
        }
        else {
            throw curr_lex;
        }
    }
    
    // Проверка идентификатора при операции и read
    void Parser::check_id() {
        if (!TID[c_val].get_declare()) {
            cout << curr_lex << endl;
            throw "identifier not declared";
        } 
        st_lex.push(TID[c_val].get_type());
    }
    
    void Parser::check_id_in_read() {
        check_id();
    }
    
    // Проверка бинарных операций по типам
    void Parser::check_op() {
        type_of_lex t1,t2,op;
        from_st(st_lex,t2); 
        from_st(st_lex,op); 
        from_st(st_lex,t1);
        if ((op==LEX_PLUS||op==LEX_MINUS||op==LEX_TIMES||op==LEX_SLASH)
            &&t1==LEX_INT&&t2==LEX_INT) {
            st_lex.push(LEX_INT);
        }
        else if ((op==LEX_PLUS||op==LEX_MINUS||op==LEX_TIMES||op==LEX_SLASH)
            &&t1==LEX_REAL&&t2==LEX_REAL) {
            st_lex.push(LEX_REAL);
        }
        else if ((op==LEX_LSS||op==LEX_GTR||op==LEX_LEQ||op==LEX_GEQ)
                 &&t1==LEX_INT&&t2==LEX_INT) {
            st_lex.push(LEX_BOOL);
        }
        else if ((op==LEX_AND||op==LEX_OR)
                 &&t1==LEX_BOOL&&t2==LEX_BOOL) {
            st_lex.push(LEX_BOOL);
        }
        else if ((op==LEX_EQ||op==LEX_NEQ)&&t1==t2) st_lex.push(LEX_BOOL);
        else if (op==LEX_PLUS&&t1==LEX_STRING&&t2==LEX_STRING) st_lex.push(LEX_STRING);
        else throw "type mismatch in operation";
    }
    
    // not должно применяться к bool
    void Parser::check_not() {
        if (st_lex.top()!=LEX_BOOL) throw "not applied to non-bool";
        st_lex.pop(); st_lex.push(LEX_BOOL);
    }
    
    // проверка присваивания по типу
    void Parser::eq_type() {
        type_of_lex rhs;
        from_st(st_lex, rhs);               // rhs = тип правого
        type_of_lex lhs = st_lex.top();     // lhs = тип левого
        if (rhs != lhs) throw "assignment type mismatch";
        st_lex.pop();                       // убрали lhs
        st_lex.push(lhs);                   // вернули на стек тип выражения lhs=rhs
    }
    
    // проверка булевого условия
    void Parser::eq_bool() {
        if (st_lex.top()!=LEX_BOOL) throw "condition not bool";
        st_lex.pop();
    }

    void Parser::check_label() {
        if (!TID[c_val].get_is_label()) {
            throw "identifier is not a label";
        }
    }


int main() {
    try {
        Parser parser("1.txt");
        parser.analyze();
        return 0;
    }
    catch (char c) {
        cout << "Line " << Scanner::line_counter << ": unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "Line " << Scanner::line_counter << ": unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char *source) {
        cout << "Line " << Scanner::line_counter << ": " << source << endl;
        return 1;
    }
    catch (...) {
        cout << "Don't know" << endl;
    }
}