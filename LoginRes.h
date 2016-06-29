#pragma once
#include<string>
class LoginRes
{
public:
	int m_res;
	LoginRes();
	~LoginRes();
	void deserialize(std::string str);
};

