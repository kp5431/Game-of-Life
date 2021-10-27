#include <SDL2/SDL.h> //graphics lib
#include <memory> //for memset()

// SDL Window and Surface for pixel manipulation
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;


// width/height of cell in pixels
unsigned int CELL_SIZE = 3;

//unitless dimensions of the window
unsigned CELLMAP_WIDTH = 200;
unsigned CELLMAP_HEIGHT = 200;

//size of the actual window in pixels
unsigned int SCREEN_WIDTH = CELLMAP_WIDTH * CELL_SIZE;
unsigned int SCREEN_HEIGHT = CELLMAP_HEIGHT * CELL_SIZE;

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

        /**
         * Randomly initialize a percentage of the cells to alive 
         **/
        void Init(){
            unsigned int seed = (unsigned)time(NULL);
            srand(seed);
            unsigned int x, y;
            for(int i = 0; i < length * 0.5; i++){
                x = rand() % (w - 1);
                y = rand() % (h - 1);
                if(!CellState(x, y)){ 
                    SetCell(x, y); //don't turn on the same cell more than once
                }
               
            }
        }

        /**
         * Check if a particular cell is alive or dead 
         **/
        int CellState(int x, int y){
            unsigned char* cell_ptr = cells + (y * w) + x;
            return *cell_ptr & 0x01; //lsb indicates whether the cell is alive or dead
        }

        /**
         * Set a particular cell to alive, and notify its neighbors
         * of its existance 
         **/
        void SetCell(unsigned int x, unsigned int y){
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

            //increment neighbor counts of each neighbor by one
            //add 0x02 because we're incrementing the second to lsb by one,
            //and 0x02 == 10
            *(cell_ptr + yabove + xleft) += 0x02; //neighbor above left
            *(cell_ptr + yabove) += 0x02; //neighbor above
            *(cell_ptr + yabove + xright) += 0x02; //above right
            *(cell_ptr + xleft) += 0x02; //left
            *(cell_ptr + xright) += 0x02; //right
            *(cell_ptr + ybelow + xleft) += 0x02; //below left
            *(cell_ptr + ybelow) += 0x02; //below
            *(cell_ptr + ybelow + xright) += 0x02; //below right

        }

        /**
         * Set a particular cell to dead, and notify its neighbors
         * of its death 
         **/
        void ClearCell(unsigned int x, unsigned int y){
            unsigned char* cell_ptr = cells + (y * w) + x; //get the particular cell to clear
            *(cell_ptr) &= ~0x01; //set the lsb of the cell to 0 to make it dead

            //now we have to decrement the neigbor counts of the 4 cells surrounding *cell_ptr
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

            //decrement neighbor counts of each neighbor by one
            //sub 0x02 because we're decrementing the second to lsb by one,
            //and 0x02 == 10
            *(cell_ptr + yabove + xleft) -= 0x02; //neighbor above left
            *(cell_ptr + yabove) -= 0x02; //neighbor above
            *(cell_ptr + yabove + xright) -= 0x02; //above right
            *(cell_ptr + xleft) -= 0x02; //left
            *(cell_ptr + xright) -= 0x02; //right
            *(cell_ptr + ybelow + xleft) -= 0x02; //below left
            *(cell_ptr + ybelow) -= 0x02; //below
            *(cell_ptr + ybelow + xright) -= 0x02; //below right
        }

        /**
         * Update the map 
         **/
        void NextGen(){
            unsigned int x, y, live_neighbors;
            unsigned char* cell_ptr;

            memcpy(temp_cells, cells, length); //save the old state of cells into temp_cells
            
            cell_ptr = temp_cells;
            for(y = 0; y < h; y++){
                x = 0;
                do{
                    //skip dead cells
                    while(*cell_ptr == 0){
                        cell_ptr++;
                        if(++x >= w){
                            goto NextRow;
                        }
                    }
                    live_neighbors = *cell_ptr >> 1;
                    if(*cell_ptr & 0x01){
                        if((live_neighbors != 2) && (live_neighbors != 3)){
                            ClearCell(x, y);
                            DrawCell(x, y, 0x00);
                        }
                    }
                    else{
                        if(live_neighbors == 3){
                            SetCell(x, y);
                            DrawCell(x, y, 0xFF);
                        }
                    }

                    cell_ptr++;

                } while (++x < w);

            NextRow:;   
            }


        }


    
    private:
        unsigned char* cells;
        unsigned char* temp_cells;
        unsigned int w, h;
        unsigned int length;
};


int main(int argc, char* argv[]){
    
    //SDL boilerplate
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    surface = SDL_GetWindowSurface(window);

    //SDL event handler
    SDL_Event e;

    //cellmap instance
    CellMap map(CELLMAP_WIDTH, CELLMAP_HEIGHT);
    map.Init(); //randomly set 50% cells to alive

    //rendering loop
    bool quit = false;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }

        map.NextGen();

        //Update frame buffer
        SDL_UpdateWindowSurface(window);
    }

    //Clean up SDL
    SDL_DestroyWindow(window);
    //Quit SDL
    SDL_Quit();
}