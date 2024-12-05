#pragma once

    struct MouseProps {
        int mouseX = 0;
        int mouseY = 0;
        int mouseXr = 0;
        int mouseYr = 0;
        int mouseXc = 0;
        int mouseYc = 0;
        bool mouseDown = false;
        bool cellIsClicked = false;
        bool released = false;
        bool rightClicked = false;
    };