#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <windows.h>
#include <conio.h>

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
	char name[10];
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
	int effect; // 0 - normalny, 1- trucizna, 3 - głód 
	};


struct enemy {
	std::string name;
	int lvl;
	int health;
	int attack;
	int mana;
	int xpdrop;
	int AS; // Attack Speed
	int type;	//1 - normalny wrog, 2- boss
	int race; // 1 - goblin, 2 - zombie, 3 - ork, 4 - wizard
};


enemy* generate_enemy() {
	enemy* en = (enemy*)malloc(sizeof enemy);
	
	int rand_enemytype = rand() % 10 + 1;
	if (rand_enemytype > 6) {
		en->name = "Goblin";
		en->lvl = 1;
		en->health = 3;
		en->attack = 1;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 5;
		en->AS = 1;
		en->race = 1;	
			
	}
	else if (rand_enemytype > 3) {
		en->name = "Zombie";
		en->lvl = 1;
		en->health = 5;
		en->attack = 2;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 7;
		en->AS = 1;
		en->race = 2;
	}
	else if(rand_enemytype > 1) {
		en->name = "Ork";
		en->lvl = 2;
		en->health = 8;
		en->attack = 5;
		en->mana = 0;
		en->type = 2;
		en->xpdrop = 10;
		en->race = 3;

	}
	else if (rand_enemytype == 1) {
		en->name = "Wizard";
		en->lvl = 5;
		en->health = 5;
		en->attack = 8;
		en->mana = 10;
		en->type = 2;
		en->xpdrop = 25;
		en->AS = 4;
		en->race = 4;
	}
	
	return en;
}


postac* generate_postac(char nazwa[10]) {
	postac* post = (postac*)malloc(sizeof postac);
	post->backpack_height = 10;
	post->backpack_width = 12;

	strcpy_s(post->name, sizeof(post->name), nazwa);
	post->posx = 1;
	post->posy = 1;
	post->xp = 0;
	post->effect = 0;

	post->lvl = 1;
	post->attack = rand() % 10 + 1;
	post->health = rand() % 20 + 10;
	post->stamina = rand() % 15 + 6;
	post->speed = rand() % 12 + 3;
	
	int size = 100;
	
	post->ekwipunek = (item*)malloc(sizeof(item) * size);

	post->backpack = (item**)malloc(sizeof(item*) * post->backpack_height);
	for (int i = 0; i < post->backpack_height; i++) {
		post->backpack[i] = (item*)malloc(sizeof(item) * post->backpack_width);
	}

	return post;
}
//void walka(postac* postac1) {

//	printf("\n\n\t[WALKA Z WROGIEM!]\n\n");
//}

void ekran_walki(postac* postac1) {
	enemy* wrog = generate_enemy();
	//while(wrog->health > 0 && postac1->health >0) {
	system("cls");

	switch (wrog->race) {
	case 1:
		printf(
				"										 \n"
				"						ENEMY	|	HERO \n"
				"							|\n"				
				"	 	    ⣠⣶⣿⣿⣶⣄⠀⠀⠀⠀⠀	⠀⠀⠀⠀⠀	⠀⠀		|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀	\tHP: %d	|	%d\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢧⡀⠀⠀⠀⠀⠀⠀⠀		⠀	|\n"
				"	⠀⠢⣤⣀⡀⠀⠀⠀⢿⣧⣄⡉⠻⢿⣿⣿⡿⠟⢉⣠⣼⡿⠀⠀⠀⠀⣀⣤⠔⠀			|\n"
				"	⠀⠀⠈⢻⣿⣶⠀⣷⠀⠉⠛⠿⠶⡴⢿⡿⢦⠶⠿⠛⠉⠀⣾⠀⣶⣿⡟⠁⠀⠀			|\n"
				"	⠀⠀⠀⠀⠻⣿⡆⠘⡇⠘⠷⠠⠦⠀⣾⣷⠀⠴⠄⠾⠃⢸⠃⢰⣿⠟⠀⠀⠀⠀			|\n"
				"	⠀⠀⠀⠀⠀⠋⢠⣾⣥⣴⣶⣶⣆⠘⣿⣿⠃⣰⣶⣶⣦⣬⣷⡄⠙⠀⠀⠀⠀			|\n"
				"	⠀⠀⠀⠀⠀⠀⢋⠛⠻⠿⣿⠟⢹⣆⠸⠇⣰⡏⠻⣿⠿⠟⠛⡙⠀⠀⠀⠀⠀⠀			|\n"
				"	⠀⠀⠀⠀⠀⠀⠈⢧⡀⠠⠄⠀⠈⠛⠀⠀⠛⠁⠀⠠⠄⢀⡼⠁⠀⠀⠀⠀⠀		⠀	|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠈⢻⣦⡀⠃⠀⣿⡆⢰⣿⠀⠘⢀⣴⡟⠁⠀⠀⠀⠀⠀⠀			|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣦⡀⠘⠇⠸⠃⢀⣴⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀			|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣷⣄⣠⣾⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀		⠀	|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀		⠀	|\n"
			"        \n"
			"        \n"
			"        \n"
			"	 ____       _     _ _		⠀\n"
			"	/ ___ | ___ | |__ | (_)_ __	⠀\n"
			"	| |  _ / _ ⧵ | '_ ⧵| | | '_ ⧵	⠀\n"
			"	| |_| | (_) | |_) | | | | | |		⠀\n"
			"	⧵____ | ⧵___/|_.__/|_ | _ | _	⠀\n",wrog->health, postac1->health
		);
		break;
	case 2:
		printf(
			"	        \n"
			"									ENEMY	|	HERP \n"
			"										|\n"		
			"	⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠊⠉⠉⠉⠉⢉⠝⠉⠓⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⢀⡴⣋⠀⠀⣤⣒⡠⢀⠀⠐⠂⠀⠤⠤⠈⠓⢦⡀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⣰⢋⢬⠀⡄⣀⠤⠄⠀⠓⢧⠐⠥⢃⣴⠤⣤⠀⢀⡙⣆⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⢠⡣⢨⠁⡘⠉⠀⢀⣤⡀⠀⢸⠀⢀⡏⠑⠢⣈⠦⠃⠦⡘⡆⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⢸⡠⠊⠀⣇⠀⠀⢿⣿⠇⠀⡼⠀⢸⡀⠠⣶⡎⠳⣸⡠⠃⡇⠀⠀⠀			\t\t\t|\n"
			"	⢀⠔⠒⠢⢜⡆⡆⠀⢿⢦⣤⠖⠒⢂⣽⢁⢀⠸⣿⣦⡀⢀⡼⠁⠀⠀⡇⠒⠑⡆			\t\t\t|\n"
			"	⡇⠀⠐⠰⢦⠱⡤⠀⠈⠑⠪⢭⠩⠕⢁⣾⢸⣧⠙⡯⣿⠏⠠⡌⠁⡼⢣⠁⡜⠁			\t\t\t|\n"
			"	⠈⠉⠻⡜⠚⢀⡏⠢⢆⠀⠀⢠⡆⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⣼⠾⢬⣹⡾⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠉⠀⠉⠀⠀⠈⣇⠀⠀⠀⣴⡟⢣⣀⡔⡭⣳⡈⠃⣼⠀⠀⠀⣼⣧⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⣸⣿⣿⣿⡿⣷⣿⣿⣷⠀⡇⠀⠀⠀⠙⠊⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣠⠀⢻⠛⠭⢏⣑⣛⣙⣛⠏⠀⡇⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⠠⠜⠓⠉⠉⠀⠐⢒⡒⡍⠐⡇⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠒⠢⠤⣀⣀⣀⣀⣘⠧⠤⠞⠁⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"        \n"
			"        \n"
			"        \n"
			"	   _____               _     _			\n"
			"	   |__  /___  _ __ ___ | |__ (_) ___	\n"
			"	    / // _ ⧵| '_ ` _ ⧵| '_ ⧵| |/ _ ⧵	\n"
			"	   / /| (_) | | | | | | |_) | |  __/	\n"
			"	  /____⧵___/|_| |_| |_|_.__/|_|⧵___|	\n");
		
		break;
	case 3:
		printf(
			"	        \n"
			"									ENEMY	|	HERP \n"
			"										|\n"
			"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣧⣄⣉⣉⣠⣼⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 		    \t\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⡿⣿⣿⣿⣿⢿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⣼⣤⣤⣈⠙⠳⢄⣉⣋⡡⠞⠋⣁⣤⣤⣧⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⢲⣶⣤⣄⡀⢀⣿⣄⠙⠿⣿⣦⣤⡿⢿⣤⣴⣿⠿⠋⣠⣿⠀⢀⣠⣤⣶⡖⠀			\t\t\t|\n"
			"	⠀⠀⠙⣿⠛⠇⢸⣿⣿⡟⠀⡄⢉⠉⢀⡀⠉⡉⢠⠀⢻⣿⣿⡇⠸⠛⣿⠋⠀			\t\t\t|\n"
			"	⠀⠀⠀⠘⣷⠀⢸⡏⠻⣿⣤⣤⠂⣠⣿⣿⣄⠑⣤⣤⣿⠟⢹⡇⠀⣾⠃⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠘⠀⢸⣿⡀⢀⠙⠻⢦⣌⣉⣉⣡⡴⠟⠋⡀⢀⣿⡇⠀⠃⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⢸⣿⣧⠈⠛⠂⠀⠉⠛⠛⠉⠀⠐⠛⠁⣼⣿⡇⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠸⣏⠀⣤⡶⠖⠛⠋⠉⠉⠙⠛⠲⢶⣤⠀⣹⠇⠀⠀⠀⠀⠀	⠀		\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣶⣿⣿⣿⣿⣿⣿⣶⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠛⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	        \n"
			"	        \n"
			"	        \n"
			"		  ___       _				\n"
			"		 / _ ⧵ _ __| | __			\n"
			"		| | | | '__| |/ /			\n"
			"		| |_| | |  |    <			\n"
			"		 ⧵___/|_|  |_ |⧵_⧵			\n"
		);
		break;
	case 4:
		printf(
												
													
			"			     _,-'|				|\n"		
			"			 ,-' ._  |		ENEMY	|	HERP \n"	
			"		 . || ,   |####⧵ |			|\n"
			"		 ⧵.`', /  ⧵####| |			|\n"
			"	   	 =, . =   | ###| |			|\n"
			"		 / || ⧵ , -'⧵#/,'`.			|\n"
			"		   || ,  '   `,,. `.			|\n"
			"		  , | ____, ' , ,;' ⧵| |		|\n"
			"		 (3 | ⧵    _/|/'   _| |		\n"
			"		   ||/, -'' | >-'' _, ⧵⧵		\n"
			"	           || '      ==⧵ ,-', '		    \n"
			"		   ||       |  V ⧵, |		    \n"
			"		   ||       |    |` |		\n"
			"		   ||       |    |   ⧵		\n"
			"		   ||       |    ⧵    ⧵		\n"
			"		   ||       |     |    ⧵		\n"
			"		   ||       |      ⧵_,-'		\n"
			"		   ||       |___,,--')_⧵		\n"
			"		   ||         |_|   ccc/		\n"
			"		   ||        ccc/			\n"
			"		   ||						\n"
			"	      ⧵ ⧵      / (_)______ _ _ __ __| |\n"
			"	       ⧵ ⧵ /⧵ / /| |_  / _` | '__/ _` |\n"
			"		⧵ V  V / | |/ / (_| | | | (_| |	\n"
			"		 ⧵_/⧵_/  |_/___⧵__,_|_|  ⧵__,_| \n"

											);
		break;
	default:
		break;
	}
	Sleep(1000);
	//walka(postac1);
	//}

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
//int uderzenie(postac& post, item& wpn, item& armr) {
//	int real_hit_point = (post.attack + 2 * wpn.AP) - armr.armorpoints;
//
//	return real_hit_point > 0 ? real_hit_point : 0;//}


void pulapka(postac* postac1) {			//healt attack stamina speed
	int trap_rand = rand() % 100 + 1;
	int trap_type;
	int trap_damage;
	if (trap_rand < 26) {
		trap_type = 1;		//health trap

	}
	else if (trap_rand < 51) {
		trap_type = 2;		//attack trap
	}
	else if (trap_rand < 76) {
		trap_type = 3;		//stamina trap
	}
	else if (trap_rand < 101) {
		trap_type = 4;		//speed trap
	}
	switch (trap_type) {
	case 1:
		trap_damage = rand() % 3 + 1;
		postac1->health -= trap_damage;
		printf("\n\t[WPADLES W PULAPKE DAMAGE!]\n\n");
		printf("\t[PULAPKA ZABIERA CI %d HP]\n", trap_damage);
		break;
	case 2:
		trap_damage = rand() % 3 + 1;		
		postac1->attack -= trap_damage;
		printf("\n\t[WPADLES W PULAPKE REDUKCJI ATAKU!]\n\n");
		printf("\t[PULAPKA ZABIERA CI %d ATAKU]\n", trap_damage);
		break;
	case 3:
		trap_damage = rand() % 3 + 1;
		postac1->stamina -= trap_damage;
		printf("\n\t[WPADLES W PULAPKE REDUKCJI STAMINY!]\n\n");
		printf("\t[PULAPKA ZABIERA CI %d STAMINY]\n", trap_damage);
		break;
	case 4:
		trap_damage = rand() % 2 + 1;
		postac1->speed -= trap_damage;
		printf("\n\t[WPADLES W PULAPKE REDUKCJI PREDKOSCI!]\n\n");
		printf("\t[PULAPKA ZABIERA CI %d PREDKOSCI]\n", trap_damage);
		break;
	default:
		break;
	}

}
int trapcount = 0;
int enemycount = 0;
void generate_map(char** mapa, int mapx_size, int mapy_, postac* postac1) {
	
    for (int i = 0; i < mapx_size; i++) {
        mapa[i] = (char*)malloc(sizeof(char) * mapy_);
    }

    for (int i = 0; i < mapx_size; i++) {
        for (int j = 0; j < mapy_; j++) {
            int sciana_rand = rand() % 100 + 1;
            int trap_rand = rand() % 100 + 1;
            int enemy_rand = rand() % 100 + 1;

            if (sciana_rand < 7) {
                mapa[i][j] = '|';
            } else if (trap_rand < 3) {
                mapa[i][j] = 'T';
				trapcount++;
            } else if (enemy_rand < 2) {
                mapa[i][j] = 'E';
				enemycount++;
            } else {
                mapa[i][j] = '.';
            }
        }
    }
	int i = postac1->posx;
	int j = postac1->posy;
	if(mapa[i][j] == 'T') {
		trapcount--;
	}
	if (mapa[i][j] == 'E') {
		enemycount--;	
	}
	mapa[i][j] = 'P'; // postac na mapie
}

void show_map(char** mapa, int mapx_size, int mapy_size){
	printf("\n\n\ttraps: %d enemies: %d\n", trapcount, enemycount);
    printf("\n");
    // Górna ramka
    printf("\t");
    for (int j = 0; j < mapy_size + 2; j++) {
        printf("# ");
    }
    printf("\n");

    // Środek mapy z ramkami bocznymi
    for (int i = 0; i < mapx_size; i++) {
        printf("\t# "); // Lewa ramka
        for (int j = 0; j < mapy_size; j++) {
			if (mapa[i][j] == 'T') {
				printf(". "); // Pułapka ukryta 
			}
			else {
			printf("%c ", mapa[i][j]);
			}
            
        }
        printf("#\n"); // Prawa ramka
    }

    // Dolna ramka
    printf("\t");
    for (int j = 0; j < mapy_size + 2; j++) {
        printf("# ");
    }
    printf("\n");
}
void model_hero(postac* postac1) {					//model asci bedzie sie roznil w zaleznosci od broni
	printf(
		"		       {}			[IMIE] = %s\n"
		"		      .--.		    \t[LEVEL] = %d\n"
		"		     /.--.⧵			[HEALTH] = %d\n"
		"		    |= == =|		\t[ATTACK] = %d		\n"
		"		    | `::` |		\t[X POINTS] = %d	\n"
		"		  .-;`⧵..../ `;-.	\t[STAMINA] = %d	\n"
		"		  /  |...::... |  ⧵		\n"
		"		  |  /''': :'''⧵   |	\n"
		"		  ; --'⧵   ::  /⧵--;	\n"
		"	         <__>,>._::_. <,<__>	\n"
		"	       |  |/   ^^    ⧵|  |	\n"
		"	       ⧵::/|         |⧵::/	\n"
		"	       |||⧵|         |/|||	\n"
		"	       ''' |___/ ⧵___| '''	\n"
		"                   ⧵_ |  | _ /			\n"
		"	            <_ >< _>				\n"
		"	            |  ||  |					\n"
		"	            |  ||  |					\n"
		"	           _⧵.:||:./_			\n"
		"		  /____/ ⧵____⧵		\n\n\n"
		"_____________________________________________________________________________________________________________\n", postac1->name, postac1->lvl, postac1->health, postac1->attack, postac1->xp, postac1->stamina);
}
void pokaz_postac(postac* postac1) {
	system("cls");
	printf("\n"
		"\t _   _ _____ ____   ___\n"
		"\t| | | | ____ | _ ⧵ / _	⧵	  \n"
		"\t| |_| |  _|  ||_) | | | |\n"
		"\t|  _  | |___ | _ <| |_| |\n"
		"\t|_| |_| _____|_|⧵_⧵⧵___/ \n\n\n");

	model_hero(postac1);
}


void widok_glowny(int mapx_size, int mapy_size, char** mapa, postac* postac1, int runda) {
	
    printf("_____________________________________________________________________________________________________________\n");
    printf("\tRunda = %d LVL: %d\tHP: %d\tSTM: %d\tXP: %d\t AT: %d\t SPD: %.1f\t", runda, postac1->lvl, postac1->health, postac1->stamina, postac1->xp, postac1->attack, postac1->speed);
    show_map(mapa, mapx_size, mapy_size);
}

void krok(char* move, char** mapa, postac* postac1, int mapx_size, int mapy_size) {


			// Ruch w gore
	system("cls");
    if (move[0] == 'w') {
		
        // Sprawdzenie, czy nie wychodzimy poza górną krawędź mapy
        if (postac1->posx - 1 < 0) {
			printf("\n\t[WYCHODZISZ POZA MAPE!]\n\n");
            return;
        }
        if (mapa[postac1->posx - 1][postac1->posy] == '|') {
			printf("\n\t[WCHODZISZ W SCIANE!]\n\n");
			return;
        }
        if (mapa[postac1->posx - 1][postac1->posy] == 'T') {
			printf("\n\t[WPADASZ W PULAPKE!]\n\n");
			trapcount--;
			pulapka(postac1);
        }
		if (mapa[postac1->posx - 1][postac1->posy] == 'E') {
			printf("\n\t[WPADASZ NA WROGA!]\n\n");
			enemycount--;
			ekran_walki(postac1);
		}
            mapa[postac1->posx][postac1->posy] = '.';
            postac1->posx--;
            mapa[postac1->posx][postac1->posy] = 'P';
        
    }

	//		ruch w dol

	if (move[0] == 's') {
		
		// Sprawdzenie, czy nie wychodzimy poza górną krawędź mapy
		if (postac1->posx + 1 > mapx_size - 1) {
			printf("\n\t[WYCHODZISZ POZA MAPE!]\n\n");
			return;
		}
		if (mapa[postac1->posx + 1][postac1->posy] == '|') {
			printf("\n\t[WCHODZISZ W SCIANE!]\n\n");
			return;
		}
		if (mapa[postac1->posx + 1][postac1->posy] == 'T') {
			printf("\n\t[WPADASZ W PULAPKE!]\n\n");
			trapcount--;
			pulapka(postac1);
		}
		if (mapa[postac1->posx + 1][postac1->posy] == 'E') {
			printf("\n\t[WPADASZ NA WROGA!]\n\n");
			enemycount--;
			ekran_walki(postac1);
		}
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posx++;
		mapa[postac1->posx][postac1->posy] = 'P';

	}

	//		ruch w lewo

	if(move[0] == 'a') {
		
		// Sprawdzenie, czy nie wychodzimy poza górną krawędź mapy
		if (postac1->posy - 1 < 0) {
			printf("\n\t[WYCHODZISZ POZA MAPE!]\n\n");
			return;
		}
		if (mapa[postac1->posx][postac1->posy - 1] == '|') {
			printf("\n\t[WCHODZISZ W SCIANE!]\n\n");
			return;
		}
		if (mapa[postac1->posx][postac1->posy - 1] == 'T') {
			printf("\n\t[WPADASZ W PULAPKE!]\n\n");
			trapcount--;
			pulapka(postac1);
		}
		if (mapa[postac1->posx][postac1->posy - 1] == 'E') {
			printf("\n\t[WPADASZ NA WROGA!]\n\n");
			enemycount--;
			ekran_walki(postac1);
		}
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posy--;
		mapa[postac1->posx][postac1->posy] = 'P';

	}
	if (move[0] == 'd') {
		
		// Sprawdzenie, czy nie wychodzimy poza górną krawędź mapy
		if (postac1->posy + 1 > mapy_size - 1) {
			printf("\n\t[WYCHODZISZ POZA MAPE!]\n\n");
			return;
		}
		if (mapa[postac1->posx][postac1->posy + 1] == '|') {
			printf("\n\t[WCHODZISZ W SCIANE!]\n\n");
			return;
		}
		if (mapa[postac1->posx][postac1->posy + 1] == 'T') {
			printf("\n\t[WPADASZ W PULAPKE!]\n\n");
			trapcount--;
			pulapka(postac1);
		}
		if (mapa[postac1->posx][postac1->posy + 1] == 'E') {
			printf("\n\t[WPADASZ NA WROGA!]\n\n");
			enemycount--;
			ekran_walki(postac1);
		}
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posy++;
		mapa[postac1->posx][postac1->posy] = 'P';

	}	
}

int main()

	
{
	
	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));

	int mapx_size = 10;
	int mapy_size = 15;

	char nazwa[10];

	char** mapa = (char**)malloc(sizeof(char*) * mapx_size);

	printf ("[PODAJ SWOJE IMIE:]\n");
	scanf_s("%9s", nazwa, (unsigned)_countof(nazwa));

	postac* postac1 = generate_postac(nazwa);

	generate_map(mapa, mapx_size, mapy_size, postac1);

	

	//item* pierscien22 = generate_ring('n');
	//postac1->backpack[0][0] = *pierscien22;
	 //Generate a random weapon
	//item* miecz1 = generate_weapon();
	// Generate random armor
	//item* zbroja1 = generate_armor();
	//item* pierscien = generate_ring('f');
	//postac1->right_hand.wpn = miecz1;
	//postac1->armor_slot.arm = zbroja1;
	//postac1->ring_slot.rg = pierscien;
	//item* spareSword = generate_weapon();
	//postac1->ekwipunek = spareSword;


	int runda = 1;

	while (postac1->health > 0) {
		

		widok_glowny(mapx_size, mapy_size, mapa, postac1, runda);
		char move[1];
		printf("Wybierz ruch (w/a/s/d): ");
		scanf_s(" %c", &move[0]);
				
		if(move[0] == 'e') {			//otwieranie ekwipunku
			char znak;
			do {
				pokaz_postac(postac1);
				znak = _getch();
			} while(znak != 'q');
			system("cls");
			continue;
		}		 
					
		krok(move, mapa, postac1, mapx_size, mapy_size);

		//enemy_check()		//sprawdza czy wrog jest w poblizu i podaje jego dane

		//no_enemy_check(int enemycount)	//jesli na mapie nie ma wroga to tworzy go 
		
		runda++;
	}
	

	free(postac1->ekwipunek);
	
	//free(pierscien);
	//free(miecz1);
	
	for (int i = 0; i < postac1->backpack_height; i++) {
		free (postac1->backpack[i]) ;
	}
	for (int i = 0; i < mapx_size; i++) {
		free(mapa[i]);
	}
	free(mapa);
	

	return 0;
}

