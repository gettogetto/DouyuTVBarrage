#pragma once
#include<string>
class Encoder
{
public:
	Encoder();
	~Encoder();
	std::string get_res();

	void add_string_item(const std::string& key, const std::string& value);

	void add_int_item(const std::string &key, const int value);
private:
	std::string m_res;
};

