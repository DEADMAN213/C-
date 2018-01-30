#pragma once

#include "GrafRoute.h"
#include "Route.h"
#include <fstream>

class BusCompany {
	uint16_t num = 0;
	GrafRoute graf;
	std::vector<std::string> citys;
	std::vector<Route> routs;

	std::string password;

public:
	BusCompany();

	~BusCompany();

	const std::vector<std::string>& getCitys() const;
	const std::vector<Route>& getRouts() const;
	double getRouteCost(const std::vector<uint16_t>& rout) const;
	std::pair<uint16_t, uint16_t> getTime(const std::vector<uint16_t>& rout) const;

	std::vector<std::vector<uint16_t>> numRouts(const uint16_t start, const uint16_t end) const;
	
	void print();
	void addRout(std::vector<uint16_t> route_points, std::vector<uint16_t> week_days, std::vector<std::pair<uint16_t, uint16_t>> time, uint16_t tk);
	
	bool dellRout(uint16_t pos);
	bool passwordConfirm(const std::string& ps) const;

	bool addTransaction(std::string _name, time_t _date, uint16_t _tickets, uint16_t ind);
};

//Публичные методы-------------------------------------------------------------------------------------------------
bool BusCompany::addTransaction(std::string _name, time_t _date, uint16_t _tickets, uint16_t ind) {
	if (!routs[ind - 1].isEmpty(_date, _tickets)) {
		routs[ind - 1].addUser(std::move(_name), _date, _tickets);
		return true;
	}

	return false;
}

bool BusCompany::passwordConfirm(const std::string& ps) const {
	if (ps == password) {
		return true;
	}

	return false;
}

bool BusCompany::dellRout(uint16_t pos) {
	if (pos > routs.size()) {
		return false;
	}

	routs.erase(routs.begin() + pos - 1);

	return true;
}

void BusCompany::addRout(std::vector<uint16_t> route_points, std::vector<uint16_t> week_days, std::vector<std::pair<uint16_t, uint16_t>> time, uint16_t tk) {
	Route temp(std::move(route_points), std::move(week_days), std::move(time), tk);
	routs.push_back(std::move(temp));
}


std::vector<std::vector<uint16_t>> BusCompany::numRouts(const uint16_t start, const uint16_t end) const {
	return std::move(graf.grafPaths(start, end));
}

void BusCompany::print() {
	for (int16_t i = 0; i < num; i++) {
		std::cout << citys[i].data() << std::endl;
	}

	graf.print(citys);
}
//-----------------------------------------------------------------------------------------------------------------

//Гетеры-----------------------------------------------------------------------------------------------------------
std::pair<uint16_t, uint16_t> BusCompany::getTime(const std::vector<uint16_t>& rout) const {
	return graf.getRouteTime(rout);
}

const std::vector<Route>& BusCompany::getRouts() const {
	return routs;
}

const std::vector<std::string>& BusCompany::getCitys() const {
	return citys;
}

double BusCompany::getRouteCost(const std::vector<uint16_t>& rout) const {
	return graf.getCost(rout);
}
//-----------------------------------------------------------------------------------------------------------------

//Конструктор/деструктор-------------------------------------------------------------------------------------------
BusCompany::~BusCompany() {
	std::ofstream ofl("base.txt", std::ios::out | std::ios::binary);
	
	uint16_t len = password.size();
	ofl.write((char*)&len, sizeof(len));
	ofl.write(password.data(), len);

	ofl.write((char*)&num, sizeof(num));
	for (uint16_t i = 0; i < num; i++) {
		len = citys[i].size();
		ofl.write((char*)&len, sizeof(len));
		ofl.write(citys[i].data(), len);
	}
	graf.writeRoute(ofl);

	len = routs.size();
	ofl.write((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < len; i++) {
		routs[i].writePath(ofl);
	}

	ofl.close();
}

BusCompany::BusCompany() {
	std::ifstream in("base.txt", std::ios::in | std::ios::binary);

	uint16_t len = 0;
	in.read((char*)&len, sizeof(len));
	password.resize(len);
	in.read((char*)&password[0], len);

	std::string str;
	in.read((char*)&num, sizeof(num));
	for (uint16_t i = 0; i < num; i++) {
		in.read((char*)&len, sizeof(len));
		str.resize(len);
		in.read((char*)&(str[0]), len);
		citys.push_back(str);
	}

	GrafRoute temp(num);
	temp.readRoute(in);
	graf = temp;

	in.read((char*)&len, sizeof(len));
	if (!in.eof()) {
		for (uint16_t i = 0; i < len; i++) {
			Route rt;
			rt.readPath(in);
			routs.push_back(rt);
		}
	}

	in.close();
}
//-----------------------------------------------------------------------------------------------------------------