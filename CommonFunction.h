// define cac bien su dung chung

#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include<Windows.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//screen
const int FRAME_PER_SECOND = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32; // do to man hinh chinh

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180; //mau` cua background icon trong hinh nhan vat

const int RENDER_DRAW_COLOR = 255;

#define BLANK_TILE 0
#define TILE_SIZE 64 // 1 tile = 64x64 pixels

#define MAX_MAP_X 400
#define MAX_MAP_Y 10 // so tile cua map

#define STATE_MONEY 4

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

namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

#endif