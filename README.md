# ETML-Grades
Gestionnaire de Notes pour les apprentis de l'etml (section informatique notamment)

## Technologies utilisées
- MSYS2 (Windows)
- Le langage C
- La bibliothèque GTK3
- Git
- Ninja
- Clang (UCRT64 toolchain)
- Cmake

## Installation
1. Clonez le dépôt : `git clone https://github.com/SHR-Zidane/ETML-Grades`
2. Si vous êtes sur Windows, installez MSYS2.
3. Installez les dépendances :
   **Windows** : Ouvrez le terminal **MSYS2 UCRT64** et installez le compilateur et GTK3 : 
   ```bash
   pacman -Syu
   pacman -S mingw-w64-ucrt-x86_64-clang \
              mingw-w64-ucrt-x86_64-cmake \
              mingw-w64-ucrt-x86_64-ninja \
              mingw-w64-ucrt-x86_64-gtk3
   ```
   **Linux** : Ouvrez simplement votre terminal et lancez ces commandes :
   ```bash
   sudo apt-get update
   sudo apt install clang libgtk-3-dev build-essential pkg-config
   ```
4. Compilez le projet :
   ```bash
   cd ETML-Grades
   cmake -B build -S . -G "Ninja" \
     -DCMAKE_C_COMPILER=clang \
     -DCMAKE_CXX_COMPILER=clang++
   
   cmake --build build
   ```
5. Exécutez le programme :
   ```bash
   ./build/programme
   ```

à partir de là dès que vous voulez compiler le projet, vous pouvez simplement exécuter `cmake --build build`.
## Utilisation (Work in progress)
