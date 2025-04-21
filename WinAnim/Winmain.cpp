#include "Winmain.h"


Winmain* Main;


// 콘솔 초기화
void InitConsole()
{
	AllocConsole(); // 콘솔을 생성한다. 
	FILE* fp; // 파일 포인터. 
	// 파일을 CONOUT$ 이름으로 읽기 모드로 연다. 
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"윈도우 메시지 콘솔 로그");
	printf("콘솔 로그 시작...\n\n");
	
	
}

void UninitConsole()
{
	// 표준 출력 스트림 닫기
	fclose(stdout);
	// 콘솔 해제
	FreeConsole();
}

// WIN32 API 에러 값에 대한 실제 메세지를 출력하는 함수
void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError(); // 변환해야 하는 오류 코드. 
	LPVOID lpMsgBuf; // 시스템 메시지 아이디 버퍼 라느 건가? 

	// 오류 코드, 시스템 메시지를 사람이 읽을 수 있는 문자열로 변환.
	// 시스템 메시지를 우리가 읽을 수 있도록 변환. 
	FormatMessageA(
		// 그릴 메시지 플래그 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //메시지 형식 플래그 
		NULL, // 메시지 원본 (NULL 은 시스템 메시지 사용 
		errorCode, // 변환할 오류코드
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 기본 언어
		(LPSTR)&lpMsgBuf,// 결과 메시지를 저장할 문자열 버퍼 
		0,				 // 사이즈 
		NULL);

	if (lpMsgBuf)
	{
		printf("오류 코드: %lu\n오류 메시지: %s\n", errorCode, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf); // 할당된 버퍼 해제
	}
	else
	{
		printf("오류 코드: %lu (메시지를 찾을 수 없음)\n", errorCode);
	}
}
// 윈도우 프로시저. 
//HWND hwnd = 윈도우 아이디 받기, UINT msg = 메시지,  
//WPARAM wParam = 키보드 입력 정보, LPARAM lParam = 마우스 입력 정보 
// 메인에서 전달 받은 메시지를 처리한다. 
// 그리기 메시지인지, 키보드 입력 메시지인지, 마우스 입력 메시지인지, 삭제 메시지인지.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)// 받은 메시지가 삭제 일 때, 프로그램 종료 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Winmain::ProgressInit()
{
	// 리소스 생성 
	texture = new Texture(g_hWnd);
	Anim = new Animation(g_hWnd);
	Anim->Intialize(cur_Anim); 

	// player 생성
	player = new Player();
	player->Init(g_hWnd, g_width, g_height);
	
	texture->SetAnimation(Anim);
}

void Winmain::ProgressUpdate()
{
	Main->ProgressInput();
	texture->TextureUpdate();
	Anim->Update(); 
}

void Winmain::ProgressRender()
{
	player->RectRender();
	texture->GDIplusRender();
	Anim->Render(cur_Anim); 
}

void Winmain::ProgressRelease()
{
	delete texture;
	delete player; 
	UninitConsole();  // 콘솔 출력 해제
	delete Main;
}
void Winmain::ProgressInput() // 키를 누르면 출력 
{
	if (Input::IsKeyDown(VK_NUMPAD1) & 0x8000)
	{
		if (cur_Anim == Run)
		{
			Anim->SetcurAnimState(Run); // 현재 애니메이션 상태를 Run 으로 설정 
			cur_Frame = 0; 
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame]; 	
		}

	}
	if (Input::IsKeyDown(VK_NUMPAD2) & 0x8000)
	{
		if (cur_Anim == attack)
		{
			Anim->SetcurAnimState(attack); // 현재 애니메이션 상태를 Run 으로 설정 
			cur_Frame = 0;
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame];
		}

	}
	if (Input::IsKeyDown(VK_NUMPAD3) & 0x8000)
	{
		if (cur_Anim == jumpAtt)
		{
			Anim->SetcurAnimState(jumpAtt); // 현재 애니메이션 상태를 Run 으로 설정 
			cur_Frame = 0;
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame];
		}

	}

}

// 현재 윈도우 아이디와 이전 윈도우 아이디, 문자(char) 타입의 그릴 선, 
// 그리고 몇개를 그릴 지 정수(int) 형 의 파라메터를 받는다. 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	InitConsole();  // 콘솔 출력 초기화
	Time::InitTime(); // 콘솔 시작 시간 초기화 
	Main = new Winmain(); 

	char szPath[MAX_PATH] = { 0, };
	// 버퍼 길이와 버퍼 경로를 받아서 현재 작업 디렉토리를 불러온다. 
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;						// 윈도우 프로시저를 받아온다. 
	wc.hInstance = hInstance;						// 만들어진 프로세스를 받아온다. 
	wc.lpszClassName = g_szClassName;				// 아이디의 이름. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 브러쉬로 변환한 Window 창의 백그라운드(배경) 을 받아온다. 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// 기본 아이콘 모양
	RegisterClass(&wc);

	Main->Resolution(hInstance, nCmdShow, g_width, g_height);

	Time::InitTime(); 
	Main->ProgressInit();

	MSG msg;
	while (Main->PeekMessages(msg))
	{
		
		Time::UpdateTime();
		Input::Update(); 
		Main->ProgressUpdate();
		Main->ProgressRender();
		
	}

	
	Main->ProgressRelease();
	//////////////////////////////////////////////////////////////////////////
	
	
	return (int)msg.wParam;
}



bool Winmain::PeekMessages(MSG& msg)
{
	//PeekMassage =>  메시지를 훔쳐본다. 메시지의 존재 여부에 따라 반환 이 가능하다. true일 시 무조건 메시지가 존재.
		// 조건문이 사용 가능하다. 
		// 마지막 인자인 UNIT wRemoveMsg => 메시지 큐에서 확인한 메시지가 존재할 시에 삭제 여부 결정 파라메터.
		// (PM_REMOVE = 삭제) 
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false; 
		
		TranslateMessage(&msg);		// 메시지를 받아서 키보드 메시지를 문자 메시지로 변환. -> 키보드 입력받은 메시지를 문자 메시지로 변환??
		DispatchMessage(&msg);		// 메시지를 윈도우 프로시저 (WndProc) 으로 전달 
	}

	return true; 
}

void Winmain::Resolution(HINSTANCE hInstance, int nCmdShow, int g_width, int g_height)
{
	{
		// 원하는 크기가 조정되어 리턴
		RECT rcClient = { 0, 0, g_width, g_height };  // 새로운 콘솔창 해상도 크기 
		AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);  // 그릴 Rect 모양을 받아서, 
		//그릴 스타일을 정하고, 메뉴가 있느냐 를 체크한다.

	//생성
	// 윈도우 아이디 타입으로 콘솔창을 생성 한다. 
		HWND hwnd = CreateWindow(
			g_szClassName,		 // 윈도우 클래스 이름 
			g_title,			 // 윈도우 창 제목 
			WS_OVERLAPPEDWINDOW, // 윈도우 스타일 
			CW_USEDEFAULT,		 // 윈도우 가로(X) 길이 
			CW_USEDEFAULT,		 // 윈도우 세로(Y) 길이 
			//클라이언트 창 크기 중앙 구하기 
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top,
			NULL, NULL, hInstance, NULL);

		g_hWnd = hwnd;
		// 윈도우 창에 보여주기. 
		// 윈도우 창 표시 상태를 변경한다. (마지막 인수 : SW_ 상수) 
		// 창이 이전에도 보였다면 true, 이전에는 숨겨져 있었다면 fasle.반환 
		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);

	}
}

Winmain::Winmain()
{

}

Winmain::~Winmain()
{
	
	//delete Main; 
}
