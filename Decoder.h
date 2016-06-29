#pragma once
#include<vector>
#include<utility>
#include<string>
class Decoder
{
public:
	Decoder();
	~Decoder();

	void decode( const std::string& str);

	std::string get_string_item(const std::string &key);

	int get_int_item(const std::string &key);
private:
	std::vector<std::pair<std::string, std::string> > m_array;



};

