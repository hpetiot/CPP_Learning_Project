# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ? ```La touche 'c' permet de créer un nouvel avion.```
Comment faire pour quitter le programme ? ```Appuyer sur la touche 'q' ou bien la touche 'x'.```
A quoi sert la touche 'F' ? ```À basculer entre les mode feunétré et plein-écran.```

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?```Il fait une boucle d'approche de l'aéroport avant d'attérrire, puis vas ce garere dans un carré blanc. enfin il re-décolle pour revenir un peu plus tard.```
Quelles informations s'affichent dans la console ?```Le nom de l'avion, dans mon cas  AY7541, suivit de ce qu'il fait, lorsque l'avion est garé il indique aussi que l'aeroport s'occupe de l'avion.```

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ? ```Trois des avion arrivent à attérir à des interval très (trop) cours puis il vont chacun occuper une des place de garage signalées par un carré blanc. Le quatrième reste en boucle d'attente et ne ce pause qu'une fois la piste libérée par les avion en départ.```

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
```
* AircraftType -> représente les différent ytpe d'avion via leurs propriété et leurs textures.
* Aircraft -> Représent eune instance d'avion,avec ces propriété dont position vitesse direction, détermine la bonne version du sprite à affier.
* AirportType -> Définie des types d'aéroport par leur nombre de terminaux, leur nombre et orrientation de pister ainsi que par le chemin d'apporche à utiliser pour attérir ou repartir de l'aéroport.
* Airport -> implemente le fonctionnnement d'un aéroport donc le chemin à utiliser entre la (les) piste et un teminal ainsi que le chemin inverse, ainis que sa texture.
* config -> Ficher de configuration de l'application.
* geometry -> Contien les strucutre de math et géometrie permettant de paser d'un espace 3d simulé à un écran 2d.
* Runnway -> definit le début et fin d'une piste ainsi que sa longueur.
* Terminal -> Gere la position de chaque terminaux, (ci-dessus "place de garage") ainsi que la progression de leur tâches.
* TowerSim -> Classe principale de l'interface.
* Tower -> tour de controle assignat chaque terminal à un avion, et verifiant le bon fonctionement du system des avion au sol.
* Waypoint -> définit des points de passage et informe de si il correspondent à un terminal, un point au sol ou un point dans les air.
```

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
```
Tower -> Assigne à chaque avion soit une trajectoire pour approcher l'aéroport si un terminal est disponible, soit une boucle d'attente si l'avion est déjà sur l'aéroport, verifie si sont service est finit si oui, envoie l'avion dans els air. via la fonction get_instruction, soit un terminal si possible et s'assure en suite que l'aéroport s'en occupe.
Aircraft -> Permet d'avoiur la distance à un autre objert, d'avoir le numéro de vol de l'avion, de s'afficher lui-même. Permet aussi de simuler son mouvement: tomber dû à une vitesse trop basse, s'approcher d'un point de passage ou déployer le train d'attérissage si nécéssaire. détecte aussi si il crashe.
Airport -> Communique avec une tour de controle, s'affiche et fait progresser les tâches des terminaux.
Terminal -> indique si il est utilisé, entrain de s'ocuper d'un avion, reçois l'assignation d'un avion, début et finit de s'occuper d'un avion, progresse dans sa tâch de s'occuper d'un avion.
```
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
D'abord l'aéroport qui assign un point de départ du chemin au terminal corrspondant, Puis la classe waypoint qui génére le chemin d'un terminal vers le point de passage du l'aeroport corspondant à "avion en l'air", et enfin la tour qui assigne le reste du trajet (actuelement une boucle entre le "point avion en l'air" et le point "avion vas attérir".)
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
 La deque permet d'ajouter des point de passage, à l'avant comme à l'arrier de façon très efficace. Le surcout de mémoire par rapport à un vector est acceptable comme dans beaucoup de simulation temps réél. Il représente aussi très bien un trajet: on a un point de départ qui est le premier waypoint et un point d'arriver qui est le dernier, on peut choisir de faire un détour impliquant de réjouter des wwaypoint devant le prochain prévu et on peut choisir de changer la fin du trajet.
Expliquez les intérêts de ce choix.

## C- Bidouillons !

### 1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Dans le ficher aircraft_typres.hpp.
### Le Concorde est censé pouvoir voler plus vite que les autres avions.
### Modifiez le programme pour tenir compte de cela.

### 2) Identifiez quelle variable contrôle le framerate de la simulation.
la variable ticks_per_sec dans le ficher opengl_interface.hpp, qui es réglé sur DEFAULT_TICKS_PER_SEC du fichier config.
Cela impacte le nombre de ticks par seconde mais il y a dans le fichier GL/opengl_interface.cpp, un appel a glutTimerFunc utilisant cette variable et un autre appelant ce précedant en temps que callback utilisant un 100 hardcodé.
### Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
### Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
On loop back à max unsigned int donc on finit par ***fortement*** accelerer le système.
### Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
Je ne sais pas vraiment qu'oi ajouter entre un toggle entre 0 et la dernière valeu ajouté pour la value de DEFAULT_TICKS_PER_SEC. ce qui serait facile à implementer ou un system de sleep qui permettrait de fair de l'attente passive. On peut cependant utiliser un methode similaire à celle du fullscreen déjà en place: un touche qui toggle un boolean. Cette modification est plus facile à faire dans la fonction GL::timer.
### 3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
C'est la variable SERVICE_CYCLES dans le fichier config.

### 4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
### Faites en sorte qu'à la place, il soit retiré du programme.\
### Indices :\
### A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?
Dans la fonction move.

### Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
Autodestruction pas sure + stoké dans la moveQueu utilisée dans Timer -> risk de pendouillage.
### A quel endroit de la callstack pourriez-vous le faire à la place ?\
On vas le faire dans lasire fonction timer, en l'informant via le retour de move().
### Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?
Tout les overrice de la fonciton virtuelle move ainsi que sont prototype.

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference constante ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
