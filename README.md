An implementation of a search Trie in c++.

Current interface (as of 21.6.2024, 0:25):
```txt
BasicTrie
|
├╌╌╌Default Ctor:
|   creates an empty Trie.
|   Complexities: T = O(1), S = O(1)
|
├╌╌╌Copy Ctor:
|   copies the contents of another Trie, without changing the other.
|   Complexities: T = O(sum of string lengths), S = O(sum of string lengths)
|
├╌╌╌Move Ctor:
|   moves contents (and responsibility over them) from another Trie.
|   Complexities: T = O(1), S = O(1)
|
├╌╌╌Copy =tor:
|   copies the contents of another Trie, without changing the other, after deleting one's own content.
|   Complexities: T = O(sum of string lengths), S = O(sum of string lengths)
|   returns self by reference
|
├╌╌╌Move =tor:
|   moves contents (and responsibility over them) from another Trie, after deleting one's own content.
|   Complexities: T = O(1), S = O(1)
|   returns self by reference
|
├╌╌╌Destructor:
|   deletes all contents of the Trie.
|   Complexities: T = O(sum of string lengths), S = O(1)
|
├╌╌╌insert(const key_type& key, const mapped_type& data):
|   inserts a new mapped value (data) associated with a key (key), if they weren't already in the Trie.
|   Complexities: T = O(key.size()), S = O(key.size()) (assuming data's allocation is in O(1))
|   returns true if the insertion was successful, false if the key is already found in the Trie
|
├╌╌╌bool erase(const key_type& key):
|   deletes a mapped value associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns true if the deletion was successful, false if the key is not found in the Trie
|
├╌╌╌bool contains (const key_type& key):
|   checks whether or not a certain mapped value in the Trie is associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns true if such an element exists, false otherwise
|
├╌╌╌value_type& find (key_type& key):
|   accesses a mapped value associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns by reference a pair of value and key if it exists
|   throws std::out_of_range otherwise
|
├╌╌╌void clear():
    deletes all contents of the Trie.
    Complexities: T = O(sum of string lengths), S = O(1)
```
Planned future additions:
+ forward iterators
+ void swap()
