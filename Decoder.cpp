#include "Decoder.h"



Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}
//·´ÐòÁÐ»¯
void Decoder::decode(const std::string& str) {
	std::string::size_type len = str.length();
	if (len == 0) return;

	m_array.clear();

	std::pair<std::string, std::string> key_value;
	std::string buf;
	std::string::size_type i = 0;
	while (i < len) {
		if (str[i] == '/') {
			key_value.second = buf;
			buf.clear();//£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡cost me a lot of time to debug
			m_array.push_back(key_value);

			key_value.first.clear();
			key_value.second.clear();
		}
		else if (str[i] == '@') {
			i++;
			if (str[i] == 'A') {
				buf += '@';
			}
			else if (str[i] == 'S') {
				buf += '/';
			}
			else if (str[i] == '=') {
				key_value.first = buf;
				buf.clear();
			}
		}
		else {
			buf += str[i];
		}
		i++;
	}
	if (i == len&&str[i - 1] != '/') {
		key_value.second = buf;
		buf.clear();
		m_array.push_back(key_value);
	}
}

std::string Decoder::get_string_item(const std::string &key) {
	std::string value;
	for (size_t i = 0; i < m_array.size(); i++) {
		if (m_array[i].first == key) {
			value = m_array[i].second;
			break;
		}
	}
	return value;
}

int Decoder::get_int_item(const std::string &key) {
	int value;
	for (auto item : m_array) {
		if (item.first == key) {
			value = atoi(item.second.c_str());
			break;
		}
	}
	return value;
}

