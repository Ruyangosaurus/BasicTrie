#ifndef BASIC_TRIE_DEFINED
#define BASIC_TRIE_DEFINED
#include <string>
#include <exception>
#include <stdexcept>


/// @tparam CharT - the strings' character type; in Trie, char.
/// @brief BasicTrie - a Trie implementation that allows insertion, search and deletion in O(str.size())
template<class CharT>
class BasicTrie{
    struct Node{
        Node* m_parent, * m_next, * m_kid;
        std::basic_string<CharT> m_key;

        Node (const std::basic_string<CharT>& data) : m_parent(nullptr), m_next(nullptr),
                                                         m_kid(nullptr), m_key(data){}
        Node (std::basic_string<CharT>&& data) : m_parent(nullptr), m_next(nullptr),
                                                         m_kid(nullptr), m_key(std::move(data)){}
        Node (const Node& other) : m_parent(nullptr), m_next(nullptr),                             // copies a node
                                                         m_kid(nullptr), m_key(other.data){} 
        Node (Node&& other) : m_parent(nullptr), m_next(nullptr),                                  // moves responsibility over another node's data and subnodes
                                                         m_kid(std::move(other.m_kid)), m_key(std::move(other.data)){}
    };

    Node m_root;

    /* AuxSearch - used by find, contains and erase */
    Node* aux_search(const std::basic_string<CharT>& key) const{
        Node* cursor = m_root.m_kid;
        if (cursor == nullptr){
            return cursor;
        }
        for (auto i = key.cbegin(); i != key.cend(); ++i){
            auto insertee = std::basic_string<CharT>(1,*i);
            while (cursor != nullptr && cursor->m_key < insertee){
                cursor = cursor->m_next;
            }
            if (cursor != nullptr && cursor->m_key == insertee){
                cursor = cursor->m_kid;
            }
            else{
                return nullptr;
            }
        }
        while (cursor != nullptr && cursor->m_key != std::basic_string<CharT>()){
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
                auto temp = new Node (other_cursor->m_kid->m_key);
                cursor->m_kid = temp;
                temp->m_parent = cursor;
                other_cursor = other_cursor->m_kid;
                cursor = cursor->m_kid;
            }
            else if (other_cursor->m_next != nullptr){ // if we can go sidewards
                auto temp = new Node (other_cursor->m_next->m_key);
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
    BasicTrie() : m_root(std::basic_string<CharT>()) {}
    /* Copy constructor - copies a Trie */
    BasicTrie(const BasicTrie& other) : m_root(std::basic_string<CharT>()) {
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
    bool insert(const std::basic_string<CharT>& key, const std::basic_string<CharT>& data) {
        if (key == std::basic_string<CharT>()){
            return false;
        }
        Node* parent = &m_root, * cursor;           // starting recursion with the root
        for (auto i = key.cbegin(); i != key.cend(); ++i){
            auto insertee = std::basic_string<CharT>(1,*i);
            if (parent->m_kid == nullptr){         // if current has no children, build downwards
                cursor = new Node (insertee);
                cursor->m_parent = parent;
                parent->m_kid = cursor;
                parent = cursor;
            }
            else{                                  // else, search where in its children list we should go down
                auto temp = parent->m_kid, temp_2 = temp;
                while (temp != nullptr && temp->m_key < insertee){
                    temp_2 = temp;
                    temp = temp->m_next;
                }
                if (temp != nullptr && temp->m_key == insertee){
                    parent = temp;
                }
                else{
                    cursor = new Node (insertee);
                    cursor->m_parent = parent;
                    if (temp == parent->m_kid && temp->m_key > insertee){
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
        cursor = new Node (std::basic_string<CharT>());                   // add the data enqueuement at the end
        parent->m_kid = cursor;
        cursor->m_parent = parent;
        parent = cursor;
        cursor = new Node (data);                   // add the data enqueuement at the end
        parent->m_kid = cursor;
        cursor->m_parent = parent;
        return true;
    }
    /* Find - allows search of strings in trie by their associated key
      throws std::out_of_range when the key isn't found in the trie */
    std::basic_string<CharT>& find (const std::basic_string<CharT>& key) const{
        Node* cursor = aux_search(key);
        if (!cursor){
            throw std::out_of_range ("Trie does not contain such a key: " + key);
        }
        return cursor->m_key;
    }
    /* Contains - allows checking if an item exists in the trie */
    bool contains (const std::basic_string<CharT>& key) const{
        return (aux_search(key) != nullptr);
    }
    /* Erase - allows deletion of strings from the Trie by their keys */
    bool erase(const std::basic_string<CharT>& key) {
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
                if (cursor == &m_root){    // if all left to delete is the root, delete it
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

using Trie = BasicTrie<char>;
using wTrie = BasicTrie<wchar_t>;
using u16Trie = BasicTrie<char16_t>;
using u32Trie = BasicTrie<char32_t>;

#endif // BASIC_TRIE_DEFINED