#pragma once
#include <windows.h>


// #定義
struct colorRGB {
    int r, g, b;
    bool operator==(const colorRGB& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};



// #取得
colorRGB getColor(int x, int y) {
    colorRGB color = { -1, -1, -1 }; // 初期値: エラー時の値

    // デスクトップ全体のデバイスコンテキストを取得
    HDC hdc = GetDC(NULL); // NULLで画面全体を取得
    if (!hdc) {
        std::cerr << "Failed to get device context!" << std::endl;
        return color;
    }

    // 指定座標のピクセルの色を取得
    COLORREF pixelColor = GetPixel(hdc, x, y);
    if (pixelColor == CLR_INVALID) { // エラーの場合
        std::cerr << "Failed to get pixel color at (" << x << ", " << y << ")!" << std::endl;
        ReleaseDC(NULL, hdc);
        return color;
    }

    // 赤、緑、青の成分を構造体に格納
    color.r = GetRValue(pixelColor);
    color.g = GetGValue(pixelColor);
    color.b = GetBValue(pixelColor);

    // デバイスコンテキストを解放
    ReleaseDC(NULL, hdc);

    return color;
}



// #操作
void click(int x, int y) {
	SetCursorPos(x,y);
    INPUT inputs[2] = {0};
    inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
}