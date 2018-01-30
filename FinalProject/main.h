#pragma once

#include "PathWay.h"
#include <iostream>
#include <ctime>

const uint16_t SIZE = 26;
const uint16_t N = 4;

const char* days_of_week[] = {
	"Понедельник",
	"Вторник",
	"Среда",
	"Четверг",
	"Пятница",
	"Суббота",
	"Воскресенье",
};

const char* menu_points[] = {
	"Меню:",
	"Администратор.",
	"Пользователь.",
	"Выход."
};

enum {
	ADMIN = 1,
	USER,
	EXIT
};

void pass() {
	std::cin.clear();
	while (std::cin.get() != '\n');
}

uint16_t menu() {
	uint16_t ch = 0;

	system("cls");

	std::cout << menu_points[0] << std::endl;
	for (int16_t i = 1; i < N; i++) {
		std::cout << i << ". " << menu_points[i] << std::endl;
	}
	std::cout << "Ваш выбор: ";
	std::cin >> ch;

	return ch;
};

tm* setDayStart(tm* timeinfo) {
	timeinfo->tm_sec = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_hour = 0;

	return timeinfo;
}

uint16_t getDayWeek(const tm* timeinfo) {
	return (timeinfo->tm_wday == 0) ? 6 : timeinfo->tm_wday - 1;
}

bool compareDays(const tm* timeinfo, const std::vector<uint16_t>& days) {
	for (int16_t i = 0; i < days.size(); i++) {
		if (getDayWeek(timeinfo) == days[i]) {
			return true;
		}
	}

	return false;
}

std::string stringDate(const tm* tm_time) {
	char buf[40] = "";
	std::string date;

	size_t len = strftime(buf, 40, "%d.%m.%Y %H:%M:%S, ", tm_time);
	uint16_t daynum = (tm_time->tm_wday == 0) ? 6 : tm_time->tm_wday - 1;

	date += buf;
	date += days_of_week[daynum];

	return std::move(date);
}