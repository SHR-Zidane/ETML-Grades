# ETML-Grades
Gestionnaire de Notes pour les apprentis de l'etml (section informatique notamment)

## Technologies utilisées
- Le langage C
- La bibliothèque GTK3
- Git
- Clang ou gcc
- Cmake
- UCRT64 / mingw64 pour la compilation (windows)

## Installation
1. Clonez le dépôt : `git clone https://github.com/SHR-Zidane/ETML-Grades`
2. Si vous êtes sur Windows, installez MSYS2.
3. Installez les dépendances :
   **Windows** : Ouvrez le terminal **MSYS2 UCRT64** et installez le compilateur et GTK3 : 
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-clang mingw-w64-ucrt-x86_64-gtk3 pkg-config make
   ```
   **Linux** : Ouvrez simplement votre terminal et lancez ces commandes :
   ```bash
   sudo apt-get update
   sudo apt install clang libgtk-3-dev build-essential pkg-config
   ```
4. Compilez le projet :
   ```bash
   cd ETML-Grades
   clang main.c report.c -o ETML-Grades `pkg-config --cflags --libs gtk+-3.0`
   ```
5. Exécutez le programme :
   ```bash
   ./ETML-Grades
   ```

## Utilisation (Work in progress)
