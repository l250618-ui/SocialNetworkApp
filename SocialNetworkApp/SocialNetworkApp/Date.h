#pragma once
#include <string>
using namespace std;

class Date {
private:
    int day, month, year;
public:
    Date();
    Date(int d, int m, int y);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    void setDate(int d, int m, int y);
    bool isWithin24Hours(const Date& current) const;  
    bool isSameDayAndMonth(const Date& other) const;
    void display() const;
    string toString() const;
};