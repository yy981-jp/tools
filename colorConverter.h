#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>


namespace ccv {
	struct RGB {
		int r, g, b;
		RGB(int ir, int ig, int ib): r(ir), g(ig), b(ib) {}
		RGB() {}
	};

	struct CMYK {
		float c, m, y, k;
		CMYK(float ic, float im, float iy, float ik): c(ic), m(im), y(iy), k(ik) {}
		CMYK() {}
	};

	struct HSL {
		float h, s, l;
		HSL(float ih, float is, float il): h(ih), s(is), l(il) {}
		HSL() {}
	};

	struct HSB {
		float h, s, b;
		HSB(float ih, float is, float ib): h(ih), s(is), b(ib) {}
		HSB() {}
	};

	struct Lab {
		float l, a, b;
		Lab(float il, float ia, float ib): l(il), a(ia), b(ib) {}
		Lab() {}
	};

	// HEXを数値として受け取り、RGBに変換する関数
	RGB hexToRgb(int hex) {
		RGB rgb;
		rgb.r = (hex >> 16) & 0xFF;  // 上位8ビットを取得
		rgb.g = (hex >> 8) & 0xFF;   // 中間8ビットを取得
		rgb.b = hex & 0xFF;		  // 下位8ビットを取得
		return rgb;
	}

	// RGBからHEXに変換する関数
	int rgbToHex(RGB rgb) {
		// RGBの値を16進数に変換して1つの整数として返す
		return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
	}

	// RGBからCMYKへの変換
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

	// CMYKからRGBへの変換
	RGB cmykToRgb(CMYK cmyk) {
		RGB rgb;
		rgb.r = static_cast<int>((1 - cmyk.c) * (1 - cmyk.k) * 255);
		rgb.g = static_cast<int>((1 - cmyk.m) * (1 - cmyk.k) * 255);
		rgb.b = static_cast<int>((1 - cmyk.y) * (1 - cmyk.k) * 255);
		return rgb;
	}

	// RGBからHSLへの変換
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

	// HSLからRGBへの変換
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

	// RGBからHSBへの変換
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

	// HSBからRGBへの変換
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

	// RGBからLabへの変換
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

	// LabからRGBへの変換
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