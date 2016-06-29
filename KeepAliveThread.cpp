#include "KeepAliveThread.h"
#include"AllRequests.h"
#include<qtcpsocket.h>
#include<qdatetime.h>
#include<fstream>
KeepAliveThread::KeepAliveThread(QTcpSocket* tcpSocket)
{
	m_tcp_socket = tcpSocket;
}


KeepAliveThread::~KeepAliveThread()
{
}

void KeepAliveThread::run() {
	QDateTime time = QDateTime::currentDateTime();
	KeepAliveRequest tmp_keepAliveRequst((int)time.toTime_t());

	std::string to_be_send = tmp_keepAliveRequst.serialize();


	QByteArray byteArray = QByteArray(to_be_send.c_str(),to_be_send.size());
	qDebug() << m_tcp_socket->write(byteArray);

}
