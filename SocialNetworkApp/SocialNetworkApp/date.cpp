#include "Date.h"
#include <iostream>
Date::Date() {
    day = 1;
    month = 1;
    year = 2000;
}
Date::Date(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}
int Date::getDay() const{
    return day;
}
int Date::getMonth() const{
    return month;
}
int Date::getYear() const{
    return year;
}
void Date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}
bool Date::isWithin24Hours(const Date& current) const{
    // Same date = within 24 hours
    if (year == current.year && month == current.month && day == current.day) {
        return true;
    }

    // Check if this date is exactly one day before current date
    if (year == current.year && month == current.month && day == current.day - 1) {
        return true;
    }

    return false;
}
void Date::display() const{
    cout << day << "/" << month << "/" << year << endl;
}
string Date::toString() const{
    return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
}