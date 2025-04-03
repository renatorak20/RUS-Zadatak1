# Upravljanje višestrukim prekidima s Arduinom 

## Sadržaj
* Opis zadatka
* Komponente 
* Funkcionalnosti 
* Kod

## Opis zadatka
Sustav demonstrira obradu više prekida sa različitim prioritetima.

## Komponente 

Ovaj projekt koristi sljedeće komponente:

* Arduino Mega 2560 
* Tipkalo(x3): Generira prekid na pritisak. 
* LED dioda(x5): Vizualna indikacija prekida. 
* HC-SR04 Ultrasonic Distance Sensor: senzor za mjerenje udaljenosti.

| Komponenta        | Količina | Pin na Arduino Mega |
| ----------------- | -------- | ------------------- |
| Arduino Mega 2560 | 1        | -                   |
| Tipkalo           | 3        | 2, 3, 4             |
| Crvena LED        | 1        | 13                  |
| Plava LED         | 1        | 12                  |
| Zelena LED        | 1        | 11                  |
| Žuta LED          | 1        | 10                  |
| Cyan LED          | 1        | 9                   |
| HC-SR04           | 1        | TRIG: 6, ECHO: 5    |

## Funkcionalnosti 

* U sistemu postoji timer koji se okida svake sekunde te ima svoj indikator tj. žutu LED diodu.

* Detekcija pritiska gumba(postoje 3 gumba sa 3 odgovarajuća indikatora pritiska tj. LED diode, crvena, plava i zelena): Kada se pritisne tipkalo, generira se prekid koji postavlja zastavicu isButtonPressedX. U glavnoj petlji, kada je isButtonPressed true, LED dioda svijetli pola sekunde a zastavica se zatim resetira.

* Senzor za mjerenje udaljenosti: u slučaju kada senzor izmjeri udaljenost manju od 100cm, svijetlo plava LED dioda će početi trepteriti svakih 200ms.

* Prioriteti po redu su: 1. tajmer, 2. led diode (po brojevima od 1-3), 3. senzor udaljenosti

### 4.3. Tablica funkcionalnosti

| ID    | Opis funkcionalnosti                                                                                 |
| ----- | ---------------------------------------------------------------------------------------------------- |
| FR-1  | Upravljanje LED-icama: Crvena (INT0), Plava (INT1), Zelena (INT2), Cyan (senzor), Žuta (timer).    |
| FR-2  | Detekcija pritiska na tipkala (INT0, INT1, INT2) i generiranje prekida.                              |
| FR-3  | Mjerenje udaljenosti pomoću HC-SR04 senzora i generiranje prekida ako je udaljenost manja od 100 cm. |
| FR-4  | Konfiguracija Timer1 za generiranje prekida svake sekunde (najviši prioritet).                       |
| FR-5  | Obrada prekida prema prioritetima: Timer1 > INT0 > INT1 > INT2 > senzor udaljenosti.                 |
| FR-6  | Vizualizacija aktivnih prekida pomoću odgovarajućih LED-ica.                                         |
| FR-7  | Debounce mehanizam za tipkala kako bi se izbjeglo višestruko okidanje prekida.                       |
| FR-8  | Serial komunikacija za ispisivanje poruka o aktivnim prekidima.                                      |
| FR-9  | Automatsko gašenje svih LED-ica prilikom aktivacije prekida višeg prioriteta.                        |
| FR-10 | Bljeskajuća LED-ica (cyan) kao indikacija alarmnog stanja zbog blizine predmeta.                    |
| FR-11 | Testiranje istovremenog okidanja svih prekida i provjera hijerarhije prioriteta.                     |


### Link za projekt na wokwi stranici je: https://wokwi.com/projects/427150239314128897

![Zadatak1_Slika](https://github.com/user-attachments/assets/c82d332c-262d-4823-a248-b9ee2ef27a05)
