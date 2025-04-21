#pragma once
#include "pch.h"
#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// ������ �ð�. �������� � ��ȣ�� ������ ������. 
// ������ ���� �������� �������� Ȯ���ϰ�, � ������ ���ؼ� ������ ������. 
// ���� ���� ������ �ҷ��� �迭�� �����ϰ�, 
// �迭�� ������ �������� �ݺ����� ���ؼ� �����ϰ� �ҷ��´�. 


typedef enum AnimState : size_t {
	attack,
	jumpAtt,
	Run,
	AnimCount
};
inline const char* AnimPath(int cur_AnimState) //�̹��� ��� ��ȯ 
{
	const char* path = "";

	switch (cur_AnimState)
	{
	case attack:
		path = "..\\Resources\\attack.txt";
		return path;
		break;
	case jumpAtt:
		path = "..\\Resources\\jumpattack.txt";
		return path;
		break;
	case Run:
		path = "..\\Resources\\RUN.txt";
		return path;
		break;
	default: return NULL;
	}
}
inline const WCHAR* SpritePath(int cur_anim)
{
	const WCHAR* Spritepath = L"";
	switch (cur_anim)
	{
	case attack:
		Spritepath = L"..\\Resources\\TurnAttack.png";
		return Spritepath;
		break;
	case jumpAtt:
		Spritepath = L"..\\Resources\\jumpattack.png";
		return Spritepath;
		break;
	case Run:
		Spritepath = L"..\\Resources\\RUN.png";
		return Spritepath;
		break;
	default: return NULL;
	}
}

struct Sprite
{
	Vector2 LeftTop; 
	Vector2 size; 
	Vector2 offset; 
	float DurctionTime; 
};

extern AnimState cur_Anim;				// ������ �ִϸ��̼� ����
extern int cur_Frame;					// ������ �ִϸ��̼� ������ 

class Animation 
{
private : 
	HWND hWnd_;
	HDC FrontBufferDC;
	HDC BackBufferDC;
	HDC ImageDC;

	HBITMAP HBackBufferBitmap;
	
	// GDI+ �ʱ�ȭ��
	ULONG_PTR g_GdiPlusToken;
	GdiplusStartupInput GSI;
	Image* Image;
	Graphics* BackBufferGraphics;
	
	FILE* fp;
	UINT ImgWidth = 0;
	UINT ImgHeight = 0;

	// ��� �ִϸ��̼� �������� ������ ���� 

	vector<vector<Sprite>> AnimFrame;
	
	// �ִϸ��̼� ������ ���� �ð� 
	float		curDurtionTime = 0.0f;

	// �ִϸ��̼� ��� ���� 
	Sprite Spr;
public : 
	Animation(HWND hWnd);
	~Animation(); 

	void Intialize(AnimState cur_anim);	 // ������ �ִϸ��̼ǿ� ���� �̹������� ���Ϳ� �����Ѵ�. 
	void Update();							 // ���� �ִϸ��̼��� ������. ������ �ð��� ���� Ű���� ��ǲ ������ ���� ����� �غ� �Ѵ�. 
	void Render(int cur_Anim);							 // ���� �ִϸ��̼��� ������ ���¿� ���� �ִϸ��̼� ��� 
	void LoadIMg(int curAnim);

	void Render_begin(); 
	void FrameTime();

	 // �ִϸ��̼��� ���� �Լ�. ������ ������ ���¿� �´� �����ӿ� �� �̹������� ����� AnimFrame���� �̹����� �޾ƿͼ� �ݺ����� ���ؼ� ���� 
	 // �������� �׸� ��ġ, �׸��� �����ӿ� ������ 
	Sprite CreateFrame(AnimState AnimName, Vector2 LeftTop, Vector2 RightBottom, Vector2 offset, float durctionTime);

	// ������ �ִϸ��̼� ������ ���� get, set
	AnimState GetcurAnimState() { return cur_Anim; };
	AnimState SetcurAnimState(AnimState SetcurAnim) { return cur_Anim = SetcurAnim; };
};


