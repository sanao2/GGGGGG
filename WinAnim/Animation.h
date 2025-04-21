#pragma once
#include "pch.h"
#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <gdiplus.h>
#include <map>
#include <unordered_map>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// ������ �ð�. �������� � ��ȣ�� ������ ������. 
// ������ ���� �������� �������� Ȯ���ϰ�, � ������ ���ؼ� ������ ������. 
// ���� ���� ������ �ҷ��� �迭�� �����ϰ�, 
// �迭�� ������ �������� �ݺ����� ���ؼ� �����ϰ� �ҷ��´�. 

typedef enum AnimState : size_t {
	ATTACK,
	JUMPATT,
	RUN,
	ANIMCOUNT
};
//inline const char* AnimPath(int cur_AnimState) //�̹��� ��� ��ȯ 
//{ // inline  
//	const char* path = "";
//
//	switch (cur_AnimState)
//	{
//	case attack:
//		path = "..\\Resources\\attack.txt";
//		return path;
//		break;
//	case jumpAtt:
//		path = "..\\Resources\\jumpattack.txt";
//		return path;
//		break;
//	case Run:
//		path = "..\\Resources\\RUN.txt";
//		return path;
//		break;
//	default: return NULL;
//	}
//}
//
//inline const WCHAR* SpritePath(int cur_anim)
//{
//	const WCHAR* Spritepath = L"";
//	switch (cur_anim)
//	{
//	case attack:
//		Spritepath = L"..\\Resources\\TurnAttack.png";
//		return Spritepath;
//		break;
//	case jumpAtt:
//		Spritepath = L"..\\Resources\\jumpattack.png";
//		return Spritepath;
//		break;
//	case Run:
//		Spritepath = L"..\\Resources\\RUN.png";
//		return Spritepath;
//		break;
//	default: return NULL;
//	}
//}


// ���� ����ȭ. 
const std::string RESOURCE_DIR = "..\\Resources\\"; 

inline std::string GetAnimPath(AnimState state)
{
	// static ���� ������ ���� ���� 
	static std::string paths[] = {
		"attack.txt",
		"jumpattack.txt",
		"RUN.txt"
	};

	if (state < 0 || state >= ANIMCOUNT) return nullptr; 

	return RESOURCE_DIR + paths[state];
}

//����
struct ResourceInfo {
	std::string animTxt;
	std::wstring spriteImg;
};

// mapping 
const std::unordered_map<AnimState, ResourceInfo> resourceMap = {
   {ATTACK, {"attack.txt", L"TurnAttack.png"}}
};
const std::string GetAnimPaths(AnimState state)
{
	auto it = resourceMap.find(state);

	if (it == resourceMap.end()) {
		return RESOURCE_DIR + it->second.animTxt;

	}
	return "";
}

struct Sprite
{
	Vector2 LeftTop; 
	Vector2 size; 
	Vector2 offset; 
	float DurctionTime; 
};

//extern �� �������� ���� �� �ν��Ͻ� �浹 �߻� ���ɼ� ���� 
//extern AnimState cur_Anim;				// ������ �ִϸ��̼� ����
//extern int cur_Frame;					// ������ �ִϸ��̼� ������ 

extern int			g_width;
extern int			g_height;

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

	vector<vector<Sprite>, AnimState> AnimFrame;
	
	// �ִϸ��̼� ������ ���� �ð� 
	float		curDurtionTime = 0.0f;

	// �ִϸ��̼� ��� ���� 
	Sprite Spr;

	AnimState cur_Anim; 
	int cur_Frame; 
public : 
	Animation(HWND hWnd);
	~Animation(); 

	void Intialize(AnimState cur_anim);	 // ������ �ִϸ��̼ǿ� ���� �̹������� ���Ϳ� �����Ѵ�. 
	void Update();						 // ���� �ִϸ��̼��� ������. ������ �ð��� ���� Ű���� ��ǲ ������ ���� ����� �غ� �Ѵ�. 
	void Render(int cur_Anim);			 // ���� �ִϸ��̼��� ������ ���¿� ���� �ִϸ��̼� ��� 
	 
	void FrameTime();

	 // �ִϸ��̼��� ���� �Լ�. ������ ������ ���¿� �´� �����ӿ� �� �̹������� ����� AnimFrame���� �̹����� �޾ƿͼ� �ݺ����� ���ؼ� ���� 
	 // �������� �׸� ��ġ, �׸��� �����ӿ� ������ 
	Sprite CreateFrame(AnimState AnimName, Vector2 LeftTop, Vector2 size, Vector2 offset, float durctionTime);

	// ������ �ִϸ��̼� ������ ���� get, set
	AnimState GetcurAnimState() { return cur_Anim; };
	AnimState SetcurAnimState(AnimState SetcurAnim) { return cur_Anim = SetcurAnim; };
};


