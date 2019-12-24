
#include "FontObject.h"

textObject::textObject()
{
    rect_.x = 500;
    rect_.y = 10;
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
}
textObject::~textObject()
{
    ;
}
void textObject::setColor(const int& type)
{
        if(type == RED_TEXT)
    {
        SDL_Color color = {255, 0, 0};
        text_color_ = color;
    }
    else if(type == WHITE_TEXT)
    {
        SDL_Color color = {255, 255, 255};
        text_color_ = color;
    }
    else if(type == BLACK_TEXT)
    {
        SDL_Color color = {0, 0, 0};
        text_color_ = color;
    }
    else if(type == GREEN_TEXT)
    {
        SDL_Color color = {0, 255, 64};
        text_color_ = color;
    }
    else if(type == BLUE_TEXT)
    {
        SDL_Color color = {0, 128, 255};
        text_color_ = color;
    }
}

void textObject::CreateGameText(TTF_Font* font, SDL_Renderer* des)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, str_val_.c_str(), text_color_);
    p_object_ = SDL_CreateTextureFromSurface(des, textSurface);
    if(p_object_ != NULL)
        {
            rect_.w = textSurface->w;
            rect_.h = textSurface->h;
        }
    SDL_FreeSurface(textSurface);
    Render(des);
}
