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
	int szerokosc;
	bool isTwoHanded; // czy przedmiot jest dwureczny
	int typ; //0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień
};


struct Slot {
	//std::string item_name;  // Nazwa przedmiotu
	char item_name[10];
	int type;			//0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień
	bool isEmpty;           // Sprawdzenie, czy slot jest pusty
	item* wpn;             // Wskaźnik na broń
	
	Slot() : isEmpty(true), wpn(nullptr), item_name("XXX") {} // Konstruktor domyślny
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
	int effect; // 0 - normalny, 1- trucizna, 3 - głód 
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


enemy* generate_enemy() {
	enemy* en = (enemy*)malloc(sizeof(enemy));
	
	int rand_enemytype = rand() % 10 + 1;
	if (rand_enemytype > 6) {		//goblin
		en->lvl = 1;
		en->health = 3;
		en->attack = 1;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 5;
		en->AS = 1;
		en->race = 1;
		en->luck = 0;
		en->def = 1;
			
	}
	else if (rand_enemytype > 3) {	//zombie
		en->lvl = 1;
		en->health = 5;
		en->attack = 2;
		en->mana = 0;
		en->type = 1;
		en->xpdrop = 7;
		en->AS = 1;
		en->race = 2;
		en->luck = 0;
		en->def = 3;
	}
	else if(rand_enemytype > 1) {		//ork
		en->lvl = 2;
		en->health = 8;
		en->attack = 5;
		en->mana = 0;
		en->type = 2;
		en->xpdrop = 10;
		en->race = 3;
		en->luck = 0;
		en->def = 5;
		en->AS = 3;

	}
	else if (rand_enemytype == 1) {		//wizard
		en->lvl = 5;
		en->health = 5;
		en->attack = 8;
		en->mana = 10;
		en->type = 2;
		en->xpdrop = 25;
		en->AS = 4;
		en->race = 4;
		en->luck = rand() % 5 + 1;
		en->def = 0;
	}
	
	return en;
}


postac* generate_postac(char nazwa[10]) {
	postac* post = (postac*)malloc(sizeof (postac));
	post->backpack_height = 10;
	post->backpack_width = 12;

	strcpy_s(post->name, sizeof(post->name), nazwa);
	post->posx = 1;
	post->posy = 1;
	post->xp = 0;
	post->effect = 0;
	post->cash = 0;
	post->skillpoints = 20;
	
	post->lvl = 1;
	post->attack = 0;
	post->health = 20;
	post->stamina = 10;
	post->speed = 0;	
	post->luck = rand()%10;
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
	rg->weight = 1;
	rg->typ = 3;
	rg->szerokosc = 1;

	

	int rand_ = rand() % 1000 + 1;
	rg->lvl = postac1->lvl;
	if (rand_ + postac1->luck < 50) { rg->lvl = postac1->lvl + 1; }
	if (rand_ + postac1->luck < 20) { rg->lvl = postac1->lvl + 2; }
	if (rand_ + postac1->luck <  1) { rg->lvl = postac1->lvl + 5; }

	

	rand_ = rand() % 1000;
	rand_ += postac1->luck;
	if (rand_ > 0 && rand_ < 200) {
		rg->AP = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Attack ring");}//to attack ring 20%
	if (rand_ >= 200 && rand_ < 400) {
		rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Deffence ring");}//deff ring 20%
	if (rand_ >= 400 && rand_ < 600) {
		rg->AS = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Speed ring");} //speed ring 20%
	if (rand_ >= 600 && rand_ < 690) {
		rg->luck_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl; strcpy_s(rg->name, sizeof(rg->name), "Luck ring");} //luck ring 9%
	if (rand_ >= 690 && rand_ < 700) {
		rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Full ring");			//full ring 1%
		rg->luck_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl;
		rg->AS = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
		rg->def = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
		rg->AP = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
		rg->hp_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5;
	}
	else if (rand_ >= 700  && rand_ < 1000) {
		rg->hp_modifier = rand() % 5 + 1 + postac1->luck * 0.1 + rg->lvl * 5; strcpy_s(rg->name, sizeof(rg->name), "Health ring");}	//health ring 30%
	
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
		rg->AS = rg->def * 5 * postac1->luck * 2 ;
		rg->luck_modifier = rg->luck_modifier * 5 * postac1->luck * 2;
		rg->hp_modifier = rg->hp_modifier * 5 * postac1->luck * 2;
		break;
	default:
		break;
	}
	
	rg->klasa = klasa;
	
return rg;}
item* generate_weapon(postac* postac1, int klasa) {
	item* wpn = (item*)malloc(sizeof (item));


	wpn->AP = rand() % 10 + 1;		//attack power
	wpn->AS = rand() % 15 + 1;
	wpn->weight = rand() % 60 + 1;
	wpn->szerokosc = rand() % 3 + 1;

	wpn->armorpoints = 0;
	wpn->klasa = 1;
	strcpy_s(wpn->name, sizeof(wpn->name), "bron");
	wpn->lvl = 1;
	wpn->health_reg = 0;
	wpn->luck_modifier = 1;

	wpn->potion_type = 0; //nie potion

	return wpn;
}
item* generate_potion(postac* postac1, int klasa) {
	item* pot = (item*)malloc(sizeof (item));




	int potion_type = rand() % 3 + 1;	//0 - nie potion, 1 -  health, 2 - luck boost, 3 - atack boost
	
	strcpy_s(pot->name, sizeof(pot->name), "Potion");
	pot->klasa = 1;
	pot->szerokosc = 1;
	pot->weight = 1;
	pot->armorpoints = 0;
	return pot;
}
item* generate_armor(postac* postac1, int klasa) {
	item* armr = (item*)malloc(sizeof (item));
	strcpy_s(armr->name, sizeof(armr->name), "zbroja");
	armr->lvl = postac1->lvl;
	armr->armorpoints = rand() % 5 + 1 * armr->lvl;
	armr->szerokosc = rand() % 5 + 1;
	armr->klasa = 1;
	return armr;


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
	default :
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

void walka(postac* postac1, enemy* wrog, int tura, int kto_pierwszy) {
  
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

				if (rzutt >= 5.5)                { sila_ataku = postac1->attack + postac1->luck; powodzenie_ataku = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = postac1->attack;                 powodzenie_ataku = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = postac1->attack  *0.5;           powodzenie_ataku = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.1 * postac1->attack;           powodzenie_ataku = 2; }	//2
				if (rzutt< 1.5)                  { sila_ataku = 0.0;                             powodzenie_ataku = 1; }		//1
				
				rzut = rand() % 6 + 1;		//nastepuje rzut obroncy
				rzutt = rzut + 0.1 * wrog->luck;

				if (rzutt >= 5.5)			     { sila_ataku = 0.4 * sila_ataku;   powodzenie_obrony = 6; }		//6
				if (rzutt >= 4.5 && rzutt < 5.5) { sila_ataku = 0.5 * sila_ataku;   powodzenie_obrony = 5; }		//5
				if (rzutt >= 2.5 && rzutt < 4.5) { sila_ataku = 0.7 * sila_ataku;   powodzenie_obrony = 4; }					//3,4
				if (rzutt >= 1.5 && rzutt < 2.5) { sila_ataku = 0.9 * sila_ataku;   powodzenie_obrony = 2; }	//2
				if (rzutt < 1.5)                 {								    powodzenie_obrony = 1; }		//1
				sila_ataku = sila_ataku - 0.1 * wrog->def;

				if (sila_ataku < 0) { sila_ataku = 0; }

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

				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n");
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

				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n", tura);
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

				sila_ataku = sila_ataku - 0.1 * wrog->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				wrog->health = wrog->health - sila_ataku;
				printf("\t[ATAKUJESZ]\n", tura);
				atak_info(sila_ataku, powodzenie_ataku, powodzenie_obrony);
			}
			break;
		case 2:
			if (tura % 2 == 0) {		//bohater atakuje 2 razy, tura wroga
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

				sila_ataku = sila_ataku - 0.1 * postac1->def;
				if (sila_ataku < 0) { sila_ataku = 0; }
				postac1->health = postac1->health - sila_ataku;
				printf("\t[WROG ATAKUJE]\n");
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
}

int item_class_generator(postac* postac1) {
	int klasa_itema = rand() % 100 + 1 + postac1->luck;	//losowanie klasy przedmiotu
	if (klasa_itema >= 0 && klasa_itema < 21) { klasa_itema = 0; }	//klasa - przerdzewiały  20% szans
	if (klasa_itema >= 21 && klasa_itema < 71) { klasa_itema = 1; }	//klasa - zwykly 50% szans
	if (klasa_itema >= 71 && klasa_itema < 91) { klasa_itema = 2; }	//klasa - rzadki 20% szans
	if (klasa_itema >= 91 && klasa_itema < 99) {
		klasa_itema = 3; //klasa - epicki 9% szans
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
		printf("blad switcha %s ",newItem->name);
		break;
	}
	if (newItem->AP > 0) {
		printf("ATK: %d ", newItem->AP);}
	if (newItem->armorpoints > 0) {
		printf("ARMOR: %d ", newItem->armorpoints);}
	if (newItem->AS > 0) {
		printf("SPD: %d ", newItem->AS);}
	if (newItem->luck_modifier > 0) {
		printf("LCK: %d ", newItem->luck_modifier);}
	if (newItem->def > 0) {
		printf("DEF: %d ", newItem->def);}
	printf("]");
	printf("\ng - wez do ekwipunku, inny klawisz - porzuc [NIE MOZNA COFNAC]\n");
	char move[2];
	getchar(); // pobiera i ignoruje zaległy znak nowej linii
	scanf_s("%c", &move[0], 1);
	if(move[0] == 'g') {
	int szerokosc = newItem->szerokosc;
	for (int i = 0; i < postac1->backpack_height; i++) {
		for (int j = 0; j < postac1->backpack_width; j++) {
			if (postac1->backpack[i][j].id == 0 && szerokosc > 0) {//sprawdzanie czy jest puste
				postac1->backpack[i][j].id = item_id;
				postac1->backpack[i][j].AP = newItem->AP;
				postac1->backpack[i][j].AS = newItem->AS;
				strcpy_s(postac1->backpack[i][j].name, sizeof(postac1->backpack[i][j].name), newItem->name);
				postac1->backpack[i][j].weight = newItem->weight;
				postac1->backpack[i][j].def = newItem->def;
				postac1->backpack[i][j].armorpoints = newItem->armorpoints;
				postac1->backpack[i][j].lvl = newItem->lvl;
				postac1->backpack[i][j].health_reg = newItem->health_reg;
				postac1->backpack[i][j].luck_modifier = newItem->luck_modifier;
				postac1->backpack[i][j].hp_modifier = newItem->hp_modifier;
				postac1->backpack[i][j].isWeapon = newItem->isWeapon;
				postac1->backpack[i][j].isArmor = newItem->isArmor;
				postac1->backpack[i][j].isRing = newItem->isRing;
				postac1->backpack[i][j].isPotion = newItem->isPotion;
				postac1->backpack[i][j].potion_type = newItem->potion_type;
				postac1->backpack[i][j].szerokosc = newItem->szerokosc;
				postac1->backpack[i][j].klasa = newItem->klasa;
				postac1->backpack[i][j].typ = newItem->typ;
				postac1->backpack[i][j].isTwoHanded = newItem->isTwoHanded;
				szerokosc--;
				item_id++;
				return;
			}
		}
	}
	
	}
}

void post_fight(postac* postac1, enemy* wrog) {
	postac1->xp = postac1->xp + wrog->xpdrop;
	int cash_drop = rand() % 10 + 1 * wrog->type;
	postac1->cash = postac1->cash + cash_drop * postac1->luck;


	printf("\n\n\t[WROG POKONANY|%dXP|%d$]\n\n",wrog->xpdrop, cash_drop*postac1->luck);

	int item_drop = rand() % 11 + postac1->luck * 0.1;	//losowanie przedmiotu
		if(item_drop >= 10){

			int klasa_itema = item_class_generator(postac1);	//losowanie klasy przedmiotu

			int type_rand = rand() % 10;	//losowanie typu przedmiotu
			int item_type = 0;//0 - potion, 1 - broń, 2 - zbroja, 3 - pierścień

			if(type_rand >= 0 && type_rand < 2) {	//20% na bron			0,1
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
		break;
	default:
		break;
	}
	postac1->lvl++;
	Sleep(1000);
}

void ekran_walki(postac* postac1) {
	enemy* wrog = generate_enemy();
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

	
	while(wrog->health > 0 && postac1->health >0) {
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
			"	  /____⧵___/|_| |_| |_|_.__/|_|⧵___|	\n",wrog->health,postac1->health, wrog->AS, postac1->speed,
			wrog->attack, postac1->attack, wrog->def, postac1->def);
		
		break;
	case 3:
		printf(
			"	        \n"
			"									ENEMY	|	HERP \n"
			"										|\n"
			"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\t\t|\n"
			"	 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀			\t\tHP %d|\n"
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
			"		 ⧵___/|_|  |_ |⧵_⧵			\n", wrog->health
		);
		break;
	case 4:
		printf(
												
													
			"			     _,-'|		\t|\n"		
			"			 ,-' ._  |		ENEMY	|	HERP \n"	
			"		 . || ,   |####⧵ |			|\n"
			"		 ⧵.`', /  ⧵####| |		HP : %d \t|\t%d\n"
			"	   	 =, . =   | ###| |		SPD: %d \t|\t%d\n"
			"		 / || ⧵ , -'⧵#/,'`.		ATK: %d \t|\t%d\n"
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
			"		 ⧵_/⧵_/  |_/___⧵__,_|_|  ⧵__,_| \n", wrog->health, postac1->health, wrog->AS, postac1->speed,
			wrog->attack, postac1->attack, wrog->def, postac1->def);
		break;
	default:
		break;
	}
	Sleep(1000);
	printf("\nTURA WALKI: %d\n",tura);
	walka(postac1, wrog, tura, kto_pierwszy);
	
	tura++;
	}
	if (postac1->health > 0) {post_fight(postac1, wrog);}
	
	while(postac1->xp >= postac1->max_xp[postac1->lvl - 1]){
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

void show_map(char** mapa, int mapx_size, int mapy_size, postac* postac1){
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
			if(postac1->skillpoints > 0){
				printf("\t[MASZ %d SKILLPOINTOW!!!]\n", postac1->skillpoints);
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
	printf(
		"		       {}			[IMIE] = %s\n"
		"		      .--.		    \t[LEVEL] = %d\t\t[ARMOR-SLOT] = ", postac1->name, postac1->lvl);
		if (postac1->armor_slot.isEmpty == false) {
		printf("%s\n", postac1->armor_slot.item_name);
		}
		else { printf("[XXX]\n"); }
	printf(
		"		     /.--.⧵			[HEALTH] = %d\n"
		"		    |= == =|		\t[ATTACK] = %d\t\t[LEFT  HAND] = ",  postac1->health, postac1->attack); 
		if (postac1->left_hand.isEmpty == false) {
		printf("%s\n", postac1->left_hand.item_name);
		}
		else { printf("[XXX]\n"); }
		printf(
		"		    | `::` |		\t[X POINTS] = %d		\n"
		"		  .-;`⧵..../ `;-.	\t[STAMINA] = %d\t\t[RIGHT HAND] = ", postac1->xp, postac1->stamina);
		if (postac1->right_hand.isEmpty == false) {
			printf("%s\n", postac1->right_hand.item_name);
		}
		else { printf("[XXX]\n"); }
		printf(
		"		  /  |...::... |  ⧵	\t[HUNGER]  = %d	\n"
		"		  |  /''': :'''⧵   |	\t[DEF] = %d\t\t[FIRST RING] = ", postac1->hunger, postac1->def);
		if (postac1->ring_slot.isEmpty == false) {
			printf("%s\n", postac1->ring_slot.item_name);
		}
		else { printf("[XXX]\n"); }
		printf(
		"		  ; --'⧵   ::  /⧵--;	\t[SPEED] = %d\n"
		"	         <__>,>._::_. <,<__>    \t[LUCK] = %d\t\t[SCND. RING] = ",postac1->speed, postac1->luck);
		if (postac1->ring_slot2.isEmpty == false) {
			printf("%s\n", postac1->ring_slot2.item_name);
		}
		else { printf("[XXX]\n"); }
		printf(
		"	       |  |/   ^^    ⧵|  |\t\t[CASH] = %d	\n"
		"	       ⧵::/|         |⧵::/	\n"
		"	       |||⧵|         |/|||	\n"
		"	       ''' |___/ ⧵___| '''	\n"
		"                   ⧵_ |  | _ /	\t\t[POTIONS] = ", postac1->cash);
		if(postac1->potion_slot.isEmpty == false){
			printf("%s ",postac1->potion_slot.item_name);
		}
		else { printf("[XXX]  "); }
		if (postac1->potion_slot2.isEmpty == false) {
			printf("%s ", postac1->potion_slot.item_name);
		}
		else { printf("[XXX]  "); }
		if (postac1->potion_slot3.isEmpty == false) {
			printf("%s ", postac1->potion_slot.item_name);
		}
		else { printf("[XXX]  "); }
		if (postac1->potion_slot4.isEmpty == false) {
			printf("%s ", postac1->potion_slot.item_name);
		}
		else { printf("[XXX]  "); }
		if (postac1->potion_slot5.isEmpty == false) {
			printf("%s \n", postac1->potion_slot.item_name);
		}
		else { printf("[XXX]  \n"); }
		printf(
		"	            <_ >< _>				\n"
		"	            |  ||  |					\n"
		"	            |  ||  |					\n"
		"	           _⧵.:||:./_			\t"); if(postac1->skillpoints > 0){printf("MASZ %d SKILLPOINTOW", postac1->skillpoints); }printf(
		"\n		  /____/ ⧵____⧵		\n\n\n"
		"_____________________________________________________________________________________________________________\n"
		 );
}

void skillpoint_distribution(postac* postac1, char move[1]) {
	model_hero(postac1);
	printf("Wybierz co ulepszyc - a:attack,  d:def, s: speed\n");
	scanf_s(" %c", &move[0]);
	
	if (move[0] == 'a') { postac1->attack += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES ATTACK!\n");} else
	if(move[0]== 'd'){  postac1->def += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES DEF!\n");} else
	if (move[0] == 's') { postac1->speed += 1; postac1->skillpoints -= 1; printf("ULEPSZYLES SPEED!\n"); }
	else { printf("blad skillpoint distribution"); }

}

void show_ekwipunek(postac* postac1) {
	printf("Ekwipunek postaci:\n");
	int last_item_id = -1;
	for (int i = 0; i < postac1->backpack_height; i++) {
		for (int j = 0; j < postac1->backpack_width; j++) {
			if (postac1->backpack[i][j].id != 0 && postac1->backpack[i][j].id != last_item_id) {
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
				printf("\n");



			}
			
		}
	} 
	if (last_item_id == -1) { printf("brak przedmiotów"); }
	 
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
	printf("\nq - wychodzenie, w - wkladanie itemow, e - podejrzyj przedmioty, r - zdejmowanie przedmiotów, t - skillpointy\n");
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
			last_fight_round = runda;
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
			last_fight_round = runda;
			enemycount--;
			ekran_walki(postac1);
		}
		mapa[postac1->posx][postac1->posy] = '.';
		postac1->posy++;
		mapa[postac1->posx][postac1->posy] = 'P';

	}	
}

void no_enemy_check(int mapx, int mapy, char** mapa, int runda) {
	if (enemycount < 1 && last_fight_round != runda) {
		int e_posx = rand() % mapx;
		int e_posy = rand() % mapy;

		mapa[e_posx][e_posy] = 'E';
		enemycount++;
		printf("\n\t[POJAWIL SIE NOWY WROG!]\n\n");
	}
}

int main(){
	
	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));

	int mapx_size = 10;
	int mapy_size = 15;

	char nazwa[10];

	char** mapa = (char**)malloc(sizeof(char*) * mapx_size);
	for (int i = 0; i < mapx_size; i++) {
		mapa[i] = (char*)malloc(sizeof(char) * mapy_size);
	}

	printf ("\t[PODAJ SWOJE IMIE:]\n\t");
	scanf_s("%9s", nazwa, (unsigned)_countof(nazwa));
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}

	postac* postac1 = generate_postac(nazwa);

	generate_map(mapa, mapx_size, mapy_size, postac1);

	while (postac1->skillpoints > 0) {
		char move[1];
		
		skillpoint_distribution(postac1, move);
	}

	

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
		char move[8];
		printf("Poruszaj się (wsad) albo otwórz ekwipunek (e): ");
		scanf_s(" %c", &move[0]);
		
		
				
		if(move[0] == 'e') {			//otwieranie ekwipunku
			char znak;
			do {
				pokaz_postac(postac1);
				scanf_s("%c", &znak);
				
			} while(znak != 'q');
			system("cls");
			continue;
		}		 
					
		krok(move, mapa, postac1, mapx_size, mapy_size, runda);

		//enemy_check()		//sprawdza czy wrog jest w poblizu i podaje jego dane

		no_enemy_check(mapx_size, mapy_size, mapa, runda);	//jesli na mapie nie ma wroga to tworzy go 

		//effects_impact();		//
		
		runda++;
	}
	printf("    ____     _    __    __ _____    _____     _______ ____   \n"
		   "   / ___|   / \  |   \/  | ____|  / _ \ \   / / ____ | _ \	\n"
		   "   | |  _  / _ \ |  |\/| |  _|   | | | \ \ / /|  _|  | |_) | \n"
		   "   | |_| |/ ___ \|  |  | | |___  | |_| |\ V / | |___ | _ <  \n"
		   "    ____/_/    \_\_ |  |_| _____| \___ / \_/  |_____ | _ | \_\ \n");

	free(postac1->ekwipunek);
	
	
	for (int i = 0; i < postac1->backpack_height; i++) {
		free (postac1->backpack[i]) ;
	}
	for (int i = 0; i < mapx_size; i++) {
		free(mapa[i]);
	}
	free(mapa);
	

	return 0;
}

