#pragma once
#include <windows.h>

/**
 * @file winMacro.h
 * @brief Windows マウス・画面操作ユーティリティ
 *
 * Windows API を使用した画面上の任意座標のピクセル色取得と
 * マウスクリック操作を提供します。
 */

/**
 * @namespace wmc
 * @brief Windows マクロ操作用名前空間
 */
namespace wmc {
	/**
	 * @struct clRGB
	 * @brief RGB カラー値を保持する構造体
	 */
	struct clRGB {
		/**
		 * @brief コンストラクタ
		 * @param red   赤成分 (0〜255)
		 * @param green 緑成分 (0〜255)
		 * @param blue  青成分 (0〜255)
		 */
		clRGB(int red, int green, int blue) : r(red), g(green), b(blue) {}
		int r; ///< 赤成分
		int g; ///< 緑成分
		int b; ///< 青成分

		/**
		 * @brief 等値比較演算子
		 * @param other 比較対象の clRGB 構造体
		 * @return RGB 全成分が等しい場合 true
		 */
		bool operator==(const wmc::clRGB& other) const {
			return r == other.r && g == other.g && b == other.b;
		}
	};

	/**
	 * @brief 指定した画面座標のピクセル色を取得する
	 * @param x 取得したいピクセルの X 座標（スクリーン座標）
	 * @param y 取得したいピクセルの Y 座標（スクリーン座標）
	 * @return 取得した RGB 値。取得失敗時は {-1, -1, -1}
	 */
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
	 * @brief 指定した画面座標でマウス左ボタンのクリックをシミュレートする
	 * @param x クリックする X 座標（スクリーン座標）
	 * @param y クリックする Y 座標（スクリーン座標）
	 */
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
