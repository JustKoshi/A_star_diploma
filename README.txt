Program wizualizujący algorytm A* przy pomocy Qt

Struktura folderów:
- przykładowe labirynty - przygotowane labirynty gotowe do uruchomienia programu wizualizującego.
- tools - narzędzia zewnętrzne do programu: generator labiryntów oraz testy (work in progress)
- icons - grafiki używane w aplikacji

Sposoby uruchomienia aplikacji:
exe.zip - wystarczy wypakować exe.zip i uruchomić qt_a_star.exe 
kompilacja przy pomocy środowiska programistycznego np. qtcreator

Krótka instrukcja:
Po uruchomieniu programu należy wczytać labirynt przy pomocy przycisku "Select read file". Następnie możemy wybrać pożądaną heurystykę i wcisnąć przycisk "Create". Jeżeli chcemy dokonać zmian w labiryncie to przed wciśnięciem przycisku "Find Path" należy dokonać edycji startu/ końca lub edycji siatki, która wykonuje się po wciśnięciu przycisku "Grid Edit Mode", który sprawia że możemy zamienić przeszkody w dozwoloną drogę i na odwrót. Reszta przycisków odpowiada za modulację prędkości lub wielkości siatki. Można też wyświetlić statystyki po udanej wizualizacji, aczkolwiek by uzyskać pełne wyniki należy przeprowadzić proces wizualizacji dla każdej heurystyki.

Wykonał: Kacper Myszka
Praca licencjacka