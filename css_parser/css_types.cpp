#include "css_types.h"

Selector::Selector(MyString&& selector_name) : selector_name(move(selector_name))
{
}

Selector::Selector(Selector&& orig) : selector_name(move(orig.selector_name)) 
{
}

Selector::Selector() : selector_name("")
{
}

Selector& Selector::operator=(Selector&& right) {
    this->selector_name = move(right.selector_name);

    return *this;
}

bool Selector::operator==(const Selector& s2) {
    return this->selector_name == s2.selector_name;
    //return false;
}

const MyString& Selector::getName() const {
    return this->selector_name;
}

Attribute& Attribute::operator=(Attribute&& right) {
    this->attribute_name = move(right.attribute_name);
    this->attribute_value = move(right.attribute_value);

    return *this;
}

const MyString& Attribute::getName() const {
    return this->attribute_name;
}

const MyString& Attribute::getValue() const {
    return this->attribute_value;
}

ostream& operator<<(ostream& os, const Selector& s) {
    os << "Selector { name: " << s.selector_name << " }";
    return os;
}

Attribute::Attribute(Attribute&& orig) : attribute_name(move(attribute_name)), attribute_value(move(attribute_value)) {
}


Attribute::Attribute(MyString&& attribute_name, MyString&& attribute_value) : attribute_name(move(attribute_name)), attribute_value(move(attribute_value))
{
}

Attribute::Attribute() : attribute_name(""), attribute_value("")
{
}

bool Attribute::operator==(const Attribute& a2) {
    return this->attribute_name == a2.attribute_name;
}

Attribute Attribute::parse(MyString& attribute)
{
    int i = 0;
    // Skip whitespaces at the beginning
    while ((attribute[i] == ' ' || attribute[i] == '\t') && i < attribute.getLength()) {
        i++;
    }

    // Skip whitespaces at the end
    int j = attribute.getLength()-1;
    while ((attribute[j] == ' ' || attribute[j] == '\t') && j >= 0) {
        j--;
    }
    MyString attribute_name(100);
    MyString attribute_value(100);
    while (attribute[i] != ':' && i <= j) {
        // Skip whitespaces inside param
        while ((attribute[i] == ' ' || attribute[i] == '\t') && i < attribute.getLength()) {
            i++;
        }
        attribute_name += attribute[i];
        i++;
    }
    i++; // Skip the colon
    // Skip the whitespaces before attr value
    while ((attribute[i] == ' ' || attribute[i] == '\t') && i < attribute.getLength()) {
        i++;
    }
    while (i <= j) {
        attribute_value += attribute[i];
        i++;
    }
    return Attribute(move(attribute_name), move(attribute_value));
}

ostream& operator<<(ostream& os, const Attribute& s) {
    os << "Attribute { name: " << s.attribute_name << ", value: " << s.attribute_value << " }";
    return os;
}