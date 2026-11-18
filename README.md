## Lien Github:
- https://github.com/EmilieH06/TP2-mobile2

## Lien vers la vidéo: 
- https://cegepsaintefoy-my.sharepoint.com/:v:/g/personal/2435295_csfoy_ca/IQAeJTp14FNuSakzKhi25fTTAUJLZx1pNzXv7niqdZKqQ5c

## Branchements:
Bouton A (Bleu):
- GPIO 27

Bouton B (Vert):
- GPIO 16

Afficheur:
- CLK : GPIO 19
- DIO : GPIO 23

Matrice:
- DIN : GPIO 5

Servomoteur:
- Signal : GPIO 25

## Librairies:
- smougenot/TM1637@0.0.0-alpha+sha.9486982048 : pour le display
- adafruit/Adafruit NeoMatrix : pour la matrice
- madhephaestus/ESP32Servo@^3.0.5 : pour le servo moteur
- sstaub/NTP : pour la date et l'heure

## Explications du projet:
Faire un coffre fort programmable.
Lorsque le coffre est activé pour la première fois, on peut entrer notre code pour pouvoir l'utiliser plus tard pour ouvrir le coffre.
Lorsque le coffre est ouvert, on peut changer son code ou fermer le coffre.
Lorsque le coffre est fermé, on peut l'ouvrir en entrant le code qu'on a saisie au début ou lors de la reprogrammation.
La saisie d'un code requiert ensuite sa confirmation.
