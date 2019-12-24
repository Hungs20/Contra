#include "PlayerObject.h"
PlayerObject::PlayerObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 100;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.up_ = 0;
    input_type_.down_ = 0;
    input_type_.jump_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_count = 0;
    num_bullet_laser = 0;
    HP_ = PLAYER_HP;
}
PlayerObject::~PlayerObject()
{
    ;
}
void PlayerObject::SetMoney()
{
    money_count++;
}

bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w / 8;
        height_frame_ = rect_.h;
    }
   // std::cout << width_frame_ << " " << height_frame_ << "\n";
    return ret;
}
SDL_Rect PlayerObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}
void PlayerObject::set_clips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for(int i = 0; i < 8; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;

        }
    }
}

void PlayerObject::Show(SDL_Renderer* des)
{

    UpdateImagePlayer(des);

    if(input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
        frame_ = 0;

    if(frame_ >= 8)
        frame_ = 0;

    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }

}

void PlayerObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* sound_amo)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0;
            UpdateImagePlayer(screen);
        }
        break;
        case SDLK_LEFT:
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            UpdateImagePlayer(screen);
        }
        break;
        case SDLK_UP:
            {
                input_type_.jump_ = 1;
            }
            break;

        }

    }
    else if(events.type == SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            input_type_.right_ = 0;
        }
        break;
        case SDLK_LEFT:
        {
            input_type_.left_ = 0;
        }
        break;
        case SDLK_UP:
            {
                input_type_.jump_ = 0;
            }
            break;
        }
    }
    if(events.type == SDL_MOUSEBUTTONDOWN)
    {
        if(events.button.button == SDL_BUTTON_RIGHT)
        {

            BulletObject* p_bullet = new BulletObject();

            if(status_ == WALK_LEFT) status_ = WALK_LEFT_UP;
            else if(status_ == WALK_RIGHT) status_= WALK_RIGHT_UP;

            if(status_ == WALK_LEFT_UP)
            {
                if(on_ground_) p_bullet->set_bullet_dir(BulletObject::DIR_UP_LEFT);
                else p_bullet->set_bullet_dir(BulletObject::DIR_DOWN_LEFT);
                p_bullet->SetRect(this->rect_.x, rect_.y + 5);
            }
            else if(status_ == WALK_RIGHT_UP)
            {
                if(on_ground_) p_bullet->set_bullet_dir(BulletObject::DIR_UP_RIGHT);
                else p_bullet->set_bullet_dir(BulletObject::DIR_DOWN_RIGHT);
                p_bullet->SetRect(this->rect_.x + width_frame_ - 25, rect_.y + 10);
            }
            if(num_bullet_laser > 0)
            {
                p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
                num_bullet_laser--;
                p_bullet->setDame(DAME_LASER_BULLET);
            }
            else
            {
                p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
                p_bullet->setDame(DAME_SPHERE_BULLET);
            }

            p_bullet->LoadImgBullet(screen);

            p_bullet->set_x_val(20);
            p_bullet->set_y_val(20);
            p_bullet->set_is_move(true);

            p_bullet_list_.push_back(p_bullet);
            int ret = Mix_PlayChannel(-1, sound_amo, 0);
        }
        else if(events.button.button == SDL_BUTTON_LEFT)
        {
            BulletObject* p_bullet = new BulletObject();

            if(status_ == WALK_LEFT_UP) status_ = WALK_LEFT;
            else if(status_ == WALK_RIGHT_UP) status_ = WALK_RIGHT;


            if(status_ == WALK_LEFT)
            {
                p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                p_bullet->SetRect(this->rect_.x, rect_.y + height_frame_ * 0.35);
            }
            else
            {
                p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.35);
            }
            if(num_bullet_laser > 0)
            {
                p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
                num_bullet_laser--;
                p_bullet->setDame(DAME_LASER_BULLET);
            }
            else
            {
                p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
                p_bullet->setDame(DAME_SPHERE_BULLET);
            }

            p_bullet->LoadImgBullet(screen);


            p_bullet->set_x_val(20);
            p_bullet->set_y_val(20);
            p_bullet->set_is_move(true);

            p_bullet_list_.push_back(p_bullet);
            int ret = Mix_PlayChannel(-1, sound_amo, 0);
        }
    }
    else if(events.type == SDL_MOUSEBUTTONUP)
    {
            if(status_ == WALK_LEFT_UP) status_ = WALK_LEFT;
            else if(status_ == WALK_RIGHT_UP) status_ = WALK_RIGHT;

    }
}

void PlayerObject::HandleBullet(SDL_Renderer* des)
{
    for(int i = 0; i < p_bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move() == true)
            {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if(p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void PlayerObject::RemoveBullet(const int& id)
{
    int Size = p_bullet_list_.size();
    if(Size > 0 && id < Size)
    {
        BulletObject* p_bullet = p_bullet_list_.at(id);
        p_bullet_list_.erase(p_bullet_list_.begin() + id);

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void PlayerObject::DoPlayer(Map& map_data)
{
    if(come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;
        if(y_val_ >= MAX_FALL_SPEED)
            y_val_ = MAX_FALL_SPEED;

        if(input_type_.left_ == 1)
            x_val_ -= PLAYER_SPEED;
        else if(input_type_.right_ == 1)
            x_val_ += PLAYER_SPEED;

        if(input_type_.jump_ == 1)
        {
            if(on_ground_ == true)
            {
                y_val_ = -PLAYER_JUMP_VALUE;
            }

            on_ground_ = false;
            input_type_.jump_ = 0;
        }

        CheckToMap(map_data);
        CenterOnMap(map_data);
    }

    if(come_back_time_ > 0)
    {
        come_back_time_--;
        if(come_back_time_ == 0) // Reset lai
        {
            on_ground_ = false;
            if(x_pos_ > 200)
            {
                x_pos_ -= 200; //4 tile map
            }
            else
                x_pos_ = 0;
            y_pos_ = 100;
            x_val_ = 0;
            y_val_ = 0;
        }
    }

}
void PlayerObject::CheckToMap(Map& map_data)
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

            if(val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                SetMoney();
            }
            else if(val1 == 71 || val1 == 72 || val2 == 71 || val2 == 72)
            {
                HP_ -= DAME_MAP;
            }
            else if(val1 == ITEM_BULLET || val2 == ITEM_BULLET)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                num_bullet_laser += 30;
            }
            else
            {
                    if((val1 != BLANK_TILE && val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;
                }
            }

        }
        else if(x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if(val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                SetMoney();
            }
            else if(val1 == 71 || val1 == 72 || val2 == 71 || val2 == 72)
            {
                HP_ -= DAME_MAP;
            }
            else if(val1 == ITEM_BULLET || val2 == ITEM_BULLET)
            {
                num_bullet_laser += 30;
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
            }
            else
            {
                if((val1 != BLANK_TILE && val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
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
            if(val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                SetMoney();
            }
            else if(val1 == 71 || val1 == 72 || val2 == 71 || val2 == 72)
            {
                HP_ -= DAME_MAP;
            }
            else if(val1 == ITEM_BULLET || val2 == ITEM_BULLET)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                num_bullet_laser += 30;
            }
            else
            {
                    if((val1 != BLANK_TILE && val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                    y_pos_ = y2*TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    y_val_ = 0;
                    on_ground_ = true;
                    if(status_ == WALK_NONE)
                    {
                        status_ = WALK_RIGHT;
                    }
                }
            }

        }
        else if(y_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if(val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                SetMoney();
            }
            else if(val1 == 71 || val1 == 72 || val2 == 71 || val2 == 72)
            {
                HP_ -= DAME_MAP;
            }
            else if(val1 == ITEM_BULLET || val2 == ITEM_BULLET)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                num_bullet_laser += 30;
            }
            else
            {
                if((val1 != BLANK_TILE && val1 != 38 && val1 != 39 && val1 != 40 && val1 != 41 && val1 != 42 && val1 != 43 && val1 != 44 && val1 != 45 && val1 != 57 && val1 != 58 && val1 != 59 && val1 != 60 && val1 != 61 && val1 != 62 && val1 != 63 && val1 != 64 && val1 != 65 && val1 != 66 && val1 != 67 && val1 != 73 && val1 != 74 && val1 != 75 && val1 != 76 && val1 != 77 && val1 != 78 && val1 != 79)
                        || (val2  != BLANK_TILE && val2 != 38 && val2 != 39 && val2 != 40 && val2 != 41 && val2 != 42 && val2 != 43 && val2 != 44 && val2 != 45 && val2 != 57 && val2 != 58 && val2 != 59 && val2 != 60 && val2 != 61 && val2 != 62 && val2 != 63 && val2 != 64 && val2 != 65 && val2 != 66 && val2 != 67 && val2 != 73 && val2 != 74 && val2 != 75 && val2 != 76 && val2 != 77 && val2 != 78 && val2 != 79))
                    {
                    y_pos_ = (y1 + 1)*TILE_SIZE;
                    y_val_ = 0;
                }
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
        HP_ -= DAME_MAP;
    }
}
void PlayerObject::CenterOnMap(Map& map_data)
{
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH)/2;
    if(map_data.start_x_ < 0)
        map_data.start_x_ = 0;
    else if(map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2);
    if(map_data.start_y_ < 0)
        map_data.start_y_ = 0;
    else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}
void PlayerObject::UpdateImagePlayer(SDL_Renderer* des)
{
    if(on_ground_ == true)
    {
        if(status_ == WALK_LEFT_UP)
        {
            LoadImg("img//player_left_up.png", des);
        }
        else if(status_ == WALK_RIGHT_UP)
        {
            LoadImg("img//player_right_up.png", des);
        }
        else if(status_ == WALK_LEFT)
        {
             LoadImg("img//player_left.png", des);
        }
        else
        {
            LoadImg("img//player_right.png", des);
        }
    }
    else
    {
        if(status_ == WALK_LEFT || status_ == WALK_LEFT_UP)
        {
            LoadImg("img//jump_left.png", des);
        }
        else
        {
            LoadImg("img//jump_right.png", des);
        }
    }
}
