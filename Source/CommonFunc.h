#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

#include <windows.h>
#include <string>
#include <time.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static Mix_Music *g_music = NULL;
static Mix_Chunk* g_sound_amo;

//Screen
const int FPS = 60;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;
static std::string g_HP[11] = {"img//HP_0.png", "img//HP_1.png", "img//HP_2.png", "img//HP_3.png","img//HP_4.png","img//HP_5.png","img//HP_6.png","img//HP_7.png","img//HP_8.png", "img//HP_9.png","img//HP_10.png"};


#define TILE_SIZE 50

#define MAX_MAP_X 400
#define MAX_MAP_Y 12

#define STATE_MONEY 400

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);

#endif // COMMON_FUNC_H_
