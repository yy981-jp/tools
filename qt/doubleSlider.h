#pragma once
#include <QtWidgets/QSlider>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QStyle>

/**
 * @file qt/doubleSlider.h
 * @brief 上下限の2点を持つ Qt スライダーウィジェット
 *
 * QSlider を継承し、選択範囲の下限値と上限値の2つのハンドルを
 * 描画するダブルレンジスライダーを提供します。
 */

/**
 * @class DoubleSlider
 * @brief 下限・上限の2値を同時に表示・操作できるスライダーウィジェット
 *
 * lowerValue と upperValue の2つのパブリックメンバを持ち、
 * paintEvent でそれぞれの位置に青いハンドルを描画します。
 *
 * @note 現バージョンではマウス操作によるハンドルの移動は未実装です
 *
 * @code
 * auto* slider = new DoubleSlider(Qt::Horizontal, this);
 * slider->lowerValue = 20;
 * slider->upperValue = 80;
 * @endcode
 */
class DoubleSlider : public QSlider {
	Q_OBJECT

public:
	/**
	 * @brief コンストラクタ
	 * @param orientation スライダーの向き（Qt::Horizontal / Qt::Vertical）
	 * @param parent       親ウィジェット（デフォルト: nullptr）
	 */
	DoubleSlider(Qt::Orientation orientation, QWidget *parent = nullptr)
		: QSlider(orientation, parent), lowerValue(0), upperValue(maximum()) {}

	int lowerValue; ///< 下限ハンドルの値
	int upperValue; ///< 上限ハンドルの値

protected:
	/**
	 * @brief ペイントイベント。2つのハンドルをスロット上に描画する
	 * @param event ペイントイベント（基底クラスに転送される）
	 */
	void paintEvent(QPaintEvent *event) override {
		QSlider::paintEvent(event);
		QPainter painter(this);
		QRect groove = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderGroove, this);
		QRect handleLower = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderHandle, this);
		QRect handleUpper = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderHandle, this);

		handleLower.moveLeft(lowerValueToPos());
		handleUpper.moveLeft(upperValueToPos());

		painter.fillRect(groove, Qt::gray);
		painter.fillRect(handleLower, Qt::blue);
		painter.fillRect(handleUpper, Qt::blue);
	}

private:
	/**
	 * @brief lowerValue をスライダー上のピクセル位置に変換する
	 * @return lowerValue に対応する X 座標（ピクセル）
	 */
	int lowerValueToPos() const {
		return static_cast<int>((lowerValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}

	/**
	 * @brief upperValue をスライダー上のピクセル位置に変換する
	 * @return upperValue に対応する X 座標（ピクセル）
	 */
	int upperValueToPos() const {
		return static_cast<int>((upperValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}
};
