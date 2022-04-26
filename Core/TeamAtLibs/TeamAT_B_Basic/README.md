# TeamAT Basic

## Version Log

**1.1.1**

- Ajouté stio.h / stdint.h
- Changé / corrigé un type dans millisToHumanTime
- Ajout fonction bit test pour tester un bit
 *    
**1.1.0**

- Modification de la fonction millisToHumanTime() pour ajouter la taille du buffer de réception pour limiter les erreurs de mémoire.
- Renommer fichiers pour inclure la version

## Introduction

 Fourni des fonctions de base utilisé dans les librairies TeamAT

## Known bugs

 TeamATBasics inclu le fichier arduino.h donc ne pourrait pas être utilisé avec ST (?) faire un fichier différent pour arduino ?
 Il suffit de commenter le include quand on est sur ST

## Hardware Requirement and compatibility

### Module compatibility

#### Supported hardware

##### Arduino boards

- Tout

##### ST boards

- Fonctionne ? Car arduino.h est déclaré (voir known bugs)



## Software Requirement and compatibility

### Required Libraries

- Arduino.h


## Quick Getting started

### For Arduino

   1. Inclure le .h et le .cpp dans le dossier de votre projet directement
