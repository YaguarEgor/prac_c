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
#include <iomanip>


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

    const string& get_slex() const {
        return s_lex;
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
        Lex  hold_lex;                         // отложенная лексема
        bool hold_flag = false;                // есть ли отложенная лексема
        int loop_depth = 0;                    // для break
    
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
        void put_back(const Lex &l);             // вернуть лексему в поток
        bool is_expr_start(type_of_lex t);
        
    
    public:
        vector<Lex> poliz; 
        explicit Parser(const char *program);   // конструктор, принимает имя файла
        void analyze();                         // точка входа синтаксического анализа
    };
    
    
    
    // Конструктор: инициализация Scanner и чтение первой лексемы
    Parser::Parser(const char *program)
      : scan(program) {
        gl();
    }

    bool Parser::is_expr_start(type_of_lex t) {
        return t == LEX_ID  || t == LEX_NUM  || t == LEX_REAL_CONST ||
               t == LEX_STR || t == LEX_TRUE || t == LEX_FALSE      ||
               t == LEX_LPAREN || t == LEX_NOT;
    }
    
    // gl(): загрузить следующую лексему из Scanner
    void Parser::gl() {
        if (hold_flag) {                 // читаем из буфера
            curr_lex  = hold_lex;
            hold_flag = false;
        } else {                         // читаем «как раньше»
            curr_lex = scan.get_lex();
        }
        c_type = curr_lex.get_type();
        c_val  = curr_lex.get_value();
    }

    void Parser::put_back(const Lex &l) {    
        hold_lex  = l;
        hold_flag = true;
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
            // красивый вывод ПОЛИЗа
        cout << "\n--- POLIZ (size=" << poliz.size() << ") ---\n";
        for (size_t i = 0; i < poliz.size(); ++i) {
            cout << setw(4) << i << ": ";
            const Lex &lx = poliz[i];
            switch (lx.get_type()) {
                case POLIZ_LABEL:
                    cout << "LABEL " << lx.get_value();
                    break;
                case POLIZ_ADDRESS: {
                    int id = lx.get_value();
                    cout << "ADDR " << id 
                        << " (" << TID[id].get_name() << ")";
                    break;
                }
                case POLIZ_GO:
                    cout << "GO";
                    break;
                case POLIZ_FGO:
                    cout << "FGO";
                    break;
                case LEX_ID:
                    // выводим имя переменной
                    cout << TID[lx.get_value()].get_name();
                    break;
                case LEX_NUM:
                    // целочисленная константа
                    cout << lx.get_value();
                    break;
                case LEX_REAL_CONST:
                    // вещественная константа
                    cout << lx.get_dval();
                    break;
                case LEX_STR:
                    // строковая константа в кавычках
                    cout << '"' << lx.get_slex() << '"';
                    break;
                case LEX_TRUE:
                case LEX_FALSE:
                    // булева константа
                    cout << Scanner::TW[lx.get_type()];
                    break;
                default:
                    // все остальные символы (операторы, скобки и т.п.)
                    if (lx.get_type() < LEX_FIN)
                        cout << Scanner::TW[lx.get_type()];
                    else
                        cout << Scanner::TD[lx.get_type() - LEX_FIN];
                    break;
            }
            cout << '\n';
        }
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
                poliz.push_back(Lex(POLIZ_ADDRESS, id));
                gl(); // съесть '='
                E();  // разобрать выражение-инициализатор
    
                // проверить, что тип инициализатора совпадает с объявленным
                if (st_lex.top() != t) throw "initializer type mismatch";
                st_lex.pop();
                
                poliz.push_back(Lex(LEX_ASSIGN));
            }
        } while (c_type == LEX_COMMA && (gl(), true));
    }
    
    // S(): разбор операторов
    void Parser::S() {
        if (c_type == LEX_IF) {
            gl(); E(); eq_bool();
            int pl2 = poliz.size();  
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            poliz.push_back(Lex(POLIZ_FGO));
            S();
            int pl3 = poliz.size();
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            poliz.push_back(Lex(POLIZ_GO));
            poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
            if (c_type == LEX_ELSE) { gl(); S(); }
            poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
        }
        else if (c_type == LEX_WHILE) {
            gl();             // съели 'while'
            ++loop_depth;                // вошли в цикл
            int pl0 = poliz.size();
            E(); eq_bool();   // условие
            // если есть 'do', то съём его
            int pl1 = poliz.size();
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            poliz.push_back(Lex(POLIZ_FGO)); 
            if (c_type == LEX_DO) gl();
            // дальше S() разберёт либо одиночный stmt, либо блок {...}
            S();
            poliz.push_back(Lex(POLIZ_LABEL, pl0));  // jump-back target
            poliz.push_back(Lex(POLIZ_GO));          // unconditional
            poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
            --loop_depth;  
        }
        else if (c_type == LEX_READ) {
            gl();              // 'read'
            if (c_type != LEX_LPAREN) throw curr_lex;
            gl();              // '('
            if (c_type != LEX_ID) throw curr_lex;
            check_id_in_read();
            poliz.push_back(Lex(POLIZ_ADDRESS, c_val)); 
            gl();              // ID
            if (c_type != LEX_RPAREN) throw curr_lex;
            gl();              // ')'
            poliz.push_back(Lex(LEX_READ));
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
            poliz.push_back(Lex(LEX_WRITE));    
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
            Lex first = curr_lex;                    // запомнили ID
            gl();                                    // заглянули вперёд
            if (c_type == LEX_COLON) {               // =====  метка  id:
                TID[first.get_value()].put_is_label();
                gl();                                // съели ':'
                S();                                 // оператор после метки
            } else {                                 // =====  не метка  →  выражение
                put_back(curr_lex);                  // вернули look‑ahead
                curr_lex = first;                    // восстановили ID
                c_type   = first.get_type();
                c_val    = first.get_value();

                E();                                 // разбираем выражение, начиная с ID
                st_lex.pop();                        // тип результата выражения больше не нужен
            }
        }
        else if (is_expr_start(c_type)) {            // 1+3;  (a+b)*c;  "hi"+s;
            E();
            st_lex.pop();
        }
        else if (c_type == LEX_BREAK) {    // <— новый кейс для break
            if (loop_depth == 0)
                throw "break outside loop";       // вне циклов нельзя
            /* если break допустим, просто читаем и кладём метку,
            которая при интерпретации будет «перепрыгивать» дальше */
            poliz.push_back(Lex(POLIZ_GO));       // (простейшая реализация)
            gl();                        // просто «съедаем» break
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
    
    //F: атомарное выражение
    void Parser::F() {
        if (c_type == LEX_ID) {
            /* 1. запоминаем лексему и её id */
            Lex id_lex = curr_lex;
            int id     = c_val;
            
            /* 2. проверяем, объявлена ли переменная — тип нужен в любом
                  случае (LHS тоже должен быть проверен) */
            check_id();               // пушит тип в st_lex
            
            /* 3. заглядываем на следующий токен */
            gl();                     // прочитали look-ahead
            
            if (c_type == LEX_ASSIGN) {
                /* ----- вариант „присваивание“  lhs := rhs ----- */
                poliz.push_back(Lex(POLIZ_ADDRESS, id));   // адрес lhs
                gl();                                      // съели '='
                E();                                       // rhs
                eq_type();                                 // типы lhs / rhs
                poliz.push_back(Lex(LEX_ASSIGN));          // сам ":="
            }
            else {
                /* ----- обычное использование переменной ----- */
                poliz.push_back(id_lex);                   // кладём ID
            
                /* возвращаем look-ahead во входной поток */
                put_back(curr_lex);
                curr_lex = id_lex;       // восстановили текущий токен
                c_type   = id_lex.get_type();
                c_val    = id_lex.get_value();
            
                gl();                    // идём дальше
            }
        }
        else if (c_type == LEX_NUM) {
            st_lex.push(LEX_INT);
            poliz.push_back(curr_lex);  // целая константа
            gl();
        }
        else if (c_type == LEX_REAL_CONST) {
            st_lex.push(LEX_REAL);
            poliz.push_back(curr_lex);  // вещественная константа
            gl();
        }
        else if (c_type == LEX_STR) {
            st_lex.push(LEX_STRING);
            poliz.push_back(curr_lex);  // строковая константа
            gl();
        }
        else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
            st_lex.push(LEX_BOOL);
            poliz.push_back(curr_lex);  // булева константа
            gl();
        }
        else if (c_type == LEX_NOT) {
            gl();
            F();
            check_not();  // сам «not» пушится внутри check_not()
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
            poliz.push_back(Lex(op));
        }
        else if ((op==LEX_PLUS||op==LEX_MINUS||op==LEX_TIMES||op==LEX_SLASH)
            &&t1==LEX_REAL&&t2==LEX_REAL) {
            st_lex.push(LEX_REAL);
            poliz.push_back(Lex(op));
        }
        else if ((op==LEX_LSS||op==LEX_GTR||op==LEX_LEQ||op==LEX_GEQ)
                 &&t1==LEX_INT&&t2==LEX_INT) {
            st_lex.push(LEX_BOOL);
            poliz.push_back(Lex(op));
        }
        else if ((op==LEX_AND||op==LEX_OR)
                 &&t1==LEX_BOOL&&t2==LEX_BOOL) {
            st_lex.push(LEX_BOOL);
            poliz.push_back(Lex(op));
        }
        else if ((op==LEX_EQ||op==LEX_NEQ)&&t1==t2) {
            st_lex.push(LEX_BOOL);
            poliz.push_back(Lex(op)); 
        }
        else if (op==LEX_PLUS&&t1==LEX_STRING&&t2==LEX_STRING) {
            st_lex.push(LEX_STRING);
            poliz.push_back(Lex(op)); 
        }
        else throw "type mismatch in operation";
    }
    
    // not должно применяться к bool
    void Parser::check_not() {
        if (st_lex.top()!=LEX_BOOL) throw "not applied to non-bool";
        st_lex.pop(); st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_NOT));
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