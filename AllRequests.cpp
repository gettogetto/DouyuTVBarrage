#include "AllRequests.h"
#include "Encoder.h"

#include"GlobalDef.h"
#include<qdebug.h>


std::string  LoginRequest::serialize() {
	Encoder tmp_encoder;
	tmp_encoder.add_string_item("type","loginreq");
	std::string encode_res = tmp_encoder.get_res();

	char end = '\0';
	encode_res.append((const char*)&end, 1);
	//qDebug() << encode_res.c_str() << " " << encode_res.size();
	//PackageHeader tmpPackageHeader(encode_res);
	return pack_header(encode_res);
	//return tmpPackageHeader.get_package_res();
}

std::string  JoinGroupRequest::serialize() {
	Encoder tmp_encoder;
	tmp_encoder.add_string_item("type", "joingroup");
	tmp_encoder.add_int_item("rid", m_roomID);
	tmp_encoder.add_int_item("gid",m_groupID);
	std::string encode_res = tmp_encoder.get_res();

	char end = '\0';
	encode_res.append((const char*)&end, 1);
	//qDebug() << encode_res.c_str() << " " << encode_res.size();
	//PackageHeader tmpPackageHeader(encode_res);

	return pack_header(encode_res);
	//return tmpPackageHeader.get_package_res();
}

std::string KeepAliveRequest::serialize() {
	Encoder tmp_encoder;
	tmp_encoder.add_string_item("type", "keeplive");
	tmp_encoder.add_int_item("tick",m_timeStamp);
	std::string encode_res = tmp_encoder.get_res();

	char end = '\0';
	encode_res.append((const char*)&end, 1);
	//PackageHeader tmpPackageHeader(encode_res);
	//qDebug() << encode_res.c_str() << " " << encode_res.size();
	return pack_header(encode_res);
	//return tmpPackageHeader.get_package_res();
}