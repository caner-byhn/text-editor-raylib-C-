#pragma once

#include "raylib.h"
#include <iostream>
#include <utility>

class Cursor{
    public:
        int scale;
        int cursorChar;
        int cursorLine;
        int lineGap;
        int startGridBuffer;

        Cursor(Font myFont);
        void updateCursor();
        void setCursorLinePos();
        void pushCursorRight();
        void pushCursorLeft();
        void pushCursorDown();
        void pushCursorUp();

    private:
        Vector2 pos;
        int cellWidth;
        int cellHeight;
};