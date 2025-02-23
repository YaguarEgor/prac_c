#include <iostream>
#include <cmath>
#include <iomanip>

class DateTimeSpan;

class DateTime {
    double m_value;

    bool isLeapYear(int year) const {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }
    
    int getDaysInMonth(int year, int month) const {
        int daysInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear(year))
            return 29;
        return daysInMonths[month-1];
    }
    
    void computeDate(int offset, int &year, int &month, int &day) const {
        year = 2000;
        month = 1;
        day = 1;
        
        if (offset > 0) {
            while (offset > 0) {
                int daysInCurrentMonth = getDaysInMonth(year, month);
                int remaining = daysInCurrentMonth - day;
                if (offset <= remaining) {
                    day += offset;
                    offset = 0;
                } else {
                    offset -= (remaining + 1);
                    day = 1;
                    month++;
                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
            }
        } else if (offset < 0) {
            offset = -offset;
            while (offset > 0) {
                if (offset < day) {
                    day -= offset;
                    offset = 0;
                } else {
                    offset -= day;
                    month--;
                    if (month < 1) {
                        month = 12;
                        year--;
                    }
                    day = getDaysInMonth(year, month);
                }
            }
        }
    }
    
public:
    DateTime() : m_value(0.0) { }
    
    DateTime(double value) : m_value(value) { }

    void print_date() const {
        int daysOffset = std::floor(m_value);
        double fraction = m_value - std::floor(m_value);
        
        int year, month, day;
        computeDate(daysOffset, year, month, day);
        
        int totalSeconds = std::round(fraction * 24*3600);

        int hour = totalSeconds / 3600;
        int minute = (totalSeconds % 3600) / 60;
        int second = totalSeconds % 60;
        
        std::cout << std::setfill('0')
           << std::setw(2) << day << "."
           << std::setw(2) << month << "."
           << std::setw(4) << year << " "
           << std::setw(2) << hour << ":"
           << std::setw(2) << minute << ":"
           << std::setw(2) << second << std::endl;
    }

    double get_m() const{
        return m_value;
    }
    
    DateTimeSpan operator-(const DateTime& other);

    DateTime operator+ (const DateTimeSpan& span);

    DateTime operator- (const DateTimeSpan& span);
};

class DateTimeSpan {
    double m_value;

public:
    DateTimeSpan (int days = 0, int hours = 0, int minutes = 0, int seconds = 0) {
        m_value = double(seconds)*(1.0/3600*24) + double(minutes)*(1.0/24*60) + double(hours)*(1.0/24) + days;
    }

    DateTimeSpan (double span) {
        m_value = span;
    } 

    double get_diff() const {
        return m_value;
    }

    DateTimeSpan operator+(const DateTimeSpan &other) const {
        return DateTimeSpan(m_value + other.m_value);
    }
    
    DateTimeSpan operator-(const DateTimeSpan &other) const {
        return DateTimeSpan(m_value - other.m_value);
    }

    DateTimeSpan operator-() const {
        return DateTimeSpan(-m_value);
    }

    void print() const {
        int days = std::floor(m_value);
        int totalSeconds = (m_value - days)*3600*24;
        int hour = totalSeconds / 3600;
        int minute = (totalSeconds % 3600) / 60;
        int second = totalSeconds % 60;
        std::cout << "Days: " << days << ", hours: " << hour << ", minutes: " << minute << ", seconds: " << second << std::endl;
    }
};

DateTimeSpan DateTime::operator-(const DateTime& other) {
    return DateTimeSpan(this->m_value - other.get_m());
}

DateTime DateTime::operator+ (const DateTimeSpan& span) {
    return DateTime(this->m_value + span.get_diff());
}

DateTime DateTime::operator- (const DateTimeSpan& span) {
    return DateTime(this->m_value - span.get_diff());
}

int main() {
    DateTime dt1(1.0);       
    DateTime dt2(2.1);      
    DateTime dt3(1.75);     
    DateTime dt4(-1.0);     
    DateTime dt5(-1.5);     
    DateTime dt6(-1.25);    
    DateTime dt7(-1.1);     

    std::cout << "1.0      => ";
    dt1.print_date();
    std::cout << "2.1      => ";
    dt2.print_date();
    std::cout << "1.75     => ";
    dt3.print_date();
    std::cout << "-1.0     => ";
    dt4.print_date();
    std::cout << "-1.5     => ";
    dt5.print_date();
    std::cout << "-1.25    => ";
    dt6.print_date();
    std::cout << "-1.1     => ";
    dt7.print_date();

    DateTime d1(1.0);      
    DateTime d2(2.1);      
    
    DateTimeSpan s = d2 - d1;
    std::cout << "d2 - d1 = s = ";
    s.print();
    
    DateTime d3 = d1 + s;
    std::cout << "d1 + s = ";
    d3.print_date();
    
    DateTime d4 = d2 - s;
    std::cout << "d2 - s = ";
    d4.print_date();
    
    DateTimeSpan s_neg = -s;
    std::cout << "-s = ";
    s_neg.print();
    
    DateTimeSpan s1(0, 1, 30, 0);
    
    DateTimeSpan s2 = s - s1;
    std::cout << "s - s1 = ";
    s2.print();
    
    DateTimeSpan s3 = s + s1;
    std::cout << "s + s1 = ";
    s3.print();

    return 0;
}
