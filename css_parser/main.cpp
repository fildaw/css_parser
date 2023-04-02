#include "css_parser.h"

using namespace std;

int stoi(MyString& str) {
    int num = 0;
    for (int i = 0; i < str.getLength(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num *= 10;
            num += (str[i]-'0');
        } else {
            return 0;
        }
    }

    return num;
}

int main() {
    List<Block> css;
    int c;
    while ((c = getchar()) != EOF) {
        ungetc(c, stdin);
        if (!parse_css(css)) {
            // Got EOF
            break;
        }
        // Skip possible newline
        if ((c = getchar()) != '\n') {
            ungetc(c, stdin); // If next char is not newline, return it to stdin
        }
        bool command_parsing_mode = true;
        while (command_parsing_mode) {
            MyString command(200);
            if (!command.getline()) {
                if (command.getLength() == 0 && command == "") {
                    return 0;
                }
            }
            //cout << command << endl;
            /*if (command == "25,A,?") {
                cout << css[24] << endl;
            }*/
            /*cout << command << endl;
            for (int i = 0; i < css.getLength(); i++) {
                cout << css[i] << endl;
            }*/
            if (command == "?") {
                // CSS sections count
                cout << "? == " << css.getLength() << endl;
            } else if (command == "????") {
                // ...
            } else if (command == "****") {
                // Stop parsing commands, start parsing css again
                command_parsing_mode = false;
            } else {
                int i = 0;
                MyString first_arg(200);
                bool got_comma = false;
                for (;i < command.getLength() && !got_comma; i++) {
                    if (command[i] == ',') {
                        got_comma = true;
                    } else {
                        first_arg += command[i];
                    }
                }
                if (!got_comma) {
                    continue; // Skip invalid command
                }
                char second_arg = command[i];
                i += 2; // Skip second_arg and comma
                MyString third_arg(20);
                for (;i < command.getLength(); i++) {
                    third_arg += command[i];
                }
                if (second_arg == 'S') {
                    // Selector related command
                    int try_int = stoi(first_arg);
                    if (try_int > 0) {
                        int block_number = try_int;
                        try_int = stoi(third_arg);
                        if (try_int > 0) {
                            // interpreting as i,S,j command
                            int selector_number = try_int;
                            block_number--;
                            selector_number--;
                            if (block_number < css.getLength()) {
                                const List<Selector>& sels = css[block_number].selector_list;
                                if (selector_number < sels.getLength()) {
                                    cout << command << " == " << sels[selector_number].getName() << endl;
                                }
                            }
                        } else if (third_arg == "?") {
                            block_number--;
                            if (block_number < css.getLength()) {
                                const List<Selector>& sels = css[block_number].selector_list;
                                cout << command << " == " << sels.getLength() << endl;
                            }
                        }
                    } else {
                        if (third_arg == "?") {
                            MyString selector_name = move(first_arg);
                            MyIterator<Block> block_iter = css.getFrontIterator();
                            int sel_occurs = 0;
                            while (block_iter.is_next()) {
                                Block& b = block_iter.next();
                                MyIterator<Selector> sel_iter = b.selector_list.getFrontIterator();
                                while (sel_iter.is_next()) {
                                    Selector& s = sel_iter.next();
                                    if (s.getName() == selector_name) {
                                        sel_occurs++;
                                    }
                                }
                            }
                            cout << command << " == " << sel_occurs << endl;
                        }
                    }
                } else if (second_arg == 'A') {
                    // Attribute related command
                    int try_int = stoi(first_arg);
                    if (try_int > 0) {
                        int block_number = try_int;
                        try_int = stoi(third_arg);
                        if (third_arg == "?") {
                            // Interpreting i,A,? command
                            block_number--;
                            //cout << "using block number: " << block_number << endl;
                            if (block_number < css.getLength()) {
                                cout << command << " == " << css[block_number].attribute_list.getLength() << endl;
                            }
                        } else {
                            // Interpreting i,A,n
                            MyString attr_name = move(third_arg);
                            block_number--;
                            if (block_number < css.getLength()) {
                                MyIterator<Attribute> attr_iter = css[block_number].attribute_list.getFrontIterator();
                                while (attr_iter.is_next()) {
                                    Attribute& a = attr_iter.next();
                                    if (a.getName() == attr_name) {
                                        cout << command << " == " << a.getValue() << endl;
                                    }
                                }
                                
                            }
                        }
                    } else {
                        if (third_arg == "?") {
                            // Interpreting n,A,?
                            MyString attr_name = move(first_arg);
                            MyIterator<Block> block_iter = css.getFrontIterator();
                            int attr_occurs = 0;
                            while (block_iter.is_next()) {
                                Block& b = block_iter.next();
                                MyIterator<Attribute> attr_iter = b.attribute_list.getFrontIterator();
                                while (attr_iter.is_next()) {
                                    Attribute& a = attr_iter.next();
                                    if (a.getName() == attr_name) {
                                        attr_occurs++;
                                    }
                                }
                            }
                            cout << command << " == " << attr_occurs << endl;
                        }
                    }
                } else if (second_arg == 'E') {
                    // Interpreting z,E,n
                    MyString sel_name = move(first_arg);
                    MyString attr_name = move(third_arg);
                    MyIterator<Block> block_iter = css.getBackIterator();
                    bool found = false;
                    while (block_iter.is_prev() && !found) {
                        Block& b = block_iter.prev();
                        MyIterator<Selector> sel_iter = b.selector_list.getFrontIterator();
                        while (sel_iter.is_next() && !found) {
                            Selector& s = sel_iter.next();
                            if (s.getName() == sel_name) {
                                found = true;
                            }
                        }
                        if (found) {
                            MyIterator<Attribute> attr_iter = b.attribute_list.getFrontIterator();
                            while (attr_iter.is_next()) {
                                Attribute& a = attr_iter.next();
                                if (a.getName() == attr_name) {
                                    cout << command << " == " << a.getValue() << endl;
                                }
                            }
                        }
                    }
                } else if (second_arg == 'D') {
                    int try_int  = stoi(first_arg);
                    if (try_int > 0) {
                        int block_number = try_int;
                        block_number--;
                        if (block_number < css.getLength()) {
                            if (third_arg == "*") {
                                // Interpreting i,D,*
                                css.remove(block_number);
                                cout << command << " == deleted" << endl;
                            } else {
                                // Interpreting i,D,n
                                MyString attr_name = move(third_arg);
                                Block& b = css.get(block_number);
                                Attribute a = Attribute(move(attr_name), "");
                                if (b.attribute_list.remove(a)) {
                                    if (b.attribute_list.getLength() == 0) {
                                        css.remove(block_number);
                                    }
                                    cout << command << " == deleted" << endl;
                                }
                            }
                        }
                    }
                }
            }
                
        }
    }
    
    
    return 0;
}