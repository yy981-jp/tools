# [日本語版] [→English Version](README_en.md)
## andor/ bool is_or(std::string, std::string...)
第一引数と第二引数以降のどれかが一致していればtrueを返し、すべて一致しない場合はfalseを返す

## andor/ bool is_and(std::string, std::string...)
第一引数と第二引数以降の全てが一致していればtrueを返し、どれか1つでも一致しない場合はfalseを返す

## colorConverter
RGB Hex CMYK HSL HSB Lab 色規格間の変換を行う

## debug/ void getSL(int)
何か値を渡されると、getSL()::<スレッドid>::CustomPoint::<渡された値> と出力し、渡されなかった場合は内部のカウンタを1づつ進め、SL::<スレッドid>::<内部カウンタ>を出力する

## debug/ checkTime()
計測開始時にオブジェクトを生成し、終了するときにオブジェクトのメンバ関数`end()`を呼び出す

## dll
https://github.com/yy981-jp/DLL のREADMEを参照

## INIParser/ INIParser(std:string) / std::string get(std::string, std::string)
オブジェクト生成時にファイル名を指定し、そのメンバの`get()`関数にセクションとキーを渡すと内容を返す
``` cpp
// 使用例
INIParser obj("exanmple.ini");
std::cout << obj.get("Network","IP");
```

## loadingTitle/ loadingTitle(int,int) / void stop()
コンソールタイトルで読み込み中であることを表す
オブジェクト生成時に文字数と更新頻度(ミリ秒)を指定し、`stop()`関数で終了する

## path/ std::string normalize_path(std::string), std::string normalize_path(char*)
パスを表す文字列を渡すと、`\`を`/`に置き換えて返す

## random/ int ranndomNum(int,int)
生成したい乱数の最低値と最高値を渡すと、その範囲内の乱数を返す

## time/ std::string getCTime(std::string)
日時フォーマットを渡すと、その形式で現在の日時を返す

## to_number-alphabet/ to_number, to_alphabet
a~Zを1~52に対応させたstd::map型変数 `to_number`はアルファベットを数字に `to_alphabet`はその逆を行う
