#ifndef YJDOUYUBARRAGE_H
#define YJDOUYUBARRAGE_H

#include <QtWidgets/QMainWindow>
#include<qtcpsocket.h>
#include "ui_YJDouyuBarrage.h"
#include"KeepAliveThread.h"
class ClientTypeCount;
class QTimer;
class YJDouyuBarrage : public QMainWindow
{
	Q_OBJECT

public:

	YJDouyuBarrage(QWidget *parent = 0);
	~YJDouyuBarrage();
private:
	void init_tcp();
	void init_thread();
	void login_room();
	void login_group(int rid, int gid);
	void init_connection();
	void updateClientTypeCount(int clientType);
public slots:
	void ok_button_clicked();
	void disconnect_button_clicked();
	void read_and_process();
	void run_keepAlive_thread();
	void handleTcpSocketError();
	void handleTcpSocketConnected();
	void handleTcpSocketDisconnected();
private:
	Ui::YJDouyuBarrageClass ui;
	QTcpSocket* m_barrage_tcp_socket;
	QString m_hostName;
	quint16 m_port;

	KeepAliveThread* m_keepAlive_thread;
	QTimer *m_keep_alive_timer;
	ClientTypeCount *m_clientTypeCount;
};

#endif // YJDOUYUBARRAGE_H
