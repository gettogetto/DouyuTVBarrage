#pragma once
#include <qthread.h>
#include<qstring.h>
class QNetworkAccessManager;
class RoomUpdateThread :
	public QThread
{
public:
	RoomUpdateThread(const QString& roomid,QNetworkAccessManager* networkmanager=nullptr);
	~RoomUpdateThread();

	void run() override;
private:
	QNetworkAccessManager* m_networkmanager;
	QString m_roomid;
};

