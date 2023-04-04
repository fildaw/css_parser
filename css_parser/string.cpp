#include "string.h"

MyString::MyString(const char *s)
{
    length = strlen(s);
    capacity = 2*length+1;
    str = new char[capacity];
    strncpy(str, s, length);
}

MyString::MyString(int capacity)
{
    this->capacity = capacity;
    str = new char[capacity];
    this->length = 0;
    str[0] = '\0';
}

MyString::MyString(const MyString& s)
{
    length = s.length;
    capacity = s.capacity;
    str = new char[capacity];
    strncpy(str, s.str, length);
}

// konstruktor przenoszący
MyString::MyString(MyString&& orig) : str(orig.str), length(orig.length), capacity(orig.capacity) {
    //cout << "Move constructor" << endl;
    orig.length = 0;
    orig.capacity = 0;
    orig.str = nullptr;
}

MyString::~MyString()
{
    if (str != nullptr) {
        //cout << "Deleting '" << str << "'" << endl;
        delete[] str;
    }
        
}

ostream& operator<<(ostream& os, const MyString& s)
{
    os << s.str;
    return os;
}

/*istream& operator>>(istream& is, MyString& s)
{
    char *temp = new char[s.capacity];
    is >> temp;
    delete[] s.str;
    s.length = strlen(temp);
    s.capacity = 2*s.length;
    s.str = new char[s.capacity];
    if (s.length > 0) {
        strcpy_s(s.str, s.capacity, temp);
    }
    delete[] temp;
    return is;
}*/

bool MyString::getline() {
    delete[] this->str;
    char* temp = new char[this->capacity];
    int i = 0;
    int c;
    while (true) {
        c = getchar();
        if (c == '\n') {
            temp[i] = '\0';
            this->length = i;
            this->str = temp;
            return true;
        }
        else if (c == EOF) {
            temp[i] = '\0';
            this->length = i;
            this->str = temp;
            return false;
        }
        else {
            temp[i] = (char) c;
        }
        i++;
    }
    delete[] temp;
    return false;
}

MyString MyString::operator+(const MyString& s2) const
{
    const MyString& s1 = *this;
    MyString temp(2*(s1.length + s2.length) + 1);
    strncpy(temp.str, s1.str, s1.length);
    strncat(temp.str, s2.str, s2.length);
    temp.length = s1.length + s2.length;
    return temp;
}

MyString MyString::operator+(const char *s2) const
{
    const MyString& s1 = *this;
    int s2_len = strlen(s2);
    MyString temp(2*(s1.length + s2_len) + 1);
    strncpy(temp.str, s1.str, s1.length);
    strncat(temp.str, s2, s2_len);
    temp.length = s2_len + s1.length;
    return temp;
}

bool MyString::operator==(const MyString& s2) const
{
    const MyString& s1 = *this;
    return strcmp(s1.str, s2.str) == 0;
}

bool MyString::operator==(const char *s2) const
{
    const MyString& s1 = *this;
    return strcmp(s1.str, s2) == 0;
}

MyString& MyString::operator+=(const MyString& s2) 
{
    MyString& s1 = *this;
    if (s1.capacity < s1.length + s2.length + 1) {
        int temp_size = 2 * (s1.length + s2.length) + 1;
        char *temp = new char[temp_size];
        strncpy(temp, s1.str, s1.length);
        strncat(temp, s2.str, s2.length);
        delete[] s1.str;
        s1.str = temp;
        s1.capacity = 2*(s1.length + s2.length) + 1;   
    } else {
        strncat(s1.str, s2.str, s2.length);
    }
    s1.length = s1.length + s2.length;

    return *this;
}

MyString& MyString::operator+=(const char *s2) 
{
    MyString& s1 = *this;
    int s2_len = strlen(s2);
    if (s1.capacity < s1.length + s2_len + 1) {
        int temp_size = 2 * (s1.length + s2_len) + 1;
        char *temp = new char[temp_size];
        strncpy(temp, s1.str, s1.length);
        strncat(temp, s2, s2_len);
        delete[] s1.str;
        s1.str = temp;
        s1.capacity = temp_size;
    } else {
        strncat(s1.str, s2, s2_len);
    }
    s1.length = s1.length + s2_len;

    return *this;
}

MyString& MyString::operator+=(char c) {
    MyString& s1 = *this;
    if (s1.capacity < s1.length + 2) {
        int temp_size = 2 * (s1.length + 1) + 1;
        char *temp = new char[temp_size];
        strncpy(temp, s1.str, s1.length);
        temp[s1.length] = c;
        temp[s1.length + 1] = '\0';
        delete[] s1.str;
        s1.str = temp;
        s1.capacity = temp_size;
    } else {
        s1.str[s1.length] = c;
        s1.str[s1.length + 1] = '\0';
    }
    s1.length += 1;

    return *this;
}

MyString& MyString::operator=(const MyString& s2)
{
    MyString& s1 = *this;
    if (s1.capacity < s2.length + 1) {
        delete[] s1.str;
        s1.str = new char[2*s2.length + 1];
        s1.capacity = 2*s2.length + 1;
    }
    strncpy(s1.str, s2.str, s2.length);
    s1.length = s2.length;

    return *this;
}

MyString& MyString::operator=(const char *s2)
{
    MyString& s1 = *this;
    int s2_len = strlen(s2);
    if (s1.capacity < s2_len + 1) {
        delete[] s1.str;
        s1.str = new char[2*s2_len + 1];
        s1.capacity = 2*s2_len + 1;
    }
    strncpy(s1.str, s2, s2_len);
    s1.length = s2_len;

    return *this;
}

// operator przypisania przenoszący
MyString& MyString::operator=(MyString&& right) {
    swap(str, right.str);
    swap(length, right.length);
    swap(capacity, right.capacity);
    return *this;
}

char MyString::operator[](int i) const
{ 
    return str[i]; 
}

int MyString::getLength() const
{ 
    return length; 
}

