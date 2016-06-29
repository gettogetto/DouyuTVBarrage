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

	void deserialize(const std::string &recvString);

};

