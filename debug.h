/**
 * @file debug.h
 * @brief デバッグ出力とパフォーマンス測定のユーティリティを提供
 * @details スレッドID付きのデバッグ出力と処理時間の計測機能
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <iostream>
#include <thread>
#include <chrono>


extern int SL; ///< デバッグカウンター

/**
 * @brief デバッグメッセージを出力
 * @param custom カスタムポイント値（指定時はこの値を出力）
 * @details
 * - custom == -100000 の場合: SLをインクリメントして出力
 * - custom != -100000 の場合: customの値を出力（SLはインクリメントしない）
 * 常にスレッドIDを付加して出力します
 */
void getSL(int custom = -100000) {
	if (custom != -100000) {std::cout << "getSL()::" << std::this_thread::get_id() << "::CustomPoint: " << custom << "\n";return;}
	SL++;
	std::cout << "SL::" << std::this_thread::get_id() << "::" << SL << "\n";
}

/**
 * @class checkTime
 * @brief 処理時間を計測するRAIIクラス
 * 
 * @details
 * コンストラクタで計測を開始し、end()メソッドで終了してミリ秒単位で出力
 * 高分解能クロック(std::chrono::high_resolution_clock)を使用
 */
class checkTime {
public:
    /**
     * @brief コンストラクタ - 計測開始
     * @details 現在時刻を記録
     */
    checkTime() {
		start = std::chrono::high_resolution_clock::now();
	}

    /**
     * @brief 計測終了 - 経過時間をミリ秒単位で出力
     * @details "CheckTime: XXX\n"形式で標準出力に出力
     */
    void end() {
		std::chrono::time_point<std::chrono::high_resolution_clock> e = std::chrono::high_resolution_clock::now();
		std::chrono::duration<long long, std::milli> duration = std::chrono::duration_cast<std::chrono::milliseconds>(e - start);
		std::cout << "CheckTime: " << duration.count() << "\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start; ///< 計測開始時刻
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