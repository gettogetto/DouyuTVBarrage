#pragma once
#include<string>
class LoginRes
{
public:
	int m_res;
	int m_livestat;
	LoginRes();
	~LoginRes();
	void deserialize(std::string str);
};

