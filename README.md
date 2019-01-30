# libretro-superflappybirds
A multiplayer Flappy Bird clone designed for the libretro framework.

![](.readme/screenshot_title.png)
![](.readme/screenshot_gameplay.png)
![](.readme/screenshot_game_over.png)

## Adding to RetroPie

Copy the script `lr-superflappybirds.sh` to `/home/pi/RetroPie-Setup/scriptmodules/libretrocores/lr-superflappybirds.sh` and run it from the `./retropie_setup` menu.

## Building from source

    git clone https://github.com/IgniparousTempest/libretro-superflappybirds
    cd libretro-superflappybirds
    cmake .
    make
