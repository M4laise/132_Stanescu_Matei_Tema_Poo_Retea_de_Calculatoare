mai multe abonamente la net - mai multe routere - fiecare router cu un abonament la net - calculatoare conectate la routere
fiecare abonament are o limita de trafic - fiecare calculator are un consum care se cumuleaza la routerul pe care il foloseste

interogare: depaseste un router capacitatea de trafic a abonamentului sau? - daca da,
parcurgem calculatoarele de pe router si le scadem traficul cu un procent in functie de abonament, astfel incat
traficul routerului sa nu depaseasca limita impusa de abonament. 
Unele calculatoare sunt servere si nu le poate fi scazut traficul.
Daca scaderea traficului nu ar rezolva problema supraincarcarii, atunci crestem pretul platit de router pentru abonament;

INPUT: (pe cate un rand)
nr abonamente
traficul maxim impus de abonament, procentul cu care va fi scazut traficul calculatoarelor conectate la router, pret initial, cu cat creste pretul per MB depasit dupa reducerea traficului
nr routere
indexul abonamentului fiecarui router
nr calculatoare
traficul consumat de calculator, indexul routerului la care este conectat si daca este sau nu server

OPERATII POSIBILE:
adaugarea dinamica a calculatoarelor pe routere.
monitorizarea consumului total si detectarea supraincarcarilor.
calcularea traficului routerului dupa o eventuala reducere procentuala a traficului permis.
scaderea traficului pe calculatoare client cu exceptia serverelor, daca acesta ar elimina supraincarcarea.
cresterea pretului abonamentului pentru routere supraincarcate.