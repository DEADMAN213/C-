#pragma once

#include "main.h"
#include <cctype>
#include <string>

namespace admin {

	const uint16_t N = 5;

	const char* menu_points[] = {
		"Меню:",
		"Просматреть маршруты.",
		"Добавить маршрут.",
		"Удалить маршрут.",
		"Выход."
	};

	enum {
		VIEW = 1,
		ADD,
		DELL,
		EXIT
	};

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

	void printRout(const BusCompany& grey, const std::vector<std::string>& citys, const Route& route) {
		std::cout << "Маршрут: ";
		std::cout << citys[route.getRoutePoints()[0]].data();
		for (size_t j = 1; j < route.getRoutePoints().size(); j++) {
			std::cout << " - " << citys[route.getRoutePoints()[j]].data();
		}
		std::cout << std::endl;

		std::cout << "Стоимость: " << grey.getRouteCost(route.getRoutePoints()) << "$" << std::endl;

		std::cout << "Дни недели: ";
		std::cout << days_of_week[route.getWeekDays()[0]];
		for (size_t j = 1; j < route.getWeekDays().size(); j++) {
			std::cout << ", " << days_of_week[route.getWeekDays()[j]];
		}
		std::cout << std::endl;

		std::cout << "Время: ";
		std::cout << route.getTime()[0].first << ":";
		route.getTime()[0].second ? std::cout << route.getTime()[0].second : std::cout << "00";
		for (size_t j = 1; j < route.getTime().size(); j++) {
			std::cout << ", " << route.getTime()[j].first << ":";
			route.getTime()[j].second ? std::cout << route.getTime()[j].second : std::cout << "00";
		}
		std::cout << std::endl << std::endl;


	}

	void print(const BusCompany& grey) {
		const std::vector<std::string>& citys = grey.getCitys();
		const std::vector<Route>& routs = grey.getRouts();

		for (size_t i = 0; i < routs.size(); i++) {
			std::cout << "#" << i + 1 << ' ';
			printRout(grey, grey.getCitys(), grey.getRouts()[i]);
		}
	}

	void admin_action(BusCompany& grey) {
		uint16_t ch = 0;

		std::string passw;
		std::cout << "Введите пароль: ";
		std::getline(std::cin, passw);

		if (!grey.passwordConfirm(passw)) {
			std::cout << "Неверный пароль. Нажмите 'Ввод'." << std::endl;
			std::cin.get();
			return;
		}

		while ((ch = menu()) != EXIT) {
			switch (ch) {
			case VIEW: {
				print(grey);

				uint16_t num = 0;
				std::cout << "Введите номер маршрута: ";
				std::cin >> num;

				grey.getRouts()[num - 1].printClients();
			}
				break;

			case ADD: {
				std::vector<uint16_t> week_days;
				std::vector<std::pair<uint16_t, uint16_t>> time;
				const std::vector<std::string>& citys = grey.getCitys();
				
				std::cout << "Города:" << std::endl;
				for (size_t i = 0; i < citys.size(); i++) {
					std::cout << i + 1 << ". " << citys[i].data() << std::endl;
				}

				int16_t start = 0;
				int16_t end = 0;
				int16_t num_rout = 0;
				std::cout << "Введите номер точки отправления: ";
				std::cin >> start;
				std::cout << "Введите номер точки прибытия: ";
				std::cin >> end;

				std::cout << "Возможные маршруты: " << std::endl;
				std::vector<std::vector<uint16_t>> routs = std::move(grey.numRouts(start - 1, end - 1));
				for (size_t i = 0; i < routs.size(); i++) {
					std::cout << i + 1 << ". " << citys[routs[i][0]].data();
					for (size_t j = 1; j < routs[i].size(); j++) {
						std::cout << " - " << citys[routs[i][j]].data();
					}
					std::cout << std::endl;
					std::cout << "Стоимость маршрута: " << grey.getRouteCost(routs[i]) << "$" << std::endl << std::endl;
				}
				std::cout << "Введите номер маршрута: ";
				std::cin >> num_rout;

				int16_t num = 0;
				int16_t day = 0;
				std::cout << "Введите количество рейсов в неделю: ";
				std::cin >> num;
				for (int16_t i = 0; i < num; i++) {
					std::cout << "Введите день недели: ";
					std::cin >> day;
					week_days.push_back(day - 1);
				}

				int16_t hours = 0;
				int16_t minutes = 0;
				std::cout << "Введите количество рейсов в день: ";
				std::cin >> num;
				for (int16_t i = 0; i < num; i++) {
					std::cout << "Введите часы: ";
					std::cin >> hours;
					std::cout << "Введите минуты: ";
					std::cin >> minutes;

					time.push_back(std::make_pair(hours, minutes));
				}

				uint16_t tickets = 0;
				std::cout << "Введите количество билетов на рейс: ";
				std::cin >> tickets;
				grey.addRout(std::move(routs[num_rout - 1]), std::move(week_days), std::move(time), tickets);

			}
				break;

			case DELL: {
				print(grey);

				int16_t num = 0;
				std::cout << "Введите номер маршрута: ";
				std::cin >> num;

				if (grey.dellRout(num)) {
					std::cout << "Маршрут успешно удален." << std::endl;
				}
				else {
					std::cout << "Ошибка при удаленнии маршрута. Проверьте правильность ввода." << std::endl;
				}
			}
				break;

			default:
				//pass();
				std::cout << "Неверный ввод." << std::endl;
				break;
			}
			pass();
			std::cout << "Нажмите любую клавишу.";
			std::cin.get();
		}
	};
	
}