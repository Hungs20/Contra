#pragma once
#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "FontObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VALUE 18
#define PLAYER_HP 3000
#define DAME_MAP 40

#define BLANK_TILE 0
#define ITEM_BULLET 37

class PlayerObject : public BaseObject
{
public:
    PlayerObject();
    ~PlayerObject();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
        WALK_LEFT_UP = 5,
        WALK_RIGHT_UP = 6,


    };
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandelInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* sound_amo);
    void set_clips();
    void DoPlayer(Map& map_data);
    void CheckToMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();
    void set_bullet_list(std::vector<BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void RemoveBullet(const int& id);
    void HandleBullet(SDL_Renderer* des);
    void SetMoney();

    void set_num_bullet_laser(const int& numBullet) {num_bullet_laser =  numBullet;}
    int get_num_bullet_laser() const {return num_bullet_laser;}
    int getHP() const {return HP_;}
    void setHP(const int& _HP) {HP_ = _HP;}
    float get_x_pos() const {return x_pos_;}
    int getMoney() const {return money_count;}
private:
    int money_count;
    int HP_;
    std::vector<BulletObject*> p_bullet_list_;
    int num_bullet_laser;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];

    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;
    int map_x_;
    int map_y_;
    int come_back_time_;
};

#endif // PLAYER_OBJECT_H_

