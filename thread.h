#pragma once
#include <thread>
#include <tuple>
#include <utility>
#include <atomic>
#include <yy981/time.h>

/**
 * @file thread.h
 * @brief 繰り返し実行スレッドクラス
 *
 * 指定した関数を一定間隔または連続でループ実行するデタッチスレッドを
 * RAII スタイルで管理します。
 */

/**
 * @class dthread
 * @brief デタッチされた繰り返し実行スレッドを管理するクラス
 *
 * コンストラクタでスレッドを起動し、デストラクタ（または stop()）で停止します。
 * スレッドはデタッチされているため、インスタンスが破棄されてもスレッドは
 * running フラグが false になるまで動き続けます。
 *
 * @code
 * // 500ms ごとに関数を実行
 * dthread dt(tu::l, 500.0, []{ std::cout << "tick\n"; });
 * std::this_thread::sleep_for(std::chrono::seconds(3));
 * dt.stop();
 * @endcode
 */
class dthread {
public:
	/**
	 * @brief 一定間隔でループ実行するスレッドを起動するコンストラクタ
	 * @tparam Func 実行する関数の型
	 * @tparam Args 関数に渡す引数の型（パック）
	 * @param unit  スリープの時間単位（tu 列挙型）
	 * @param value スリープの時間値
	 * @param func  繰り返し実行する関数
	 * @param args  func に渡す引数
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
	 * @brief スリープなしで連続ループ実行するスレッドを起動するコンストラクタ
	 * @tparam Func 実行する関数の型
	 * @tparam Args 関数に渡す引数の型（パック）
	 * @param noSleep スリープなしフラグ（値は無視、オーバーロード解決用）
	 * @param func    繰り返し実行する関数
	 * @param args    func に渡す引数
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
	 * @brief デストラクタ。スレッドのループを停止する
	 */
	~dthread() {running=false;}

	/**
	 * @brief スレッドのループを手動で停止する
	 */
	inline void stop() {running=false;}
	
	/**
	 * @brief スレッドが実行中かどうかを返す
	 * @return スレッドが実行中なら true
	 */
	operator bool() {
		return running;
	}

private:
	/// @brief スレッドの実行状態フラグ（アトミック）
	std::atomic<bool> running{false};
};
