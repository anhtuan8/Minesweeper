#ifndef Text_hpp
#define Text_hpp

#include<string>
#include<SDL2/SDL_ttf.h>
#include"Game.hpp"
class Text{
    public:
        Text();
        ~Text();
        enum TextColor{
            RED = 0,
            WHITE = 1,
            BLACK = 2,
        };
        int xTile,yTile;
        void setText(std::string text){
            this->value = text;
        }
        SDL_Rect getRect(){
            return desR;
        }
        void setColor(SDL_Color color);
        void createText(TTF_Font* font, SDL_Renderer* renderer,int x,int y,SDL_Color color);
        void RenderText(SDL_Renderer* renderer);
        void free();
        std::string value;
    private:
        SDL_Rect desR;
        SDL_Color color;
        SDL_Texture* texture;
};

#endif // Text_hpp
