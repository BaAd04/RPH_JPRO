#include <stdlib.h>
#include <time.h>
#include <iostream>

int liczba;
int x;

int wypisz(int &liczba) {
    if (liczba == 0) {
        x = rand() % 2;
        printf("Poczatkowe x = %d", x);
    }
    else { x = 1 - x; }
    liczba++; 
    return liczba;
}

int maksind(int &size, int &tab) {
    int max = 0,
        avg,
        max_ind;
    
    for (int i = 0; i < size; i++) {
        if (*tab[i] > max) {
            max = *tab[i];
            max_ind = i;
        }
        avg = avg + *tab[i];
    }
    avg = avg / size;
    return max;
}



int main()
{
    srand(time(NULL));

    //double** tablica = (double**)malloc(1 * sizeof(double));
    //for (int i = 0; i < 2; i++) {
         //*tablica[i] = rand() % 200 - 99; //wartosc miedzy -100 a +100
         // printf("%f", tablica[i]);}     
        
        int n = rand() % 100000 + 1;
        int liczba = 0;
        for (int i = 0; i < n; i++) {
            int licz = wypisz(liczba);
        }
        printf(" Liczba powtorzen %d %d \n", liczba, x);
        

        int size = rand() % 10 + 1;
        printf("size = %d \n", size);
        int* tab = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            tab[i] =  rand() % 100 + 1;     //tworzymy tablice 1D o int ach od 1 do 100 o dlugosci los. size 1-10
            //printf("%d \n", tab[i]);
        } 
        int max_ind = maksind(size);


    
}


