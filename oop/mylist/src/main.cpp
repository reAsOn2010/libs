#include "alist.h"
#include "llist.h"
#include "stack_alist.h"
#include "stack_llist.h"

int main(){

    /*---------------用于测试两个链表---------------
    AList <int> l;
    //LList <int> l;

    l.insert(0,100);
    l.append(12);
    l.insert(1,18);
    l.append(25);
    l.traverse();
    l.append(33);
    l.remove(2);
    l.traverse();
    cout << "length = " <<l.length() << endl;
    cout << "get = " << l.get(1) << endl;
    cout << "prev = " << l.prev(1) << endl;
    cout << "next = " << l.next(1) << endl;
    cout << "locate(18) = " << l.locate(18) << endl;
    l.remove(l.locate(12));
    l.remove(l.locate(18));
    l.traverse();
    cout << "is_empty = " << l.is_empty() << endl;
    l.clear();
    cout << "is_empty = " << l.is_empty() << endl;
    l.traverse();
    ---------------结束---------------*/

    int temp;
    StackAlist <int> s;
    //StackLlist <int> s;
    s.push(100);
    s.push(20);
    temp = s.pop();
    cout << temp << endl;
    s.push(15);
    temp = s.pop();
    cout << temp << endl;
    s.push(123);
    s.push(4556);
    temp = s.pop();
    cout << temp << endl;
    temp = s.pop();
    cout << temp << endl;
    temp = s.pop();
    cout << temp << endl;
    temp = s.pop();
    cout << temp << endl;

    return 0;
}
