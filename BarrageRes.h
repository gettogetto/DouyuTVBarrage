#pragma once
#include<string>
class BarrageRes
{
public:
	BarrageRes();
	~BarrageRes();

	int m_client_type;

	int m_sender_room_group;

	int m_sender_platform_group;

	std::string m_barrage_content;

	std::string m_sender_name;

	int m_sender_id;

	int m_sender_level;

	int m_sender_type;

	void deserialize(const std::string &recvString);

};

