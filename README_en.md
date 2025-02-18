!!!!! Information is out of date We are currently in the process of creating instructions on my website !!!!!

It's incomplete, but the information at this link is newer https://yy981.web.fc2.com/cpp-h/index.html

# [English Version] [→日本語版](README.md)
## andor/ bool is_or(std::string, std::string...)
Returns true if the first argument and any of the arguments after the second match, or false if they do not match.

## andor/ bool is_and(std::string, std::string...)
Returns true if the first argument and all subsequent arguments match, false if any of the arguments do not match.

## colorConverter
Convert between RGB Hex CMYK HSL HSB Lab color standards.

## debug/ void getSL(int)
If any value is passed, outputs getSL()::<thread id>::CustomPoint::<passed value>, otherwise advances the internal counter by 1 and outputs SL::<thread id>::<internal counter>.

## debug/ checkTime()
Creates an object at the start of a measurement and calls the object's member function `end()` when the measurement ends.

## dll
See README at https://github.com/yy981-jp/DLL

## INIParser/ INIParser(std:string) / std::string get(std::string, std::string)
returns the contents of a file by specifying the filename when creating the object and passing the section and key to the ``get()`` function of the member.
``` cpp
// Example usage
INIParser obj(“exanmple.ini”);
std::cout << obj.get(“Network”, “IP”);
```

## loadingTitle/ loadingTitle(int,int) / void stop()
Console title to indicate that loading is in progress.
Specify the number of characters and update frequency (in milliseconds) when the object is created, and terminate with the `stop()` function.

## path/ std::string normalize_path(std::string), std::string normalize_path(char*)
Returns a string representing a path, replacing `\` with `/`.

## random/ int ranndomNum(int,int)
If you pass the lowest and highest random number you want to generate, it will return a random number within that range.

## time/ std::string getCTime(std::string)
Given a date/time format, returns the current date/time in that format

## to_number-alphabet/ to_number, to_alphabet
std::map type variable that maps a~Z to 1~52 `to_number` converts alphabet to number `to_alphabet` does the opposite

Translated with DeepL.com (free version)
