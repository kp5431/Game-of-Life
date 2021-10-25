#include <SDL2/SDL.h> //graphics lib
#include <memory> //for memset()

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

/**
 * Datastructure to represent the cell collection
 **/
class CellMap{
    public:
        CellMap(unsigned int width, unsigned int height) : w(width), h(height){
            length = w * h;
            cells = new unsigned char[length];
            temp_cells = new unsigned char[length];
            memset(cells, 0, length); //ensure memory at cells is initially 0
        }

        ~CellMap(){
            delete[] cells;
            delete[] temp_cells;
        }

        void SetCells(unsigned int x, unsigned int y){
            unsigned char* cell_ptr = cells + (y * w) + x; //get the particular cell to set
            *(cell_ptr) |= 0x01; //set the lsb of the cell to 1 to make it alive

            //now we have to increment the neigbor counts of the 4 cells surrounding *cell_ptr
            int xleft, xright, yabove, ybelow; //offsets to get neighboring cells

            //using 'pacman' borders, so have to check if the current cell
            //is on top, bottom, left, or right edge of screen
            if(x == 0){ //cell is on left border
                xleft = w - 1; //left neighbor is on right edge
            }
            else{
                xleft = -1;
            }
            if(x == (w - 1)){ //right border
                xright = -(w - 1); //right neighbor on left edge
            }
            else{
                xright = 1;
            }
            if(y == 0){
                yabove = length - h;
            }
            else{
                yabove = -h;
            }
            if(y == (h - 1)){
                ybelow = -(length - h);
            }
            else{
                ybelow = h;
            }

        }
    
    private:
        unsigned char* cells;
        unsigned char* temp_cells;
        unsigned int w, h;
        unsigned int length;
};


/**
 * Draw a single cell
 * x,y: coordinates
 * color: single val needed bc only support black/white
 **/
void DrawCell(unsigned int x, unsigned int y, unsigned int color){

    //pixels are stored in a 2dArray. In order to find the particular pixel, use the below formula
    //start at pixels, then offset by formula * 4 (bc the array stores the 4 pixel modifiers contiguously,
    // r,g,b and alpha)
    Uint8* pixel_ptr = (Uint8*) surface->pixels + (y * CELL_SIZE * SCREEN_WIDTH + x * CELL_SIZE) * 4;

    //draw CELL_SIZE * CELL_SIZE pixels for a single cell
    for(unsigned int i = 0; i < CELL_SIZE; i++){
        for(unsigned int j = 0; j < CELL_SIZE; j++){
            //set the rgb attributes  of the pixel to the param color
            *(pixel_ptr + j * 4) = color;
            *(pixel_ptr + j * 4 + 1) = color;
            *(pixel_ptr + j * 4 + 2) = color;
        }

        //each completion of j loop renders a row on the screen
        pixel_ptr += SCREEN_WIDTH * 4; // m by 4 bc rgba
    } 

}

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