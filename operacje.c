#include "operacje.h"


/************************************************************************************
 * Funkcja wczytuje obraz PGM lub PPM z pliku do tablicy                            *
 *                                                                                  *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM                      *
 * \param[out] struktura przechowująca obraz                                        *
 * \return liczba wczytanych pikseli                                                * 
 ************************************************************************************/
int wczytaj(FILE *plik_we, plik_pgm_ppm *obraz) 
{
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j,k;

  /*Sprawdzenie czy podano prawid³owy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || ((buf[1]!='2') && (buf[1]!='3')) || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM, ani PPM\n");
    return(0);
  }
  
  obraz->nr_magiczny = buf[1]; //Zapamiętanie nr magicznego

  /* Zapamietanie komentarzy */
  for(i=0; i < ILE_LINII; i++)
  {
    sprintf(obraz->komentarz[i]," "); /* Zainicjowanie tablicy pustymi liniami */
  }                             /* Pozbycie sie ewentualnych "smieci", np po poprzednim obrazie */ 

  i = 0;        /* Zainicjowanie iteratora */
  do {
    if ((znak = fgetc(plik_we)) == '#') 
    {                                         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we) == NULL)  /* Przeczytaj ja do bufora                */
      {
        koniec=1;                      /* Zapamietaj ewentualny koniec danych */
      }
      else if(i < ILE_LINII)                 /* Sprawdzenie, czy nie przekraczamy limitu linii w tablicy */
      {
        strcpy(obraz->komentarz[i],"#");    /* Dopisujemy na poczatek przepisywanego komentarza "#" */
        strcat(obraz->komentarz[i],buf);    /* Przepiosujemy linie komentarza z bufora */
        i++;                          /* inkrementacja iteratora */
      }
      else
      {
        i = 0;                        /* Gdy dojdziemy do konca tablicy */
        strcpy(obraz->komentarz[i],"#");    /* zaczynamy nadpisywac komentarze od pierwszego */
        strcpy(obraz->komentarz[i],buf);    /* ...Dopisujemy na poczatek przepisywanego komentarza "#" */
      }
    }  
    else 
    {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",&(obraz->wymx),&(obraz->wymy),&(obraz->szarosci))!=3) 
  {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }

  if(obraz->nr_magiczny == '2') // Gdy PGM
  {
      /* Dynamiczna alokacja pamięci na wskaźniku obraz.pix */
      // Tablica para-dwuwymiarowa
      obraz->pix = calloc(obraz->wymx * obraz->wymy, sizeof(int));
    
      /* Pobranie obrazu i zapisanie w tablicy */
      for (i = 0; i < obraz->wymy; i++) 
      {
        for (j = 0; j < obraz->wymx; j++) 
        {
          if (fscanf(plik_we,"%d",&(obraz->pix[i * obraz->wymx + j])) != 1) 
          {
              fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
              return(0);
          }
        }
      }
  }
  else if(obraz->nr_magiczny == '3') //Gdy PPM
  {
      /* Dynamiczna alokacja pamięci na wskaźniku obraz.pix_kolor */
      // Tablica para-trójwymiarowa
      obraz->pix_kolor = calloc(obraz->wymx * obraz->wymy, sizeof(int*));
        for(i = 0; i < obraz->wymx*obraz->wymy; i++)
          obraz->pix_kolor[i] = calloc( 3, sizeof(int));

      /* Pobranie obrazu i zapisanie w tablicy */
      for (i = 0; i < obraz->wymy; i++) 
      {
        for (j = 0; j < obraz->wymx; j++) 
        {
          for (k = 0; k < 3; k++)
          {
            if (fscanf(plik_we,"%d",&(obraz->pix_kolor[i * obraz->wymx + j][k])) != 1) 
            {
              fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
              return(0);
            }
          }
        }
      }
  }

  return (obraz->wymx)*(obraz->wymy);   /* Czytanie zakonczone sukcesem    */
}                                       /* Zwroc liczbe wczytanych pikseli */


/************************************************************************************
 * Funkcja czyta obraz PGM lub PPM ze standardowego strumienia wejścia              *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return liczba wczytanych pikseli                                                * 
 ************************************************************************************/
int czytaj_stdin(plik_pgm_ppm *obraz) 
{
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j,k;


  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,stdin)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || ((buf[1]!='2') && (buf[1]!='3')) || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM, ani PPM\n");
    return(0);
  }
  
  obraz->nr_magiczny = buf[1]; //Zapamiętanie nr magicznego

  /* Zapamietanie komentarzy */
  for(i=0; i < ILE_LINII; i++)
  {
    sprintf(obraz->komentarz[i]," "); /* Zainicjowanie tablicy pustymi liniami */
  }                             /* Pozbycie sie ewentualnych "smieci", np po poprzednim obrazie */ 

  i = 0;        /* Zainicjowanie iteratora */
  do {
    if ((znak = fgetc(stdin)) == '#') 
    {                                         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,stdin) == NULL)  /* Przeczytaj ja do bufora                */
      {
        koniec=1;                      /* Zapamietaj ewentualny koniec danych */
      }
      else if(i < ILE_LINII)                 /* Sprawdzenie, czy nie przekraczamy limitu linii w tablicy */
      {
        strcpy(obraz->komentarz[i],"#");    /* Dopisujemy na poczatek przepisywanego komentarza "#" */
        strcat(obraz->komentarz[i],buf);    /* Przepiosujemy linie komentarza z bufora */
        i++;                          /* inkrementacja iteratora */
      }
      else
      {
        i = 0;                        /* Gdy dojdziemy do konca tablicy */
        strcpy(obraz->komentarz[i],"#");    /* zaczynamy nadpisywac komentarze od pierwszego */
        strcpy(obraz->komentarz[i],buf);    /* ...Dopisujemy na poczatek przepisywanego komentarza "#" */
      }
    }  
    else 
    {
      ungetc(znak,stdin);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(stdin,"%d %d %d",&(obraz->wymx),&(obraz->wymy),&(obraz->szarosci))!=3) 
  {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }

  if(obraz->nr_magiczny == '2') // Gdy PGM
  {
      /* Dynamiczna alokacja pamięci na wskaźniku obraz.pix */
      // Tablica para-dwuwymiarowa
      obraz->pix = calloc(obraz->wymx * obraz->wymy, sizeof(int));
    
      /* Pobranie obrazu i zapisanie w tablicy */
      for (i = 0; i < obraz->wymy; i++) 
      {
        for (j = 0; j < obraz->wymx; j++) 
        {
          if (fscanf(stdin,"%d",&(obraz->pix[i * obraz->wymx + j])) != 1) 
          {
              fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
              return(0);
          }
        }
      }
  }
  else if(obraz->nr_magiczny == '3') //Gdy PPM
  {
      /* Dynamiczna alokacja pamięci na wskaźniku obraz.pix_kolor */
      // Tablica para-trójwymiarowa
      obraz->pix_kolor = calloc(obraz->wymx * obraz->wymy, sizeof(int*));
        for(i = 0; i < obraz->wymx*obraz->wymy; i++)
          obraz->pix_kolor[i] = calloc( 3, sizeof(int));

      /* Pobranie obrazu i zapisanie w tablicy */
      for (i = 0; i < obraz->wymy; i++) 
      {
        for (j = 0; j < obraz->wymx; j++) 
        {
          for (k = 0; k < 3; k++)
          {
            if (fscanf(stdin,"%d",&(obraz->pix_kolor[i * obraz->wymx + j][k])) != 1) 
            {
              fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
              return(0);
            }
          }
        }
      }
  }

  return (obraz->wymx)*(obraz->wymy);   /* Czytanie zakonczone sukcesem    */
}                                       /* Zwroc liczbe wczytanych pikseli */


/************************************************************************************
 * Funkcja konwertuje obraz PPM do formatu PGM                                      *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return 0, gdy pojawi sie blad i 1 gdy zapisano poprawnie                        * 
 ************************************************************************************/
int konwertuj_ppm_pgm(plik_pgm_ppm* obraz)
{
    int i,j;

      /* Dynamiczna alokacja pamięci na wskaźniku obraz.pix */
      // Tablica dwuwymiarowa do przechowywania obrazu PGM
      obraz->pix = calloc(obraz->wymx * obraz->wymy, sizeof(int));
    
      for (i = 0; i < obraz->wymy; i++) 
      {
        for (j = 0; j < obraz->wymx; j++) 
        {
          obraz->pix[i * obraz->wymx + j] = 
                  ( obraz->pix_kolor[i * obraz->wymx + j][0]
                  + obraz->pix_kolor[i * obraz->wymx + j][1]
                  + obraz->pix_kolor[i * obraz->wymx + j][2]
                  ) / 3;
        }
      }
      obraz->nr_magiczny = '2';

      //Zwolnienie niepotrzebnej już pamięci
      for(i=0; i < obraz->wymx*obraz->wymy; i++)
          free(obraz->pix_kolor[i]);
      free(obraz->pix_kolor);
}


/************************************************************************************
 * Funkcja zapisuje obraz PGM lub PPM z tablicy do pliku                            *
 *                                                                                  *
 * \param[out] plik_we uchwyt do pliku do zapisania obrazu                          *
 * \param[in] struktura przechowująca obraz                                         *
 * \return 0, gdy pojawi sie blad i 1 gdy zapisano poprawnie                        * 
 ************************************************************************************/
int zapisz(FILE *plik_we,plik_pgm_ppm obraz) 
{
  int i,j,k;
  char znak;

  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we == NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  fprintf(plik_we, "P%c\n", obraz.nr_magiczny);         /* Wpisanie na poczatku numeru magicznego P2 */

  i=0;
  do
  {
    znak = obraz.komentarz[i][0];          /* Przepisanie pierwszego znaku linii*/
   if(znak == '#')                   /* Sprawdzenie, czy linia zaczyna sie od '#' */
    { 
      fprintf(plik_we, "%s",obraz.komentarz[i]); /* Przepisanie kolejnego komentarza do pliku */
    }
    i++;                              /* inkrementacja iteratora */
  }while(i<DL_LINII && znak == '#');
  
  /* Dopisanie wymiarow obrazu i liczby odcieni szarosci */
  fprintf(plik_we,"%d %d\n%d\n",obraz.wymx,obraz.wymy,obraz.szarosci);

  // Sprawdzenie nr magicznego i odpowiednie przepisanie tablicy do pliku
  switch(obraz.nr_magiczny)
  {
    case '2':
        for (i = 0; i < obraz.wymy; i++)
        {
          for (j = 0; j < obraz.wymx; j++) 
          {
            fprintf(plik_we,"%d\t",(obraz.pix[i * obraz.wymx + j])); 
          }
        }
      break;

    case '3':
        for (i = 0; i < obraz.wymy; i++)
        {
          for (j = 0; j < obraz.wymx; j++) 
          {
            for(k = 0; k < 3; k++)
            {
                fprintf(plik_we,"%d\t",(obraz.pix_kolor[i * obraz.wymx + j][k])); 
            }
          }
        }        
  }
   


  return 1;     /* Zapisywanie zakonczone sukcesem    */
}                      


/************************************************************************************
 * Funkcja wypisuje obraz PGM lub PPM z tablicy na standardowy strumien wyjscia     *
 *                                                                                  *
 * \param[in] struktura przechowująca obraz                                         *
 * \return 0, gdy pojawi sie blad i 1 gdy zapisano poprawnie                        * 
 ************************************************************************************/
int wypisz_stdout(plik_pgm_ppm obraz) 
{
  int i,j,k;
  char znak;

  fprintf(stdout, "P%c\n", obraz.nr_magiczny);         /* Wpisanie na poczatku numeru magicznego P2 */

  i=0;
  do
  {
    znak = obraz.komentarz[i][0];          /* Przepisanie pierwszego znaku linii*/
   if(znak == '#')                   /* Sprawdzenie, czy linia zaczyna sie od '#' */
    { 
      fprintf(stdout, "%s",obraz.komentarz[i]); /* Przepisanie kolejnego komentarza do pliku */
    }
    i++;                              /* inkrementacja iteratora */
  }while(i<DL_LINII && znak == '#');
  
  /* Dopisanie wymiarow obrazu i liczby odcieni szarosci */
  fprintf(stdout,"%d %d\n%d\n",obraz.wymx,obraz.wymy,obraz.szarosci);

  // Sprawdzenie nr magicznego i odpowiednie przepisanie tablicy do pliku
  switch(obraz.nr_magiczny)
  {
    case '2':
        for (i = 0; i < obraz.wymy; i++)
        {
          for (j = 0; j < obraz.wymx; j++) 
          {
            fprintf(stdout,"%d\t",(obraz.pix[i * obraz.wymx + j])); 
          }
        }
      break;

    case '3':
        for (i = 0; i < obraz.wymy; i++)
        {
          for (j = 0; j < obraz.wymx; j++) 
          {
            for(k = 0; k < 3; k++)
            {
                fprintf(stdout,"%d\t",(obraz.pix_kolor[i * obraz.wymx + j][k])); 
            }
          }
        }        
  }
   


  return 1;     /* Zapisywanie zakonczone sukcesem    */
}


/************************************************************************************
 * Funkcja wyswietla obraz o zadanej nazwie za pomoca programu "display"            *
 *                                                                                  *
 * \param[in] tablica nazwirajaca nazwe pliku do wyswietlenia                       *
 ************************************************************************************/
void wyswietl(char *n_pliku) 
{
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  system(polecenie);             /* wykonanie polecenia        */
  sleep(1);      //Trochę czasu, żeby zewnętrzny program zdążył otworzyć plik tmp
}




/************************************************************************************
 * Funkcja nakladana negatyw na obraz                                               *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int negatyw(plik_pgm_ppm *obraz) 
{
  int i,j;
  for (i = 0; i < obraz->wymy; i++)
  {
      for (j = 0; j < obraz->wymx; j++) 
      {
        obraz->pix[i * obraz->wymx + j] = obraz->szarosci - obraz->pix[i * obraz->wymx + j]; //odwrocenie wartosci pixela wzgledem skali szarosci
         
         if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j] < 0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */
      }
  }

return 1; //Zwroc 1, gdy pomyslnie
}                       


/************************************************************************************
 * Funkcja nakladana konturowanie na obraz                                          *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int konturowanie(plik_pgm_ppm *obraz) 
{
  int i,j;
  for (i=0;i<obraz->wymy;i++)
  {
      for (j=0;j<obraz->wymx;j++) 
      {                         
        if(i==obraz->wymy-1||j==obraz->wymx-1)
        {
          obraz->pix[i * obraz->wymx + j] = 0; /*Dla ostaniego wiersza i kolumny przypisanie wartosci 0*/
        }
        else
        {               /*obliczanie wartosci pixela wzgledem dwoch innych z otoczenia (obok i ponizej)*/
          obraz->pix[i * obraz->wymx + j] =  (abs(obraz->pix[(i+1) * obraz->wymx + j]-obraz->pix[i * obraz->wymx + j]) + abs(obraz->pix[i * obraz->wymx + j+1]-obraz->pix[i * obraz->wymx + j]));
          if(obraz->pix[i * obraz->wymx + j]>obraz->szarosci) //Sprawdzenie, czy wartosc pixela nie wychodzi poza skale szarosci
          obraz->pix[i * obraz->wymx + j]=obraz->szarosci;
        }
         if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j] < 0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */
      }
    }

return 1; //Zwroc 1, gdy pomyslnie
}         


/************************************************************************************
 * Funkcja nakladana progowanie na obraz                                            *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \param[in] prog , podany wczesniej przez uzytkownika i funkcji progowanie_menu() *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int progowanie(plik_pgm_ppm *obraz, float procent) 
{
  int prog=((int)obraz->szarosci*procent/100);
  int i,j;
  for (i = 0; i < obraz->wymy; i++)
  {
      for (j = 0; j < obraz->wymx; j++) 
      {                    
        if(obraz->pix[i * obraz->wymx + j] > prog)    /*dla wartosci wiekszych niz "prog" wpisywanie */ 
          obraz->pix[i * obraz->wymx + j] = obraz->szarosci; /* maksymalnej wartosci ze skali szarosci */
        else
          obraz->pix[i * obraz->wymx + j]=0; //dla wartosci mniejszych, lub rownych wpisywanie wartosci "0"

          if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j] < 0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */
      } 
  }

return 1;     //Zwroc 1, gdy pomyslnie
}


/************************************************************************************
 * Funkcja nakladana rozmycie pionowe na obraz                                      *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int rozmycie_pion(plik_pgm_ppm *obraz) 
{
  int bufor1 = 0; /* Bufory pomocnicze */
  int bufor2 = 0; /* do przechowywania wartosci pixeli */
  int i,j;
  for (j = 0; j < obraz->wymx; j++) 
  {
      for (i = 0; i < obraz->wymy; i++)  
      {                
          if(i > 0 && i < obraz->wymy-1)    /* Nie liczymy rozmycia dla gornej i dolnej krawedzi */
            bufor1 = (obraz->pix[(i-1) * obraz->wymx + j] + obraz->pix[i * obraz->wymx + j] + obraz->pix[(i+1) * obraz->wymx + j])/3;
         
          if(i > 1)         /* Przypisanie wartosci bufora dopiero drugiemu pixelowi w kazdej kolumnie, */
            obraz->pix[(i-1) * obraz->wymx + j] = bufor2;   /* czyli dopiero, gdy iterator "i" bedzie na trzecim rzedzie*/
         
          bufor2 = bufor1; // Przekazanie wartosci miedzy buforami

          if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j] < 0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */      
        } 
    }

return 1; //zwrocenie 1, gdy wszystko pomyslnie
}


/************************************************************************************
 * Funkcja nakladana rozmycie poziome na obraz                                      *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int rozmycie_poz(plik_pgm_ppm *obraz) 
{
  int bufor1 = 0;        /* Bufory pomocnicze */
  int bufor2 = 0;        /* do przechowywania wartosci pixeli */
  int i,j;
  for (i = 0; i < obraz->wymy; i++)  
  {
      for (j = 0; j < obraz->wymx; j++) 
      {                
          if(j > 0 && j < obraz->wymx-1)    /* Nie liczymy rozmycia dla bocznych krawedzi */
            bufor1 = (obraz->pix[i * obraz->wymx + j-1] + obraz->pix[i * obraz->wymx + j] + obraz->pix[i * obraz->wymx + j+1])/3;
         
          if(j > 1)            /* Przypisanie wartosci bufora dopiero drugiemu pixelowi w kazdym rzedzie, */
            obraz->pix[i * obraz->wymx + j-1] = bufor2;   /* czyli dopiero, gdy iterator "i" bedzie na trzeciej kolumnie*/
         
          bufor2 = bufor1; // Przekazanie wartosci miedzy buforami
          if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j]<0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */  
      } 
  }

return 1; //zwroc 1, gdy wszystko pomyslnie
}


/************************************************************************************
 * Funkcja zmienia poziomy czerni i bieli obrazu                                    *
 *                                                                                  *
 * \param[out] struktura przechowująca obraz                                        *
 * \param[in] poz1 , podany wczesniej przez uzytkownika (czerni lub bieli)          *
 * \param[in] poz2 , podany wczesniej przez uzytkownika (czerni lub bieli)          *
 * \return 1 gdy pomyslnie, 0 gdy cos nie tak                                       *
 ************************************************************************************/
int zmiana_poziomow(plik_pgm_ppm *obraz, float poz1, float poz2)
{
  float max, min;
  if(poz1>poz2)
  {
    max=((int)((poz1*obraz->szarosci)/100));
    min=((int)((poz2*obraz->szarosci)/100));
  }
  else
  {
    max=((int)((poz2*obraz->szarosci)/100));
    min=((int)((poz1*obraz->szarosci)/100));
  }

  int i,j;
  for (i = 0; i < obraz->wymy; i++)
  {
      for (j = 0; j < obraz->wymx; j++) 
      {                    
        if(obraz->pix[i * obraz->wymx + j] >= max)    /*dla wartosci wiekszych, lub rownych "max" wpisywanie */ 
          obraz->pix[i * obraz->wymx + j] = obraz->szarosci; /* maksymalnej wartosci ze skali szarosci */
        
        else if (obraz->pix[i * obraz->wymx + j] <= min)
          obraz->pix[i * obraz->wymx + j]=0; //dla wartosci mniejszych, lub rownych "min" wpisywanie wartosci "0"

        else //Dla wartosci po miedzy stosujemy wzor: (x-min)*szarosci/(max-min)
          obraz->pix[i * obraz->wymx + j]=
                (obraz->pix[i * obraz->wymx + j]-min) * (obraz->szarosci/(max-min));


          if(obraz->pix[i * obraz->wymx + j] > obraz->szarosci || obraz->pix[i * obraz->wymx + j] < 0)  
          return (0);                             /* Zwroc zero, gdy cos nie tak */
      } 
  }

  return 1;
}


/************************************************************************************
 * Funkcja przygotowuje strukturę typu "wybory"                                     *
 *                                                                                  *
 * \param[out] struktura przechowująca informacje związane opcjami programu         *
 ************************************************************************************/
void zerowanie(wybory* opcja)
{
    opcja->prog = 0;
    opcja->poziom1 = 0;
    opcja->poziom2 = 0;
    opcja->odczytano = 0;
    opcja->zapisano = 0;
    opcja->wyswietlono = 0;
    opcja->nazwa_tmp = "tmp.pgm";  
}


/************************************************************************************
 * Funkcja wyświetla użytkownikowi poradnik obsługi programu                        *
 *                                                                                  *
 ************************************************************************************/
void guide()
{
  system("clear"); //wyczyszczenie okna konsoli dla menu

  printf("\n\n============================================\n");
  fprintf(stdout,"\t\tGUIDE\n"); //Poradnik wypisywany na stdout
  printf("--------------------------------------------\n");   
  fprintf(stdout,"> [-i nazwa_pliku] \t Wymagany argument. Nalezy podac przed wszelkimi innymi. Jako wartosc przyjmuje nazwe pliku do wczytania, lub znak '-' (czytanie pliku ze standardowego strumienia wejscia). \n\n");
  fprintf(stdout,"> [-o nazwa_do_zapisu] \t Zapis do pliku. Jako wartosc przyjmuje nazwe pliku do zapisu, lub znak '-'. Brak argumentu \"-o\", lub przyjecie przez niego wartosci '-' oznacza wypisanie obrazu na standardowym strumieniu wyjscia.\n\n");
  fprintf(stdout,"> [-p prog] \t Progowanie. Jako wartosc przyjmuje prog podany w procentach [0;100]. Musi przyjac jakas wartosc. (Obrazy PPM konwertowane sa na typ PGM).\n\n");
  fprintf(stdout,"> [-n] \t Negatyw. (Obrazy PPM konwertowane sa na typ PGM). \n\n");
  fprintf(stdout,"> [-k] \t Konturowanie. (Obrazy PPM konwertowane sa na typ PGM). \n\n");
  fprintf(stdout,"> [-rx][-ry] \t Rozmycie. x - poziome. y - pionowe. (Obrazy PPM konwertowane sa na typ PGM). \n\n");
  fprintf(stdout,"> [-z poz1 poz2] \t Zmiana poziomow. Przyjmuje dwie wartosci z zakresu [0;100], jako procent skali szarosci. (Obrazy PPM konwertowane sa na typ PGM). \n\n");

}