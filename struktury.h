#ifndef STRUKTURY_H
#define STRUKTURY_H

#include "stale.h"

typedef struct
{
	int wymx, wymy, szarosci;          /* wymiary, odcienie szarosci*/
  char nr_magiczny;                   
	int *pix;		                      /* wskazniki do dyn. alokacji pamieci (tab[wymx][wymy])*/
	int **pix_kolor;
	char komentarz[ILE_LINII][DL_LINII];  /* tablica przechowująca komentarze */
}plik_pgm_ppm;

typedef struct 
{	
  char param;                     // Przechowuja każdy kolejny argument użytkownika
  float prog;                     // przy progrowaniu
  float poziom1;                  // Przechowują poziomy przy
  float poziom2;                  // zmianie poziomów
  int odczytano, zapisano;        // 1 lub 0
  int wyswietlono;                // 1 lub 0
  char *nazwa;                    // pliku do odczytu
  char *nazwa_out;                // do zapisu
  char *nazwa_tmp;                // to utworzenia pliku tmp
  FILE *plik;                     // uchwyt na plik do czytania
  FILE *plik_out;                 // uczhwyt na plik do zapisu
}wybory;

#endif /* !STRUKTURY_H */