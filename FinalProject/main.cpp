#define _CRT_SECURE_NO_WARNINGS

#include "BusCompany.h"
#include "GrafRoute.h"
#include "PathWay.h"
#include "Admin.h"
#include "User.h"
#include "main.h"
#include <clocale>
#include <string>

int main() {
	setlocale(LC_ALL, "Russian");

	BusCompany greyHound;

	uint16_t ch = 0;
	while ((ch = menu()) != EXIT) {
		switch (ch) {
		case ADMIN:
			pass();
			admin::admin_action(greyHound);
			break;

		case USER:
			user::user_action(greyHound);
			break;

		default:
			std::cout << "Неверный ввод." << std::endl;
			break;
		}
		pass();
		std::cout << "Нажмите любую клавишу.";
		std::cin.get();
	}
	
	return 0;
}