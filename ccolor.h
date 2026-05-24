/**
 * @file ccolor.h
 * @brief コンソール(Windows)のテキスト色を操作する機能を提供
 * @details Windows APIを使用してコンソール出力のテキスト色を制御
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <windows.h>


/**
 * @namespace cc
 * @brief コンソール色操作関数の名前空間
 */
namespace cc {
	/**
	 * @brief コンソールテキスト属性を設定
	 * @param flag FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE などの色フラグ
	 * @details 複数のフラグをビット演算で組み合わせることで色を指定可能
	 * @see SetConsoleTextAttribute, GetStdHandle
	 */
	inline void set(DWORD flag) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flag);
	}

	/**
	 * @brief コンソールテキスト色をデフォルト(白)にリセット
	 * @details FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUEで白色を設定
	 */
	inline void reset() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}