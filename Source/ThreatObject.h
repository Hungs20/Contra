#pragma once
#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREAT_FRAME_NUM 8
#define BOSS_FRAME_NUM 3
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 2

#define HP_STATIC_THREAT 150
#define HP_MOVE_THREAT 100
#define HP_BOSS 1000

#define DAME_STATIC_THREAT 30
#define DAME_MOVE_THREAT 25
#define DAME_BOSS 50

#define BLANK_TILE 0
#define ITEM_BULLET 37

class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    enum TypeMove
    {
        STATIC_THREAT = 0,
        MOVE_IN_SPACE_THREAT = 1,
        BOSS = 2
    };

    void set_x_val(const float& xVal){x_val_ = xVal;}
    void set_y_val(const float& yVal){y_val_ = yVal;}
    void set_x_pos(const float& xPos) {x_pos_ = xPos;}
    void set_y_pos(const float& yPos) {y_pos_ = yPos;}

    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}

    void SetMapXY(const int& mp_x, const int& mp_y){map_x_ = mp_x; map_y_ = mp_y;}
    void setFrameNum(const int& fn) {frame_num_ = fn;}
    int getFrameNum() const {return frame_num_;}
    void set_clips();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);

    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    SDL_Rect GetRectFrame();
    void DoPlayer(Map& map_data);
    void CheckToMap(Map& map_data);
    void InitThreat();
    void set_type_move(const int& type_move) {type_move_ = type_move;}
    int get_type_move() const {return type_move_;}
    void SetAnimation(const int& pos_a, const int& pos_b) {animation_a_ = pos_a, animation_b_ = pos_b;}
    void set_input_left(const int& ipLeft){input_type_.left_ = ipLeft;}
    void ImpMoveType(SDL_Renderer* screen);

    std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const std::vector<BulletObject*>& bulletList) {bullet_list_ = bulletList;}
    void RemoveBullet(const int& id);
    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen, int TypeMove);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
    void setHP(const int& HP) {HP_ = HP;}
    int getHP() const {return HP_;}
    void setDame(const int& dame) {dame_ = dame;}
    int getDame() const {return dame_;}
private:

    int map_x_;
    int map_y_;
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    bool on_ground_;
    int come_back_time_;

    SDL_Rect frame_clip_[THREAT_FRAME_NUM];
    int frame_num_;
    int frame_;
    int width_frame_;
    int height_frame_;

    int type_move_;
    int animation_a_;
    int animation_b_;

    Input input_type_;

    std::vector<BulletObject*> bullet_list_;
    int HP_;
    int dame_;
};

#endif // THREAT_OBJECT_H_
