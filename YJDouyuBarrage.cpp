#include "YJDouyuBarrage.h"
#include"AllRequests.h"
#include"BarrageRes.h"
#include"LoginRes.h"
#include"Encoder.h"
#include"Decoder.h"
#include"GlobalDef.h"
#include<qdatastream.h>
#include<qbytearray.h>
#include<qthread.h>
#include<qstring.h>
#include<qtimer.h>
#include<iostream>
#include<fstream>

YJDouyuBarrage::YJDouyuBarrage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	init_tcp();
	init_thread();
	init_connection();


}

YJDouyuBarrage::~YJDouyuBarrage()
{
	delete m_keepAlive_thread;
}

void YJDouyuBarrage::init_tcp() {
	m_hostName = "openbarrage.douyutv.com";
	m_port = 8602;
	m_barrage_tcp_socket = new QTcpSocket(this);


}

void YJDouyuBarrage::init_thread() {
	m_keep_alive_timer = new QTimer(this);
	m_keepAlive_thread = new KeepAliveThread(m_barrage_tcp_socket);
}
void YJDouyuBarrage::login_room() {
	LoginRequest tmp_loginRequset;
	std::string to_be_send = tmp_loginRequset.serialize();

	QByteArray byteArray = QByteArray(to_be_send.data(), to_be_send.size());

	qDebug()<<m_barrage_tcp_socket->write(byteArray);

}

void YJDouyuBarrage::login_group(int rid, int gid) {
	JoinGroupRequest tmp_JoinGroupRequest(rid, gid);
	std::string to_be_send = tmp_JoinGroupRequest.serialize();


	QByteArray byteArray = QByteArray(to_be_send.data(), to_be_send.size());

	qDebug() << m_barrage_tcp_socket->write(byteArray);

}

void YJDouyuBarrage::init_connection() {

	connect(ui.m_getBarrage_pushButton, SIGNAL(clicked()), this, SLOT(ok_button_clicked()));
	connect(ui.m_disconnect_pushButton, SIGNAL(clicked()), this, SLOT(disconnect_button_clicked()));
	connect(m_barrage_tcp_socket, SIGNAL(readyRead()), this, SLOT(read_and_process()));
	connect(m_keep_alive_timer, SIGNAL(timeout()), this, SLOT(run_keepAlive_thread()));

	//connect(m_barrage_tcp_socket, SIGNAL(error()), this, SLOT(handleTcpSocketError()));
	connect(m_barrage_tcp_socket, SIGNAL(connected()), this, SLOT(handleTcpSocketConnected()));
	connect(m_barrage_tcp_socket, SIGNAL(disconnected()), this, SLOT(handleTcpSocketDisconnected()));
}

void YJDouyuBarrage::ok_button_clicked() {
	if (ui.m_roomID_lineEdit->text().isEmpty()) return;
	m_barrage_tcp_socket->connectToHost(QString(m_hostName), m_port, QIODevice::ReadWrite);

}

void YJDouyuBarrage::disconnect_button_clicked() {
	m_barrage_tcp_socket->disconnectFromHost();

	m_keepAlive_thread->terminate();
}

void YJDouyuBarrage::read_and_process() {
	qDebug() << "void YJDouyuBarrage::read_and_process()";

	qint32 messageLen;

	m_barrage_tcp_socket->read((char*)&messageLen, 4);
	if (messageLen <= 8) {
		qDebug() << "messageLen <= 8";
		return;
	}
	qDebug() << messageLen;



	char messageBuf[MAXMESSAGELEN];
	int bufsize=m_barrage_tcp_socket->read(messageBuf, messageLen);
	//for (int i = 0; i < bufsize; i++) qDebug() << messageBuf[i];
	std::string message(messageBuf,bufsize);

	int tmp_messageType = get_message_type(message);

	if (tmp_messageType == MESSAGE_TYPE::LOGIN_TYPE) {
		ui.m_barrage_textBrowser->append("LOGIN_TYPE");
		LoginRes tmp_loginres;
		tmp_loginres.deserialize(message);
		if (tmp_loginres.m_res == -1) {
			ui.m_barrage_textBrowser->append("login room fail!");
		}
	}
	else if (tmp_messageType == MESSAGE_TYPE::BARRAGE_TYPE) {

		BarrageRes tmp_barrageres;
		tmp_barrageres.deserialize(message);
		
		
		ui.m_barrage_textBrowser->append(QString(tmp_barrageres.m_sender_name.data())
			+ " : "
			+ QString(tmp_barrageres.m_barrage_content.data()));
	}
}

void YJDouyuBarrage::run_keepAlive_thread() {
	m_keepAlive_thread->run();
	m_keep_alive_timer->start(45000);
	ui.m_barrage_textBrowser->append("keep alive every 45s");
}

void YJDouyuBarrage::handleTcpSocketError() {
	ui.m_barrage_textBrowser->append(m_barrage_tcp_socket->errorString());
}

void YJDouyuBarrage::handleTcpSocketConnected() {
	ui.m_barrage_textBrowser->append(QString("connected success!"));
	login_room();
	login_group(ui.m_roomID_lineEdit->text().toInt(), -9999);
	m_keepAlive_thread->run();
	m_keep_alive_timer->start(45000);

}

void YJDouyuBarrage::handleTcpSocketDisconnected() {
	ui.m_barrage_textBrowser->append(QString("disconnected success!"));
}