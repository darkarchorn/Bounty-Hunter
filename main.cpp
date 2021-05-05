// SDL2Game.cpp : Defines the entry point for the console application.
//
#include<iostream>
#include "stdafx.h"
#include "CommonFunction.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "game_map.h"
#include "ImpTimer.h"
#include "ThreatObject.h"
#include "Text.h"
using namespace std;

BaseObject g_background;
TTF_Font* font_time;


bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret<0)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("game dang chay ngon vl buh buh lmao",
								SDL_WINDOWPOS_UNDEFINED, 
								SDL_WINDOWPOS_UNDEFINED, 
								SCREEN_WIDTH, 
								SCREEN_HEIGHT, 
								SDL_WINDOW_SHOWN);
	if(g_window == NULL)
	{
		success = false;
	}
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
		if(TTF_Init() == -1)
		{
			success = false;
		}
		font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if(font_time == NULL)
		{
			success = false;
		}
	}
	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("img/background.png", g_screen);
	if(ret == false)
		return false;
	return true;
}

void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	IMG_Quit();
	SDL_Quit();
}

std::vector<ThreatObject*>MakeThreatList()
{
	std::vector<ThreatObject*> list_threats;
	ThreatObject* dynamic_threats = new ThreatObject[20];
	for(int i=0;i<20;i++)
	{
		ThreatObject* p_threat = (dynamic_threats + i);
		if(p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500+i*500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() -60;
			int pos2 = p_threat->get_x_pos() +60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);
		}
	}

	ThreatObject* threats_objs = new ThreatObject[20];
	for(int i=0;i<20;i++)
	{
		ThreatObject* p_threat = (threats_objs + i);
		if(p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(i*1200+700);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int main(int argc, char* argv[])
{
	ImpTimer fps_timer;

	int health = 120;
	if(InitData() == false)
		return -1;
	if(LoadBackground() == false)
		return -1;

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
	{
		std::cout << Mix_GetError() << std::endl;
	}
	Mix_Music* bgm = Mix_LoadMUS("audio/bgm.mp3");
	Mix_PlayMusic(bgm, -1);

	GameMap game_map;
	game_map.LoadMap("map/map01.dat");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img/player_right.png", g_screen);
	p_player.set_clips();

	std::vector<ThreatObject*> threats_list = MakeThreatList();

	int num_die = p_player.GetLife()-1;

	Text time_game;
	time_game.SetColor(Text::WHITE_TEXT);

	Text mark_game;
	mark_game.SetColor(Text::WHITE_TEXT);
	UINT mark_value = 0;

	Text money_game;
	money_game.SetColor(Text::WHITE_TEXT);

	bool is_quit = false;
	while(!is_quit)
	{
		fps_timer.start();
		while(SDL_PollEvent(&g_event)!=0)
		{
			if(g_event.type==SDL_QUIT)
			{
				is_quit=true;
			}
			p_player.HandleInputAction(g_event, g_screen);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);

		for(int i=0; i < threats_list.size(); i++)
		{
			ThreatObject* p_threat = threats_list.at(i);
			bool live = p_threat->isalive();
			if(live)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;
				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for(int j = 0; j< tBullet_list.size(); ++j)
				{
					BulletObject* pt_bullet = tBullet_list.at(j);
					if(pt_bullet)
					{
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if(bCol1)
						{
							//mark_value ++;
							//p_threat->RemoveBullet(j);

							health--;
							if(health==-2)
							{
								Mix_FreeMusic(bgm);
								if(MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
								{
									close();
									SDL_Quit();
									return 0;
								}
							}

							
							bCol1 = false;
							break;
						}
					}
				}

				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				if(bCol1 || bCol2 || p_player.get_y()==map_data.max_y_)
				{
					num_die++;
					if(num_die<=2)
					{
						p_player.SetRect(0,0);
						p_player.set_comeback_time(60);
						SDL_Delay(100);
						continue;
					}
					else {Mix_FreeMusic(bgm);if(MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
					{
						p_threat->Free();
						close();
						SDL_Quit();
						return 0;}
					}
				}
			}
		}


		std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for(int r = 0; r < bullet_arr.size();++r)
		{
			BulletObject* p_bullet = bullet_arr.at(r);
			if(p_bullet != NULL)
			{
				for(int t =0; t<threats_list.size(); ++t)
				{
					ThreatObject* obj_threat= threats_list.at(t);
					if(obj_threat != NULL)
					{
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						if(bCol)
						{
							mark_value++;
							obj_threat->kill();
							std::cerr << "hit!" << endl;
							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin()+t);
						}
					}
				}
			}
		}

		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks()/1000;
		Uint32 val_time = 180 - time_val;
		if(val_time <=0)
		{
			Mix_FreeMusic(bgm);
			if(MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				is_quit = true;
				break;
			}
		}
		
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.Set_text(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH-200, 15);
		}

		std::string val_str_mark = std:: to_string(mark_value);
		std::string strMark("Enemies Killed: ");
		strMark+= val_str_mark;

		mark_game.Set_text(strMark);
		mark_game.LoadFromRenderText(font_time, g_screen);
		mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5-50,15);

		int money_count = p_player.GetMoneyCount();
		int money_left = 33;
		std::string money_str = std::to_string(money_count);
		std::string moneyleft = std::to_string(money_left);
		std::string str_money("Coin: ");
		str_money = str_money + money_str + '/' + moneyleft;

		money_game.Set_text(str_money);
		money_game.LoadFromRenderText(font_time, g_screen);
		money_game.RenderText(g_screen, 15, 15);

		std::string health_str = std::to_string(health);
		std::string strHealth("Health: ");
		strHealth = strHealth + health_str + " / " + std::to_string(4- p_player.GetLife());

		money_game.Set_text(strHealth);
		money_game.LoadFromRenderText(font_time, g_screen);
		money_game.RenderText(g_screen, SCREEN_WIDTH*0.25, 15);
		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000/FRAME_PER_SECOND; //ms

		if(money_count == 33)
		{
			Mix_FreeMusic(bgm);
			if(MessageBox(NULL, L"YOU WON!", L"Info", MB_ICONINFORMATION | MB_ICONSTOP) == IDOK)
			{
				is_quit = true;
				break;
			}
		}

		if(real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if(delay_time >= 0)
				SDL_Delay(delay_time);
		}
	}

	for(int i=0; i<threats_list.size(); i++)
	{
		ThreatObject* p_threat = threats_list.at(i);
		if(p_threat)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}

	threats_list.clear();

	close();
	return 0;
}

//You have 180 seconds to collect all 33 coins on the map and that is your winning condition. You have the 100 health and 3 lives to live on and fight!
