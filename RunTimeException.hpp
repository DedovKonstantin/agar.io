#pragma once

#include <MyLib\MyString.hpp>
#include <vector>

using namespace std;

class RunTimeException
{
private:
	MyString message;
public:
	RunTimeException()
	{
	}
	RunTimeException(const char *mes)
	{
		message = mes;
	}
	RunTimeException(const RunTimeException& other)
	{
		message = other.message;
	}
	~RunTimeException()
	{
	}
	RunTimeException& operator<<(const MyString& ms)
	{
		for (size_t i = 0; i < ms.getLength(); ++i)
			message += ms[i];
		return *this;
	}
	RunTimeException& operator<<(int num)
	{
		if (num < 0)
			message += '-';
		if (num == 0)
		{
			message += '0';
			return *this;
		}
		vector<int> number;
		while (num)
		{
			number.push_back(num % 10);
			num /= 10;
		}
		for (size_t i = 0; i < number.size(); ++i)
			message += number[number.size() - i - 1] + '0';
		return *this;
	}
	const MyString& getMessage()const
	{
		return message;
	}
};
inline ostream& operator<<(ostream& os, const RunTimeException& exc)
{
	os << exc.getMessage();
	return os;
}
