#ifndef ALIST_H
#define ALIST_H

#include "list.h"

template<typename Elem>
class AList: public List<Elem>
{
private:
    int   max_size;
    int   list_size;
    Elem* list_array;

public:
    AList(int size = DefaultListSize);
    ~AList();

    void make_empty();
    int  length();
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

    AList<Elem>& operator= ( AList<Elem>& t );
    Elem& operator[](int i);
};
//数组类型线性标需要定义表的最大容量
template<typename Element>
AList<Element>::AList(int size)
{
    max_size= size;
    list_size= 0;
    list_array= new Element[max_size];
}

template<typename Element>
AList<Element>::~AList()
{
    delete [] list_array;
}

template<typename Element>
void AList<Element>::make_empty()
{
    list_size = 0;//采用lazy delete方式
}

template<typename Element>
int AList<Element>::length()
{
    return list_size;
}

template<typename Element>
Element AList<Element>::get(int i)
{
    if(i >= 0 && i < list_size)
        return list_array[i];
}

template<typename Element>
Element AList<Element>::prev(int i)
{
    if(i > 0 && i < list_size)
        return list_array[i-1];
}

template<typename Element>
Element AList<Element>::next(int i)
{
    if(i >= 0 && i < list_size - 1)
        return list_array[i+1];
}

template<typename Element>
int AList<Element>::locate(Element elem)
{
    int i;
    for(i = 0; i < list_size; i++)
        if(elem == list_array[i])
            return i;
    return -1;//若在表中未找到该元素，返回-1
}

template<typename Element>
void AList<Element>::insert(int i, Element elem)
{
    int j;
    if(i>=0 && i<=list_size){
        for(j = list_size; j > i; j--){
            list_array[j] = list_array[j-1];//将i及其以后的元素向后移一位
        }
        list_array[i] = elem;//i处赋值为element
        list_size++;
    }
}

template<typename Element>
void AList<Element>::append(Element elem)
{
    if(list_size + 1 <= max_size)
        list_array[list_size++] = elem;//添加在表尾
}

template<typename Element>
void AList<Element>::remove(int i)
{
    int j;
    if(i >= 0 && i < list_size ){
        for(j = i; j < list_size-1; j++){//i处之后的元素全部向前移动1位
            list_array[j] = list_array[j+1];
        }
        list_size--;
    }
}

template<typename Element>
bool AList<Element>::is_empty()
{
    return list_size == 0;
}

template<typename Element>
void AList<Element>::clear()
{
    list_size = 0;
}

template<typename Element>
void AList<Element>::traverse()
{
    //将element从头至尾遍历一边，输出到标准输出流
    int i;
    for(i = 0; i < list_size; i++){
        cout << list_array[i] << endl;
    }
}

#endif
