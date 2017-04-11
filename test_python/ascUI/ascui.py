#!/usr/bin/env python
# -*- coding: utf-8 -*-

# @Author:  Lentil1016
# @Date:    2017-4-11
'''
网易运音乐 UI
'''

from __future__ import division
from __future__ import unicode_literals

from asciimatics.widgets import Frame, TextBox, Layout, Label, Divider, Text, \
    CheckBox, RadioButtons, Button, PopUpDialog
from asciimatics.screen import Screen
from asciimatics.scene import Scene
from asciimatics.exceptions import ResizeScreenError, NextScene, StopApplication, \
    InvalidFields
import sys
import re


class private_text(Text):
    def __init__(self, label = None, name = None, on_change = None, validator = None):
        super(private_text, self).__init__(name)
        self._label = label
        self._column = 0
        self._start_column = 0
        self._on_change = on_change
        self.validator = validator


class UI_frame(Frame):
    def __init__(self, screen):
        super(UI_frame, self).__init__(screen,
                                      int(screen.height * 2 // 3),
                                      int(screen.width *2 // 3),
                                      has_shadow = True,
                                      name = "MusicBox",
                                      title = "MusicBox")
        layout = Layout([3])
        self.add_layout(layout)
        layout.add_widget(Button(u"排行榜", self._rank), 0)
        layout.add_widget(Button(u"miu", self._rank), 0)
        layout.add_widget(Button(u"shsh", self._rank), 0)

        self.fix()

    def _rank(self):
        self._scene.add_effect(
            PopUpDialog(self._screen, "ranking", ["OK"])
        )


def demo(screen, scene):
    screen.play([Scene([UI_frame(screen)], -1)], stop_on_resize = True, start_scene = scene)


last_scene = None
while True:
    try:
        Screen.wrapper(demo, catch_interrupt = False, arguments = [last_scene])
        sys.exit(0)
    except ResizeScreenError as e:
        last_scene = e.scene
