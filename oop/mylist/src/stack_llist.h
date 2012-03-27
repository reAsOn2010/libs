#ifndef STACK_LLIST_H
#define STACK_LLIST_H

#include "llist.h"
#include <cstdlib>
//因llist与alist的接口完全相同，两个stack仅有private元素不同，其余都相同
template<typename Elem>
class StackLlist
{
    private:
        LList<Elem>* l;
    public:
        StackLlist();
        ~StackLlist();

        Elem pop();
        void push(Elem);
};

template<typename Element>
StackLlist<Element>::StackLlist()
{
    l = new LList<Element>();
}

template<typename Element>
StackLlist<Element>::~StackLlist()
{
    delete l;
}

template<typename Element>
Element StackLlist<Element>::pop()
{
    if(!l->is_empty()){
        Element temp;
        temp = l->get(l->length()-1);
        l->remove(l->length()-1);
        return temp;
    }
    else
        exit(-1);
}

template<typename Element>
void StackLlist<Element>::push(Element elem)
{
    l->append(elem);
}

#endif
