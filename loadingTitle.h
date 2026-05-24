/**
 * @file loadingTitle.h
 * @brief コンソールウィンドウのタイトルにローディングアニメーションを表示
 * @details 別スレッドで□■の切り替えアニメーションをコンソールタイトルに表示
 * @author yy981
 * @version 1.0
 * @note Windows専用 (SetConsoleTitleW使用)
 */
#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>


/**
 * @class loadingTitle
 * @brief コンソールタイトルにローディングアニメーションを表示
 * 
 * @details
 * - コンストラクタでアニメーション開始
 * - デタッチされたスレッドで実行（インスタンスはメモリ自動削除）
 * - stop()メソッドで停止可能
 */
class loadingTitle {
public:
	/**
	 * @brief コンストラクタ - アニメーション開始
	 * @param size アニメーション内のブロック数（表示幅）
	 * @param duration 各フレームの表示時間(ミリ秒)
	 * @details
	 * デタッチされたスレッドで実行され、インスタンスはスレッド内で自動削除される
	 * コンストラクタが戻った後は参照してはいけない
	 */
	loadingTitle(int size, int duration) {
		std::thread([size,duration,this]{
			char windowTitle[256];
			GetConsoleTitle(windowTitle,256);
			std::wstring loadTitle(size, L'□'); // サイズ分だけ初期化
			int count = 0;

			while (true) {
				count %= size;

				loadTitle[(count == 0 ? size - 1 : count - 1)] = L'□';
				loadTitle[count] = L'■';

				SetConsoleTitleW(loadTitle.c_str());
				std::this_thread::sleep_for(std::chrono::milliseconds(duration));

				++count;
				if (stopSignal) break;
			}
			delete this;
		}).detach();
	}
	/**
	 * @brief アニメーションを停止
	 * @details stopSignalをtrueに設定し、スレッドループを終了させる
	 */
	void stop() {stopSignal=true;}
		
private:
	bool stopSignal = false; ///< 停止フラグ
};