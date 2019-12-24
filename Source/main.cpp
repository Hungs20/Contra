
#include <windows.h>
#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "PlayerObject.h"
#include "Timer.h"
#include "ThreatObject.h"
#include "FontObject.h"
#undef main

BaseObject g_background;
TTF_Font* g_font_text = NULL;
TTF_Font* g_font_big = NULL;
bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if(ret == false)
        return false;
    return true;

}


void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    Mix_FreeMusic(g_music);
    Mix_Quit();
    SDL_Quit();
}
std::vector<ThreatsObject*> MakeThreatList()
{
    std::vector<ThreatsObject*> list_threat;


    ThreatsObject* boss_threat = new ThreatsObject();
    if(boss_threat != NULL)
    {
        boss_threat->setFrameNum(2);
        boss_threat->LoadImg("img//boss_left.png", g_screen);
        boss_threat->set_clips();
        boss_threat->set_type_move(ThreatsObject::BOSS);
        boss_threat->set_x_pos(19800);
        boss_threat->set_y_pos(110);
        boss_threat->set_input_left(1);
        boss_threat->setDame(DAME_BOSS);
        boss_threat->setHP(HP_BOSS);
        int pos1 = boss_threat->get_x_pos() - 120;
        int pos2 = boss_threat->get_x_pos() + 120;
        boss_threat->SetAnimation(pos1, pos2);


        BulletObject* p_bullet = new BulletObject();
        boss_threat->InitBullet(p_bullet, g_screen, ThreatsObject::BOSS);

        list_threat.push_back(boss_threat);
    }

    ThreatsObject* move_threat_objs = new ThreatsObject[15];
    for(int i = 0; i < 15; i++)
    {
        ThreatsObject* p_threat = (move_threat_objs + i);
        if(p_threat != NULL)
        {
            p_threat->setFrameNum(8);
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(1450 + i*700);
            p_threat->set_y_pos(100);
            p_threat->set_input_left(1);
            p_threat->setDame(DAME_MOVE_THREAT);
            p_threat->setHP(HP_MOVE_THREAT);
            int pos1 = p_threat->get_x_pos() - 80;
            int pos2 = p_threat->get_x_pos() + 80;
            p_threat->SetAnimation(pos1, pos2);


            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen, ThreatsObject::MOVE_IN_SPACE_THREAT);

            list_threat.push_back(p_threat);
        }
    }


    ThreatsObject* threat_objs = new ThreatsObject[20];

    for(int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threat_objs + i);
        if(p_threat != NULL)
        {
            p_threat->setFrameNum(8);
            p_threat->LoadImg("img//threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(900 + i * 1000);
            p_threat->set_y_pos(100);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->setDame(DAME_STATIC_THREAT);
            p_threat->setHP(HP_STATIC_THREAT);
            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen, ThreatsObject::STATIC_THREAT);
            list_threat.push_back(p_threat);
        }
    }
    return list_threat;
}

bool InitData()
{
    bool success = true;
    bool ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("StickMan",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if(g_window == NULL)
        success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    ///Music
    g_music = Mix_LoadMUS( "sound//background.wav" );
    if(g_music == NULL)
        return 0;
    g_sound_amo = Mix_LoadWAV("sound//shot.wav");
    if(g_sound_amo == NULL)
        return false;

    if(TTF_Init() == -1)
        return false;
    g_font_text = TTF_OpenFont("font//BodoniFLF-Bold.ttf", 18);
    if(g_font_text == NULL)
        return false;
    g_font_big = TTF_OpenFont("font//BodoniFLF-Bold.ttf", 30);
    if(g_font_big == NULL)
        return false;

    return success;
}


int main(int argc, char* argv[])
{
    Timer fps_timer;

    if(InitData() == false)
        return -1;

    if(LoadBackground() == false)
        return -1;
    Mix_PlayMusic(g_music, -1 );

    GameMap game_map;
    game_map.LoadMap("map//tiles//map01.dat");
    game_map.LoadTiles(g_screen);

    PlayerObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();

    textObject p_score;
    p_score.SetRect(0, 0);
    p_score.setColor(textObject::WHITE_TEXT);

    BaseObject hpBar;
    hpBar.SetRect(0, 20);
    hpBar.LoadImg("HP_10.png", g_screen);

    std::vector<ThreatsObject*> threats_list = MakeThreatList();

    bool is_quit = false;
    bool dieBoss = false;
    while(!is_quit)
    {
        fps_timer.start();
        while(SDL_PollEvent(&g_event)!= 0)
        {
            if(g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            if(p_player.getHP() > 0 && dieBoss == false) p_player.HandelInputAction(g_event, g_screen, g_sound_amo);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        //g_background.Render(g_screen, NULL);

        Map map_data = game_map.getMap();


        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);

        game_map.setMap(map_data);
        game_map.DrawMap(g_screen);



        p_player.HandleBullet(g_screen);
        p_player.Show(g_screen);


        int score = p_player.getMoney();
        std::string txtScore = "Score: ";
        txtScore += std::to_string(score);
        p_score.setText(txtScore);
        p_score.SetRect(0,0);
        p_score.setColor(textObject::WHITE_TEXT);
        p_score.CreateGameText(g_font_text, g_screen);

        int tmpHPbar = ceil((double)p_player.getHP() * 10 / PLAYER_HP);
        tmpHPbar = tmpHPbar > 0 ? tmpHPbar : 0;
        if(tmpHPbar == 0 && p_player.getHP() > 0)
            tmpHPbar = 1;
        hpBar.LoadImg(g_HP[tmpHPbar].c_str(), g_screen);
        hpBar.Render(g_screen);

        if(p_player.getHP() <= 0)
        {
            textObject gameOver;
            gameOver.SetRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
            gameOver.setColor(textObject::RED_TEXT);
            gameOver.setText("GAME OVER!");
            gameOver.CreateGameText(g_font_big, g_screen);
            p_score.SetRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+50);
            p_score.setColor(textObject::RED_TEXT);
            p_score.CreateGameText(g_font_big, g_screen);

            SDL_RenderPresent(g_screen);
            continue;
        }
        if(dieBoss)
        {
            textObject gameOver;
            gameOver.SetRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
            gameOver.setColor(textObject::GREEN_TEXT);
            gameOver.setText("VICTORY !");
            gameOver.CreateGameText(g_font_big, g_screen);
            p_score.SetRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+50);
            p_score.setColor(textObject::GREEN_TEXT);
            p_score.CreateGameText(g_font_big, g_screen);

            SDL_RenderPresent(g_screen);
            continue;
        }




        for(int i = 0; i < (int)threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {

                SDL_Rect rect_player = p_player.GetRectFrame();

                p_threat->Show(g_screen);
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                SDL_Rect rect_thread = p_threat->GetRectFrame();
                int distance = abs(rect_player.x - rect_thread.x);
                if(distance <= SCREEN_WIDTH + 100)
                {



                    p_threat->ImpMoveType(g_screen);
                    p_threat->DoPlayer(map_data);
                    p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);


                    /// Va cham Player - Bullet Threat
                    bool bCol1 = false;
                    std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                    for(int jj = 0; jj < tBullet_list.size(); jj++)
                    {
                        BulletObject* pt_bullet = tBullet_list.at(jj);
                        if(pt_bullet)
                        {
                            bCol1 = CheckCollision(pt_bullet->GetRect(), rect_player);
                            if(bCol1 == true)
                            {
                                p_player.setHP(p_player.getHP() - pt_bullet->getDame());
                                p_threat->RemoveBullet(jj);
                                if(p_player.getHP() <= 0)
                                    break;
                            }
                        }
                    }

                    ///Va cham Player - Threat

                    bool bCol2 = CheckCollision(rect_player, rect_thread);
                    if(bCol2)
                    {
                        p_player.setHP(p_player.getHP() - p_threat->getDame());
                    }

                }
            }

            ///Va cham bullet Player - Threat
            std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
            for(int r = 0; r < bullet_arr.size(); r++)
            {
                BulletObject* p_bullet = bullet_arr.at(r);
                if(p_bullet != NULL)
                {
                    for(int t = 0; t < threats_list.size(); t++)
                    {
                        ThreatsObject* obj_threat = threats_list.at(t);
                        if(obj_threat != NULL)
                        {
                            SDL_Rect tRect;
                            tRect.x = obj_threat->GetRect().x;
                            tRect.y = obj_threat->GetRect().y;
                            tRect.w = obj_threat->get_width_frame();
                            tRect.h = obj_threat->get_height_frame();

                            SDL_Rect bRect = p_bullet->GetRect();

                            bool bCol = CheckCollision(bRect, tRect);
                            if(bCol)
                            {
                                p_player.RemoveBullet(r);
                                obj_threat->setHP(obj_threat->getHP() - p_bullet->getDame());
                                if(obj_threat->getHP() <= 0)
                                {
                                    p_player.SetMoney();
                                    if(obj_threat->get_type_move() == ThreatsObject::BOSS) dieBoss = true;
                                    obj_threat->Free();
                                    threats_list.erase(threats_list.begin() + t);
                                }
                            }
                        }
                    }
                }
            }
        }



        SDL_RenderPresent(g_screen);

        int real_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FPS;
        if(real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            SDL_Delay(delay_time);
        }
    }

    for(int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject* p_threat = threats_list.at(i);
        if(p_threat != NULL)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    close();
    return 0;
}
