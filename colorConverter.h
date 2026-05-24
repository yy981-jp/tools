/**
 * @file colorConverter.h
 * @brief 複数の色空間間の変換機能を提供
 * @details RGB、CMYK、HSL、HSB、Lab色空間間の相互変換関数を提供
 * @author yy981
 * @version 1.0
 * 
 * @note
 * - RGB: 赤、緑、青の成分 (0-255)
 * - CMYK: シアン、マゼンタ、黄色、黒インク比 (0.0-1.0)
 * - HSL: 色相、彩度、明度 (H: 0-360°, S/L: 0.0-1.0)
 * - HSB: 色相、彩度、明るさ (H: 0-360°, S/B: 0.0-1.0)
 * - Lab: 知覚的な明度、a成分、b成分
 */
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>


/**
 * @namespace ccv
 * @brief 色空間変換関数と色構造体の名前空間
 */
namespace ccv {
	/**
	 * @struct RGB
	 * @brief RGB色を表す構造体
	 * @details 赤(R)、緑(G)、青(B)の各成分を0-255の値で表現
	 */
	struct RGB {
		int r, g, b; ///< 赤、緑、青の成分 (0-255)
		/**
		 * @brief コンストラクタ
		 * @param ir 赤成分 (0-255)
		 * @param ig 緑成分 (0-255)
		 * @param ib 青成分 (0-255)
		 */
		RGB(int ir, int ig, int ib): r(ir), g(ig), b(ib) {}
		RGB() {}
	};

	/**
	 * @struct CMYK
	 * @brief CMYK色を表す構造体
	 * @details シアン、マゼンタ、黄色、黒インクの各比率を0.0-1.0で表現
	 */
	struct CMYK {
		float c, m, y, k; ///< シアン、マゼンタ、黄色、黒インク比 (0.0-1.0)
		CMYK(float ic, float im, float iy, float ik): c(ic), m(im), y(iy), k(ik) {}
		CMYK() {}
	};

	/**
	 * @struct HSL
	 * @brief HSL色を表す構造体
	 * @details 色相、彩度、明度を表現
	 */
	struct HSL {
		float h, s, l; ///< 色相(0-360°)、彩度(0.0-1.0)、明度(0.0-1.0)
		HSL(float ih, float is, float il): h(ih), s(is), l(il) {}
		HSL() {}
	};

	/**
	 * @struct HSB
	 * @brief HSB(HSV)色を表す構造体
	 * @details 色相、彩度、明るさを表現
	 */
	struct HSB {
		float h, s, b; ///< 色相(0-360°)、彩度(0.0-1.0)、明るさ(0.0-1.0)
		HSB(float ih, float is, float ib): h(ih), s(is), b(ib) {}
		HSB() {}
	};

	/**
	 * @struct Lab
	 * @brief Lab色を表す構造体
	 * @details 知覚的な明度とa*b*成分を表現
	 */
	struct Lab {
		float l, a, b; ///< L成分(0-100)、a成分(-128～127)、b成分(-128～127)
		Lab(float il, float ia, float ib): l(il), a(ia), b(ib) {}
		Lab() {}
	};

	/**
	 * @brief 16進数(整数)をRGBに変換
	 * @param hex 16進数カラーコード (0xRRGGBB形式)
	 * @return RGB構造体
	 */
	RGB hexToRgb(int hex) {
		RGB rgb;
		rgb.r = (hex >> 16) & 0xFF;  // 上位8ビットを取得
		rgb.g = (hex >> 8) & 0xFF;   // 中間8ビットを取得
		rgb.b = hex & 0xFF;		  // 下位8ビットを取得
		return rgb;
	}

	/**
	 * @brief RGBを16進数(整数)に変換
	 * @param rgb RGB構造体
	 * @return 16進数カラーコード (0xRRGGBB形式)
	 */
	int rgbToHex(RGB rgb) {
		// RGBの値を16進数に変換して1つの整数として返す
		return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
	}

	/**
	 * @brief RGBをCMYKに変換
	 * @param rgb RGB構造体
	 * @return 変換されたCMYK構造体
	 */
	CMYK rgbToCmyk(RGB rgb) {
		CMYK cmyk;
		float r = rgb.r / 255.0;
		float g = rgb.g / 255.0;
		float b = rgb.b / 255.0;

		cmyk.k = 1 - std::max({r, g, b});
		if (cmyk.k < 1) {
			cmyk.c = (1 - r - cmyk.k) / (1 - cmyk.k);
			cmyk.m = (1 - g - cmyk.k) / (1 - cmyk.k);
			cmyk.y = (1 - b - cmyk.k) / (1 - cmyk.k);
		} else {
			cmyk.c = cmyk.m = cmyk.y = 0;
		}
		return cmyk;
	}

	/**
	 * @brief CMYKをRGBに変換
	 * @param cmyk CMYK構造体
	 * @return 変換されたRGB構造体
	 */
	RGB cmykToRgb(CMYK cmyk) {
		RGB rgb;
		rgb.r = static_cast<int>((1 - cmyk.c) * (1 - cmyk.k) * 255);
		rgb.g = static_cast<int>((1 - cmyk.m) * (1 - cmyk.k) * 255);
		rgb.b = static_cast<int>((1 - cmyk.y) * (1 - cmyk.k) * 255);
		return rgb;
	}

	/**
	 * @brief RGBをHSLに変換
	 * @param rgb RGB構造体
	 * @return 変換されたHSL構造体
	 */
	HSL rgbToHsl(RGB rgb) {
		HSL hsl;
		float r = rgb.r / 255.0;
		float g = rgb.g / 255.0;
		float b = rgb.b / 255.0;

		float max = std::max({r, g, b});
		float min = std::min({r, g, b});
		hsl.l = (max + min) / 2;

		if (max == min) {
			hsl.h = hsl.s = 0; // achromatic
		} else {
			float d = max - min;
			hsl.s = (hsl.l > 0.5) ? d / (2 - max - min) : d / (max + min);
			if (max == r) {
				hsl.h = fmod((g - b) / d + (g < b ? 6 : 0), 6);
			} else if (max == g) {
				hsl.h = (b - r) / d + 2;
			} else {
				hsl.h = (r - g) / d + 4;
			}
			hsl.h *= 60;
		}
		return hsl;
	}

	/**
	 * @brief HSLをRGBに変換
	 * @param hsl HSL構造体
	 * @return 変換されたRGB構造体
	 */
	RGB hslToRgb(HSL hsl) {
		RGB rgb;
		float c = (1 - std::abs(2 * hsl.l - 1)) * hsl.s;
		float x = c * (1 - std::abs(fmod(hsl.h / 60.0, 2) - 1));
		float m = hsl.l - c / 2;

		float r, g, b;
		if (hsl.h < 60) {
			r = c; g = x; b = 0;
		} else if (hsl.h < 120) {
			r = x; g = c; b = 0;
		} else if (hsl.h < 180) {
			r = 0; g = c; b = x;
		} else if (hsl.h < 240) {
			r = 0; g = x; b = c;
		} else if (hsl.h < 300) {
			r = x; g = 0; b = c;
		} else {
			r = c; g = 0; b = x;
		}

		rgb.r = static_cast<int>((r + m) * 255);
		rgb.g = static_cast<int>((g + m) * 255);
		rgb.b = static_cast<int>((b + m) * 255);
		return rgb;
	}

	/**
	 * @brief RGBをHSBに変換
	 * @param rgb RGB構造体
	 * @return 変換されたHSB構造体
	 */
	HSB rgbToHsb(RGB rgb) {
		HSB hsb;
		float r = rgb.r / 255.0;
		float g = rgb.g / 255.0;
		float b = rgb.b / 255.0;

		float max = std::max({r, g, b});
		float min = std::min({r, g, b});
		hsb.b = max;

		float d = max - min;
		hsb.s = (max == 0) ? 0 : d / max;

		if (max == min) {
			hsb.h = 0; // achromatic
		} else {
			if (max == r) {
				hsb.h = fmod((g - b) / d, 6);
			} else if (max == g) {
				hsb.h = (b - r) / d + 2;
			} else {
				hsb.h = (r - g) / d + 4;
			}
			hsb.h *= 60;
			if (hsb.h < 0) hsb.h += 360;
		}
		return hsb;
	}

	/**
	 * @brief HSBをRGBに変換
	 * @param hsb HSB構造体
	 * @return 変換されたRGB構造体
	 */
	RGB hsbToRgb(HSB hsb) {
		RGB rgb;
		float c = hsb.b * hsb.s;
		float x = c * (1 - std::abs(fmod(hsb.h / 60.0, 2) - 1));
		float m = hsb.b - c;

		float r, g, b;
		if (hsb.h < 60) {
			r = c; g = x; b = 0;
		} else if (hsb.h < 120) {
			r = x; g = c; b = 0;
		} else if (hsb.h < 180) {
			r = 0; g = c; b = x;
		} else if (hsb.h < 240) {
			r = 0; g = x; b = c;
		} else if (hsb.h < 300) {
			r = x; g = 0; b = c;
		} else {
			r = c; g = 0; b = x;
		}

		rgb.r = static_cast<int>((r + m) * 255);
		rgb.g = static_cast<int>((g + m) * 255);
		rgb.b = static_cast<int>((b + m) * 255);
		return rgb;
	}

	/**
	 * @brief RGBをLab色空間に変換
	 * @param rgb RGB構造体
	 * @return 変換されたLab構造体
	 * @details sRGB -> XYZ -> Labの2段階変換を実行
	 */
	Lab rgbToLab(RGB rgb) {
		Lab lab;
		float r = rgb.r / 255.0;
		float g = rgb.g / 255.0;
		float b = rgb.b / 255.0;

		// sRGBをXYZに変換
		r = (r > 0.04045) ? std::pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
		g = (g > 0.04045) ? std::pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
		b = (b > 0.04045) ? std::pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

		float x = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
		float y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
		float z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;

		// XYZをLabに変換
		x /= 0.95047; // reference white
		y /= 1.00000;
		z /= 1.08883;

		x = (x > 0.008856) ? std::pow(x, 1.0 / 3.0) : (x * 7.787 + 16 / 116.0);
		y = (y > 0.008856) ? std::pow(y, 1.0 / 3.0) : (y * 7.787 + 16 / 116.0);
		z = (z > 0.008856) ? std::pow(z, 1.0 / 3.0) : (z * 7.787 + 16 / 116.0);

		lab.l = (116 * y) - 16;
		lab.a = 500 * (x - y);
		lab.b = 200 * (y - z);

		return lab;
	}

	/**
	 * @brief Lab色空間をRGBに変換
	 * @param lab Lab構造体
	 * @return 変換されたRGB構造体
	 * @details Lab -> XYZ -> sRGBの逆変換を実行
	 */
	RGB labToRgb(Lab lab) {
		RGB rgb;
		float y = (lab.l + 16) / 116;
		float x = lab.a / 500 + y;
		float z = y - lab.b / 200;

		// Inverse transformation
		y = std::pow(y, 3) > 0.008856 ? std::pow(y, 3) : (y - 16 / 116) / 7.787;
		x = std::pow(x, 3) > 0.008856 ? std::pow(x, 3) : (x - 16 / 116) / 7.787;
		z = std::pow(z, 3) > 0.008856 ? std::pow(z, 3) : (z - 16 / 116) / 7.787;

		x *= 0.95047; // reference white
		y *= 1.00000;
		z *= 1.08883;

		rgb.r = static_cast<int>(std::clamp((x * 3.2404542 - y * 1.5371385 - z * 0.4985314) * 255, 0.0, 255.0));
		rgb.g = static_cast<int>(std::clamp((-x * 0.9692660 + y * 1.8760108 + z * 0.0415560) * 255, 0.0, 255.0));
		rgb.b = static_cast<int>(std::clamp((x * 0.0556434 - y * 0.2040259 + z * 1.0572252) * 255, 0.0, 255.0));

		return rgb;
	}
}