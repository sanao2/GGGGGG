#pragma once
#include "pch.h"
#include "Texture.h"
#include "Player.h"
#pragma comment(lib, "Msimg32.lib")	


LPCTSTR		g_title = TEXT("윈도우 타이틀바에 표시할 문자열");
LPCTSTR		g_szClassName = TEXT("윈도우 클래스 이름");

//해상도 크기 기본 길이 
int			g_width = 1024;
int			g_height = 768;

HWND		g_hWnd;					// 윈도우 창 아이디 받기
HDC			g_FrontBufferDC;		// 앞면 DC
HDC			g_BackBufferDC;			// 뒷면 DC;

Texture*	texture; 
Player*		player;
Animation*  Anim;
Sprite		Spr; 

AnimState	cur_Anim;				// 현재의 애니메이션 상태
int			cur_Frame = 0;			// 현재의 애니메이션 프레임 

extern array<vector<Sprite>, AnimState::AnimCount> AnimFrame;

class Winmain
{
public : 
	void ProgressInit(); 
	void ProgressUpdate(); 
	void ProgressRender(); 
	void ProgressRelease();
	void ProgressInput(); 

	bool PeekMessages(MSG& msg);
	void Resolution(HINSTANCE hInstance, int nCmdShow, int g_width, int g_height);
	 
	Winmain(); 
	~Winmain();
};


