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
#include <unordered_map>
#include <variant>


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
typedef std::variant<int, bool, double, std::string> Value;

class Ident {
    std::string name;
    bool declare = false;
    type_of_lex type;
    bool assign = false;
    bool is_label = false;
    Value value;
public:
    Ident() = default;
    explicit Ident(const std::string &n)
      : name(n) {}

    bool operator==(const std::string &s) const {
        return name == s;
    }

    const std::string &get_name() const {
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
        // initialize default value based on declared type
        switch (t) {
            case LEX_INT:    value = 0;       break;
            case LEX_BOOL:   value = false;   break;
            case LEX_REAL:   value = 0.0;     break;
            case LEX_STRING: value = std::string(); break;
            default: break;
        }
    }

    bool get_assign() const {
        return assign;
    }

    void put_assign() {
        assign = true;
    }

    Value get_value() const {
        return value;
    }

    void put_value(const Value &v) {
        // Optionally, check compatibility of v and type
        value = v;
    }

    bool get_is_label() const {
        return is_label;
    }

    void put_is_label() {
        is_label = true;
    }
};

// Global table of identifiers
std::vector<Ident> TID;

// Add or find an identifier by name, return its index
int put(const std::string &buf) {
    auto it = std::find(TID.begin(), TID.end(), buf);
    if (it != TID.end())
        return static_cast<int>(it - TID.begin());
    TID.emplace_back(buf);
    return static_cast<int>(TID.size() - 1);
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
        std::vector<std::vector<int>> break_stack;
        unordered_map<int,int>    label_positions; 
        vector<pair<int,int>>     unresolved_gotos; // список незакрытых goto: пара<позиция_в_poliz, id_метки>
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
               // Перед выводом POLIZ-а — бэчпатч всех forward-goto
        for (auto &p : unresolved_gotos) {
            int pos = p.first, id = p.second;
            auto it = label_positions.find(id);
            if (it == label_positions.end())
                throw "undefined label for goto";
            // заменяем placeholder-позицию на реальную
            poliz[pos] = Lex(POLIZ_LABEL, it->second);
        }
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
                TID[id].put_assign();
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
            ++loop_depth;  
            break_stack.emplace_back();              // вошли в цикл
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
            int loop_end = poliz.size();
            for (int pos : break_stack.back()) {
                poliz[pos] = Lex(POLIZ_LABEL, loop_end);
            }
            break_stack.pop_back();
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

            int arg_cnt = 0;
            do {
                E();                  // каждое E() запушило в poliz одно значение
                st_lex.pop();         // убираем тип
                ++arg_cnt;
            } while (c_type == LEX_COMMA && (gl(), true));
            if (c_type != LEX_RPAREN) throw curr_lex;
            gl();              // ')'
            poliz.push_back(Lex(LEX_NUM, arg_cnt));
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
            int id = first.get_value();
            gl();                                    // заглянули вперёд
            if (c_type == LEX_COLON) {               // =====  метка  id:
                // 1) Помечаем в TID как метку
                TID[id].put_is_label();
                // 2) Запоминаем позицию этой метки в poliz и вставляем её туда:
                int pos = poliz.size();
                label_positions[id] = pos;
                poliz.push_back(Lex(POLIZ_LABEL, pos));
                gl();  // съели ':'
                S();   // дальше оператор после метки                                // оператор после метки
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
            if (loop_depth == 0) throw "break outside loop";
            // 1) вставляем placeholder-метку и GO
            int pos = poliz.size();
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            poliz.push_back(Lex(POLIZ_GO));
            // 2) запоминаем, чтобы потом патчить:
            break_stack.back().push_back(pos);
            gl();
        }
        else if (c_type == LEX_GOTO) {
            gl();  // 'goto'
            if (c_type != LEX_ID) throw curr_lex;
            int id = c_val;
            gl();  // ID
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            // 1) В poliz вставляем placeholder-метку и GO
            int pos = poliz.size();
            poliz.push_back(Lex(POLIZ_LABEL, 0));  // сюда позже подставим правильную позицию
            poliz.push_back(Lex(POLIZ_GO));
            // 2) Запомним этот goto для последующего бэчпатча
            unresolved_gotos.emplace_back(pos, id);
            // ';' скушает внешний код
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
                TID[id].put_assign();
            }
            else {
                /* ----- обычное использование переменной ----- */
                poliz.push_back(id_lex);                   // кладём ID
            
                /* возвращаем look-ahead во входной поток */
                put_back(curr_lex);
                curr_lex = id_lex;       // восстановили текущий токен
                c_type   = id_lex.get_type();
                c_val    = id_lex.get_value();

                if (!TID[id].get_declare())
                    throw "identifier not declared";
                if (!TID[id].get_assign())                
                    throw "variable used before assignment";
            
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

// Value can hold int, bool, double or string
typedef std::variant<int, bool, double, std::string> Value;

// Helper for printing a Value
struct ValuePrinter {
    void operator()(int v) const           { std::cout << v; }
    void operator()(bool v) const          { std::cout << (v ? "true" : "false"); }
    void operator()(double v) const        { std::cout << v; }
    void operator()(const std::string &v) const { std::cout << v; }
};

class Executer {
public:
    void execute(const std::vector<Lex> &poliz);
private:
    // Pop a Value from stack
    Value pop(std::stack<Value> &st) {
        Value v = st.top(); st.pop();
        return v;
    }
};

void Executer::execute(const std::vector<Lex> &poliz) {
    std::stack<Value> args;
    for (size_t idx = 0; idx < poliz.size(); ++idx) {
        const Lex &pc = poliz[idx];
        switch (pc.get_type()) {
            case LEX_NUM:
                args.push(pc.get_value());
                break;

            case LEX_TRUE:
                args.push(true);
                break;
            case LEX_FALSE:
                args.push(false);
                break;

            case LEX_REAL_CONST:
                args.push(pc.get_dval());
                break;

            case LEX_STR:
                args.push(pc.get_slex());
                break;

            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(pc.get_value());
                break;

            case LEX_ID: {
                int id = pc.get_value();
                if (!TID[id].get_assign())
                    throw "POLIZ: indefinite identifier";
                args.push(TID[id].get_value()); // assumes Ident::get_value() returns Value
                break;
            }

            case LEX_NOT: {
                Value v = pop(args);
                if (auto p = std::get_if<bool>(&v)) args.push(!*p);
                else throw "POLIZ: not applied to non-bool";
                break;
            }

            case LEX_AND:
            case LEX_OR: {
                Value v2 = pop(args), v1 = pop(args);
                if (!std::holds_alternative<bool>(v1) || !std::holds_alternative<bool>(v2))
                    throw "POLIZ: &&/|| on non-bool";
                bool b1 = std::get<bool>(v1);
                bool b2 = std::get<bool>(v2);
                args.push(pc.get_type() == LEX_AND ? (b1 && b2) : (b1 || b2));
                break;
            }

            case POLIZ_GO: {
                int dest = std::get<int>(pop(args));
                idx = dest - 1;
                break;
            }

            case POLIZ_FGO: {
                int dest = std::get<int>(pop(args));
                bool cond = std::get<bool>(pop(args));
                if (!cond) idx = dest - 1;
                break;
            }

            case LEX_WRITE: {
                // сначала достаём, сколько аргументов нужно вывести
                int n = std::get<int>( pop(args) );
                // забираем их в вектор (они лежат в обратном порядке!)
                std::vector<Value> out;
                out.reserve(n);
                for (int k = 0; k < n; ++k) {
                    out.push_back( pop(args) );
                }
                // восстанавливаем нормальный порядок
                std::reverse(out.begin(), out.end());
                // выводим их подряд через пробел
                for (size_t i = 0; i < out.size(); ++i) {
                    std::visit(ValuePrinter(), out[i]);
                    if (i + 1 < out.size()) std::cout << ' ';
                }
                std::cout << '\n';
                break;
            }

            case LEX_READ: {
                int id = std::get<int>(pop(args));
                Value input;
                switch (TID[id].get_type()) {
                    case LEX_INT: {
                        int i; std::cin >> i;
                        input = i;
                        break;
                    }
                    case LEX_BOOL: {
                        std::string s;
                        while (std::cin >> s) {
                            if (s == "true" || s == "false") break;
                            std::cout << "Input true/false" << std::endl;
                        }
                        input = (s == "true");
                        break;
                    }
                    case LEX_REAL: {
                        double d; std::cin >> d;
                        input = d;
                        break;
                    }
                    case LEX_STRING: {
                        std::string str; std::cin >> str;
                        input = str;
                        break;
                    }
                    default:
                        throw "POLIZ: read to non-variable";
                }
                TID[id].put_value(input); // assumes Ident::put_value(Value)
                TID[id].put_assign();
                break;
            }

            case LEX_PLUS:
            case LEX_MINUS:
            case LEX_TIMES:
            case LEX_SLASH: {
                Value v2 = pop(args), v1 = pop(args);
                // Integer arithmetic
                if (std::holds_alternative<int>(v1) && std::holds_alternative<int>(v2)) {
                    int a = std::get<int>(v1), b = std::get<int>(v2);
                    switch (pc.get_type()) {
                        case LEX_PLUS:  args.push(a + b); break;
                        case LEX_MINUS: args.push(a - b); break;
                        case LEX_TIMES: args.push(a * b); break;
                        case LEX_SLASH:
                            if (b == 0) throw "POLIZ: divide by zero";
                            args.push(a / b);
                            break;
                        default: break;
                    }
                }
                // Real arithmetic
                else if (std::holds_alternative<double>(v1) && std::holds_alternative<double>(v2)) {
                    double a = std::get<double>(v1), b = std::get<double>(v2);
                    switch (pc.get_type()) {
                        case LEX_PLUS:  args.push(a + b); break;
                        case LEX_MINUS: args.push(a - b); break;
                        case LEX_TIMES: args.push(a * b); break;
                        case LEX_SLASH:
                            if (b == 0.0) throw "POLIZ: divide by zero";
                            args.push(a / b);
                            break;
                        default: break;
                    }
                }
                // String concatenation
                else if (pc.get_type() == LEX_PLUS &&
                         std::holds_alternative<std::string>(v1) &&
                         std::holds_alternative<std::string>(v2)) {
                    args.push(std::get<std::string>(v1) + std::get<std::string>(v2));
                }
                else throw "POLIZ: type mismatch in arithmetic";
                break;
            }

            case LEX_EQ:
            case LEX_NEQ:
            case LEX_GTR:
            case LEX_LSS:
            case LEX_GEQ:
            case LEX_LEQ: {
                Value v2 = pop(args), v1 = pop(args);
                bool result;
                // Integer comparisons
                if (std::holds_alternative<int>(v1) && std::holds_alternative<int>(v2)) {
                    int a = std::get<int>(v1), b = std::get<int>(v2);
                    switch (pc.get_type()) {
                        case LEX_EQ:  result = (a == b); break;
                        case LEX_NEQ: result = (a != b); break;
                        case LEX_GTR: result = (a >  b); break;
                        case LEX_LSS: result = (a <  b); break;
                        case LEX_GEQ: result = (a >= b); break;
                        case LEX_LEQ: result = (a <= b); break;
                        default:    result = false;
                    }
                }
                // Real comparisons
                else if (std::holds_alternative<double>(v1) && std::holds_alternative<double>(v2)) {
                    double a = std::get<double>(v1), b = std::get<double>(v2);
                    switch (pc.get_type()) {
                        case LEX_EQ:  result = (a == b); break;
                        case LEX_NEQ: result = (a != b); break;
                        case LEX_GTR: result = (a >  b); break;
                        case LEX_LSS: result = (a <  b); break;
                        case LEX_GEQ: result = (a >= b); break;
                        case LEX_LEQ: result = (a <= b); break;
                        default:    result = false;
                    }
                }
                // String equality/inequality
                else if (std::holds_alternative<std::string>(v1) &&
                         std::holds_alternative<std::string>(v2) &&
                         (pc.get_type() == LEX_EQ || pc.get_type() == LEX_NEQ)) {
                    const auto &s1 = std::get<std::string>(v1);
                    const auto &s2 = std::get<std::string>(v2);
                    result = (pc.get_type() == LEX_EQ ? (s1 == s2) : (s1 != s2));
                }
                else throw "POLIZ: type mismatch in comparison";
                args.push(result);
                break;
            }

            case LEX_ASSIGN: {
                Value val = pop(args);
                int id    = std::get<int>(pop(args));
                TID[id].put_value(val); // assumes Ident::put_value(Value)
                TID[id].put_assign();
                args.push(val);
                break;
            }

            default:
                throw "POLIZ: unexpected elem";
        }
    }
    std::cout << "Finish of executing!!!" << std::endl;
}

class Interpretator { 
    Parser pars; 
    Executer E; 
public: 
    Interpretator (const char * program) : pars(program) { }; 
    void interpretation ( ); 
};  

void Interpretator :: interpretation ( ) { 
    pars.analyze ( ); 
    E.execute ( pars.poliz ); 
}


int main() {
    try {
        Interpretator i("1.txt");
        i.interpretation();
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