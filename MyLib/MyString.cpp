#include "MyString.hpp"

MyString::MyString()
{
	data = new char[1];
	data[0] = 0;
	size = 1;
}
MyString::MyString(char c)
{
	data = new char[2];
	data[0] = c;
	data[1] = 0;
	size = 2;
}
MyString::MyString(const char *s)
{
	data = new char[size = strlen(s) + 1];
	char *ptrThis = data;

	while (*ptrThis++ = *s++)
		;
	data[size - 1] = 0;
}
MyString::MyString(const MyString& other)
{
	data = new char[size = strlen(other.data) + 1];
	char *ptrThis = data;
	char *otherdata = other.data;
	while (*ptrThis++ = *otherdata++)
		;
	data[size - 1] = 0;
}
MyString::~MyString()
{
	delete[] data;
}
MyString& MyString::operator += (const char c)
{
	++size;
	if (data == 0)
	{
		data = new char[size = 1 + 1];
		data[0] = c;
		data[1] = 0;
		return *this;
	}
	char *newData = new char[size + 1];
	char *ptrThis = data, *ptrNew = newData, *ptrNewCurr = ptrNew;
	while (*ptrNewCurr++ = *ptrThis++)
		;
	ptrNewCurr[-1] = c;
	ptrNewCurr[0] = 0;
	delete[] data;
	data = ptrNew;

	return *this;
}
MyString::operator const char *()const
{
	return data;
}
MyString::operator std::string() const
{
	return std::string(data);
}
void MyString::scan(std::istream& is)
{
	const int block = 0x100;
	delete[] data;
	data = new char[block];
	char currentSymbol;
	int currentSize = 0;

	is.get(currentSymbol);
	while (!is.eof() && !isspace((unsigned char)currentSymbol))
	{
		data[currentSize++] = currentSymbol;
		is.get(currentSymbol);
		if (currentSize % block == 0)
		{
			char *newData = new char[currentSize + block];
			for (int i = 0; i < currentSize; ++i)
				newData[i] = data[i];

			delete[] data;
			data = newData;
		}
	}
	data[currentSize++] = 0;
	size = currentSize;
}
void MyString::fscan(FILE *fin)
{
	const int block = 0x100;
	delete[] data;
	data = new char[block];
	unsigned char currentSymbol;
	int currentSize = 0;

	currentSymbol = fgetc(fin);
	while (!isspace(currentSymbol) && !feof(fin))
	{
		data[currentSize++] = currentSymbol;
		currentSymbol = fgetc(fin);
		if (currentSize % block == 0)
		{
			char *newData = new char[currentSize + block];
			for (int i = 0; i < currentSize; ++i)
				newData[i] = data[i];

			delete[] data;
			data = newData;
		}
	}
	data[currentSize++] = 0;
	size = currentSize;
}


bool MyString::operator==(const char* other)const
{
	return strncmp(data, other, size) == 0;
}
bool MyString::operator!=(const char* other)const
{
	return strncmp(data, other, size) != 0;
}
void MyString::doForEach(void(*f)(int&))
{
	for (size_t i = 0; i < size; ++i)
		f((int&)data[i]);
}
void MyString::doForEach(int(*f)(int))
{
	for (size_t i = 0; i < size; ++i)
		data[i] = f(data[i]);
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
	for (size_t i = 0; i < str.getLength(); ++i)
		os << str[i];
	return os;
}
std::istream& operator>>(std::istream& is, MyString& str)
{
	str.scan(is);
	return is;
}
std::string& operator>>(std::string& is, MyString& str)
{
	str = is;
	return is;
}
MyString MyString::operator=(const char *other)
{
	data = new char[size = strlen(other) + 1];
	char *ptrThis = data;

	while (*ptrThis++ = *other++)
		;
	data[size - 1] = 0;

	return *this;
}
MyString MyString::operator=(const std::string other)
{
	data = new char[size = other.size() + 1];
	char *ptrThis = data;
	for (size_t i = 0; i < other.size(); ++i)
		*ptrThis++ = other[i];
	data[size - 1] = 0;

	return *this;
}

MyString MyString::operator=(const MyString& other)
{
	data = new char[size = other.getLength() + 1];
	char *ptrThis = data;
	for (size_t i = 0; i < other.getLength(); ++i)
		*ptrThis++ = other[i];
	data[size - 1] = 0;

	return *this;
}
size_t MyString::getLength()const
{
	return size - 1;
}


MyString& MyString::operator += (const char *str)
{
	while (*str)
		*this += *(str++);
	return *this;
}
MyString& MyString::operator += (const MyString& other)
{
	return *this += (const char *)other;
}
MyString MyString::operator + (const MyString& other)
{
	MyString result(*this);

	result += other;

	return result;
}
MyString MyString::GetStringFromNumber(__int64 num)
{
	if (num == 0)
		return "0";

	MyString result;
	if (num < 0)
		num += '-';
	while (num)
	{
		result = MyString((char)(num % 10 + '0')) + result;
		num /= 10;
	}

	return result;
}
