#ifndef BASIC_TRIE_DEFINED
#define BASIC_TRIE_DEFINED
#include <string>
#include <exception>
#include <stdexcept>


/// @tparam CharT - the strings' character type; in Trie, char.
/// @brief BasicTrie - a Trie implementation that allows insertion, search and deletion in O(str.size())
template<class CharT, class Val = std::basic_string<CharT>>
class BasicTrie{
public:
    using key_type = std::basic_string<CharT>;
    using mapped_type = Val;
    using value_type = std::pair<key_type, mapped_type>;
private:
    struct Node{
        Node* m_parent, * m_next, * m_kid;
        
        Node () : m_parent(nullptr), m_next(nullptr),                             // copies a node
                                                         m_kid(nullptr){} 
        Node (Node&& other) : m_parent(nullptr), m_next(nullptr),                                  // moves responsibility over another node's subnodes
                                                         m_kid(std::move(other.m_kid)){}
        virtual ~Node(){}
        virtual const key_type& get_key() const =0;
        virtual Node* clone () const =0;
    };

    struct KeyNode : public Node{
        key_type m_key;

        KeyNode (const key_type& data) : Node(), m_key(data){}
        KeyNode (key_type&& data) : Node(), m_key(std::move(data)){}
        KeyNode (const KeyNode& other) : Node(), m_key(other.m_key){} 
        KeyNode (KeyNode&& other) : Node(std::move(other)), m_key(std::move(other.m_key)){}
        const key_type& get_key() const override {return m_key;}
        Node* clone () const override {return new KeyNode(*this);}
    };

    struct ValueNode : public Node{
        value_type m_data;

        ValueNode (const key_type& data, const mapped_type& val) : Node(), m_data(data, val){}
        ValueNode (key_type&& data, mapped_type&& val) : Node(), m_data(std::move(data), std::move(val)){}
        ValueNode (const ValueNode& other) : Node(), m_data(other.m_data){} 
        ValueNode (ValueNode&& other) : Node(std::move(other)), m_data(std::move(other.m_data)){}
        const key_type& get_key() const override {return m_data.first;}
        Node* clone () const override {return new ValueNode(*this);}
    };

    KeyNode m_root;

    /* AuxSearch - used by find, contains and erase */
    Node* aux_search(const key_type& key) const{
        Node* cursor = m_root.m_kid;
        if (cursor == nullptr){
            return cursor;
        }
        for (auto i = key.cbegin(); i != key.cend(); ++i){
            auto insertee = key_type(1,*i);
            while (cursor != nullptr && cursor->get_key() < insertee){
                cursor = cursor->m_next;
            }
            if (cursor != nullptr && cursor->get_key() == insertee){
                cursor = cursor->m_kid;
            }
            else{
                return nullptr;
            }
        }
        while (cursor != nullptr && cursor->get_key() != key_type()){
            cursor = cursor->m_next;
        }
        if (cursor == nullptr){
            return cursor;
        }
        return cursor->m_kid;
    }
    /* AuxClone - used by copy ctor and =tor */
    void aux_clone (const BasicTrie& other){
        Node* cursor = &m_root;
        const Node* other_cursor = &other.m_root;
        bool from_subnode = false;
        while (other_cursor != nullptr){
            if (other_cursor->m_kid != nullptr && !from_subnode){ // if we can go downwards and we hadn't already been there
                Node* temp = other_cursor->m_kid->clone();
                cursor->m_kid = temp;
                temp->m_parent = cursor;
                other_cursor = other_cursor->m_kid;
                cursor = cursor->m_kid;
            }
            else if (other_cursor->m_next != nullptr){ // if we can go sidewards
                Node* temp = other_cursor->m_next->clone();
                cursor->m_next = temp;
                temp->m_parent = cursor->m_parent;
                other_cursor = other_cursor->m_next;
                cursor = cursor->m_next;
                from_subnode = false;
            }
            else{ // go to the parent
                other_cursor = other_cursor->m_parent;
                cursor = cursor->m_parent;
                from_subnode = true;
            }
        }
    }
public:
    /* Default constructor - constructs a new Trie */
    BasicTrie() : m_root(key_type()) {}
    /* Copy constructor - copies a Trie */
    BasicTrie(const BasicTrie& other) : m_root(key_type()) {
        aux_clone (other);
    }
    /* Copy assignment - copies a Trie */
    BasicTrie& operator=(const BasicTrie& other) {
        clear();
        aux_clone (other);
        return *this;
    }
    /* Move constructor - takes all the nodes of another Trie */
    BasicTrie(BasicTrie&& other) : m_root(std::move(other.m_root)) {
        other.m_root.m_kid = nullptr;
    }
    /* Move assignment - takes all the nodes of another Trie */
    BasicTrie& operator=(BasicTrie&& other) {
        clear();
        m_root.m_kid = other.m_root.m_kid;
        other.m_root.m_kid = nullptr;
        return *this;
    }
    /* Destructor - destroys the Trie by traversal of all its nodes */
    ~BasicTrie(){
        clear();
    }
    /* Insert - allows insertion of pairs of strings */
    bool insert(const key_type& key, const mapped_type& data) {
        if (key == key_type()){
            return false;
        }
        Node* parent = &m_root, * cursor;           // starting recursion with the root
        for (auto i = key.cbegin(); i != key.cend(); ++i){
            auto insertee = key_type(1,*i);
            if (parent->m_kid == nullptr){         // if current has no children, build downwards
                cursor = new KeyNode (insertee);
                cursor->m_parent = parent;
                parent->m_kid = cursor;
                parent = cursor;
            }
            else{                                  // else, search where in its children list we should go down
                auto temp = parent->m_kid, temp_2 = temp;
                while (temp != nullptr && temp->get_key() < insertee){
                    temp_2 = temp;
                    temp = temp->m_next;
                }
                if (temp != nullptr && temp->get_key() == insertee){
                    parent = temp;
                }
                else{
                    cursor = new KeyNode (insertee);
                    cursor->m_parent = parent;
                    if (temp == parent->m_kid && temp->get_key() > insertee){
                        cursor->m_next = temp;
                        parent->m_kid = cursor;
                    }
                    else{
                        cursor->m_next = temp_2->m_next;
                        temp_2->m_next = cursor;
                    }
                    parent = cursor;
                }
            }
        }
        cursor = new KeyNode (key_type());                   // add the data enqueuement at the end
        parent->m_kid = cursor;
        cursor->m_parent = parent;
        parent = cursor;
        cursor = new ValueNode ({key, data});                   // add the data enqueuement at the end
        parent->m_kid = cursor;
        cursor->m_parent = parent;
        return true;
    }
    /* Find - allows search of strings in trie by their associated key
      throws std::out_of_range when the key isn't found in the trie */
    value_type& find (const key_type& key) const{
        ValueNode* cursor = reinterpret_cast<ValueNode*> (aux_search(key));
        if (!cursor){
            throw std::out_of_range ("Trie does not contain such a key: " + key);
        }
        return cursor->m_data;
    }
    /* Contains - allows checking if an item exists in the trie */
    bool contains (const key_type& key) const{
        return (aux_search(key) != nullptr);
    }
    /* Erase - allows deletion of strings from the Trie by their keys */
    bool erase(const key_type& key) {
        Node* cursor = aux_search(key);
        if (!cursor){
            return false;
        }
        while (cursor != &m_root && cursor->m_kid == nullptr){
            if (cursor->m_parent->m_kid == cursor){
                cursor->m_parent->m_kid = cursor->m_next;
            }
            else{
                auto temp = cursor->m_parent->m_kid;
                while (temp->m_next != cursor){
                    temp = temp->m_next;
                }
                temp->m_next = cursor->m_next;
            }
            auto deletee = cursor;
            cursor = cursor->m_parent;
            delete deletee;
        }
        return true;
    }
    /* Clear - frees all the stored memory */
    void clear() {
        Node* cursor = &m_root; // initialize recursion with root
        while(true){
            if(cursor->m_kid != nullptr){ // if we can go down, we'll go down
                cursor = cursor->m_kid;
            }
            else{
                if (cursor == &m_root){    // if all left is the root, stop
                    break;
                }
                else{                     // otherwise, delete cursor and go up
                    cursor->m_parent->m_kid = cursor->m_next;
                    auto temp = cursor;
                    cursor = cursor->m_parent;
                    delete temp;
                }
            }
        }
    }
};

template<class Val = std::string>
using Trie = BasicTrie<char, Val>;

template<class Val = std::wstring>
using wTrie = BasicTrie<wchar_t, Val>;

template<class Val = std::u16string>
using u16Trie = BasicTrie<char16_t, Val>;

template<class Val = std::u32string>
using u32Trie = BasicTrie<char32_t, Val>;

#endif // BASIC_TRIE_DEFINED
