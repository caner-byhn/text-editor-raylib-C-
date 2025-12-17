#include "cursor.h"
#include "window_utils.h"
#include "raylib.h"
#include <iostream>
#include <math.h>
#include <utility>


Cursor::Cursor(Font myFont) 
    : scale(1)
{
    auto fontSize = measureFont(myFont);
    this->cellWidth = scale * fontSize.first;
    this->cellHeight = scale * fontSize.second;

    this->lineGap = 8 * scale;
    this->startGridBuffer = 3 * scale;

    this->pos = { (float)cellWidth * startGridBuffer, (float)lineGap };

    this->cursorChar = ((int)pos.x - (int)(startGridBuffer * cellWidth)) / cellWidth;
    this->cursorLine = (int)pos.y / (int)(cellHeight + lineGap);
}

void Cursor::updateCursor(){
    pos.x = cellWidth*startGridBuffer + cursorChar*cellWidth;
    pos.y = lineGap + cursorLine*(cellHeight + lineGap);

    DrawRectangle(pos.x,pos.y,cellWidth,cellHeight, WHITE);
}

void Cursor::pushCursorRight(){
    cursorChar++;
}

void Cursor::pushCursorLeft(){
    if(pos.x > startGridBuffer*cellWidth){
        cursorChar--;
    }
}

void Cursor::pushCursorDown(){
    cursorLine++;
}

void Cursor::pushCursorUp(){
    cursorLine--;
}