An implementation of a Trie as a left-child, right-sibling binary tree in c++.

Current interface (as of 21.6.2024, 19:46):
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
├╌╌╌Iterator
|   |
|   ├╌╌╌Default Ctor:
|   |   creates an undereferenceable Iterator to end()
|   |   Complexities: T = O(1), S = O(1)
|   |
|   ├╌╌╌Copy Ctor & Copy =tor:
|   |   copies an Iterator
|   |   Complexities: T = O(1), S = O(1)
|   |
|   ├╌╌╌Move Ctor & Move =tor:
|   |   moves an Iterator
|   |   Complexities: T = O(1), S = O(1)
|   |
|   ├╌╌╌Swap(Iterator& itr)
|   |   Swaps contents between two iterators
|   |   Complexities: T = O(1), S = O(1)
|   |
|   ├╌╌╌operator* () & operator-> ()
|   |   Dereferences the iterator
|   |   Complexities: T = O(1), S = O(1)
|   |
|   ├╌╌╌operator++
|       Increments the iterator
|       Complexities: T = O(length of longest string in Trie), S = O(1)
|
├╌╌╌begin():
|   creates an Iterator to the first element of the Trie
|   Complexities: T = O(length of longest string in Trie}), S = O(1)
|
├╌╌╌end():
|   creates an undereferenceable Iterator to the one-past-last element of the Trie
|   Complexities: T = O(1), S = O(1)
|
├╌╌╌insert(const key_type& key, const mapped_type& data):
|   inserts a new mapped value (data) associated with a key (key), if they weren't already in the Trie.
|   Complexities: T = O(key.size()), S = O(key.size()) (assuming data's allocation is in O(1))
|   returns true if the insertion was successful, false if the key is already found in the Trie; in addition, returns an iterator to the inserted data.
|
├╌╌╌erase(const key_type& key):
|   deletes a mapped value associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns true if the deletion was successful, false if the key is not found in the Trie
|
├╌╌╌contains (const key_type& key):
|   checks whether or not a certain mapped value in the Trie is associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns true if such an element exists, false otherwise
|
├╌╌╌find (key_type& key):
|   accesses a mapped value associated with the input key (key).
|   Complexities: T = O(key.size()), S = O(1)
|   returns by iterator a pair of value and key if it exists, end() otherwise
|
├╌╌╌void clear():
    deletes all contents of the Trie.
    Complexities: T = O(sum of string lengths), S = O(1)
```
Planned future additions:
+ backwards iterators
+ void swap()
