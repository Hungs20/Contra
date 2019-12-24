
#include "ThreatObject.h"

ThreatsObject::ThreatsObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    on_ground_ = 0;
    frame_ = 0;
    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    type_move_ = STATIC_THREAT;
    come_back_time_ = 0;
    HP_ = HP_STATIC_THREAT;
    dame_ = DAME_STATIC_THREAT;
    frame_num_ = THREAT_FRAME_NUM;
}
ThreatsObject::~ThreatsObject()
{

}
bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret)
    {
        width_frame_ = rect_.w/frame_num_;
        height_frame_ = rect_.h;
    }

    //std::cout << width_frame_ << " " << height_frame_ << "\n";
    return ret;
}
SDL_Rect ThreatsObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}
void ThreatsObject::set_clips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for(int i = 0; i < frame_num_; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;

        }
    }
}

void ThreatsObject::Show(SDL_Renderer* des)
{
    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if(frame_ >= frame_num_)
            frame_ = 0;

        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}
void ThreatsObject::DoPlayer(Map& map_data)
{
    if(come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += THREAT_GRAVITY_SPEED;
        if(y_val_ >= THREAT_MAX_FALL_SPEED)
        {
            y_val_ = THREAT_MAX_FALL_SPEED;
        }
        if(input_type_.left_ == 1)
        {
            x_val_ -=THREAT_SPEED;
        }
        else if(input_type_.right_ == 1)
        {
            x_val_ += THREAT_SPEED;
        }
        CheckToMap(map_data);
    }
    else if(come_back_time_ > 0)
    {
        come_back_time_--;
        if(come_back_time_ == 0)
        {
            InitThreat();
        }
    }
}
void ThreatsObject::InitThreat()
{
    x_val_ = 0;
    y_val_ = 0;
    if(x_pos_ > 200)
    {
        x_pos_ -= 200;
        animation_a_ -= 200;
        animation_b_ -= 200;
    }
    else
        x_pos_ = 0;
    y_pos_ = 100;
    come_back_time_ = 0;
    input_type_.left_ = 1;
}
void ThreatsObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    int height_min = height_frame_  < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_ )/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
    {
        if(x_val_ > 0) //move right
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if((val1 != BLANK_TILE && val1 != ITEM_BULLET && val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != ITEM_BULLET && val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        else if(x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if((val1 != BLANK_TILE && val1 != ITEM_BULLET&& val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != ITEM_BULLET&& val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    int width_min =  width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;

    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val_ > 0)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];
            if((val1 != BLANK_TILE && val1 != ITEM_BULLET&& val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != ITEM_BULLET&& val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
        else if(y_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if((val1 != BLANK_TILE && val1 != ITEM_BULLET&& val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != ITEM_BULLET&& val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                y_pos_ = (y1 + 1)*TILE_SIZE;
                y_val_ = 0;
            }
        }
    }
    x_pos_ += x_val_;
    y_pos_ += y_val_;
    if(x_pos_ < 0)
        x_pos_ = 0;
    else if(x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

    if(y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 60;
    }
}

void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
    if(type_move_ == STATIC_THREAT)
    {
        //
    }
    else if(type_move_ == MOVE_IN_SPACE_THREAT)
    {
        if(on_ground_ == true)
        {
            if(x_pos_ > animation_b_)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("img//threat_left.png", screen);
            }
            else if(x_pos_ < animation_a_)
            {
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("img//threat_right.png", screen);
            }
        }
        else
        {
            if(input_type_.left_ == 1)
            {
                LoadImg("img//threat_left.png", screen);
            }

        }
    }
    else if(type_move_ == BOSS)
    {
        if(on_ground_ == true)
        {
            if(x_pos_ > animation_b_)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("img//boss_left.png", screen);
            }
            else if(x_pos_ < animation_a_)
            {
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("img//boss_right.png", screen);
            }
        }
        else
        {
            if(input_type_.left_ == 1)
            {
                LoadImg("img//boss_left.png", screen);
            }
        }

    }
}
void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen, int TypeMove )
{
    if(p_bullet != NULL)
    {
        if(TypeMove == ThreatsObject::STATIC_THREAT)
        {
            p_bullet->setDame(DAME_THREAT_BULLET);
            p_bullet->set_bullet_type(BulletObject::THREAT_BULLET);
            p_bullet->SetRect(rect_.x, y_pos_ + 25);
        }
        else if(TypeMove == ThreatsObject::MOVE_IN_SPACE_THREAT)
        {
            p_bullet->setDame(DAME_THREAT_MOVE_BULLET);
            p_bullet->set_bullet_type(BulletObject::THREAT_MOVE_BULLET);
            p_bullet->SetRect(rect_.x, y_pos_ + 15);
        }
        else if(TypeMove == ThreatsObject::BOSS)
        {
            p_bullet->setDame(DAME_BOSS_BULLET);
            p_bullet->set_bullet_type(BulletObject::BOSS_BULLET);
            p_bullet->SetRect(rect_.x, y_pos_ + 30);
        }
        bool ret = p_bullet->LoadImgBullet(screen);
        if(ret)
        {
            p_bullet->set_is_move(true);
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            p_bullet->set_x_val(10);
            bullet_list_.push_back(p_bullet);
        }
    }

}
void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    for(int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                int bullet_distance = abs(rect_.x + width_frame_ - p_bullet->GetRect().x);
                if(p_bullet->get_bullet_type() == BulletObject::BOSS_BULLET)
                {
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(screen);

                }
                else if(bullet_distance < 300 && bullet_distance > 0)
                {
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet->set_is_move(true);
                if(p_bullet->get_bullet_type() == BulletObject::BOSS_BULLET) p_bullet->SetRect(rect_.x, y_pos_ + 30);
                else if(p_bullet->get_bullet_type() == BulletObject::THREAT_BULLET) p_bullet->SetRect(rect_.x, y_pos_ + 25);
                else if(p_bullet->get_bullet_type() == BulletObject::THREAT_MOVE_BULLET) p_bullet->SetRect(rect_.x, y_pos_ + 15);
                if(input_type_.left_)
                    p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                else if(input_type_.right_)
                    p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
            }
        }
    }
}
void ThreatsObject::RemoveBullet(const int& id)
{
    int Size = bullet_list_.size();
    if(Size > 0 && id < Size)
    {
        BulletObject* p_bullet = bullet_list_.at(id);
        p_bullet->set_is_move(true);
        p_bullet->SetRect(rect_.x + 5, y_pos_ + 25);
        if(input_type_.left_)
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        else if(input_type_.right_)
            p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
    }
}

