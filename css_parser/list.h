#include "css_types.h"

#define PACK 4

template<class T>
class Element {
    public:
        T value;
        bool empty;
        Element();
};

template<class T>
Element<T>::Element() : value(T()), empty(true) 
{
}

template<class T>
class Node {
    public:
        Element<T> elements[PACK];
        Node *next;
        Node *prev;
        Node<T>& operator=(const Node<T>&);
};

template<class T>
class MyIterator {
    private:
        Node<T> *current_node;
        int pos_in_pack;
    public:
        MyIterator(Node<T>* start_node);
        MyIterator(Node<T>* start_node, int init_pos_in_pack);
        T& next();
        T& prev();
        bool is_next();
        bool is_prev();
};

template<class T>
MyIterator<T>::MyIterator(Node<T>* start_node) : current_node(start_node), pos_in_pack(-1) 
{
}

template<class T>
MyIterator<T>::MyIterator(Node<T>* start_node, int init_pos_in_pack) : current_node(start_node), pos_in_pack(init_pos_in_pack)
{
}

template<class T>
T& MyIterator<T>::next() {
    T& start = this->current_node->elements[this->pos_in_pack].value;
    Node<T> *curr = this->current_node;
    int pos = this->pos_in_pack;
    pos++;
    while (curr != nullptr) {
        while (pos < PACK) {
            if (!(curr->elements[pos].empty)) {
                // If found, update iterator data
                this->pos_in_pack = pos;
                this->current_node = curr;
                return curr->elements[pos].value;
            }
            pos++;
        }
        pos = 0;
        curr = curr->next;
    }
    // If not next, return prev element. We will not update iterator data
    return start;
}

template<class T>
T& MyIterator<T>::prev() {
    T& start = this->current_node->elements[this->pos_in_pack].value;
    Node<T> *curr = this->current_node;
    int pos = this->pos_in_pack;
    pos--;
    while (curr != nullptr) {
        while (pos >= 0) {
            if (!(curr->elements[pos].empty)) {
                // If found, update iterator data
                this->pos_in_pack = pos;
                this->current_node = curr;
                return curr->elements[pos].value;
            }
            pos--;
        }
        pos = PACK-1;
        curr = curr->prev;
    }
    // If not prev, return start element. We will not update iterator data
    return start;
}

template<class T>
bool MyIterator<T>::is_next() {
    Node<T> *curr = this->current_node;
    int pos = this->pos_in_pack;
    pos++;
    while (curr != nullptr) {
        while (pos < PACK) {
            if (!(curr->elements[pos].empty)) {
                // Found
                return true;
            }
            pos++;
        }
        pos = 0;
        curr = curr->next;
    }
    return false;
}

template<class T>
bool MyIterator<T>::is_prev() {
    Node<T> *curr = this->current_node;
    int pos = this->pos_in_pack;
    pos--;
    while (curr != nullptr) {
        while (pos >= 0) {
            if (!(curr->elements[pos].empty)) {
                // Found
                return true;
            }
            pos--;
        }
        pos = PACK-1;
        curr = curr->prev;
    }
    return false;
}

template<class T>
class List {
private:
    Node<T> *first;
    Node<T> *last;
    int length;
public:
    List();
    ~List();
    void add(T&& value);
    void remove(int i);
    bool remove(T& value);
    int getLength() const;
    static List<T> parse(MyString& buffer);
    List<T>& operator=(const List<T>& l2);
    const T& operator[](int i) const;
    T& get(int i);
    T getValue();
    MyIterator<T> getFrontIterator() const;
    MyIterator<T> getBackIterator() const;

friend ostream& operator<<(ostream& os, const List<Selector>& l);
friend ostream& operator<<(ostream& os, const List<Attribute>& l);
};

template <class T>
MyIterator<T> List<T>::getFrontIterator() const {
    return MyIterator<T>(first);
}

template <class T>
MyIterator<T> List<T>::getBackIterator() const {
    return MyIterator<T>(last, PACK);
}

template <class T>
const T& List<T>::operator[](int i) const
{
    int j = 0;
    Node<T> *curr = first;
    while (curr != nullptr) {
        for (int k = 0; k < PACK; k++) {
            Element<T>& curr_el = curr->elements[k];
            if (curr_el.empty) {
                continue;
            }
            if (j == i) {
                return curr_el.value;
            }
            j++;
        }
        curr = curr->next;
    }
    return first->elements[0].value;
}

template <class T>
T& List<T>::get(int i) {
    int j = 0;
    Node<T> *curr = first;
    while (curr != nullptr) {
        for (int k = 0; k < PACK; k++) {
            Element<T>& curr_el = curr->elements[k];
            if (curr_el.empty) {
                continue;
            }
            if (j == i) {
                return curr_el.value;
            }
            j++;
        }
        curr = curr->next;
    }
    return first->elements[0].value;
}

template <class T>
void List<T>::remove(int i) {
    if (this->length == 0 || first == nullptr) {
        // For safety
        return;
    }
    int j = 0;
    Node<T> *curr = first;
    bool found = false;
    while (curr != nullptr && !found) {
        bool empty_node = true;
        for (int k = 0; k < PACK; k++) {
            Element<T>& curr_el = curr->elements[k];
            if (curr_el.empty) {
                continue;
            }
            if (j == i) {
                // Found element to delete, mark it as empty
                curr_el.empty = true;
                found = true;
                this->length--;
            }
            if (!curr_el.empty) {
                empty_node = false;
            }
            j++;
        }
        if (empty_node) {
            // Whole node is empty
            Node<T>* prev = curr->prev;
            Node<T>* next = curr->next;
            if (prev != nullptr) {
                prev->next = next;
            }
            if (next != nullptr) {
                next->prev = prev;
            }
            if (curr == this->first) {
                this->first = next;
            }
            if (curr == this->last) {
                this->last = prev;
            }
            delete curr;
            break;
        }
        curr = curr->next;
    }
}

template <class T>
bool List<T>::remove(T& value) {
    if (this->length == 0 || first == nullptr) {
        // For safety
        return false;
    }
    Node<T> *curr = first;
    bool found = false;
    while (curr != nullptr && !found) {
        bool empty_node = true;
        for (int k = 0; k < PACK; k++) {
            Element<T>& curr_el = curr->elements[k];
            if (curr_el.empty) {
                continue;
            }
            if (curr_el.value == value) {
                // Found element to delete, mark it as empty
                curr_el.empty = true;
                found = true;
                this->length--;
            }
            if (!curr_el.empty) {
                empty_node = false;
            }
        }
        if (empty_node) {
            // Whole node is empty
            Node<T>* prev = curr->prev;
            Node<T>* next = curr->next;
            if (prev != nullptr) {
                prev->next = next;
            }
            if (next != nullptr) {
                next->prev = prev;
            }
            if (curr == this->first) {
                this->first = next;
            }
            if (curr == this->last) {
                this->last = prev;
            }
            delete curr;
            break;
        }
        curr = curr->next;
    }

    return found;
}

template <class T>
int List<T>::getLength() const {
    return this->length;
}

template <class T>
List<T>::List()
{
    Node<T> *_first = new Node<T>();
    _first->next = nullptr;
    _first->prev = nullptr;
    this->length = 0;
    this->first = _first;
    this->last = _first;
}

template <class T>
List<T>::~List() {
    
}

template <class T>
void List<T>::add(T&& value)
{
    if (first == nullptr && this->length == 0) {
        // Somehow got uninitialized list, maybe all elements have been deleted.
        // We must initialize it before we start adding to it
        first = new Node<T>();
        first->next = nullptr;
        first->prev = nullptr;
        last = first;
    }

    Node<T> *curr = first;
    Node<T>* prev = curr->prev;
    while (curr != nullptr) {
        for (int k = 0; k < PACK; k++) {
            Element<T>& curr_el = curr->elements[k];
            if (curr_el.empty) {
                curr_el.value = move(value);
                curr_el.empty = false;
                this->length++;
                return;
            }
            else if (curr_el.value == value) {
                curr_el.value = move(value);
                curr_el.empty = false;
                // We are replacing, so length shouldn't be modified
                return;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    Node<T> *new_node = new Node<T>();
    new_node->elements[0].value = move(value);
    new_node->elements[0].empty = false;
    new_node->next = nullptr;
    new_node->prev = prev;
    prev->next = new_node;
    this->last = new_node;
    this->length++;
}

template <class T>
List<T>& List<T>::operator=(const List<T>& l2) {
    this->first = l2.first;
    this->length = l2.length;
    this->last = l2.last;
    return *this;
}
