#ifndef LLIST_H
#define LLIST_H

#include "list.h"

//定义节点类，内含本节点的element，以及指向下一个节点的指针
template<typename Elem>
class Node
{
    public:
        Elem element;
        Node* next;

        Node(Elem, Node*);
        Node(Elem);
        Node();
};
//传入element，和next指针的构造
template<typename Elem>
Node<Elem>::Node(Elem elem, Node* np)
{
    element = elem;
    next = np;
}
//仅仅传入element的构造，next赋值为NULL
template<typename Elem>
Node<Elem>::Node(Elem elem)
{
    element = elem;
    next = NULL;
}
//无参数构造
template<typename Elem>
Node<Elem>::Node(){
    next = NULL;
}
//link-list类
template<typename Elem>
class LList: public List<Elem>
{
    private:
        Node<Elem>* head;//头指针
        Node<Elem>* tail;//为了方便append操作，保存了链表尾
        int list_size;
    public:
        LList();
        ~LList();

        void make_empty();
        int length();
        Elem get(int);
        Elem prev(int);
        Elem next(int);
        int locate(Elem);
        void insert(int, Elem);
        void append(Elem);
        void remove(int);
        bool is_empty();
        void clear();
        void traverse();
};
//无参数构造
template<typename Element>
LList<Element>::LList()
{
    head = new Node<Element>();//新建头指针
    tail = NULL;
    list_size = 0;
}

template<typename Element>
LList<Element>::~LList()
{
    Node<Element>* temp;
    while(head != NULL){//为保证不发生内存泄露，链式删除整个链表
        temp = head;
        head = head->next;
        delete temp;
    }
}

template<typename Element>
void LList<Element>::make_empty()
{
    Node<Element> *temp;
    Node<Element> *t_head = head->next;
    while(t_head != NULL){//保留头指针，链式删除之后的节点
        temp = t_head;
        t_head = t_head->next;
        delete temp;
    }
    head->next = NULL;
    tail = NULL;
    list_size = 0;
}

template<typename Element>
int LList<Element>::length()
{
    return list_size;
}

template<typename Element>
Element LList<Element>::get(int i)
{
    int j;
    Node<Element> *temp = head;
    if(i >= 0 && i < list_size)
        for(j = 0; j <= i; j++)//从头指针开始向后遍历到第i个
            temp = temp->next;
    return temp->element;
}

template<typename Element>
Element LList<Element>::prev(int i)
{
    int j;
    Node<Element> *temp = head;
    if(i > 0 && i <list_size)
        for(j = 0; j < i; j++)//从头指针开始向后遍历到第i-1个

            temp = temp->next;
    return temp->element;
}

template<typename Element>
Element LList<Element>::next(int i)
{
    int j;
    Node<Element> *temp = head;
    if(i >= 0 && i < list_size -1)
        for(j = 0; j <= i + 1; j++)//从头指针开始向后遍历到第i+1个
            temp = temp->next;
    return temp->element;
}

template<typename Element>
int LList<Element>::locate(Element elem)
{
    int i;
    Node<Element> *temp = head;
    for(i = 0; i < list_size; i++){//遍历链表，若发现该元素，返回位置
        temp = temp->next;
        if(elem == temp->element)
            return i;
    }
    return -1;//否则返回-1
}

template<typename Element>
void LList<Element>::insert(int i, Element elem)
{
    int j;
    Node<Element> *prev, *current, *temp = head, *new_node;
    //链表的插入操作
    if(i>=0 && i<=list_size){
        for(j = 0; j < i; j++)
            temp = temp->next;
        prev = temp;
        current = temp->next;
        new_node = new Node<Element>(elem, current);
        prev->next = new_node;

        if(tail == NULL)
            tail = new_node;
        list_size++;
    }
}

template<typename Element>
void LList<Element>::append(Element elem)
{
    Node<Element> *new_node;
    new_node = new Node<Element>(elem);
    if(tail == NULL)//若链表为空，则类似插入与i=0处
        this->insert(0, elem);
    else{
        tail->next = new_node;//否则更新tail指针
        tail = new_node;
        list_size++;
    }
}

template<typename Element>
void LList<Element>::remove(int i)
{
    //链表的删除节点操作
    if(i>=0 && i < list_size){
        int j;
        Node<Element> *temp = head;
        for(j = 0; j < i; j++)
            temp = temp->next;
        Node<Element> *prev = temp;
        Node<Element> *current = temp->next;
        prev->next = current->next;

        delete current;
        list_size--;
    }
}

template<typename Element>
bool LList<Element>::is_empty()
{
    return list_size == 0;
}

template<typename Element>
void LList<Element>::clear()
{
    this->make_empty();
}

template<typename Element>
void LList<Element>::traverse()
{
    //将element从头至尾遍历一边，输出到标准输出流
    int i;
    Node<Element> *temp = head;
    for(i = 0; i < list_size; i++){
        temp = temp->next;
        cout << temp->element << endl;
    }
}

#endif
