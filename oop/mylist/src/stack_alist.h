#ifndef STACK_ALIST_H
#define STACK_ALIST_H

#include "alist.h"
#include <cstdlib>
//利用已有的array list建立堆栈
template<typename Elem>
class StackAlist
{
    private:
        AList<Elem>* l;
    public:
        StackAlist(int size = DefaultListSize);
        ~StackAlist();

        Elem pop();
        void push(Elem elem);
};

template<typename Element>
StackAlist<Element>::StackAlist(int size)
{
    l = new AList<Element>(size);
}

template<typename Element>
StackAlist<Element>::~StackAlist()
{
    delete l;
}

template<typename Element>
Element StackAlist<Element>::pop()
{
    if(!l->is_empty()){
        Element temp;
        temp = l->get(l->length()-1);//得到表尾元素
        l->remove(l->length()-1);//删除表尾元素
        return temp;
    }
    else
        exit(-1);
}

template<typename Element>
void StackAlist<Element>::push(Element elem)
{
    l->append(elem);//添加新元素到表尾
}

#endif
