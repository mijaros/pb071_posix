# Cvičení 10
V tomto cvičení se budeme učit, jak přistupovat k souborům v POSIX.

Narozdíl od stdlib, kde hlavní identifikátor souboru je struktura FILE, používá POSIX pro přístup k souboru tzv. file deskriptory, česky řečeno popisovače souborů. 

FD je celé číslo, které odkazuje do tabulky otevřených souborů (udržovaná operačním systémem) a na základě popisovače požaduje po OS akci na souboru (čtení, zápis, přejmenování, smazání atd.).

Pro přístup k soborům exitují funkce:
* open -> alternativa k fopen
* close -> alternativa k fclose
* read -> alternativa k fread
* write -> alternativa k fwrite

Přístup k souboru je tedy velmi podobný jako ve stdlib, s rozdílem, že využití POSIX nám umožňuje i další zásahy do souboru, jako změna majitele, změna uživatele, čtení adresáře atd.

## 1. úkol cat
Upravte následující kód tak, aby používal POSIXové funkce pro přístup k souborům.

```{C}
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Invalid amount of arguments\n");
    return -1;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror(argv[1]);
    return -1;
  }
  
  char buffer[1024] = {'\0'};
  size_t size = 0;
  while ( (size = fread(buffer, 1, 1024, file)) > 0) {
    fwrite(buffer, 1, size, stdout);
  }
  
  fclose(file);
  return 0;
}
```
Využijte manuálové stránky pro zjištění jak se funkce chovají:
open(3), close(3), read(3), write(3). 
Pro otevření manuálové stránky využijte příkazu man, například pro manuál k open použijte
```$man 3 open```

Všimněte si rozdílu v zadávaní módu, narozdíl od stdlib, open použivá číselné konstanty, které se kombinují pomocí logického součtu. Například fopen modifikátoru "w" je ekvivalentní součet hodnot ```O_WRONLY | O_CREAT | O_TRUNC```

## 2. Úkol Informace o souboru
Vaším úkolem je rozšířit předchozí program o výpis informací o souboru, ktere zjistite pomoci funkce fstat(2)
po vypsani souboru, zapiste informace v nasledujicim formatu:
```
Velikost souboru: 
Datum posledniho pristupu:
UID majitele:
GID majitele:
```
Pro vypsání data použijte funkci ctime(3)

## 3. Úkol Refactoring
Vytvořte funkce
```
int print_file(const char *path);
int print_stats(const char *path);
```
Přesuňte do těchto funkcí funkcinalitu vašeho main.
Upravte váš program aby přijímal z příkazové řádky přepínače -s a -p, které budou vždy jako první argument, a 1 - N souborů.
Na základě zvoleného přepínače použijte nad danými soubory funkce:
* -s print_stats
* -p print_file

Pokud funkce selže, pomocí perror vypište na stderr jaká chyba nastala a pokračujte dál. Nezapomeňte, že funkce fstat bere file deskriptor, nicméně existuje funkce stat, která přijímá cestu k souboru místo file descriptoru.

## 4. Úkol Výpis adresářů
Nyní máte program, který dokáže vypsat statistiky nebo obsah daných souborů. Vaším úkolem bude rozšířit jeho funkcionalitu o práci nad soubory

Vytvořte funkci
```
int read_directory(const char *path, int (*func)(const char *) );
```
Tato funkce přečte obsah zadaného adresáře - argument path - a na jednotlivé potomky aplikuje funkci func, pokud jsou soubory.
Pokud nejsou vypíše na standardní výstup informaci, že byl nalezen neregulární soubor (například adresář), a pokračuje dále, bez zanoření.

Pro toto budete potřebovat použít funkce stat(2) readdir(3), opendir(3) closedir(3) a makra S_ISREG apod, tato makra jsou popsaná v manuálové stránce stat.

Na závěr rozšiřte svůj main o detekci adresáře nad argumenty z příkazové řádky, pokud byl na příkazové řádce předán adresář, váš program vypíše:
```
Skenovani adresare %s
--------------------------------
/*obsah vypisu, zavolani funkce read_directory*/
--------------------------------
```
