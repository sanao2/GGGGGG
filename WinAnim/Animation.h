#pragma once
#include "pch.h"
#include <iostream>
using namespace std;

#include <unordered_map>
using::unordered_map;

// 프레임 시간. 프레임을 어떤 신호로 변경할 것인지. 
// 프레임 현재 프레임이 무엇인지 확인하고, 어떤 조건을 통해서 변경할 것인지. 
// 여러 장의 사진을 불러와 배열에 저장하고, 
// 배열에 저장한 사진들을 반복문을 통해서 저장하고 불러온다. 

typedef enum AnimState : size_t {
	ATTACK,
	JUMPATT,
	RUN,
	ANIMCOUNT
};
//inline const char* AnimPath(int cur_AnimState) //이미지 경로 반환 
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


// 예시 최적화. 
const wstring RESOURCE_DIR = L"..\\Resources\\"; 

inline wstring GetAnimPath(AnimState state)
{
	// static 으로 일정한 값을 구현 
	static wstring paths[] = {
		L"attack.txt",
		L"jumpattack.txt",
		L"RUN.txt"
	};

	if (state < 0 || state >= ANIMCOUNT) return nullptr; 

	return RESOURCE_DIR + paths[state];
}

//예제
struct ResourceInfo {
	std::string animTxt;
	std::wstring spriteImg;
};

// mapping 
const unordered_map<AnimState, ResourceInfo> resourceMap = {
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

//extern 은 전역으로 관리 시 인스턴스 충돌 발생 가능성 높음 
//extern AnimState cur_Anim;				// 현재의 애니메이션 상태
//extern int cur_Frame;					// 현재의 애니메이션 프레임 

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
	
	// GDI+ 초기화용
	ULONG_PTR g_GdiPlusToken;
	GdiplusStartupInput GSI;
	Image* Image;
	Graphics* BackBufferGraphics;

	FILE* fp;
	UINT ImgWidth = 0;
	UINT ImgHeight = 0;

	// 모든 애니메이션 프레임을 저장할 벡터 

	vector<vector<Sprite>, AnimState> AnimFrame;
	
	// 애니메이션 프레임 지속 시간 
	float		curDurtionTime = 0.0f;

	// 애니메이션 멤버 변수 
	Sprite Spr;

	AnimState cur_Anim; 
	int cur_Frame; 
public : 
	Animation(HWND hWnd);
	~Animation(); 

	void Intialize(AnimState cur_anim);	 // 현재의 애니메이션에 따른 이미지들을 벡터에 저장한다. 
	void Update();						 // 현재 애니메이션의 변경점. 프레임 시간에 따라서 키보드 인풋 정보에 따라서 출력할 준비를 한다. 
	void Render(int cur_Anim);			 // 현재 애니메이션의 프레임 상태에 따라서 애니메이션 출력 
	
	void FrameTime();
	string ConvertWStringToString(const wstring& wstr);
	 // 애니메이션을 만들 함수. 현재의 프레임 상태에 맞는 프레임에 들어갈 이미지들이 저장된 AnimFrame에서 이미지를 받아와서 반복문을 통해서 저장 
	 // 프레임을 그릴 위치, 그리고 프레임에 적용할 
	Sprite CreateFrame(AnimState AnimName, Vector2 LeftTop, Vector2 size, Vector2 offset, float durctionTime);

	// 현재의 애니메이션 프레임 상태 get, set
	AnimState GetcurAnimState() { return cur_Anim; };
	AnimState SetcurAnimState(AnimState SetcurAnim) { return cur_Anim = SetcurAnim; };
};


