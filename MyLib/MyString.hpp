#pragma once
#ifdef _DEBUG
#pragma comment(lib, "MyLib/MyString.lib")
#else
#pragma comment(lib, "MyLib/MyString_Release.lib")
#endif
#include <iostream>
#include <sstream>
#include <varargs.h>

class MyString
{
private:
	char *data;
	size_t size;
public:
	MyString();
	MyString(char c);
	MyString(const char *s);
	MyString(const MyString& other);
	~MyString();

	static MyString GetStringFromNumber(__int64 num);

	//Присоединение символа в конец строки.
	MyString& operator += (const char c);

	//Преобразование в const char*
	operator const char *()const;
	operator std::string() const;

	//Считывание строки из входного потока с выделением памяти. 
	void scan(std::istream& is);

	//Считывание строки из файла с выделением памяти. 
	void fscan(FILE *fin);

	//Сравнение с другой строкой
	bool operator==(const char* other)const;
	bool operator!=(const char* other)const;
	MyString operator=(const char* other);
	MyString operator=(const std::string other);
	MyString operator=(const MyString& other);

	size_t getLength()const;
	//Преобразовать каждый символ функцией
	void doForEach(void(*f)(int&));
	//Преобразовать каждый символ функцией
	void doForEach(int(*f)(int));
	//Для каждого символа строки делать
	template<typename T> void doForEach_const(T(*nextT)(T prev, int symbol), T&t) const
	{
		for (size_t i = 0; i < size; ++i)
			t = nextT(t, data[i]);
	}
	operator bool() { return data != nullptr; }

	MyString& operator += (const char *str);
	MyString& operator += (const MyString& other);
	MyString operator + (const MyString& other);
};

std::ostream& operator<<(std::ostream& os, const MyString& str);
std::istream& operator>>(std::istream& is, MyString& str);
std::string& operator>>(std::string& is, MyString& str);
