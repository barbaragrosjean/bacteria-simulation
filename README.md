*********************
La simulation informatique de colonies de bactéries et de nutriments dans une assiette de Petri vise à représenter l'évolution et les interactions de ces micro-organismes dans des conditions variables.

Nous abordons deux types de nutriments et quatre types de bactéries.

Ces dernières se différencient entre elles par leur mode de déplacement et de nutrition. De plus elles ont des aspects physiques différents. Quand aux nutriments la principale différence réside dans la quantité d'énergie apportée aux différents types de bactéries, visuellement on les distincte par leurs couleurs.

Plus précisément:

- Simple Bacterium: se déplacent individuellement attirés par les sources de nutriments. Elles sont bleues et possèdent une flagelle à l'arrière.

- Twitching Bacterium: se déplacent grâce à leur tentacule qui explore les environs et attire la bactérie quand elle détecte une source de nutriment. Elles sont roses.

- Swarm Bacterium: se déplacent en groupe en suivant le leader qui est attiré par les nutriments. Le leader est bactérie la plus proche de sources de nutriments. Il y a en deux types qui forment deux groupes (chacun avec son propre leader): l’un est bleu, l’autre est vert.

- Friendly Bacterium: de couleur blanche avec une croix rouge elles laissent sur leur passage des aides qui ont une durée de vie limitée et donnent de l'énergie aux Simple Bacteria et Swarm Bacteria

- NutrimentA : de couleur vert, offre la même quantité disponible a toutes les bactéries.

- NutrimentB : de couleur violet et plus rare que les NutrimentA, la quantité disponible pour une Twitching Bacterium est plus importante, celle pour une Simple Bacterium est plus petite elle aura donc plus de mal à le manger, et pour les Swarm Bacteria ces nutriments agissent comme un poison.

Les bactéries évoluent dans le temps: elles se déplacent, elles mangent, elles meurent et peuvent se diviser. Cette division entraîne des mutations sur la couleur et sur les paramètres mutables spécifiques à chaque type de bactérie.

Toutes les bactéries sont en compétition pour les nutriments et, selon la configuration choisie, un certain type sera avantagé par rapport aux autres. Il n’y a aucune interaction entre les différentes classes de bactéries, à part la distribution d’aide de la part des Friendly Bacteria.

L'environnement peut influencer l'évolution des colonies. Il est possible de changer les conditions environnementales au cours de la simulation grâce aux commandes spécifiques : augmenter/diminuer le gradient et la température.

Les graphes présent en bas de la fenêtre de simulation rendent l’évolution des colonies et les mutations de certains paramètres visibles.

De plus, en mode Debug il est possible de voir la quantité de nutriment présente dans chaque source, l’énergie de chaque bactérie et un cercle rouge apparaît autour du leader.

## Compilation et exécution:

Ce projet utilise [Cmake] (https://cmake.org/) pour compiler

En ligne de commande :

- dans le dossier build: cmake ../src

- make nom_cible pour la génération de la cible

Dans QTCreator:

- mise en place du projet : ouvrir le fichier src/CmakeLists.txt

- choisir la cible à exécuter


## Cibles principales:

* application: cible principale qui permet de pouvoir utiliser toutes les commandes codées

* swarmTest: cible permettant d'observer les Swarm Bacteria seuls dans le milieu

* twitchingTest: cible permettant d'observer les Twitching Bacteria seuls dans le milieu

* simpleBactTest: cible permettant d'observer les Simple Bacteria seuls dans le milieu

Il y a aussi d'autres cibles mineurs de contrôle d'exécution.
 

## Configurations:

Nous allons lister les fichiers de configuration fournis et leurs effets sur nos colonies de bactéries.

### 0) Fichier général

Le fichier app.json fourni est une moyenne des fichiers suivant permettant d'apprécier la simulation dans son ensemble sans mettre en évidence un comportement en particulier.  

### 1) Effet des Nutriments B

Dans le fichier app1.json nous modifions la probabilité de génération des nutriments B de sorte à ce que elle soit égale à celle des Nutriments A. Les autres facteurs liés aux Nutriments B que nous avons incrémenté sont le poison factor, le resistance factor et le nutritive factor.

Grâce à ce fichier de configuration nous pouvons observer l'effet négatif sur la durée de vie des Swarm Bacteria et des Simple Bacteria, et l'effet positif sur celle des Twitching Bacteria.

De plus quand la température favorise les Nutriments B, on voit que les Swarm et les Simple Bacteria ne survivent pas longtemps, au contraire des Friendly et des Twitching qui sont avantagées par la diminution de concurrence.

### 2) Effet des Friendly Bacteria

Grâce au fichier app2.json il est possible d'observer l'effet positif qu'apportent les Friendly Bacteria et les Help distribué par ces derniers sur les Simple et Swarm Bacteria .

Les Friendly Bacteria mangent les Nutriments B qui sont désavantageux pour ces deux types de bactéries et en plus relâchent des aides.

En incrémentant le nombre des Friendly et la quantité d'énergie que les autres bactéries peuvent prendre des Help, nous observons qu'ils vivent beaucoup plus longtemps qu'en absence de Friendly Bacteria.

### 3) Facteur de mutation

Grâce au fichier app3.json il est possible d’observer les différents caractères mutables. nous avons augmenté l'écart type et la probabilité des mutations pour tumble better et tumble worse des simple Bacteria et tentacle length et tentacle speed. Nous pouvons ainsi remarquer qu’au bout d’un certain temps les mutations les plus adaptées à la survie, sur ces caractéristiques, prédominent.

Nous avons pris la précaution de baisser le délai de la génération automatique des nutriments afin de favoriser la survie et les mutations et d'éviter la mort précoce d’une colonie (notamment pour les Simple Bacteria).

## Commandes:

Les commandes sont données dans le panneau d'aide à droite de la fenêtre de simulation. Elles dépendent de la cible.
  

## Modifications de conception:

Nous n’avons pas apporté de modifications conceptuelles au projet, nous sommes restées proches du sujet.


## Extensions:

Nous avons apporté des fonctions supplémentaires à notre programme:

### 1) Ajout de statistiques:

Nous avons ajouté:

- La longueur et vitesse moyenne des Twitching Bacteria (tentacle lenght, tentacle speed)

- La moyenne de probabilité de basculer mieux ou pire des Simple Bacteria (tumble better prob, tumble worse prob)

- Le nombre de Help présent dans la PetriDish (help)

- Dans le graphe qui représente le nombre totale de bactéries nous avons ajouté le nombre de Friendly Bacteria

- Un graphe qui indique la durée de vie moyenne des bactéries de chaque classe présentes dans l’assiette de Petri

### 2) Ajout d'un nouveau type de bactérie: FriendlyBacterium

Ces nouvelle bactéries, comme le nom l'indique, aident les autres bactéries (en particulier les Simple et les Swarm Bacteria).

Ces bactéries bougent, se clonent comme les Simple et en plus elles peuvent libérer des Help. La libération d'un Help se fait que quand une certaine énergie minimale est rejointe par la bactérie et qu'un certain délai de temps est passé depuis le dernier Help distribué. Cette action coûte de l'énergie à la bactérie.

Nous avons décidé que les Help ne bougent pas et que les bactéries ne sont pas attirés par ces derniers. Donc la rencontre d'un Help est un événement casuel.

Une Simple ou Swarm Bacterium gagne de l'énergie quand elle rencontre un Help, mais avant l'utilisation d'un prochain Help il doit se passer un certain délai de temps.

Les Friendly Bacteria ne font aucune différence entre les Nutriments A et les Nutriments B. Ceci veut dire qu'elles aident aussi en mangeant les Nutriments B qui sont néfastes pour certains types de bactéries.

Nous avons décidé à travers le fichier de configuration (qui reste modifiable) de faire "naître" les Friendly avec une énergie assez basse, de façon à ce qu'elles doivent croitre un moment avant de rejoindre l'énergie nécessaire à libérer un Help et qu'elles aient une énergie de division très élevée. De cette manière il est rare qu'elles se multiplient.
