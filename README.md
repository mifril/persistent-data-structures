# Persistent data structures #

## Base classes ##

* PersistentVector<T> 
* PersistentList<T>
* PersistentMap<K, V, Comparator>

## Additional classes ##

* PersistentAVLTree<K, V, Comparator>
* VersionTree. insertAfter(p, q) complexity: O(log k), order(p, q) complexity: O(1)

## algoritms ##

Let n is number of elements in data structure, k - number of versions.

* PersistentVector: fat-node realisation,  read: O(log k), write: O(log k), memory: O(kn).
* PersistentList: Path Copying algorithm, read: O(1), insert to front: O(1), insert to random place: O(n), memory: O(kn).
* PersistentMap: based on PersistentAVLTree, implemented using Path Copying algorithm, read/write: O(log n), memory: O(kn).
