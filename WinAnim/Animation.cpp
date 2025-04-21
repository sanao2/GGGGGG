#include "Animation.h"


Animation::Animation(HWND hWnd) : hWnd_(hWnd)
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

	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &GSI, nullptr);
	BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);
	
	//������ ���� ��ŭ �ʱ�ȭ 
	AnimFrame.resize(AnimState::AnimCount);
}

Animation::~Animation()
{
	fclose(fp); 
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

void Animation::Intialize(AnimState cur_anim) // ������ �ִϸ��̼ǿ� ���� �̹������� ���Ϳ� �����Ѵ�. 
{
	//���� �ִϸ��̼� ���¿� ���� ������ ���� 
	const WCHAR* path = SpritePath(cur_Anim);
	char* SpritePath; 

	WideCharToMultiByte(CP_ACP, 0, path, -1, SpritePath, sizeof(SpritePath), NULL, NULL);
	fp = fopen(SpritePath, "r");

	if (fp == NULL)
	{
		cout << "���� ���� ����!" << endl;
		return;
	}

}
void Animation::Update()
{
	curDurtionTime += Time::GetDeltaTime();  // ������ �ǽð� ��� �ð� 

	Sprite& Spr = AnimFrame[cur_Anim][cur_Frame];

	if (!AnimFrame.empty()) {
		cur_Frame = (cur_Frame + 1) % AnimFrame.size();
	}
	else {
		cur_Frame = 0; // �Ǵ� ���� �α� ���
	}

	int FrameCount;
	fscanf_s(fp, "%d", &FrameCount); //ù �ٿ��� ������ �� �ޱ� 


	char line[256];
	fgets(line, sizeof(line), fp); //���� ó�� 
	// ���� ���ڸ� ���� 
	int len = strlen(line);  //���̸� �޾ƿ´�. 
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = 0;

	printf("���� ���� �� : %s \n", line);

	// ��ū ,(��ǥ) ������ ã�� 
	char* context = nullptr;
	char* token = strtok_s(line, ", ", &context);
	int tokenVec[6] = {  };

	for (int i = 0; i < 6 && token != NULL; ++i)
	{
		if (token == NULL)
		{
			cout << "token is Noting!" << endl;
			return;
		}
		sscanf_s(token, "%d", &tokenVec[i]);	   // ���ڸ� �����Ѵ�. 
		token = strtok_s(NULL, ", ", &context);    // ��ū ���� 

	}


	fclose(fp);
}

void Animation::Render(int curAnim)
{	

	Sprite spr = AnimFrame[cur_Anim][cur_Frame];
	
	// ������ ���� �ҷ����� 
	BackBufferGraphics->DrawImage(
		Image, Gdiplus::Rect(100, 100,
			//��� ��ġ �� ũ�� 
		(int)Spr.size.x,	    (int)Spr.size.y),
		(int)Spr.LeftTop.x,		(int)Spr.LeftTop.y,  
		(int)Spr.size.x,	    (int)Spr.size.y,
		Gdiplus::UnitPixel); 

}

void Animation::LoadIMg(int curAnim)
{
	WCHAR path[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, path);
	// switch �� ���ؼ� ��θ� ��ȯ �޾Ƽ� path�� ���� 
	wcscat_s(path, SpritePath(curAnim));
	//MessageBoxW(hWnd_, path, L"���� �õ��ϴ� ���", MB_OK);

	Image = new Gdiplus::Bitmap(path);
	//ImageBitmap = new Gdiplus::Bitmap(L"..\\Resources\\elf32.png");

	if (Image->GetLastStatus() != Gdiplus::Ok) {
		MessageBox(NULL, L"�̹��� �ε� ����", L"����", MB_ICONERROR);
	}
}

void Animation::FrameTime()
{

	if (curDurtionTime >= Spr.DurctionTime)
	{
		cur_Frame = (cur_Frame + 1) % AnimFrame.size();
		curDurtionTime = 0.0f;
	}
}

Sprite Animation::CreateFrame(AnimState AnimName, Vector2 LeftTop, Vector2 RightBottom, Vector2 offset, float durctionTime)
{	 
	for (int i = 0; i < 10; ++i)
	{
		Spr.LeftTop =		LeftTop;
		Spr.size =			RightBottom;
		Spr.offset =		offset;
		Spr.DurctionTime =  durctionTime;

		AnimFrame[cur_Anim].push_back(Spr);
	}
	return Spr; 
}






