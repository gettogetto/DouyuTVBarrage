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
QString JsonMultiNestedObject(const QJsonValue& value, const std::vector<QString>&keyVector, int start);

QString JsonParse(const QByteArray& byte_array, const QString& key);
