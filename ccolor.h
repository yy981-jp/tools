#pragma once
#include <windows.h>

/**
 * @file ccolor.h
 * @brief Windows コンソール文字色制御ユーティリティ
 *
 * Windows API を使用してコンソール出力の文字色・背景色を設定するための
 * シンプルなユーティリティ関数を提供します。
 */

/**
 * @namespace cc
 * @brief コンソールカラー操作用名前空間
 */
namespace cc {
	/**
	 * @brief コンソールのテキスト属性（文字色・背景色）を設定する
	 * @param flag 設定するカラーフラグ（例: FOREGROUND_RED | FOREGROUND_GREEN）
	 * @note Windows API の SetConsoleTextAttribute に渡す値を直接指定します
	 */
	inline void set(DWORD flag) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flag);
	}

	/**
	 * @brief コンソールのテキスト属性をデフォルト（白色）にリセットする
	 * @note FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE（白）に戻します
	 */
	inline void reset() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}
