#include "PackageHeader.h"

#include"qdebug.h"

PackageHeader::PackageHeader()
{
}


PackageHeader::~PackageHeader()
{
}

PackageHeader::PackageHeader(const std::string & str) {
	std::string tmp_pack_str;
	int data_len = str.length() + 8;
	short messageType = 689;
	char encrypt = 0;
	char reserve = 0;

	tmp_pack_str.append((const char*)&data_len, sizeof(data_len));
	tmp_pack_str.append((const char*)&data_len, sizeof(data_len));
	tmp_pack_str.append((const char*)&messageType, sizeof(messageType));
	tmp_pack_str.append((const char*)&encrypt, sizeof(encrypt));
	tmp_pack_str.append((const char*)&reserve, sizeof(reserve));
	tmp_pack_str.append(str.data(), str.size());

	qDebug() <<"PackageHeader::PackageHeader(const std::string & str):"<< tmp_pack_str.data();
	m_package_res = tmp_pack_str;
	m_len = data_len;

}
std::string PackageHeader::get_package_res() {

	return m_package_res;
}

int PackageHeader::get_package_len() {
	return m_len;
}