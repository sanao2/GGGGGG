#include "Time.h"
#include <windows.h>

namespace Time
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	LARGE_INTEGER intialCounter;
	float deltaTime;

	void InitTime()
	{
		QueryPerformanceFrequency(&frequency);	// 고성능 타이머가 1초 동안 증가시킬수 있는 TickCount 값
		QueryPerformanceCounter(&intialCounter);  // 초기 TickCount 값
		prevCounter = intialCounter;
	}

	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter); // 현재 TickCount 값
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);  // 카운터차이를 시간 초단위로 변환

		prevCounter = currentCounter;
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}
	float GetTotalTime()
	{
		return static_cast<float>(currentCounter.QuadPart - intialCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);  // 카운터차이를 시간 초단위로 변환
	}
}