#pragma once
#include<qbytearray.h>
#include<qjsondocument.h>
#include<qjsonobject.h>
#include<qjsonarray.h>
#include<qjsonvalue.h>
#include<qstring.h>
#include<qvariant.h>
#include<qbytearray.h>
#include<vector>


//嵌套解析
QString JsonMultiNestedObject(const QJsonValue& value,const std::vector<QString>&keyVector,int start ) {
	QJsonObject obj = value.toObject();
	QString key = keyVector[start];
	if (obj.contains(key))
	{
		QJsonValue value2 = obj.take(key);
		if (value2.isObject()) {
			return JsonMultiNestedObject(value2, keyVector,start+1);
		}
		else if (value2.isString())
		{
			QString res = value2.toString();
			return res;
		}
		else if (value2.isDouble()) {
			QString res = QString(std::to_string(value2.toInt()).data());
			return res;
		}
	}
	return "unknown value";
}


//单重解析
QString JsonParse(const QByteArray& byte_array, const QString& key) {
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
	if (json_error.error == QJsonParseError::NoError)
	{
		
		if (parse_doucment.isObject())
		{
			QJsonObject obj = parse_doucment.object();
			
			if (obj.contains(key))
			{
				QJsonValue value = obj.take(key);
				if (value.isString())
				{
					QString res = value.toString();
					return res;
				}
				else if (value.isDouble()) {
					QString res = QString(std::to_string(value.toInt()).data());
					return res;
				}
				else if (value.isObject()) {
					
				}
			}
		}

	}
	return QString("unknown value");
}