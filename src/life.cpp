#include <SDL2/SDL.h>

// SDL Window and Surface for pixel manipulation
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;


// width/height of cell in pixels
unsigned int CELL_SIZE = 1;

//unitless dimensions of the window
unsigned CELLMAP_WIDTH = 200;
unsigned CELLMAP_HEIGHT = 200;

//size of the actual window in pixels
unsigned int SCREEN_WIDTH = CELLMAP_WIDTH * CELL_SIZE;
unsigned int SCREEN_HEIGHT = CELLMAP_HEIGHT * CELL_SIZE;


//void Draw

int main(int argc, char* argv[]){
    
    //SDL boilerplate
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    surface = SDL_GetWindowSurface(window);

    //SDL event handler
    SDL_Event e;

    //rendering loop
    bool quit = false;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }

        //Update frame buffer
        SDL_UpdateWindowSurface(window);
    }

    //Clean up SDL
    SDL_DestroyWindow(window);
    //Quit SDL
    SDL_Quit();
}