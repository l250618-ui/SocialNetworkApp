#include "Date.h"
#include <iostream>

// this is the default constructor for date
Date::Date() {
    day = 1;
    month = 1;
    year = 2000;
}
Date::Date(int d, int m, int y) { // used when you know the actual date
    day = d;
    month = m;
    year = y;
}

// getters
// const means they don't modify the object
int Date::getDay() const{
    return day;
}
int Date::getMonth() const{
    return month;
}
int Date::getYear() const{
    return year;
}

// setter that updates all 3 fields
// later used in socialnetworkingapp to set the current date
void Date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}

// returns true if this date is within 24 hours before current
// this is later used in viewHome to filter posts
bool Date::isWithin24Hours(const Date& current) const {
    // Same date = within 24 hours
    if (year == current.year && month == current.month && day == current.day) {
        return true;
    }

    // Check if this date is exactly one day before current date
    if (year == current.year && month == current.month && day == current.day - 1) {
        return true;
    }

    // Handle month boundary (e.g. Oct 31 when current is Nov 1)
    if (current.day == 1 && month == current.month - 1 && year == current.year) {
        int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (day == daysInMonth[month]) {
            return true;
        }
    }

    // Handle year boundary (Dec 31 when current is Jan 1)
    if (current.day == 1 && current.month == 1 && month == 12 && day == 31 && year == current.year - 1) {
        return true;
    }

    return false;
}

// returns true if day and month match and ignores year.
// later used in seeYourMemories to find posts from the same calendar day in past years
bool Date::isSameDayAndMonth(const Date& other) const {
    if (day == other.day && month == other.month) {
        return true;
    }
    return false;
}
void Date::display() const{
    cout << day << "/" << month << "/" << year << endl;
}

// Returns the date as a string instead of printing it
// used inside ActivityPost::display() & Memory::display() to embed the date inside a longer output line
// basically allows date to be used in a sentence
string Date::toString() const{
    return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
}