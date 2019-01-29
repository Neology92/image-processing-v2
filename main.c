
/*
/     Autor: Oskar Legner
/     Temat: Przetwarzanie obrazow 2
/     Data ukonczenia (wykonania): 5 stycznia 2019
/
/
/
/*    Poczatek kodu zrodlowego    */

#include "main.h"


/************************************************************************************
 * Program wypisywana na wywołaniu argumenty i na ich podstawie określa działania.  *
 * Składnia wywołania:                                                              *
 *    ./program [--help] |                                                          * 
 *    {[-i name] [-o name] [-p procent] [-n] [-k] [-rx] [-ry] [-d] [-z poz1 poz2] } *
 *                                                                                  *
 * Program przyjmuje i przetwarza obrazy PGM i PPM.                                 *
 * Funkcje: czytaj, zapisz i wyswietl w pełni przetwarzają obrazy PPM, natomiast    *
 * negatyw, konturowanie, rozmycie, progowanie, zmiana poziomow -                   *
 * najpierw konwertują pliki PPM na PGM.                                            *
 *                                                                                  *
 *                                                                                  *
 *                                                                                  *
 ************************************************************************************/


/************************************************************************************
 * Główna funkcja programu.                                                         *
 *                                                                                  *
 * \param[in] argc - liczba argumentów + nazwa programu                             *
 * \param[in] argv - argumenty wywołania i ich wartości, podane przez użytkownika   *
 * \return 0 (Przy poprawnym działaniu prog.) lub -1 (przy komunikatach o błędach)  * 
 ************************************************************************************/
int main(int argc, char **argv) 
{
  int i;
  plik_pgm_ppm obraz;
  wybory opcja;

  zerowanie(&opcja);  


    if(argc == 1)   //Sprawdzenie, czy zostały podane argumenty wywołania
    {
      fprintf(stderr, "Należy podać argumenty wywołania! Użyj --help, aby zobaczyć szczegóły.\n" );
      fprintf(stderr, "Zakończono działanie programu!\n" );
      return(ERR);
    }

    for(i=0; i < argc; i++) //Sprawdzenie, czy nie został podany argument "--help"
    {
      if(strcmp(argv[i],"--help") == 0 ) 
      {
        guide(); //Wyowłanie poradnika
        return(OK);
      }
    }


    while(--argc > 0)
    {
      if(*(*++argv) == '-')
      {
        switch (opcja.param = *++(*argv))
        {
          case 'i': //wczytaj
                  /*Jesli juz wczesniej wczytano obraz*/
                  if(opcja.odczytano)
                  {
                    zerowanie(&opcja);  
                    
                    if(obraz.nr_magiczny == '2')
                      free(obraz.pix); // Usunięcie poprzedniego obrazu
                    else if(obraz.nr_magiczny == '3')
                    {
                      for(i=0; i < obraz.wymx*obraz.wymy; i++)
                         free(obraz.pix_kolor[i]);
                      free(obraz.pix_kolor);
                    }

                  }

                  if(!opcja.odczytano)
                  {
                    
                    if(!(--argc))
                    {
                      fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                    if (*(++argv)[0] == '-')
                    {
                      if (strlen(*argv) != 1)
                      {
                        fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                        fprintf(stderr,"Zakończono działanie programu!\n");
                        return(ERR);
                      }
                      else
                      {
                        opcja.odczytano = czytaj_stdin(&obraz); //Czytaj z stdin, jesli po -i występuje samo '-'
                        opcja.nazwa = "z stdin"; //Podpisanie pochodzenia obrazu
                      }
                    }
                    else
                    {
                      opcja.nazwa = *argv;    //Pobranie nazwy 
                      opcja.plik=fopen(opcja.nazwa,"r"); //otworzenie pliku o nazwie pobranej ze stdin w funkcji czytaj_menu()

                      if (opcja.plik != NULL) 
                      {         
                         /* Wczytanie zawartosci wskazanego pliku do pamieci */
                        opcja.odczytano = wczytaj(opcja.plik,&obraz);
                        fclose(opcja.plik);  //Zamkniecie pliku
                      }
                    }

                    if(!opcja.odczytano) //Gdy błąd - poinformowanie uzytkownika i zakonczenie programu
                    {
                      fprintf(stderr,"Blad: Nie udało się wczytywać pliku \"%s\".\n",opcja.nazwa); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }
                    else
                      fprintf(stderr,"Wczytano plik \"%s\".\n",opcja.nazwa);
                  }
                break;
          
          case 'o': //zapisz
                if(opcja.odczytano)
                {
                    if(!(--argc))
                    {
                      fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                    if (*(++argv)[0] == '-')
                    {
                      fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                  opcja.nazwa_out = *argv;    //Pobranie nazwy 
                  opcja.plik_out=fopen(opcja.nazwa_out,"w"); //Otworzenie pliku o nazwie podanej przez uzytkownika
                  
                  opcja.zapisano=0;

                  if (opcja.plik_out != NULL) 
                  { 
                                    /* Zapisanie zawartosci tablicy do  pliku */
                    opcja.zapisano = zapisz(opcja.plik_out,obraz);
                    fclose(opcja.plik_out); //zamkniecie pliku
                  }   

                  if(!opcja.zapisano) //Poinformowanie uzytkownika jesli wystapil blad i zakonczenie dzialania programu
                  {
                    fprintf(stderr,"Blad: Nie udalo sie zapisac pliku!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Zapisano do pliku \"%s\"\n",opcja.nazwa_out );
                  }
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }
              break;

          case 'd': //wyswietl
                if(opcja.odczytano)
                {  
                  opcja.plik_out=fopen(opcja.nazwa_tmp,"w");

                  if (opcja.plik_out != NULL) 
                  { 
                                  /* Zapisanie zawartosci tablicy do  pliku */
                    opcja.wyswietlono = zapisz(opcja.plik_out,obraz);
                    fclose(opcja.plik_out);
                  } 

                  if(!opcja.wyswietlono)
                  {
                    fprintf(stderr,"Blad: Nie udalo sie utworzyc pliku tmp.pgm!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Poprawnie wyswietlono obraz.\n");
                    opcja.wyswietlono = 0; //Przygotowanie do kolejnej akcji wyswietlania
                  }
                  
                    /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
                  wyswietl(opcja.nazwa_tmp);
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }
              break;

          case 'n': //negatyw
                if(opcja.odczytano)
                {
                  if(obraz.nr_magiczny == '3')  //Przekonwertowanie obrazu na czarno-biały
                      konwertuj_ppm_pgm(&obraz);

                          /* Odwracanie wartosci tablicy wzgledem zakresu */
                  if(!negatyw(&obraz))
                  {
                    fprintf(stderr,"Blad: Nie udalo sie nalozyc negatywu na obraz!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Nalozono negatyw.\n");
                  }
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }
              break;
      
          case 'k': //konturowanie
              if(opcja.odczytano)
                { 
                  if(obraz.nr_magiczny == '3')  //Przekonwertowanie obrazu na czarno-biały
                      konwertuj_ppm_pgm(&obraz);

                  if(!konturowanie(&obraz))       /* Nalozenie konturowania */
                  {
                    fprintf(stderr,"Blad: Nie udalo sie nalozyc konturowania na obraz!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Nalozono konturowanie.\n");
                  }                
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }                
              break;

          case 'p': //progowanie
                if(opcja.odczytano)
                {
                  if(obraz.nr_magiczny == '3')  //Przekonwertowanie obrazu na czarno-biały
                      konwertuj_ppm_pgm(&obraz);

                    if(!(--argc))
                    {
                      fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                    if (*(++argv)[0] == '-')
                    {
                      fprintf(stderr,"Blad: Brak wartosci (lub ujemna wartosc) po argumencie \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }
                    
                      /* Pobranie wartosci progowania */
                  sscanf(*argv, "%f", &opcja.prog);

                  if (opcja.prog>100||opcja.prog<0) // Sprawdzenie czy miesci sie w przedziale
                  {
                    fprintf(stderr,"Blad: Niepoprawny prog! Powinien miescic sie w przedziale [0;100]\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }

                  if(!progowanie(&obraz,opcja.prog))  /* Nalozenie progowania i podanie komunikatu */
                  {
                    fprintf(stderr,"Blad: Nie udalo sie zastosowac progrwania na obrazie!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Nalozono progowanie, o progu: %.1f%% \n",opcja.prog);
                  }                
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }
              break;

          case 'r': //rozmycie
              if(opcja.odczytano)
                {
                  if(obraz.nr_magiczny == '3')  //Przekonwertowanie obrazu na czarno-biały
                      konwertuj_ppm_pgm(&obraz);

                  switch (*++(*argv))
                  {    
                    case 'x':  //poziome
                          if(!rozmycie_pion(&obraz))             /* Wywolanie fnkcji rozmywajacej */
                          {
                            fprintf(stderr,"Blad: Nie udalo sie zastosowac rozmycia poziomego na obrazie!\n"); // Obsługa błędów
                            fprintf(stderr,"Zakończono działanie programu!\n");
                            return(ERR);
                          }
                          else
                          {
                            fprintf(stderr, "Nalozono rozmycie poziome.\n" );
                          }
                        break;
                         
                    case 'y': //pionowe
                          if(!rozmycie_poz(&obraz))              /* Wywolanie funkcji rozmywajacej */
                          {
                            fprintf(stderr,"Blad: Nie udalo sie zastosowac rozmycia pionowego na obrazie!\n"); // Obsługa błędów
                            fprintf(stderr,"Zakończono działanie programu!\n");
                            return(ERR);
                          }
                          else
                          {
                            fprintf(stderr, "Nalozono rozmycie pionowe.\n" );
                          }
                        break;

                    default:
                          fprintf(stderr, "Blad: Nie podano poprawnego typu rozmycia!\n"); // Obsługa błędów
                          fprintf(stderr,"Zakończono działanie programu!\n");
                          return(ERR);
                        break;
                  }                  
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }  
              break;

          case 'z': //zmiana poziomow
                if(opcja.odczytano)
                {
                  if(obraz.nr_magiczny == '3')  //Przekonwertowanie obrazu na czarno-biały
                      konwertuj_ppm_pgm(&obraz);

                    if(!(--argc))
                    {
                      fprintf(stderr,"Blad: Brak wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                    if (*(++argv)[0] == '-')
                    {
                      fprintf(stderr,"Blad: Brak wartosci (lub ujemna wartosc) po argumencie \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }
                    
                      /* Pobranie pierwszej wartosci */
                  sscanf(*argv, "%f", &opcja.poziom1);

                  if (opcja.poziom1>100||opcja.poziom1<0) // Sprawdzenie czy miesci sie w przedziale
                  {
                    fprintf(stderr,"Blad: Niepoprawna wartosc po arg. \"-%c\"! Powinna miescic sie w przedziale [0;100]\n",opcja.param); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }

                  if(!(--argc))
                    {
                      fprintf(stderr,"Blad: Brak drugiej wartosci po argumencie  \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }

                    if (*(++argv)[0] == '-')
                    {
                      fprintf(stderr,"Blad: Brak drugiej wartosci (lub ujemna wartosc) po argumencie \"-%c\".\n",opcja.param); // Obsługa błędów
                      fprintf(stderr,"Zakończono działanie programu!\n");
                      return(ERR);
                    }
                    
                      /* Pobranie drugiej wartosci */
                  sscanf(*argv, "%f", &opcja.poziom2);

                  if (opcja.poziom2>100||opcja.poziom2<0) // Sprawdzenie czy miesci sie w przedziale
                  {
                    fprintf(stderr,"Blad: Niepoprawna druga wartosc po argumencie \"-%c\"! Powinna miescic sie w przedziale [0;100]\n",opcja.param); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }

                  if(opcja.poziom1 == opcja.poziom2)
                  {
                    fprintf(stderr,"Blad: Niepoprawne wartosci po argumencie \"-%c\"! Nie moga byc sobie rowne!\n",opcja.param); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }

                  if(!zmiana_poziomow(&obraz,opcja.poziom1,opcja.poziom2))  /* Nalozenie progowania i podanie komunikatu */
                  {
                    fprintf(stderr,"Blad: Nie udalo sie zastosowac zmiany poziomow na obrazie!\n"); // Obsługa błędów
                    fprintf(stderr,"Zakończono działanie programu!\n");
                    return(ERR);
                  }
                  else
                  {
                    fprintf(stderr, "Nalozono zmiane poziomow.\n" );
                  }                
                }
                else
                {
                  fprintf(stderr,"Blad: Nalezy wczytac obraz przed wszelkimi innymi akcjami. Uzyj arg \"-i\", a po nim nazwe pliku do wczytania.\n"); // Obsługa błędów
                  fprintf(stderr,"Zakończono działanie programu!\n");
                  return(ERR);
                }
              break;

          default: fprintf(stderr,"Nie ma takiego argumentu jak \"-%c\" \n",opcja.param);
          
        }//switch
      }//if
    }//while


  if(!opcja.zapisano)
  {
    wypisz_stdout(obraz);
  }


  if( access("tmp.pgm", F_OK ) != -1 ) /*Usuniecie pliku tmp, jesli istnieje */
  {                                    /*Sprawdzenie czy istnieje za pomoca funkcji access() */
    system("rm tmp.pgm");              /*z biblioteki "unistd.h" */
  }

  if(opcja.odczytano)                 // Jeśli wczytano plik - zaalokowano pamięć
  { 
    switch(obraz.nr_magiczny)
    {
      case '2':
          free(obraz.pix);           // Zwolnienie zaalokowanej pamięci
        break;

      case '3':
          for(i=0; i < obraz.wymx*obraz.wymy; i++)
            free(obraz.pix_kolor[i]);
          free(obraz.pix_kolor);
        break;
    }
  }

  return (OK);
}
/*    Koniec kodu zrodlowego    */

/*    Testy oprogramowania:

      ( Przy sprawdzaniu, kazdego z ponizszych:
        negatywu, konturowania, progowania, rozmycia, oraz zmian poziomów
        - sprawdzenie takze wyswietlaniem, czy obraz sie odpowiedniu wizualnie zmienia! )


      1. Sprawdzenie pustego wywołania
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program                        | *"Należy podać argumenty wywołania| * Poprawne dzialanie programu
                                         |  Użyj --help, aby zobaczyć        |
                                         |  szczegóły."                      |
                                         | * "Zakończono działanie programu!"|    
                                         |                                   |
    ============================================================================================================

     2. Sprawdzenie wczytywania plikow - 1 (i wypisywania na wyjsciu)
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      | Dane wyjsciowe                    |  Komentarz
      * ./program -i abc.pgm             | * "Blad: Nie udało się wczytywać  |    * Przy niepoprawnych danych
      (nazwa nieistniejacego pliku)      |    pliku "abc.pgm".               |      program dziala poprawnie
                                         | * "Zakończono działanie programu" |
                                         |                                   |
                                         |                                   |
    ============================================================================================================
     
     3. Sprawdzenie wczytywania plikow - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      | Dane wyjsciowe                    |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm"      |    * Przy poprawnych danych
        (poprawna nazwa pliku)           |                                   |      program dziala poprawnie
                                         |                                   |
                                         | * Wypisanie obrazu na stdout      |
                                         |                                   |
    ============================================================================================================
    
     4. Sprawdzenie wczytywania plikow - 3
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      | Dane wyjsciowe                    |  Komentarz
      * ./program -i kubus.pgm           | * Wczytano plik "kubus.pgm"       |    * Przy poprawnych danych
        -i kubus2.ppm                    | * Wczytano plik "kubus2.ppm"      |      program dziala poprawnie
        (poprawna nazwa pliku)           |                                   |
                                         |                                   |
                                         | * Wypisanie obrazu na stdout      |
                                         |                                   |
    ============================================================================================================

      5. Sprawdzenie wczytywania plikow - 4
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * Wczytano plik "kubus.ppm"       |    
        (poprawna nazwa pliku)           | * "Blad: Nie udało się wczytywać  |   * program dziala poprawnie
        -i abc                           |    pliku "abc".                   |
                                         | * "Zakończono działanie programu" |
                                         |                                   |
                                         |                                   |
    ============================================================================================================

      6. Sprawdzenie wczytywania plików - 5
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * cat kubus.pgm | ./program  -i -  |  * Wczytano plik "z stdin".       | * Poprawne dzialanie programu
                                         |  * Wypisano obraz na stdout       |
                                         |                                   |
    ============================================================================================================

      7. Sprawdzenie arg. --help
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program  --help                | * Wyświetlenie poradnika          | * Poprawne dzialanie programu
                                         |                                   |
    ============================================================================================================

      8. Sprawdzenie zapisywania plikow - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
          -o 'loremipsum.pgm'            | * "Zapisano do pliku              |
                                         |     "loremipsum.pgm""             |
                                         |                                   |
    ============================================================================================================

      9. Sprawdzenie zapisywania - 2 
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
          -d > loremipsum.ppm            | * "Poprawnie wyswietlono          |  
                                         |     obraz"                        |    * Został utworzony plik
                                         |                                   |    "loremipsum.pgm", poprawnie 
                                         |                                   |    z poprawnie zapisanym obrazem
    ============================================================================================================

      10. Sprawdzenie negatywu
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -n -o plik.pgm                  | * "Nalozono negatyw"              |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      11. Sprawdzenie konturowania
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -k -o plik.pgm                  | * "Nalozono konturowanie"         |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      12. Sprawdzenie progowania - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -p 51.1 -o plik.pgm             | * "Nalozono progowanie,           |   
                                         |   o progu: 51.1%"                 |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================
      
      13. Sprawdzenie progowania - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -p 0 -o plik.pgm                | * "Nalozono progowanie,           |   
                                         |   o progu: 0.0%"                  |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================

      14. Sprawdzenie progowania - 3
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -p -1 -o plik.pgm               | * "Blad: Brak wartosci            |   
                                         |    (lub ujemna wartosc)           |   
                                         |    po argumencie "-p".            |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   |  
    ============================================================================================================

      15. Sprawdzenie progowania - 4
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -p 100 -o plik.pgm              | * "Nalozono progowanie,           |   
                                         |   o progu: 100.0%"                |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================

      16. Sprawdzenie progowania - 5
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -p 100.1 -o plik.pgm            | * "Blad: Niepoprawny prog!        |   
                                         |   Powinien miescic sie            |   
                                         |   w przedziale [0;100]"           |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   | 
    ============================================================================================================

      17. Sprawdzenie rozmycia - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -rx -o plik.pgm                 | * "Nalozono rozmycie poziome."    |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      18. Sprawdzenie rozmycia - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.ppm           | * "Wczytano plik "kubus.ppm""     |    * Poprawne dzialanie
         -ry -o plik.pgm                 | * "Nalozono rozmycie pionowe."    |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      19. Sprawdzenie negatywu
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -n -o plik.pgm                  | * "Nalozono negatyw"              |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      20. Sprawdzenie konturowania
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -k -o plik.pgm                  | * "Nalozono konturowanie"         |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      21. Sprawdzenie progowania - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -p 51.1 -o plik.pgm             | * "Nalozono progowanie,           |   
                                         |   o progu: 51.1%"                 |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================
      
      22. Sprawdzenie progowania - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -p 0 -o plik.pgm                | * "Nalozono progowanie,           |   
                                         |   o progu: 0.0%"                  |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================

      23. Sprawdzenie progowania - 3
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -p -1 -o plik.pgm               | * "Blad: Brak wartosci            |   
                                         |    (lub ujemna wartosc)           |   
                                         |    po argumencie "-p".            |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   |  
    ============================================================================================================

      24. Sprawdzenie progowania - 4
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -p 100 -o plik.pgm              | * "Nalozono progowanie,           |   
                                         |   o progu: 100.0%"                |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |  
    ============================================================================================================

      25. Sprawdzenie progowania - 5
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -p 100.1 -o plik.pgm            | * "Blad: Niepoprawny prog!        |   
                                         |   Powinien miescic sie            |   
                                         |   w przedziale [0;100]"           |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   | 
    ============================================================================================================

      26. Sprawdzenie rozmycia - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -rx -o plik.pgm                 | * "Nalozono rozmycie poziome."    |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      27. Sprawdzenie rozmycia - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -ry -o plik.pgm                 | * "Nalozono rozmycie pionowe."    |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      28. Sprawdzenie zmiany poziomow - 1
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -z 1 99 -o plik.pgm             | * "Nalozono zmiane poziomow."     |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      29. Sprawdzenie zmiany poziomow - 2
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -z 99 1 -o plik.pgm             | * "Nalozono zmiane poziomow.""    |   
                                         | * "Zapisano do pliku "plik.pgm""  | 
                                         |                                   |   
                                         |                                   |  
    ============================================================================================================

      30. Sprawdzenie zmiany poziomow - 3
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -z 100.1 1 -o plik.pg           | * "Blad: Niepoprawna wartosc po   |   
                                         |   arg. "-z"! Powinna miescic sie  |   
                                         |   w przedziale [0;100]"           |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   |  
    ============================================================================================================

      31. Sprawdzenie zmiany poziomow - 4
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program -i kubus.pgm           | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -z 49.1 -o plik.pg              | * "Blad: Brak drugiej wartosci    |   
                                         |   (lub ujemna wartosc)            |   
                                         |   po argumencie "-z"."            |  
                                         | * "Zakończono działanie programu!"| 
                                         |                                   |  
    ============================================================================================================

      32. Sprawdzenie nieistniejących argumentów
    ------------------------------------------------------------------------------------------------------------
     Dane wejsciowe                      |  Dane wyjsciowe                   |  Komentarz
      * ./program abc -i kubus.pgm       | * "Wczytano plik "kubus.pgm""     |    * Poprawne dzialanie
         -a -o plik.pgm ffffffffffff     | * "Nie ma takiego argumentu       |    
                                         |    jak "-a"                       |    * Niepoprawne argumenty są
                                         | * "Zapisano do pliku "plik.pgm""  |      po prostu pomijane
                                         |                                   |      (Nie kończą działania
                                         |                                   |      programu)
                                         |                                   |  
    ============================================================================================================

Podsumowanie:

Program wykonuje akcje w zadanej kolejności (umożliwia to m.in. przetworzenie i zapisanie kilku obrazów z rzędu 
- jednym wywołaniem, czy wyświetlenie obrazu w różnych kombinacjach filtrów, etc.). Wymaga więc aby jako pierwszy
argument pojawiła się flaga "-i". (wyjątkiem jest argument --help).

Wszelkie zbitki znaków nie zaczynające się od -, a nie są wartościami po argumentach są kompletnie pomijane.

Wszyskie komunikaty wypisywane są na stderr, aby nie mieszały się z danymi wypisywanymi na stdout 
(Guide wypisywany na stdout, aby mozna było go zapisać w pliku).

*/