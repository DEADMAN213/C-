#pragma once

#include "PathWay.h"
#include <cctype>
#include <vector>
#include <list>

class GrafRoute {
	PathWay** graf = nullptr;
	uint16_t num = 0;

public:
	GrafRoute() = default;
	GrafRoute(uint16_t _num);

	~GrafRoute();

	void readRoute(std::istream& in);
	void writeRoute(std::ostream& out);

	std::vector<std::vector<uint16_t>> grafPaths(const uint16_t start, const uint16_t end) const;

	GrafRoute& operator=(const GrafRoute& right);

	void print(const std::vector<std::string>& from) const;

	double getCost(const std::vector<uint16_t>& rout) const;
	std::pair<uint16_t, uint16_t> getRouteTime(const std::vector<uint16_t>& rout) const;

private:
	bool isVisited(std::vector<uint16_t> visitedNodes[], uint16_t node, uint16_t candidat) const;
	void delGraf();
	void newGraf();
};

//Приватные члены---------------------------------------------------------------------------------------------------
void GrafRoute::newGraf() {
	graf = new PathWay*[num];

	for (uint16_t i = 0; i < num; i++) {
		graf[i] = new PathWay[num];
	}
}

void GrafRoute::delGraf() {
	for (uint16_t i = 0; i < num; i++) {
		delete[] graf[i];
	}
	delete[] graf;
}

bool GrafRoute::isVisited(std::vector<uint16_t> visitedNodes[], uint16_t node, uint16_t candidat) const {
	for (uint16_t i = 0; i < visitedNodes[node].size(); i++) {
		if (candidat == visitedNodes[node][i]) {
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------------

//Публичные члены---------------------------------------------------------------------------------------------------
std::pair<uint16_t, uint16_t> GrafRoute::getRouteTime(const std::vector<uint16_t>& rout) const {
	std::pair<uint16_t, uint16_t> temp(std::make_pair(0, 0));

	for (int16_t i = 0; i < rout.size() - 1; i++) {
		temp.first += graf[rout[i]][rout[i + 1]].getHours();
		temp.second += graf[rout[i]][rout[i + 1]].getMinutes();
	}

	return std::move(temp);
}

double GrafRoute::getCost(const std::vector<uint16_t>& rout) const {
	double cost = 0.0;

	for (size_t i = 0; i < rout.size() - 1; i++) {
		cost += graf[rout[i]][rout[i + 1]].getCost();
	}

	return cost;
}

void GrafRoute::print(const std::vector<std::string>& from) const {
	for (int16_t i = 0; i < num; i++) {
		for (int16_t j = 0; j < num; j++) {
			if (graf[i][j].getHours() > 0) {
				std::cout << from[i].data() << " - " << from[j].data() << std::endl;
				graf[i][j].print();
				std::cout << std::endl;
			}
		}
	}
}

GrafRoute& GrafRoute::operator=(const GrafRoute& right) {
	delGraf();
	num = right.num;
	newGraf();

	for (int16_t i = 0; i < num; i++) {
		for (int16_t j = 0; j < num; j++) {
			graf[i][j] = right.graf[i][j];
		}
	}

	return *this;
}

std::vector<std::vector<uint16_t>> GrafRoute::grafPaths(const uint16_t start, const uint16_t end) const {
	std::vector<std::vector<uint16_t>> paths;
	std::vector<uint16_t> stack;
	std::vector<uint16_t>* visitedNodes = new std::vector<uint16_t>[num];

	bool add = true;
	bool dell = false;

	stack.push_back(start);

	while (!stack.empty()) {
		uint16_t top = stack.back();

		if (top == end || !add) {
			if (top == end) {
				paths.push_back(stack);
			}

			stack.pop_back();
			dell = true;
		}
		add = false;

		for (unsigned int i = 0; i < num; i++) {
			if (!stack.empty() && graf[stack.back()][i].getHours() && (visitedNodes[i].size() == 0) && !isVisited(visitedNodes, stack.back(), i)) {
				visitedNodes[stack.back()].push_back(i);
				stack.push_back(i);
				add = true;
				break;
			}
		}

		if (dell && !add || dell) {
			dell = false;
			visitedNodes[top].clear();
		}
	}

	return std::move(paths);
}
//------------------------------------------------------------------------------------------------------------------

//чтение/запись в файл----------------------------------------------------------------------------------------------
void GrafRoute::readRoute(std::istream& in) {
	for (uint16_t i = 0; i < num; i++) {
		for (uint16_t j = 0; j < num; j++) {
			graf[i][j].readPath(in);
		}
	}
}

void GrafRoute::writeRoute(std::ostream& out) {
	for (uint16_t i = 0; i < num; i++) {
		for (uint16_t j = 0; j < num; j++) {
			graf[i][j].writePath(out);
		}
	}
}
//------------------------------------------------------------------------------------------------------------------

//конструктор/деструктор--------------------------------------------------------------------------------------------
GrafRoute::GrafRoute(uint16_t _num) : num(_num) {
	newGraf();
}

GrafRoute::~GrafRoute() {
	delGraf();
}
//------------------------------------------------------------------------------------------------------------------