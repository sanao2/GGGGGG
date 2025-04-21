#pragma once
#include "pch.h"
#include "Texture.h"
#include "Player.h"
#pragma comment(lib, "Msimg32.lib")	


LPCTSTR		g_title = TEXT("������ Ÿ��Ʋ�ٿ� ǥ���� ���ڿ�");
LPCTSTR		g_szClassName = TEXT("������ Ŭ���� �̸�");

//�ػ� ũ�� �⺻ ���� 
int			g_width = 1024;
int			g_height = 768;

HWND		g_hWnd;					// ������ â ���̵� �ޱ�
HDC			g_FrontBufferDC;		// �ո� DC
HDC			g_BackBufferDC;			// �޸� DC;

Texture*	texture; 
Player*		player;
Animation*  Anim;
Sprite		Spr; 

AnimState	cur_Anim;				// ������ �ִϸ��̼� ����
int			cur_Frame = 0;			// ������ �ִϸ��̼� ������ 

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


