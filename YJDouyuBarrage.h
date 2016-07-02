#ifndef YJDOUYUBARRAGE_H
#define YJDOUYUBARRAGE_H

#include <QtWidgets/QMainWindow>
#include<qtcpsocket.h>
#include "ui_YJDouyuBarrage.h"
#include"KeepAliveThread.h"
class ClientTypeCount;
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
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

	//http
	void init_http();
public slots:
	void ok_button_clicked();
	void disconnect_button_clicked();
	void read_and_process();
	void run_keepAlive_thread();
	void handleTcpSocketError();
	void handleTcpSocketConnected();
	void handleTcpSocketDisconnected();
	void handleHttpReply(QNetworkReply*);

	void roomInformationParse(const QByteArray& byte_array);

	
private:
	Ui::YJDouyuBarrageClass ui;
	//tcp
	QTcpSocket* m_barrage_tcp_socket = nullptr;
	QString m_hostName_barrage;
	quint16 m_port_barrage;

	KeepAliveThread* m_keepAlive_thread = nullptr;
	QTimer *m_keep_alive_timer = nullptr;
	ClientTypeCount *m_clientTypeCount = nullptr;
	//http
	QNetworkAccessManager* m_network_access_manager = nullptr;
	QNetworkReply* m_network_reply = nullptr;
	QNetworkRequest* m_network_request = nullptr;
	

};

#endif // YJDOUYUBARRAGE_H
