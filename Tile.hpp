#ifndef Tile_hpp
#define Tile_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Tile{
    public:
        Tile();
        ~Tile();
        int xTile,yTile;
        bool isFlaged(){
            return flaged;
        }
        bool open;
        bool isOpen(){
            return open;
        };
        bool isMine(){
            if(data == -1){
                return true;
            }
            return false;
        };
        int getData(){
            return data;
        };
        void setData(int n){
            this->data = n;
        }
        SDL_Rect* getRect(){
            return &desR;
        };
        void setRect(int x, int y, int tilesize){
            desR.x = x;
            desR.y = y;
            desR.h = tilesize;
            desR.w = tilesize;
        };
        void setTexture(std::string path, SDL_Renderer* renderer){
            const char* link = path.c_str();
            SDL_Surface* tmpSurface = IMG_Load(link);
            tex = SDL_CreateTextureFromSurface(renderer,tmpSurface);
            SDL_FreeSurface(tmpSurface);
        };
        void init(int data,int x, int y,int tilesize);
        void renderTile(Game* game);
        void openTile(Game* game);
        void flag(Game* game);
        void deflag(Game* game);
        void free();
    private:
        int state;
        bool flaged;
        SDL_Texture* tex;
        SDL_Rect desR;
        int data;

};

#endif // Tile_hpp
