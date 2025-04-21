#include "Player.h"



Player::Player() : m_hWnd(0), drawDC(0), playerPos{0,0}, playerScale{100,100}
{
	
}

Player::~Player()
{
	ReleaseDC(g_hWnd, drawDC);
}

int Player::Init(HWND hWnd, int Rewidth, int Reheight )
{
	m_hWnd = hWnd;
	drawDC = GetDC(m_hWnd);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, g_width, g_height };  // 새로운 콘솔창 해상도 크기 
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);  // 그릴 Rect 모양을 받아서, 
	//그릴 스타일을 정하고, 메뉴가 있느냐 를 체크한다.


	return S_OK;
}
void Player::RectRender()
{
	Rectangle(drawDC, 10, 10, 110, 110);
}