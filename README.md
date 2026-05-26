# Specta-League: joc RPG auto-battler in terminal

## Descriere
Specta-League este un joc RPG de tip auto-battler / turn-based combat in terminal. Jucatorul gestioneaza un grup de eroi (Party), echipeaza arme si obiecte folosind un sistem de inventar si se lupta impotriva diferitilor inamici (ex: Dragoni, Monstri). Proiectul este construit pe conceptele fundamentale de Programare Orientata pe Obiecte (OOP), utilizand polimorfism, mostenire, exceptii si design pattern-uri pentru a crea un sistem de combat modular.
Inspiratie: jocurile RPG clasice turn-based si auto-battlers.

## Caracteristici implementate
* sistem de combat pentru rezolvarea luptelor intre eroi si inamici, cu calcularea daunelor
* ierarhii de eroi si inamici: clase abstracte extinse prin derivate specifice (Warrior, Dragon)
* sistem de inventar template capabil sa stocheze si sa sorteze iteme
* design patterns: EnemyFactory (generare inamici), Strategy (decizii in lupta), CombatObserver (monitorizare evenimente)
* exceptii custom: erori specifice de logica tratate prin clase derivate din std::exception
* meniu in terminal pentru gestionarea actiunilor

## Cum se joaca
Jocul se compileaza cu `cmake -S . -B build && cmake --build build`. Apoi rulezi `./build/oop`.
Tastezi optiunea numerica corespunzatoare din meniu si apesi Enter pentru a echipa eroi, a vizualiza inventarul sau a initia o lupta.

## Controls
* tastatura numerica pentru alegerea optiunii curente in meniu
* Enter confirma alegerea

## Resurse:
| Laborant  | Link template                                |
|-----------|----------------------------------------------|
| Dragoș B  | https://github.com/Ionnier/oop-template      |
| Tiberiu M | https://github.com/MaximTiberiu/oop-template |
| Marius MC | https://github.com/mcmarius/oop-template     |

## Instrucțiuni de compilare
Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
Sau pe Windows cu GCC:

Bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.

Cerințe obligatorii
Nerespectarea duce la nepunctarea proiectului

[x] programul va fi scris în C++

[x] programul va avea un meniu interactiv (doar pentru ilustrarea funcționalității)

[x] programul nu are erori de compilare

[x] fară variabile globale

[x] datele membre private(sau protected)

[x] GitHub Actions trecute

[x] commit-uri pe Git adecvate si punctuale

[ ] folosirea a funcționalităților limbajului fără sens

[ ] folosirea a funcționlităților limbajului cu scopul de a încălca "legal" o altă regulă  

[ ] folosirea excesivă a claselor friend  

[ ] folosirea excesviă a elementelor statice  

[x] lipsa separarea implementarii de definitie  

Cerințe
pentru fiecare cerință (sau subcerință) neîndeplinită se scade 1 punct  

[x] definirea a minim 2-3 ieararhii de clase care sa interactioneze in cadrul temei alese (fie prin compunere, agregare sau doar sa apeleze metodele celeilalte intr-un mod logic)  

minim o clasa cu:

[x] constructori de inițializare *

[x] constructor supraîncărcat *

[x] constructori de copiere *  

[x] operator= de copiere *  

[x] destructor *  

[x] operator<< pentru afișare (std::ostream) *

[x] operator>> pentru citire (std::istream) *

[x] alt operator supraîncărcat ca funcție membră *

[x] alt operator supraîncărcat ca funcție non-membră * - nu neaparat ca friend

in derivate

[x] implementarea funcționalităților alese prin upcast și downcast

aceasta va fi făcută prin 2-3 metode specifice temei alese

funcțiile pentru citire / afișare sau destructorul nu sunt incluse deși o să trebuiască să le implementați

[x] apelarea constructorului din clasa de bază din constructori din derivate

[x] suprascris cc/op= pentru copieri/atribuiri corecte

[x] destructor virtual

pentru celelalte clase se va definii doar ce e nevoie

minim o ierarhie mai dezvoltata (cu 2-3 clase dintr-o clasa de baza)

ierarhie de clasa se considera si daca exista doar o clasa de bază însă care nu moștenește dintr-o clasă din altă ierarhie

[x] cât mai multe const *

[x] funcții și atribute static (în clase) *

[x] 1+ atribute statice non-triviale

[x] 1+ funcții statice non-triviale

[x] excepții *

porniți de la std::exception

ilustrați propagarea excepțiilor

ilustrati upcasting-ul în blocurile catch

minim folosit într-un loc în care tratarea erorilor în modurile clasice este mai dificilă  

[x] folosirea unei clase abstracte *  

[x] clase template  

[x] crearea unei clase template *  

[x] 2 instanțieri ale acestei clase  

[x] STL *

[x] utilizarea a două structuri (containere) diferite (vector, list sau orice alt container care e mai mult sau mai putin un array)

[x] utilizarea a unui algoritm cu funcție lambda (de exemplu, sort, transform)

[x] Design Patterns *

[x] utilizarea a două șabloane de proiectare

Observații
Pot exista depunctări până la 2p pentru diferite aspecte precum:

memory leak-uri

nefolosirea destructorului virtual la nevoie

abuzarea de diferite concepte (toate funcțiile declarate virtual)

apelarea de funcții virtual în constructori

În general, acestea sunt prezente în CppCoreGuideline, dar nu e nevoie să parcurgeți documentul, doar să scrieți codul suficient de organizat

folderele build/ și install_dir/ sunt adăugate în fișierul .gitignore deoarece conțin fișiere generate și nu ne ajută să le versionăm.