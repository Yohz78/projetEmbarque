Communication maitre-esclave :

Pour mettre en place une communication maître-esclave en C++, vous devrez utiliser les concepts de programmation de sockets. 
Vous pouvez utiliser la bibliothèque de sockets de Berkeley (BSD) pour créer des sockets et établir des connexions réseau.

Le système maître devra créer un socket en utilisant la fonction socket() et lier ce socket à une adresse IP et un port en utilisant la fonction bind(). 
Il devra ensuite écouter les connexions entrantes en utilisant la fonction listen() et accepter les connexions en utilisant la fonction accept(). 
Une fois qu'une connexion est établie avec un esclave, le maître peut envoyer et recevoir des données en utilisant les fonctions send() et recv().

Le système esclave devra également créer un socket en utilisant la fonction socket(), puis se connecter au maître en utilisant la fonction connect(). 
Il pourra ensuite envoyer et recevoir des données en utilisant les fonctions send() et recv().

Il est important de noter que les communications maître-esclave peuvent être implémentées avec d'autre protocoles tels que TCP/IP,UDP, et d'autres, 
Il est donc important de choisir le protocole le plus approprié pour votre cas d'utilisation.

************************************************************************************************************************************************************
Controle des moteurs :

Ce code utilise une boucle infinie pour permettre à l'utilisateur de saisir des instructions à chaque itération. 

L'instruction saisie est stockée dans la variable "position" et est utilisée pour déterminer quel moteur doit être contrôlé. 

Il utilise un switch-case pour exécuter le code approprié pour chaque instruction.
Il est important de noter que ceci est juste un exemple simple pour vous donner une idée de comment l'instruction de position peut être utilisé pour contrôler les moteurs, 

il faut implémenter la logique de contrôle de moteur en fonction de votre système.