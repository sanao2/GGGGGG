#include "Texture.h"



Texture::Texture(HWND hWnd) : hWnd_(hWnd)
{
	RECT rc;
	GetClientRect(hWnd_, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	////////Renderer::Initialize
	// �´� DC�� ������ ��, ������ ���� 
	FrontBufferDC = GetDC(hWnd_); //������ Ŭ���̾�Ʈ ������ DeviceContext���
	BackBufferDC = CreateCompatibleDC(FrontBufferDC); // ȣȯ�Ǵ� DeviceContext ����

	// �޸� ��������
	// HDC => DC�� �ڵ� ���̵��� ���ο� ���� ���̸� �޾ƿ� ������ ����. 
	HBackBufferBitmap = CreateCompatibleBitmap(FrontBufferDC, g_width, g_height);
	SelectObject(BackBufferDC, HBackBufferBitmap); // g_BackBufferDC�� �޸𸮿��� ����

	// DeviceContext ���� �� HBitmap ����
	ImageDC = CreateCompatibleDC(FrontBufferDC);	// ȣȯ�Ǵ� DeviceContext ����

	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
	BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);
}

Texture::~Texture()
{
	// �� �� �͵� ����. 
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

void Texture::TextureUpdate() //���߿� �ִϸ��̼� ���¿� ���� ����ؼ� �̹����� �ٲ��� �ɼ� (�ݺ���) 
{
	if (Image == nullptr)
	  GDIplusLoadImage(Anim->GetcurAnimState());
}

void Texture::GDIRender()
{	
	// Renderer::BeginDraw()
		// �տ� �׷ȴ� ���� �����. 
		// DC �ĸ� ������ ���ʿ� �׷����´�. // ���� ���� �׸��� ����. 
	PatBlt(BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);

	// Render()
	// ��ü �̹��� �״�� �����ϱ�
	BitBlt(BackBufferDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, ImageDC, 0, 0, SRCCOPY);

	//Ư�� �÷�(�÷�Ű)�� �����ϰ� �׸���
	TransparentBlt(BackBufferDC, bmpInfo.bmWidth, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
		ImageDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
		RGB(255, 255, 255));	// Msimg32.lib ��ũ �ʿ�

	// Renderer::EndDraw()
	// DC ���� ���۷� �����ͼ� �׸���. 
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
	// switch �� ���ؼ� ��θ� ��ȯ �޾Ƽ� path�� ���� 
	wcscat_s(path, L"\\Resources\\elf32.bmp");
	//MessageBoxW(hWnd_, path, L"���� �õ��ϴ� ���", MB_OK);

	HBITMAP hImageBitmap = (HBITMAP)LoadImage(
		NULL,										// �ν��Ͻ� �ڵ� (�����̹Ƿ� NULL)
		path,										// BMP ���� ���
		IMAGE_BITMAP,								// ��Ʈ�� �ε�
		bmpInfo.bmWidth,
		bmpInfo.bmHeight,							// ũ�� �ڵ� (���� ũ��)
		LR_LOADFROMFILE								// �ܺ� ���Ͽ��� �ε�
	);

	GetObject(hImageBitmap, sizeof(BITMAP), &bmpInfo);
	if (!hImageBitmap)
		PrintLastErrorMessage();

	SelectObject(ImageDC, hImageBitmap);
	
}

//GDI plus Load
void Texture::GDIplusLoadImage(AnimState curAnim) // ���� �Լ��� ���ؼ� �ִϸ��̼� �������� ��ü ���� �ٸ� �̹����� �θ� �� �ְ� �غ� �� ������? 
{

	WCHAR path[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, path);
	// switch �� ���ؼ� ��θ� ��ȯ �޾Ƽ� path�� ���� 
	//wcscat_s(path, L"\\Resources\\elf32.bmp");
	//MessageBoxW(hWnd_, path, L"���� �õ��ϴ� ���", MB_OK);

	//Image = new Gdiplus::Bitmap(AnimPath(curAnim));
	//ImageBitmap = new Gdiplus::Bitmap(L"..\\Resources\\elf32.png");

	if (Image->GetLastStatus() != Gdiplus::Ok) {
		MessageBox(NULL, L"�̹��� �ε� ����", L"����", MB_ICONERROR);
	}
}






