
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct item
{
	char name[10];
	int AP;
	int AS;
	int weight;
	int durability;
	int armorpoints; // od XP-levelu 5
	int speed; // 1 .. 10
	int fire_res; // procentowo
	int arrow_res;
	int mag_boost;
	int lvl;
    int mana;
	int health_reg;
	int luck_modifier;
	int szerokosc;
};
struct Slot {
	//std::string item_name;  // Nazwa przedmiotu
	char item_name[10];
	bool isEmpty;           // Sprawdzenie, czy slot jest pusty
	item* wpn;            // Wskaźnik na broń
	item* arm;             // Wskaźnik na zbroję
	item* rg;               // Wskaźnik na pierścień

	Slot() : isEmpty(true), wpn(nullptr), arm(nullptr), rg(nullptr) {} // Konstruktor domyślny
};

struct postac {
	char name;
	int lvl;
	int health;
	int attack;
	int pojemnosc;
	int xp;
	int stamina;
	int hunger;
	int mana;
	int posx;
	int posy;
	float speed;
	int luck;
	int points;
	Slot right_hand;   // Slot na broń w prawej ręce
	Slot left_hand;    // Slot na broń w lewej ręce
	Slot armor_slot;   // Slot na zbroję
	Slot ring_slot;    // Slot na pierścień
	item* ekwipunek;
	item** backpack;
	int backpack_height;
	int backpack_width;
	};

postac* generate_postac() {
	postac* post = (postac*)malloc(sizeof postac);
	post->backpack_height = 10;
	post->backpack_width = 12;

	post->posx = 1;
	post->posy = 1;

	post->lvl = 1;
	post->attack = rand() % 10 + 1;
	post->health = rand() % 10 + 1;
	post->stamina = rand() % 15 + 6;
	
	int size = 100;
	
	post->ekwipunek = (item*)malloc(sizeof(item) * size);

	post->backpack = (item**)malloc(sizeof(item*) * post->backpack_height);
	for (int i = 0; i < post->backpack_height; i++) {
		post->backpack[i] = (item*)malloc(sizeof(item) * post->backpack_width);
	}


	

	return post;
}

item* generate_ring(char nazwa) {

	
	 
	item* rg = (item*)malloc(sizeof item);

	rg->name[0] = 'a';
	rg->mana = rand() % 20;
	rg->health_reg = rand() % 4 + 1;
	rg->szerokosc = 1;
	
	if (rand() % 10 + 1 >= 5) {
		rg->luck_modifier = rand() % 200 + 1;
	};
	if (rand() % 10 + 1 >= 5) {
		rg->fire_res = rand() % 3 + 1;
	};

	return rg;
}

item* generate_weapon() {
	item* wpn = (item*)malloc(sizeof item);
	
	wpn->AP = rand() % 10 + 1;
	wpn->AS = rand() % 15 + 1;
	wpn->weight = rand() % 60 + 1;
	wpn->durability = rand() % 100 + 1;
	wpn->szerokosc = rand() % 3 + 1;

	wpn->armorpoints = 0;
	wpn->speed = 9;
	wpn->fire_res = 3;

	wpn->arrow_res = 0;
	wpn->mag_boost = 0;
	wpn->lvl = 1;
	wpn->mana = 5;
	wpn->health_reg = 0;
	wpn->luck_modifier = 1;


	return wpn;
}

item* generate_armor() {
	item* armr = (item*)malloc(sizeof item);

	armr->durability = rand() % 100+1;
	armr->armorpoints = rand() % 50 + 1;;
	armr->szerokosc = rand() % 5 + 1;
	return armr;


}
int uderzenie(postac& post, item& wpn, item& armr) {
	int real_hit_point = (post.attack + 2 * wpn.AP) - armr.armorpoints;

	return real_hit_point > 0 ? real_hit_point : 0;

}

void generate_map() {

	int mapx_size = 20;
	int mapy_size = 10;

	char **mapa = (char**)malloc(sizeof(char*) * mapx_size);
	for (int i = 0; i < mapy_size; i++) {
		mapa[i] = (char*)malloc(sizeof(char) * mapy_size);
		}

	for (int i = 0; i < mapx_size; i++) {
		for (int j = 0; j < mapy_size; j++) {
			int sciana_rand = rand() % 100 + 1;

			int trap_rand = rand() % 100 + 1;
			int enemy_rand = rand() % 100 + 1;

			if (sciana_rand < 7) {
				mapa[i][j] = 'r';
			}
			else if (trap_rand < 3) {
				mapa[i][j] = 'T';
			}
			else if (enemy_rand < 2) {
				mapa[i][j] = 'E';
			}
			else {
				mapa[i][j] = '.'; // empty tile or floor
			}

			}
	}


	

	

}

float equip(postac* postac1, item* new_ring = NULL) {

	return 0;
}








int main()
{
	srand(time(NULL));

	generate_map();
	

		

	postac* postac1 = generate_postac();
	std::cout << "Generated postac with hp: " << postac1->health << ", attack: " << postac1->attack << "\n";




	char nazwa;
	//std::cin >> nazwa;
	item* pierscien22 = generate_ring('n');
	printf("tu dziala");
	postac1->backpack[0][0] = *pierscien22;
	printf("Zawartosc plecaka: %s", postac1->backpack[0][0].name);
	

	 //Generate a random weapon
	
	item* miecz1 = generate_weapon();
	//std::cout << "Random weapon generated with AP: " << miecz1.AP << ", AS: " << miecz1.AS << "\n";


	// Generate random armor
	item* zbroja1 = generate_armor();
	//std::cout << "Random armor generated with Armor Points: " << zbroja1.armorpoints << "\n";

	item* pierscien = generate_ring('f');
	std::cout << "Mana regen :" << pierscien->health_reg << "\n";

	postac1->right_hand.wpn = miecz1;
	postac1->armor_slot.arm = zbroja1;
	postac1->ring_slot.rg = pierscien;

	
	item* spareSword = generate_weapon();
	postac1->ekwipunek = spareSword;
	
	//printf("Zawartosc plecaka:",);
	

	printf ( "Szerokosc zbroi : %d oraz miecza %d", zbroja1->szerokosc, miecz1->szerokosc);

	

	



	/*int damage = uderzenie(postac1, miecz1, zbroja1);
	postac1.health = postac1.health - damage;
	std::cout << "Damage dealt: " << damage << "\n  Healt left:" << postac1.health << "\n";
 */

	free(postac1->ekwipunek);
	
	free(pierscien);
	free(miecz1);
	
	for (int i = 0; i < postac1->backpack_height; i++) {
		free (postac1->backpack[i]) ;
	}

	return 0;
}

