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
	AnimFrame.resize(AnimState::ANIMCOUNT);
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
	const wstring path = GetAnimPath(cur_Anim);
	string str = ConvertWStringToString(path);

	fp = fopen(str.c_str(), "r");

	if (fp == NULL)
	{
		cout << "���� ���� ����!" << endl;
		return;
	}

}
void Animation::Update()
{
	curDurtionTime += Time::GetDeltaTime();  // ������ �ǽð� ��� �ð� 
	FrameTime();

	auto& frames = AnimFrame[cur_Anim]; 

	if (!AnimFrame.empty()) {
		if (curDurtionTime >= frames[cur_Anim].DurctionTime)
		{
			cur_Frame = (cur_Frame + 1) % frames.size(); 
			curDurtionTime = 0.0f;
		}
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

	auto& frames = AnimFrame[cur_Anim];

	// ������ ���� �ҷ����� 
	BackBufferGraphics->DrawImage(
		Image, Gdiplus::Rect(100, 100,
		//��� ��ġ �� ũ�� 
		(int)Spr.size.x,	    (int)Spr.size.y),
		(int)Spr.LeftTop.x,		(int)Spr.LeftTop.y,
		(int)Spr.size.x,	    (int)Spr.size.y,
		Gdiplus::UnitPixel); 


}

string Animation::ConvertWStringToString(const wstring& wstr)
{
	if (wstr.empty()) return; 

	int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	string result(size, 0); 

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], size, NULL, NULL); 
	return result; 
}

void Animation::FrameTime()
{
	if (curDurtionTime >= Spr.DurctionTime)
	{
		cur_Frame = (cur_Frame + 1) % AnimFrame.size();
		curDurtionTime = 0.0f;
	}
}

Sprite Animation::CreateFrame(AnimState AnimName, Vector2 LeftTop, Vector2 size, Vector2 offset, float durctionTime)
{	 
	Sprite frame; 
	frame.LeftTop = LeftTop; 
	frame.size = size; 
	frame.offset = offset; 
	frame.DurctionTime = durctionTime; 

	for (int i = 0; i < ANIMCOUNT; ++i)
	{
		AnimFrame[AnimName].push_back(frame);
	}

	return frame; 
}






