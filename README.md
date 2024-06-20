An implementation of a search Trie in c++.

Current interface (as of 21.6.2024, 0:25):
BasicTrie<br>
|<br>
├╌╌╌Default Ctor:<br>
|   creates an empty Trie.<br>
|   Complexities: T = O(1), S = O(1)<br>
|<br>
├╌╌╌Copy Ctor:<br>
|   copies the contents of another Trie, without changing the other.<br>
|   Complexities: T = O(sum of string lengths), S = O(sum of string lengths)<br>
|<br>
├╌╌╌Move Ctor:<br>
|   moves contents (and responsibility over them) from another Trie.<br>
|   Complexities: T = O(1), S = O(1)<br>
|<br>
├╌╌╌Copy =tor:<br>
|   copies the contents of another Trie, without changing the other, after deleting one's own content.<br>
|   Complexities: T = O(sum of string lengths), S = O(sum of string lengths)<br>
|   returns self by reference<br>
|<br>
├╌╌╌Move =tor:<br>
|   moves contents (and responsibility over them) from another Trie, after deleting one's own content.<br>
|   Complexities: T = O(1), S = O(1)<br>
|   returns self by reference<br>
|<br>
├╌╌╌Destructor:<br>
|   deletes all contents of the Trie.<br>
|   Complexities: T = O(sum of string lengths), S = O(1)<br>
|<br>
├╌╌╌insert(const std::basic_string<CharT>& key, const std::basic_string<CharT>& data):<br>
|   inserts a new mapped value (data) associated with a key (key), if they weren't already in the Trie.<br>
|   Complexities: T = O(key.size()), S = O(key.size()) (assuming data's allocation is in O(1))<br>
|   returns true if the insertion was successful, false if the key is already found in the Trie<br>
|<br>
├╌╌╌bool erase(const std::basic_string<CharT>& key):<br>
|   deletes a mapped value associated with the input key (key).<br>
|   Complexities: T = O(key.size()), S = O(1)<br>
|   returns true if the deletion was successful, false if the key is not found in the Trie<br>
|<br>
├╌╌╌bool contains (const std::basic_string<CharT>& key):<br>
|   checks whether or not a certain mapped value in the Trie is associated with the input key (key).<br>
|   Complexities: T = O(key.size()), S = O(1)<br>
|   returns true if such an element exists, false otherwise<br>
|<br>
├╌╌╌std::basic_string<CharT>& find (const std::basic_string<CharT>& key):<br>
|   accesses a mapped value associated with the input key (key).<br>
|   Complexities: T = O(key.size()), S = O(1)<br>
|   returns by reference the mapped value if it exists<br>
|   throws std::out_of_range otherwise<br>
|<br>
├╌╌╌void clear():<br>
    deletes all contents of the Trie.<br>
    Complexities: T = O(sum of string lengths), S = O(1)<br>

Planned future additions:
+ forward iterators
+ compatibility with mapped types besides std::basic_string<CharT>
+ void swap()
