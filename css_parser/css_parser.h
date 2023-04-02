#include "list.h"

class Block {
    
    public:
        List<Selector> selector_list;
        List<Attribute> attribute_list;
        Block();
        bool operator==(const Block& b2) const;
        Block(List<Selector> sels, List<Attribute> attrs);
    friend ostream& operator<<(ostream& os, const Block& s);
};

bool parse_css(List<Block>& block_list);