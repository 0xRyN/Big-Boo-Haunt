AVANT PARTIE : 

OK GAMES n*** // NBRE DE PARTIES  ::S v C::
OK OGAME m s*** // Numéro de partie + nbre de joueurs  ::R::
OK NEWPL ID port***// Créer nouvelle partie ::E::
OK REGIS ID PORT m***// Se connecter à la partie m de port avec id  ::E::
OK REGOK***// Apres NEWPL OU REGIS PR CONFIRMER M  ::R::
OK REGNO***// Apres NEWPL OU REGIS SI PROBLEME ::R::
OK START***// Pour dire que le joueur est pret si tt le monde start partie commence ::E::
OK UNREG m***// POUR SE DESENREGISTRER DE LA PARTIE m DS LAQUELLE ON EST ::E::
OK UNROK m***// Désenregistrement effectué de m ::R::
OK DUNNO*** // Erreur lors du deregister car client n est ds aucune partie ::R::
SIZE? m***// DEMANDER AU SERVEUR TAILLE DU LABBY
SIZE! m h w***// REPONSE A SIZE si m n'existe pas DUNNO
LIST? m***// Demander la liste des joueurs de la partie m
LIST! m s***// Retourne la liste des joueurs de la partie m sous la forme de s messages : PLAYR ID*** si m n'existe pas 
OK PLAYR id***// Pour list des joueurs
OK GAME?***// Retourne les parties non commencées avec des joueurs inscrits non commencée sous la forme GAMES n*** puis n messages OGAME m s***

PENDANT PARTIE : 

WELCO m h w f ip port***// Quand la partie commence 
POSIT 
UPMOV
DOMOV
LEMOV
RIMOV
MOVE!
MOVEF
IQUIT
GOBYE
GLIS?
GLIS!
GPLYR
MALL?
MALL!
SEND?
SEND!
NSEND