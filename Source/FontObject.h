#pragma once
#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class textObject : public BaseObject
{
private:
    std::string str_val_;
    SDL_Color text_color_;
public:
    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
        GREEN_TEXT = 3,
        BLUE_TEXT = 4
    };
    textObject();
    ~textObject();
    void setText(const std::string text) {str_val_ =  text;}
    void setColor(const int& type);
    void CreateGameText(TTF_Font* font, SDL_Renderer* des);
};

#endif // TEXT_OBJECT_H_
