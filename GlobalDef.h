#pragma once
#include"Decoder.h"
#include<string>
#include<qdebug.h>
#include<qstring.h>
const int MAXMESSAGELEN = 40960;
enum MESSAGE_TYPE {
	BARRAGE_TYPE = 1,
	LOGIN_TYPE = 2
};

 int get_message_type(const std::string package_str);
 std::string pack_header(std::string data_str);



