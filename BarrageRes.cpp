#include "BarrageRes.h"
#include "Decoder.h"


BarrageRes::BarrageRes()
{
}


BarrageRes::~BarrageRes()
{
}

void BarrageRes::deserialize(const std::string& recvString) {
	//去掉包首部8个字节
	std::string to_be_decode = recvString.substr(8);
	Decoder tmp_decoder;
	tmp_decoder.decode(to_be_decode);
	m_client_type = tmp_decoder.get_int_item("ct");
	m_sender_room_group = tmp_decoder.get_int_item("rg");
	m_sender_platform_group = tmp_decoder.get_int_item("pg");
	m_barrage_content = tmp_decoder.get_string_item("txt");
	m_sender_name = tmp_decoder.get_string_item("nn");
	m_sender_id = tmp_decoder.get_int_item("uid");
	m_sender_level = tmp_decoder.get_int_item("level");

	m_sender_type = tmp_decoder.get_int_item("ct");
	

}