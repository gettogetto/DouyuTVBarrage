#include "YJDouyuBarrage.h"
#include"AllRequests.h"
#include"BarrageRes.h"
#include"LoginRes.h"
#include"Encoder.h"
#include"Decoder.h"
#include"GlobalDef.h"
#include"ClientTypeCount.h"
#include"Json.h"
#include<qdatastream.h>
#include<qbytearray.h>
#include<qthread.h>
#include<qstring.h>
#include<qtimer.h>
#include<iostream>
#include<fstream>
#include<qnetworkaccessmanager.h>
#include<qnetworkreply.h>
#include<qnetworkrequest.h>
#include<qurl.h>
#include<qjsondocument.h>
const int ROOMTIMEINTERVAL = 50000;
const int KEEPALIVETIMEINTERVAL = 50000;
YJDouyuBarrage::YJDouyuBarrage(QWidget *parent)
	: QMainWindow(parent)
{
	m_clientTypeCount = new ClientTypeCount;
	ui.setupUi(this);
	init_tcp();

	init_keepAlive_thread();
	init_connection();
}

YJDouyuBarrage::~YJDouyuBarrage()
{
	delete m_clientTypeCount;
	delete m_keepAlive_thread;
}

void YJDouyuBarrage::init_tcp() {
	m_hostName_barrage = "openbarrage.douyutv.com";
	m_port_barrage = 8601;
	m_barrage_tcp_socket = new QTcpSocket(this);


}

void YJDouyuBarrage::init_http() {
	QString tmp_room = ui.m_roomID_lineEdit->text();
	if (tmp_room.isEmpty()) return;
	QUrl tmp_url = "http://open.douyucdn.cn/api/RoomApi/room/" + tmp_room;

	if (m_network_access_manager == nullptr)
		m_network_access_manager = new QNetworkAccessManager(this);
	if (m_network_request == nullptr)
		m_network_request = new QNetworkRequest(tmp_url);
	connect(m_network_access_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpReply(QNetworkReply*)));
	m_network_reply = m_network_access_manager->get(*m_network_request);

	init_roomUpdate_thread();
}

void YJDouyuBarrage::init_keepAlive_thread() {
	m_keep_alive_timer = new QTimer(this);
	m_keepAlive_thread = new KeepAliveThread(m_barrage_tcp_socket);


}


void YJDouyuBarrage::init_roomUpdate_thread() {
	QString tmp_room = ui.m_roomID_lineEdit->text();
	if (m_room_update_thread == nullptr) m_room_update_thread = new RoomUpdateThread(tmp_room, m_network_access_manager);
	if (m_room_update_timer == nullptr) m_room_update_timer = new QTimer(this);
	m_room_update_timer->start(ROOMTIMEINTERVAL);
	connect(m_room_update_timer, SIGNAL(timeout()), this, SLOT(run_room_update_thread()));

}

void YJDouyuBarrage::login_room() {
	LoginRequest tmp_loginRequset;
	std::string to_be_send = tmp_loginRequset.serialize();

	QByteArray byteArray = QByteArray(to_be_send.data(), to_be_send.size());

	qDebug() << m_barrage_tcp_socket->write(byteArray);

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
	m_barrage_tcp_socket->connectToHost(QString(m_hostName_barrage), m_port_barrage, QIODevice::ReadWrite);

	init_http();
}

void YJDouyuBarrage::disconnect_button_clicked() {
	m_barrage_tcp_socket->disconnectFromHost();

	m_keepAlive_thread->terminate();

	m_room_update_thread->terminate();
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
	int bufsize = m_barrage_tcp_socket->read(messageBuf, messageLen);
	//for (int i = 0; i < bufsize; i++) qDebug() << messageBuf[i];
	std::string message(messageBuf, bufsize);

	int tmp_messageType = get_message_type(message);

	if (tmp_messageType == MESSAGE_TYPE::LOGIN_TYPE) {
		ui.m_barrage_textBrowser->append("LOGIN_TYPE");
		LoginRes tmp_loginres;
		tmp_loginres.deserialize(message);
		if (tmp_loginres.m_res == -1) {
			ui.m_barrage_textBrowser->append("login room fail!");
		}
		ui.m_barrage_textBrowser->append(
			QString("LiveStat : ") +
			std::to_string(tmp_loginres.m_livestat).data()
			);
	}
	else if (tmp_messageType == MESSAGE_TYPE::BARRAGE_TYPE) {

		BarrageRes tmp_barrageres;
		tmp_barrageres.deserialize(message);

		int clientType = tmp_barrageres.m_sender_type;
		updateClientTypeCount(clientType);

		ui.m_barrage_textBrowser->append(
			QString(clientType == 0 ? "Web" : clientType == 1 ? "Andriod" : clientType == 2 ? "iOS" : "Computer")
			+ " "
			+ QString(tmp_barrageres.m_sender_name.data())
			+ "(" + QString(std::to_string(tmp_barrageres.m_sender_level).data()) + ")"
			+ " : "
			+ QString(tmp_barrageres.m_barrage_content.data())
			);
	}
}
void YJDouyuBarrage::updateClientTypeCount(int clientType) {
	clientType == 0 ? 0 : clientType == 1 ? m_clientTypeCount->m_android++ : clientType == 2 ? m_clientTypeCount->m_iOS++ : m_clientTypeCount->m_computer++;
	ui.m_andriod_count_lineEdit->setText(std::to_string(m_clientTypeCount->m_android).data());
	ui.m_iOS_count_lineEdit->setText(std::to_string(m_clientTypeCount->m_iOS).data());
	ui.m_computer_count_lineEdit->setText(std::to_string(m_clientTypeCount->m_computer).data());
}
void YJDouyuBarrage::run_keepAlive_thread() {
	m_keepAlive_thread->start();
	//m_keep_alive_timer->start(KEEPALIVETIMEINTERVAL);
	ui.m_barrage_textBrowser->append("keep alive every 45s");
}

void YJDouyuBarrage::run_room_update_thread() {
	ui.m_barrage_textBrowser->append("run_room_update_thread");
	m_room_update_thread->start();
	//m_room_update_timer->start(ROOMTIMEINTERVAL);
	//m_room_update_thread->run();
}

//void YJDouyuBarrage::handleTcpSocketError() {
//	ui.m_barrage_textBrowser->append(m_barrage_tcp_socket->errorString());
//}

void YJDouyuBarrage::handleTcpSocketConnected() {
	ui.m_barrage_textBrowser->append(QString("connected success!"));
	login_room();
	login_group(ui.m_roomID_lineEdit->text().toInt(), -9999);
	m_keepAlive_thread->start();
	m_keep_alive_timer->start(KEEPALIVETIMEINTERVAL);

}

void YJDouyuBarrage::handleTcpSocketDisconnected() {
	ui.m_barrage_textBrowser->append(QString("disconnected success!"));
}

void YJDouyuBarrage::handleHttpReply(QNetworkReply* reply) {
	qDebug() << "void YJDouyuBarrage::handleHttpReply(QNetworkReply* reply) ";
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray replyData = reply->readAll();

		QString error = JsonMultiNestedObject(
			QJsonValue(QJsonObject(QJsonDocument::fromJson(replyData).object())), 
			std::vector<QString>{"error"}, 0);
		if (error != "0") {
			ui.m_barrage_textBrowser->append("failed to get room imformation!");
			return;
		}
		else {
			roomInformationParse(replyData);
		}

	}
	else {
		ui.m_barrage_textBrowser->append(QString("http reply error!"));
	}
}

void YJDouyuBarrage::roomInformationParse(const QByteArray& bytearray) {

	QString room_id = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "room_id"}, 0);

	QString room_name = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "room_name"}, 0);

	QString room_status = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "room_status"}, 0);

	QString online = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "online"}, 0);

	QString owner_weight = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "owner_weight"}, 0);

	QString fans_num = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "fans_num"}, 0);

	QString owner = JsonMultiNestedObject(
		QJsonValue(QJsonObject(QJsonDocument::fromJson(bytearray).object())),
		std::vector<QString>{"data", "owner_name"}, 0);
	/*
	ui.m_barrage_textBrowser->append("fans_num:" + fans_num);
	ui.m_barrage_textBrowser->append("room_id:" + room_id);
	ui.m_barrage_textBrowser->append("room_name:" + room_name);
	ui.m_barrage_textBrowser->append("room_status:" + room_status);
	ui.m_barrage_textBrowser->append("online:" + online);
	ui.m_barrage_textBrowser->append("owner_weight:" + owner_weight);
	*/
	ui.m_fansNum_lineEdit->setText(fans_num);
	ui.m_onlineNum_lineEdit->setText(online);
	ui.m_roomOwner_lineEdit->setText(owner);
	ui.m_roomName_lineEdit->setText(room_name);
	ui.m_roomOwnerWeight_lineEdit->setText(owner_weight);
	ui.m_roomStatus_lineEdit->setText(room_status=="1"?"On":"Off");
}
/*
"{
\"error\": 0,
\"data\": {
\"room_id\": \"485503\",
\"room_thumb\": \"http: \\/\\/rpic.douyucdn.cn\\/z1607\\/01\\/21\\/485503_160701214739.jpg\",
\"cate_id\": \"124\",
\"cate_name\": \"Outdoor\",
\"room_name\": \"\\u7528\\u5fc3\\u770b\\u4e16\\u754c\\uff0c\\u63a2\\u9669\\u7ea2\\u77f3\\u5be8\",
\"room_status\": \"1\",
\"owner_name\": \"\\u5f61\\u5f61\\u4e5d\\u6237\\u5916\",
\"avatar\": \"http: \\/\\/uc.douyutv.com\\/upload\\/avatar\\/029\\/84\\/29\\/38_avatar_big.jpg?rltime\",
\"online\": 264229,
\"owner_weight\": \"101.33t\",
\"fans_num\": \"671199\",
\"start_time\": \"2016-07-0116: 51\",
\"gift\": [
{
\"id\": \"59\",
\"name\": \"\\u706b\\u7bad\",
\"type\": \"2\",
\"pc\": 500,
\"gx\": 5000,
\"desc\": \"\\u8d60\\u9001\\u7f51\\u7ad9\\u5e7f\\u64ad\\u5e76\\u6d3e\\u9001\\u51fa\\u795e\\u79d8\\u5b9d\\u7bb1\",
\"intro\": \"\\u6211\\u4eec\\u7684\\u5f81\\u9014\\u662f\\u661f\\u8fb0\\u5927\\u6d77\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/c659808dbe34657b2bddd788a0eecfe4.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/66e93802817e71f6b9543b11670c7048.gif\"
},
{
\"id\": \"195\",
\"name\": \"\\u98de\\u673a\",
\"type\": \"2\",
\"pc\": 100,
\"gx\": 1000,
\"desc\": \"\\u8d60\\u9001\\u623f\\u95f4\\u5e7f\\u64ad\",
\"intro\": \"\\u4e3b\\u64ad\\u5e26\\u6211\\u98de\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/76dd0d8c803b133467e4f0196c7d071d.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/93daef170894a9d6bd8495fa0f81e165.gif\"
},
{
\"id\": \"194\",
\"name\": \"666\",
\"type\": \"2\",
\"pc\": 6,
\"gx\": 60,
\"desc\": \"\",
\"intro\": \"\\u4e3b\\u64ad\\u8fd9\\u4e486\\uff0c\\u4f60\\u9ebb\\u9ebb\\u77e5\\u9053\\u5417\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/9c30da343dc3b1e65156ae0570603d06.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/6681d1a713ece37912a1dac339c9fb29.gif\"
},
{
\"id\": \"193\",
\"name\": \"\\u5f31\\u9e21\",
\"type\": \"2\",
\"pc\": 0.2,
\"gx\": 2,
\"desc\": \"\",
\"intro\": \"\\u771f\\u662f\\u592a\\u9db8\\u62c9~\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/3cecdd879dc9e9ec5f0c77099201ca94.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/e144b05fd7dec1b194909e02b6dff92f.gif\"
},
{
\"id\": \"192\",
\"name\": \"\\u8d5e\",
\"type\": \"2\",
\"pc\": 0.1,
\"gx\": 1,
\"desc\": \"\",
\"intro\": \"\\u8d5e\\u4e00\\u4e2a\\uff01\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/d1ebe78032472a3146b5f8e717ead680.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/b09488ff5ebe85c4f66e72e27b53d517.gif\"
},
{
\"id\": \"191\",
\"name\": \"100\\u9c7c\\u4e38\",
\"type\": \"1\",
\"pc\": 100,
\"gx\": 1,
\"desc\": \"\",
\"intro\": \"\\u7edd\\u4f73\\u7684\\u589e\\u91cd\\u7f8e\\u98df\",
\"mimg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/37a913eb1d9b3e8e6278d8a309696d58.png\",
\"himg\": \"http: \\/\\/staticlive.douyutv.com\\/upload\\/dygift\\/1606\\/072dc9aea67fb42f095f973c8e0a183e.png\"
}
]
}
}"
*/