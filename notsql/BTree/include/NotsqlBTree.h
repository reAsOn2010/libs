/* 
 * filename: BPlusTree.h
 * date: 9.18.2012
 * author: Leon Zhou
 * function: B+ Tree implementation
 */
#ifndef __NOTSQLBTREE__
#define __NOTSQLBTREE__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#define NOTSQL_BTREE_VALUE struct pointer
using namespace std;
template<class T>
class Entry;
template<class T>
class NotsqlBTreeNode;
template<class T>
class NotsqlBTree;

struct pointer {
    int block_num;
    int record_index;
};

const static struct pointer null_pointer = {-1, -1};

// internal nodes: only use key and next
// external nodes: only use key and value
template<class T>
class Entry {
public:
    T key;
    NOTSQL_BTREE_VALUE value;
    NotsqlBTreeNode<T>* next;

    //public
    friend class NotsqlBTree<T>;
    friend class NotsqlBTreeNode<T>;
    Entry() {}
    Entry(T key, NOTSQL_BTREE_VALUE value, NotsqlBTreeNode<T>* next);
    ~Entry() {}
    /*
    ~Entry() {
        if (next != NULL)
            delete[] next;
    }
    */
};

template<class T>
class NotsqlBTreeNode {
public:
    int m;              // number of children
    Entry<T>* children;

    //public
    friend class NotsqlBTree<T>;
    NotsqlBTreeNode(int M, int k); //create a node with k children
    ~NotsqlBTreeNode() {
        if (children != NULL)
            delete[] children;
    }
};

template<class T>
class NotsqlBTree {
public:
    int M;              // max children is M-1
    int N;              // number of key-value pairs in the B-tree
    NotsqlBTreeNode<T>* root;
    int HT;             // height of B tree
    NOTSQL_BTREE_VALUE search(NotsqlBTreeNode<T>* x, T key, int ht);
    NotsqlBTreeNode<T>* insert(NotsqlBTreeNode<T>* h,
            T key,
            NOTSQL_BTREE_VALUE value,
            int ht);
    NotsqlBTreeNode<T>* split(NotsqlBTreeNode<T>* h);
    void deleteChildren(NotsqlBTreeNode<T>* x, int ht);
    NotsqlBTreeNode<T>* searchThatNode(NotsqlBTreeNode<T>* x, T key, int ht);
    NotsqlBTreeNode<T>* getLeftestChild();
    //public
    NotsqlBTree(int M);
    ~NotsqlBTree();

    int size();
    int height();
    NotsqlBTreeNode<T> *getRoot();
    NOTSQL_BTREE_VALUE get(T key);
    void getEqual(T key, vector<NOTSQL_BTREE_VALUE> &output);
    void getGreatThan(T key, vector<NOTSQL_BTREE_VALUE> &output);
    void getLessThan(T key, vector<NOTSQL_BTREE_VALUE> &output);
    void getAllPairs(vector< pair<NOTSQL_BTREE_VALUE, T> > &output);
    void put(T key, NOTSQL_BTREE_VALUE value);
    void del(T key, NOTSQL_BTREE_VALUE value);
    void del(NOTSQL_BTREE_VALUE value);
    string traversal();
    string toString();
    string toString(NotsqlBTreeNode<T>* h, int ht, string indent);
    bool less(T key1, T key2);
    bool eq(T key1, T key2);
    bool more(T key1, T key2);

};

template<class T>
Entry<T>::Entry(T key, NOTSQL_BTREE_VALUE value, NotsqlBTreeNode<T> *next) {
    this->key = key;
    this->value = value;
    this->next = next;
}

template<class T>
NotsqlBTreeNode<T>::NotsqlBTreeNode(int M, int k) {
    m = k;
    children = new Entry<T>[M];
    children[M-1].next = NULL;
}

template<class T>
NotsqlBTree<T>::NotsqlBTree(int M) {
    this->M = M;
    HT = 0;
    root = new NotsqlBTreeNode<T>(M, 0);
}

template<class T>
NotsqlBTree<T>::~NotsqlBTree() {
    if (root != NULL) {
        deleteChildren(root, HT);
    }
}

template<class T>
void NotsqlBTree<T>::deleteChildren(NotsqlBTreeNode<T>* x, int ht) {
    // leaf node
    Entry<T>* tmp = x->children;
    if (ht == 0) {
        delete x;
        return;
    }
    // internal node
    else {
        for (int j = 0; j < x->m; j++) {
            deleteChildren(tmp[j].next, ht-1);
        }
        delete x;
        return;
    }
}

template<class T>
int NotsqlBTree<T>::size() {
    return N;
}

template<class T>
int NotsqlBTree<T>::height() {
    return HT;
}

template<class T>
NotsqlBTreeNode<T> *NotsqlBTree<T>::getRoot() {
    return root;
}

template<class T>
NOTSQL_BTREE_VALUE NotsqlBTree<T>::get(T key) {
    return search(root, key, HT);
}

template<class T>
NOTSQL_BTREE_VALUE NotsqlBTree<T>::search(NotsqlBTreeNode<T>* x,
        T key,
        int ht) {
    Entry<T>* tmp = x->children;

    // leaf node
    if (ht == 0) {
        for (int j = 0; j < x->m; j++) {
            if (eq(key, tmp[j].key))
                return (NOTSQL_BTREE_VALUE)tmp[j].value;
        }
    }
    // internal node
    else {
        for (int j = 0; j < x->m; j++) {
            if (j+1 == x->m || less(key, tmp[j+1].key))
                return search(tmp[j].next, key, ht-1);
        }
    }

    return null_pointer;
}

template<class T>
NotsqlBTreeNode<T>* NotsqlBTree<T>::searchThatNode(NotsqlBTreeNode<T>* x,
        T key,
        int ht) {
    Entry<T>* tmp = x->children;

    // leaf node
    if (ht == 0) {
        return x;
    }
    // internal node
    else {
        for (int j = 0; j < x->m; j++) {
            if (j+1 == x->m || less(key, tmp[j+1].key) || eq(key, tmp[j+1].key))
                return searchThatNode(tmp[j].next, key, ht-1);
        }
    }
    return NULL;
}

template<class T>
NotsqlBTreeNode<T>* NotsqlBTree<T>::getLeftestChild() {
    NotsqlBTreeNode<T>* x = root;
    int ht = HT;
    while (ht > 0) {
        x = x->children[0].next;
        ht--;
    }
    return x;
}

template<class T>
void NotsqlBTree<T>::put(T key, NOTSQL_BTREE_VALUE value) {
    NotsqlBTreeNode<T>* u = insert(root, key, value, HT);
    N++;
    if (u == NULL) return;

    // need to split
    NotsqlBTreeNode<T>* t = new NotsqlBTreeNode<T>(M, 2);
    t->children[0] = Entry<T>(root->children[0].key, null_pointer, root);
    t->children[1] = Entry<T>(u->children[0].key, null_pointer, u);
    root = t;
    HT++;
}

template<class T>
NotsqlBTreeNode<T>* NotsqlBTree<T>::insert(NotsqlBTreeNode<T>* h,
        T key,
        NOTSQL_BTREE_VALUE value,
        int ht) {
    int j;
    Entry<T> t = Entry<T>(key, value, NULL);

    // external node
    if (ht == 0) {
        for (j = 0; j < h->m; j++) {
            if (less(key, h->children[j].key))
                break;
        }
    }


    // internal node
    else {
        for (j = 0; j < h->m; j++) {
            if((j+1 == h->m) || less(key, (h->children[j+1].key))) {
                NotsqlBTreeNode<T>* u = insert(h->children[j++].next, key, value, ht-1);
                if (u == NULL) return NULL;
                t.key = u->children[0].key;
                t.next = u;
                break;
            }
        }
    }
    // restore the next pointer
    NotsqlBTreeNode<T>* restore = h->children[M-1].next;
    // or it'll be turn to 0 by the following code;
    for (int i = h->m; i > j; i--)
        h->children[i] = h->children[i-1];
    h->children[j] = t;
    h->m++;
    if (h->m < M) 
        return NULL;
    else if(ht == 0) {
        // store back the pointer when is leaf node
        h->children[M-1].next = restore;
        NotsqlBTreeNode<T>* new_node = split(h);
        new_node->children[M-1].next = h->children[M-1].next;
        h->children[M-1].next = new_node;
        return new_node;
    }
    else 
        return split(h);
}

template<class T>
void NotsqlBTree<T>::del(NOTSQL_BTREE_VALUE pointer) {
    NotsqlBTreeNode<T>* x = getLeftestChild();
    while(1) {
        Entry<T>* tmp = x->children;
        int j;
        for (j = 0; j < x->m; j++) {
            if (tmp[j].value.block_num == pointer.block_num &&
                    tmp[j].value.record_index == pointer.record_index) {
                for (int i = j; i < x->m; i++) {
                    tmp[i].key = tmp[i+1].key;
                    tmp[i].value = tmp[i+1].value;
                }
                x->m--;
                N--;
                return;
            }
        }
        if (tmp[M-1].next == NULL)
            break;
        x = tmp[M-1].next;
    }
}
template<class T>
void NotsqlBTree<T>::del(T key, NOTSQL_BTREE_VALUE pointer) {
    NotsqlBTreeNode<T>* x = searchThatNode(root, key, HT);
    while(1) {
        Entry<T>* tmp = x->children;
        int j;
        for (j = 0; j < x->m; j++) {
            if (eq(key, tmp[j].key) &&
                    tmp[j].value.block_num == pointer.block_num &&
                    tmp[j].value.record_index == pointer.record_index) {
                for(int i = j; i < x->m; i++) {
                    tmp[i].key = tmp[i+1].key;
                    tmp[i].value = tmp[i+1].value;
                }
                x->m--;
                N--;
            }
            if (less(key, tmp[j].key)) {
                break;
            }
        }
        if (tmp[M-1].next == NULL || j < x->m)
            break;
        x = tmp[M-1].next;
    }
}

template<class T>
void NotsqlBTree<T>::getEqual(T key, vector<NOTSQL_BTREE_VALUE> &output) {
    NotsqlBTreeNode<T>* x = searchThatNode(root, key, HT);
    while(1) {
        Entry<T>* tmp = x->children;
        int j;
        for (j = 0; j < x->m; j++) {
            if (eq(key, tmp[j].key))
                output.push_back(tmp[j].value);
            if (less(key, tmp[j].key)) // already find all equal ones
                break;
        }
        if(tmp[M-1].next == NULL || j < x->m) //if the last node or already not equal with key break!
            break;
        x = tmp[M-1].next;
    }
}

template<class T>
void NotsqlBTree<T>::getGreatThan(T key, vector<NOTSQL_BTREE_VALUE> &output) {
    NotsqlBTreeNode<T>* x = searchThatNode(root, key, HT);
    while(1) {
        Entry<T>* tmp = x->children;
        int j;
        for (j = 0; j < x->m; j++) {
            if (less(key, tmp[j].key))
                output.push_back(tmp[j].value);
        }
        if (tmp[M-1].next == NULL)
            break;
        x = tmp[M-1].next;
    }
}

template<class T>
void NotsqlBTree<T>::getLessThan(T key, vector<NOTSQL_BTREE_VALUE> &output) {
    NotsqlBTreeNode<T>* x = searchThatNode(root, key, HT);
    NotsqlBTreeNode<T>* leftest = getLeftestChild();
    while(1) {
        Entry<T>* tmp = leftest->children;
        int j;
        for (j = 0; j < x->m; j++) {
            if (more(key, tmp[j].key))
                output.push_back(tmp[j].value);
        }
        if (tmp[M-1].next == NULL || j < x->m)
            break;
        leftest = tmp[M-1].next;
    }
}

template<class T>
void NotsqlBTree<T>::getAllPairs(vector< pair<NOTSQL_BTREE_VALUE, T> > &output) {
    
    output.clear();
    NotsqlBTreeNode<T> * x = getLeftestChild();

    while(1) {
        Entry<T>* tmp = x->children;
        int j;
        for (j = 0; j < x->m; j++) {
            pair<NOTSQL_BTREE_VALUE, T> tmp_pair (tmp[j].value, tmp[j].key);
            output.push_back(tmp_pair);
        }
        if (tmp[M-1].next == NULL)
            break;
        x = tmp[M-1].next;
    }
}

template<class T>
NotsqlBTreeNode<T>* NotsqlBTree<T>::split(NotsqlBTreeNode<T>* h) {

    // TODO: maybe just divided by 2 is not correct!!
    NotsqlBTreeNode<T>* t = new NotsqlBTreeNode<T>(M, M/2);
    h->m = M/2;
    for (int j = 0; j < M/2; j++)
        t->children[j] = h->children[M/2+j];
    return t;
}

template<class T>
bool NotsqlBTree<T>::less(T key1, T key2) {
    return key1 < key2;
}

template<class T>
bool NotsqlBTree<T>::eq(T key1, T key2) {
    return key1 == key2;
}

template<class T>
bool NotsqlBTree<T>::more(T key1, T key2) {
    return key1 > key2;
}

template<class T>
string NotsqlBTree<T>::traversal() {
    NotsqlBTreeNode<T>* x = getLeftestChild();
    stringstream ret;
    while (x->children[M-1].next != NULL) {
        for (int i = 0; i < x->m; i++) {
            ret << x->children[i].key
                << " "
                << x->children[i].value.block_num
                << " "
                << x->children[i].value.record_index
                << "\n";
        }
        x = x->children[M-1].next;
    }
    for (int i = 0; i < x->m; i++) {
        ret << x->children[i].key
            << " "
            << x->children[i].value.block_num
            << " "
            << x->children[i].value.record_index
            << "\n";
    }
    return ret.str();
}

template<class T>
string NotsqlBTree<T>::toString() {
    return toString(root, HT, "") + "\n";
}

template<class T>
string NotsqlBTree<T>::toString(NotsqlBTreeNode<T>* h, int ht, string indent) {
    stringstream s;
    Entry<T>* children = h->children;

    if (ht == 0) {
        for (int j = 0; j < h->m; j++) {
            s << indent
                << children[j].key
                << " "
                << children[j].value.block_num
                <<"-"
                << children[j].value.record_index
                << endl;
            //s += indent + children[j].key + " " + children[j].value + "\n";
        }
    }
    else {
        for (int j = 0; j < h->m; j++) {
            if (j > 0) s << indent << "(" << children[j].key << ")\n";
            s << toString(children[j].next, ht-1, indent + "    ");
        }
    }
    return s.str();
}

#endif
