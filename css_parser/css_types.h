#include "string.h"

class Selector {
    MyString selector_name;
    public:
        Selector();
        Selector(Selector&& orig);
        Selector(MyString&& selector_name);
        Selector& operator=(Selector&& right);
        bool operator==(const Selector& s2);
        const MyString& getName() const;
    friend ostream& operator<<(ostream& os, const Selector& s);
};

class Attribute {
    MyString attribute_name;
    MyString attribute_value;
    public:
        Attribute(MyString&& attribute_name, MyString&& attribute_value);
        Attribute();
        Attribute(Attribute&& orig);
        Attribute& operator=(Attribute&& right);
        bool operator==(const Attribute& a2);
        static Attribute parse(MyString& attribute);
        const MyString& getName() const;
        const MyString& getValue() const;
    friend ostream& operator<<(ostream& os, const Attribute& s);
};


