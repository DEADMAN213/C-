#pragma once

#include <cctype>
#include <iostream>

class PathWay {
	double cost = 0.0;
	uint16_t hours = 0;
	uint16_t minutes = 0;

public:
	PathWay() = default;
	PathWay(double _cost, uint16_t _hours, uint16_t _minutes) : cost(_cost), hours(_hours), minutes(_minutes) {};

	void print() const;

	double getCost() const;
	uint16_t getHours() const;
	uint16_t getMinutes() const;

	void setCost(double _cost);
	void setHours(uint16_t _hours);
	void setMinutes(uint16_t _minuts);

	void readPath(std::istream& stream);
	void writePath(std::ostream& stream);
};

//set-методы-------------------------------------------------------------------------------------------------------
void PathWay::setCost(double _cost) {
	cost = _cost;
}

void PathWay::setHours(uint16_t _hours) {
	hours = _hours;
}

void PathWay::setMinutes(uint16_t _minuts) {
	minutes = _minuts;
}
//-----------------------------------------------------------------------------------------------------------------

//get-методы-------------------------------------------------------------------------------------------------------
double PathWay::getCost() const {
	return cost;
}

uint16_t PathWay::getHours() const {
	return hours;
}

uint16_t PathWay::getMinutes() const {
	return minutes;
}
//-----------------------------------------------------------------------------------------------------------------

void PathWay::print() const {
	std::cout << "Стоимость: " << cost << std::endl;
	std::cout << "Время в пути: " << hours << " ч " << minutes << " м." << std::endl;
}

//ввод/вывод-------------------------------------------------------------------------------------------------------
void PathWay::readPath(std::istream& stream) {
	stream.read((char*)&cost, sizeof(cost));
	stream.read((char*)&hours, sizeof(hours));
	stream.read((char*)&minutes, sizeof(minutes));
}

void PathWay::writePath(std::ostream& stream) {
	stream.write((char*)&cost, sizeof(cost));
	stream.write((char*)&hours, sizeof(hours));
	stream.write((char*)&minutes, sizeof(minutes));
}
//-----------------------------------------------------------------------------------------------------------------