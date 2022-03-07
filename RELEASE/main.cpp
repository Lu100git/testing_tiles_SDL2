#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define IMAGE_SIZE 64  
#define TILE_SIZE 32
using std::cout;
using std::endl;

class Object{
private:
    SDL_Rect src_rect;
    SDL_Texture* texture;
    double main_angle = 0;
    SDL_RendererFlip main_flip = SDL_FLIP_NONE;
public:
    bool rotate = false;
    SDL_Rect position_rect;
    Object(SDL_Renderer*, std::string, int, int, int, int, int, int);
    void setSource(int, int, int, int);
    void draw(SDL_Renderer*);
    ~Object();
};

Object::Object(SDL_Renderer* ren, std::string filepath, int x, int y, int w, int h, int x_src, int y_src){
    SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());
    if(surface == NULL) std::cout << "image loading error " << SDL_GetError() << std::endl;
    else{
        texture = SDL_CreateTextureFromSurface(ren, surface);
        if (texture == NULL) std::cout << "surface convertion error: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surface);
    
    position_rect.x = x;
    position_rect.y = y;
    position_rect.w = w;
    position_rect.h = h;

    int tu = x_src % (IMAGE_SIZE / TILE_SIZE);
    int tv = y_src / (IMAGE_SIZE / TILE_SIZE);
    setSource(tu * TILE_SIZE, tv * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}


void Object::setSource(int x, int y, int w, int h){
    src_rect.x = x;
    src_rect.y = y;
    src_rect.w = w;
    src_rect.h = h;
}

void Object::draw(SDL_Renderer* rend){
    if (rotate){
        main_angle+=1;
        if (main_angle >360){
            main_angle = 0;
        }
    }
    SDL_RenderCopyEx(rend, texture, &src_rect, &position_rect, main_angle, NULL, main_flip);
}

Object::~Object(){
    texture = NULL;
    SDL_DestroyTexture(texture);
}

int main(int argc, char const *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool running = true;
    std::vector<Object> tiles;

    int map_size[15][20] ={
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}
    };

    if (SDL_INIT_VIDEO < 0) cout << "video initialization error: " << SDL_GetError() << endl;
    else{
        window = SDL_CreateWindow("TILES IN SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) cout << "window creation error: " << SDL_GetError() << endl;
        else{
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            for (int i = 0; i < WINDOW_HEIGHT / TILE_SIZE; ++i){
                for (int j = 0; j < WINDOW_WIDTH / TILE_SIZE; ++j){
                    int current_x = map_size[i][j];
                    int current_y = map_size[i][j];
                    tiles.push_back(Object(renderer, "sprites/MAPA.bmp", j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, current_x, current_y));
                }
            }

            while(running){
                while (SDL_PollEvent(&event) !=0){
                    if (event.type == SDL_QUIT) running = false;
                    else if (event.type == SDL_MOUSEBUTTONDOWN){
                        if (event.button.button == SDL_BUTTON_RIGHT){
                            std::cout << "right click! " << std::endl;
                        }
                    }
                    else if (event.type == SDL_KEYDOWN){
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                                running = false;
                                break;
                        }
                    }
                }
                SDL_RenderClear(renderer);
                for (int i = 0; i < tiles.size(); ++i){ tiles[i].draw(renderer);}
                SDL_RenderPresent(renderer);                
            }
        }
    }
    window = NULL;
    renderer = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
