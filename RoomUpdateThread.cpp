#include "RoomUpdateThread.h"
#include<qnetworkaccessmanager.h>

#include<qnetworkrequest.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qurl.h>



RoomUpdateThread::RoomUpdateThread(const QString& roomid,QNetworkAccessManager* networkmanager)
{
	m_networkmanager = networkmanager;
	m_roomid = roomid;
}


RoomUpdateThread::~RoomUpdateThread()
{
}

void RoomUpdateThread::run() {
	
	QUrl tmp_url = "http://open.douyucdn.cn/api/RoomApi/room/" + m_roomid;
	QNetworkRequest request = QNetworkRequest(tmp_url);
	m_networkmanager->get(request);

}