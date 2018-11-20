# Game

## How to build

1. Install Haskell Platform
2. `hsc2hs Get.hsc`
3. `ghc Get.hs`
4. `ghc -no-hs-main -optl-s game.c Get.hs -o game`
