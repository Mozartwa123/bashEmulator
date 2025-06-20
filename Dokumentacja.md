# Opis klas

## Lexer.hpp:


### Token:


Zawiera informację o typie tokenu, o jego (ewentualnej) wartości, rodzajem wiązania (lewo-, prawostronne) oraz jego priotytecie. Wszystkie metody służą tylko do wypisywania wartości pól oraz do debugowania.

### Lexer:


Zawiera tylko metodę lexing do przekształcania tekstu z wejścia na tokeny. Korzysta z funkcji rozbijających napis na mniejsze w module tokenizer.hpp.


## Parser.hpp:


### Ast:


Składnia abstrakcyjna jest reprezentowana jako struct - dlatego, że nie ma żadnych metod. Poszczególny "węzeł" może być potokiem "|", and'em "&&", komendą, operatorem dopisania ">>", nadpisania >.


### Parser:


Odpowiada za przekształcanie tokenów na Ast.


## User.hpp:


### User


Klasa ta reprezentuje użytkownika. Zawiera informacje o nazwie użytkownika, jego uprawnieniach, oraz na temat tego, jak wygląda jego shell promt (kolor nazwy urządzenia, kolor @, kolor nazwy użytkownika i znak reprezentujący znak zachęty). Wszystkie metody mają trywialne zastosowanie - zmieniają zawartości niektórych pól lub wręcz tylko je zwracają.

## MemObj.hpp:


### MemObj


Klasa ta reprezentuje w najbardziej ogólny sposób obiekty systemu plików - tzn. wspólne cechy plików i folderów. Zawiera ona informacje o nazwie obiektu, autorze obiektu, oraz jednoznaczym identyfikatorze - ścieżce. Jak wyżej metody są trywialne i pokazują tylko zawartości pól (oprócz ścieżki).


## MyDirectory.hpp:


### MyDirectory


Reprezentuje folder i dziedziczy po klasie MemObj - ponadto zawiera metodę givePath, która pokazuje ścieżkę do katalogu, ponadto zawiera zbiór wszystkich katalogów potomnych, zbiór wszystkich plików w tym katalogu zawartych oraz informację o rodzicu danego katalogu (może nie istnieć w przypadku roota).


## File.hpp:


### File


Reprezentuje plik i dziedziczy po klasie MemObj, zawiera też metodę givePath, która działa identycznie jak w przypadku MyDirectory oraz pole content, które reprezentuje zawartość pliku. Związku z powyższym, dostępne są też metody modyfikujące plik, zarówno nadpisujące, jak i dopisujące oraz metoda pokazująca zawartość pliku.


## computer.cpp:


### Computer


W uproszczony sposób symuluje działanie komputera w środowisku linux. Zawiera informację o aktualnym katalogu (w którym użytkownik obecnie się znajduje), katalogu domowym oraz o katalogu root (tzn. /, a nie /root), użytkownikach, którzy korzystają z tego komputera oraz aktualnie zalogowanym użytkowniku. Posiada metody do rekursywnego usuwania katalogów oraz do przeszukiwania zbioru znanych użytkowników.


### Command


Reprezentuje komendę. Zawiera informacje o jej działaniu, dostępnych flagach (jeszcze nie zaimplementowane - użycie nieznanej flagi skutkuje po prostu jej zignorowaniem) oraz funkcję reprezentującą jej działanie, która jako argumenty przyjmuje listę flag, argumentów, komputer (żeby dokonać na nim zmian) oraz samą komendę (bardzo nierozsądne, chodziło o to, żeby w razie niepoprawnego użycia wyświetlić jej opis). Zaimplementowane komendy znajdują się w pliku README.md


### Interpreter


Zawiera lexer, parser i komputer. Jego rolą jest (przy pomocy obiektów klasy Lexer i Parser) jest odwzorowanie napisu z shell promt na rzeczywiste działanie komend.


### Console


Odpowiada za główną interakcję z użytkownikiem bashEmulatora.


# Pozostałe elementy:

## Obsługa błędów (error.hpp):


Wiadomość o błędu jest przechowywana w globalErrorMessage. Ponadto można sprawdzić, czy błąd nastąpił (errorOccured) oraz można ją zaktualizować updateErrorMessage.


## Proste pomocnicze narzędzia do lexera i nie tylko: tokenizer.hpp


### lineSplit(tekst) - rozbija tekst ze względu na białe znaki


### genSplit(tekst, ogranicznik) - rozbija tekst ze względu na dowolny ogranicznik


### pathSplit(tekst) - rozbija ścieżkę objektu systemu plików na kolejne słowa, tj. rozbija tekst ze względu na "/"


### pathSplitLast(tekst) - rozbija ścieżkę obiektu systemu plików na dwa słowa - drugie jest ostatnim elementem ścieżki, a pierwsze resztą ścieżki. Jeżeli słowo nie zawiera "/", to żadne słowo nie jest zwracane. 