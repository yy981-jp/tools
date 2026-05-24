#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>

/**
 * @file loadingTitle.h
 * @brief コンソールウィンドウタイトルにローディングアニメーションを表示するクラス
 *
 * Windows のコンソールウィンドウタイトルバーに □/■ を使った
 * スロットアニメーションを別スレッドで描画します。
 */

/**
 * @brief コンソールタイトルにローディングアニメーションを表示するクラス
 *
 * コンストラクタで別スレッドを起動し、タイトルバーに ■ が左右に動くアニメーションを描画します。
 * stop() を呼ぶとアニメーションが停止し、インスタンスは自動で delete されます。
 *
 * @warning stop() を呼んだ後はポインタを使用しないでください（自動削除されます）
 *
 * @code
 * auto* lt = new loadingTitle(5, 100); // 5マス、100ms間隔
 * // ... 処理 ...
 * lt->stop(); // アニメーション停止（ltはdeleteされる）
 * @endcode
 */
class loadingTitle {
public:
	/**
	 * @brief コンストラクタ。別スレッドでアニメーションを開始する
	 * @param size     アニメーションに使うマス（□/■）の数
	 * @param duration 1フレームの表示時間（ミリ秒）
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
	 * @brief アニメーションを停止する
	 * @note 停止後、スレッドが終了次第 delete this が実行されます
	 */
	void stop() {stopSignal=true;}
		
private:
	/// @brief スレッド停止フラグ
	bool stopSignal = false;
};
