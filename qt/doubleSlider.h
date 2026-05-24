/**
 * @file qt/doubleSlider.h
 * @brief 範囲選択機能を持つカスタムQtスライダーウィジェット
 * @details 下限と上限の2つのハンドルを備えるスライダー（レンジスライダー）
 * @author yy981
 * @version 1.0
 * @note Qt5以上が必要
 */
#pragma once
#include <QtWidgets/QSlider>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QStyle>


/**
 * @class DoubleSlider
 * @brief 2つのハンドル（下限・上限）を持つスライダーウィジェット
 * 
 * @details
 * - 下限と上限の値を独立して管理
 * - 両方のハンドルを描画
 * @note 実装は未完成（完全な相互作用は未実装）
 */
class DoubleSlider : public QSlider {
	Q_OBJECT

public:
	/**
	 * @brief コンストラクタ
	 * @param orientation スライダーの方向（Qt::Horizontal/Qt::Vertical）
	 * @param parent 親ウィジェット
	 */
	DoubleSlider(Qt::Orientation orientation, QWidget *parent = nullptr)
		: QSlider(orientation, parent), lowerValue(0), upperValue(maximum()) {}

	int lowerValue;  ///< 下限値
	int upperValue;  ///< 上限値

protected:
	/**
	 * @brief ペイント イベント - スライダーの描画
	 * @param event ペイントイベント
	 * @details
	 * 基本スライダーを描画後、グルーブ全体（灰色）と
	 * 2つのハンドル位置（青色）を描画
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
	int lowerValueToPos() const {
		return static_cast<int>((lowerValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}

	int upperValueToPos() const {
		return static_cast<int>((upperValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}
};
