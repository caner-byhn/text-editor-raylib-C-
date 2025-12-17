#include "raylib.h"
#include "window_utils.h"
#include "cursor.h"
#include "text_engine.h"
#include <iostream>

int main(){
    int screenWidth = 1200;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "editor");
    SetTargetFPS(60);

    Font firaRegular = LoadFont("../assets/FiraMono-Regular.ttf");
    Editor editor(firaRegular);

    bool activateGrid = false;


    editor.openFile();
    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(GRAY);

            if(IsKeyPressed(KEY_RIGHT_ALT)){
                activateGrid = !activateGrid;
            }
            if(activateGrid){
               drawGrid(screenWidth, screenHeight, firaRegular); 
            }

            editor.handleInput();
            editor.drawBuffer(firaRegular);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}