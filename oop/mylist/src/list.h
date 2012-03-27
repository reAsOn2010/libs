#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <string>

using namespace std;

#define DefaultListSize  5000

//建立纯虚基类，用于被继承
template<typename Elem>
class List
{
    public:
        virtual void make_empty() = 0;//使线性表为空
        virtual int length() = 0;//返回线性表长度
        virtual Elem get(int) = 0;//从0计数，返回第i个
        virtual Elem prev(int) = 0;//返回第i个的前一个
        virtual Elem next(int) = 0;//返回第i个的后一个
        virtual int locate(Elem) = 0;//定位element
        virtual void insert(int, Elem) = 0;//在第i个位置插入element, 原本在i处及其以后的都向后移一位
        virtual void append(Elem) = 0;//插入线性表尾
        virtual void remove(int) = 0;//移除第i个，之后的补上
        virtual bool is_empty() = 0;//察看是否为空
        virtual void clear() = 0;//类似make_empty()
        virtual void traverse() = 0;//遍历，调用到标准输出
};

#endif
