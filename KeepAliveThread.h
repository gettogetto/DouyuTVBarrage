#pragma once
#include<qthread.h>
class QTcpSocket;
class KeepAliveThread:public QThread
{
public:
	KeepAliveThread(QTcpSocket* tcpSocket=nullptr);
	~KeepAliveThread();

protected:
	void run() override;
private:
	QTcpSocket *m_tcp_socket;
};

