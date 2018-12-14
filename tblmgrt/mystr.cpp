#include <stdlib.h>
#include <string.h>
#include "mystr.h"

//请编写MyString的上述4个函数。
//普通构造函数
MyString::MyString(const char *str)
{

	if (str == NULL)
	{
		m_data = new char[1]; // 得分点：对空字符串自动申请存放结束标志'\0'的//加分点：对m_data加NULL 判断
		*m_data = '\0';
	}
	else
	{
	        size_t length = strlen(str);
		m_data = new char[length + 1]; // 若能加 NULL 判断则更好
		strcpy(m_data, str);
	}

}

MyString::MyString( MyString &&other)    // 移动构造函数
{
	m_data = other.m_data;
	other.m_data = NULL;

}
// MyString的析构函数
MyString::~MyString(void)
{
	delete[] m_data; // 或delete m_data;
	m_data = NULL;
}
//拷贝构造函数
MyString::MyString(const MyString &other)  // 得分点：输入参数为const型
{
	size_t length = strlen(other.m_data);
	m_data = new char[length + 1]; //加分点：对m_data加NULL 判断
	strcpy(m_data, other.m_data);
}

MyString & MyString::operator = (const MyString &other) // 得分点：输入参数为const 型
{

	if (this == &other)                  //得分点：检查自赋值
		return *this;
	delete[] m_data;            //得分点：释放原有的内存资源
	size_t length = strlen(other.m_data);
	m_data = new char[length + 1]; //加分点：对m_data加NULL 判断
	strcpy(m_data, other.m_data);
	return *this;
};  //得分点：返回本对象的引用  */

	//移动赋值运算符  
MyString& MyString::operator=(MyString&& other)
{
	if (this == &other)
		return *this;
	if (m_data)
		delete[] m_data;
 
	m_data = other.m_data;	 
	other.m_data = nullptr;
 
	return *this;
}

//赋值函数

