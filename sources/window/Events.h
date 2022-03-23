//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Window.h"

typedef unsigned int uint;

class Events {

public:
    static bool* _keys;
    static uint* _frames;
    static uint _current;
    static float deltaX;
    static float deltaY;
    static float x;
    static float y;
    static bool _cursor_locked;
    static bool _cursor_started;

    static int Initialize();
    static void PollEvents();
    static void Terminate();

    static bool Pressed(int keycode);
    static bool JustPressed(int keycode);

    static bool Clicked(int button);
    static bool JustClicked(int button);
};


#endif //WINDOW_EVENTS_H
