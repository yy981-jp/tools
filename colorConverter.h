#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

/**
 * @file colorConverter.h
 * @brief 色空間変換ユーティリティ
 *
 * RGB・CMYK・HSL・HSB(HSV)・CIE Lab の各色空間間で相互変換する
 * 関数と構造体を提供します。
 */

/**
 * @namespace ccv
 * @brief 色変換ユーティリティ用名前空間
 */
namespace ccv {

/**
 * @struct RGB
 * @brief RGB 色空間を表す構造体（各成分 0〜255 の整数）
 */
struct RGB {
	int r; ///< 赤成分 (0〜255)
	int g; ///< 緑成分 (0〜255)
	int b; ///< 青成分 (0〜255)
	/** @brief コンストラクタ @param ir 赤 @param ig 緑 @param ib 青 */
	RGB(int ir, int ig, int ib): r(ir), g(ig), b(ib) {}
	RGB() {}
};

/**
 * @struct CMYK
 * @brief CMYK 色空間を表す構造体（各成分 0.0〜1.0 の浮動小数点数）
 */
struct CMYK {
	float c; ///< シアン (0.0〜1.0)
	float m; ///< マゼンタ (0.0〜1.0)
	float y; ///< イエロー (0.0〜1.0)
	float k; ///< キー（黒）(0.0〜1.0)
	/** @brief コンストラクタ @param ic シアン @param im マゼンタ @param iy イエロー @param ik キー */
	CMYK(float ic, float im, float iy, float ik): c(ic), m(im), y(iy), k(ik) {}
	CMYK() {}
};

/**
 * @struct HSL
 * @brief HSL 色空間を表す構造体
 */
struct HSL {
	float h; ///< 色相 (0〜360)
	float s; ///< 彩度 (0.0〜1.0)
	float l; ///< 輝度 (0.0〜1.0)
	/** @brief コンストラクタ @param ih 色相 @param is 彩度 @param il 輝度 */
	HSL(float ih, float is, float il): h(ih), s(is), l(il) {}
	HSL() {}
};

/**
 * @struct HSB
 * @brief HSB (HSV) 色空間を表す構造体
 */
struct HSB {
	float h; ///< 色相 (0〜360)
	float s; ///< 彩度 (0.0〜1.0)
	float b; ///< 明度 (0.0〜1.0)
	/** @brief コンストラクタ @param ih 色相 @param is 彩度 @param ib 明度 */
	HSB(float ih, float is, float ib): h(ih), s(is), b(ib) {}
	HSB() {}
};

/**
 * @struct Lab
 * @brief CIE L*a*b* 色空間を表す構造体
 */
struct Lab {
	float l; ///< 明度 L* (0〜100)
	float a; ///< 色度軸 a* (負：緑〜正：赤)
	float b; ///< 色度軸 b* (負：青〜正：黄)
	/** @brief コンストラクタ @param il L* @param ia a* @param ib b* */
	Lab(float il, float ia, float ib): l(il), a(ia), b(ib) {}
	Lab() {}
};

/**
 * @brief HEX 値（整数）を RGB 構造体に変換する
 * @param hex 0xRRGGBB 形式の色値
 * @return 変換後の RGB 構造体
 */
RGB hexToRgb(int hex) {
	RGB rgb;
	rgb.r = (hex >> 16) & 0xFF;  // 上位8ビットを取得
	rgb.g = (hex >> 8) & 0xFF;   // 中間8ビットを取得
	rgb.b = hex & 0xFF;		  // 下位8ビットを取得
	return rgb;
}

/**
 * @brief RGB 構造体を HEX 値（整数）に変換する
 * @param rgb 変換元の RGB 構造体
 * @return 0xRRGGBB 形式の整数値
 */
int rgbToHex(RGB rgb) {
	return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

/**
 * @brief RGB を CMYK に変換する
 * @param rgb 変換元の RGB 構造体
 * @return 変換後の CMYK 構造体
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
 * @brief CMYK を RGB に変換する
 * @param cmyk 変換元の CMYK 構造体
 * @return 変換後の RGB 構造体
 */
RGB cmykToRgb(CMYK cmyk) {
	RGB rgb;
	rgb.r = static_cast<int>((1 - cmyk.c) * (1 - cmyk.k) * 255);
	rgb.g = static_cast<int>((1 - cmyk.m) * (1 - cmyk.k) * 255);
	rgb.b = static_cast<int>((1 - cmyk.y) * (1 - cmyk.k) * 255);
	return rgb;
}

/**
 * @brief RGB を HSL に変換する
 * @param rgb 変換元の RGB 構造体
 * @return 変換後の HSL 構造体（色相 0〜360°）
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
 * @brief HSL を RGB に変換する
 * @param hsl 変換元の HSL 構造体（色相 0〜360°）
 * @return 変換後の RGB 構造体
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
 * @brief RGB を HSB (HSV) に変換する
 * @param rgb 変換元の RGB 構造体
 * @return 変換後の HSB 構造体（色相 0〜360°）
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
 * @brief HSB (HSV) を RGB に変換する
 * @param hsb 変換元の HSB 構造体（色相 0〜360°）
 * @return 変換後の RGB 構造体
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
 * @brief RGB を CIE L*a*b* に変換する
 *
 * sRGB → XYZ (D65) → Lab の2段階変換を行います。
 *
 * @param rgb 変換元の RGB 構造体
 * @return 変換後の Lab 構造体
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
 * @brief CIE L*a*b* を RGB に変換する
 *
 * Lab → XYZ (D65) → sRGB の2段階変換を行います。
 * 変換後の RGB 値は 0〜255 にクランプされます。
 *
 * @param lab 変換元の Lab 構造体
 * @return 変換後の RGB 構造体
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
