#include <random>
struct weapon {
	char name;
	int AP;
	int AS;
	int durability;
	int weight;
	int lvl;
};
struct armor
{
	char name;
	int weight;
	int durability;
	int armorpoints;
	int speed;
	int fire_res;
	int arrow_res;
	int mag_boost;
	int lvl;
};
struct ring
{
	char name;
	int mana;
	int fire_res;
	int durability;
	int health_reg;
	int luck_modifier;
};
struct postac {
	char name;
	ring first;
	ring second;
	int lvl;
	int health;
	int attack;
	int xp;
	int stamina;
	int hunger;
	int mana;
	int posx;
	int posy;
	float speed;
	int luck;
	int points;
	weapon right_hand;
	weapon left_hand;

};
postac generate_postac() {
	postac post;
	post.lvl = 1;
	



	return post;
}
ring generate_ring() {
	ring rg;

	int num = rand() % 10 + 1;
	rg.mana = num;

	num = rand() % 10 + 1;
	rg.health_reg = num;

	num = rand() % 1;
	if (num == 1) {
		num = rand() % 10 + 1;
		rg.luck_modifier = num;
	};


	return rg;
}
weapon generate_weapon() {
	weapon wpn;
	int num = rand() % 10 + 1;
	wpn.AP = num;

	num = rand() % 10 + 1;
	wpn.AS = num;

	num = rand() % 20 + 1;
	wpn.weight = num;

	num = rand() % 100 + 1;
	wpn.durability = num;

	return wpn;
}
armor generate_armor() {
	armor armr;

	int num = rand() % 100 + 1;
	armr.durability = num;

	num = rand() % 30 + 1;
	armr.armorpoints = num;

	num = rand() % 10 + 1;
	
	
	return armr;
}




#include <iostream>
//#include "Header.h"

int main()
{
    std::cout << "Hello World!\n";
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
