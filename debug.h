#pragma once
#include <iostream>
#include <thread>
#include <chrono>

/**
 * @file debug.h
 * @brief デバッグ・パフォーマンス計測ユーティリティ
 *
 * スレッド追跡用のシーケンスログ出力と、高精度な実行時間計測クラスを提供します。
 */

/// @brief シーケンスログのカウンタ（グローバル）
int SL = 0;

/**
 * @brief シーケンスログを出力する
 *
 * 呼び出しごとにカウンタをインクリメントしてスレッド ID と共に出力します。
 * custom 引数を指定した場合はカスタム値のみ出力します。
 *
 * @param custom 任意のカスタムポイント値。デフォルト値(-100000)のときは自動カウントモード
 * @note 出力例（自動）: `SL::0x12345678::3`
 * @note 出力例（カスタム）: `getSL()::0x12345678::CustomPoint: 42`
 */
void getSL(int custom = -100000) {
	if (custom != -100000) {std::cout << "getSL()::" << std::this_thread::get_id() << "::CustomPoint: " << custom << "\n";return;}
	SL++;
	std::cout << "SL::" << std::this_thread::get_id() << "::" << SL << "\n";
}

/**
 * @brief 実行時間計測クラス
 *
 * インスタンス生成時に計測開始し、end() 呼び出し時にミリ秒単位で経過時間を出力します。
 *
 * @code
 * checkTime ct;
 * // ... 計測したい処理 ...
 * ct.end(); // "CheckTime: 123" のように出力
 * @endcode
 */
class checkTime {
public:
    /**
     * @brief コンストラクタ。計測を開始する
     */
    checkTime() {
		start = std::chrono::high_resolution_clock::now();
	}

    /**
     * @brief 計測を終了し、経過時間をミリ秒単位で標準出力に表示する
     */
    void end() {
		std::chrono::time_point<std::chrono::high_resolution_clock> e = std::chrono::high_resolution_clock::now();
		std::chrono::duration<long long, std::milli> duration = std::chrono::duration_cast<std::chrono::milliseconds>(e - start);
		std::cout << "CheckTime: " << duration.count() << "\n";
	}
private:
	/// @brief 計測開始時刻
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
/*
struct getInfo {
	getInfo(int flags) {
	enum LocationFlags {
		FileName = 0b0001,
		FunctionName = 0b0010,
		Line = 0b0100,
		Column = 0b1000
	};
}*/
