#!/usr/bin/python
#coding=utf-8

import math
from asciimatics.screen import Screen
from asciimatics.renderers import StaticRenderer
from asciimatics.effects import Sprite
from asciimatics.paths import Path

def demo(screen):
    centre = (screen.width // 2, screen.height // 2)
    curve_path = []
    for i in range(0, 11):
        curve_path.append((
            centre[0] + (Screen.width / 4 * math.sin(i * math.pi / 5)),
            centre[1] - (screen.height / 4 * math.cos(i * math.pi / 5)))
        )

    path = Path()
    path.jump_to(centre[0], centre[1] - screen.height // 4)
    path.move_round_to(curve_path, 60)
    sprite = Sprite(
        screen,
        renderer_dict ={
            "default":StaticRenderer(images=["X"])
        },
        path = path,
        colour=Screen.COLOUR_RED,
        clear=False
    )


Screen.wrapper(demo)
