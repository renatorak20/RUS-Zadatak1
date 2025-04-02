# Upravljanje višestrukim prekidima s Arduinom 

## Sadržaj
* Komponente 
* Funkcionalnosti 
* Kod 

## Komponente 

Ovaj projekt koristi sljedeće komponente:

* Arduino Uno: Glavna razvojna ploča. 
* Tipkalo(x3) (Push Button): Generira prekid na pritisak. 
* LED dioda(x5): Vizualna indikacija detekcije pokreta. 
* HC-SR04 Ultrasonic Distance Sensor: senzor za mjerenje udaljenosti.


## Funkcionalnosti 

* U sistemu postoji timer koji se okida svake sekunde te ima svoj indikator tj. žutu LED diodu.

* Detekcija pritiska gumba(postoje 3 gumba sa 3 odgovarajuća indikatora pritiska tj. LED diode, crvena, plava i zelena): Kada se pritisne tipkalo, generira se prekid koji postavlja zastavicu isButtonPressedX. U glavnoj petlji, kada je isButtonPressed true, LED dioda svijetli pola sekunde a zastavica se zatim resetira.

* Senzor za mjerenje udaljenosti: u slučaju kada senzor izmjeri udaljenost manju od 100cm, svijetlo plava LED dioda će početi trepteriti svakih 200ms.

* Prioriteti po redu su: 1. tajmer, 2. led diode (po brojevima od 1-3), 3. senzor udaljenosti


### Link za projekt na wokwi stranici je: https://wokwi.com/projects/427150239314128897

![Zadatak1_Slika](https://github.com/user-attachments/assets/c82d332c-262d-4823-a248-b9ee2ef27a05)
