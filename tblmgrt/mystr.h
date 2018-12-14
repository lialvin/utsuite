#pragma once
#include <iostream>  
using namespace std;
class MyString
{
public:
	MyString(const char *str = NULL);// 普通构造函数
	MyString(const MyString &other);    // 拷贝构造函数
	MyString(  MyString &&other);    // 移动构造函数
	~MyString(void);    // 析构函数
	MyString&  operator=(MyString&& other);
	MyString & operator = (const MyString &other); // 赋值函数
	friend ostream & operator<< (ostream &os,const MyString &st)
	{
		os << st.m_data << endl;
		return os;
	}

	  
private:
	char *m_data;// 用于保存字符串
};

