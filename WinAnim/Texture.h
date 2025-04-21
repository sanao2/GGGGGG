#pragma once
#include "pch.h"
#include "Animation.h"
#include "gdiplus.h"
#include "Time.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Msimg32.lib")

extern void PrintLastErrorMessage();

extern int g_width;
extern int g_height;

class Animation; 

class Texture 
{
private : 

	HWND hWnd_;
	HDC FrontBufferDC;
	HDC BackBufferDC;
	HDC ImageDC;

	HBITMAP HBackBufferBitmap;

	// GDI+ �ʱ�ȭ��
	ULONG_PTR g_GdiPlusToken;
	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::Image* Image;
	Gdiplus::Graphics* BackBufferGraphics;

	BITMAP bmpInfo; // ���ο� ũ�� ������ �ִ�.
		
	UINT ImgWidth = 0; 
	UINT ImgHeight = 0; 

	Animation* Anim;

public : 
	void TextureUpdate();
	void GDIRender();
	void GDIplusRender(); 

	void GDILoadimage(); 
	void GDIplusLoadImage(AnimState curAnim);
	
	Texture(HWND hWnd);
	~Texture(); 

	// ��Ʈ�� �̹��� ���� ���� ���� ���� ĸ��ȭ 
	UINT GetImgWidth() { return ImgWidth = Image->GetWidth(); }
	UINT GetImgHeight() { return ImgHeight = Image->GetHeight(); }
	
	void SetAnimation(Animation* anim) { Anim = anim; }

	friend class Animation; 

};

