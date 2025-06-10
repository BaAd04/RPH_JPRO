#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <windows.h>
#include <conio.h>

int last_fight_round = 0;
int item_id = 1;


struct item
{
	char name[20];
	int klasa; //1-5 klasa przedmiotu, generowanie losowe zalezne od poziomu szczescia
	int AP;
	int AS;
	int weight;
	int id;
	int def;	//defence
	int armorpoints;
	int lvl;
	int health_reg;
	int luck_modifier;
	int hp_modifier;
	bool isWeapon;
	bool isArmor;
	bool isRing;
	bool isPotion;
	int potion_type; // 0 not potion, 1 - health, 
	int weapon_type; // 0 - not wpn, 1- sword, 2 - axe, 3 - shield,
	int szerokosc;
	int wysokosc;
	bool isTwoHanded; // czy przedmiot jest dwureczny
	int typ; //0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień
};


struct Slot {
	//std::string item_name;  // Nazwa przedmiotu
	char item_name[10];
	int type;			//0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień
	bool isEmpty;           // Sprawdzenie, czy slot jest pusty
	item* wpn;             // Wskaźnik na broń

	Slot() : isEmpty(true), wpn(nullptr), item_name("XXX") {
		wpn = (item*)malloc(sizeof(item));
	} // Konstruktor domyślny
};

struct Staty {
	int enemies_slayed;	//done
	int orcs_slayed;	//done
	int goblins_slayed;	//done
	int zombie_slayed;	//done
	int wizard_slayed;	//done
	int dmg_given;		//done
	int dmg_taken;		//done
	int hits_given;		//done
	int hits_taken;		//done
	int items_picked;	//done
	int potions_used;	//done
	int hp_healed;		//done
	int poisons_got;	//done
	int poisond_dmg_got;	//done
	int hungers_got;	//done
	int xp_got;		//done
	int moved;		//done
	int moved_up;	//done
	int moved_down;	//done
	int moved_left;	//done
	int moved_right;	//done
	int cash_got;		//done
	int skillpoints_got;	//done
};

struct postac {
	char name[10];
	int lvl;
	int health;
	int attack;
	int pojemnosc;
	int xp;
	int stamina;
	int max_stamina;
	int hunger;
	int max_hunger;
	int posx;
	int posy;
	int def;
	int speed;
	int luck;
	int skillpoints;
	int cash;
	int max_hp[50];
	int max_xp[50];
	Slot right_hand;   // Slot na broń w prawej ręce
	Slot left_hand;    // Slot na broń w lewej ręce
	Slot armor_slot;   // Slot na zbroję
	Slot ring_slot;    // Slot na pierścień
	Slot ring_slot2;
	Slot potion_slot,
		potion_slot2,
		potion_slot3,
		potion_slot4,
		potion_slot5;
	item* ekwipunek;
	item** backpack;
	int backpack_height;
	int backpack_width;
	int effect; // 0 - normalny, 1- trucizna
	int effect_duration;
	Staty* statystyki; // Struktura do przechowywania statystyk postaci
};


struct enemy {
	int lvl;
	int health;
	int attack;
	int mana;
	int def;
	int xpdrop;
	int luck;
	int AS; // Attack Speed
	int type;	//1 - normalny wrog, 2- boss
	int race; // 1 - goblin, 2 - zombie, 3 - ork, 4 - wizard
};


enemy* generate_enemy(postac* postac1) {
	enemy* en = (enemy*)malloc(sizeof(enemy));

	en->lvl = postac1->lvl;
	if(postac1->lvl > 5){
		en->lvl = postac1->lvl / 2;
	}
	if (en->lvl > 10) { en->lvl = 4; }	//minimalny poziom wroga to 1


	int rand_enemytype = rand() % 10 + 1;
	if (rand_enemytype > 6) {		//goblin
		en->health = en->lvl * 3;
		en->attack = en->lvl * 1;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 5;
		en->AS = en->lvl * 1;
		en->race = 1;
		en->luck = 0;
		en->def = en->lvl * 1;

	}
	else if (rand_enemytype > 3) {	//zombie
		en->health = en->lvl * 5;
		en->attack = en->lvl * 2;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 7;
		en->AS = en->lvl * 1;
		en->race = 2;
		en->luck = 0;
		en->def = en->lvl * 3;
	}
	else if (rand_enemytype > 1) {		//ork
		en->lvl = en->lvl * 2;
		en->health = en->lvl * 8 / 3;
		en->attack = en->lvl * 5 / 3;
		en->mana = 0;
		en->type = 2;
		en->xpdrop = 10;
		en->race = 3;
		en->luck = 0;
		en->def = en->lvl * 5 / 4;
		en->AS = en->lvl * 3 / 4;

	}
	else if (rand_enemytype == 1) {		//wizard
		en->lvl = en->lvl * 5;
		en->health = en->lvl * 5 / 5;
		en->attack = en->lvl * 8 / 6;
		en->mana = 10;
		en->type = 2;
		en->xpdrop = 25;
		en->AS = en->lvl * 4 / 5;
		en->race = 4;
		en->luck = rand() % 5 + 1;
		en->def = 0;
	}

	return en;
}


postac* generate_postac(char nazwa[10]) {
	postac* post = (postac*)malloc(sizeof(postac));
	memset(post, 0, sizeof(postac));
	post->right_hand.isEmpty = true;
	post->left_hand.isEmpty = true;
	post->armor_slot.isEmpty = true;
	post->ring_slot.isEmpty = true;
	post->ring_slot2.isEmpty = true;
	post->potion_slot.isEmpty = true;
	post->potion_slot2.isEmpty = true;
	post->potion_slot3.isEmpty = true;
	post->potion_slot4.isEmpty = true;
	post->potion_slot5.isEmpty = true;

	post->backpack_height = 10;
	post->backpack_width = 12;

	strcpy_s(post->name, sizeof(post->name), nazwa);
	post->posx = 1;
	post->posy = 1;
	post->xp = 0;
	post->effect = 0;
	post->cash = 0;
	post->skillpoints = 20;
	post->effect_duration = 0;
	post->max_stamina = 10;
	post->max_hunger = 10;

	post->lvl = 1;
	post->attack = 0;
	post->health = 20;
	post->stamina = 10;
	post->speed = 0;
	post->luck = 0;
	post->def = 0;
	post->hunger = 100;


	int hpmax = 20;
	int xpmax = 20;			//xp wymagane do lvlowania po 1lvl
	for (int i = 0; i < 49; i++) {
		post->max_hp[i] = i * 5 + hpmax;	//utworzenie ciągu kolejnych liczb, które beda nastepnym limitem hp
	}
	for (int i = 0; i < 49; i++) {
		post->max_xp[i] = i * 5 + xpmax;	//utworzenie ciągu kolejnych liczb, które beda nastepnym limitem xp
	}

	int size = 100;

	post->ekwipunek = (item*)malloc(sizeof(item) * size);

	post->backpack = (item**)malloc(sizeof(item*) * post->backpack_height);
	for (int i = 0; i < post->backpack_height; i++) {
		post->backpack[i] = (item*)malloc(sizeof(item) * post->backpack_width);
	}

	for (int i = 0; i < post->backpack_height; i++) {
		for (int j = 0; j < post->backpack_width; j++) {
			post->backpack[i][j].id = 0;
		}
	}

	post->statystyki = (Staty*)malloc(sizeof(Staty));
	if (post->statystyki != NULL) {
		memset(post->statystyki, 0, sizeof(Staty)); // Initialize all fields to zero
	}

	return post;
}
item* generate_ring(postac* postac1, int klasa) {

	item* rg = (item*)malloc(sizeof(item));	//health, attack, def, speed, luck, full

	rg->isRing = true;
	rg->isArmor = false;
	rg->isWeapon = false;
	rg->isPotion = false;
	rg->isTwoHanded = false;
	rg->AP = 0;
	rg->def = 0;
	rg->AS = 0;
	rg->luck_modifier = 0;
	rg->hp_modifier = 0;
	rg->armorpoints = 0;
	rg->potion_type = 0;
	rg->weapon_type = 0; //nie weapon
	rg->weight = 1;
	rg->typ = 3;
	rg->szerokosc = 1;
	rg->wysokosc = 1;



	int rand_ = rand() % 1000 + 1;
	rg->lvl = postac1->lvl;
	if (rand_ - postac1->luck < 50) { rg->lvl = postac1->lvl + 1; }
	if (rand_ - postac1->luck < 20) { rg->lvl = postac1->lvl + 2; }
	if (rand_ - postac1->luck < 1) { rg->lvl = postac1->lvl + 5; }



	rand_ = rand() % 1000;
	rand_ += postac1->luck;
	if (rand_ > 0 && rand_ < 200) {
		rg->AP = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Attack ring");
	}
	else//to attack ring 20%
		if (rand_ >= 200 && rand_ < 400) {
			rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Deffence ring");
		}
		else//deff ring 20%
			if (rand_ >= 400 && rand_ < 600) {
				rg->AS = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Speed ring");
			}
			else //speed ring 20%
				if (rand_ >= 600 && rand_ < 690) {
					rg->luck_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl; strcpy_s(rg->name, sizeof(rg->name), "Luck ring");
				}
				else //luck ring 9%
					if (rand_ >= 690 && rand_ < 700) {
						rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Full ring");			//full ring 1%
						rg->luck_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl;
						rg->AS = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
						rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
						rg->AP = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
						rg->hp_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
					}
					else if (rand_ >= 700 && rand_ < 1000) {
						rg->hp_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Health ring");
					}	//health ring 30%

	switch (klasa) {	//wylosowane wartosci zostaja przemnozone przez klase
	case 0:						//klasa zardzewiale
		rg->AP = rg->AP * 0.5;
		rg->def = rg->def * 0.5;
		rg->AS = rg->def * 0.5;
		rg->luck_modifier = rg->luck_modifier * 0.5;
		rg->hp_modifier = rg->hp_modifier * 0.5;
		break;
	case 2:				//klasa rzadkie (klasa normalne nie wymaga zmian)
		rg->AP = rg->AP * 1.2 + postac1->luck * 0.1;
		rg->def = rg->def * 1.2 + postac1->luck * 0.1;
		rg->AS = rg->def * 1.2 + postac1->luck * 0.1;
		rg->luck_modifier = rg->luck_modifier * 1.2 + postac1->luck * 0.1;
		rg->hp_modifier = rg->hp_modifier * 1.2 + postac1->luck * 0.1;
		break;
	case 3:
		rg->AP = rg->AP * 1.5 + postac1->luck * 0.5;
		rg->def = rg->def * 1.5 + postac1->luck * 0.5;
		rg->AS = rg->def * 1.5 + postac1->luck * 0.5;
		rg->luck_modifier = rg->luck_modifier * 1.5 + postac1->luck * 0.5;
		rg->hp_modifier = rg->hp_modifier * 1.5 + postac1->luck * 0.5;
		break;
	case 4:
		rg->AP = rg->AP * 3 + postac1->luck;
		rg->def = rg->def * 3 + postac1->luck;
		rg->AS = rg->def * 3 + postac1->luck;
		rg->luck_modifier = rg->luck_modifier * 3 + postac1->luck;
		rg->hp_modifier = rg->hp_modifier * 3 + postac1->luck;
		break;
	case 5:
		rg->AP = rg->AP * 5 * postac1->luck * 2;
		rg->def = rg->def * 5 * postac1->luck * 2;
		rg->AS = rg->def * 5 * postac1->luck * 2;
		rg->luck_modifier = rg->luck_modifier * 5 * postac1->luck * 2;
		rg->hp_modifier = rg->hp_modifier * 5 * postac1->luck * 2;
		break;
	default:
		break;
	}

	rg->klasa = klasa;

	return rg;
}
item* generate_weapon(postac* postac1, int klasa) {
	item* wpn = (item*)malloc(sizeof(item));

	wpn->isRing = false;
	wpn->isArmor = false;
	wpn->isWeapon = true;
	wpn->isPotion = false;
	wpn->isTwoHanded = false;
	wpn->typ = 1;
	wpn->potion_type = 0; //nie potion
	wpn->health_reg = 0;
	wpn->armorpoints = 0;
	wpn->AP = 0;
	wpn->def = 0;
	wpn->health_reg = 0;
	wpn->hp_modifier = 0;
	wpn->luck_modifier = 0;


	int rand_ = rand() % 1000 + 1;
	wpn->lvl = postac1->lvl;					//poziom broni
	if (rand_ - postac1->luck < 50) { wpn->lvl = postac1->lvl + 1; }
	if (rand_ - postac1->luck < 20) { wpn->lvl = postac1->lvl + 2; }
	if (rand_ - postac1->luck < 1) { wpn->lvl = postac1->lvl + 5; }

	int rand_type = rand() % 100 + 1;	//axe, sword, shield
	if (30 > rand_type > 0) {	//sword
		rand_type = rand() % 100 + 1;
		if (40 > rand_type > 0) {//miecz lekki
			rand_type = 0;
		}
		else if (80 > rand_type > 40) {	//miecz sredni
			rand_type = 1;
		}
		else {	// miecz ciezki
			rand_type = rand() % 10 + 1;
			if (rand_type == 10) {//miecz dwureczny
				rand_type = 9;
			}
			else {
				rand_type = 2;
			}
		}
	}
	else
		if (60 > rand_type > 30) { //axe
			rand_type = rand() % 100 + 1;
			if (40 > rand_type > 0) {//axe lekki
				rand_type = 3;
			}
			else if (80 > rand_type > 40) {	//axe sredni
				rand_type = 4;
			}
			else {	// miecz ciezki
				rand_type = 5;
			}
		}
		else { //shield
			rand_type = rand() % 100 + 1;
			if (40 > rand_type > 0) {//shield lekki
				rand_type = 6;
			}
			else if (80 > rand_type > 40) {	//shield sredni
				rand_type = 7;
			}
			else {	// shield ciezki
				rand_type = 8;
			}
		}





	switch (rand_type)
	{
	case 0://miecz lekki

		strcpy_s(wpn->name, sizeof(wpn->name), "Lekki miecz");
		wpn->AP = wpn->lvl * rand() % 3 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 2 + 1;
		wpn->weight = 3;
		wpn->szerokosc = 3;
		wpn->wysokosc = 1;
		wpn->weapon_type = 1; //sword
		break;
	case 1:	//miecz sredni
		strcpy_s(wpn->name, sizeof(wpn->name), "Sredni miecz");
		wpn->AP = wpn->lvl * rand() % 5 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 4 + 1;
		wpn->weight = 5;
		wpn->szerokosc = 3;
		wpn->wysokosc = 1;
		wpn->weapon_type = 1; //sword
		break;
	case 2:	//miecz ciezki
		strcpy_s(wpn->name, sizeof(wpn->name), "Ciezki miecz");
		wpn->AP = wpn->lvl * rand() % 9 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 7 + 1;
		wpn->weight = 9;
		wpn->szerokosc = 4;
		wpn->wysokosc = 1;
		wpn->weapon_type = 1; //sword
		break;
	case 3:	//lekki topor
		strcpy_s(wpn->name, sizeof(wpn->name), "Lekka siekiera");
		wpn->AP = wpn->lvl * rand() % 5 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 4 + 1;
		wpn->weight = 4;
		wpn->szerokosc = 3;
		wpn->wysokosc = 1;
		wpn->weapon_type = 2; //axe
		break;
	case 4:	//sredni topor
		strcpy_s(wpn->name, sizeof(wpn->name), "Srednia siekiera");
		wpn->AP = wpn->lvl * rand() % 7 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 6 + 1;
		wpn->weight = 5;
		wpn->szerokosc = 3;
		wpn->wysokosc = 1;
		wpn->weapon_type = 2; //axe
		break;
	case 5: //ciezki topor
		strcpy_s(wpn->name, sizeof(wpn->name), "Ciezka siekiera");
		wpn->AP = wpn->lvl * rand() % 10 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 8 + 1;
		wpn->weight = 10;
		wpn->szerokosc = 4;
		wpn->wysokosc = 1;
		wpn->weapon_type = 2; //axe
		break;
	case 6:	//lekka tarcza
		strcpy_s(wpn->name, sizeof(wpn->name), "Lekka tarcza");
		wpn->AP = 0;
		wpn->def = wpn->lvl * rand() % 3 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 4 + 1;
		wpn->weight = 5;
		wpn->szerokosc = 2;
		wpn->wysokosc = 1;
		wpn->weapon_type = 3; //shield
		break;
	case 7:	//srednia tarcza
		strcpy_s(wpn->name, sizeof(wpn->name), "Lekka tarcza");
		wpn->AP = 0;
		wpn->def = wpn->lvl * rand() % 5 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 5 + 1;
		wpn->weight = 6;
		wpn->szerokosc = 2;
		wpn->wysokosc = 2;
		wpn->weapon_type = 3; //shield
		break;
	case 8:
		strcpy_s(wpn->name, sizeof(wpn->name), "Ciezka tarcza");
		wpn->AP = 0;
		wpn->def = wpn->lvl * rand() % 7 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 6 + 1;
		wpn->weight = 5;
		wpn->szerokosc = 3;
		wpn->wysokosc = 2;
		wpn->weapon_type = 3; //shield
		break;
	case 9:
		strcpy_s(wpn->name, sizeof(wpn->name), "Dwureczny miecz");
		wpn->AP = wpn->lvl * rand() % 12 + 1 + postac1->luck * 0.5;
		wpn->AS = wpn->lvl * rand() % 9 + 1;
		wpn->weight = 10;
		wpn->szerokosc = 3;
		wpn->wysokosc = 2;
		wpn->isTwoHanded = true;
		wpn->weapon_type = 1; //sword
		break;
	default:
		break;
	}
	wpn->klasa = klasa;

	switch (klasa){
	case 0://klasa zardzewiale
		wpn->AP = wpn->AP * 0.5;
		wpn->def =wpn->def * 0.5;
		wpn->AS = wpn->AS * 0.5;
		break;
	case 2:	//klasa rzadkie (klasa normalne nie wymaga zmian)
		wpn->def = wpn->def * 1.2 + postac1->luck * 0.1;
		wpn->AP = wpn->AP * 1.2 + postac1->luck * 0.1;
		wpn->AS = wpn->AS * 1.2 + postac1->luck * 0.1;
		break;
	case 3:	//klasa epicka	150% + 05luck
		wpn->def = wpn->def * 1.5 + postac1->luck * 0.5;
		wpn->AP = wpn->AP * 1.5 + postac1->luck * 0.5;
		wpn->AS = wpn->AS * 1.5 + postac1->luck * 0.5;
		break;
	case 4: //klasa legendarna	300% + luck
		wpn->def = wpn->def * 3 + postac1->luck ;
		wpn->AP = wpn->AP * 3 + postac1->luck ;
		wpn->AS = wpn->AS * 3 + postac1->luck ;
		break;
	case 5:	//destroyer 500% + 200luck
		wpn->def = wpn->def * 5 + postac1->luck * 2;
		wpn->AP = wpn->AP * 5 + postac1->luck * 2;
		wpn->AS = wpn->AS * 5 + postac1->luck * 2;
		break;
	default:
		break;
}

	
	return wpn;
}
item* generate_potion(postac* postac1, int klasa) {
	item* pot = (item*)malloc(sizeof(item));

	int potion_type = 1;	//0 - nie potion, 1 -  health, 2 - luck boost, 3 - atack boost

	strcpy_s(pot->name, sizeof(pot->name), "Health Potion");
	pot->klasa = klasa;
	pot->szerokosc = 1;
	pot->weight = 1;
	pot->armorpoints = 0;
	pot->weapon_type = 0; //nie weapon
	pot->AP = 0;
	pot->AS = 0;
	pot->def = 0;
	pot->health_reg = 0;
	pot->hp_modifier = rand() % 5 + 1;
	pot->isArmor = false;
	pot->isPotion = true;
	pot->isRing = false;
	pot->isTwoHanded = false;
	pot->isWeapon = false;
	pot->luck_modifier = 0;
	pot->typ = 0;
	return pot;
}
item* generate_armor(postac* postac1, int klasa) {
	item* armr = (item*)malloc(sizeof(item));
	armr->isArmor = true;
	armr->isPotion = false;
	armr->isRing = false;
	armr->isWeapon = false;
	armr->isTwoHanded = false;
	armr->AP = 0;
	armr->klasa = klasa;
	armr->health_reg = 0;
	armr->hp_modifier = 0;
	armr->luck_modifier = 0;
	armr->potion_type = 0;
	armr->weapon_type = 0; //nie weapon
	armr->typ = 2;	//typ zbroja

	int armr_type = rand() % 5 + 1;

	int rand_ = rand() % 1000 + 1;
	armr->lvl = postac1->lvl;
	if (rand_ + postac1->luck < 50) { armr->lvl = postac1->lvl + 1; }
	if (rand_ + postac1->luck < 20) { armr->lvl = postac1->lvl + 2; }
	if (rand_ + postac1->luck < 1) { armr->lvl = postac1->lvl + 5; }

	if (armr_type == 5) {	//ciezka zbroja
		strcpy_s(armr->name, sizeof(armr->name), "Ciężka zbroja");
		armr->AS = -(rand() % 10 + 5) + postac1->luck * 0.1 + armr->lvl * 0.1;
		armr->def = rand() % 10 + 1 + postac1->luck * 0.1 + armr->lvl * 5;	//defence
		armr->armorpoints = rand() % 10 + 1 + postac1->luck * 0.1 + armr->lvl * 5;	//armor (potem dzielony przez 2)
		armr->weight = rand() % 10 + 10;	//waga zbroi
		armr->szerokosc = 3;	//szerokosc zbroi
		armr->wysokosc = 2;	//wysokosc zbroi
	}
	else
		if (armr_type > 2) {	//normalna zbroja
			strcpy_s(armr->name, sizeof(armr->name), "Normalna zbroja");
			armr->AS = -(rand() % 5 + 1) + postac1->luck * 0.1 + armr->lvl * 0.1;
			armr->def = rand() % 5 + 1 + postac1->luck * 0.1 + armr->lvl * 2;	//defence
			armr->armorpoints = rand() % 5 + 1 + postac1->luck * 0.1 + armr->lvl * 2;	//armor (potem dzielony przez 2)
			armr->weight = rand() % 10 + 5;	//waga zbroi
			armr->szerokosc = 3;	//szerokosc zbroi
			armr->wysokosc = 2;	//wysokosc zbroi
		}
		else {
			strcpy_s(armr->name, sizeof(armr->name), "Lekka zbroja");
			armr->AS = -(rand() % 3 + 1) + postac1->luck * 0.1 + armr->lvl * 0.1;
			armr->def = rand() % 3 + 1 + postac1->luck * 0.1 + armr->lvl;	//defence
			armr->armorpoints = rand() % 3 + 1 + postac1->luck * 0.1 + armr->lvl;	//armor (potem dzielony przez 2)
			armr->weight = rand() % 5 + 5;	//waga zbroi
			armr->szerokosc = 3;	//szerokosc zbroi
			armr->wysokosc = 2;	//wysokosc zbroi
		}
	switch (klasa) {
	case 0://klasa zardzewiale
		armr->def = armr->def * 0.5;
		armr->armorpoints = armr->def * 0.5;
		break;
	case 2:	//klasa rzadkie (klasa normalne nie wymaga zmian)
		armr->def = armr->def * 1.2 + postac1->luck * 0.1;
		armr->armorpoints = armr->def * 1.2 + postac1->luck * 0.1;
		break;
	case 3:	//klasa epicka
		armr->def = armr->def * 1.5 + postac1->luck * 0.5;
		armr->armorpoints = armr->def * 1.5 + postac1->luck * 0.5;
		break;
	case 4: //klasa legendarna
		armr->def = armr->def * 3 + postac1->luck;
		armr->armorpoints = armr->def * 3 + postac1->luck;
		break;
	case 5:
		armr->def = armr->def * 5 + postac1->luck * 2;
		armr->armorpoints = armr->def * 5 + postac1->luck * 2;
		break;
	default:
		break;
	}

	return armr;


}

void show_stats(postac* postac1, char move[1]) {
			printf("\n\t\t\t[STATYSTYKI GRACZA]\n\n"
		"\t\t\tNazwa: %s\tPoziom: %d\n"
		"\t\t\tPokonani wrogowie: %d\n"
		"\t\t\t\tGobliny: %d\n"
		"\t\t\t\tZombie: %d\n"
		"\t\t\t\tOrki: %d\n"
		"\t\t\t\tWizardy: %d\n"
		"\t\t\tUderzono: %d/%d DMG\n"
		"\t\t\tPrzyjeto uderzen: %d/%d DMG\n"
		"\t\t\tPunkty umiejetnosci: %d\n"
		"\t\t\tZdobyte pieniadze: %d\n"
		"\t\t\tPodniesione przedmioty: %d\n"
		"\t\t\tWypite mikstury: %d\n"
		"\t\t\tWyleczone zdrowie: %d\n"
		"\t\t\tOtrzymane zatrucie: %d/%d DMG\n"
		"\t\t\tOtrzymany glod: %d\n"
		"\t\t\tOtrzymane doswiadczenie: %d\n"
		"\t\t\tOtrzymane pieniadze: %d\n"
		"\t\t\tWykonane ruchy: %d\n"
		"\t\t\t\tWykonane ruchy w gore: %d\n"
		"\t\t\t\tWykonane ruchy w dol: %d\n"
		"\t\t\t\tWykonane ruchy w lewo: %d\n"
		"\t\t\t\tWykonane ruchy w prawo: %d\n"

		"\n", postac1->name, postac1->lvl, postac1->statystyki->enemies_slayed, postac1->statystyki->goblins_slayed, postac1->statystyki->zombie_slayed, postac1->statystyki->orcs_slayed
			, postac1->statystyki->wizard_slayed, postac1->statystyki->hits_given, postac1->statystyki->dmg_given, postac1->statystyki->hits_taken, postac1->statystyki->dmg_taken, 
				postac1->statystyki->skillpoints_got, postac1->statystyki->cash_got, postac1->statystyki->items_picked, postac1->statystyki->potions_used, postac1->statystyki->hp_healed
			, postac1->statystyki->poisons_got, postac1->statystyki->poisond_dmg_got, postac1->statystyki->hungers_got, postac1->statystyki->xp_got, postac1->statystyki->cash_got
			, postac1->statystyki->moved, postac1->statystyki->moved_up, postac1->statystyki->moved_down, postac1->statystyki->moved_left, postac1->statystyki->moved_right);

		scanf_s(" %c", &move[0]);


	
}
void clear_potion_slot(postac* postac1, int which) {
	switch (which) {
	case 1:
		free(postac1->potion_slot.wpn);
		break;
	case 2:
		free(postac1->potion_slot2.wpn);
		break;
	case 3:
		free(postac1->potion_slot3.wpn);
		break;
	case 4:
		free(postac1->potion_slot4.wpn);
		break;
	case 5:
		free(postac1->potion_slot5.wpn);
		break;
	}
}

void use_health_potion(postac* postac1) {
	if (postac1->potion_slot.wpn != NULL && postac1->potion_slot.wpn->potion_type == 1) {
		postac1->health += postac1->potion_slot.wpn->hp_modifier;
		printf("\n\t\tUZYTO POTION HP - 1 SLOT - %dHP", postac1->potion_slot.wpn->hp_modifier);
		postac1->statystyki->potions_used++;
		int which = 1;
		clear_potion_slot(postac1, which);
	}
	else if (postac1->potion_slot2.wpn != NULL && postac1->potion_slot2.wpn->potion_type == 1) {
		postac1->health += postac1->potion_slot2.wpn->hp_modifier;
		printf("\n\t\tUZYTO POTION HP - 2 SLOT - %dHP", postac1->potion_slot2.wpn->hp_modifier);
		postac1->statystyki->potions_used++;
		int which = 2;
		clear_potion_slot(postac1, which);
	}
	else if (postac1->potion_slot3.wpn != NULL && postac1->potion_slot3.wpn->potion_type == 1) {
		postac1->health += postac1->potion_slot3.wpn->hp_modifier;
		printf("\n\t\tUZYTO POTION HP - 3 SLOT - %dHP", postac1->potion_slot3.wpn->hp_modifier);
		postac1->statystyki->potions_used++;
		int which = 3;
		clear_potion_slot(postac1, which);
	}
	else if (postac1->potion_slot4.wpn != NULL && postac1->potion_slot4.wpn->potion_type == 1) {
		postac1->health += postac1->potion_slot4.wpn->hp_modifier;
		printf("\n\t\tUZYTO POTION HP - 4 SLOT - %dHP", postac1->potion_slot4.wpn->hp_modifier);
		postac1->statystyki->potions_used++;
		int which = 4;
		clear_potion_slot(postac1, which);
	}
	else if (postac1->potion_slot5.wpn != NULL && postac1->potion_slot5.wpn->potion_type == 1) {
		postac1->health += postac1->potion_slot5.wpn->hp_modifier;
		printf("\n\t\tUZYTO POTION HP - 5 SLOT - %dHP", postac1->potion_slot5.wpn->hp_modifier);
		postac1->statystyki->potions_used++;
		int which = 5;
		clear_potion_slot(postac1, which);
	}

}

void atak_info(double sila_ataku, int powodzenie_ataku, int powodzenie_obrony) {
	switch (powodzenie_ataku) {
	case 1:
		printf("\t[PORAŻKA ATAKU!]\n\n");
		break;
	case 2:
		printf("\t[SLABY ATAK!]\n\n");
		break;
	case 4:
		printf("\t[SREDNI ATAK!]\n\n");
		break;
	case 5:
		printf("\t[STANDARDOWY ATAK!]\n\n");
		break;
	case 6:
		printf("\t[SILNY ATAK!]\n");
		break;
	default:
		break;
	}
	printf("\t[OBRONCA SIE BRONI]\n");

	if (powodzenie_ataku > 1) {
		switch (powodzenie_obrony) {
		case 1:
			printf("\t[PORAŻKA OBRONY!]\n\n");
			break;
		case 2:
			printf("\t[SLABA OBRONA!]\n\n");
			break;
		case 4:
			printf("\t[SREDNIA OBRONA!]\n\n");
			break;
		case 5:
			printf("\t[DOBRA OBRONA!]\n\n");
			break;
		case 6:
			printf("\t[SWIETNA OBRONA!]\n\n");
			break;
		default:
			break;
		}
	}
	printf("\n\t[ATAK ZABIERA %.1f HP!]\n", sila_ataku);
	Sleep(3000);


}

void put_effect(enemy* wrog, postac* postac1) {
	int poison_chance = rand() % 100 - 1 - postac1->luck;
	int poison_duration = rand() % 10 - 1 - postac1->luck * 0.1;
	if (poison_duration <= 0) { poison_duration = 1; }
	switch (wrog->race) {
	case 2:		//zombie - 10% szans na poison
		if (poison_chance >= 90) {
			printf("\n\t\t\t[WROG NALOZYL POISON]\n");
			postac1->effect = 1;
			postac1->effect_duration = poison_duration;
			postac1->statystyki->poisons_got++;
			postac1->statystyki->poisond_dmg_got += poison_duration;
		}
			break;
	case 3:
		if (poison_chance >= 90) {
			printf("\n\t\t\t[WROG NALOZYL HUNGER]\n");
			postac1->effect = 2;
			postac1->hunger = 0;
			postac1->effect_duration = poison_duration;
			postac1->statystyki->hungers_got++;	
		}
		break;
	case 4:
		if (poison_chance >= 75) {
			printf("\n\t\t\t[WROG NALOZYL POISON]\n");
			postac1->effect = 1;
			postac1->effect_duration = poison_duration;
			postac1->statystyki->poisons_got++;
			postac1->statystyki->poisond_dmg_got += poison_duration;
		}
		else
			if (poison_chance >= 50) {
				printf("\n\t\t\t[WROG NALOZYL HUNGER]\n");
				postac1->effect = 2;
				postac1->effect_duration = poison_duration;
				postac1->statystyki->hungers_got++;
			}
		break;
	default:
		printf("error switch effect put");
		break;

		}
	}

	void walka(postac * postac1, enemy * wrog, int tura, int kto_pierwszy) {

		if (wrog->AS <= postac1->speed)
		{
			kto_pierwszy = 0;
			if (postac1->speed >= 3 * wrog->AS) {
				kto_pierwszy = 2;
			}
		}
		else {
			kto_pierwszy = 1;
		}
		double rzut;	//rzut koscia
		double rzutt;	//rzut po działaniu luck
		double sila_ataku;
		int powodzenie_ataku;	//wskaznik potrzebny do wyrzucania komunikatow na ekran
		int powodzenie_obrony;
		switch (kto_pierwszy)
		{
		case 0:						//gracz ma pierwszy atak
			if (tura % 2 == 1) {		//atakuje gracz
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + postac1->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = postac1->attack + postac1->luck; powodzenie_ataku = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = postac1->attack;                 powodzenie_ataku = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = postac1->attack * 0.5;           powodzenie_ataku = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * postac1->attack;           powodzenie_ataku = 2; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;                             powodzenie_ataku = 1; }		//1

				rzut = rand() % 6 + 1;		//nastepuje rzut obroncy
				rzutt = rzut + 0.1 * wrog->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = 2; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = 1; }		//1
				sila_ataku = sila_ataku - 0.1 * wrog->def;

				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->statystyki->hits_given++;
				postac1->statystyki->dmg_given += sila_ataku;
				wrog->health = wrog->health - sila_ataku;
				printf("\t[ATAKUJESZ]\n", tura);
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}
			else {	//gdy to tura wroga
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + wrog->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = wrog->attack + wrog->luck;					  powodzenie_ataku = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = wrog->attack;                 powodzenie_ataku = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = wrog->attack * 0.5;           powodzenie_ataku = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * wrog->attack;           powodzenie_ataku = 2; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;										  powodzenie_ataku = 1; }		//1

				rzut = rand() % 6 + 1;
				rzutt = rzut + 0.1 * postac1->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = 2; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = 1; }		//1

				postac1->statystyki->hits_taken++;
				postac1->statystyki->dmg_taken += sila_ataku;
				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n");
				if (powodzenie_ataku == 6 && wrog->race > 1) {		//powinno byc ==, zmienione do testu
					put_effect(wrog, postac1);
				}
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}
			break;

		case 1:
			if (tura % 2 == 1) {		//wrog atakuje pierwszy, tura wroga
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + wrog->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = wrog->attack + wrog->luck;					  powodzenie_ataku = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = wrog->attack;                 powodzenie_ataku = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = wrog->attack * 0.5;           powodzenie_ataku = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * wrog->attack;           powodzenie_ataku = 2; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;										  powodzenie_ataku = 1; }		//1

				rzut = rand() % 6 + 1;			//nastepuje rzut obroncy
				rzutt = rzut + 0.1 * postac1->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = 2; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = 1; }		//1

				postac1->statystyki->hits_taken++;
				postac1->statystyki->dmg_taken += sila_ataku;
				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n", tura);
				if (powodzenie_ataku == 6 && wrog->race > 1) {		//powinno byc ==, zmienione do testu
					put_effect(wrog, postac1);
				}
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}
			else {	//gdy to tura bohatera
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + postac1->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = postac1->attack + postac1->luck; powodzenie_ataku = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = postac1->attack;                 powodzenie_ataku = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = postac1->attack * 0.5;           powodzenie_ataku = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * postac1->attack;           powodzenie_ataku = rzut; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;										  powodzenie_ataku = rzut; }		//1

				rzut = rand() % 6 + 1;
				rzutt = rzut + 0.1 * wrog->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = rzut; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = rzut; }		//1

				postac1->statystyki->hits_given++;
				postac1->statystyki->dmg_given += sila_ataku;
				sila_ataku = sila_ataku - 0.1 * wrog->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				wrog->health = wrog->health - sila_ataku;
				printf("\t[ATAKUJESZ]\n", tura);
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}
			break;
		case 2:
			if (tura % 3 == 0) {		//bohater atakuje 2 razy, tura wroga
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + wrog->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = wrog->attack + wrog->luck;					powodzenie_ataku = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = wrog->attack;                 powodzenie_ataku = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = wrog->attack * 0.5;           powodzenie_ataku = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * wrog->attack;           powodzenie_ataku = rzut; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;                             powodzenie_ataku = rzut; }		//1

				rzut = rand() % 6 + 1;			//nastepuje rzut obroncy
				rzutt = rzut + 0.1 * postac1->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = rzut; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = rzut; }		//1

				postac1->statystyki->hits_taken++;
				postac1->statystyki->dmg_taken += sila_ataku;
				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n");
				if (powodzenie_ataku == 6 && wrog->race > 1) {		//powinno byc ==, zmienione do testu
					put_effect(wrog, postac1);
				}
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);

			}
			else {	//tura postaci
				rzut = rand() % 6 + 1;				//nastepuje rzut atakujacego
				rzutt = rzut + postac1->luck * 0.1;

				if (rzutt >= 5.5) { sila_ataku = postac1->attack + wrog->luck; powodzenie_ataku = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = postac1->attack;                 powodzenie_ataku = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = postac1->attack * 0.5;           powodzenie_ataku = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * postac1->attack;           powodzenie_ataku = rzut; }	//2
				if (rzutt < 1.5) { sila_ataku = 0.0;										  powodzenie_ataku = rzut; }		//1

				rzut = rand() % 6 + 1;		//nastepuje rzut obroncy
				rzutt = rzut + 0.1 * wrog->luck;

				if (rzutt >= 5.5) { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = rzut; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = rzut; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = rzut; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = rzut; }	//2
				if (rzutt < 1.5) { powodzenie_obrony = rzut; }		//1

				postac1->statystyki->hits_given++;
				postac1->statystyki->dmg_given += sila_ataku;
				sila_ataku = sila_ataku - 0.1 * wrog->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				wrog->health = wrog->health - sila_ataku;
				printf("\t[ATAKUJESZ]\n");
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}

			break;
		default:
			break;
		}
		if (postac1->health < 1 &&
			(!postac1->potion_slot.isEmpty ||
				!postac1->potion_slot2.isEmpty ||
				!postac1->potion_slot3.isEmpty ||
				!postac1->potion_slot4.isEmpty ||
				!postac1->potion_slot5.isEmpty)) {
			use_health_potion(postac1);
		}
	}

	int item_class_generator(postac * postac1) {
		int klasa_itema = rand() % 1000 + 1 + postac1->luck;	//losowanie klasy przedmiotu
		if (klasa_itema >= 0 && klasa_itema < 201) { klasa_itema = 0; }	//klasa - przerdzewiały  20% szans
		if (klasa_itema >= 201 && klasa_itema < 910) { klasa_itema = 1; }	//klasa - zwykly 70% szans
		if (klasa_itema >= 910 && klasa_itema < 970) { klasa_itema = 2; }	//klasa - rzadki 5% szans
		if (klasa_itema >= 970 && klasa_itema < 1000) {	//2%
			klasa_itema = 3; //klasa - epicki 4% szans
		}
		else {
			klasa_itema = 4;		//klasa - legendarny 1% szans
			int destroyer_class = rand() % 100 + 1;	//losowanie klasy przedmiotu
			if (destroyer_class == 2) {
				klasa_itema = 5;	//klasa - destroyer 0.01 * 0.01 szans
			}
		}
		return klasa_itema;
	}

	void put_it_in(postac* postac1, item* newItem, int i, int j) {
		int szerokosc= newItem->szerokosc  ;
		int wysokosc = newItem->wysokosc ;
		switch (newItem->szerokosc) {
		case 1:
			postac1->backpack[i][j] = *newItem;
			postac1->backpack[i][j].id = item_id;
			free(newItem);
			newItem = nullptr;
			item_id++;
			break;
		case 2:
			if(newItem->wysokosc == 1 && postac1->backpack_width >= j + 1) {//2x1
				if(postac1->backpack[i][j + 1].id == 0){
				postac1->backpack[i][j] = *newItem;
				postac1->backpack[i][j].id = item_id;

				postac1->backpack[i][j + 1] = *newItem;
				postac1->backpack[i][j + 1].id = item_id;
				free(newItem);
				newItem = nullptr;
				item_id++;} 
			}else
			if(newItem->wysokosc == 2 && postac1->backpack_width >= j + 1 && postac1->backpack_height >= j + 1) {//2x2
				if(postac1->backpack[i][j + 1].id == 0 && postac1->backpack[i + 1][j].id == 0 && postac1->backpack[i + 1][j + 1].id == 0){
				postac1->backpack[i][j] = *newItem;
				postac1->backpack[i][j].id = item_id;

				postac1->backpack[i][j + 1] = *newItem;
				postac1->backpack[i][j + 1].id = item_id;

				postac1->backpack[i + 1][j] = *newItem;
				postac1->backpack[i + 1][j].id = item_id;

				postac1->backpack[i + 1][j + 1] = *newItem;
				postac1->backpack[i + 1][j + 1].id = item_id;

				free(newItem);
				newItem = nullptr;
				item_id++;
				}
			}
			break;
		case 3:
			if (newItem->wysokosc == 1 && postac1->backpack_width >= j + 2) {//3x1
				if(postac1->backpack[i][j + 1].id == 0 && postac1->backpack[i][j + 2].id == 0){
				postac1->backpack[i][j] = *newItem;
				postac1->backpack[i][j].id = item_id;

				postac1->backpack[i][j + 1] = *newItem;
				postac1->backpack[i][j + 1].id = item_id;

				postac1->backpack[i][j + 2] = *newItem;
				postac1->backpack[i][j + 2].id = item_id;
				free(newItem);
				newItem = nullptr;
				item_id++;
				}
			}else
				if (newItem->wysokosc == 2 && postac1->backpack_width >= j + 2 && postac1->backpack_height >= i + 1) {//3x2
					if (postac1->backpack[i][j + 1].id == 0 && postac1->backpack[i][j + 2].id == 0 && postac1->backpack[i+1][j].id == 0 && postac1->backpack[i + 1][j + 1].id == 0 && postac1->backpack[i + 1][j+2].id == 0) {
						postac1->backpack[i][j] = *newItem;
						postac1->backpack[i][j].id = item_id;

						postac1->backpack[i][j+1] = *newItem;
						postac1->backpack[i][j+1].id = item_id;

						postac1->backpack[i][j+2] = *newItem;
						postac1->backpack[i][j+2].id = item_id;

						postac1->backpack[i+1][j] = *newItem;
						postac1->backpack[i+1][j].id = item_id;

						postac1->backpack[i + 1][j+1] = *newItem;
						postac1->backpack[i + 1][j+1].id = item_id;

						postac1->backpack[i + 1][j+2] = *newItem;
						postac1->backpack[i + 1][j+2].id = item_id;
						free(newItem);
						newItem = nullptr;
						item_id++;
				}
			 }
			break;
		case 4:
			if (newItem->wysokosc == 1 && postac1->backpack_width >= j+3) {//4x1
				if (postac1->backpack[i][j + 1].id == 0 && postac1->backpack[i][j + 2].id == 0 && postac1->backpack[i][j + 3].id == 0) {
					postac1->backpack[i][j] = *newItem;
					postac1->backpack[i][j].id = item_id;

					postac1->backpack[i][j + 1] = *newItem;
					postac1->backpack[i][j + 1].id = item_id;

					postac1->backpack[i][j + 2] = *newItem;
					postac1->backpack[i][j + 2].id = item_id;

					postac1->backpack[i][j + 3] = *newItem;
					postac1->backpack[i][j + 3].id = item_id;
					free(newItem);
					newItem = nullptr;
					item_id++;
				}
			}
			break;
		default:
			//printf("Błąd w funkcji put_it_in, nieznana szerokość przedmiotu %d", szerokosc);
			break;
		}

	}
void pick_item(item* newItem, postac* postac1) {
	printf("\n\t[WYLECIAŁ ");
	switch (newItem->klasa) {
	case 0:
		printf("ZARDZEWIAŁY/A %s ", newItem->name);
		break;
	case 1:
		printf("NORMALNY/A %s ", newItem->name);
		break;
	case 2:
		printf("RZADKI/A %s ", newItem->name);
		break;
	case 3:
		printf("EPICKI/A %s ", newItem->name);
		break;
	case 4:
		printf("LEGENDARNY/A %s ", newItem->name);
		break;
	case 5:
		printf("D E S T R O Y E R  %s ", newItem->name);
		break;
	default:
		printf("blad switcha %s ", newItem->name);
		break;
	}
	if (newItem->AP > 0) {
		printf("ATK: %d ", newItem->AP);
	}
	if (newItem->armorpoints > 0) {
		printf("ARMOR: %d ", newItem->armorpoints);
	}
	if (newItem->AS > 0) {
		printf("SPD: %d ", newItem->AS);
	}
	if (newItem->luck_modifier > 0) {
		printf("LCK: %d ", newItem->luck_modifier);
	}
	if (newItem->def > 0) {
		printf("DEF: %d ", newItem->def);
	}
	if (newItem->hp_modifier > 0) {
		printf("HP: %d ", newItem->hp_modifier);
	}
	printf("]");

	switch (newItem->typ)
	{
	case 3:	//pierscien
		printf(
			"          __ \n"
			"	      _\/__ \n"
			"	    //----\\ \n"
			"	   ||      || \n"
			"	   ||      || \n"
			"       \\____// \n"
			"      \t ---- \n"
				"	");
		break;
	case 1:	//broń
		switch (newItem->weapon_type)
		{
			case 1:	//miecz
				printf(
					"\n"
",. \n"
 "\%`.		\n"
  "`.%`.\n"
   " `.%`.	\n"
    "  `.%`.			\n"
     "   `.%`.			\n"
      "    `.%`.    __	\n"
       "     `.%`.  \ \ \n"
        "      `.%`./_/ \n"
         "       `./ /. \n"
          "     __/\/:/;. \n"
           "    \__/  `:/;.\n"
            "           `:/;.,    \n"
             "           `:/ ; \n"
              "             `' \n"	);
				break;
			case 2:	//topór
				printf(
					"\n"

					", :\ / :.		\n"
					"//  \_()_/  \\	\n"
				   "||   |    |   ||	\n"
				   "||   |    |   ||	\n"
				   "||   |____|   ||	\n"
				   "\\  / || \  //	\n"
				   "`:/   ||  \; '	\n"
				   "	  ||			\n"
				   "      ||		\n"
				   "	  XX	\n"
				   "      XX	\n"
				   "      XX	\n"
				   "      XX	\n"
				   "      OO	\n");

			case 3:	//shield
				printf(
					"\n"
					
"\_________________/	\n"
"|       | |       |	\n"
"|       | |       |	\n"
"|       | |       |	\n"
"|_______| |_______|	\n"
"|_______   _______|	\n"
"|       | |       |	\n"
"|       | |       |	\n"
" \      | |      /		\n"
"  \     | |     /		\n"
"   \    | |    /		\n"
"    \   | |   /		\n"
"     \  | |  /			\n"
"      \ | | /			\n"
"       \| |/			\n"
"        \_/			\n");
					
					
		default:
			break;
		}
		break;
	case 2:	//zbroja
		printf(

			"*****************##******************\n"
			"**************#**#*#*#***************\n"
			"**************#*******#**************\n"
			"**************####*####**************\n"
			"**************#*#***#*#**************\n"
			"**************###***###**************\n"
			"*************#*#**#*####*************\n"
			"******######*#*#*###**########*******\n"
			"******###**#**########*#*#**###******\n"
			"*****#****#***#**********#*****#*****\n"
			"*****##***********************##*****\n"
			"*****#####*****#**********######*****\n"
			"*****##*#*#**%********#**###***#*****\n"
			"*****#**#*##*************#*#***#*****\n"
			"*****#**#*#*****************#**#*****\n"
			"****##**#*#**************#*##*##*****\n"
			"****##**#*#**************#*##*###****\n"
			"****#***#*#******#%#******###**##****\n"
			"****%######**************##*###%*****\n"
			"****#***#*#******###*******#*#*#*****\n"
			"****#***##*****##***##*****#*#*#*****\n"
			"***##**###****#*#***#*#*****##*##****\n"
			"****#*##*######*#***#*#####*##*##****\n"
			"*****#**#*#*****#***#*****#*#**#*****\n"
			"**********#**************************\n"
			"*************************************\n"
			"****************%%#%%#***************\n"
		);
		break;
	case 0:	//potion
		printf("\n"
			"	   _____\n"
			"`.___,' \n"
			"(_____)\n"
			"     <     >\n"
			"      )---(\n"
			"     /     \ \n"
			"    /       \\n"
			"   |         |\n"
			"   |         |\n"
			"   |_________|\n"
			"    |_______|\n"
			"     |     |\n"
			"     '-----'\n");

		break;
	default:
		printf("blad switch ascii itema");
		break;
	}
	printf("\ng - wez do ekwipunku, inny klawisz - porzuc [NIE MOZNA COFNAC]\n");
	char move[2];
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {}
	scanf_s("%c", &move[0], 1);
	if (move[0] == 'g') {

		int szerokosc = newItem->szerokosc;
		postac1->statystyki->items_picked++;
		for (int i = 0; i < postac1->backpack_height; i++) {
			for (int j = 0; j < postac1->backpack_width; j++) {
				if (postac1->backpack[i][j].id == 0) {//sprawdzanie czy jest puste
					
					put_it_in(postac1, newItem, i, j);
					if (newItem == nullptr) { return; }
				}

			}
		}
	}
}

void post_fight(postac* postac1, enemy* wrog) {
	postac1->xp = postac1->xp + wrog->xpdrop;
	int cash_drop = rand() % 10 + 1 * wrog->type;
	postac1->cash = postac1->cash + cash_drop * postac1->luck;
	postac1->statystyki->cash_got += cash_drop * postac1->luck;
	postac1->statystyki->xp_got += wrog->xpdrop;

	printf("\n\n\t[WROG POKONANY|%dXP|%d$]\n\n", wrog->xpdrop, cash_drop * postac1->luck);

	int item_drop = rand() % 11 + postac1->luck * 0.1;	//losowanie przedmiotu
	if (item_drop >= 1) {			//todo zmienic 1 na 10

		int klasa_itema = item_class_generator(postac1);	//losowanie klasy przedmiotu

		int type_rand = rand() % 10;	//losowanie typu przedmiotu
		int item_type = 0;//0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień

		if (type_rand >= 0 && type_rand < 2) {	//20% na bron			0,1
			item_type = 1;
			item* newItem = generate_weapon(postac1, klasa_itema);
			pick_item(newItem, postac1);

		}
		else if (type_rand >= 2 && type_rand < 4) {	//zbroja 20%		2,3
			item_type = 2;
			item* newItem = generate_armor(postac1, klasa_itema);
			pick_item(newItem, postac1);
		}
		else if (type_rand >= 4 && type_rand < 7) {	//potion 30%	4,5, 6
			item_type = 0;
			item* newItem = generate_potion(postac1, klasa_itema);
			pick_item(newItem, postac1);
		}
		else if (type_rand >= 6 && type_rand < 10) {	// pierścień 30%	7,8,9
			item_type = 3;
			item* newItem = generate_ring(postac1, klasa_itema);
			pick_item(newItem, postac1);
		}

	}


}

void lvlowanie(postac* postac1) {

	postac1->skillpoints = postac1->skillpoints + postac1->lvl;
	postac1->xp = postac1->xp - postac1->max_xp[postac1->lvl - 1];
	postac1->statystyki->skillpoints_got ++;
	printf("\n\t[LVLUJESZ!!]\n\t[DOSTAJESZ SKILLPOINT]\n");

	int losowanie_powerupa = rand() % 10 + 1;

	switch (losowanie_powerupa) {		//def luck speed attack skillpoints gracz ma szanse na dodatkowy punkt
	case 1:
		postac1->def++;
		printf("\n\t[DEFF ZOSTAJE ULEPSZONY]\n");
		break;
	case 2:
		postac1->luck++;
		printf("\n\t[LUCK ZOSTAJE ULEPSZONY]\n");
		break;
	case 3:
		postac1->speed++;
		printf("\n\t[SPEED ZOSTAJE ULEPSZONY]\n");
		break;
	case 4:
		postac1->attack++;
		printf("\n\t[ATTACK ZOSTAJE ULEPSZONY]\n");
		break;
	case 5:
		postac1->skillpoints++;
		printf("\n\t[DOSTAJESZ DODATKOWY SKILLPOINT]\n");
		postac1->statystyki->skillpoints_got++;
		break;
	default:
		break;
	}
	postac1->lvl++;
	Sleep(1000);
}

void ekran_walki(postac* postac1) {
	enemy* wrog = generate_enemy(postac1);
	int tura = 1;
	int kto_pierwszy = 4;
	printf("\n\n\t[WALKA Z ");
	switch (wrog->race)
	{
	case 1:
		printf("GOBLINEM]\n\n");
		break;
	case 2:
		printf("ZOMBIE]\n\n");
		break;
	case 3:
		printf("ORKIEM]\n\n");
		break;
	case 4:
		printf("WIZARDEM]\n\n");
		break;
	default:
		break;
	}


	while (wrog->health > 0 && postac1->health > 0) {
		system("cls");

		switch (wrog->race) {
		case 1:
			printf(
				"										 \n"
				"						ENEMY %d	|	HERO %d \n"
				"							|\n"
				"	 	    ⣠⣶⣿⣿⣶⣄⠀⠀⠀⠀⠀	⠀⠀⠀⠀⠀	⠀⠀		|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀	\tHP : %d	|	%d\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢧⡀⠀⠀⠀⠀⠀⠀⠀	\tSPD: %d	|	%d\n"
				"	⠀⠢⣤⣀⡀⠀⠀⠀⢿⣧⣄⡉⠻⢿⣿⣿⡿⠟⢉⣠⣼⡿⠀⠀⠀⠀⣀⣤⠔⠀	\tATK: %d	|	%d\n"
				"	⠀⠀⠈⢻⣿⣶⠀⣷⠀⠉⠛⠿⠶⡴⢿⡿⢦⠶⠿⠛⠉⠀⣾⠀⣶⣿⡟⠁⠀⠀	\tDEF  %d	|	%d\n"
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
				"	⧵____ | ⧵___/|_.__/|_ | _ | _	⠀\n", wrog->lvl, postac1->lvl, wrog->health, postac1->health, wrog->AS, postac1->speed,
				wrog->attack, postac1->attack, wrog->def, postac1->def
			);
			break;
		case 2:
			printf(
				"	        \n"
				"									ENEMY %d | HERP %d \n"
				"										|\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠊⠉⠉⠉⠉⢉⠝⠉⠓⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀			\t\tHP : %d\t|\t %d\n"
				"	⠀⠀⠀⠀⠀⠀⢀⡴⣋⠀⠀⣤⣒⡠⢀⠀⠐⠂⠀⠤⠤⠈⠓⢦⡀⠀⠀⠀⠀⠀			\t\tSPD: %d\t|\t %d\n"
				"	⠀⠀⠀⠀⠀⣰⢋⢬⠀⡄⣀⠤⠄⠀⠓⢧⠐⠥⢃⣴⠤⣤⠀⢀⡙⣆⠀⠀⠀⠀			\t\tATK: %d\t|\t %d\n"
				"	⠀⠀⠀⠀⢠⡣⢨⠁⡘⠉⠀⢀⣤⡀⠀⢸⠀⢀⡏⠑⠢⣈⠦⠃⠦⡘⡆⠀⠀⠀			\t\tDEF: %d\t|\t %d\n"
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
				"	  /____⧵___/|_| |_| |_|_.__/|_|⧵___|	\n", wrog->lvl, postac1->lvl, wrog->health, postac1->health, wrog->AS, postac1->speed,
				wrog->attack, postac1->attack, wrog->def, postac1->def);

			break;
		case 3:
			printf(
				"	        \n"
				"									ENEMY %d | 	HERP %d \n"
				"										|\n"
				"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
				"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\tHP : %d\t|\t%d\n"
				"	⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣧⣄⣉⣉⣠⣼⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 		    \t\t\tSPD: %d\t|\t%d\n"
				"	⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⡿⣿⣿⣿⣿⢿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀			\t\tATK: %d\t|\t%d\n"
				"	⠀⠀⠀⠀⠀⠀⠀⣼⣤⣤⣈⠙⠳⢄⣉⣋⡡⠞⠋⣁⣤⣤⣧⠀⠀⠀⠀⠀⠀⠀			\t\tDEF: %d\t|\t%d\n"
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
				"		 ⧵___/|_|  |_ |⧵_⧵			\n", wrog->lvl, postac1->lvl, wrog->health, postac1->health, wrog->AS, postac1->speed,
				wrog->attack, postac1->attack, wrog->def, postac1->def);
			break;
		case 4:
			printf(


				"			     _,-'|		\t|\n"
				"			 ,-' ._  |		ENEMY %d|	HERP %d \n"
				"		 . || ,   |####⧵ |			|\n"
				"		 ⧵.`', /  ⧵####| |		HP : %d \t|\t%d\n"
				"	   	 =, . =   | ###| |		SPD: %d \t|\t%d\n"
				"		 / || ⧵ , -'⧵#/,'`.		ATK: %d |\t%d\n"
				"		   || ,  '   `,,. `.   \t\tDEF: %d\t|\t%d\n"
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
				"		 ⧵_/⧵_/  |_/___⧵__,_|_|  ⧵__,_| \n", wrog->lvl, postac1->lvl, wrog->health, postac1->health, wrog->AS, postac1->speed,
				wrog->attack, postac1->attack, wrog->def, postac1->def);
			break;
		default:
			break;
		}
		Sleep(1000);
		printf("\nTURA WALKI: %d\n", tura);
		walka(postac1, wrog, tura, kto_pierwszy);

		if(wrog->health <= 0) {
			postac1->statystyki->enemies_slayed++;
			switch (wrog->race) {
			case 1:	//goblin
				postac1->statystyki->goblins_slayed++;
				break;
			case 2:	//zombie
				postac1->statystyki->zombie_slayed++;
				break;
			case 3:	//ork
				postac1->statystyki->orcs_slayed++;
				break;
			case 4:	//wizard
				postac1->statystyki->wizard_slayed++;
				break;
			}
			break;
		}
		tura++;
	}
	if (postac1->health > 0) { post_fight(postac1, wrog); }

	while (postac1->xp >= postac1->max_xp[postac1->lvl - 1]) {
		lvlowanie(postac1);
	}

}

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
			}
			else if (trap_rand < 3) {
				mapa[i][j] = 'T';
				trapcount++;
			}
			else if (enemy_rand < 2) {
				mapa[i][j] = 'E';
				enemycount++;
			}
			else {
				mapa[i][j] = '.';
			}
		}
	}
	int i = postac1->posx;
	int j = postac1->posy;
	if (mapa[i][j] == 'T') {
		trapcount--;
	}
	if (mapa[i][j] == 'E') {
		enemycount--;
	}
	mapa[i][j] = 'P'; // postac na mapie
}

void show_map(char** mapa, int mapx_size, int mapy_size, postac* postac1) {
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
		printf("#"); // Prawa ramka
		switch (i) {
		case 0:
			printf("\tLVL: %d\t\tXP: %d/%d\n", postac1->lvl, postac1->xp, postac1->max_xp[postac1->lvl - 1]);
			break;
		case 2:
			printf("\tCSH: %d$ \tHP: %d/%d\n", postac1->cash, postac1->health, postac1->max_hp[postac1->lvl - 1]);
			break;
		case 4:
			printf("\tLCK: %d\t\tSPD: %d\n", postac1->luck, postac1->speed);
			break;
		case 6:
			printf("\tATK: %d\t\tSTM: %d\n", postac1->attack, postac1->stamina);
			break;
		case 8:
			printf("\tDEF: %d\t\tSTATUS: ", postac1->def);
			switch (postac1->effect) {
			case 0:
				printf("NORMALNY \n");
				break;
			case 1:
				printf("ZATRUTY - %d RUND\n", postac1->effect_duration);
				break;
			case 2:
				printf("FAMINE - %d RUND\n", postac1->effect_duration);
				break;
			default:
				printf("ERROR EFFECT \n");
				break;

			}
			break;
		case 5:
			if (postac1->skillpoints > 0) {
				printf("\t[MASZ %d SKILLPOINT/OW!!!]\n", postac1->skillpoints);
			}
			else {
				printf("\n");
			}

			break;
		default:
			printf("\n");
			break;
		}

	}

	// Dolna ramka
	printf("\t");
	for (int j = 0; j < mapy_size + 2; j++) {
		printf("# ");
	}
	printf("\n");
}
void model_hero(postac* postac1) {					//model asci bedzie sie roznil w zaleznosci od broni
	int model = 0; //1 - dwureczny miecz, 2- miecz w prawej, 3 - miecz w lewej, 4, tarcza w prawej, 5 tarcza w lewej, 6 topor w prawej, 7 topor w lewej, 8 miecz prawa + tarcza, 9 miecz lewa + tarcza, 10 topor prawa+ tarcza, 11 topor lewa + tarcza, 12 miecz prawa + topor, 13 miecz lewa + topor, 14 dwa miecze, 15 dwie tarcze, 16 dwa topory
	if (postac1->right_hand.isEmpty == false && postac1->right_hand.wpn->isTwoHanded == true) { model = 1; }
	if (postac1->right_hand.isEmpty == false && postac1->left_hand.isEmpty == true) {//tylko prawa dlon
		if (postac1->right_hand.wpn->weapon_type == 1) { model = 2; } //miecz w prawej
		if (postac1->right_hand.wpn->weapon_type == 2) { model = 6; }//topor w prawej
		if (postac1->right_hand.wpn->weapon_type == 3) { model = 4; }	//tarcza w prawej
	}
	if (postac1->right_hand.isEmpty == true && postac1->left_hand.isEmpty == false) {	//lewa pelna prawa pusta
		if (postac1->left_hand.wpn->weapon_type == 1) { model = 3; } //miecz w lewej
		if (postac1->left_hand.wpn->weapon_type == 2) { model = 7; } //topor w lewej
		if (postac1->left_hand.wpn->weapon_type == 3) { model = 5; } //miecz w lewej
	}
	if (postac1->right_hand.isEmpty == false && postac1->left_hand.isEmpty == false && postac1->left_hand.wpn->isTwoHanded == false) {//obie dlonie zajete
		if (postac1->right_hand.wpn->weapon_type == 1 && postac1->left_hand.wpn->weapon_type == 3) { model = 8; }	//miecz prawa + tarcza
		if (postac1->right_hand.wpn->weapon_type == 3 && postac1->left_hand.wpn->weapon_type == 1) { model = 9; }	//miecz lewa + tarcza

		if (postac1->right_hand.wpn->weapon_type == 2 && postac1->left_hand.wpn->weapon_type == 3) { model = 10; }	//topor prawa + tarcza
		if (postac1->right_hand.wpn->weapon_type == 3 && postac1->left_hand.wpn->weapon_type == 2) { model = 11; }	//topor lewa + tarcza

		if (postac1->right_hand.wpn->weapon_type == 1 && postac1->left_hand.wpn->weapon_type == 2) { model = 12; }	// miezce prawa + topor
		if (postac1->right_hand.wpn->weapon_type == 2 && postac1->left_hand.wpn->weapon_type == 1) { model = 13; }	//miecz lewa + topor

		if (postac1->right_hand.wpn->weapon_type == 1 && postac1->left_hand.wpn->weapon_type == 1) { model = 14; }	//dwa miezce
		if (postac1->right_hand.wpn->weapon_type == 3 && postac1->left_hand.wpn->weapon_type == 3) { model = 15; }	//dwie tarcze
		if (postac1->right_hand.wpn->weapon_type == 2 && postac1->left_hand.wpn->weapon_type == 2) { model = 16; }	//dwa topory
	}


	switch (model) {
	case 1:	//dwureczny miecz
		printf("\n"
			"              {}	\n"
			"             {{}}	\n"
			"             {{}}	\n"
			"              {}	\n"
			"            .-''-.	\n"
			"           /  __  \	\n"
			"          /.-'  '-.\	\n"
			"          \::.  .::/	\n"
			"           \'    '/	\n"
			"      __ ___)    (___ __	\n"
			"    .'   \\        //   `.	\n"
			"   /     | '-.__.-' |     \	\n"
			"   |     |  '::::'  |     |	\n"
			"   |    /    '::'    \    |	\n"
			"   |_.-;\     __     /;-._|	\n"
			"   \.'^`\\    \/    //`^'./	\n"
			"   /   _.-._ _||_ _.-._   \	\n"
			"  `\___\    '-..-'    /___/`	\n"
			"       /'---.  `\.---'\		\n"
			"      ||    |`\\\|    ||	\n"
			"      ||    | || |    ||	\n"
			"      |;.__.' || '.__.;|	\n"
			"      |       ||       |	\n"
			"      {{{{{{{{||}}}}}}}}	\n"
			"       |      ||      |	\n"
			"       |.-==-.||.-==-.|	\n"
			"       <.    .||.    .>	\n"
			"        \'=='/||\'=='/		\n"
			"        |   / || \   |		\n"
			"        |   | || |   |		\n"
			"        |   | || |   |		\n"
			"        /^^\| || |/^^\		\n"
			"       /   .' || '.   \	\n"
			"      /   /   ||   \   \	\n"
			"     (__.'    \/    '.__)	\n");
		break;
	case 2:	//miecz prawa
		printf("\n"
			" /\	\n"
			" ||	\n"
			" ||	\n"
			" ||	\n"
			" ||           {}	\n"
			" ||          .--.	\n"
			" ||         /.--.\	\n"
			" ||         |====|	\n"
			" ||         |`::`|	\n"
			"_||_    .-;`\..../`;-. \n"
			" /\\   /  |...::...|  \ \n"
			" |:'\ |   /'''::'''\   | \n"
			"  \ /\;-,/\   ::   /\--; \n"
			"   \ <` >  >._::_.<,<__> \n"
			"    `""`  /   ^^   \|  | \n"
			"          |        |\::/ \n"
			"          |        |/||| \n"
			"          |___/\___| ''' \n"
			"           \_ || _/	\n"
			"           <_ >< _>  \n"
			"           |  ||  | \n"
			"           |  ||  |	\n"
			"          _\.:||:./_	\n"
			"         /____/\____\	\n");
		break;
	case 3://miecz lewa

		printf("\n"
			" 		               /\	\n"
			"		               ||	\n"
			"				       ||	\n"
			"					   ||	\n"
			"      	   {}	       ||	\n"
			"         .--.	       ||		\n"
			"        /.--.\	       ||	\n"
			"        |====|	       ||	\n"
			"        |`::`|	       ||	\n"
			"   .-;`\..../`;-.    _||_	\n"
			"  /  |...::...|  \   //\	\n"
			"  |   /'''::'''\   | /`:|	\n"
			"  ;--'\   ::   /\,-;/\ /	\n"
			"  <__>,>._::_.<  < `> /	\n"
			"  |  |/   ^^   \  `""`		\n"
			"  \::/|        |			\n"
			"  |||\|        |			\n"
			"  ''' |___/\___|			\n"
			"       \_ || _/			\n"
			"       <_ >< _>			\n"
			"       |  ||  |			\n"
			"       |  ||  |			\n"
			"      _\.:||:./_			\n"
			"     /____/\____\			\n");
		break;
	case 4: //tarcza w prawej
		break;





	case 8:	//miecz w prawej + tarcza
		printf("\n"

			" /\	\n"
			" ||	\n"
			" ||	\n"
			" ||			\n"
			" ||           {}	\n"
			" ||          .--.	\n"
			" ||         /.--.\	\n"
			" ||         |====|	\n"
			" ||         |`::`|	\n"
			"_||_    .-;`\..../`;_.-^-._	\n"
			" /\\   /  |...::..|`   :   `|	\n"
			" |:'\ |   /'''::''|   .:.   |	\n"
			"  \ /\;-,/\   ::  |..:::::..|	\n"
			"   \ <` >  >._::_.| ':::::' |	\n"
			"    `""`  /   ^^  |   ':'   |	\n"
			"          |       \    :    /	\n"
			"          |        \   :   /	\n"
			"          |___/\___|`-.:.-`	\n"
			"           \_ || _/    `		\n"
			"           <_ >< _>			\n"
			"           |  ||  |			\n"
			"           |  ||  |			\n"
			"          _\.:||:./_			\n"
			"        /____/\____\			\n");

		break;


	default:
	printf(
		"		       {}			[IMIE] = %s\n"
		"		      .--.		    \t[LEVEL] = %d\t\t[ARMOR-SLOT] = ", postac1->name, postac1->lvl);
	if (postac1->armor_slot.isEmpty == false) {
		printf("%s\n", postac1->armor_slot.wpn->name);
	}
	else { printf("[XXX]\n"); }
	printf(
		"		     /.--.⧵			[HEALTH] = %d/%d\n"
		"		    |= == =|		\t[ATTACK] = %d\t\t[LEFT  HAND] = ", postac1->health, postac1->max_hp[postac1->lvl - 1], postac1->attack);
	if (postac1->left_hand.isEmpty == false) {
		printf("%s\n", postac1->left_hand.wpn->name);
	}
	else { printf("[XXX]\n"); }
	printf(
		"		    | `::` |		\t[X POINTS] = %d/%d		\n"
		"		  .-;`⧵..../ `;-.	\t[STAMINA] = %d\t\t[RIGHT HAND] = ", postac1->xp, postac1->max_xp[postac1->lvl - 1], postac1->stamina);
	if (postac1->right_hand.isEmpty == false) {
		printf("%s\n", postac1->right_hand.wpn->name);
	}
	else { printf("[XXX]\n"); }
	printf(
		"		  /  |...::... |  ⧵	\t[HUNGER]  = %d	\n"
		"		  |  /''': :'''⧵   |	\t[DEF] = %d\t\t[FIRST RING] = ", postac1->hunger, postac1->def);
	if (postac1->ring_slot.isEmpty == false) {
		printf("%s\n", postac1->ring_slot.wpn->name);
	}
	else { printf("[XXX]\n"); }
	printf(
		"		  ; --'⧵   ::  /⧵--;	\t[SPEED] = %d\n"
		"	         <__>,>._::_. <,<__>    \t[LUCK] = %d\t\t[SCND. RING] = ", postac1->speed, postac1->luck);
	if (postac1->ring_slot2.isEmpty == false) {
		printf("%s\n", postac1->ring_slot2.wpn->name);
	}
	else { printf("[XXX]\n"); }
	printf(
		"	       |  |/   ^^    ⧵|  |\t\t[CASH] = %d	\n"
		"	       ⧵::/|         |⧵::/	\t[STATUS] = ", postac1->cash);
	switch (postac1->effect) {
	case 0:
		printf("[NORMALNY]\n");
		break;
	case 1:
		printf("[POISON - %d RUND]\n", postac1->effect_duration);
		break;
	case 2:
		printf("[FAMINE - %d RUND]\n", postac1->effect_duration);
		break;
	default:
		printf("error \n");
		break;
	}
	printf(
		"	       |||⧵|         |/|||	\n"
		"	       ''' |___/ ⧵___| '''	\n"
		"                   ⧵_ |  | _ /	\t\t[POTIONS] = ");
	if (postac1->potion_slot.isEmpty == false) {
		printf("%s ", postac1->potion_slot.wpn->name);
	}
	else { printf("[XXX]  "); }
	if (postac1->potion_slot2.isEmpty == false) {
		printf("%s ", postac1->potion_slot.wpn->name);
	}
	else { printf("[XXX]  "); }
	if (postac1->potion_slot3.isEmpty == false) {
		printf("%s ", postac1->potion_slot.wpn->name);
	}
	else { printf("[XXX]  "); }
	if (postac1->potion_slot4.isEmpty == false) {
		printf("%s ", postac1->potion_slot.wpn->name);
	}
	else { printf("[XXX]  "); }
	if (postac1->potion_slot5.isEmpty == false) {
		printf("%s \n", postac1->potion_slot.wpn->name);


	}
	else { printf("[XXX]  \n"); }
	printf(
		"	            <_ >< _>				\n"
		"	            |  ||  |					\n"
		"	            |  ||  |					\n"
		"	           _⧵.:||:./_			\t"); if (postac1->skillpoints > 0) { printf("MASZ %d SKILLPOINTOW", postac1->skillpoints); }printf(
			"\n		  /____/ ⧵____⧵		\n\n\n"
			"_____________________________________________________________________________________________________________\n"
		);
	break;
	}
}

void skillpoint_distribution(postac* postac1, char move[1]) {
	model_hero(postac1);
	printf("Wybierz co ulepszyc - a:attack,  d:def, s: speed, f:luck(2SP)\n");
	scanf_s(" %c", &move[0]);
	if (postac1->skillpoints > 0) {
		if (move[0] == 'a') { postac1->attack += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES ATTACK!\n"); }
		else
			if (move[0] == 'd') { postac1->def += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES DEF!\n"); }
			else
				if (move[0] == 's') { postac1->speed += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES SPEED!\n"); }
				else
					if (move[0] == 'f' && postac1->skillpoints >= 2) {
						postac1->skillpoints = postac1->skillpoints - 2; postac1->luck += 1; printf("ULEPSZYLES LUCK!\n");
					}
					else { printf("blad skillpoint distribution"); }
	}
}

void show_ekwipunek(postac* postac1) {
	printf("Ekwipunek postaci:\n");
	int last_item_id = -1;
	for (int i = 0; i < postac1->backpack_height; i++) {
		for (int j = 0; j < postac1->backpack_width; j++) {
			if (postac1->backpack[i][j].id != 0 && postac1->backpack[i][j].id > last_item_id) {
				last_item_id = postac1->backpack[i][j].id; // Zapamiętaj ostatni ID przedmiotu
				printf("\n%d  ", postac1->backpack[i][j].id);
				switch (postac1->backpack[i][j].klasa) {
				case 0:
					printf("PRZERDZEWIAŁY/A ");
					break;
				case 1:
					printf("NORMALNY/A");
					break;
				case 2:
					printf("RZADKI/A");
					break;
				case 3:
					printf("EPICKI/A");
					break;
				case 4:
					printf("LEGENDARNY/A");
					break;
				case 5:
					printf("!D E S T R O Y E R!");
					break;
				default:
					printf("BŁĄD W KLASIE PRZEDMIOTU! 1144l\n");
					break;
				}
				printf(" %s ", postac1->backpack[i][j].name);
				if (postac1->backpack[i][j].AP > 0) {
					printf("ATK: %d ", postac1->backpack[i][j].AP);
				}
				if (postac1->backpack[i][j].armorpoints > 0) {
					printf("ARMOR: %d ", postac1->backpack[i][j].armorpoints);
				}
				if (postac1->backpack[i][j].AS > 0) {
					printf("SPD: %d ", postac1->backpack[i][j].AS);
				}
				if (postac1->backpack[i][j].luck_modifier > 0) {
					printf("LCK: %d ", postac1->backpack[i][j].luck_modifier);
				}
				if (postac1->backpack[i][j].def > 0) {
					printf("DEF: %d ", postac1->backpack[i][j].def);
				}
				if (postac1->backpack[i][j].hp_modifier > 0) {
					printf("HP: %d ", postac1->backpack[i][j].hp_modifier);
				}
				printf("\n");



			}

		}
	}
	if (last_item_id == -1) { printf("brak przedmiotów"); }

}

void take_down_item(postac* postac1, char move[1]) {
	printf("\n\t[ZDEJMOWANIE ITEMOW]\n");

	if (postac1->armor_slot.isEmpty == false) {	//wyrzucenie danych posiadanego armora
		printf("a - %s, %d DEF %d ARMORPOINT\n", postac1->armor_slot.wpn->name, postac1->armor_slot.wpn->def, postac1->armor_slot.wpn->armorpoints);
	}
	if (!postac1->right_hand.isEmpty && postac1->right_hand.wpn->isTwoHanded == true) {	//wyrzucenie danych posiadanego miecza dwurecznego
		printf("t - %s, %d AP, %d AS\n", postac1->right_hand.wpn->name, postac1->right_hand.wpn->AP, postac1->right_hand.wpn->AS);
	}else{
		if (postac1->right_hand.isEmpty == false) {
			printf("r - %s", postac1->right_hand.wpn->name);
			if (postac1->right_hand.wpn->AP > 0) { printf("%d AP ", postac1->right_hand.wpn->AP); }
			if (postac1->right_hand.wpn->AS > 0) { printf("%d AS ", postac1->right_hand.wpn->AS); }
			if (postac1->right_hand.wpn->def > 0) { printf("%d def ", postac1->right_hand.wpn->def); }
			printf("\n");
		}
		if (postac1->left_hand.isEmpty == false) {
			printf("l - %s", postac1->left_hand.wpn->name);
			if (postac1->left_hand.wpn->AP > 0) { printf("%d AP ", postac1->left_hand.wpn->AP); }
			if (postac1->left_hand.wpn->AS > 0) { printf("%d AS ", postac1->left_hand.wpn->AS); }
			if (postac1->left_hand.wpn->def > 0) { printf("%d def ", postac1->left_hand.wpn->def); }
			printf("\n");
		}	
	}
	if (postac1->left_hand.isEmpty == false) {
		printf("l - %s", postac1->left_hand.wpn->name);
		if (postac1->left_hand.wpn->AP > 0) { printf("%d AP ", postac1->left_hand.wpn->AP); }
		if (postac1->left_hand.wpn->AS > 0) { printf("%d AS ", postac1->left_hand.wpn->AS); }
		if (postac1->left_hand.wpn->def > 0) { printf("%d def ", postac1->left_hand.wpn->def); }
		printf("\n");
	}
	if (postac1->ring_slot.isEmpty == false) {	//wyrzucenie danych posiadanego pierścienia
		printf("i - %s, %d LUCK\n", postac1->ring_slot.wpn->name, postac1->ring_slot.wpn->luck_modifier);
	}
	if (postac1->ring_slot2.isEmpty == false) {	//wyrzucenie danych posiadanego drugiego pierścienia
		printf("o - %s, %d LUCK\n", postac1->ring_slot2.wpn->name, postac1->ring_slot2.wpn->luck_modifier);
	}


	if (postac1->potion_slot.isEmpty == false) {	//wyrzucenie danych posiadanej mikstury
		printf("z - %s, %d HP\n", postac1->potion_slot.wpn->name, postac1->potion_slot.wpn->health_reg);
	}
	if (postac1->potion_slot2.isEmpty == false) {	//wyrzucenie danych posiadanej mikstury
		printf("x - %s, %d HP\n", postac1->potion_slot2.wpn->name, postac1->potion_slot2.wpn->health_reg);
	}
	if (postac1->potion_slot3.isEmpty == false) {	//wyrzucenie danych posiadanej mikstury
		printf("c - %s, %d HP\n", postac1->potion_slot3.wpn->name, postac1->potion_slot3.wpn->health_reg);
	}
	if (postac1->potion_slot4.isEmpty == false) {	//wyrzucenie danych posiadanej mikstury
		printf("v - %s, %d HP\n", postac1->potion_slot4.wpn->name, postac1->potion_slot4.wpn->health_reg);
	}
	if (postac1->potion_slot5.isEmpty == false) {	//wyrzucenie danych posiadanej mikstury
		printf("b - %s, %d HP\n", postac1->potion_slot5.wpn->name, postac1->potion_slot5.wpn->health_reg);
	}
	scanf_s(" %c", &move[0]);

	switch (move[0])
	{
	case 'a':	//zdjecie armora
		if (postac1->armor_slot.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ ARMOR %s]\n", postac1->armor_slot.wpn->name);
			item* newItem = postac1->armor_slot.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->armor_slot.isEmpty = true;	
		}
		else { printf("\n\t[ARMOR SLOT JEST PUSTY!]\n"); }
		break;
	case 'r':	//zdjecie prawej reki
		if (postac1->right_hand.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ PRAWY PRZEDMIOT %s]\n", postac1->right_hand.wpn->name);
			item* newItem = postac1->right_hand.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->right_hand.isEmpty = true;
		}
		else { printf("\n\t[PRAWA REKA JEST PUSTA!]\n"); }
		break;
	case 'l':	//zdjecie lewej reki
		if (postac1->left_hand.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ LEWY PRZEDMIOT %s]\n", postac1->left_hand.wpn->name);
			item* newItem = postac1->left_hand.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->left_hand.isEmpty = true;
		}
		else { printf("\n\t[LEWA REKA JEST PUSTA!]\n"); }
		break;
	case 'i':	//zdjecie pierwszego pierścionka
		if (postac1->ring_slot.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ PIERŚCIONEK %s]\n", postac1->ring_slot.wpn->name);
			item* newItem = postac1->ring_slot.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->ring_slot.isEmpty = true;
		}
		else { printf("\n\t[PIERŚCIONEK SLOT JEST PUSTY!]\n"); }
		break;
	case 'o':	//zdjecie drugiego pierścionka
		if (postac1->ring_slot2.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ DRUGI PIERŚCIONEK %s]\n", postac1->ring_slot2.wpn->name);
			item* newItem = postac1->ring_slot2.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->ring_slot2.isEmpty = true;
		}
		else { printf("\n\t[DRUGI PIERŚCIONEK SLOT JEST PUSTY!]\n"); }
		break;
	case 't':	//zdjecie dwurecznego
		if (postac1->right_hand.wpn->isTwoHanded == true) {
			printf("\n\t[ZDJMUJESZ DWURECZNY PRZEDMIOT %s]\n", postac1->right_hand.wpn->name);
			item* newItem = postac1->right_hand.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->right_hand.isEmpty = true;
			postac1->left_hand.isEmpty = true;	//zdjecie przedmiotu z lewej reki, jesli dwureczny
		}
		else { printf("\n\t[PRZEDMIOT NIE JEST DWURECZNY!]\n"); }
		break;
	case 'z':	//zdjecie mikstury 1
		if (postac1->potion_slot.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ MIKSTURE %s]\n", postac1->potion_slot.wpn->name);
			item* newItem = postac1->potion_slot.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->potion_slot.isEmpty = true;
		}
		else { printf("\n\t[PIERWSZY SLOT MIKSTURY JEST PUSTY!]\n"); }
		break;
	case 'x':	//zdjecie mikstury 2
		if (postac1->potion_slot2.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ MIKSTURE %s]\n", postac1->potion_slot2.wpn->name);
			item* newItem = postac1->potion_slot2.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->potion_slot2.isEmpty = true;
		}
		else { printf("\n\t[DRUGI SLOT MIKSTURY JEST PUSTY!]\n"); }
		break;
	case 'c':	//zdjecie mikstury 3
		if (postac1->potion_slot3.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ MIKSTURE %s]\n", postac1->potion_slot3.wpn->name);
			item* newItem = postac1->potion_slot3.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->potion_slot3.isEmpty = true;
		}
		else { printf("\n\t[TRZECI SLOT MIKSTURY JEST PUSTY!]\n"); }
		break;
	case 'v':	//zdjecie mikstury 4
		if (postac1->potion_slot4.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ MIKSTURE %s]\n", postac1->potion_slot4.wpn->name);
			item* newItem = postac1->potion_slot4.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->potion_slot4.isEmpty = true;
		}
		else { printf("\n\t[CZWARTY SLOT MIKSTURY JEST PUSTY!]\n"); }
		break;
	case 'b':	//zdjecie mikstury 5
		if (postac1->potion_slot5.isEmpty == false) {
			printf("\n\t[ZDJMUJESZ MIKSTURE %s]\n", postac1->potion_slot5.wpn->name);
			item* newItem = postac1->potion_slot5.wpn;	//przypisanie itemu do zmiennej newItem
			pick_item(newItem, postac1);	//dodanie itemu do ekwipunku
			
			postac1->potion_slot5.isEmpty = true;
		}
		else { printf("\n\t[PIĄTY SLOT MIKSTURY JEST PUSTY!]\n"); }
		break;
	default:
		printf("\n\t[ZLE WYBRANY PRZEDMIOT DO ZDJECIA]\n");
		break;
	}
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
	show_ekwipunek(postac1);
	printf("\nq - wychodzenie, w - wkladanie itemow, e - zdejmowanie itemów, r - skillpointy, t - stats\n");
}


void widok_glowny(int mapx_size, int mapy_size, char** mapa, postac* postac1, int runda) {

	printf("_____________________________________________________________________________________________________________\n");
	printf("\tRunda = %d ", runda);
	show_map(mapa, mapx_size, mapy_size, postac1);
}

void krok(char* move, char** mapa, postac* postac1, int mapx_size, int mapy_size, int runda) {


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
			
			last_fight_round = runda;
			enemycount--;
			ekran_walki(postac1);
		}
		postac1->statystyki->moved++;
		postac1->statystyki->moved_up++;
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
			last_fight_round = runda;
			enemycount--;
			ekran_walki(postac1);
		}
		postac1->statystyki->moved++;
		postac1->statystyki->moved_down++;
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posx++;
		mapa[postac1->posx][postac1->posy] = 'P';

	}

	//		ruch w lewo

	if (move[0] == 'a') {

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
			last_fight_round = runda;
			enemycount--;
			ekran_walki(postac1);
		}
		postac1->statystyki->moved++;
		postac1->statystyki->moved_left++;
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
			last_fight_round = runda;
			enemycount--;
			ekran_walki(postac1);
		}
		postac1->statystyki->moved++;
		postac1->statystyki->moved_right++;
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posy++;
		mapa[postac1->posx][postac1->posy] = 'P';

	}
}

void czyszcznie_eq(postac* postac1, int id) {
	for (int i = 0; i < postac1->backpack_height; i++) {
		for (int j = 0; j < postac1->backpack_width; j++) {
			if (postac1->backpack[i][j].id == id) {
				postac1->backpack[i][j].id = 0;
				postac1->backpack[i][j].armorpoints = 0;
				postac1->backpack[i][j].def = 0;
			}
		}
	}
}

void put_armor_on(postac* postac1, int i, int j, int id) {
	// Alokacja pamięci jeśli wskaźnik jest pusty
	if (postac1->armor_slot.wpn == nullptr) {
		postac1->armor_slot.wpn = (item*)malloc(sizeof(item));
	}
	postac1->armor_slot.isEmpty = false;
	strcpy_s(postac1->armor_slot.wpn->name, sizeof(postac1->armor_slot.wpn->name), postac1->backpack[i][j].name);
	postac1->armor_slot.wpn->armorpoints = postac1->backpack[i][j].armorpoints;
	postac1->armor_slot.wpn->def = postac1->backpack[i][j].def;

	postac1->def += postac1->armor_slot.wpn->def;
	postac1->def += postac1->armor_slot.wpn->def * 0.5;

	czyszcznie_eq(postac1, id);
}
void put_potion_on(postac* postac1, int i, int j, int id) {
	int which;
	int areEmpty = 0;
	if (postac1->potion_slot.isEmpty == true) { printf("\n\t POTION SLOT 1: EMPTY"); areEmpty++; }
	if (postac1->potion_slot2.isEmpty == true) { printf("\n\t POTION SLOT 2: EMPTY"); areEmpty++; }
	if (postac1->potion_slot3.isEmpty == true) { printf("\n\t POTION SLOT 3: EMPTY"); areEmpty++; }
	if (postac1->potion_slot4.isEmpty == true) { printf("\n\t POTION SLOT 4: EMPTY"); areEmpty++; }
	if (postac1->potion_slot5.isEmpty == true) { printf("\n\t POTION SLOT 5: EMPTY"); areEmpty++; }

	if (areEmpty == 5) { printf("BRAK PUSTYCH SLOTOW\n"); }
	printf("Ktory slot? 1- pierwszy, 2 - drugi ...");
	scanf_s("%d", &which);

	switch (which) {
	case 1:
		if (postac1->potion_slot.wpn == nullptr) {
			postac1->potion_slot.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->potion_slot.isEmpty == true) {
			postac1->potion_slot.isEmpty = false;
			strcpy_s(postac1->potion_slot.wpn->name, sizeof(postac1->potion_slot.wpn->name), postac1->backpack[i][j].name);
			postac1->potion_slot.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->potion_slot.wpn->typ = postac1->backpack[i][j].typ;
			czyszcznie_eq(postac1, id);
		}
		break;
	case 2:
		if (postac1->potion_slot2.wpn == nullptr) {
			postac1->potion_slot2.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->potion_slot2.isEmpty == true) {
			postac1->potion_slot2.isEmpty = false;
			strcpy_s(postac1->potion_slot2.wpn->name, sizeof(postac1->potion_slot2.wpn->name), postac1->backpack[i][j].name);
			postac1->potion_slot2.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->potion_slot2.wpn->typ = postac1->backpack[i][j].typ;
			czyszcznie_eq(postac1, id);
		}
		break;
	case 3:
		if (postac1->potion_slot3.wpn == nullptr) {
			postac1->potion_slot3.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->potion_slot3.isEmpty == true) {
			postac1->potion_slot3.isEmpty = false;
			strcpy_s(postac1->potion_slot3.wpn->name, sizeof(postac1->potion_slot3.wpn->name), postac1->backpack[i][j].name);
			postac1->potion_slot3.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->potion_slot3.wpn->typ = postac1->backpack[i][j].typ;
			czyszcznie_eq(postac1, id);
		}
		break;
	case 4:
		if (postac1->potion_slot4.wpn == nullptr) {
			postac1->potion_slot4.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->potion_slot4.isEmpty == true) {
			postac1->potion_slot4.isEmpty = false;
			strcpy_s(postac1->potion_slot4.wpn->name, sizeof(postac1->potion_slot4.wpn->name), postac1->backpack[i][j].name);
			postac1->potion_slot4.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->potion_slot4.wpn->typ = postac1->backpack[i][j].typ;
			czyszcznie_eq(postac1, id);
		}
		break;
	case 5:
		if (postac1->potion_slot5.wpn == nullptr) {
			postac1->potion_slot5.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->potion_slot5.isEmpty == true) {
			postac1->potion_slot5.isEmpty = false;
			strcpy_s(postac1->potion_slot5.wpn->name, sizeof(postac1->potion_slot5.wpn->name), postac1->backpack[i][j].name);
			postac1->potion_slot5.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->potion_slot5.wpn->typ = postac1->backpack[i][j].typ;
			czyszcznie_eq(postac1, id);
		}
		break;
	default:
		printf("blad switch ");
		break;
	}
}
void put_wpn_on(postac* postac1, int i, int j, int id) {
	if (postac1->backpack[i][j].AS > postac1->speed) {
				printf("\n\t[Zbyt duza predkosc broni, nie mozesz jej uzyc]\n");
				return;
	}
	else {
		if (postac1->backpack[i][j].isTwoHanded == true) {
			if (postac1->right_hand.isEmpty == true && postac1->left_hand.isEmpty == true) {
				if (postac1->right_hand.wpn == nullptr) {
					postac1->right_hand.wpn = (item*)malloc(sizeof(item));
				}
				if (postac1->left_hand.wpn == nullptr) {
					postac1->left_hand.wpn = (item*)malloc(sizeof(item));
				}
				*postac1->right_hand.wpn = postac1->backpack[i][j];
				*postac1->left_hand.wpn = postac1->backpack[i][j];
				czyszcznie_eq(postac1, id);
				postac1->attack += postac1->right_hand.wpn->AP;
				postac1->speed -= postac1->right_hand.wpn->AS;
			}
			else {
				printf("\n\tBron dwureczna, brak wolnej dloni");

			}
		}
		else {
			char which;
			if (postac1->right_hand.isEmpty == true) { printf("\n\t LEWA REKA: EMPTY"); }
			if (postac1->left_hand.isEmpty == true) { printf("\n\t PRAWA REKA: EMPTY"); }
			if (postac1->right_hand.isEmpty == false && postac1->left_hand.isEmpty == false) { printf("BRAK PUSTYCH RAK\n"); }
			printf("Ktora reka? p- prawa, l - lewa");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {} // czyści bufor
			scanf_s("%c", &which);
			switch (which) {
			case 'p':
				if (postac1->right_hand.isEmpty == true) {
					if (postac1->right_hand.wpn == nullptr) {
						postac1->right_hand.wpn = (item*)malloc(sizeof(item));
					}
					*postac1->right_hand.wpn = postac1->backpack[i][j];
					postac1->right_hand.isEmpty = false;
					czyszcznie_eq(postac1, id);

					postac1->attack += postac1->right_hand.wpn->AP;
					postac1->speed -= postac1->right_hand.wpn->AS;
					postac1->def += postac1->right_hand.wpn->def;

				}
				else {
					printf("reka zajeta");
				}
				break;
			case 'l':
				if (postac1->left_hand.isEmpty == true) {
					if (postac1->left_hand.wpn == nullptr) {
						postac1->left_hand.wpn = (item*)malloc(sizeof(item));
					}
					if (postac1->backpack[i][j].isTwoHanded == false) {
						*postac1->left_hand.wpn = postac1->backpack[i][j];
						postac1->left_hand.isEmpty = false;
						czyszcznie_eq(postac1, id);

						postac1->attack += postac1->left_hand.wpn->AP;
						postac1->speed -= postac1->left_hand.wpn->AS;
						postac1->def += postac1->left_hand.wpn->def;
					}
				}
				else {
					printf("reka zajeta");
				}
				break;
			default:

				break;

			}
		}
	}
}
void put_ring_on(postac* postac1, int i, int j, int id) {
	int which;
	if (postac1->ring_slot.isEmpty == true) { printf("\n\t RING 1: EMPTY"); }
	if (postac1->ring_slot2.isEmpty == true) { printf("\n\t RING 2: EMPTY"); }
	if (postac1->ring_slot2.isEmpty == false && postac1->ring_slot.isEmpty == false) { printf("BRAK PUSTYCH SLOTOW\n"); }
	printf("Ktory slot? 1- pierwszy, 2 - drugi");
	scanf_s("%d", &which);
	switch (which) {
	case 1:

		if (postac1->ring_slot.wpn == nullptr) {
			postac1->ring_slot.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->ring_slot.isEmpty == true) {
			postac1->ring_slot.isEmpty = false;
			strcpy_s(postac1->ring_slot.wpn->name, sizeof(postac1->ring_slot.wpn->name), postac1->backpack[i][j].name);
			postac1->ring_slot.wpn->AS = postac1->backpack[i][j].AS;
			postac1->ring_slot.wpn->def = postac1->backpack[i][j].def;
			postac1->ring_slot.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->ring_slot.wpn->AP = postac1->backpack[i][j].AP;
			postac1->ring_slot.wpn->luck_modifier = postac1->backpack[i][j].luck_modifier;
			postac1->ring_slot.wpn->typ = postac1->backpack[i][j].typ;

			postac1->attack += postac1->ring_slot.wpn->AP;
			postac1->speed += postac1->ring_slot.wpn->AS;
			postac1->def += postac1->ring_slot.wpn->def;
			postac1->luck += postac1->ring_slot.wpn->luck_modifier;
			for (int i = 0; i < 49; i++) {
				postac1->max_hp[i] += postac1->ring_slot.wpn->hp_modifier;
			}
			czyszcznie_eq(postac1, id);
		}
		else {
			printf("TEN SLOT JEST ZAJETY");
		}
		break;
	case 2:
		if (postac1->ring_slot2.wpn == nullptr) {
			postac1->ring_slot2.wpn = (item*)malloc(sizeof(item));
		}
		if (postac1->ring_slot2.isEmpty == true) {
			postac1->ring_slot2.isEmpty = false;
			strcpy_s(postac1->ring_slot2.wpn->name, sizeof(postac1->ring_slot2.wpn->name), postac1->backpack[i][j].name);
			postac1->ring_slot2.wpn->AS = postac1->backpack[i][j].AS;
			postac1->ring_slot2.wpn->def = postac1->backpack[i][j].def;
			postac1->ring_slot2.wpn->hp_modifier = postac1->backpack[i][j].hp_modifier;
			postac1->ring_slot2.wpn->AP = postac1->backpack[i][j].AP;
			postac1->ring_slot2.wpn->luck_modifier = postac1->backpack[i][j].luck_modifier;
			postac1->ring_slot2.wpn->typ = postac1->backpack[i][j].typ;

			postac1->attack += postac1->ring_slot2.wpn->AP;
			postac1->speed += postac1->ring_slot2.wpn->AS;
			postac1->def += postac1->ring_slot2.wpn->def;
			postac1->luck += postac1->ring_slot2.wpn->luck_modifier;
			for (int i = 0; i < 49; i++) {
				postac1->max_hp[i] += postac1->ring_slot2.wpn->hp_modifier;
			}
			czyszcznie_eq(postac1, id);
		}
		else {
			printf("TEN SLOT JEST ZAJETY");
		}
		break;
	default:
		printf("error zakladania ringa");
		break;

	}


}
void no_enemy_check(int mapx, int mapy, char** mapa, int runda, postac* postac1) {
	if (enemycount < 1 && last_fight_round != runda) {
		int e_posx = rand() % mapx;
		int e_posy = rand() % mapy;

		if (postac1->posx != e_posx || postac1->posy != e_posy) {
			mapa[e_posx][e_posy] = 'E';
			enemycount++;
			printf("\n\t[POJAWIL SIE NOWY WROG!]\n\n");
		}

	}
}

void put_item_on(postac* postac1, char move[1]) {

	int last_item_id = -1;
	int id = 1;
	do {
		Sleep(500);
		system("cls");
		show_ekwipunek(postac1);
		printf("\n\t[Wybierz przedmiot do wlozenia na slot - w,s gora dol, d - zatwierdz]\n");
		printf("\n\t[Item nr %d]\n\n", id);
		scanf_s(" %c", &move[0]);
		if (move[0] == 's') { id++; }
		if (move[0] == 'w' && id != 1) { id--; }
	} while (move[0] != 'd' && move[0] != 'q');

	if (move[0] == 'd') {
		for (int i = 0; i < postac1->backpack_height; i++) {
			for (int j = 0; j < postac1->backpack_width; j++) {
				if (postac1->backpack[i][j].id == id) {

					switch (postac1->backpack[i][j].typ) {

					case 0:
						printf("potions slot\n");
						put_potion_on(postac1, i, j, id);
						break;
					case 1:	//bron slot
						if (postac1->speed > postac1->backpack[i][j].AS) {
							put_wpn_on(postac1, i, j, id);
							printf("\n\t[WLOZONO PRZEDMIOT %s NA SLOT BRONI]\n\n", postac1->backpack[i][j].name);
						}
						else { printf("\nITEM ZBYT CIEZKI\n"); }
						break;
					case 2:	//armor slot
						if (postac1->armor_slot.isEmpty == true) {
							put_armor_on(postac1, i, j, id);

							printf("\n\t[WLOZONO PRZEDMIOT %s NA ARMOR SLOT]\n\n", postac1->backpack[i][j].name);
						}
						else { printf("\n\t[NA ARMOR SLOCIE JEST JUŻ PRZEDMIOT!]\n\n"); }

						break;
					case 3:

						printf("WYBRALES PIERSCIEN\n");
						put_ring_on(postac1, i, j, id);
						break;
					default:
						printf("\n\t[ERROR - NIEZNANY TYPO PRZEDMIOTU]\n\n");
						break;

					}
				}
			}
		}
	}
}


void effects_impact(postac* postac1) {
	switch (postac1->effect)
	{
	case 0:		//healowanie
		if (postac1->health == postac1->max_hp[postac1->lvl - 1]) {		//healowanie gdy hp jest pelne (zapelnianie staminy i hunger)

			if (postac1->stamina < postac1->max_stamina && postac1->hunger >= 2) {			//stamina up
				postac1->stamina++;
				printf("\n\tSTAMINA UP\n");
				postac1->hunger = postac1->hunger - 2;
			}
			else if (postac1->hunger < postac1->max_hunger)
			{
				postac1->hunger++;
			}
		}
		else {		//hp nie jest pelne
			if (postac1->stamina >= 2) {	//jesli masz stamine sie healowac 
				postac1->stamina -= 2;
				postac1->health++;
				postac1->statystyki->hp_healed++;
				printf("\n\tHEALING\n");
			}
			else if (postac1->hunger >= 2) {		//nie masz stamine, masz hunger, wiec zmieniasz hunger w stamine
				postac1->stamina++;
				printf("\n\tSTAMINA UP\n");
				postac1->hunger = postac1->hunger - 2;
			}
			else { postac1->hunger++; }		//nie masz full hp, nie masz staminy ani hunger (>1)

		}
		break;
	case 1:			//poison
		postac1->health--;
		printf("\n\tPOISON ZABIERA 1 HP\n");
		postac1->effect_duration--;
		if (postac1->effect_duration == 0) {
			postac1->effect = 0;
			printf("\n\tKONIEC POISONA\n");
		}		//jesli skonczyl sie efekt, zmienia sie stan efectu()
		break;
	case 2:				//famine
		printf("\n\tFAMINE - NO HEALING\n");
		postac1->effect_duration--;
		if (postac1->effect_duration == 0)
			postac1->effect = 0;
		printf("\n\tKONIEC POISONA\n");
		break;
	default:
		printf("error switch effect_impact");
		break;
	}
}

int main() {

	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));

	int mapx_size = 10;
	int mapy_size = 15;

	char nazwa[10];

	char** mapa = (char**)malloc(sizeof(char*) * mapx_size);
	for (int i = 0; i < mapx_size; i++) {
		mapa[i] = (char*)malloc(sizeof(char) * mapy_size);
	}



	printf("\t[PODAJ SWOJE IMIE:]\n\t");
	scanf_s("%9s", nazwa, (unsigned)_countof(nazwa));
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}

	postac* postac1 = generate_postac(nazwa);

	generate_map(mapa, mapx_size, mapy_size, postac1);

	while (postac1->skillpoints > 0) {
		char move[1];
		postac1->statystyki->skillpoints_got = 20;
		skillpoint_distribution(postac1, move);
	}

	int runda = 1;

	while (postac1->health > 0) {


		widok_glowny(mapx_size, mapy_size, mapa, postac1, runda);
		char move[8];
		printf("Poruszaj się (wsad) albo otwórz ekwipunek (e): ");
		scanf_s(" %c", &move[0]);



		if (move[0] == 'e') {			//otwieranie ekwipunku
			char znak;
			do {
				pokaz_postac(postac1);
				scanf_s("%c", &znak, 1);

				if (znak == 'w') {
					char znak2[1];
					do {
						put_item_on(postac1, znak2);
					} while (znak2[0] != 'q');
				}
				if (znak == 'e') {
					char znak2[1];
					do {
						take_down_item(postac1, znak2);
					} while (znak2[0] != 'q');
				}
				if (znak == 'r') {
					char znak2[1];
					do {
						skillpoint_distribution(postac1, znak2);
					} while (znak2[0] != 'q');
				}
				
				if (znak == 't') {
					char znak2[1];
					do {
						system("cls");
						show_stats(postac1, znak2);
					} while (znak2[0] != 'q');
				}
				

			} while (znak != 'q');
			system("cls");
			continue;
		}

		krok(move, mapa, postac1, mapx_size, mapy_size, runda);

		no_enemy_check(mapx_size, mapy_size, mapa, runda, postac1);	//jesli na mapie nie ma wroga to tworzy go 

		effects_impact(postac1);

		runda++;
	}
	printf("    ____     _    __    __ _____    _____     _______ ____   \n"
		"   / ___|   / \\  |   \\/  | ____|  / _ \\ \\   / / ____ | _ \\	\n"
		"   | |  _  / _ \\ |  |\\/| |  _|   | | | \\ \\ / /|  _|  | |_) | \n"
		"   | |_| |/ ___ \\|  |  | | |___  | |_| |\\ V / | |___ | _ <  \n"
		"    ____/_/    \\_\\_ |  |_| _____| \\___ / \\_/  |_____ | _ | \\_\\ \n");
	char znak1[1];
	show_stats(postac1, znak1);

	free(postac1->ekwipunek);


	for (int i = 0; i < postac1->backpack_height; i++) {
		free(postac1->backpack[i]);
	}
	for (int i = 0; i < mapx_size; i++) {
		free(mapa[i]);
	}
	free(mapa);


	return 0;
}