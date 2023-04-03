#include "css_parser.h"
#include <iostream>

using namespace std;

Block::Block(List<Selector> sels, List<Attribute> attrs) : selector_list(sels), attribute_list(attrs)
{
}

Block::Block()
{
}

bool Block::operator==(const Block& b2) const {
    return false;
}

ostream& operator<<(ostream& os, const Block& s) {
    os << "Block {";
    os << " selector_list: " << s.selector_list;
    os << " attribute_list: " << s.attribute_list;
    os << " }";

    return os;
}

ostream& operator<<(ostream& os, const List<Block>& l) {
    os << "BlockList [ ";
    for (int i = 0; i < l.getLength(); i++) {
        os << l[i] << " ";
    }
    os << "]";
    return os;
}

bool parse_css(List<Block>& block_list)
{
    MyString buffer(2000);
    int c;
    List<Attribute> current_attributes;
    List<Selector> selector_list;
    while ((c = getchar()) != EOF) {
        // Beginning of a block, so the contents of the buffer is a selector
        if (c == '{') {
            selector_list = List<Selector>::parse(buffer);
            buffer = "";
        } else if (c == ';') { // Property
            // End of a property or a whole block, so the contents of the buffer is a property
            if (buffer == "") {
                // Skip empty parameter
                continue;
            }
            Attribute attribute = Attribute::parse(buffer);
            current_attributes.add(move(attribute));
            buffer = "";
        } else if (c == '}') {
            // End of a whole block, so the contents of the buffer is a remaining property without a semicolon (if any)
            if (!(buffer == "")) {
                Attribute attribute = Attribute::parse(buffer);
                current_attributes.add(move(attribute));
            }
            block_list.add(Block(selector_list, current_attributes));
            current_attributes = List<Attribute>();
            selector_list = List<Selector>();
            buffer = "";
        } else {
            if (c != '\n' && c > 31) {
                if (c == '?' && getchar() == '?' && getchar() == '?' && getchar() == '?') {
                    // Captured four '?', assuming end of css parsing and beginning of command section
                    return true;
                }
                buffer += (char) c;
            }
        }
    }
    return false; // Got EOF
}