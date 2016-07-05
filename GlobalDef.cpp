#include"GlobalDef.h"

 int get_message_type(const std::string package_str) {
	if (package_str.length() <= 8) {
		return -1;
	}
	MESSAGE_TYPE messageType;
	std::string to_be_decode = package_str.substr(8);
	Decoder tmp_decoder;
	tmp_decoder.decode(to_be_decode);
	std::string tmp_type = tmp_decoder.get_string_item("type");
	if (tmp_type == "chatmsg") {
		messageType = MESSAGE_TYPE::BARRAGE_TYPE;
	}
	else if (tmp_type == "loginres") {
		messageType = MESSAGE_TYPE::LOGIN_TYPE;
	}
	return messageType;
}
 std::string pack_header(std::string data_str)
{
	std::string pack_str;

	int data_len = data_str.length() + 8;
	short msg_type = 689;     //client message type is 689
	char encrypt = 0;
	char reserve = 0;

	pack_str.append((const char *)&data_len, sizeof(data_len));      // 4 bytes is len
	pack_str.append((const char *)&data_len, sizeof(data_len));      // 4 bytes is len
	pack_str.append((const char *)&msg_type, sizeof(msg_type));      // 2 bytes is message type
	pack_str.append((const char *)&encrypt, sizeof(encrypt));      // 1 bytes is encrypt
	pack_str.append((const char *)&reserve, sizeof(reserve));      // 1 bytes is reserve
	pack_str.append(data_str.data(), data_str.size());        //data

	return pack_str;
}