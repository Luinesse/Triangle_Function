#include <windows.h>
#include <math.h>
#include <iostream>
#define PI 3.14

using namespace std;

int gDegree = 0;

// 각도를 호도법으로 변경
// degree * PI / 180
float deg2rad(float degree) {
    return (degree * PI / 180);
}

// 시초선이 세타만큼 회전할 때, 동경이 원과 맞닿는 지점을 구하는 공식 응용
// 반시계로 갈 경우 양의각.
// x좌표는 cos세타 = x / radius 임을 이용.
// x = cos세타 * radius.
// 위 식은 원점이 0,0 일때 기준이므로, 우리는 원점 좌표를 결과값에 더해준다.
float xDest(float radian, float radius, float xStart) {
    return cos(radian) * radius + xStart;
}

// y좌표는 sin세타 = y / radius 임을 이용
// y = sin세타 * radius
// 위 식은 원점이 0,0 일때 기준이므로, 우리는 원점 좌표를 결과값에 더해준다.
float yDest(float radian, float radius, float yStart) {
    return sin(radian) * radius + yStart;
}

// 윈도우 프로시저 함수 선언
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// WinMain: 윈도우 애플리케이션의 시작점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    // 윈도우 클래스 정의
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;            // 윈도우 메시지 처리 함수
    wc.hInstance = hInstance;            // 현재 인스턴스
    wc.lpszClassName = CLASS_NAME;       // 클래스 이름
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH); // 배경 흰색 설정
    

    RegisterClass(&wc); // 윈도우 클래스 등록

    // 윈도우 생성
    HWND hWnd = CreateWindowEx(
        0,                              // 확장 스타일
        CLASS_NAME,                     // 윈도우 클래스 이름
        L"삼각함수를 이용한 포신 그리기",               // 타이틀 바에 표시될 이름
        WS_OVERLAPPEDWINDOW,           // 윈도우 스타일
        CW_USEDEFAULT, CW_USEDEFAULT,  // 위치
        800, 600,                       // 크기
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow); // 윈도우 표시

    // 메시지 루프
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN:
        if (wParam == VK_UP) {
            // 키 입력 시 각도를 5도 수정.
            // 원의 세타는 360을 넘지 않으므로 나머지 연산.
            gDegree-=5;
            gDegree %= 360;

            // 다시 그리기 요청
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_DOWN) {
            // 키 입력 시 각도를 5도 수정.
            // 원의 세타는 360을 넘지 않으므로 나머지 연산.
            gDegree+=5;
            gDegree %= 360;

            // 다시 그리기 요청
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 이전에 있던 내용을 밀어버리기 위해 화면 전체를 흰색으로 다시 색칠
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(WHITE_BRUSH));

        // 시작점과 끝점 좌표를 구하여 직선을 그리기.
        MoveToEx(hdc, 400, 300, NULL);
        LineTo(hdc, xDest(deg2rad(gDegree), 50, 400), yDest(deg2rad(gDegree), 50, 300));

        EndPaint(hWnd, &ps);

        return 0;
    }
    case WM_DESTROY: // 윈도우가 닫힐 때
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
