#pragma once

#include "main.h"
#include <ctime>
#include <vector>
#include <utility>

class Route {
	struct userInfo {
		std::string name = "";
		time_t date = 0;
		uint16_t tickets = 0;

		userInfo() = default;
		userInfo(std::string& _name, time_t _date, uint16_t _tickets) :
			name(std::move(_name)), date(_date), tickets(_tickets) {};
	};

	std::vector<uint16_t> route_points;
	std::vector<uint16_t> week_days;
	std::vector<std::pair<uint16_t, uint16_t>> time;
	uint16_t tickets;

	std::vector<userInfo> users;

public:
	Route() = default;
	Route(std::vector<uint16_t> _route, std::vector<uint16_t> _days, std::vector<std::pair<uint16_t, uint16_t>> _time, uint16_t _tickets);

	const std::vector<uint16_t>& getRoutePoints() const;
	const std::vector<uint16_t>& getWeekDays() const;
	const std::vector<std::pair<uint16_t, uint16_t>>& getTime() const;
	uint16_t getTickets(time_t _date) const;

	void printClients() const;

	void readPath(std::istream& stream);
	void writePath(std::ostream& stream);

	void addUser(std::string& _name, time_t date, uint16_t tickets);

	bool isEmpty(time_t date, uint16_t tickets);
};

//Публичные методы-------------------------------------------------------------------------------------------------
void Route::printClients() const {
	for (int16_t i = 0; i < users.size(); i++) {
		std::cout << std::endl;

		std::cout << "Логин клиента: ";
		std::cout << users[i].name.data() << std::endl;

		std::cout << "Дата: ";
		std::cout << stringDate(localtime(&users[i].date)).data() << std::endl;

		std::cout << "Количество билетов: ";
		std::cout << users[i].tickets;

		std::cout << std::endl;
	}
}

bool Route::isEmpty(time_t date, uint16_t _tickets) {
	uint16_t tk = _tickets;
	for (int16_t i = 0; i < users.size(); i++) {
		if (users[i].date == date) {
			tk += users[i].tickets;
		}
	}

	if (tk <= tickets) {
		return false;
	}

	return true;
}

void Route::addUser(std::string& _name, time_t date, uint16_t tickets) {
	userInfo temp(std::move(_name), date, tickets);
	users.push_back(temp);
}

void Route::readPath(std::istream& stream) {
	uint16_t temp = 0;
	uint16_t len = 0;
	stream.read((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < len; i++) {
		stream.read((char*)&temp, sizeof(temp));
		route_points.push_back(temp);
	}

	stream.read((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < len; i++) {
		stream.read((char*)&temp, sizeof(temp));
		week_days.push_back(temp);
	}

	uint16_t first = 0;
	uint16_t second = 0;
	stream.read((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < len; i++) {
		stream.read((char*)&first, sizeof(first));
		stream.read((char*)&second, sizeof(second));
		time.push_back(std::make_pair(first, second));
	}

	stream.read((char*)&tickets, sizeof(tickets));

	stream.read((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < len; i++) {
		userInfo temp;
		uint16_t t = 0;
		stream.read((char*)&t, sizeof(t));
		temp.name.resize(t);
		stream.read((char*)&temp.name[0], t);

		stream.read((char*)&temp.date, sizeof(temp.date));
		stream.read((char*)&temp.tickets, sizeof(temp.tickets));

		users.push_back(temp);
	}
}

void Route::writePath(std::ostream& stream) {
	uint16_t len = route_points.size();
	stream.write((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < route_points.size(); i++) {
		stream.write((char*)&route_points[i], sizeof(route_points[i]));
	}

	len = week_days.size();
	stream.write((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < week_days.size(); i++) {
		stream.write((char*)&week_days[i], sizeof(week_days[i]));
	}

	len = time.size();
	stream.write((char*)&len, sizeof(len));
	for (uint16_t i = 0; i < time.size(); i++) {
		stream.write((char*)&time[i].first, sizeof(time[i].first));
		stream.write((char*)&time[i].second, sizeof(time[i].second));
	}

	stream.write((char*)&tickets, sizeof(tickets));

	len = users.size();
	stream.write((char*)&len, sizeof(len));
	for (int16_t i = 0; i < users.size(); i++) {
		len = users[i].name.size();
		stream.write((char*)&len, sizeof(len));
		stream.write((char*)&users[i].name[0], len);
		stream.write((char*)&users[i].date, sizeof(users[i].date));
		stream.write((char*)&users[i].tickets, sizeof(users[i].tickets));
	}
}
//-----------------------------------------------------------------------------------------------------------------

//Геттеры----------------------------------------------------------------------------------------------------------
uint16_t Route::getTickets(time_t _date) const {
	uint16_t tk = 0;
	for (int16_t i = 0; i < users.size(); i++) {
		if (users[i].date == _date) {
			tk += users[i].tickets;
		}
	}

	return tickets - tk;
}

const std::vector<uint16_t>& Route::getRoutePoints() const {
	return route_points;
}

const std::vector<uint16_t>& Route::getWeekDays() const {
	return week_days;
}

const std::vector<std::pair<uint16_t, uint16_t>>& Route::getTime() const {
	return time;
}
//-----------------------------------------------------------------------------------------------------------------

//Конструктор/деструктор-------------------------------------------------------------------------------------------
Route::Route(std::vector<uint16_t> _route, std::vector<uint16_t> _days, std::vector<std::pair<uint16_t, uint16_t>> _time, uint16_t _tickets) {
	route_points = std::move(_route);
	week_days = std::move(_days);
	time = std::move(_time);
	tickets = _tickets;
}
//-----------------------------------------------------------------------------------------------------------------