#include "raylib.h"
#include "cursor.h"
#include "text_engine.h"
#include "window_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


float backspaceTimer = 0.0f;
float rightTimer = 0.0f;
float leftTimer = 0.0f;
float upTimer = 0.0f;
float downTimer = 0.0f;


Editor::Editor(Font font)
    :font(font), cursor(font), isSelecting(false)
{
    textBuffer = {""};
}

void Editor::handleInput(){
    int key = GetCharPressed();

    if(key > 0){
        insertChar((char)key);
    }

    keyRepeat(KEY_BACKSPACE, backspaceTimer);
    keyRepeat(KEY_RIGHT, rightTimer);
    keyRepeat(KEY_LEFT, leftTimer);
    keyRepeat(KEY_DOWN, downTimer);
    keyRepeat(KEY_UP, upTimer);

    if(IsKeyPressed(KEY_ENTER)){
        newLine();
    }

    if(IsKeyPressed(KEY_END)){
        cursor.cursorChar = textBuffer[cursor.cursorLine].size();
    }

    if(IsKeyPressed(KEY_HOME)){
        cursor.cursorChar = 0;
    }

    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_RIGHT)){
        jumpRight();
    }

    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_LEFT)){
        jumpLeft();
    }

    highlight();
}

void Editor::insertChar(char key){
    textBuffer[cursor.cursorLine].insert(cursor.cursorChar, 1, (char)key);
    cursor.pushCursorRight();
}

void Editor::deleteChar(){
    if(isSelecting){
        if(anchorChar > cursor.cursorChar){
            textBuffer[cursor.cursorLine].erase(cursor.cursorChar,anchorChar+1);
        }
        if(anchorChar < cursor.cursorChar){
            textBuffer[cursor.cursorLine].erase(anchorChar,cursor.cursorChar);
            cursor.cursorChar = anchorChar;
        }
        isSelecting = false;
        return;
    }

    if(cursor.cursorChar > 0){
        if(cursor.cursorChar <= textBuffer[cursor.cursorLine].length())
            textBuffer[cursor.cursorLine].erase(cursor.cursorChar-1,1);
        cursor.pushCursorLeft();
    }
    else if(cursor.cursorChar == 0){
        if(cursor.cursorLine > 0){
            int temp = textBuffer[cursor.cursorLine].length();

            textBuffer[cursor.cursorLine-1] += textBuffer[cursor.cursorLine];
            textBuffer.erase(textBuffer.begin() + cursor.cursorLine);
            cursor.pushCursorUp();
            cursor.cursorChar = textBuffer[cursor.cursorLine].length() - temp;
        }
    }
}

void Editor::newLine(){
    std::string copy = textBuffer[cursor.cursorLine].substr(cursor.cursorChar);
    textBuffer[cursor.cursorLine].erase(cursor.cursorChar);
    textBuffer.insert(textBuffer.begin() + cursor.cursorLine+1, copy);
    cursor.pushCursorDown();
    cursor.cursorChar = 0;
}

void Editor::moveRight(){
    int bound = textBuffer[cursor.cursorLine].length();
    if(cursor.cursorChar < bound){
        cursor.pushCursorRight();
    }
    if(!IsKeyDown(KEY_LEFT_SHIFT) && isSelecting){
        isSelecting = false;
    }
}

void Editor::moveLeft(){
    if(cursor.cursorChar > 0){
        cursor.pushCursorLeft();
    }
    if(!IsKeyDown(KEY_LEFT_SHIFT) && isSelecting){
        isSelecting = false;
    }
}

void Editor::moveDown(){
    int lineBound = textBuffer.size()-1;

    if(cursor.cursorLine < lineBound){
        cursor.pushCursorDown();
        if(textBuffer[cursor.cursorLine].length() < cursor.cursorChar)
            cursor.cursorChar = textBuffer[cursor.cursorLine].length();
    }
    if(!IsKeyDown(KEY_LEFT_SHIFT) && isSelecting){
        isSelecting = false;
    }
}

void Editor::moveUp(){
    if(cursor.cursorLine > 0){
        cursor.pushCursorUp();
        if(textBuffer[cursor.cursorLine].length() < cursor.cursorChar)
            cursor.cursorChar = textBuffer[cursor.cursorLine].length();
    }
    if(!IsKeyDown(KEY_LEFT_SHIFT) && isSelecting){
        isSelecting = false;
    }
}

void Editor::drawBuffer(Font myFont){
    cursor.updateCursor();
    float posX = 0;
    float posY = 8;
    auto [cellWidth, cellHeight] = measureFont(myFont);

    for (int i = 0; i < textBuffer.size(); i++){
        std::string linePrefix = std::to_string(i+1) + ". ";
        DrawTextEx(myFont, linePrefix.c_str(), {posX, posY + i*(cursor.lineGap+cellHeight)}, myFont.baseSize, 0, LIGHTGRAY);
        DrawTextEx(myFont, textBuffer[i].c_str(), {posX + cursor.startGridBuffer*cellWidth, posY + i*(cursor.lineGap+cellHeight)}, myFont.baseSize, 0, WHITE);

        if(cursor.cursorLine == i && cursor.cursorChar < textBuffer[i].length()){
            std::string s(1, textBuffer[i][cursor.cursorChar]);
            DrawTextEx(myFont, s.c_str(), {posX + cursor.startGridBuffer*cellWidth + cursor.cursorChar*cellWidth, posY + i*(cursor.lineGap+cellHeight)}, myFont.baseSize, 0, BLACK);
        }
    }
}

void Editor::openFile(){
    std::ifstream in("../test.txt");
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string contents(buffer.str());

    std::string line = "";
    int lineNum = 0;
    std::vector<std::string> text;

    for (int i = 0; i < contents.length(); i++){
        if(contents[i] == '\n'){
            text.push_back(line);
            line.clear();
        }
        else{
            line += contents[i];
        }
    }

    if(!line.empty()){
            text.push_back(line);
    }
    
    textBuffer = text;
}

void Editor::keyRepeat(int key, float& timer){
    float dt = GetFrameTime();

    float repeatDelay = 0.3f;
    float repeatRate = 0.05f;

    if(IsKeyPressed(key)){
        switch(key){
            case KEY_BACKSPACE: deleteChar(); break;
            case KEY_RIGHT: moveRight(); break;
            case KEY_LEFT: moveLeft(); break;
            case KEY_DOWN: moveDown(); break;
            case KEY_UP: moveUp(); break;
            default: break;
        }
    }
    else if(IsKeyDown(key)){
        timer += dt;
        if(timer >= repeatDelay){
            switch(key){
                case KEY_BACKSPACE: deleteChar(); break;
                case KEY_RIGHT: moveRight(); break;
                case KEY_LEFT: moveLeft(); break;
                case KEY_DOWN: moveDown(); break;
                case KEY_UP: moveUp(); break;
                default: break;
            }
            timer -= repeatRate;
        }
    }
    else
        timer = 0.0f;
}

void Editor::jumpRight(){
    std::string currentLine = textBuffer[cursor.cursorLine];

    for (int i = cursor.cursorChar; i < currentLine.length(); i++){
        if(currentLine[i] == ' '){
            cursor.cursorChar = i;
            break;
        }
        if(i == currentLine.length()-1){
            cursor.cursorChar = i+1;
        }
    }
}

void Editor::jumpLeft(){
    std::string currentLine = textBuffer[cursor.cursorLine];

    for (int i = cursor.cursorChar; i >= 0; i--){
        if(currentLine[i] == ' '){
            cursor.cursorChar = i;
            break;
        }
        if(i == 0){
            cursor.cursorChar = i;
        }
    }
}

void Editor::highlight(){
    auto [cellWidth, cellHeight] = measureFont(font);

    if(IsKeyDown(KEY_LEFT_SHIFT)){
        if(!isSelecting){
            anchorChar = cursor.cursorChar;
            anchorLine = cursor.cursorLine;
            isSelecting = true;
        }
    }
    if(isSelecting){
        float startX = cursor.startGridBuffer * cellWidth + (anchorChar * cellWidth);
        float endX = cursor.startGridBuffer * cellWidth + (cursor.cursorChar * cellWidth);
        
        float rectX = std::min(startX, endX);

        float rectWidth;
        if(startX > endX)
            rectWidth = std::abs(startX - endX) + cellWidth;
        else
            rectWidth = std::abs(startX -endX);
        
        float rectY = cursor.lineGap + (anchorLine * (cellHeight + cursor.lineGap));

        if (rectWidth > 0) {
            DrawRectangle(rectX, rectY, rectWidth, cellHeight, MAROON); 
        }
    }
}