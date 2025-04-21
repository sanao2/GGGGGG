#include "Texture.h"



Texture::Texture(HWND hWnd) : hWnd_(hWnd)
{
	RECT rc;
	GetClientRect(hWnd_, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	////////Renderer::Initialize
	// 맞는 DC를 가져온 후, 버퍼의 생성 
	FrontBufferDC = GetDC(hWnd_); //윈도우 클라이언트 영역의 DeviceContext얻기
	BackBufferDC = CreateCompatibleDC(FrontBufferDC); // 호환되는 DeviceContext 생성

	// 메모리 영역생성
	// HDC => DC의 핸들 아이디의 가로와 세로 길이를 받아와 영역을 생성. 
	HBackBufferBitmap = CreateCompatibleBitmap(FrontBufferDC, g_width, g_height);
	SelectObject(BackBufferDC, HBackBufferBitmap); // g_BackBufferDC의 메모리영역 지정

	// DeviceContext 생성 및 HBitmap 연결
	ImageDC = CreateCompatibleDC(FrontBufferDC);	// 호환되는 DeviceContext 생성

	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
	BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);
}

Texture::~Texture()
{
	// 다 쓴 것들 삭제. 
	delete BackBufferGraphics;
	delete Image;
	Gdiplus::GdiplusShutdown(g_GdiPlusToken);

	DeleteObject(HBackBufferBitmap);
	DeleteDC(ImageDC);

	// Renderer::Uninitialize
	DeleteObject(HBackBufferBitmap);
	DeleteDC(BackBufferDC);
	ReleaseDC(hWnd_, FrontBufferDC);
}

void Texture::TextureUpdate() //나중에 애니메이션 상태에 따라서 계속해서 이미지를 바꿔줄 옵션 (반복문) 
{
	if (Image == nullptr)
	  GDIplusLoadImage(Anim->GetcurAnimState());
}

void Texture::GDIRender()
{	
	// Renderer::BeginDraw()
		// 앞에 그렸던 것을 지운다. 
		// DC 후면 버퍼의 앞쪽에 그려놓는다. // 더블 버퍼 그리기 연산. 
	PatBlt(BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);

	// Render()
	// 전체 이미지 그대로 복사하기
	BitBlt(BackBufferDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, ImageDC, 0, 0, SRCCOPY);

	//특정 컬러(컬러키)를 제외하고 그리기
	TransparentBlt(BackBufferDC, bmpInfo.bmWidth, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
		ImageDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
		RGB(255, 255, 255));	// Msimg32.lib 링크 필요

	// Renderer::EndDraw()
	// DC 전면 버퍼로 가져와서 그린다. 
	BitBlt(FrontBufferDC, 0, 0, g_width, g_height, BackBufferDC, 0, 0, SRCCOPY);
}

void Texture::GDIplusRender()
{
	if (Image == nullptr) return;

	BackBufferGraphics->DrawImage(Image, Gdiplus::Rect(0, 0, Image->GetWidth(), Image->GetHeight()));
	GetImgWidth();
	GetImgHeight();

	BitBlt(FrontBufferDC, 0, 0, ImgWidth, ImgHeight, BackBufferDC, 0, 0, SRCCOPY);
}

// GDI Load
void Texture::GDILoadimage()
{		
	WCHAR path[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, path);
	// switch 를 통해서 경로를 반환 받아서 path에 적용 
	wcscat_s(path, L"\\Resources\\elf32.bmp");
	//MessageBoxW(hWnd_, path, L"실제 시도하는 경로", MB_OK);

	HBITMAP hImageBitmap = (HBITMAP)LoadImage(
		NULL,										// 인스턴스 핸들 (파일이므로 NULL)
		path,										// BMP 파일 경로
		IMAGE_BITMAP,								// 비트맵 로드
		bmpInfo.bmWidth,
		bmpInfo.bmHeight,							// 크기 자동 (원본 크기)
		LR_LOADFROMFILE								// 외부 파일에서 로드
	);

	GetObject(hImageBitmap, sizeof(BITMAP), &bmpInfo);
	if (!hImageBitmap)
		PrintLastErrorMessage();

	SelectObject(ImageDC, hImageBitmap);
	
}

//GDI plus Load
void Texture::GDIplusLoadImage(AnimState curAnim) // 가상 함수를 통해서 애니메이션 프레임의 객체 마다 다른 이미지를 부를 수 있게 해볼 수 있을까? 
{

	WCHAR path[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, path);
	// switch 를 통해서 경로를 반환 받아서 path에 적용 
	//wcscat_s(path, L"\\Resources\\elf32.bmp");
	//MessageBoxW(hWnd_, path, L"실제 시도하는 경로", MB_OK);

	//Image = new Gdiplus::Bitmap(AnimPath(curAnim));
	//ImageBitmap = new Gdiplus::Bitmap(L"..\\Resources\\elf32.png");

	if (Image->GetLastStatus() != Gdiplus::Ok) {
		MessageBox(NULL, L"이미지 로드 실패", L"오류", MB_ICONERROR);
	}
}






