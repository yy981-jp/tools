/**
 * @file thread.h
 * @brief 定期実行機能を持つデタッチスレッドクラスを提供
 * @details std::threadをラッピングし、定期的に関数を実行するスレッドを簡単に管理
 * @author yy981
 * @version 1.0
 * @see time.h (tu型の定義が必要)
 */
#pragma once
#include <thread>
#include <tuple>
#include <utility>
#include <atomic>
#include <yy981/time.h>


/**
 * @class dthread
 * @brief 定期実行またはループ実行するデタッチスレッド
 * 
 * @details
 * - コンストラクタで自動的にスレッドを開始
 * - デストラクタで自動的に停止
 * - 定期実行と無限ループ実行の2つのモードをサポート
 */
class dthread {
public:
	/**
	 * @brief コンストラクタ（定期実行モード）
	 * @tparam Func 実行する関数の型
	 * @tparam Args 関数の引数型
	 * @param unit 時間単位（tu型）
	 * @param value 待機時間
	 * @param func 定期実行する関数
	 * @param args 関数に渡す引数
	 * @details
	 * funcを定期的（unit*value間隔）で実行するスレッドを開始
	 * 前回のスレッドが実行中の場合は先に停止
	 */
	template <typename Func, typename... Args>
	dthread(tu unit, double value, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, unit, value, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
				sleepc(unit,value);
			}
		}).detach();
	}

	/**
	 * @brief コンストラクタ（無限ループモード）
	 * @tparam Func 実行する関数の型
	 * @tparam Args 関数の引数型
	 * @param noSleep true: スリープなし、false: 無視（互換性用）
	 * @param func 繰り返し実行する関数
	 * @param args 関数に渡す引数
	 * @details
	 * funcを連続実行するスレッドを開始（スリープなし）
	 * スリープが必要な場合は別途sleepc()を呼び出してください
	 */
	template <typename Func, typename... Args>
	dthread(bool noSleep, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
			}
		}).detach();
	}

	/**
	 * @brief デストラクタ - スレッド停止
	 */
	~dthread() {running=false;}
	
	/**
	 * @brief スレッドを明示的に停止
	 * @details runningフラグをfalseに設定
	 */
	inline void stop() {running=false;}
	
	/**
	 * @brief スレッドの実行状態を取得
	 * @return true: 実行中、false: 停止中
	 */
	operator bool() {
		return running;
	}

private:
	std::atomic<bool> running{false}; ///< スレッド実行フラグ
};
