Hovercraft_PFE
==============

Ce Projet de Fin d'Etude en partenariat avec le laboratoire de recherche en aerospatial ONERA, propose la conception d'une alternative au drone ayant pour mission : l'exploration autonome d'un milieu intérieur ainsi que le suivi de cibles en mouvement. Le type de véhicule choisi est l'aéroglisseur et intègre les systèmes suivants :
- Station de control SOL (IHM de contrôle à distance via WIFI - cartographie en 3 dimensions)
- Ordinateur de Bord (Raspberry sous OS Raspbian - algorithme SLAM, scaner laser, traitements d'images)
- Autopilot (Arduino Mega 2560 - contrôle en boucle fermée)

==============

Les technologies utilisées sont :
 - Communication en WIFI 2,4 Ghz 802.11 sur protocol personnalisé.
 - Détection et repérage des cibles : camera embarqué et laser ligne (5mA).
 - Odométrie effectuée par : capteur optique (ADNS2610 - accélération linéaire) & IMU (MPU6050 - dérive angulaire).

==============

Ce PFE reprend le PPE Hovercraft de Alexandre Brand, Simon Burgala, Eliott Chastel, Maxime Jumeline et Lucie Teilhac développé en 2013/2014.

L'objectif est de reprendre le travail effectué et de consolider la solution proposée par l'équipe précédente.

==============

La date de rendu du projet est le 6 Février 2015.
