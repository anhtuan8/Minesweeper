#include"text.hpp"

Text::Text(){

}

Text::~Text(){

}

void Text::setColor(SDL_Color color){
    this->color = color;
}

void Text::createText(TTF_Font* font, SDL_Renderer* renderer,int horizontal,int vertical,SDL_Color color){
    const char* text = value.c_str();
    SDL_Surface* tmp = TTF_RenderText_Solid(font,text,color);
    texture = SDL_CreateTextureFromSurface(renderer,tmp);
    desR = {horizontal,vertical,tmp->w,tmp->h};
    SDL_FreeSurface(tmp);
}

void Text::RenderText(SDL_Renderer* renderer){
    if(texture!=NULL){
        SDL_RenderCopy(renderer,texture,NULL,&desR);
    }
}

void Text::free(){
    if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		desR = {0,0,0,0};
	}
}
