# bashEmulator

## Wstęp

bashEmulator jest programem konsolowym naśladującym działanie linuxowego terminala. Umożliwia bardzo uproszczoną emulację ograniczoną do katalogów, plików tekstowych, 12 komend, potoków, wykonywania poleceń w jednej linii za pomocą &&, przekierowywania wyniku polecenia do pliku za pomocą > oraz >>.

## Co udało się zaimplementować

✓ Tworzenie katalogów i plików i przechodzenie pomiędzy nimi
✓ Bardzo podstawową obsługę użytkowników ograniczoną do zmiany pomiędzy aktualnym użytkownikiem a rootem

## Czego nie udało się zaimplementować

✘ Skaner bezpieczeństwa skryptów (poza rm -r / nie ma nic niebezpiecznego)
✘ GUI - które nadałoby większy sens tej inicjatywie (zabrakło czasu i chęci)
✘ hucznie zapowiadanej komendy dd

## Szczegóły implementacji i jej wady

Wykorzystałem możliwości smart pointerów w c++ do odwoływania się do obiektów.

Odczytywanie komend odbywa się za pomocą interpretera. Najpierw zaznaczę, że omyłkowo rozróżniłem pojęcia tokenizera i parsera, choć znaczą dokładnie to samo. Moduł tokenizer.hpp jest zestawem prostych funkji do
cięcia tekstu względem białych znaków, itd., a lexer.hpp napierw dzieli tekst wykorzystując tokenizer.hpp, następnie lexer tworzy odpowiednie tokeny, np. PIPE (napotykając |), ARG --nazwa argumentu--. Token jest osobną klasą.

Parser (który nie jest oddzielony twardo od Lexera jako klasa) przyjmuję listę (wektor) tokenów i tworzy AST, ale ono ma już formę structa, a nie klasy (powód: brak konkretnych metod). Z racji, że nie opieram projektu na żadnej bibliotece, sam musiałem wymyślić algorytm parsowania. Jest on taki: znajdź token o najwyższym priotytecie (pole klasy Token), jeżeli napotkasz kolejny o takim samym priorytecie sprawdź sposób wiązania.
Zauważmy pewną zależność:

Niech * będzie dowolnym spójkiem, który przyjmuje dwa poddrzewa AST, a p1, p2,... będą podrzewami AST. Jeżeli będziemy zmieniać token na drugi o takim samym priorytecie (może być to ten sam spójkik * ). Jeżeli go zmienimy otrzymamy następujące nawiasowanie:

(((((p1 * p2) * p3) * p4) * p5) * p6)

Jeżeli nie to otrzymamy:

(p1 * (p2 * (p3 * (p4 * (p5 * p6)))))

Stąd bardzo prosto rozstrzygam sposób wiązania w osobnej funkcji porównującej.

UWAGA! Jeżeli natrafimy na komendę parsujemy od lewej do prawej zbierając flagi i argumenty.

Interpreter jest w miarę oczywisty. Jedyna wątpliwość to wektor pipeStack. Zwykle przekazujemy jeden wynik do następnych komend, ale na wszelki wypadek zostawiłem możliwość przekazywania wielu. Może istnieje jakiś wyjątek w bashu, o którym nic nie wiem, więc tak zostawiłem.

Lexer rozróżnia flagi wielo- i jedno znakowe. Tak więc -flag lexer przypisze tokeny FLAG f, l, a, g, a --flag 
lexer przypisze token flag.

Komenda jest klasą, a komendy obiektami, a ich wywołanie jest polem, a nie metodą. Największy bezsens, jakiego się dopuściłem, jest przekazywanie obiektu komendy to tego wywołania, głównie po to, aby wyświetlać instrukcję obsługi, w razie niewłaściwego użycia - niestety tego nie zmieniłem - co gorsza, jeżeli wywołujemy jedną komendę w innej (np. cd w ls, kiedy podamy ścieżkę do katalogu, którego zawartość chcemy wyświetlić) to pole Command jest nullptr - co nie jest bezpieczne. W ogólności wywołanie jako pole, było najbezpieczniejszym 
(w konteście dalszego rozwoju programu) wyjściem, jeżeli chodzi o zasatę ocp - implementujemy execute dla odpowiedniej komendy - dodajemy do mapy w interpreterze, żeby wiedział co wywołać i do lexera, żeby wiedział,
że komenda jest komendą a nie argumentem. Kdybym zaimplementował komendę jako klasy (co o zgrozo planowałem - ale na szczęście szybko się pozbyłem tego złego pomysłu) - musiałbym stworzyć globalne obiekty i zrobić gigantyczny pattern matching w interpreterze - co by dodało wiele pracy i potencjalnych błędów.

Niestety obecna implementacja przyjmuje za koniec komendy znak nowego wiersza, a nie EOF, przez co implementacja pętli "for" byłaby problematyczna i ograniczona do średników. Niestety za bardzo też opieram działanie interpretera na mutowalnym stanie. Co więcej prywatne metody i pola prawie nie istnieją w tym projekcie (ale zdażają się na przykład w klasie User, czy Console)...

## Dostępne funkcjonalności

Zaimplementowanych jest 12 komend:

"cd" -> przechodzenie do katalogów - nie posiada żadnych flag, ale logika przetwarzania argumentów jest bardzo podobna do tego co w bashu, w szczególności napisy cd /, cd /etc, cd username/documents w katalogu home oraz cd ~/documents są poprawne i poprawnie wykonywane
"touch" -> tworzenie plików bez flag.
"pwd" -> wyświetla ścieżkę obecnego katalogu
"fastfetch" -> to jest rodzaj żartu. Wyświetla logo ascii (z pliku załączonego do projektu ascii_logo.txt) i mało poważne informacje o systemie, anaoliczne do znanych z neofetcha, fastfetcha, hyfetcha, itp.
"ls" -> wyświetla zawartość katalogu. Dostępne są 2 flagi, które można łączyć:
    -a -> wyświetla ukryte katalogi (zaczynające się od kropki)
    -R -> wyświetla katalogi rekurencyjnie
"clear" -> czyści konsolę.
"mkdir" -> tworzy katalog - niestety nie ma flag
"cat" -> wyświetla zawartość pliku
"rmdir" -> usuwa pusty katalog
"su" -> zmienia użytkownika, ale tylko pomiędzy root42, a użytkownikiem utworzonym na początku programu. Ponadto nie ma jeszcze obsługi haseł.
"rm" -> usuwa plik. Flaga -r usuwa rekurencyjnie foldery. Są zachowane restrykcje z uprawnieniami. Działania rm -r / nie będę zdradzał, ale na szczęście nie powoduje segfaultów.
"echo" -> wypisuje na ekran podaną treść. Niestety nie zaimplementowałem żadnych ograniczników i ich logiki do lexera, przez co echo może wypisać tylko jedno słowo

Ponadto:

Komenda > plik powoduje nadpisanie zawartości pliku. Jeżeli plik nie istnieje zostanie utworzony.
Komenda >> plik powoduje dopisanie linijki będącej wynikiem działania komendy.
Komenda1 | Komenda2 powoduje, że wynik pierwszej komendy zostanie przekazany drugiej jako ostatni argument (potok)
Komenda1 && Komenda2 wykonuje po sobie 2 polecenia, z powodu drobnych nieznajomości linuxa działa to tak samo jak średnik (który nie został zaimplemntowany), czyli nie przejmuje się błędami. W bashu jeżeli Komenda1 jest błędna, to druga nie jest już wykonywana.
Flaga --help dopisana do dowolnej komendy wypisze instrukcje użytkowania (niestety większość jest niekomplenta). Jest to obsługiwane na poziomie interpretera.

Tworzona też jest podstawowa struktura katalogów (bez plików niestety), np. /etc, /var, /sys, /dev, /lib itp.
UWAGA!!! Użytkownik root figuruje jako root42, żeby nie spowodować pomyłki między bashEmulatorem, a własnym terminalem...

Jest dostępne kolorowanie tekstu emulatora. Niestety nie ma tutaj .bashrc, gdzie można te kolory ustawiać.

## Kompilacja i zależności

Program nie wymaga żadnych zewnętrznych bibliotek. Wystarczą standardowe biblioteki c++. Tak naprawdę wystarczą pakiety deweloperskie dostępne w repozytoriach linuxa. Można je zainstalować następująco:

sudo apt install build-essential (na dystrybucjach opartych o Debian)

sudo pacman -S base-devel (na dystrybucjach opartych o Arch linux)

Fedory nie pamiętam. Nigdy z niej nie korzystałem...

Projekt posiada skrypt go kompilujący za pomocą g++ bez żadnych flag (głupota z mojej strony). Kompilacja trwa mniej niż 10 sekund (na 15 letnim komputerze). Więc wystarczy wpisać:

chmod +x compile.sh
./compile.sh

Następnie:

./bashem

Ewentualnie można jeszcze dodać ścieżkę w .bashrc:

alias bashem='/lokalizacja/repozytorium/bashem/'

Następnie:

source ~/.bashrc

Albo ponownie uruchomić terminal.

## Interfejs programu

Nie różni się wiele od klasycznego terminala. Na początku zostaniemy poproszeni o podanie nazw użytkownika i komputera, a potem można rozpocząć sesję. Kończymy ją wpisując end jak komendę, a potem klikając enter. Są jeszcze inne sposoby, których nie będę prezentował, ale podpowiem, że się przewinęły. O to przykładowa sesja:

mozartwa123@salamandra m1bashScriptScannerCpp $ ./bashem 
WELCOME TO BASH EMULATOR
How would you like to name your linux machine?
salamandra
How would you like to name yourself?
mozartwa123
mozartwa123@salamandra ~ $ ls
lsis command

documents	

mozartwa123@salamandra ~ $ mkdir newDir
mkdiris command
newDiris argument
mozartwa123@salamandra ~ $ ls
lsis command

documents	newDir	

mozartwa123@salamandra ~ $ echo Hello_World > file
echois command
Hello_Worldis argument
>is Overwrite
fileis argument
mozartwa123mozartwa123@salamandra ~ $ ls
lsis command

file	documents	newDir	

mozartwa123@salamandra ~ $ cat file
catis command
fileis argument
Hello_Worldmozartwa123@salamandra ~ $ end
mozartwa123@salamandra m1bashScriptScannerCpp $ 

Jak widać jest niestety dużo logów, ale nie będę ich usuwał. Może to pomóc w raportowaniu błędów.