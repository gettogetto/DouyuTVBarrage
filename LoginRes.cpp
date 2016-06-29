#include "LoginRes.h"
#include"Decoder.h"


LoginRes::LoginRes():m_res(0)
{
}


LoginRes::~LoginRes()
{
}

void LoginRes::deserialize(std::string str) {
	m_res = 0;
	std::string to_be_decode = str.substr(8);

	Decoder tmp_decoder;
	tmp_decoder.decode(to_be_decode);

	std::string type_str = tmp_decoder.get_string_item("type");

	if (type_str != "loginres") {
		m_res = -1;
	}

	m_livestat = tmp_decoder.get_int_item("live_stat");
}
