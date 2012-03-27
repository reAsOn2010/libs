// Class String

#include<iostream>
#include<cstring>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

class String{
    //对<<和>>进行重载
    friend ostream& operator<< (ostream&, String&);
    friend istream& operator>> (istream&, String&);
    public:
        String(const char* str=NULL);//无参数构造与c风格字符串构造
        String(const String &s);//复制构造
        //String& operator=(const String &s);//赋值操作
        char& operator[](unsigned int);//下标访问
        bool operator < (const String &str_right) const;//对小于号重载，用于排序
        String substr(size_t begin, size_t length) const;//获得子串
        bool hassubstr(const char* str) const;//判断是否包含子串
        const char* c_str()const{return str_data;};//获得c风格字符串
        size_t length()const{return strlen(str_data);};//获得字符串长度
        int compare_to(const String str) const;//字符串比较
        ~String(void){delete[] str_data;}//析构函数
    private:
        char *str_data;
};

inline String::String(const char *str){
    if(!str){
        str_data = NULL;//若参数为空，即无参数构造，则使字符串指针指向空
    }else{
        str_data = new char[strlen(str) + 1];//建立新的字符数组用于存储字符串
        strcpy(str_data, str);
    }
}

inline String::String(const String &s){
    if(!s.str_data){
        str_data = NULL;//若用于复制的对象string本身为空，则使字符串指针指向空
    }else{
        str_data = new char[strlen(s.str_data) + 1];
        strcpy(str_data, s.str_data);
    }
}
/*
inline String& String::operator=(const String &s){
    if(this != &s)
    {
        delete[] str_data;//防止内存泄露
        if(!s.str_data)
            str_data = NULL;
        else{
            str_data = new char[strlen(s.str_data) + 1];
            strcpy(str_data, s.str_data);
        }
    }
    return *this;
}
*/
inline char& String::operator[](unsigned int index){
    if(index >= 0 && index <= strlen(str_data))//确保下标在范围之内
        return str_data[index];
}

inline bool String::operator<(const String &str_right) const{
    return this -> compare_to(str_right) < 0? true: false;
}

inline String String::substr(size_t begin, size_t length) const{
    String sub;
    sub.str_data = new char[length + 1];
    strncpy(sub.str_data, str_data + begin, length);//使用c的strncpy拷贝length个字符
    sub.str_data[length + 1] = '\0';//确保以'\0'结尾
    return sub;
}

inline bool String::hassubstr(const char* str) const{
    int i, j;
    String sub;
    bool ans = false;
    for(i = 0; i <= strlen(str_data); i++){
        for(j = 0; j <= strlen(str_data) - i; j++){//枚举所有字串
            sub = this->substr(i,j);
            if(strcmp(sub.str_data, str) == 0)//若存在与str相同的字串，则为真
                ans = true;
        }
    }
    return ans;
}

inline int String::compare_to(const String str) const{
    if(strcmp(this -> str_data, str.str_data) < 0)//对strcmp函数的一个封装，只返回-1，0，1而不是相差的大小
        return -1;
    else if(strcmp(this  -> str_data, str.str_data) == 0)
        return 0;
    else
        return 1;
}

ostream& operator<<(ostream &os, String &str){
    os<<str.str_data;
    return os;
}

istream& operator>>(istream &is, String &str){
    str.str_data = new char [100005];//新建一个足够大的字符数组用于保存输入
    is >> str.str_data;
    return is;
}

int main(){
    ifstream fin("letters.txt");//测试数据在"letters.txt"中
    vector<String> v;
    String temp;
    cout << "The original file: " << endl;
    while(fin >> temp){
        if(temp.hassubstr("apple"))
            v.push_back(temp);
        cout << temp << endl;
    }
    cout << endl;
    sort(v.begin(), v.end());//对在v向量中的字符串排序
    cout << "Strings that has substring 'apple' and sorted: " <<endl;
    for(int i = 0; i < v.size(); i++){//遍历输出
        cout << v[i] << endl;
    }
    cout << "Total: " << v.size() << endl;

    //用于测试其他函数
    //String str1="Hello,";
    //String str2="World!";
    //String str=str1.substr(2,10);
    //bool b1 = str1.hassubstr("llo");
    //bool b2 = str1.hassubstr("orl");
    //int cmp1 = str1.compare_to(str2);
    //int cmp2 = str2.compare_to(str1);
    //int cmp3 = str2.compare_to(str2);
    //cout<<cmp1<<cmp2<<cmp3<<endl;
    //cout<<b1<<' '<<b2<<endl;
    //cout<<str<<endl;
    //cout<<str1<<' '<<str2<<'\n'<<str1.length()<<endl;
    return 0;
}
