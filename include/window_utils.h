#pragma once

#include "raylib.h"
#include <iostream>
#include <utility>

inline std::pair<int,int> measureFont(Font myFont){
    Vector2 charSize = MeasureTextEx(myFont, "A", myFont.baseSize, 0);

    int cellWidth = (int)charSize.x;
    int cellHeight = (int)charSize.y;

    return {cellWidth, cellHeight};
}

inline void drawGrid(int screenWidth, int screenHeight, Font myFont){
    auto [cellWidth, cellHeight] = measureFont(myFont);

    for(int x = 0; x<screenWidth; x+=cellWidth){
        DrawLine(x, 0, x, screenHeight, DARKBLUE);
    }

    for(int y = 0; y<screenHeight; y+=cellHeight){
        DrawLine(0,y,screenWidth,y, DARKBLUE);
        y+=8;
        DrawLine(0,y,screenWidth,y, DARKBLUE);
    }
}