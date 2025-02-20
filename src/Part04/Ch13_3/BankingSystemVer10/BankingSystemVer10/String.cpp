/*
* 파일이름: String.cpp
* 작성자: 이우식
* 업데이트 정보: [2025, 02, 19] 파일버전 0.1
*/

#include "String.h"

String::String()
{
	len = 0;
	str = NULL;
}

String::String(const char* s)
{
	len = strlen(s) + 1;
	str = new char[len];
	strcpy_s(str, len, s);
}

String::String(const String& s)
{
	len = s.len;
	str = new char[len];
	strcpy_s(str, len, s.str);
}

String::~String()
{
	if (str != NULL)
		delete[] str;
}

String& String::operator= (const String& s)
{
	if (str != NULL)
		delete[] str;
	len = s.len;
	str = new char[len];
	strcpy_s(str, len, s.str);
	return *this;
}

String& String::operator+= (const String& s)
{
	len += (s.len - 1);
	char* tempstr = new char[len];
	strcpy_s(tempstr, len, str);
	strcat_s(tempstr, len, s.str);
	if (str != NULL)
		delete[] str;
	str = tempstr;
	return *this;
}

bool String::operator== (const String& s)
{
	return strcmp(str, s.str) ? false : true;
}

String String::operator+ (const String& s)
{
	char* tempstr = new char[len + s.len - 1];
	strcpy_s(tempstr, len, str);
	strcat_s(tempstr, len, s.str);
	String temp(tempstr);
	delete[] tempstr;
	return temp;
}

ostream& operator<< (ostream& os, const String& s)
{
	os << s.str;
	return os;
}

istream& operator>> (istream& is, String& s)
{
	char str[100];
	is >> str;
	s = String(str);
	return is;
}