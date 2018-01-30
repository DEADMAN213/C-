#pragma once

#include "Admin.h"
#include "Recipient.h"
#include <cctype>

namespace user {
	const uint16_t N = 4;

	const char* menu_points[] = {
		"Меню:",
		"Просмотреть билеты.",
		"Купить билеты.",
		"Выход."
	};

	enum {
		VIEW = 1,
		BUY,
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

	

	void user_action(BusCompany& grey) {
		pass();

		std::string login;
		std::string password;
		
		std::cout << "Введите логин: ";
		std::getline(std::cin, login);

		std::cout << "Введите пароль: ";
		std::getline(std::cin, password);

		Recipient client(login, password);
		login += ".txt";
		std::ifstream ifl(login.data(), std::ios::out | std::ios::binary);
		
		uint16_t len = 0;
		ifl.read((char*)&len, sizeof(len));

		if (!ifl.fail()) {
			std::string passconfirm(len, ' ');
			ifl.read((char*)&passconfirm[0], len);

			if (password != passconfirm) {
				std::cout << "Неверный пароль." << std::endl;
				return;
			}
		}

		client.readRecipient(ifl);

		uint16_t ch = 0;
		while ((ch = menu()) != EXIT) {
			switch (ch) {
			case VIEW: {
				uint16_t num = client.getRidesNum();
				const std::vector<std::string>& citys = grey.getCitys();
				for (int16_t i = 0; i < num; i++) {
					std::cout << "Маршрут: ";
					const std::vector<uint16_t>& rt = client.getRoutPoints(i);
					std::cout << citys[rt[0]];
					for (uint16_t j = 1; j < rt.size(); j++) {
						std::cout << " - " << citys[j];
					}
					std::cout << std::endl;

					std::cout << "Время в пути: ";
					time_t time = client.getRouteTime(i);
					tm* temp = localtime(&time);
					std::pair<uint16_t, uint16_t> mh = std::move(grey.getTime(grey.getRouts()[num - 1].getRoutePoints()));
					std::cout << stringDate(temp) << " - ";
					temp->tm_min += mh.second;
					temp->tm_hour += mh.first;
					mktime(temp);
					std::cout << stringDate(temp) << std::endl;

					std::cout << "Количество билетов: " << client.getRouteTikets(i) << std::endl;

				}
				std::cout << std::endl;
			}
				break;

			case BUY: {
				pass();
				admin::print(grey);

				uint16_t num = 0;
				std::cout << "Введите номер маршрута: ";
				std::cin >> num;

				system("cls");
				
				std::vector<time_t> dates;
				time_t now = time(NULL);
				struct tm * timeinfo = localtime(&now);

				for (uint16_t i = 0; i < 7; i++) {
					setDayStart(timeinfo);

					for (uint16_t j = 0; j < grey.getRouts()[num - 1].getTime().size(); j++) {
						timeinfo->tm_min = grey.getRouts()[num - 1].getTime()[j].second;
						timeinfo->tm_hour = grey.getRouts()[num - 1].getTime()[j].first;
						time_t time_temp = mktime(timeinfo);
						if (time_temp > now && compareDays(timeinfo, grey.getRouts()[num - 1].getWeekDays())) {
							dates.push_back(time_temp);
						}
					}
					
					setDayStart(timeinfo);
					timeinfo->tm_mday += 1;
					mktime(timeinfo);
				}

				admin::printRout(grey, grey.getCitys(), grey.getRouts()[num - 1]);
				std::cout << "Дата и время отправления и прибытия: " << std::endl;
				for (int16_t i = 0; i < dates.size(); i++) {
					tm* temp = localtime(&dates[i]);
					std::pair<uint16_t, uint16_t> mh = std::move(grey.getTime(grey.getRouts()[num - 1].getRoutePoints()));

					std::cout << i + 1 << ". " << stringDate(temp) << " - ";
					temp->tm_min += mh.second;
					temp->tm_hour += mh.first;
					mktime(temp);
					std::cout << stringDate(temp) << " - " << grey.getRouts()[num - 1].getTickets(dates[i]) << " билетов.";
					std::cout << std::endl;
				}

				std::cout << "Выберте удобную дату: ";
				uint16_t datenum = 0;
				std::cin >> datenum;

				std::cout << "Введите количество билетов: ";
				uint16_t ticketsnum;
				std::cin >> ticketsnum;

				if (grey.addTransaction(client.getLogin(), dates[datenum - 1], ticketsnum, num)) {
					client.addRide(grey.getRouts()[num - 1].getRoutePoints(), dates[datenum - 1], ticketsnum);
					std::cout << "Транзакция прошла успешно." << std::endl;
				}
				else {
					std::cout << "Транзакция невозможна." << std::endl;
				}
			}
				break;

			default:
				std::cout << "Неверный ввод." << std::endl;
				break;
			}
			pass();
			std::cout << "Нажмите любую клавишу." << std::endl;
			std::cin.get();
		}
	}
}