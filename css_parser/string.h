#include <iostream>
#include <cstring>

using namespace std;

class MyString {
    private:
        char *str;
        int capacity;
        int length;
    public:
        MyString(const MyString& s);
        MyString(const char *s);
        MyString(int capacity);
        MyString(MyString&& orig);
        ~MyString();

        MyString operator+(const MyString& s2);
        MyString operator+(const char *s2);
        bool operator==(const MyString& s2) const;
        bool operator==(const char *s2) const;
        MyString& operator+=(const MyString& s2);
        MyString& operator+=(const char *s2);
        MyString& operator+=(char c);
        MyString& operator=(MyString&& right);
        MyString& operator=(const MyString& s2);
        MyString& operator=(const char *s2);
        char operator[](int i);
        int getLength();
        int getCapacity();
        bool getline();

    friend ostream& operator<<(ostream& os, const MyString& s);
    //friend istream& operator>>(istream& is, MyString& s);
};