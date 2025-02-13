# Snake on Everything

Snake running on everything*!<br>
<sup><sub>*I feel like porting it to :P</sub></sup>

# Currently supported platforms:

- Windows (SDL2)
- Linux (SDL2)
- macOS (SDL2)
- GameBoy Advance (libtonc) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-gba.gba))
- Gamecube (GRRLib) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-gamecube.dol))
- DS (libnds) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-ds.nds))
- Wii (GRRLib) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-wii.dol))
- 3DS (citro2d) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-3ds.3dsx))
- Wii U (SDL2 + wut)
- Switch (SDL2 + libnx) ([download](https://github.com/matthew-5pl/snake-on-everything/releases/download/1.0.0/snake-on-switch.nro))

Platforms that don't currently have a download link need to be compiled manually!

# Compiling on PC

All you need to compile this to run on your OS is gcc, SDL2 and make.

Simply clone the repo and run make, the default target will build snake for your OS!

To build for other platforms, simply run `make PLATFORM=x` where `x` is one of the platforms listed above (check for `Makefile.x` files in the project directory for exact filenames)
