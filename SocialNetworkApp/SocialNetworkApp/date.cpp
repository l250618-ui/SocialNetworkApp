#include "Date.h"
#include <iostream>
using namespace std;

Date::Date() : day(1), month(1), year(2000) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay()
 const 
 { return day; }


int Date::getMonth()
 const
  { return month; }


int Date::getYear() 


const { return year; }

void Date::setDate(int d, int m, int y) {
    day = d; month = m; year = y;
}

// here returning  true if this date is the same or 1 day before current

bool Date::isWithin24Hours(const Date& current) const {
    if (year == current.year && month == current.month) {
        return (current.day - day) <= 1 && (current.day - day) >= 0;
    }
    return false;
}

void Date::display() const {
    cout << day << "/" << month << "/" << year;
}

string Date::toString() const {
    return to_string(day) + "/" + to_string(month) + "/" + to_string(year);}