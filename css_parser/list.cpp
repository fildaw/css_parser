#include "list.h"

template <>
List<Selector> List<Selector>::parse(MyString& buffer) {
    int i = 0;
    List<Selector> list;
    // Skip whitespaces at the beginning
    while ((buffer[i] == ' ' || buffer[i] == '\t') && i < buffer.getLength()) {
        i++;
    }

    // Skip whitespaces at the end
    int j = buffer.getLength()-1;
    while ((buffer[j] == ' ' || buffer[j] == '\t') && j >= 0) {
        j--;
    }
    while (i <= j) {
        MyString selector_name(10); 
        while (buffer[i] != ',' && i <= j) {
            selector_name += buffer[i];
            i++;
        }
        i++; // Skip the comma
        // Skip the whitespaces
        while ((buffer[i] == ' ' || buffer[i] == '\t') && i < buffer.getLength()) {
            i++;
        }
        Selector selector(move(selector_name));
        list.add(move(selector));
    }

    return list;
}

ostream& operator<<(ostream& os, const List<Selector>& l) {
    os << "SelectorList [ ";
    Node<Selector> *l_ptr = static_cast<Node<Selector>*> (l.first);
    while (l_ptr != nullptr) {
        for (int i = 0; i < PACK; i++) {
            Element<Selector>& curr_el = l_ptr->elements[i];
            if (!curr_el.empty)
                os << l_ptr->elements[i].value << " ";
        }
        l_ptr = static_cast<Node<Selector>*> (l_ptr->next);
    }
    os << "]";
    return os;
}

ostream& operator<<(ostream& os, const List<Attribute>& l) {
    os << "AttributeList [ ";
    Node<Attribute> *l_ptr = static_cast<Node<Attribute>*> (l.first);
    while (l_ptr != nullptr) {
        for (int i = 0; i < PACK; i++) {
            Element<Attribute>& curr_el = l_ptr->elements[i];
            if (!curr_el.empty)
                os << l_ptr->elements[i].value << " ";
        }
        l_ptr = static_cast<Node<Attribute>*> (l_ptr->next);
    }
    os << "]";
    return os;
}

