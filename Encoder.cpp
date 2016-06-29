#include "Encoder.h"



Encoder::Encoder()
{
}


Encoder::~Encoder()
{
}

std::string Encoder::get_res() {
	return m_res;
}
//–Ú¡–ªØ
void Encoder::add_string_item(const std::string& key, const std::string &value) {
	std::string::size_type len = key.length();
	if (len == 0) return;
	std::string::size_type i = 0;

	while (i < len) {
		if (key[i] == '/') {
			m_res.append( "@S",2);
		}
		else if (key[i]=='@') {

			m_res.append("@A", 2);
		}
		else {
			m_res += key[i];
		}

		i++;
	}

	m_res.append("@=", 2);
	len = value.length();
	i = 0;

	while (i < len) {
		if (value[i] == '/') {
			m_res.append("@S", 2);
		}
		else if (value[i] == '@') {
			m_res.append("@A", 2);
		}
		else {
			m_res += value[i];
		}

		i++;
	}

	m_res.append("/", 1);
}

void Encoder::add_int_item(const std::string& key, const int value) {
	std::string string_value = std::to_string(value);

	add_string_item(key, string_value);
}