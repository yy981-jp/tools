/**
 * @file winMacro.h
 * @brief Windows API を使用した低レベル操作機能を提供
 * @details 色取得、マウス操作、RGB構造体など
 * @author yy981
 * @version 1.0
 * @note Windows専用
 */
#pragma once
#include <windows.h>


/**
 * @namespace wmc
 * @brief Windows マクロ・操作関数の名前空間
 */
namespace wmc {
	/**
	 * @struct clRGB
	 * @brief RGB色を表す構造体
	 * @details 赤、緑、青の各成分を0-255の値で表現
	 */
	struct clRGB {
		clRGB(int red, int green, int blue) : r(red), g(green), b(blue) {}
		int r, g, b; ///< 赤、緑、青の成分 (0-255)
		
		/**
		 * @brief 等値比較
		 * @param other 比較対象
		 * @return r, g, b がすべて等しい場合true
		 */
		bool operator==(const wmc::clRGB& other) const {
			return r == other.r && g == other.g && b == other.b;
		}
	};



	/**
	 * @brief 指定座標のスクリーン上のピクセル色を取得
	 * @param x X座標
	 * @param y Y座標
	 * @return 該当位置のRGB色、取得失敗時は{-1, -1, -1}
	 * @note GetPixel, GetDC を使用
	 */
	// #取得
	wmc::clRGB getColor(int x, int y) {
		wmc::clRGB color = { -1, -1, -1 }; // 初期値: エラー時の値

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



	/**
	 * @brief マウスをクリック
	 * @param x クリック位置のX座標
	 * @param y クリック位置のY座標
	 * @details
	 * マウスカーソルを指定位置に移動し、左ボタンをクリック(押下→解放)
	 * @note SetCursorPos, SendInput を使用
	 */
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
}