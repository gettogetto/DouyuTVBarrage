#pragma once
#include<string>
#include"PackageHeader.h"
class LoginRequest
{
public:
	LoginRequest() {

	}
	~LoginRequest() {

	}
	std::string serialize();
};

class JoinGroupRequest {
public:
	int m_roomID;
	int m_groupID;
	JoinGroupRequest() {
		m_roomID = 0;
		m_groupID = 0;
	}
	JoinGroupRequest(int rid,int gid) {
		m_roomID = rid;
		m_groupID = gid;
	}
	~JoinGroupRequest() {

	}
	std::string serialize();
};

class KeepAliveRequest {
public:
	int m_timeStamp;
	KeepAliveRequest() {

	}
	KeepAliveRequest(int timeStamp):m_timeStamp(timeStamp) {
		
	}
	~KeepAliveRequest() {

	}
	std::string serialize();
};

