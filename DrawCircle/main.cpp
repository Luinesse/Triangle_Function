#include <windows.h>
#include <math.h>
#include <iostream>
#include <vector>
#define PI 3.14
#define SPEED 5.0f

using namespace std;

int gDegree = 0;

class Bullet {
public:
    float x;
    float y;
    float vx, vy;
public:
    Bullet(float x, float y, float vx, float vy) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
    }
};

vector<shared_ptr<Bullet>> vBullets;

// ������ ȣ�������� ����
// degree * PI / 180
float deg2rad(float degree) {
    return (degree * PI / 180);
}

// ���ʼ��� ��Ÿ��ŭ ȸ���� ��, ������ ���� �´�� ������ ���ϴ� ���� ����
// �ݽð�� �� ��� ���ǰ�.
// x��ǥ�� cos��Ÿ = x / radius ���� �̿�.
// x = cos��Ÿ * radius.
// �� ���� ������ 0,0 �϶� �����̹Ƿ�, �츮�� ���� ��ǥ�� ������� �����ش�.
float xDest(float radian, float radius, float xStart) {
    return cos(radian) * radius + xStart;
}

// y��ǥ�� sin��Ÿ = y / radius ���� �̿�
// y = sin��Ÿ * radius
// �� ���� ������ 0,0 �϶� �����̹Ƿ�, �츮�� ���� ��ǥ�� ������� �����ش�.
float yDest(float radian, float radius, float yStart) {
    return sin(radian) * radius + yStart;
}

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// WinMain: ������ ���ø����̼��� ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    // ������ Ŭ���� ����
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;            // ������ �޽��� ó�� �Լ�
    wc.hInstance = hInstance;            // ���� �ν��Ͻ�
    wc.lpszClassName = CLASS_NAME;       // Ŭ���� �̸�
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH); // ��� ��� ����


    RegisterClass(&wc); // ������ Ŭ���� ���

    // ������ ����
    HWND hWnd = CreateWindowEx(
        0,                              // Ȯ�� ��Ÿ��
        CLASS_NAME,                     // ������ Ŭ���� �̸�
        L"�ﰢ�Լ��� �̿��� ���� �׸���",               // Ÿ��Ʋ �ٿ� ǥ�õ� �̸�
        WS_OVERLAPPEDWINDOW,           // ������ ��Ÿ��
        CW_USEDEFAULT, CW_USEDEFAULT,  // ��ġ
        800, 600,                       // ũ��
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow); // ������ ǥ��

    SetTimer(hWnd, 1, 33, NULL); // ȭ�� ������ ���� Ÿ�̸� ȣ��

    // �޽��� ����
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// �޽��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Ű���� �����Է��� ���� if - else if�� �ƴ�, if�θ� ����
    switch (message) {
    case WM_KEYDOWN:
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            // Ű �Է� �� ������ 5�� ����.
            // ���� ��Ÿ�� 360�� ���� �����Ƿ� ������ ����.
            gDegree -= 5;
            gDegree %= 360;

            // �ٽ� �׸��� ��û
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            // Ű �Է� �� ������ 5�� ����.
            // ���� ��Ÿ�� 360�� ���� �����Ƿ� ������ ����.
            gDegree += 5;
            gDegree %= 360;

            // �ٽ� �׸��� ��û
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            // ������ �������� ���� �Ѿ��� �߻�.
            // �Ѿ��� �߻������ cos, sin�� �ﰢ�� ������ ������ ���� �ӵ��� ���Ͽ� �߻�
            // �� �Ѿ��� ������ ����Ʈ �����Ϳ� ���ͷ� ����.
            float rad = deg2rad(gDegree);
            float startX = xDest(rad, 50, 400);
            float startY = yDest(rad, 50, 300);
            float vx = cos(rad) * SPEED; // velocity_x
            float vy = sin(rad) * SPEED; // velocity_y
            shared_ptr<Bullet> bullet = make_shared<Bullet>(startX, startY, vx, vy);
            vBullets.push_back(bullet);
        }
        return 0;

    case WM_TIMER:
        // �Ѿ��� ����(����)�� ����
        for (auto& it : vBullets) {
            it->x += it->vx;
            it->y += it->vy;
        }

        // �Ѿ��� ȭ������� �����ٸ� ���Ϳ��� �� ���� ����.
        // ����Ʈ �����ͷ� �����ϹǷ� ���Ϳ��� �����Ǹ� reference_count�� 0�̵Ǿ� ������.
        vBullets.erase(remove_if(vBullets.begin(), vBullets.end(), [](const shared_ptr<Bullet>& b) {
            return (b->x < 0 || b->x > 800 || b->y < 0 || b->y > 600);
            }), vBullets.end());

        InvalidateRect(hWnd, NULL, TRUE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // ������ �ִ� ������ �о������ ���� ȭ�� ��ü�� ������� �ٽ� ��ĥ
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(WHITE_BRUSH));

        // �������� ���� ��ǥ�� ���Ͽ� ������ �׸���.
        MoveToEx(hdc, 400, 300, NULL);
        LineTo(hdc, xDest(deg2rad(gDegree), 50, 400), yDest(deg2rad(gDegree), 50, 300));

        for (auto& b : vBullets) {
            Ellipse(hdc, static_cast<int>(b->x) - 3, static_cast<int>(b->y) - 3, static_cast<int>(b->x) + 3, static_cast<int>(b->y) + 3);
        }

        EndPaint(hWnd, &ps);

        return 0;
    }
    case WM_DESTROY: // �����찡 ���� ��
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
