#ifndef BIT_WISE_TRIE_DEFINED
#define BIT_WISE_TRIE_DEFINED

#include <utility>
#include <cstdint>
#include <cstring>
#include <climits>

template<class Mapped>
class BitwiseTrie{
public:
    using key_type = std::string;
    using mapped_type = Mapped;
    using value_type = std::pair<key_type, mapped_type>;
    struct NodeBase{
        NodeBase* m_parent;
        NodeBase* m_subnodes [2];

        NodeBase(){
            m_parent = nullptr;
            m_subnodes[0]  = nullptr;
            m_subnodes[1]  = nullptr;
        }
        virtual ~NodeBase() = default;
    };
    struct Node : public NodeBase{
        NodeBase* m_delim;
        Node() : NodeBase(), m_delim(nullptr) {}
    };
    struct ValueNode : public NodeBase{
        value_type m_data;
        ValueNode(const value_type& data) : NodeBase(), m_data(data) {}
    };

    Node m_root;
    BitwiseTrie() {}
    ~BitwiseTrie(){
        Node* cursor = &m_root;
        while (true){
            if (cursor->m_subnodes[0] != nullptr){
                cursor = reinterpret_cast<Node*> (cursor->m_subnodes[0]);
                continue;
            }
            if (cursor->m_subnodes[1] != nullptr){
                cursor = reinterpret_cast<Node*> (cursor->m_subnodes[1]);
                continue;
            }
            if (cursor->m_delim != nullptr){
                delete cursor->m_delim;
                cursor->m_delim = nullptr;
            }
            if (cursor == &m_root){
                break;
            }
            auto temp = cursor;
            cursor = reinterpret_cast<Node*> (cursor->m_parent);
            if (temp == cursor->m_subnodes[0]){
                cursor->m_subnodes[0] = nullptr;
            }
            else{
                cursor->m_subnodes[1] = nullptr;
            }
            delete temp;
        }
    }

    bool insert(const key_type& key, const mapped_type& mapped){
        const char* ptr = key.data();
        Node* cursor = &m_root;
        for (size_t i = 0; i < key.size(); ++i){
            char val = ptr[i];
            for (int j = 0; j < CHAR_BIT ; ++j){
                if (cursor->m_subnodes[val & 1] == nullptr){
                    cursor->m_subnodes[val & 1] = new Node;
                }
                cursor->m_subnodes[val & 1]->m_parent = cursor;
                cursor = reinterpret_cast<Node*> (cursor->m_subnodes[val & 1]);
                val >>= 1;
            }
        }
        if (cursor->m_delim != nullptr){
            return false;
        }
        cursor->m_delim = new ValueNode ({key, mapped});
        cursor->m_delim->m_parent = cursor;
        return true;
    }

    value_type& find (const key_type& key){ /* Warning - SEGFAULTS when the key isn't in the trie. will be fixed when Iterators will be added. */
        const char* ptr = key.data();
        Node* cursor = &m_root;
        for (size_t i = 0; i < key.size(); ++i){
            for (int j = 0; j < CHAR_BIT ; ++j){
                cursor = reinterpret_cast<Node*> (cursor->m_subnodes[(ptr[i]>>j) & 1]);
            }
        }
        return (reinterpret_cast<ValueNode*>(cursor->m_delim))->m_data;
    }
};

#endif //BIT_WISE_TRIE_DEFINED