#pragma once

#include <string>

class Recipient {
	struct Ride {
		std::vector<uint16_t> routs;
		time_t departure = 0;
		uint16_t tikets = 0;

		Ride() = default;
		Ride(std::vector<uint16_t> _routs, time_t _departure, uint16_t _tikets) :
			routs(std::move(_routs)), departure(_departure), tikets(_tikets) {};
	};
	
	std::string login;
	std::string password;

	std::vector<Ride> rides;

public:
	Recipient() = default;
	Recipient(const std::string& _login, const std::string& _password) : login(_login), password(_password) {};

	~Recipient();

	void readRecipient(std::istream& in);
	void addRide(std::vector<uint16_t> rts, time_t dep, uint16_t tik);

	uint16_t getRidesNum() const;
	const std::vector<uint16_t>& getRoutPoints(uint16_t num) const;
	time_t getRouteTime(uint16_t num) const;
	uint16_t getRouteTikets(uint16_t num) const;
	std::string getLogin() const;
};

//Геттеры----------------------------------------------------------------------------------------------------------
std::string Recipient::getLogin() const {
	return login;
}

uint16_t Recipient::getRouteTikets(uint16_t num) const {
	return rides[num].tikets;
}

time_t Recipient::getRouteTime(uint16_t num) const {
	return rides[num].departure;
}

const std::vector<uint16_t>& Recipient::getRoutPoints(uint16_t num) const {
	return rides[num].routs;
}

uint16_t Recipient::getRidesNum() const {
	return rides.size();
}
//-----------------------------------------------------------------------------------------------------------------

//Публичные метода-------------------------------------------------------------------------------------------------
void Recipient::addRide(std::vector<uint16_t> rts, time_t dep, uint16_t tik) {
	Ride temp(std::move(rts), dep, tik);
	rides.push_back(temp);
}

void Recipient::readRecipient(std::istream& in) {
	uint16_t len = 0;
	in.read((char*)&len, sizeof(len));

	if (!in.eof()) {
		for (uint16_t i = 0; i < len; i++) {
			Ride temp;
			uint16_t routslen = 0;
			in.read((char*)&routslen, sizeof(routslen));
			for (uint16_t j = 0; j < routslen; j++) {
				uint16_t tmp = 0;
				in.read((char*)&tmp, sizeof(tmp));
				temp.routs.push_back(tmp);
			}

			in.read((char*)&temp.departure, sizeof(temp.departure));
			in.read((char*)&temp.tikets, sizeof(temp.tikets));

			rides.push_back(temp);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------

//Конструктор/деструктор-------------------------------------------------------------------------------------------
Recipient::~Recipient() {
	std::string lg = login;
	lg += ".txt";
	std::ofstream ofl(lg.data(), std::ios::out | std::ios::binary);

	uint16_t len = password.size();
	ofl.write((char*)&len, sizeof(len));
	ofl.write(password.data(), len);

	len = rides.size();
	ofl.write((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < rides.size(); i++) {
		len = rides[i].routs.size();
		ofl.write((char*)&len, sizeof(len));
		for (uint16_t j = 0; j < rides[i].routs.size(); j++) {
			ofl.write((char*)&rides[i].routs[j], sizeof(rides[i].routs[j]));
		}

		ofl.write((char*)&rides[i].departure, sizeof(rides[i].departure));
		ofl.write((char*)&rides[i].tikets, sizeof(rides[i].tikets));
	}

	ofl.close();
}
//-----------------------------------------------------------------------------------------------------------------