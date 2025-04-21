#include "Winmain.h"


Winmain* Main;


// �ܼ� �ʱ�ȭ
void InitConsole()
{
	AllocConsole(); // �ܼ��� �����Ѵ�. 
	FILE* fp; // ���� ������. 
	// ������ CONOUT$ �̸����� �б� ���� ����. 
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"������ �޽��� �ܼ� �α�");
	printf("�ܼ� �α� ����...\n\n");
	
	
}

void UninitConsole()
{
	// ǥ�� ��� ��Ʈ�� �ݱ�
	fclose(stdout);
	// �ܼ� ����
	FreeConsole();
}

// WIN32 API ���� ���� ���� ���� �޼����� ����ϴ� �Լ�
void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError(); // ��ȯ�ؾ� �ϴ� ���� �ڵ�. 
	LPVOID lpMsgBuf; // �ý��� �޽��� ���̵� ���� ��� �ǰ�? 

	// ���� �ڵ�, �ý��� �޽����� ����� ���� �� �ִ� ���ڿ��� ��ȯ.
	// �ý��� �޽����� �츮�� ���� �� �ֵ��� ��ȯ. 
	FormatMessageA(
		// �׸� �޽��� �÷��� 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //�޽��� ���� �÷��� 
		NULL, // �޽��� ���� (NULL �� �ý��� �޽��� ��� 
		errorCode, // ��ȯ�� �����ڵ�
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �⺻ ���
		(LPSTR)&lpMsgBuf,// ��� �޽����� ������ ���ڿ� ���� 
		0,				 // ������ 
		NULL);

	if (lpMsgBuf)
	{
		printf("���� �ڵ�: %lu\n���� �޽���: %s\n", errorCode, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf); // �Ҵ�� ���� ����
	}
	else
	{
		printf("���� �ڵ�: %lu (�޽����� ã�� �� ����)\n", errorCode);
	}
}
// ������ ���ν���. 
//HWND hwnd = ������ ���̵� �ޱ�, UINT msg = �޽���,  
//WPARAM wParam = Ű���� �Է� ����, LPARAM lParam = ���콺 �Է� ���� 
// ���ο��� ���� ���� �޽����� ó���Ѵ�. 
// �׸��� �޽�������, Ű���� �Է� �޽�������, ���콺 �Է� �޽�������, ���� �޽�������.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)// ���� �޽����� ���� �� ��, ���α׷� ���� 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Winmain::ProgressInit()
{
	// ���ҽ� ���� 
	texture = new Texture(g_hWnd);
	Anim = new Animation(g_hWnd);
	Anim->Intialize(cur_Anim); 

	// player ����
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
	UninitConsole();  // �ܼ� ��� ����
	delete Main;
}
void Winmain::ProgressInput() // Ű�� ������ ��� 
{
	if (Input::IsKeyDown(VK_NUMPAD1) & 0x8000)
	{
		if (cur_Anim == Run)
		{
			Anim->SetcurAnimState(Run); // ���� �ִϸ��̼� ���¸� Run ���� ���� 
			cur_Frame = 0; 
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame]; 	
		}

	}
	if (Input::IsKeyDown(VK_NUMPAD2) & 0x8000)
	{
		if (cur_Anim == attack)
		{
			Anim->SetcurAnimState(attack); // ���� �ִϸ��̼� ���¸� Run ���� ���� 
			cur_Frame = 0;
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame];
		}

	}
	if (Input::IsKeyDown(VK_NUMPAD3) & 0x8000)
	{
		if (cur_Anim == jumpAtt)
		{
			Anim->SetcurAnimState(jumpAtt); // ���� �ִϸ��̼� ���¸� Run ���� ���� 
			cur_Frame = 0;
			Spr = (Sprite)AnimFrame[cur_Anim][cur_Frame];
		}

	}

}

// ���� ������ ���̵�� ���� ������ ���̵�, ����(char) Ÿ���� �׸� ��, 
// �׸��� ��� �׸� �� ����(int) �� �� �Ķ���͸� �޴´�. 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	InitConsole();  // �ܼ� ��� �ʱ�ȭ
	Time::InitTime(); // �ܼ� ���� �ð� �ʱ�ȭ 
	Main = new Winmain(); 

	char szPath[MAX_PATH] = { 0, };
	// ���� ���̿� ���� ��θ� �޾Ƽ� ���� �۾� ���丮�� �ҷ��´�. 
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;						// ������ ���ν����� �޾ƿ´�. 
	wc.hInstance = hInstance;						// ������� ���μ����� �޾ƿ´�. 
	wc.lpszClassName = g_szClassName;				// ���̵��� �̸�. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // �귯���� ��ȯ�� Window â�� ��׶���(���) �� �޾ƿ´�. 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// �⺻ Ŀ�� ���
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// �⺻ ������ ���
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
	//PeekMassage =>  �޽����� ���ĺ���. �޽����� ���� ���ο� ���� ��ȯ �� �����ϴ�. true�� �� ������ �޽����� ����.
		// ���ǹ��� ��� �����ϴ�. 
		// ������ ������ UNIT wRemoveMsg => �޽��� ť���� Ȯ���� �޽����� ������ �ÿ� ���� ���� ���� �Ķ����.
		// (PM_REMOVE = ����) 
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false; 
		
		TranslateMessage(&msg);		// �޽����� �޾Ƽ� Ű���� �޽����� ���� �޽����� ��ȯ. -> Ű���� �Է¹��� �޽����� ���� �޽����� ��ȯ??
		DispatchMessage(&msg);		// �޽����� ������ ���ν��� (WndProc) ���� ���� 
	}

	return true; 
}

void Winmain::Resolution(HINSTANCE hInstance, int nCmdShow, int g_width, int g_height)
{
	{
		// ���ϴ� ũ�Ⱑ �����Ǿ� ����
		RECT rcClient = { 0, 0, g_width, g_height };  // ���ο� �ܼ�â �ػ� ũ�� 
		AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);  // �׸� Rect ����� �޾Ƽ�, 
		//�׸� ��Ÿ���� ���ϰ�, �޴��� �ִ��� �� üũ�Ѵ�.

	//����
	// ������ ���̵� Ÿ������ �ܼ�â�� ���� �Ѵ�. 
		HWND hwnd = CreateWindow(
			g_szClassName,		 // ������ Ŭ���� �̸� 
			g_title,			 // ������ â ���� 
			WS_OVERLAPPEDWINDOW, // ������ ��Ÿ�� 
			CW_USEDEFAULT,		 // ������ ����(X) ���� 
			CW_USEDEFAULT,		 // ������ ����(Y) ���� 
			//Ŭ���̾�Ʈ â ũ�� �߾� ���ϱ� 
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top,
			NULL, NULL, hInstance, NULL);

		g_hWnd = hwnd;
		// ������ â�� �����ֱ�. 
		// ������ â ǥ�� ���¸� �����Ѵ�. (������ �μ� : SW_ ���) 
		// â�� �������� �����ٸ� true, �������� ������ �־��ٸ� fasle.��ȯ 
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
