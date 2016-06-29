#pragma once

#include<string>
class PackageHeader
{
private:
	std::string m_package_res;
	int m_len;
public:
	
	PackageHeader();
	PackageHeader(const std::string& str);
	~PackageHeader();

	std::string get_package_res();
	int get_package_len();
};

