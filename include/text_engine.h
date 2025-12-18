#pragma once

#include "raylib.h"
#include "cursor.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <functional>

class Editor{
    public:
        Editor(Font font);
        void handleInput();
        void insertChar(char key);
        void deleteChar();
        void newLine();
        void moveRight();
        void moveLeft();
        void moveDown();
        void moveUp();
        void jumpRight();
        void jumpLeft();
        void drawBuffer(Font myFont);
        void openFile();
        void keyRepeat(int key, float& timer);
        void highlight();

    private:
        Cursor cursor;
        Font font;
        std::vector<std::string> textBuffer;
        int anchorChar;
        int anchorLine;
        bool isSelecting;
};