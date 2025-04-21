#include "Animation.h"


Animation::Animation(HWND hWnd) : hWnd_(hWnd)
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

	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &GSI, nullptr);
	BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);
	
	//프레임 개수 만큼 초기화 
	AnimFrame.resize(AnimState::AnimCount);
}

Animation::~Animation()
{
	fclose(fp); 
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

void Animation::Intialize(AnimState cur_anim) // 현재의 애니메이션에 따른 이미지들을 벡터에 저장한다. 
{
	//현재 애니메이션 상태에 따라서 파일을 오픈 
	const WCHAR* path = SpritePath(cur_Anim);
	char* SpritePath; 

	WideCharToMultiByte(CP_ACP, 0, path, -1, SpritePath, sizeof(SpritePath), NULL, NULL);
	fp = fopen(SpritePath, "r");

	if (fp == NULL)
	{
		cout << "파일 열기 실패!" << endl;
		return;
	}

}
void Animation::Update()
{
	curDurtionTime += Time::GetDeltaTime();  // 프레임 실시간 재생 시간 

	Sprite& Spr = AnimFrame[cur_Anim][cur_Frame];

	if (!AnimFrame.empty()) {
		cur_Frame = (cur_Frame + 1) % AnimFrame.size();
	}
	else {
		cur_Frame = 0; // 또는 에러 로그 출력
	}

	int FrameCount;
	fscanf_s(fp, "%d", &FrameCount); //첫 줄에서 프레임 수 받기 


	char line[256];
	fgets(line, sizeof(line), fp); //개행 처리 
	// 개행 문자를 제거 
	int len = strlen(line);  //길이를 받아온다. 
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = 0;

	printf("개행 제거 후 : %s \n", line);

	// 토큰 ,(쉼표) 구분자 찾기 
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
		sscanf_s(token, "%d", &tokenVec[i]);	   // 숫자를 추출한다. 
		token = strtok_s(NULL, ", ", &context);    // 토큰 삭제 

	}


	fclose(fp);
}

void Animation::Render(int curAnim)
{	

	Sprite spr = AnimFrame[cur_Anim][cur_Frame];
	
	// 프레임 정보 불러오기 
	BackBufferGraphics->DrawImage(
		Image, Gdiplus::Rect(100, 100,
			//출력 위치 및 크기 
		(int)Spr.size.x,	    (int)Spr.size.y),
		(int)Spr.LeftTop.x,		(int)Spr.LeftTop.y,  
		(int)Spr.size.x,	    (int)Spr.size.y,
		Gdiplus::UnitPixel); 

}

void Animation::LoadIMg(int curAnim)
{
	WCHAR path[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, path);
	// switch 를 통해서 경로를 반환 받아서 path에 적용 
	wcscat_s(path, SpritePath(curAnim));
	//MessageBoxW(hWnd_, path, L"실제 시도하는 경로", MB_OK);

	Image = new Gdiplus::Bitmap(path);
	//ImageBitmap = new Gdiplus::Bitmap(L"..\\Resources\\elf32.png");

	if (Image->GetLastStatus() != Gdiplus::Ok) {
		MessageBox(NULL, L"이미지 로드 실패", L"오류", MB_ICONERROR);
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






