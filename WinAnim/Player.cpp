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

	// ���ϴ� ũ�Ⱑ �����Ǿ� ����
	RECT rcClient = { 0, 0, g_width, g_height };  // ���ο� �ܼ�â �ػ� ũ�� 
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);  // �׸� Rect ����� �޾Ƽ�, 
	//�׸� ��Ÿ���� ���ϰ�, �޴��� �ִ��� �� üũ�Ѵ�.


	return S_OK;
}
void Player::RectRender()
{
	Rectangle(drawDC, 10, 10, 110, 110);
}