# STELLARIS TI remake

## Presentation

This is a remake of Stellaris (by _Paradox Development Studio_) for TI-83 Premium CE and TI-84 Plus CE.

The game is actually on a dev stage. It is in French and will soon be in English.

## Getting started

Thes instructions help you to run this program on your machine

### Installing

The game need the toolchain [CeDev](https://github.com/CE-Programming/toolchain) to be make

- 64 bit can use the last version ([v8.8](https://github.com/CE-Programming/toolchain/releases/tag/v8.8))
- 32 bit need to use the v8.6 and use the last _graphX_ include files from v8.8 ([v8.6](https://github.com/CE-Programming/toolchain/releases/tag/v8.8))

We recommend to use an emulator like [CeMu](https://github.com/CE-Programming/CEmu))

- 64 bit can use the last version ([v1.3](https://github.com/CE-Programming/CEmu/releases/tag/v1.3))
- 32 bit use the v1.0 version ([v1.0](https://github.com/CE-Programming/CEmu/releases/tag/v1.0))

Use `make gfx` command to make the graphx files and `make` to compil the program.

**Warning!!** The gfx and the font cannot be made with the 32 bit version

## To do list (French)

- [x] Menus stations
- [x] Menu planètes
- [ ] Niveaux des batiments
- [x] Plusieurs actions de suite
- [x] Flottes militaires de plusieurs types
- [ ] Gèrer l'update des données du joueur
- [x] Rajout d'une console
- [ ] Menu flottes
- [ ] Rajout des stations scientifiques et minières
- [ ] Actions flottes
- [ ] Pathfinding flottes
- [ ] Recherches
- [ ] Rajout de plusieurs civilisations
- [ ] Rajout de l'ia
- [ ] Rajout des notifications
- [ ] Menu marché
- [ ] Revenus
- [ ] Changement des sprites de local à appvar
- [ ] Refaire le menu d'entrée
- [ ] Création de sauvegardes
- [ ] Traduction totale
