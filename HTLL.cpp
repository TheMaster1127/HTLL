#if __has_include("srell.hpp")
    #include "srell.hpp"
    #define USE_POWERFUL_REGEX 1
    #pragma message("SUCCESS: Compiling with powerful SRELL regex engine. Lookbehinds will work.")
#else
    #include <regex>
    #define USE_POWERFUL_REGEX 0
    #pragma message("WARNING: srell.hpp not found. Falling back to limited std::regex. Lookbehinds will NOT work.")
#endif

#include <algorithm>
#include <any>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

// Function to escape special characters for regex
std::string escapeRegex(const std::string& str) {
    static const std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
    return std::regex_replace(str, specialChars, R"(\$&)");
}
// Function to split a string based on delimiters
std::vector<std::string> LoopParseFunc(const std::string& var, const std::string& delimiter1 = "", const std::string& delimiter2 = "") {
    std::vector<std::string> items;
    if (delimiter1.empty() && delimiter2.empty()) {
        // If no delimiters are provided, return a list of characters
        for (char c : var) {
            items.push_back(std::string(1, c));
        }
    } else {
        // Escape delimiters for regex
        std::string escapedDelimiters = escapeRegex(delimiter1 + delimiter2);
        // Construct the regular expression pattern for splitting the string
        std::string pattern = "[" + escapedDelimiters + "]+";
        std::regex regexPattern(pattern);
        std::sregex_token_iterator iter(var.begin(), var.end(), regexPattern, -1);
        std::sregex_token_iterator end;
        while (iter != end) {
            items.push_back(*iter++);
        }
    }
    return items;
}

// Print function for const char*
void print(const char* value) {
    std::cout << std::string(value) << std::endl;
}
// Handle signed 8-bit integers
void print(int8_t value) {
    std::cout << static_cast<int>(value) << std::endl;
}
// Handle unsigned 8-bit integers
void print(uint8_t value) {
    std::cout << static_cast<unsigned int>(value) << std::endl;
}
// Generic print function fallback
template <typename T>
void print(const T& value) {
    std::cout << value << std::endl;
}

// Convert various types to std::string
std::string STR(int value) {
    return std::to_string(value);
}
// Convert various types to std::string
std::string STR(long long value) {
    return std::to_string(value);
}
std::string STR(float value) {
    return std::to_string(value);
}
std::string STR(double value) {
    return std::to_string(value);
}
std::string STR(size_t value) {
    return std::to_string(value);
}
std::string STR(bool value) {
    return value ? "1" : "0";
}
std::string STR(const char* value) {
    return std::string(value);
}
std::string STR(const std::string& value) {
    return value;
}

// Function to find the position of needle in haystack (std::string overload)
int InStr(const std::string& haystack, const std::string& needle) {
    size_t pos = haystack.find(needle);
    return (pos != std::string::npos) ? static_cast<int>(pos) + 1 : 0;
}

std::string FileRead(const std::string& path) {
    // This function relies on <fstream>, which is already in your global includes.
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file: " + path);
    }
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + '\n';
    }
    file.close();
    return content;
}

bool FileAppend(const std::string& content, const std::string& path) {
    std::ofstream file;
    // Open the file in append mode
    file.open(path, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file for appending." << std::endl;
        return false;
    }
    // Append the content to the file
    file << content;
    // Close the file
    file.close();
    return true;
}

bool FileDelete(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

size_t StrLen(const std::string& str) {
    return str.length();
}

std::string SubStr(const std::string& str, int startPos, int length = -1) {
    std::string result;
    size_t strLen = str.size();
    // Handle negative starting positions (counting from the end)
    if (startPos < 0) {
        startPos = strLen + startPos;
        if (startPos < 0) startPos = 0;  // Ensure it doesn't go beyond the start of the string
    }
    else {
        startPos -= 1; // Convert to 0-based index for internal operations
    }
    // Handle length
    if (length < 0) {
        length = strLen - startPos; // Length to the end of the string
    } else if (startPos + length > static_cast<int>(strLen)) {
        length = strLen - startPos; // Adjust length to fit within the string
    }
    // Extract the substring
    result = str.substr(startPos, length);
    return result;
}

std::string Trim(const std::string &inputString) {
    if (inputString.empty()) return "";
    size_t start = inputString.find_first_not_of(" \t\n\r\f\v");
    size_t end = inputString.find_last_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : inputString.substr(start, end - start + 1);
}

std::string StrReplace(const std::string &originalString, const std::string &find, const std::string &replaceWith) {
    std::string result = originalString;
    size_t pos = 0;
    while ((pos = result.find(find, pos)) != std::string::npos) {
        result.replace(pos, find.length(), replaceWith);
        pos += replaceWith.length();
    }
    return result;
}

std::string StringTrimLeft(const std::string &input, int numChars) {
    return (numChars <= input.length()) ? input.substr(numChars) : input;
}

std::string StringTrimRight(const std::string &input, int numChars) {
    return (numChars <= input.length()) ? input.substr(0, input.length() - numChars) : input;
}

std::string StrLower(const std::string &string) {
    std::string result = string;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StrSplit(const std::string &inputStr, const std::string &delimiter, int num) {
    size_t start = 0, end = 0, count = 0;
    while ((end = inputStr.find(delimiter, start)) != std::string::npos) {
        if (++count == num) {
            return inputStr.substr(start, end - start);
        }
        start = end + delimiter.length();
    }
    if (count + 1 == num) {
        return inputStr.substr(start);
    }
    return "";
}

std::string Chr(int number) {
    return (number >= 0 && number <= 0x10FFFF) ? std::string(1, static_cast<char>(number)) : "";
}

int Mod(int dividend, int divisor) {
    return dividend % divisor;
}

// Function to check if the operating system is Windows
bool isWindows() {
    #ifdef _WIN32
        return true;
    #else
        return false;
    #endif
}

#ifdef _WIN32
    #define ARGC __argc
    #define ARGV __argv
#else
    extern char **environ;
    int ARGC;
    char** ARGV;
    __attribute__((constructor)) void init_args(int argc, char* argv[], char* envp[]) {
        ARGC = argc;
        ARGV = argv;
    }
#endif
std::string GetParams() {
    // [FIX] This function is now safe as it does not use std::filesystem.
    std::vector<std::string> params;
    for (int i = 1; i < ARGC; ++i) {
        params.push_back(ARGV[i]);
    }
    std::string result;
    for (const auto& param : params) {
        result += param + "\n";
    }
    return result;
}

std::string RegExReplace(std::string_view inputStr, std::string_view regexPattern, std::string_view replacement) {
#if USE_POWERFUL_REGEX
    // --- SRELL PATH ---
    try {
        const srell::regex re = srell::regex(regexPattern.data(), regexPattern.size());
        return srell::regex_replace(std::string(inputStr), re, std::string(replacement));
    } catch (const srell::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
        return std::string(inputStr); // Return original string on failure
    }
#else
    // --- FALLBACK PATH ---
    try {
        const std::regex re{std::string(regexPattern)};
        return std::regex_replace(std::string(inputStr), re, std::string(replacement));
    } catch (const std::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
        return std::string(inputStr); // Return original string on failure
    }
#endif
}

int RegExMatch(std::string_view haystack, std::string_view needle) {
#if USE_POWERFUL_REGEX
    // --- SRELL PATH ---
    try {
        const srell::regex re = srell::regex(needle.data(), needle.size());
        srell::cmatch match;
        if (srell::regex_search(haystack.data(), haystack.data() + haystack.size(), match, re)) {
            return match.position(0) + 1;
        }
    } catch (const srell::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
    }
#else
    // --- FALLBACK PATH ---
    try {
        const std::regex re{std::string(needle)};
        std::match_results<std::string_view::const_iterator> match;
        if (std::regex_search(haystack.begin(), haystack.end(), match, re)) {
            return match.position(0) + 1;
        }
    } catch (const std::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
    }
#endif
    return 0; // Return 0 on failure
}

// Overload for counting a single character
int countChars(const std::string& str, char theChar) {
    int count = 0;
    for (char c : str) {
        if (c == theChar) {
            count++;
        }
    }
    return count;
}
// Overload for counting a substring
int countChars(const std::string& str, const std::string& substring) {
    if (substring.empty()) return 0; // Avoid infinite loop
    int count = 0;
    size_t pos = 0;
    // Find occurrences of the substring
    while ((pos = str.find(substring, pos)) != std::string::npos) {
        count++;
        pos += substring.length(); // Move past the found substring
    }
    return count;
}

void HTVM_Append(std::vector<std::string>& arr, const std::string& value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<std::string>& arr, const char* value) {
    arr.push_back(std::string(value));
}
void HTVM_Append(std::vector<int>& arr, int value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<float>& arr, float value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<bool>& arr, bool value) {
    arr.push_back(value);
}

size_t HTVM_Size(const std::vector<std::string>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<int>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<float>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<bool>& arr) {
    return arr.size();
}



// start of HT-Lib.htvm
// HT-Lib - A Library for Creating Programming Languages
// Copyright (C) 2025  TheMaster1127
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// global vars NEEDED
int HT_LIB_theIdNumOfThe34 = 0;
std::vector<std::string> HT_Lib_theIdNumOfThe34theVar;
//;;;;;;;;;;;;;;;;;;;;;;;;;
std::string preserveStrings(std::string code, std::string keyWordEscpaeChar = "\\") {
    std::vector<std::string> getAllCharForTheFurtureSoIcanAddEscapeChar;
    std::string ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = "";
    std::string str21 = "";
    std::string htCodeOUT754754 = "";
    std::string OutFixDoubleQuotesInsideDoubleQuotes = "";
    int fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
    int removeNexFixkeyWordEscpaeChar = 0;
    int areWEinSome34sNum = 0;
    std::vector<std::string> items1 = LoopParseFunc(code);
    for (size_t A_Index1 = 0; A_Index1 < items1.size(); A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
    }
    std::vector<std::string> items2 = LoopParseFunc(code);
    for (size_t A_Index2 = 0; A_Index2 < items2.size(); A_Index2++) {
        std::string A_LoopField2 = items2[A_Index2 - 0];
        HT_Lib_theIdNumOfThe34theVar[A_Index2] = HT_Lib_theIdNumOfThe34theVar[A_Index2] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField2);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "--" + str21 + "asds" + str21 + "as--" + str21 + "theuhtuwaesphoutr" + Chr(34);
    std::vector<std::string> items3 = LoopParseFunc(code);
    for (size_t A_Index3 = 0; A_Index3 < items3.size(); A_Index3++) {
        std::string A_LoopField3 = items3[A_Index3 - 0];
        if (A_LoopField3 == keyWordEscpaeChar && getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index3 + 1] == Chr(34)) {
            fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 1;
            OutFixDoubleQuotesInsideDoubleQuotes += ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes;
        } else {
            if (fixOutFixDoubleQuotesInsideDoubleQuotesFIXok != 1) {
                OutFixDoubleQuotesInsideDoubleQuotes += A_LoopField3;
            } else {
                fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
            }
        }
    }
    code = OutFixDoubleQuotesInsideDoubleQuotes;
    if (keyWordEscpaeChar != Chr(92)) {
        code = StrReplace(code, Chr(92), Chr(92) + Chr(92));
    }
    if (keyWordEscpaeChar == Chr(92)) {
        std::vector<std::string> items4 = LoopParseFunc(code);
        for (size_t A_Index4 = 0; A_Index4 < items4.size(); A_Index4++) {
            std::string A_LoopField4 = items4[A_Index4 - 0];
            if (A_LoopField4 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField4 != Chr(34)) {
                    if (A_LoopField4 == keyWordEscpaeChar) {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + Chr(92);
                    } else {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + A_LoopField4;
                    }
                } else {
                    HT_LIB_theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(HT_LIB_theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField4 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField4;
                }
                areWEinSome34sNum = 0;
            }
        }
    } else {
        std::vector<std::string> items5 = LoopParseFunc(code);
        for (size_t A_Index5 = 0; A_Index5 < items5.size(); A_Index5++) {
            std::string A_LoopField5 = items5[A_Index5 - 0];
            if (A_LoopField5 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField5 != Chr(34)) {
                    if (A_LoopField5 == keyWordEscpaeChar && keyWordEscpaeChar == getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index5 + 1]) {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + keyWordEscpaeChar;
                        removeNexFixkeyWordEscpaeChar = 1;
                    }
                    else if (A_LoopField5 == keyWordEscpaeChar) {
                        if (removeNexFixkeyWordEscpaeChar != 1) {
                            HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + Chr(92);
                        } else {
                            removeNexFixkeyWordEscpaeChar = 0;
                        }
                    } else {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + A_LoopField5;
                    }
                } else {
                    HT_LIB_theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(HT_LIB_theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField5 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField5;
                }
                areWEinSome34sNum = 0;
            }
        }
    }
    code = htCodeOUT754754;
    for (int A_Index6 = 0; A_Index6 < HT_LIB_theIdNumOfThe34; A_Index6++) {
        HT_Lib_theIdNumOfThe34theVar[A_Index6] = HT_Lib_theIdNumOfThe34theVar[A_Index6] + Chr(34);
    }
    HTVM_Append(HT_Lib_theIdNumOfThe34theVar, Chr(34));
    return code;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;
std::string restoreStrings(std::string codeOUT, std::string keyWordEscpaeChar = Chr(92)) {
    for (int A_Index7 = 0; A_Index7 < HT_LIB_theIdNumOfThe34; A_Index7++) {
        if (HT_LIB_theIdNumOfThe34 == A_Index7 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\"));
        }
    }
    return codeOUT;
}
std::string cleanUpFirst(std::string code) {
    code = StrReplace(code, Chr(13), "");
    std::string out = "";
    std::vector<std::string> items8 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index8 = 0; A_Index8 < items8.size(); A_Index8++) {
        std::string A_LoopField8 = items8[A_Index8 - 0];
        out += Trim(A_LoopField8) + Chr(10);
    }
    out = StringTrimRight(out, 1);
    return out;
}
std::string getLangParams(std::string binName, std::string langExtension, std::string extra = "") {
    std::string params = Trim(GetParams());
    std::string paramsTemp = "";
    if (params == "") {
        if (isWindows()) {
            print("Usage:" + Chr(10) + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        } else {
            print("Usage:" + Chr(10) + "./" + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        }
        return "";
    } else {
        return params;
    }
    return "MASSIVE ERROR";
}
void saveOutput(std::string outCode, std::string fileName) {
    FileDelete(Trim(fileName));
    FileAppend(Trim(outCode), Trim(fileName));
    print("Generation finished: " + Trim(fileName) + " generated.");
}
bool beginning(std::string line, std::string what) {
    if (SubStr(line, 1, StrLen(what)) == what) {
        return true;
    }
    return false;
}
std::string formatCurlyBracesForParsing(std::string code, std::string openBrace = "{", std::string closeBrace = "}") {
    code = StrReplace(code, openBrace, Chr(10) + "{" + Chr(10));
    code = StrReplace(code, closeBrace, Chr(10) + "}" + Chr(10));
    code = cleanUpFirst(code);
    return code;
}
std::string handleComments(std::string code, std::string commentKeyword = ";") {
    std::string str1 = "";
    std::vector<std::string> items9 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index9 = 0; A_Index9 < items9.size(); A_Index9++) {
        std::string A_LoopField9 = items9[A_Index9 - 0];
        str1 += StrSplit(A_LoopField9, commentKeyword, 1) + Chr(10);
    }
    code = StringTrimRight(str1, 1);
    return code;
}
// Define the function to check odd spaces at the beginning
std::string CheckOddLeadingSpaces(std::string string123) {
    // Initialize a variable to count the spaces
    int spaceCount = 0;
    // Loop through the string one character at a time
    std::vector<std::string> items10 = LoopParseFunc(string123);
    for (size_t A_Index10 = 0; A_Index10 < items10.size(); A_Index10++) {
        std::string A_LoopField10 = items10[A_Index10 - 0];
        // Check if the current character is a space
        if (A_LoopField10 == Chr(32)) {
            spaceCount++;
        } else {
            // When we hit a non-space character, break the loop
            break;
        }
    }
    // Return true if the number of spaces is odd, false otherwise
    std::string sdsfawasd = STR(Mod(spaceCount, 2) == 1);
    //MsgBox, % sdsfawasd
    return sdsfawasd;
}
std::string RepeatSpaces(int count) {
    std::string spaces = "";
    for (int A_Index11 = 0; A_Index11 < count; A_Index11++) {
        spaces += Chr(32);
    }
    return spaces;
}
// if you wanna convert to python, nim etc... indentation style we set modeCurlyBracesOn to 0
std::string indent_nested_curly_braces(std::string input_string, int modeCurlyBracesOn = 1) {
    int indent_size = 4;
    int current_indent = 0;
    std::string result = "";
    std::string trimmed_line = "";
    std::string resultOut = "";
    std::string culyOpenFix = "{";
    std::string culyCloseFix = "}";
    //MsgBox, % input_string
    std::vector<std::string> items12 = LoopParseFunc(input_string, "\n", "\r");
    for (size_t A_Index12 = 0; A_Index12 < items12.size(); A_Index12++) {
        std::string A_LoopField12 = items12[A_Index12 - 0];
        trimmed_line = Trim(A_LoopField12);
        if (trimmed_line == Chr(123)) {
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
            current_indent = current_indent + indent_size;
        }
        else if (trimmed_line == Chr(125)) {
            current_indent = current_indent - indent_size;
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
        } else {
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
        }
    }
    if (modeCurlyBracesOn == 0) {
        std::string resultOut = "";
        std::vector<std::string> items13 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index13 = 0; A_Index13 < items13.size(); A_Index13++) {
            std::string A_LoopField13 = items13[A_Index13 - 0];
            if (Trim(A_LoopField13) != "{" && Trim(A_LoopField13) != "}") {
                resultOut += A_LoopField13 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    } else {
        // format curly braces in a K&R style
        std::vector<std::string> lookIntoFurture;
        std::vector<std::string> items14 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index14 = 0; A_Index14 < items14.size(); A_Index14++) {
            std::string A_LoopField14 = items14[A_Index14 - 0];
            HTVM_Append(lookIntoFurture, Trim(A_LoopField14));
        }
        HTVM_Append(lookIntoFurture, " ");
        std::string resultOut = "";
        int skipNext = 0;
        std::vector<std::string> items15 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index15 = 0; A_Index15 < items15.size(); A_Index15++) {
            std::string A_LoopField15 = items15[A_Index15 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(lookIntoFurture[A_Index15 + 1]) == "{") {
                resultOut += A_LoopField15 + " " + culyOpenFix + Chr(10);
                skipNext = 2;
            }
            if (skipNext == 0) {
                resultOut += A_LoopField15 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
        std::vector<std::string> lookIntoFurture2;
        std::vector<std::string> items16 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index16 = 0; A_Index16 < items16.size(); A_Index16++) {
            std::string A_LoopField16 = items16[A_Index16 - 0];
            HTVM_Append(lookIntoFurture2, Trim(A_LoopField16));
        }
        HTVM_Append(lookIntoFurture2, " ");
        resultOut = "";
        skipNext = 0;
        std::string addSpacesAtTheBegginig = "";
        std::vector<std::string> items17 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index17 = 0; A_Index17 < items17.size(); A_Index17++) {
            std::string A_LoopField17 = items17[A_Index17 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(A_LoopField17) == "}" && Trim(lookIntoFurture2[A_Index17 + 1]) == "else {") {
                skipNext = 2;
                addSpacesAtTheBegginig = "";
                std::vector<std::string> items18 = LoopParseFunc(A_LoopField17);
                for (size_t A_Index18 = 0; A_Index18 < items18.size(); A_Index18++) {
                    std::string A_LoopField18 = items18[A_Index18 - 0];
                    if (A_LoopField18 == " ") {
                        if (A_LoopField18 != " ") {
                            break;
                        }
                        addSpacesAtTheBegginig += StrReplace(A_LoopField18, "}", culyCloseFix);
                    }
                }
                resultOut += addSpacesAtTheBegginig + culyCloseFix + " else " + culyOpenFix + Chr(10);
            }
            if (skipNext == 0) {
                resultOut += A_LoopField17 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    }
    resultOut = "";
    std::string ALoopField = "";
    std::vector<std::string> items19 = LoopParseFunc(result, "\n", "\r");
    for (size_t A_Index19 = 0; A_Index19 < items19.size(); A_Index19++) {
        std::string A_LoopField19 = items19[A_Index19 - 0];
        if (CheckOddLeadingSpaces(A_LoopField19) == "1") {
            ALoopField = StringTrimLeft(A_LoopField19, 1);
            resultOut += ALoopField + Chr(10);
        } else {
            resultOut += A_LoopField19 + Chr(10);
        }
    }
    result = StringTrimRight(resultOut, 1);
    // Return the result
    return result;
}
// end of HT-Lib.htvm
std::string str0 = "";
std::string str1 = "";
std::string str2 = "";
std::string str3 = "";
std::string str4 = "";
std::string str5 = "";
std::string str6 = "";
std::string str7 = "";
std::string str8 = "";
std::string str9 = "";
std::string str10 = "";
std::string str11 = "";
std::string str12 = "";
std::string str13 = "";
std::string str14 = "";
std::string str15 = "";
std::string str16 = "";
std::string str17 = "";
std::string str18 = "";
std::string str19 = "";
std::string str20 = "";
int is_arm = 1;
int ring0 = 0;
std::vector<std::string> nintArr;
std::string SubStrLastChars(std::string text, int numOfChars) {
    std::string LastOut = "";
    int NumOfChars = 0;
    std::vector<std::string> items20 = LoopParseFunc(text);
    for (size_t A_Index20 = 0; A_Index20 < items20.size(); A_Index20++) {
        std::string A_LoopField20 = items20[A_Index20 - 0];
        NumOfChars++;
    }
    for (int A_Index21 = 0; A_Index21 < numOfChars; A_Index21++) {
        NumOfChars--;
    }
    std::vector<std::string> items22 = LoopParseFunc(text);
    for (size_t A_Index22 = 0; A_Index22 < items22.size(); A_Index22++) {
        std::string A_LoopField22 = items22[A_Index22 - 0];
        if (A_Index22 >= NumOfChars) {
            LastOut += A_LoopField22;
        }
    }
    return LastOut;
}
std::string HTLL_Lang(std::string code) {
    std::string out = "";
    code = cleanUpFirst(code);
    code = preserveStrings(code);
    code = handleComments(code);
    code = formatCurlyBracesForParsing(code);
    for (int A_Index23 = 0; A_Index23 < HTVM_Size(HT_Lib_theIdNumOfThe34theVar); A_Index23++) {
        print(HT_Lib_theIdNumOfThe34theVar[A_Index23]);
    }
    std::vector<std::string> items24 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index24 = 0; A_Index24 < items24.size(); A_Index24++) {
        std::string A_LoopField24 = items24[A_Index24 - 0];
        if (SubStr(A_LoopField24, 1, 4) == "str ") {
            str1 = Trim(StringTrimLeft(A_LoopField24, 4));
        } else {
            out += A_LoopField24 + Chr(10);
        }
    }
    out = Trim(out);
    out = restoreStrings(out);
    return out;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bool isNint(std::string name) {
    for (int A_Index25 = 0; A_Index25 < HTVM_Size(nintArr); A_Index25++) {
        if (Trim(name) == Trim(nintArr[A_Index25])) {
            return true;
        }
    }
    return false;
}
std::string compiler(std::string code) {
    code = HTLL_Lang(code);
    std::string out = "";
    std::string main_syntax = "";
    if (is_arm == 1) {
        if (RegExMatch(code, "\\bargs_array\\b")) {
            main_syntax = "_start:" + Chr(10);
            // FIXED: Removed parens and comments that confuse the parser
            main_syntax += "    ldr x19, [sp]" + Chr(10);
            main_syntax += "    stp x29, x30, [sp, #-32]!" + Chr(10);
            main_syntax += "    mov x29, sp" + Chr(10);
            main_syntax += "    ldr x0, =args_array" + Chr(10);
            main_syntax += "    bl array_clear" + Chr(10);
            main_syntax += "    add x20, sp, #40" + Chr(10);
            main_syntax += "    mov x21, #1" + Chr(10);
            main_syntax += ".arg_loop:" + Chr(10);
            main_syntax += "    cmp x21, x19" + Chr(10);
            main_syntax += "    b.ge .args_done" + Chr(10);
            // Load argv[x21]
            main_syntax += "    ldr x22, [x20, x21, lsl #3]" + Chr(10);
            main_syntax += ".unpack_byte_loop:" + Chr(10);
            main_syntax += "    ldrb w1, [x22], #1" + Chr(10);
            main_syntax += "    cmp w1, #0" + Chr(10);
            main_syntax += "    b.eq .unpack_byte_done" + Chr(10);
            main_syntax += "    ldr x0, =args_array" + Chr(10);
            main_syntax += "    bl array_append" + Chr(10);
            main_syntax += "    b .unpack_byte_loop" + Chr(10);
            main_syntax += ".unpack_byte_done:" + Chr(10);
            main_syntax += "    mov x1, #10" + Chr(10);
            main_syntax += "    ldr x0, =args_array" + Chr(10);
            main_syntax += "    bl array_append" + Chr(10);
            main_syntax += "    add x21, x21, #1" + Chr(10);
            main_syntax += "    b .arg_loop" + Chr(10);
            main_syntax += ".args_done:" + Chr(10) + Chr(10);
        } else {
            // --- THE FIX IS HERE ---
            main_syntax = "_start:" + Chr(10);
            main_syntax += "    ; Standard ARM prologue" + Chr(10);
            main_syntax += "    stp x29, x30, [sp, #-16]!" + Chr(10);
            main_syntax += "    mov x29, sp" + Chr(10) + Chr(10);
        }
    } else {
        // --- YOUR EXISTING X86 CODE - UNCHANGED AND CORRECT ---
        if (RegExMatch(code, "\\bargs_array\\b")) {
            main_syntax = Chr(10) + "_start:" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "and rsp, -16" + Chr(10) + "mov rdi, args_array" + Chr(10) + "call array_clear" + Chr(10) + "mov r12, [rbp+8]" + Chr(10) + "lea r13, [rbp+16]" + Chr(10) + "mov r14, 1" + Chr(10) + ".arg_loop:" + Chr(10) + "cmp r14, r12" + Chr(10) + "jge .args_done" + Chr(10) + "mov r15, [r13 + r14*8]" + Chr(10) + ".unpack_byte_loop:" + Chr(10) + "movzx rbx, byte [r15]" + Chr(10) + "cmp rbx, 0" + Chr(10) + "je .unpack_byte_done" + Chr(10) + "mov rdi, args_array" + Chr(10) + "mov rsi, rbx" + Chr(10) + "call array_append" + Chr(10) + "inc r15" + Chr(10) + "jmp .unpack_byte_loop" + Chr(10) + ".unpack_byte_done:" + Chr(10) + "mov rdi, args_array" + Chr(10) + "mov rsi, 10" + Chr(10) + "call array_append" + Chr(10) + "inc r14" + Chr(10) + "jmp .arg_loop" + Chr(10) + ".args_done:" + Chr(10) + Chr(10);
        } else {
            main_syntax = Chr(10) + "_start:" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "and rsp, -16 " + Chr(10) + Chr(10);
        }
    }
    std::string HTLL_Libs_x86 = "";
    if (ring0 == 0) {
        HTLL_Libs_x86 = FileRead("HTLL_Libs_x86.txt");
    } else {
        HTLL_Libs_x86 = FileRead("HTLL_Libs_ring0.txt");
    }
    if (is_arm == 1) {
        HTLL_Libs_x86 = FileRead("HTLL_Libs_AArch64.txt");
    }
    int isDotCompile = 0;
    std::string dot_data = Chr(10);
    std::string dot_data_print_temp_strings = Chr(10);
    int dot_data_print_temp_strings_count = 0;
    std::string dot_data_ints = Chr(10);
    std::string path_label = "";
    std::string dot_bss = Chr(10);
    std::string dot_bss_str = Chr(10);
    std::string arrBss = "";
    std::vector<std::string> theIdNumOfThe34theVar;
    std::vector<std::string> getAllCharForTheFurtureSoIcanAddEscapeChar;
    std::string ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = "";
    std::string str21 = "";
    std::string htCodeOUT754754 = "";
    std::string OutFixDoubleQuotesInsideDoubleQuotes = "";
    std::string keyWordEscpaeChar = "\\";
    int fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
    int removeNexFixkeyWordEscpaeChar = 0;
    int theIdNumOfThe34 = 0;
    int areWEinSome34sNum = 0;
    int loopCount = 0;
    int loopCount1 = 0;
    int loopCount2 = 0;
    int loopCount3 = 0;
    int loopCount4 = 0;
    int loopCount5 = 0;
    int loopCount6 = 0;
    int loopCount7 = 0;
    int loopCount8 = 0;
    int loopCount9 = 0;
    int ifCount = 0;
    int ifCount1 = 0;
    int ifCount2 = 0;
    int ifCount3 = 0;
    int ifCount4 = 0;
    int ifCount5 = 0;
    int ifCount6 = 0;
    int ifCount7 = 0;
    int ifCount8 = 0;
    int ifCount9 = 0;
    int funcCount = 0;
    int isNum = 0;
    int localVarNum = 1;
    int inFunc = 0;
    int funcArgsCount = 0;
    std::vector<std::string> funcArgsArr;
    std::string funcName = "";
    std::string str_index = "";
    std::string str_value = "";
    int int1 = 0;
    int int2 = 0;
    int int3 = 0;
    int int4 = 0;
    int int5 = 0;
    int int6 = 0;
    std::vector<std::string> items26 = LoopParseFunc(code);
    for (size_t A_Index26 = 0; A_Index26 < items26.size(); A_Index26++) {
        std::string A_LoopField26 = items26[A_Index26 - 0];
        HTVM_Append(theIdNumOfThe34theVar, "");
        HTVM_Append(theIdNumOfThe34theVar, "");
    }
    std::vector<std::string> items27 = LoopParseFunc(code);
    for (size_t A_Index27 = 0; A_Index27 < items27.size(); A_Index27++) {
        std::string A_LoopField27 = items27[A_Index27 - 0];
        theIdNumOfThe34theVar[A_Index27] = theIdNumOfThe34theVar[A_Index27] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField27);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "--" + str21 + "asds" + str21 + "as--" + str21 + "theuhtuwaesphoutr" + Chr(34);
    std::vector<std::string> items28 = LoopParseFunc(code);
    for (size_t A_Index28 = 0; A_Index28 < items28.size(); A_Index28++) {
        std::string A_LoopField28 = items28[A_Index28 - 0];
        if (A_LoopField28 == keyWordEscpaeChar && getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index28 + 1] == Chr(34)) {
            fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 1;
            OutFixDoubleQuotesInsideDoubleQuotes += ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes;
        } else {
            if (fixOutFixDoubleQuotesInsideDoubleQuotesFIXok != 1) {
                OutFixDoubleQuotesInsideDoubleQuotes += A_LoopField28;
            } else {
                fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
            }
        }
    }
    code = OutFixDoubleQuotesInsideDoubleQuotes;
    if (keyWordEscpaeChar != Chr(92)) {
        code = StrReplace(code, Chr(92), Chr(92) + Chr(92));
    }
    if (keyWordEscpaeChar == Chr(92)) {
        std::vector<std::string> items29 = LoopParseFunc(code);
        for (size_t A_Index29 = 0; A_Index29 < items29.size(); A_Index29++) {
            std::string A_LoopField29 = items29[A_Index29 - 0];
            if (A_LoopField29 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField29 != Chr(34)) {
                    if (A_LoopField29 == keyWordEscpaeChar) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField29;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField29 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField29;
                }
                areWEinSome34sNum = 0;
            }
        }
    } else {
        std::vector<std::string> items30 = LoopParseFunc(code);
        for (size_t A_Index30 = 0; A_Index30 < items30.size(); A_Index30++) {
            std::string A_LoopField30 = items30[A_Index30 - 0];
            if (A_LoopField30 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField30 != Chr(34)) {
                    if (A_LoopField30 == keyWordEscpaeChar && keyWordEscpaeChar == getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index30 + 1]) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + keyWordEscpaeChar;
                        removeNexFixkeyWordEscpaeChar = 1;
                    }
                    else if (A_LoopField30 == keyWordEscpaeChar) {
                        if (removeNexFixkeyWordEscpaeChar != 1) {
                            theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                        } else {
                            removeNexFixkeyWordEscpaeChar = 0;
                        }
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField30;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField30 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField30;
                }
                areWEinSome34sNum = 0;
            }
        }
    }
    code = htCodeOUT754754;
    for (int A_Index31 = 0; A_Index31 < theIdNumOfThe34; A_Index31++) {
        theIdNumOfThe34theVar[A_Index31] = theIdNumOfThe34theVar[A_Index31] + Chr(34);
    }
    HTVM_Append(theIdNumOfThe34theVar, Chr(34));
    code = StrReplace(code, "{", Chr(10) + "{" + Chr(10));
    code = StrReplace(code, "}", Chr(10) + "}" + Chr(10));
    std::vector<std::string> items32 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index32 = 0; A_Index32 < items32.size(); A_Index32++) {
        std::string A_LoopField32 = items32[A_Index32 - 0];
        out += Trim(A_LoopField32) + Chr(10);
    }
    code = StringTrimRight(out, 1);
    str1 = "";
    out = "";
    std::vector<std::string> items33 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index33 = 0; A_Index33 < items33.size(); A_Index33++) {
        std::string A_LoopField33 = items33[A_Index33 - 0];
        std::vector<std::string> items34 = LoopParseFunc(A_LoopField33);
        for (size_t A_Index34 = 0; A_Index34 < items34.size(); A_Index34++) {
            std::string A_LoopField34 = items34[A_Index34 - 0];
            if (A_LoopField34 != ";") {
                str1 += A_LoopField34;
            } else {
                break;
            }
        }
        out += Trim(str1) + Chr(10);
        str1 = "";
    }
    code = StringTrimRight(out, 1);
    int seenMain = 0;
    out = "";
    std::vector<std::string> items35 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index35 = 0; A_Index35 < items35.size(); A_Index35++) {
        std::string A_LoopField35 = items35[A_Index35 - 0];
        if (Trim(A_LoopField35) == "main") {
            seenMain = 1;
            out += main_syntax + Chr(10);
        } else {
            out += A_LoopField35 + Chr(10);
        }
    }
    code = StringTrimRight(out, 1);
    int fix_func_temp_int = 0;
    std::vector<std::string> fix_func_temp_arr;
    out = "";
    std::vector<std::string> items36 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index36 = 0; A_Index36 < items36.size(); A_Index36++) {
        std::string A_LoopField36 = items36[A_Index36 - 0];
        if (SubStr(A_LoopField36, 1, 5) == "func ") {
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            str1 = Trim(StringTrimLeft(A_LoopField36, 5));
            str2 = Trim(StrSplit(str1, "(", 1));
            str3 = StrSplit(str1, "(", 2);
            str3 = Trim(StrReplace(str3, ")", ""));
            str4 = "";
            if (Trim(str3) != "") {
                fix_func_temp_arr = {};
                fix_func_temp_int = 1;
                std::vector<std::string> items37 = LoopParseFunc(str3, ",");
                for (size_t A_Index37 = 0; A_Index37 < items37.size(); A_Index37++) {
                    std::string A_LoopField37 = items37[A_Index37 - 0];
                    str4 += "_jhkjli_HTLL_HTLL_HTLL_" + Trim(A_LoopField37) + ", ";
                    HTVM_Append(fix_func_temp_arr, Trim(A_LoopField37));
                }
                str4 = StringTrimRight(str4, 2);
                out += "func " + str2 + "(" + str4 + ")" + Chr(10);
            } else {
                out += A_LoopField36 + Chr(10);
            }
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        }
        else if (Trim(A_LoopField36) == "funcend" || Trim(A_LoopField36) == "endfunc") {
            fix_func_temp_int = 0;
            fix_func_temp_arr = {};
            out += A_LoopField36 + Chr(10);
        } else {
            if (fix_func_temp_int == 1) {
                str1 = A_LoopField36;
                for (int A_Index38 = 0; A_Index38 < HTVM_Size(fix_func_temp_arr); A_Index38++) {
                    str1 = RegExReplace(str1, "\\b" + Trim(fix_func_temp_arr[A_Index38]) + "\\b", "_jhkjli_HTLL_HTLL_HTLL_" + Trim(fix_func_temp_arr[A_Index38]));
                }
                out += str1 + Chr(10);
            } else {
                out += A_LoopField36 + Chr(10);
            }
        }
    }
    code = StringTrimRight(out, 1);
    out = "";
    std::vector<std::string> items39 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index39 = 0; A_Index39 < items39.size(); A_Index39++) {
        std::string A_LoopField39 = items39[A_Index39 - 0];
        if (SubStr(A_LoopField39, 1, 7) == "arradd ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 7));
            str2 = StrSplit(str1, " ", 1);
            str3 = StringTrimLeft(str1, StrLen(str2) + 1);
            str4 = "";
            if (is_arm == 1) {
                // --- AArch64 CODE GENERATION ---
                std::vector<std::string> items40 = LoopParseFunc(str3);
                for (size_t A_Index40 = 0; A_Index40 < items40.size(); A_Index40++) {
                    std::string A_LoopField40 = items40[A_Index40 - 0];
                    // For each character, we call array_append(array_ptr, char_value)
                    // Arg 1 (array_ptr) goes in x0
                    // Arg 2 (char_value) goes in x1
                    str4 += "    ldr x0, =" + str2 + Chr(10);
                    str4 += "    mov x1, #" + Chr(39) + A_LoopField40 + Chr(39) + Chr(10);
                    str4 += "    bl array_append" + Chr(10);
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                std::vector<std::string> items41 = LoopParseFunc(str3);
                for (size_t A_Index41 = 0; A_Index41 < items41.size(); A_Index41++) {
                    std::string A_LoopField41 = items41[A_Index41 - 0];
                    str4 += "mov rsi, " + Chr(39) + A_LoopField41 + Chr(39) + Chr(10) + "mov rdi, " + str2 + Chr(10) + "call array_append" + Chr(10);
                }
            }
            out += str4 + Chr(10);
        }
        else if (SubStr(A_LoopField39, 1, 4) == "int ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            if (is_arm == 1) {
                // --- AArch64 CODE GENERATION ---
                if (InStr(str1, ":=")) {
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        dot_data_ints += str2 + ": .quad " + str3 + Chr(10);
                    } else {
                        dot_data_ints += str2 + ": .quad 0" + Chr(10);
                        // ############## FIX START ##############
                        if (str3 == "rax") {
                            out += "    mov x9, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x9, x20" + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x9, [x10]" + Chr(10);
                        }
                        // ############## FIX END ##############
                        out += "    ldr x10, =" + str2 + Chr(10);
                        out += "    str x9, [x10]" + Chr(10);
                    }
                } else {
                    if (InStr(str1, ":=") == false && InStr(str1, "+=") == false && InStr(str1, "-=") == false && InStr(str1, "*=") == false) {
                        dot_data_ints += Trim(str1) + ": .quad 0" + Chr(10);
                    }
                }
                if (InStr(str1, "*=")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                }
                else if (InStr(str1, "+=")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                }
                else if (InStr(str1, "-=")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                }
                if (InStr(A_LoopField39, " += ")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10);
                    out += "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    add x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    add x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    add x9, x9, #" + str3 + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    add x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                }
                else if (InStr(A_LoopField39, " *= ")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10);
                    out += "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    mul x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mul x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    mov x11, #" + str3 + Chr(10);
                        out += "    mul x9, x9, x11" + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    mul x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                }
                else if (InStr(A_LoopField39, " -= ")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10);
                    out += "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    sub x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    sub x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    sub x9, x9, #" + str3 + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    sub x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                str1 = Trim(StringTrimLeft(A_LoopField39, 4));
                if (InStr(str1, ":=")) {
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        dot_data_ints += str2 + " dq " + str3 + Chr(10);
                    } else {
                        dot_data_ints += str2 + " dq 0" + Chr(10);
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rdi" + Chr(10);
                    }
                } else {
                    if (InStr(str1, ":=") == false && InStr(str1, "+=") == false && InStr(str1, "-=") == false && InStr(str1, "*=") == false) {
                        dot_data_ints += Trim(str1) + " dq 0" + Chr(10);
                    }
                }
                if (InStr(str1, "*=")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                }
                else if (InStr(str1, "+=")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                }
                else if (InStr(str1, "-=")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                }
                if (InStr(A_LoopField39, " += ")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "add qword [" + str2 + "], " + str3 + Chr(10);
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "add qword [" + str2 + "], rdi" + Chr(10);
                    }
                }
                else if (InStr(A_LoopField39, " *= ")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, " + str3 + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                    }
                }
                else if (InStr(A_LoopField39, " -= ")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "sub qword [" + str2 + "], " + str3 + Chr(10);
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "sub qword [" + str2 + "], rdi" + Chr(10);
                    }
                }
            }
        }
        else if (SubStr(A_LoopField39, 1, 5) == "nint ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 5));
            if (InStr(str1, " ")) {
                HTVM_Append(nintArr, StrSplit(str1, " ", 1));
            } else {
                HTVM_Append(nintArr, Trim(str1));
            }
            if (is_arm == 1) {
                // --- AArch64 CODE GENERATION ---
                if (InStr(str1, ":=")) {
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        dot_data_ints += str2 + ": .quad " + str3 + Chr(10);
                        dot_data_ints += str2 + "_is_negative: .quad 0" + Chr(10);
                    } else {
                        dot_data_ints += str2 + ": .quad 0" + Chr(10);
                        dot_data_ints += str2 + "_is_negative: .quad 0" + Chr(10);
                        // ############## FIX START ##############
                        if (str3 == "rax") {
                            out += "    mov x9, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x9, x20" + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x9, [x10]" + Chr(10);
                        }
                        // ############## FIX END ##############
                        out += "    ldr x10, =" + str2 + Chr(10) + "    str x9, [x10]" + Chr(10);
                    }
                } else {
                    if (InStr(str1, ":=") == false && InStr(str1, "+=") == false && InStr(str1, "-=") == false && InStr(str1, "*=") == false) {
                        dot_data_ints += Trim(str1) + ": .quad 0" + Chr(10);
                        dot_data_ints += Trim(str1) + "_is_negative: .quad 0" + Chr(10);
                    }
                }
                if (InStr(str1, "*=")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: .quad 0" + Chr(10);
                }
                else if (InStr(str1, "+=")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: .quad 0" + Chr(10);
                }
                else if (InStr(str1, "-=")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    dot_data_ints += str2 + ": .quad 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: .quad 0" + Chr(10);
                }
                if (InStr(A_LoopField39, " += ")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10) + "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    add x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    add x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    add x9, x9, #" + str3 + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    add x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                    out += "    ldr x0, =" + str2 + Chr(10);
                    out += "    ldr x1, =" + str2 + "_is_negative" + Chr(10);
                    out += "    bl is_nint_negative" + Chr(10);
                }
                else if (InStr(A_LoopField39, " *= ")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10) + "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    mul x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mul x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    mov x11, #" + str3 + Chr(10);
                        out += "    mul x9, x9, x11" + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    mul x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                    out += "    ldr x0, =" + str2 + Chr(10);
                    out += "    ldr x1, =" + str2 + "_is_negative" + Chr(10);
                    out += "    bl is_nint_negative" + Chr(10);
                }
                else if (InStr(A_LoopField39, " -= ")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    out += "    ldr x10, =" + str2 + Chr(10) + "    ldr x9, [x10]" + Chr(10);
                    // ############## FIX START ##############
                    if (str3 == "rax") {
                        out += "    sub x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    sub x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    sub x9, x9, #" + str3 + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str3 + Chr(10) + "    ldr x11, [x11]" + Chr(10);
                        out += "    sub x9, x9, x11" + Chr(10);
                    }
                    // ############## FIX END ##############
                    out += "    str x9, [x10]" + Chr(10);
                    out += "    ldr x0, =" + str2 + Chr(10);
                    out += "    ldr x1, =" + str2 + "_is_negative" + Chr(10);
                    out += "    bl is_nint_negative" + Chr(10);
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                if (InStr(str1, ":=")) {
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        dot_data_ints += str2 + " dq " + str3 + Chr(10);
                        dot_data_ints += str2 + "_is_negative: dq 0" + Chr(10);
                    } else {
                        dot_data_ints += str2 + " dq 0" + Chr(10);
                        dot_data_ints += str2 + "_is_negative: dq 0" + Chr(10);
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rdi" + Chr(10);
                    }
                } else {
                    if (InStr(str1, ":=") == false && InStr(str1, "+=") == false && InStr(str1, "-=") == false && InStr(str1, "*=") == false) {
                        dot_data_ints += Trim(str1) + " dq 0" + Chr(10);
                        dot_data_ints += Trim(str1) + "_is_negative: dq 0" + Chr(10);
                    }
                }
                if (InStr(str1, "*=")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: dq 0" + Chr(10);
                }
                else if (InStr(str1, "+=")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: dq 0" + Chr(10);
                }
                else if (InStr(str1, "-=")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    dot_data_ints += str2 + " dq 0" + Chr(10);
                    dot_data_ints += str2 + "_is_negative: dq 0" + Chr(10);
                }
                if (InStr(A_LoopField39, " += ")) {
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "add qword [" + str2 + "], " + str3 + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "add qword [" + str2 + "], rdi" + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    }
                }
                else if (InStr(A_LoopField39, " *= ")) {
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, " + str3 + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    }
                }
                else if (InStr(A_LoopField39, " -= ")) {
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "sub qword [" + str2 + "], " + str3 + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "sub qword [" + str2 + "], rdi" + Chr(10);
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    }
                }
            }
        }
        else if (InStr(A_LoopField39, " := ") || InStr(A_LoopField39, " += ") || InStr(A_LoopField39, " -= ") || InStr(A_LoopField39, " *= ")) {
            if (is_arm == 1) {
                if (InStr(A_LoopField39, " := ")) {
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index42 = 0; A_Index42 < HTVM_Size(funcArgsArr); A_Index42++) {
                            if (Trim(funcArgsArr[A_Index42]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        out += "    mov x9, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index43 = 0; A_Index43 < HTVM_Size(funcArgsArr); A_Index43++) {
                                if (Trim(funcArgsArr[A_Index43]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (isRhsStackVar == 1) {
                            out += "    ldr x9, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + Chr(10);
                            out += "    ldr x9, [x10]" + Chr(10);
                        }
                    }
                    if (isLhsStackVar == 1) {
                        out += "    str x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str2 + Chr(10) + Chr(10);
                        out += "    str x9, [x10]" + Chr(10);
                    }
                    if (isNint(str2)) {
                        out += "    ldr x0, =" + str2 + Chr(10) + "    ldr x1, =" + str2 + "_is_negative" + Chr(10) + "    bl is_nint_negative" + Chr(10);
                    }
                } else {
                    std::string op = "";
                    std::string op_asm = "";
                    if (InStr(A_LoopField39, " += ")) {
                        op = "+=";
                        op_asm = "add";
                    }
                    else if (InStr(A_LoopField39, " -= ")) {
                        op = "-=";
                        op_asm = "sub";
                    }
                    else if (InStr(A_LoopField39, " *= ")) {
                        op = "*=";
                        op_asm = "mul";
                    }
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, op, 1));
                    str3 = Trim(StrSplit(str1, op, 2));
                    // --- Load Destination Value into x9 ---
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index44 = 0; A_Index44 < HTVM_Size(funcArgsArr); A_Index44++) {
                            if (Trim(funcArgsArr[A_Index44]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str2 + Chr(10) + Chr(10);
                        out += "    ldr x9, [x10]" + Chr(10);
                    }
                    // --- Load Source Value into x11 and perform operation ---
                    if (str3 == "rax") {
                        out += "    " + op_asm + " x9, x9, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    " + op_asm + " x9, x9, x20" + Chr(10);
                    }
                    else if (RegExMatch(str3, "^\\d+$")) {
                        if (op_asm == "mul") {
                            out += "    mov x11, #" + str3 + Chr(10) + Chr(10);
                            out += "    " + op_asm + " x9, x9, x11" + Chr(10);
                        } else {
                            out += "    " + op_asm + " x9, x9, #" + str3 + Chr(10);
                        }
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index45 = 0; A_Index45 < HTVM_Size(funcArgsArr); A_Index45++) {
                                if (Trim(funcArgsArr[A_Index45]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (isRhsStackVar == 1) {
                            out += "    ldr x11, =" + str3 + Chr(10) + Chr(10);
                            out += "    " + op_asm + " x9, x9, x11" + Chr(10);
                        } else {
                            out += "    ldr x11, =" + str3 + Chr(10) + Chr(10);
                            out += "    ldr x11, [x11]" + Chr(10) + Chr(10);
                            out += "    " + op_asm + " x9, x9, x11" + Chr(10);
                        }
                    }
                    // --- Store Result back into Destination ---
                    if (isLhsStackVar == 1) {
                        out += "    str x9, =" + str2 + Chr(10);
                    } else {
                        out += "    str x9, [x10]" + Chr(10);
                    }
                    if (isNint(str2)) {
                        out += "    ldr x0, =" + str2 + Chr(10) + "    ldr x1, =" + str2 + "_is_negative" + Chr(10) + "    bl is_nint_negative" + Chr(10);
                    }
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                if (InStr(A_LoopField39, " := ")) {
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, ":=", 1));
                    str3 = Trim(StrSplit(str1, ":=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "mov qword [" + str2 + "], " + str3 + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rdi" + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    }
                }
                else if (InStr(A_LoopField39, " += ")) {
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, "+=", 1));
                    str3 = Trim(StrSplit(str1, "+=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "add qword [" + str2 + "], " + str3 + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "add qword [" + str2 + "], rdi" + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    }
                }
                else if (InStr(A_LoopField39, " *= ")) {
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, "*=", 1));
                    str3 = Trim(StrSplit(str1, "*=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, " + str3 + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "imul rax, [" + str3 + "]" + Chr(10);
                        out += "mov [" + str2 + "], rax" + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    }
                }
                else if (InStr(A_LoopField39, " -= ")) {
                    str1 = Trim(A_LoopField39);
                    str2 = Trim(StrSplit(str1, "-=", 1));
                    str3 = Trim(StrSplit(str1, "-=", 2));
                    if (RegExMatch(str3, "^\\d+$")) {
                        out += "sub qword [" + str2 + "], " + str3 + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    } else {
                        out += "mov rdi, [" + str3 + "]" + Chr(10);
                        out += "sub qword [" + str2 + "], rdi" + Chr(10);
                        if (isNint(str2)) {
                            out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                        }
                    }
                }
            }
        }
        else if (InStr(A_LoopField39, "++")) {
            str1 = Trim(A_LoopField39);
            str1 = StringTrimRight(str1, 2);
            str1 = Trim(str1);
            if (is_arm == 1) {
                if (str1 == "rax") {
                    out += "    add x0, x0, #1" + Chr(10);
                }
                else if (str1 == "A_Index") {
                    out += "    add x20, x20, #1" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index46 = 0; A_Index46 < HTVM_Size(funcArgsArr); A_Index46++) {
                            if (Trim(funcArgsArr[A_Index46]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x9, =" + str1 + Chr(10);
                        out += "    add x9, x9, #1" + Chr(10);
                        out += "    str x9, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str1 + Chr(10);
                        out += "    ldr x9, [x10]" + Chr(10);
                        out += "    add x9, x9, #1" + Chr(10);
                        out += "    str x9, [x10]" + Chr(10);
                    }
                    if (isNint(str1)) {
                        out += "    ldr x0, =" + str1 + Chr(10) + "    ldr x1, =" + str1 + "_is_negative" + Chr(10) + "    bl is_nint_negative" + Chr(10);
                    }
                }
            } else {
                out += "inc qword [" + str1 + "]" + Chr(10);
                if (isNint(str1)) {
                    out += "lea rdi, [" + str1 + "]" + Chr(10) + "lea rsi, [" + str1 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                }
            }
        }
        else if (InStr(A_LoopField39, "--")) {
            str1 = Trim(A_LoopField39);
            str1 = StringTrimRight(str1, 2);
            str1 = Trim(str1);
            if (is_arm == 1) {
                if (str1 == "rax") {
                    out += "    sub x0, x0, #1" + Chr(10);
                }
                else if (str1 == "A_Index") {
                    out += "    sub x20, x20, #1" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index47 = 0; A_Index47 < HTVM_Size(funcArgsArr); A_Index47++) {
                            if (Trim(funcArgsArr[A_Index47]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x9, =" + str1 + Chr(10);
                        out += "    sub x9, x9, #1" + Chr(10);
                        out += "    str x9, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str1 + Chr(10);
                        out += "    ldr x9, [x10]" + Chr(10);
                        out += "    sub x9, x9, #1" + Chr(10);
                        out += "    str x9, [x10]" + Chr(10);
                    }
                    if (isNint(str1)) {
                        out += "    ldr x0, =" + str1 + Chr(10) + "    ldr x1, =" + str1 + "_is_negative" + Chr(10) + "    bl is_nint_negative" + Chr(10);
                    }
                }
            } else {
                out += "dec qword [" + str1 + "]" + Chr(10);
                if (isNint(str1)) {
                    out += "lea rdi, [" + str1 + "]" + Chr(10) + "lea rsi, [" + str1 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                }
            }
        }
        else if (SubStr(A_LoopField39, 1, 6) == "print(") {
            str1 = StringTrimLeft(A_LoopField39, 6);
            str1 = StringTrimRight(str1, 1);
            if (is_arm == 1) {
                if (str1 == "rax") {
                    // The value to print is ALREADY in x0.
                    // We just need to set the 2nd arg (is_negative) and call.
                    out += "    mov x1, xzr" + Chr(10);
                    out += "    bl print_number" + Chr(10);
                }
                else if (str1 == "A_Index") {
                    out += "    mov x0, x20" + Chr(10);
                    out += "    mov x1, xzr" + Chr(10);
                    out += "    bl print_number" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x0, #" + Trim(str1) + Chr(10);
                    out += "    mov x1, xzr" + Chr(10);
                    out += "    bl print_number" + Chr(10);
                }
                else if (InStr(str1, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC")) {
                    dot_data_print_temp_strings_count++;
                    std::string temp_label = "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count);
                    // GNU AS needs the string and newline handled separately for length calculation
                    std::string parsed_string = str1;
                    dot_data_print_temp_strings += temp_label + ": .string " + parsed_string + Chr(10);
                    dot_data_print_temp_strings += temp_label + "_len = . - " + temp_label + Chr(10);
                    out += "    ldr x1, =" + temp_label + Chr(10);
                    out += "    mov x2, " + temp_label + "_len" + Chr(10);
                    out += "    bl print_str" + Chr(10);
                    out += "    ldr x1, =HTLL_nl" + Chr(10);
                    out += "    mov x2, 1" + Chr(10);
                    out += "    bl print_str" + Chr(10);
                } else {
                    // --- START FIX ---
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index48 = 0; A_Index48 < HTVM_Size(funcArgsArr); A_Index48++) {
                            if (Trim(str1) == funcArgsArr[A_Index48]) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        // Stack Var: Regex will convert '=str1' to '[sp, #offset]'
                        // This generates: ldr x0, [sp, #offset]
                        out += "    ldr x0, =" + Trim(str1) + Chr(10);
                        out += "    mov x1, xzr" + Chr(10);
                        out += "    bl print_number" + Chr(10);
                    } else {
                        // Global Var (your existing logic)
                        if (isNint(str1)) {
                            out += "    ldr x9, =" + Trim(str1) + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                            out += "    ldr x9, =" + Trim(str1) + "_is_negative" + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                            out += "    bl print_number" + Chr(10);
                        } else {
                            out += "    ldr x9, =" + Trim(str1) + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                            out += "    mov x1, xzr" + Chr(10);
                            out += "    bl print_number" + Chr(10);
                        }
                    }
                    // --- END FIX ---
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "mov rdi, " + Trim(str1) + Chr(10) + "mov rsi, 0" + Chr(10) + "call print_number" + Chr(10);
                }
                else if (InStr(str1, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC")) {
                    dot_data_print_temp_strings_count++;
                    dot_data_print_temp_strings += "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + " db " + Trim(str1) + ", 10" + Chr(10) + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len = $-" + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10);
                    out += "mov rsi, ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10) + "mov rdx, " + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len" + Chr(10) + "call print_str" + Chr(10);
                } else {
                    if (isNint(str1)) {
                        out += "mov rdi, [" + Trim(str1) + "]" + Chr(10) + "mov rsi, [" + str1 + "_is_negative]" + Chr(10) + "call print_number" + Chr(10);
                    } else {
                        out += "mov rdi, [" + Trim(str1) + "]" + Chr(10) + "mov rsi, 0" + Chr(10) + "call print_number" + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "print_rax_as_char") {
            if (is_arm == 1) {
                // rax maps to x0, which is already the first argument for print_char
                out += "    bl print_char" + Chr(10);
            } else {
                out += "mov rdi, rax" + Chr(10) + "push rcx" + Chr(10) + "call print_char" + Chr(10) + "pop rcx" + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue" || StrLower(Trim(A_LoopField39)) == "continue1") {
            if (is_arm == 1) {
                out += "    b .cloop1_end" + STR(loopCount1) + Chr(10);
            } else {
                out += "jmp .cloop1_end" + STR(loopCount1) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue2") {
            if (is_arm == 1) {
                out += "    b .cloop2_end" + STR(loopCount2) + Chr(10);
            } else {
                out += "jmp .cloop2_end" + STR(loopCount2) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue3") {
            if (is_arm == 1) {
                out += "    b .cloop3_end" + STR(loopCount3) + Chr(10);
            } else {
                out += "jmp .cloop3_end" + STR(loopCount3) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue4") {
            if (is_arm == 1) {
                out += "    b .cloop4_end" + STR(loopCount4) + Chr(10);
            } else {
                out += "jmp .cloop4_end" + STR(loopCount4) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue5") {
            if (is_arm == 1) {
                out += "    b .cloop5_end" + STR(loopCount5) + Chr(10);
            } else {
                out += "jmp .cloop5_end" + STR(loopCount5) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue6") {
            if (is_arm == 1) {
                out += "    b .cloop6_end" + STR(loopCount6) + Chr(10);
            } else {
                out += "jmp .cloop6_end" + STR(loopCount6) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue7") {
            if (is_arm == 1) {
                out += "    b .cloop7_end" + STR(loopCount7) + Chr(10);
            } else {
                out += "jmp .cloop7_end" + STR(loopCount7) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue8") {
            if (is_arm == 1) {
                out += "    b .cloop8_end" + STR(loopCount8) + Chr(10);
            } else {
                out += "jmp .cloop8_end" + STR(loopCount8) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "continue9") {
            if (is_arm == 1) {
                out += "    b .cloop9_end" + STR(loopCount9) + Chr(10);
            } else {
                out += "jmp .cloop9_end" + STR(loopCount9) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break" || StrLower(Trim(A_LoopField39)) == "break1") {
            if (is_arm == 1) {
                out += "    b .loop1_end" + STR(loopCount1) + Chr(10);
            } else {
                out += "jmp .loop1_end" + STR(loopCount1) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break2") {
            if (is_arm == 1) {
                out += "    b .loop2_end" + STR(loopCount2) + Chr(10);
            } else {
                out += "jmp .loop2_end" + STR(loopCount2) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break3") {
            if (is_arm == 1) {
                out += "    b .loop3_end" + STR(loopCount3) + Chr(10);
            } else {
                out += "jmp .loop3_end" + STR(loopCount3) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break4") {
            if (is_arm == 1) {
                out += "    b .loop4_end" + STR(loopCount4) + Chr(10);
            } else {
                out += "jmp .loop4_end" + STR(loopCount4) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break5") {
            if (is_arm == 1) {
                out += "    b .loop5_end" + STR(loopCount5) + Chr(10);
            } else {
                out += "jmp .loop5_end" + STR(loopCount5) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break6") {
            if (is_arm == 1) {
                out += "    b .loop6_end" + STR(loopCount6) + Chr(10);
            } else {
                out += "jmp .loop6_end" + STR(loopCount6) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break7") {
            if (is_arm == 1) {
                out += "    b .loop7_end" + STR(loopCount7) + Chr(10);
            } else {
                out += "jmp .loop7_end" + STR(loopCount7) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break8") {
            if (is_arm == 1) {
                out += "    b .loop8_end" + STR(loopCount8) + Chr(10);
            } else {
                out += "jmp .loop8_end" + STR(loopCount8) + Chr(10);
            }
        }
        else if (StrLower(Trim(A_LoopField39)) == "break9") {
            if (is_arm == 1) {
                out += "    b .loop9_end" + STR(loopCount9) + Chr(10);
            } else {
                out += "jmp .loop9_end" + STR(loopCount9) + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 6) == "loop, " || SubStr(StrLower(A_LoopField39), 1, 7) == "loop1, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 6));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index49 = 0; A_Index49 < HTVM_Size(funcArgsArr); A_Index49++) {
                            if (Trim(funcArgsArr[A_Index49]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop1_" + STR(loopCount1) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop1_end" + STR(loopCount1) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop1_" + STR(loopCount1) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop1_end" + STR(loopCount1) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop1_" + STR(loopCount1) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop1_end" + STR(loopCount1) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend" || Trim(A_LoopField39) == "endloop" || Trim(A_LoopField39) == "loopend1" || Trim(A_LoopField39) == "endloop1") {
            if (is_arm == 1) {
                out += ".cloop1_end" + STR(loopCount1) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop1_" + STR(loopCount1) + Chr(10) + ".loop1_end" + STR(loopCount1) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop1_end" + STR(loopCount1) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop1_" + STR(loopCount1) + Chr(10) + ".loop1_end" + STR(loopCount1) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount1++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop2, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index50 = 0; A_Index50 < HTVM_Size(funcArgsArr); A_Index50++) {
                            if (Trim(funcArgsArr[A_Index50]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop2_" + STR(loopCount2) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop2_end" + STR(loopCount2) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop2_" + STR(loopCount2) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop2_end" + STR(loopCount2) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop2_" + STR(loopCount2) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop2_end" + STR(loopCount2) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend2" || Trim(A_LoopField39) == "endloop2") {
            if (is_arm == 1) {
                out += ".cloop2_end" + STR(loopCount2) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop2_" + STR(loopCount2) + Chr(10) + ".loop2_end" + STR(loopCount2) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop2_end" + STR(loopCount2) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop2_" + STR(loopCount2) + Chr(10) + ".loop2_end" + STR(loopCount2) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount2++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop3, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index51 = 0; A_Index51 < HTVM_Size(funcArgsArr); A_Index51++) {
                            if (Trim(funcArgsArr[A_Index51]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop3_" + STR(loopCount3) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop3_end" + STR(loopCount3) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop3_" + STR(loopCount3) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop3_end" + STR(loopCount3) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop3_" + STR(loopCount3) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop3_end" + STR(loopCount3) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend3" || Trim(A_LoopField39) == "endloop3") {
            if (is_arm == 1) {
                out += ".cloop3_end" + STR(loopCount3) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop3_" + STR(loopCount3) + Chr(10) + ".loop3_end" + STR(loopCount3) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop3_end" + STR(loopCount3) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop3_" + STR(loopCount3) + Chr(10) + ".loop3_end" + STR(loopCount3) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount3++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop4, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index52 = 0; A_Index52 < HTVM_Size(funcArgsArr); A_Index52++) {
                            if (Trim(funcArgsArr[A_Index52]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop4_" + STR(loopCount4) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop4_end" + STR(loopCount4) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop4_" + STR(loopCount4) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop4_end" + STR(loopCount4) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop4_" + STR(loopCount4) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop4_end" + STR(loopCount4) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend4" || Trim(A_LoopField39) == "endloop4") {
            if (is_arm == 1) {
                out += ".cloop4_end" + STR(loopCount4) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop4_" + STR(loopCount4) + Chr(10) + ".loop4_end" + STR(loopCount4) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop4_end" + STR(loopCount4) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop4_" + STR(loopCount4) + Chr(10) + ".loop4_end" + STR(loopCount4) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount4++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop5, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index53 = 0; A_Index53 < HTVM_Size(funcArgsArr); A_Index53++) {
                            if (Trim(funcArgsArr[A_Index53]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop5_" + STR(loopCount5) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop5_end" + STR(loopCount5) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop5_" + STR(loopCount5) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop5_end" + STR(loopCount5) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop5_" + STR(loopCount5) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop5_end" + STR(loopCount5) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend5" || Trim(A_LoopField39) == "endloop5") {
            if (is_arm == 1) {
                out += ".cloop5_end" + STR(loopCount5) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop5_" + STR(loopCount5) + Chr(10) + ".loop5_end" + STR(loopCount5) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop5_end" + STR(loopCount5) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop5_" + STR(loopCount5) + Chr(10) + ".loop5_end" + STR(loopCount5) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount5++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop6, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index54 = 0; A_Index54 < HTVM_Size(funcArgsArr); A_Index54++) {
                            if (Trim(funcArgsArr[A_Index54]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop6_" + STR(loopCount6) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop6_end" + STR(loopCount6) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop6_" + STR(loopCount6) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop6_end" + STR(loopCount6) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop6_" + STR(loopCount6) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop6_end" + STR(loopCount6) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend6" || Trim(A_LoopField39) == "endloop6") {
            if (is_arm == 1) {
                out += ".cloop6_end" + STR(loopCount6) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop6_" + STR(loopCount6) + Chr(10) + ".loop6_end" + STR(loopCount6) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop6_end" + STR(loopCount6) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop6_" + STR(loopCount6) + Chr(10) + ".loop6_end" + STR(loopCount6) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount6++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop7, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index55 = 0; A_Index55 < HTVM_Size(funcArgsArr); A_Index55++) {
                            if (Trim(funcArgsArr[A_Index55]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop7_" + STR(loopCount7) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop7_end" + STR(loopCount7) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop7_" + STR(loopCount7) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop7_end" + STR(loopCount7) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop7_" + STR(loopCount7) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop7_end" + STR(loopCount7) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend7" || Trim(A_LoopField39) == "endloop7") {
            if (is_arm == 1) {
                out += ".cloop7_end" + STR(loopCount7) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop7_" + STR(loopCount7) + Chr(10) + ".loop7_end" + STR(loopCount7) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop7_end" + STR(loopCount7) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop7_" + STR(loopCount7) + Chr(10) + ".loop7_end" + STR(loopCount7) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount7++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop8, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index56 = 0; A_Index56 < HTVM_Size(funcArgsArr); A_Index56++) {
                            if (Trim(funcArgsArr[A_Index56]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop8_" + STR(loopCount8) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop8_end" + STR(loopCount8) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop8_" + STR(loopCount8) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop8_end" + STR(loopCount8) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop8_" + STR(loopCount8) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop8_end" + STR(loopCount8) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend8" || Trim(A_LoopField39) == "endloop8") {
            if (is_arm == 1) {
                out += ".cloop8_end" + STR(loopCount8) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop8_" + STR(loopCount8) + Chr(10) + ".loop8_end" + STR(loopCount8) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop8_end" + STR(loopCount8) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop8_" + STR(loopCount8) + Chr(10) + ".loop8_end" + STR(loopCount8) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount8++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "loop9, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                out += "    stp x19, x20, [sp, #-16]!" + Chr(10);
                out += "    mov x20, xzr" + Chr(10);
                if (Trim(str1) == "rax") {
                    out += "    mov x19, x0" + Chr(10);
                }
                else if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x19, #" + str1 + Chr(10);
                }
                else if (Trim(str1) == "A_Index") {
                    out += "    mov x19, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index57 = 0; A_Index57 < HTVM_Size(funcArgsArr); A_Index57++) {
                            if (Trim(funcArgsArr[A_Index57]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x19, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x19, [x9]" + Chr(10);
                    }
                }
                out += ".loop9_" + STR(loopCount9) + ":" + Chr(10) + "    cmp x19, #0" + Chr(10) + "    b.eq .loop9_end" + STR(loopCount9) + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop9_" + STR(loopCount9) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop9_end" + STR(loopCount9) + Chr(10);
                } else {
                    out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop9_" + STR(loopCount9) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop9_end" + STR(loopCount9) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField39) == "loopend9" || Trim(A_LoopField39) == "endloop9") {
            if (is_arm == 1) {
                out += ".cloop9_end" + STR(loopCount9) + ":" + Chr(10) + "    add x20, x20, #1" + Chr(10) + "    sub x19, x19, #1" + Chr(10) + "    b .loop9_" + STR(loopCount9) + Chr(10) + ".loop9_end" + STR(loopCount9) + ":" + Chr(10) + "    ldp x19, x20, [sp], #16" + Chr(10);
            } else {
                out += ".cloop9_end" + STR(loopCount9) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop9_" + STR(loopCount9) + Chr(10) + ".loop9_end" + STR(loopCount9) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            }
            loopCount9++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 3) == "if " || SubStr(StrLower(A_LoopField39), 1, 4) == "if1 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 3));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    // --- LHS (str2) ---
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index58 = 0; A_Index58 < HTVM_Size(funcArgsArr); A_Index58++) {
                            if (Trim(funcArgsArr[A_Index58]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    // --- RHS (str3) ---
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index59 = 0; A_Index59 < HTVM_Size(funcArgsArr); A_Index59++) {
                            if (Trim(funcArgsArr[A_Index59]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ne .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index60 = 0; A_Index60 < HTVM_Size(funcArgsArr); A_Index60++) {
                            if (Trim(funcArgsArr[A_Index60]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index61 = 0; A_Index61 < HTVM_Size(funcArgsArr); A_Index61++) {
                                if (Trim(funcArgsArr[A_Index61]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.eq .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index62 = 0; A_Index62 < HTVM_Size(funcArgsArr); A_Index62++) {
                            if (Trim(funcArgsArr[A_Index62]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index63 = 0; A_Index63 < HTVM_Size(funcArgsArr); A_Index63++) {
                                if (Trim(funcArgsArr[A_Index63]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.le .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jle .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jle .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index64 = 0; A_Index64 < HTVM_Size(funcArgsArr); A_Index64++) {
                            if (Trim(funcArgsArr[A_Index64]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index65 = 0; A_Index65 < HTVM_Size(funcArgsArr); A_Index65++) {
                                if (Trim(funcArgsArr[A_Index65]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ge .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jge .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jge .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index66 = 0; A_Index66 < HTVM_Size(funcArgsArr); A_Index66++) {
                            if (Trim(funcArgsArr[A_Index66]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index67 = 0; A_Index67 < HTVM_Size(funcArgsArr); A_Index67++) {
                                if (Trim(funcArgsArr[A_Index67]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.lt .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jl .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jl .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index68 = 0; A_Index68 < HTVM_Size(funcArgsArr); A_Index68++) {
                            if (Trim(funcArgsArr[A_Index68]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index69 = 0; A_Index69 < HTVM_Size(funcArgsArr); A_Index69++) {
                                if (Trim(funcArgsArr[A_Index69]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.gt .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jg .end_if1_" + STR(ifCount1) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jg .end_if1_" + STR(ifCount1) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend" || Trim(A_LoopField39) == "endif" || Trim(A_LoopField39) == "ifend1" || Trim(A_LoopField39) == "endif1") {
            out += ".end_if1_" + STR(ifCount1) + ":" + Chr(10);
            ifCount1++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if2 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    // --- LHS (str2) ---
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index70 = 0; A_Index70 < HTVM_Size(funcArgsArr); A_Index70++) {
                            if (Trim(funcArgsArr[A_Index70]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    // --- RHS (str3) ---
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index71 = 0; A_Index71 < HTVM_Size(funcArgsArr); A_Index71++) {
                            if (Trim(funcArgsArr[A_Index71]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ne .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index72 = 0; A_Index72 < HTVM_Size(funcArgsArr); A_Index72++) {
                            if (Trim(funcArgsArr[A_Index72]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index73 = 0; A_Index73 < HTVM_Size(funcArgsArr); A_Index73++) {
                                if (Trim(funcArgsArr[A_Index73]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.eq .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index74 = 0; A_Index74 < HTVM_Size(funcArgsArr); A_Index74++) {
                            if (Trim(funcArgsArr[A_Index74]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index75 = 0; A_Index75 < HTVM_Size(funcArgsArr); A_Index75++) {
                                if (Trim(funcArgsArr[A_Index75]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.le .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jle .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jle .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index76 = 0; A_Index76 < HTVM_Size(funcArgsArr); A_Index76++) {
                            if (Trim(funcArgsArr[A_Index76]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index77 = 0; A_Index77 < HTVM_Size(funcArgsArr); A_Index77++) {
                                if (Trim(funcArgsArr[A_Index77]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ge .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jge .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jge .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index78 = 0; A_Index78 < HTVM_Size(funcArgsArr); A_Index78++) {
                            if (Trim(funcArgsArr[A_Index78]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index79 = 0; A_Index79 < HTVM_Size(funcArgsArr); A_Index79++) {
                                if (Trim(funcArgsArr[A_Index79]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.lt .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jl .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jl .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index80 = 0; A_Index80 < HTVM_Size(funcArgsArr); A_Index80++) {
                            if (Trim(funcArgsArr[A_Index80]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index81 = 0; A_Index81 < HTVM_Size(funcArgsArr); A_Index81++) {
                                if (Trim(funcArgsArr[A_Index81]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.gt .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jg .end_if2_" + STR(ifCount2) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jg .end_if2_" + STR(ifCount2) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend2" || Trim(A_LoopField39) == "endif2") {
            out += ".end_if2_" + STR(ifCount2) + ":" + Chr(10);
            ifCount2++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if3 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index82 = 0; A_Index82 < HTVM_Size(funcArgsArr); A_Index82++) {
                            if (Trim(funcArgsArr[A_Index82]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index83 = 0; A_Index83 < HTVM_Size(funcArgsArr); A_Index83++) {
                            if (Trim(funcArgsArr[A_Index83]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ne .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index84 = 0; A_Index84 < HTVM_Size(funcArgsArr); A_Index84++) {
                            if (Trim(funcArgsArr[A_Index84]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index85 = 0; A_Index85 < HTVM_Size(funcArgsArr); A_Index85++) {
                                if (Trim(funcArgsArr[A_Index85]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.eq .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index86 = 0; A_Index86 < HTVM_Size(funcArgsArr); A_Index86++) {
                            if (Trim(funcArgsArr[A_Index86]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index87 = 0; A_Index87 < HTVM_Size(funcArgsArr); A_Index87++) {
                                if (Trim(funcArgsArr[A_Index87]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.le .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jle .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jle .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index88 = 0; A_Index88 < HTVM_Size(funcArgsArr); A_Index88++) {
                            if (Trim(funcArgsArr[A_Index88]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index89 = 0; A_Index89 < HTVM_Size(funcArgsArr); A_Index89++) {
                                if (Trim(funcArgsArr[A_Index89]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.ge .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jge .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jge .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index90 = 0; A_Index90 < HTVM_Size(funcArgsArr); A_Index90++) {
                            if (Trim(funcArgsArr[A_Index90]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index91 = 0; A_Index91 < HTVM_Size(funcArgsArr); A_Index91++) {
                                if (Trim(funcArgsArr[A_Index91]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.lt .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jl .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jl .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index92 = 0; A_Index92 < HTVM_Size(funcArgsArr); A_Index92++) {
                            if (Trim(funcArgsArr[A_Index92]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index93 = 0; A_Index93 < HTVM_Size(funcArgsArr); A_Index93++) {
                                if (Trim(funcArgsArr[A_Index93]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10) + "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10) + "    b.gt .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jg .end_if3_" + STR(ifCount3) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jg .end_if3_" + STR(ifCount3) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend3" || Trim(A_LoopField39) == "endif3") {
            out += ".end_if3_" + STR(ifCount3) + ":" + Chr(10);
            ifCount3++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if4 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index94 = 0; A_Index94 < HTVM_Size(funcArgsArr); A_Index94++) {
                            if (Trim(funcArgsArr[A_Index94]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index95 = 0; A_Index95 < HTVM_Size(funcArgsArr); A_Index95++) {
                            if (Trim(funcArgsArr[A_Index95]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index96 = 0; A_Index96 < HTVM_Size(funcArgsArr); A_Index96++) {
                            if (Trim(funcArgsArr[A_Index96]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index97 = 0; A_Index97 < HTVM_Size(funcArgsArr); A_Index97++) {
                                if (Trim(funcArgsArr[A_Index97]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index98 = 0; A_Index98 < HTVM_Size(funcArgsArr); A_Index98++) {
                            if (Trim(funcArgsArr[A_Index98]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index99 = 0; A_Index99 < HTVM_Size(funcArgsArr); A_Index99++) {
                                if (Trim(funcArgsArr[A_Index99]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index100 = 0; A_Index100 < HTVM_Size(funcArgsArr); A_Index100++) {
                            if (Trim(funcArgsArr[A_Index100]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index101 = 0; A_Index101 < HTVM_Size(funcArgsArr); A_Index101++) {
                                if (Trim(funcArgsArr[A_Index101]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index102 = 0; A_Index102 < HTVM_Size(funcArgsArr); A_Index102++) {
                            if (Trim(funcArgsArr[A_Index102]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index103 = 0; A_Index103 < HTVM_Size(funcArgsArr); A_Index103++) {
                                if (Trim(funcArgsArr[A_Index103]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index104 = 0; A_Index104 < HTVM_Size(funcArgsArr); A_Index104++) {
                            if (Trim(funcArgsArr[A_Index104]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index105 = 0; A_Index105 < HTVM_Size(funcArgsArr); A_Index105++) {
                                if (Trim(funcArgsArr[A_Index105]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if4_" + STR(ifCount4) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if4_" + STR(ifCount4) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend4" || Trim(A_LoopField39) == "endif4") {
            out += ".end_if4_" + STR(ifCount4) + ":" + Chr(10);
            ifCount4++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if5 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index106 = 0; A_Index106 < HTVM_Size(funcArgsArr); A_Index106++) {
                            if (Trim(funcArgsArr[A_Index106]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index107 = 0; A_Index107 < HTVM_Size(funcArgsArr); A_Index107++) {
                            if (Trim(funcArgsArr[A_Index107]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index108 = 0; A_Index108 < HTVM_Size(funcArgsArr); A_Index108++) {
                            if (Trim(funcArgsArr[A_Index108]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index109 = 0; A_Index109 < HTVM_Size(funcArgsArr); A_Index109++) {
                                if (Trim(funcArgsArr[A_Index109]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index110 = 0; A_Index110 < HTVM_Size(funcArgsArr); A_Index110++) {
                            if (Trim(funcArgsArr[A_Index110]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index111 = 0; A_Index111 < HTVM_Size(funcArgsArr); A_Index111++) {
                                if (Trim(funcArgsArr[A_Index111]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index112 = 0; A_Index112 < HTVM_Size(funcArgsArr); A_Index112++) {
                            if (Trim(funcArgsArr[A_Index112]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index113 = 0; A_Index113 < HTVM_Size(funcArgsArr); A_Index113++) {
                                if (Trim(funcArgsArr[A_Index113]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index114 = 0; A_Index114 < HTVM_Size(funcArgsArr); A_Index114++) {
                            if (Trim(funcArgsArr[A_Index114]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index115 = 0; A_Index115 < HTVM_Size(funcArgsArr); A_Index115++) {
                                if (Trim(funcArgsArr[A_Index115]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index116 = 0; A_Index116 < HTVM_Size(funcArgsArr); A_Index116++) {
                            if (Trim(funcArgsArr[A_Index116]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index117 = 0; A_Index117 < HTVM_Size(funcArgsArr); A_Index117++) {
                                if (Trim(funcArgsArr[A_Index117]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if5_" + STR(ifCount5) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if5_" + STR(ifCount5) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend5" || Trim(A_LoopField39) == "endif5") {
            out += ".end_if5_" + STR(ifCount5) + ":" + Chr(10);
            ifCount5++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if6 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index118 = 0; A_Index118 < HTVM_Size(funcArgsArr); A_Index118++) {
                            if (Trim(funcArgsArr[A_Index118]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index119 = 0; A_Index119 < HTVM_Size(funcArgsArr); A_Index119++) {
                            if (Trim(funcArgsArr[A_Index119]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index120 = 0; A_Index120 < HTVM_Size(funcArgsArr); A_Index120++) {
                            if (Trim(funcArgsArr[A_Index120]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index121 = 0; A_Index121 < HTVM_Size(funcArgsArr); A_Index121++) {
                                if (Trim(funcArgsArr[A_Index121]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index122 = 0; A_Index122 < HTVM_Size(funcArgsArr); A_Index122++) {
                            if (Trim(funcArgsArr[A_Index122]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index123 = 0; A_Index123 < HTVM_Size(funcArgsArr); A_Index123++) {
                                if (Trim(funcArgsArr[A_Index123]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index124 = 0; A_Index124 < HTVM_Size(funcArgsArr); A_Index124++) {
                            if (Trim(funcArgsArr[A_Index124]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index125 = 0; A_Index125 < HTVM_Size(funcArgsArr); A_Index125++) {
                                if (Trim(funcArgsArr[A_Index125]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index126 = 0; A_Index126 < HTVM_Size(funcArgsArr); A_Index126++) {
                            if (Trim(funcArgsArr[A_Index126]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index127 = 0; A_Index127 < HTVM_Size(funcArgsArr); A_Index127++) {
                                if (Trim(funcArgsArr[A_Index127]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index128 = 0; A_Index128 < HTVM_Size(funcArgsArr); A_Index128++) {
                            if (Trim(funcArgsArr[A_Index128]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index129 = 0; A_Index129 < HTVM_Size(funcArgsArr); A_Index129++) {
                                if (Trim(funcArgsArr[A_Index129]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if6_" + STR(ifCount6) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if6_" + STR(ifCount6) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend6" || Trim(A_LoopField39) == "endif6") {
            out += ".end_if6_" + STR(ifCount6) + ":" + Chr(10);
            ifCount6++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if7 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index130 = 0; A_Index130 < HTVM_Size(funcArgsArr); A_Index130++) {
                            if (Trim(funcArgsArr[A_Index130]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index131 = 0; A_Index131 < HTVM_Size(funcArgsArr); A_Index131++) {
                            if (Trim(funcArgsArr[A_Index131]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index132 = 0; A_Index132 < HTVM_Size(funcArgsArr); A_Index132++) {
                            if (Trim(funcArgsArr[A_Index132]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index133 = 0; A_Index133 < HTVM_Size(funcArgsArr); A_Index133++) {
                                if (Trim(funcArgsArr[A_Index133]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index134 = 0; A_Index134 < HTVM_Size(funcArgsArr); A_Index134++) {
                            if (Trim(funcArgsArr[A_Index134]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index135 = 0; A_Index135 < HTVM_Size(funcArgsArr); A_Index135++) {
                                if (Trim(funcArgsArr[A_Index135]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index136 = 0; A_Index136 < HTVM_Size(funcArgsArr); A_Index136++) {
                            if (Trim(funcArgsArr[A_Index136]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index137 = 0; A_Index137 < HTVM_Size(funcArgsArr); A_Index137++) {
                                if (Trim(funcArgsArr[A_Index137]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index138 = 0; A_Index138 < HTVM_Size(funcArgsArr); A_Index138++) {
                            if (Trim(funcArgsArr[A_Index138]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index139 = 0; A_Index139 < HTVM_Size(funcArgsArr); A_Index139++) {
                                if (Trim(funcArgsArr[A_Index139]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index140 = 0; A_Index140 < HTVM_Size(funcArgsArr); A_Index140++) {
                            if (Trim(funcArgsArr[A_Index140]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index141 = 0; A_Index141 < HTVM_Size(funcArgsArr); A_Index141++) {
                                if (Trim(funcArgsArr[A_Index141]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if7_" + STR(ifCount7) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if7_" + STR(ifCount7) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend7" || Trim(A_LoopField39) == "endif7") {
            out += ".end_if7_" + STR(ifCount7) + ":" + Chr(10);
            ifCount7++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if8 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index142 = 0; A_Index142 < HTVM_Size(funcArgsArr); A_Index142++) {
                            if (Trim(funcArgsArr[A_Index142]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index143 = 0; A_Index143 < HTVM_Size(funcArgsArr); A_Index143++) {
                            if (Trim(funcArgsArr[A_Index143]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index144 = 0; A_Index144 < HTVM_Size(funcArgsArr); A_Index144++) {
                            if (Trim(funcArgsArr[A_Index144]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index145 = 0; A_Index145 < HTVM_Size(funcArgsArr); A_Index145++) {
                                if (Trim(funcArgsArr[A_Index145]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index146 = 0; A_Index146 < HTVM_Size(funcArgsArr); A_Index146++) {
                            if (Trim(funcArgsArr[A_Index146]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index147 = 0; A_Index147 < HTVM_Size(funcArgsArr); A_Index147++) {
                                if (Trim(funcArgsArr[A_Index147]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index148 = 0; A_Index148 < HTVM_Size(funcArgsArr); A_Index148++) {
                            if (Trim(funcArgsArr[A_Index148]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index149 = 0; A_Index149 < HTVM_Size(funcArgsArr); A_Index149++) {
                                if (Trim(funcArgsArr[A_Index149]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index150 = 0; A_Index150 < HTVM_Size(funcArgsArr); A_Index150++) {
                            if (Trim(funcArgsArr[A_Index150]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index151 = 0; A_Index151 < HTVM_Size(funcArgsArr); A_Index151++) {
                                if (Trim(funcArgsArr[A_Index151]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index152 = 0; A_Index152 < HTVM_Size(funcArgsArr); A_Index152++) {
                            if (Trim(funcArgsArr[A_Index152]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index153 = 0; A_Index153 < HTVM_Size(funcArgsArr); A_Index153++) {
                                if (Trim(funcArgsArr[A_Index153]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if8_" + STR(ifCount8) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if8_" + STR(ifCount8) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend8" || Trim(A_LoopField39) == "endif8") {
            out += ".end_if8_" + STR(ifCount8) + ":" + Chr(10);
            ifCount8++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "if9 ") {
            isNum = 0;
            str1 = Trim(StringTrimLeft(A_LoopField39, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index154 = 0; A_Index154 < HTVM_Size(funcArgsArr); A_Index154++) {
                            if (Trim(funcArgsArr[A_Index154]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    int isRhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index155 = 0; A_Index155 < HTVM_Size(funcArgsArr); A_Index155++) {
                            if (Trim(funcArgsArr[A_Index155]) == str3) {
                                isRhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str3 == "rax") {
                        out += "    mov x10, x0" + Chr(10);
                    }
                    else if (str3 == "A_Index") {
                        out += "    mov x10, x20" + Chr(10);
                    }
                    else if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    }
                    else if (isRhsStackVar == 1) {
                        out += "    ldr x10, =" + str3 + Chr(10);
                    } else {
                        out += "    ldr x10, =" + str3 + Chr(10);
                        out += "    ldr x10, [x10]" + Chr(10);
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ne .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jne .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jne .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index156 = 0; A_Index156 < HTVM_Size(funcArgsArr); A_Index156++) {
                            if (Trim(funcArgsArr[A_Index156]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index157 = 0; A_Index157 < HTVM_Size(funcArgsArr); A_Index157++) {
                                if (Trim(funcArgsArr[A_Index157]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.eq .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "je .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "je .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index158 = 0; A_Index158 < HTVM_Size(funcArgsArr); A_Index158++) {
                            if (Trim(funcArgsArr[A_Index158]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index159 = 0; A_Index159 < HTVM_Size(funcArgsArr); A_Index159++) {
                                if (Trim(funcArgsArr[A_Index159]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.le .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jle .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jle .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index160 = 0; A_Index160 < HTVM_Size(funcArgsArr); A_Index160++) {
                            if (Trim(funcArgsArr[A_Index160]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index161 = 0; A_Index161 < HTVM_Size(funcArgsArr); A_Index161++) {
                                if (Trim(funcArgsArr[A_Index161]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.ge .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jge .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jge .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index162 = 0; A_Index162 < HTVM_Size(funcArgsArr); A_Index162++) {
                            if (Trim(funcArgsArr[A_Index162]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index163 = 0; A_Index163 < HTVM_Size(funcArgsArr); A_Index163++) {
                                if (Trim(funcArgsArr[A_Index163]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.lt .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jl .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jl .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = RegExMatch(str3, "^\\d+$");
                if (is_arm == 1) {
                    int isLhsStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index164 = 0; A_Index164 < HTVM_Size(funcArgsArr); A_Index164++) {
                            if (Trim(funcArgsArr[A_Index164]) == str2) {
                                isLhsStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (str2 == "rax") {
                        out += "    mov x9, x0" + Chr(10);
                    }
                    else if (str2 == "A_Index") {
                        out += "    mov x9, x20" + Chr(10);
                    }
                    else if (isLhsStackVar == 1) {
                        out += "    ldr x9, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10);
                        out += "    ldr x9, [x9]" + Chr(10);
                    }
                    if (isNum == 1) {
                        out += "    mov x10, #" + str3 + Chr(10);
                    } else {
                        int isRhsStackVar = 0;
                        if (inFunc == 1) {
                            for (int A_Index165 = 0; A_Index165 < HTVM_Size(funcArgsArr); A_Index165++) {
                                if (Trim(funcArgsArr[A_Index165]) == str3) {
                                    isRhsStackVar = 1;
                                    break;
                                }
                            }
                        }
                        if (str3 == "rax") {
                            out += "    mov x10, x0" + Chr(10);
                        }
                        else if (str3 == "A_Index") {
                            out += "    mov x10, x20" + Chr(10);
                        }
                        else if (isRhsStackVar == 1) {
                            out += "    ldr x10, =" + str3 + Chr(10);
                        } else {
                            out += "    ldr x10, =" + str3 + Chr(10);
                            out += "    ldr x10, [x10]" + Chr(10);
                        }
                    }
                    out += "    cmp x9, x10" + Chr(10);
                    out += "    b.gt .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    if (isNum == 1) {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, " + str3 + Chr(10);
                        out += "jg .end_if9_" + STR(ifCount9) + Chr(10);
                    } else {
                        out += "mov rax, [" + str2 + "]" + Chr(10);
                        out += "cmp rax, [" + str3 + "]" + Chr(10);
                        out += "jg .end_if9_" + STR(ifCount9) + Chr(10);
                    }
                }
            }
        }
        else if (Trim(A_LoopField39) == "ifend9" || Trim(A_LoopField39) == "endif9") {
            out += ".end_if9_" + STR(ifCount9) + ":" + Chr(10);
            ifCount9++;
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 5) == "func ") {
            // --- YOUR PARSING LOGIC IS UNCHANGED ---
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 5));
            str2 = "";
            std::vector<std::string> items166 = LoopParseFunc(str1);
            for (size_t A_Index166 = 0; A_Index166 < items166.size(); A_Index166++) {
                std::string A_LoopField166 = items166[A_Index166 - 0];
                if (A_LoopField166 == "(") {
                    break;
                } else {
                    str2 += A_LoopField166;
                }
            }
            str2 = Trim(str2);
            localVarNum = 1;
            inFunc = 1;
            funcArgsCount = (countChars(str1, ",") + 1);
            int1 = 0;
            int2 = 0;
            if (InStr(str1, ",") || InStr(str1, "()") == false) {
                funcArgsArr = {};
                std::vector<std::string> items167 = LoopParseFunc(str1);
                for (size_t A_Index167 = 0; A_Index167 < items167.size(); A_Index167++) {
                    std::string A_LoopField167 = items167[A_Index167 - 0];
                    if (A_LoopField167 == ")") {
                        break;
                    }
                    if (int1 == 1 && A_LoopField167 != "," && A_LoopField167 != " ") {
                        funcArgsArr[int2] = funcArgsArr[int2] + A_LoopField167;
                    }
                    if (A_LoopField167 == ",") {
                        HTVM_Append(funcArgsArr, "");
                        int2++;
                    }
                    if (A_LoopField167 == "(") {
                        HTVM_Append(funcArgsArr, "");
                        int1 = 1;
                    }
                }
            }
            funcName = str2;
            // --- ARCHITECTURE-SPECIFIC CODE GENERATION ---
            if (is_arm == 1) {
                out += funcName + ":" + Chr(10);
                out += "    stp x29, x30, [sp, #-16]!" + Chr(10);
                out += "    mov x29, sp" + Chr(10);
                // --- NEW REQUIRED LOGIC FOR ARM ---
                // Calculate space needed for local vars AND for saving register arguments
                int args_to_save = HTVM_Size(funcArgsArr) > 8 ? 8 : HTVM_Size(funcArgsArr);
                int total_space = ( (localVarNum * 8) + (args_to_save * 8) + 15 ) & ~15;
                out += "    sub sp, sp, #" + STR(total_space) + Chr(10);
                // Save the incoming register arguments to the stack
                for (int A_Index168 = 0; A_Index168 < args_to_save; A_Index168++) {
                    int offset = (A_Index168 * 8);
                    out += "    str x" + STR(A_Index168) + ", [sp, #" + STR(offset) + "]" + Chr(10);
                }
                // --- END NEW LOGIC ---
            } else {
                out += str2 + ":" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "sub rsp, " + STR(8 + (localVarNum * 8)) + Chr(10);
            }
        }
        else if (Trim(A_LoopField39) == "funcend" || Trim(A_LoopField39) == "endfunc") {
            if (is_arm == 1) {
                out += "." + funcName + "_return:" + Chr(10);
                out += "    mov sp, x29" + Chr(10);
                out += "    ldp x29, x30, [sp], #16" + Chr(10);
                out += "    ret" + Chr(10);
            } else {
                out += "." + funcName + "_return:" + Chr(10) + "add rsp, " + STR(8 + (localVarNum * 8)) + Chr(10) + "pop rbp" + Chr(10) + "ret" + Chr(10);
            }
            funcCount++;
        }
        else if (Trim(A_LoopField39) == "return") {
            if (is_arm == 1) {
                out += "    b ." + funcName + "_return" + Chr(10);
            } else {
                out += "jmp ." + funcName + "_return" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "return ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 7));
            if (is_arm == 1) {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x0, #" + str1 + Chr(10);
                }
                else if (str1 == "rax") {
                    // Value is already in x0, no instruction needed.
                }
                else if (str1 == "A_Index") {
                    out += "    mov x0, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index169 = 0; A_Index169 < HTVM_Size(funcArgsArr); A_Index169++) {
                            if (Trim(funcArgsArr[A_Index169]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x0, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                    }
                }
                out += "    b ." + funcName + "_return" + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "mov rax, " + str1 + Chr(10) + "jmp ." + funcName + "_return" + Chr(10);
                } else {
                    out += "mov rax, [" + str1 + "]" + Chr(10) + "jmp ." + funcName + "_return" + Chr(10);
                }
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 4) == "arr ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField39), 4));
            if (is_arm == 1) {
                // A struct of 3 quadwords is 24 bytes
                arrBss += ".lcomm " + Trim(str1) + ", 24" + Chr(10);
            } else {
                arrBss += "    " + Trim(str1) + " rq 3" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".add ")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            str2 = Trim(StrSplit(A_LoopField39, ".add", 2));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str1 + Chr(10);
                if (RegExMatch(str2, "^\\d+$")) {
                    out += "    mov x1, #" + str2 + Chr(10);
                }
                else if (SubStr(str2, 1, 1) == "'") {
                    out += "    mov x1, #" + str2 + Chr(10);
                }
                else if (str2 == "rax") {
                    out += "    mov x1, x0" + Chr(10);
                }
                else if (str2 == "A_Index") {
                    out += "    mov x1, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index170 = 0; A_Index170 < HTVM_Size(funcArgsArr); A_Index170++) {
                            if (Trim(funcArgsArr[A_Index170]) == str2) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x1, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                    }
                }
                out += "    bl array_append" + Chr(10);
            } else {
                if (RegExMatch(str2, "^\\d+$")) {
                    str3 = "mov rsi, " + str2 + Chr(10) + "mov rdi, " + str1 + Chr(10);
                } else {
                    str3 = "mov rsi, [" + str2 + "]" + Chr(10) + "mov rdi, " + str1 + Chr(10);
                }
                out += str3 + Chr(10) + "call array_append" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".pop")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str1 + Chr(10);
                out += "    bl array_pop" + Chr(10);
            } else {
                out += "mov rdi, " + str1 + Chr(10) + "call array_pop" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".clear")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str1 + Chr(10);
                out += "    bl array_clear" + Chr(10);
            } else {
                out += "mov rdi, " + str1 + Chr(10) + "call array_clear" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".copy ")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            str2 = Trim(StrSplit(A_LoopField39, ".copy", 2));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str1 + Chr(10);
                out += "    ldr x1, =" + str2 + Chr(10);
                out += "    bl array_copy" + Chr(10);
            } else {
                out += "mov rdi, " + str1 + Chr(10) + "mov rsi, " + str2 + Chr(10) + "call array_copy" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".size")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            if (is_arm == 1) {
                out += "    ldr x9, =" + str1 + Chr(10);
                out += "    ldr x0, [x9, #DynamicArray_size]" + Chr(10);
            } else {
                out += "mov rax, [" + str1 + " + DynamicArray.size]" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".index ")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            str2 = Trim(StrSplit(A_LoopField39, ".index", 2));
            if (is_arm == 1) {
                if (RegExMatch(str2, "^\\d+$")) {
                    out += "    mov x10, #" + str2 + Chr(10);
                }
                else if (str2 == "A_Index") {
                    out += "    mov x10, x20" + Chr(10);
                }
                else if (str2 == "rax") {
                    out += "    mov x10, x0" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index171 = 0; A_Index171 < HTVM_Size(funcArgsArr); A_Index171++) {
                            if (Trim(funcArgsArr[A_Index171]) == str2) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x10, =" + str2 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str2 + Chr(10) + "    ldr x10, [x9]" + Chr(10);
                    }
                }
                out += "    ldr x9, =" + str1 + Chr(10);
                out += "    ldr x9, [x9, #DynamicArray_pointer]" + Chr(10);
                out += "    ldr x0, [x9, x10, lsl #3]" + Chr(10);
            } else {
                if (RegExMatch(str2, "^\\d+$")) {
                    str3 = "mov rcx, " + str2 + Chr(10);
                } else {
                    str3 = "mov rcx, [" + str2 + "]" + Chr(10);
                }
                out += str3 + Chr(10) + "mov rbx, [" + str1 + " + DynamicArray.pointer]" + Chr(10) + "mov rax, [rbx + rcx*8]" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".compile ")) {
            // arrName.compile outArr
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            str2 = Trim(StrSplit(A_LoopField39, ".compile", 2));
            isDotCompile = 1;
            out += "mov rdi, " + str1 + Chr(10) + "call array_pack_to_bytes" + Chr(10) + "mov [source_ptr], rax" + Chr(10) + "mov rdi, [source_ptr]" + Chr(10) + "call compiler_c" + Chr(10) + "mov [asm_code_ptr], rax" + Chr(10) + "mov rdi, " + str2 + Chr(10) + "mov rsi, [asm_code_ptr]" + Chr(10) + "call array_unpack_from_bytes" + Chr(10) + "mov rdi, [source_ptr]" + Chr(10) + "mov rsi, [source_ptr_size]" + Chr(10) + "call free_packed_string" + Chr(10) + "mov rdi, [asm_code_ptr]" + Chr(10) + "call free_string_c" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 5) == "goto ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 5));
            if (is_arm == 1) {
                out += "    b .__HTLL_HTLL_" + str1 + Chr(10);
            } else {
                out += "jmp .__HTLL_HTLL_" + str1 + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 5) == "togo ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 5));
            // This is architecture-independent, no 'if (is_arm)' needed.
            out += ".__HTLL_HTLL_" + str1 + ":" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 7) == "sleep, ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 7));
            if (is_arm == 1) {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "    mov x0, #" + str1 + Chr(10);
                }
                else if (str1 == "rax") {
                    // Value is already in x0, no instruction is needed
                }
                else if (str1 == "A_Index") {
                    out += "    mov x0, x20" + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index172 = 0; A_Index172 < HTVM_Size(funcArgsArr); A_Index172++) {
                            if (Trim(funcArgsArr[A_Index172]) == str1) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x0, =" + str1 + Chr(10);
                    } else {
                        out += "    ldr x9, =" + str1 + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                    }
                }
                out += "    bl sleep_ms" + Chr(10);
            } else {
                if (RegExMatch(str1, "^\\d+$")) {
                    out += "mov rdi, " + str1 + Chr(10);
                }
                else if (str1 == "rax") {
                    out += "mov rdi, rax" + Chr(10);
                }
                else if (str1 == "A_Index") {
                    out += "mov rdi, r13" + Chr(10);
                } else {
                    out += "mov rdi, [" + str1 + "]" + Chr(10);
                }
                out += "call sleep_ms" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 13) == "fileread_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 13));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str3 + Chr(10) + "    bl array_pack_to_bytes" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    str x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    str x1, [x9]" + Chr(10);
                out += "    ldr x0, =" + str2 + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                out += "    bl fileread_from_ptr" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                out += "    bl free_packed_string" + Chr(10);
            } else {
                out += "mov rdi, " + str3 + Chr(10) + "call array_pack_to_bytes" + Chr(10);
                out += "mov [filename_ptr], rax" + Chr(10) + "mov [filename_ptr_size], rdx" + Chr(10);
                out += "mov rdi, " + str2 + Chr(10) + "mov rsi, [filename_ptr]" + Chr(10) + "call fileread_from_ptr" + Chr(10);
                out += "mov rdi, [filename_ptr]" + Chr(10) + "mov rsi, [filename_ptr_size]" + Chr(10) + "call free_packed_string" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 15) == "fileappend_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 15));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str2 + Chr(10) + "    bl array_pack_to_bytes" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    str x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    str x1, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                out += "    ldr x1, =" + str3 + Chr(10);
                out += "    bl fileappend_from_ptr" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                out += "    bl free_packed_string" + Chr(10);
            } else {
                out += "mov rdi, " + str2 + Chr(10) + "call array_pack_to_bytes" + Chr(10);
                out += "mov [filename_ptr], rax" + Chr(10) + "mov [filename_ptr_size], rdx" + Chr(10);
                out += "mov rdi, [filename_ptr]" + Chr(10) + "mov rsi, " + str3 + Chr(10) + "call fileappend_from_ptr" + Chr(10);
                out += "mov rdi, [filename_ptr]" + Chr(10) + "mov rsi, [filename_ptr_size]" + Chr(10) + "call free_packed_string" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 15) == "filedelete_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 15));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str1 + Chr(10) + "    bl array_pack_to_bytes" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    str x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    str x1, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                out += "    bl filedelete_from_ptr" + Chr(10);
                out += "    ldr x9, =filename_ptr" + Chr(10) + "    ldr x0, [x9]" + Chr(10);
                out += "    ldr x9, =filename_ptr_size" + Chr(10) + "    ldr x1, [x9]" + Chr(10);
                out += "    bl free_packed_string" + Chr(10);
            } else {
                out += "mov rdi, " + str1 + Chr(10) + "call array_pack_to_bytes" + Chr(10);
                out += "mov [filename_ptr], rax" + Chr(10) + "mov [filename_ptr_size], rdx" + Chr(10);
                out += "mov rdi, [filename_ptr]" + Chr(10) + "call filedelete_from_ptr" + Chr(10);
                out += "mov rdi, [filename_ptr]" + Chr(10) + "mov rsi, [filename_ptr_size]" + Chr(10) + "call free_packed_string" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 9) == "fileread ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 9));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            if (is_arm == 1) {
                dot_data_print_temp_strings += path_label + ": .string " + str3 + Chr(10);
                out += "    ldr x0, =" + str2 + Chr(10);
                out += "    ldr x1, =" + path_label + Chr(10);
                out += "    bl file_read" + Chr(10);
            } else {
                dot_data_print_temp_strings += path_label + " db " + str3 + ", 0" + Chr(10);
                out += "mov rdi, " + str2 + Chr(10) + "mov rsi, " + path_label + Chr(10) + "call file_read" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 11) == "fileappend ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 11));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            if (is_arm == 1) {
                dot_data_print_temp_strings += path_label + ": .string " + str2 + Chr(10);
                out += "    ldr x0, =" + path_label + Chr(10);
                out += "    ldr x1, =" + str3 + Chr(10);
                out += "    bl file_append" + Chr(10);
            } else {
                dot_data_print_temp_strings += path_label + " db " + str2 + ", 0" + Chr(10);
                out += "mov rdi, " + path_label + Chr(10) + "mov rsi, " + str3 + Chr(10) + "call file_append" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField39), 1, 11) == "filedelete ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 11));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            if (is_arm == 1) {
                dot_data_print_temp_strings += path_label + ": .string " + str1 + Chr(10);
                out += "    ldr x0, =" + path_label + Chr(10);
                out += "    bl file_delete" + Chr(10);
            } else {
                dot_data_print_temp_strings += path_label + " db " + str1 + ", 0" + Chr(10);
                out += "mov rdi, " + path_label + Chr(10) + "call file_delete" + Chr(10);
            }
        }
        else if (SubStr(A_LoopField39, 1, 6) == "input ") {
            str1 = Trim(StringTrimLeft(A_LoopField39, 6));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            if (is_arm == 1) {
                out += "    ldr x0, =" + str2 + Chr(10);
                out += "    ldr x1, =" + str3 + Chr(10);
                out += "    bl get_user_input" + Chr(10);
            } else {
                out += "mov rdi, " + str2 + Chr(10) + "mov rsi, " + str3 + Chr(10) + "call get_user_input" + Chr(10);
            }
        }
        else if (InStr(A_LoopField39, ".set ")) {
            str1 = Trim(StrSplit(A_LoopField39, ".", 1));
            str2 = Trim(StringTrimLeft(A_LoopField39, StrLen(str1) + 5));
            str_index = Trim(StrSplit(str2, ",", 1));
            str_value = Trim(StrSplit(str2, ",", 2));
            if (is_arm == 1) {
                // 1. Load Array Pointer into x9
                out += "    ldr x9, =" + str1 + Chr(10);
                out += "    ldr x9, [x9, #DynamicArray_pointer]" + Chr(10);
                // 2. Load INDEX into x10
                if (str_index == "A_Index") {
                    out += "    mov x10, x20" + Chr(10);
                }
                else if (str_index == "rax") {
                    out += "    mov x10, x0" + Chr(10);
                }
                else if (RegExMatch(str_index, "^\\d+$")) {
                    out += "    mov x10, #" + str_index + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index173 = 0; A_Index173 < HTVM_Size(funcArgsArr); A_Index173++) {
                            if (Trim(funcArgsArr[A_Index173]) == str_index) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x10, =" + str_index + Chr(10);
                    } else {
                        out += "    ldr x11, =" + str_index + Chr(10);
                        out += "    ldr x10, [x11]" + Chr(10);
                    }
                }
                // 3. Load VALUE into x11
                if (str_value == "A_Index") {
                    out += "    mov x11, x20" + Chr(10);
                }
                else if (str_value == "rax") {
                    out += "    mov x11, x0" + Chr(10);
                }
                else if (RegExMatch(str_value, "^\\d+$")) {
                    out += "    mov x11, #" + str_value + Chr(10);
                } else {
                    int isStackVar = 0;
                    if (inFunc == 1) {
                        for (int A_Index174 = 0; A_Index174 < HTVM_Size(funcArgsArr); A_Index174++) {
                            if (Trim(funcArgsArr[A_Index174]) == str_value) {
                                isStackVar = 1;
                                break;
                            }
                        }
                    }
                    if (isStackVar == 1) {
                        out += "    ldr x11, =" + str_value + Chr(10);
                    } else {
                        out += "    ldr x12, =" + str_value + Chr(10);
                        out += "    ldr x11, [x12]" + Chr(10);
                    }
                }
                // 4. Store x11 into Array[x10]
                out += "    str x11, [x9, x10, lsl #3]" + Chr(10);
            } else {
                // --- EXISTING x86 CODE (Unchanged) ---
                out += "mov rbx, [" + str1 + " + DynamicArray.pointer]" + Chr(10);
                if (RegExMatch(str_index, "^\\d+$")) {
                    out += "mov rcx, " + str_index + Chr(10);
                } else {
                    out += "mov rcx, [" + str_index + "]" + Chr(10);
                }
                if (RegExMatch(str_value, "^\\d+$")) {
                    out += "mov rsi, " + str_value + Chr(10);
                } else {
                    out += "mov rsi, [" + str_value + "]" + Chr(10);
                }
                out += "mov [rbx + rcx*8], rsi" + Chr(10);
            }
        }
        else if (Trim(A_LoopField39) == "rax++") {
            if (is_arm == 1) {
                out += "    add x0, x0, #1" + Chr(10);
            } else {
                out += "inc rax" + Chr(10);
            }
        }
        else if (Trim(A_LoopField39) == "rax--") {
            if (is_arm == 1) {
                out += "    sub x0, x0, #1" + Chr(10);
            } else {
                out += "dec rax" + Chr(10);
            }
        }
        else if (SubStrLastChars(Trim(A_LoopField39), 1) == ")") {
            // --- YOUR PARSING LOGIC IS UNCHANGED ---
            str1 = StringTrimRight(Trim(A_LoopField39), 1);
            str2 = StrSplit(str1, "(", 1);
            std::vector<std::string> items175 = LoopParseFunc(str2, " ");
            for (size_t A_Index175 = 0; A_Index175 < items175.size(); A_Index175++) {
                std::string A_LoopField175 = items175[A_Index175 - 0];
                str3 = A_LoopField175;
            }
            str2 = str3;
            str3 = StrSplit(str1, "(", 2);
            str4 = "";
            int5 = 0;
            int1 = 0;
            if (Trim(str3) == "") {
                int1 = 1;
            }
            if (is_arm == 1) {
                // --- AArch64 CODE GENERATION ---
                int stack_arg_count = 0;
                if (int1 == 0) {
                    std::vector<std::string> items176 = LoopParseFunc(str3, ", ");
                    for (size_t A_Index176 = 0; A_Index176 < items176.size(); A_Index176++) {
                        std::string A_LoopField176 = items176[A_Index176 - 0];
                        if (A_Index176 > 8) {
                            stack_arg_count++;
                        }
                    }
                    if (stack_arg_count > 0) {
                        str4 += "    sub sp, sp, #" + STR(stack_arg_count * 8) + Chr(10);
                    }
                    std::vector<std::string> items177 = LoopParseFunc(str3, ", ");
                    for (size_t A_Index177 = 0; A_Index177 < items177.size(); A_Index177++) {
                        std::string A_LoopField177 = items177[A_Index177 - 0];
                        int current_arg_num = A_Index177;
                        if (current_arg_num < 8) {
                            if (RegExMatch(A_LoopField177, "^\\d+$")) {
                                str4 += "    mov x" + STR(current_arg_num) + ", #" + A_LoopField177 + Chr(10);
                            }
                            else if (A_LoopField177 == "rax") {
                                str4 += "    mov x" + STR(current_arg_num) + ", x0" + Chr(10);
                            }
                            else if (A_LoopField177 == "A_Index") {
                                str4 += "    mov x" + STR(current_arg_num) + ", x20" + Chr(10);
                            } else {
                                int isStackVar = 0;
                                if (inFunc == 1) {
                                    for (int A_Index178 = 0; A_Index178 < HTVM_Size(funcArgsArr); A_Index178++) {
                                        if (Trim(funcArgsArr[A_Index178]) == A_LoopField39) {
                                            isStackVar = 1;
                                            break;
                                        }
                                    }
                                }
                                if (isStackVar == 1) {
                                    str4 += "    ldr x" + STR(current_arg_num) + ", =" + A_LoopField177 + Chr(10);
                                } else {
                                    str4 += "    ldr x9, =" + A_LoopField177 + Chr(10) + "    ldr x" + STR(current_arg_num) + ", [x9]" + Chr(10);
                                }
                            }
                        } else {
                            int stack_offset = (current_arg_num - 8) * 8;
                            if (RegExMatch(A_LoopField177, "^\\d+$")) {
                                str4 += "    mov x9, #" + A_LoopField177 + Chr(10);
                            }
                            else if (A_LoopField177 == "rax") {
                                str4 += "    mov x9, x0" + Chr(10);
                            }
                            else if (A_LoopField177 == "A_Index") {
                                str4 += "    mov x9, x20" + Chr(10);
                            } else {
                                int isStackVar = 0;
                                if (inFunc == 1) {
                                    for (int A_Index179 = 0; A_Index179 < HTVM_Size(funcArgsArr); A_Index179++) {
                                        if (Trim(funcArgsArr[A_Index179]) == A_LoopField177) {
                                            isStackVar = 1;
                                            break;
                                        }
                                    }
                                }
                                if (isStackVar == 1) {
                                    str4 += "    ldr x9, =" + A_LoopField177 + Chr(10);
                                } else {
                                    str4 += "    ldr x10, =" + A_LoopField177 + Chr(10) + "    ldr x9, [x10]" + Chr(10);
                                }
                            }
                            str4 += "    str x9, [sp, #" + STR(stack_offset) + "]" + Chr(10);
                        }
                    }
                }
                out += str4 + "    bl " + str2 + Chr(10);
                if (stack_arg_count > 0) {
                    out += "    add sp, sp, #" + STR(stack_arg_count * 8) + Chr(10);
                }
            } else {
                // --- YOUR EXISTING X86 CODE - UNCHANGED ---
                std::vector<std::string> items180 = LoopParseFunc(str3, ", ");
                for (size_t A_Index180 = 0; A_Index180 < items180.size(); A_Index180++) {
                    std::string A_LoopField180 = items180[A_Index180 - 0];
                    if (RegExMatch(A_LoopField180, "^\\d+$")) {
                        str4 += "push " + A_LoopField180 + Chr(10);
                    } else {
                        str4 += "push qword [" + A_LoopField180 + "]" + Chr(10);
                    }
                    int5++;
                }
                if (int1 == 0) {
                    out += str4 + "call " + str2 + Chr(10) + "add rsp, " + STR(8 * int5) + Chr(10);
                } else {
                    out += str4 + "call " + str2 + Chr(10);
                }
            }
        } else {
            out += A_LoopField39 + Chr(10);
        }
        // Place this AFTER your main compilation loop finishes generating 'out'
        for (int A_Index181 = 0; A_Index181 < HTVM_Size(funcArgsArr); A_Index181++) {
            if (is_arm == 1) {
                out = RegExReplace(out, Chr(61) + "\\b" + Trim(funcArgsArr[A_Index181]) + "\\b", "[sp, #" + STR(A_Index181 * 8) + "]");
            } else {
                // Your existing x86 formula
                out = RegExReplace(out, "\\b" + Trim(funcArgsArr[A_Index181]) + "\\b", "rbp + " + STR(8 + ( (HTVM_Size(funcArgsArr) - A_Index181) * 8 )));
            }
        }
        // ... after the main compilation loop ...
        if (is_arm == 0) {
            out = StrReplace(out, "']", "'");
            out = StrReplace(out, "['", "'");
            out = StrReplace(out, "push qword []", "");
            out = StrReplace(out, "[A_Index]", "r13");
        }
        // This replacement is likely a mistake in your original code, as it would
        // turn `mov rax, [my_var]` into `mov rax, my_var`, which is a different instruction.
        // I'll keep it here but comment it out as a warning.
        // codeOUT := StrReplace(codeOUT, " [rax]", " rax")
    }
    code = out;
    std::vector<std::string> allFuncCALLS;
    std::vector<std::string> allFuncCALLS_alredy;
    std::string code_TEMP = code + Chr(10) + main_syntax;
    std::vector<std::string> items182 = LoopParseFunc(code_TEMP, "\n", "\r");
    for (size_t A_Index182 = 0; A_Index182 < items182.size(); A_Index182++) {
        std::string A_LoopField182 = items182[A_Index182 - 0];
        if (is_arm == 1) {
            if (SubStr(Trim(A_LoopField182), 1, 3) == "bl " || SubStr(Trim(A_LoopField182), 1, 2) == "b ") {
                HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(A_LoopField182), 2)));
            }
        } else {
            if (SubStr(Trim(A_LoopField182), 1, 5) == "call " || SubStr(Trim(A_LoopField182), 1, 4) == "jmp ") {
                HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(A_LoopField182), 4)));
            }
        }
    }
    std::string HTLL_Libs_x86_new = Chr(10);
    std::string temp_funcName = "";
    int temp_in_funcName = 0;
    int canWeGetFunc = 0;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    std::vector<std::string> items183 = LoopParseFunc(HTLL_Libs_x86, "\n", "\r");
    for (size_t A_Index183 = 0; A_Index183 < items183.size(); A_Index183++) {
        std::string A_LoopField183 = items183[A_Index183 - 0];
        if (InStr(A_LoopField183, "$$$$") == false && InStr(A_LoopField183, "%%%%") == false && temp_in_funcName == 1 && canWeGetFunc == 1) {
            HTLL_Libs_x86_new += A_LoopField183 + Chr(10);
        }
        if (InStr(A_LoopField183, "$$$$")) {
            temp_funcName = StringTrimLeft(StringTrimRight(Trim(A_LoopField183), 4), 4);
            temp_in_funcName = 1;
            canWeGetFunc = 0;
            for (int A_Index184 = 0; A_Index184 < HTVM_Size(allFuncCALLS); A_Index184++) {
                if (temp_funcName == allFuncCALLS[A_Index184]) {
                    canWeGetFunc = 1;
                    HTVM_Append(allFuncCALLS_alredy, temp_funcName);
                    break;
                }
            }
        }
        else if (InStr(A_LoopField183, "%%%%")) {
            temp_in_funcName = 0;
        }
    }
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    allFuncCALLS = {};
    int anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 0;
    std::string ALoopField = "";
    std::vector<std::string> items185 = LoopParseFunc(HTLL_Libs_x86_new, "\n", "\r");
    for (size_t A_Index185 = 0; A_Index185 < items185.size(); A_Index185++) {
        std::string A_LoopField185 = items185[A_Index185 - 0];
        ALoopField = A_LoopField185;
        if (is_arm == 1) {
            if (SubStr(Trim(A_LoopField185), 1, 3) == "bl " || SubStr(Trim(A_LoopField185), 1, 2) == "b ") {
                anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 1;
                for (int A_Index186 = 0; A_Index186 < HTVM_Size(allFuncCALLS_alredy); A_Index186++) {
                    if (allFuncCALLS_alredy[A_Index186] == Trim(StringTrimLeft(Trim(ALoopField), 2))) {
                        anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 0;
                    }
                }
                if (anotherVar_INT_HELP_BUILD_IN_FUNCS_INT == 1) {
                    HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(ALoopField), 4)));
                }
            }
        } else {
            if (SubStr(Trim(A_LoopField185), 1, 5) == "call " || SubStr(Trim(A_LoopField185), 1, 4) == "jmp ") {
                anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 1;
                for (int A_Index187 = 0; A_Index187 < HTVM_Size(allFuncCALLS_alredy); A_Index187++) {
                    if (allFuncCALLS_alredy[A_Index187] == Trim(StringTrimLeft(Trim(ALoopField), 4))) {
                        anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 0;
                    }
                }
                if (anotherVar_INT_HELP_BUILD_IN_FUNCS_INT == 1) {
                    HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(ALoopField), 4)));
                }
            }
        }
    }
    temp_funcName = "";
    temp_in_funcName = 0;
    canWeGetFunc = 0;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    std::vector<std::string> items188 = LoopParseFunc(HTLL_Libs_x86, "\n", "\r");
    for (size_t A_Index188 = 0; A_Index188 < items188.size(); A_Index188++) {
        std::string A_LoopField188 = items188[A_Index188 - 0];
        if (InStr(A_LoopField188, "$$$$") == false && InStr(A_LoopField188, "%%%%") == false && temp_in_funcName == 1 && canWeGetFunc == 1) {
            HTLL_Libs_x86_new += A_LoopField188 + Chr(10);
        }
        if (InStr(A_LoopField188, "$$$$")) {
            temp_funcName = StringTrimLeft(StringTrimRight(Trim(A_LoopField188), 4), 4);
            temp_in_funcName = 1;
            canWeGetFunc = 0;
            for (int A_Index189 = 0; A_Index189 < HTVM_Size(allFuncCALLS); A_Index189++) {
                if (temp_funcName == allFuncCALLS[A_Index189]) {
                    canWeGetFunc = 1;
                    break;
                }
            }
        }
        else if (InStr(A_LoopField188, "%%%%")) {
            temp_in_funcName = 0;
        }
    }
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    // =========================================================================
    // FINAL FASM OBJECT FILE ASSEMBLY - THE TRUE BLUEPRINT
    // =========================================================================
    dot_data += dot_data_print_temp_strings + Chr(10);
    dot_data += dot_data_ints + Chr(10);
    dot_bss += dot_bss_str + Chr(10);
    // ... inside your 'where:' block ...
    std::string fasm_header = "";
    // --- FASM OBJECT FILE HEADER & LINKER DIRECTIVES ---
    if (is_arm == 1) {
        // --- ARM GNU AS HEADER & CONSTANTS ---
        // NOTE: GNU AS doesn't have a direct 'format' or 'entry' like FASM.
        // Those are handled by the linker (ld). This header sets up constants.
        fasm_header = "; ARM AArch64 Header for GNU AS" + Chr(10);
        fasm_header += ".equ SYS_READ, 63" + Chr(10);
        fasm_header += ".equ SYS_WRITE, 64" + Chr(10);
        fasm_header += ".equ SYS_OPENAT, 56" + Chr(10);
        fasm_header += ".equ SYS_CLOSE, 57" + Chr(10);
        fasm_header += ".equ SYS_MMAP, 222" + Chr(10);
        fasm_header += ".equ SYS_MUNMAP, 215" + Chr(10);
        fasm_header += ".equ SYS_EXIT, 93" + Chr(10);
        fasm_header += ".equ SYS_NANOSLEEP, 101" + Chr(10);
        fasm_header += ".equ SYS_UNLINKAT, 35" + Chr(10);
        fasm_header += Chr(10);
        fasm_header += ".equ AT_FDCWD, -100" + Chr(10);
        fasm_header += ".equ O_RDONLY, 0" + Chr(10);
        fasm_header += ".equ O_CREAT_WRONLY_TRUNC, 0101 | 01000 | 0100" + Chr(10);
        fasm_header += ".equ PROT_READ_WRITE, 0x1 | 0x2" + Chr(10);
        fasm_header += ".equ MAP_PRIVATE_ANONYMOUS, 0x20 | 0x02" + Chr(10);
        fasm_header += Chr(10);
        fasm_header += ".global _start" + Chr(10);
        fasm_header += Chr(10);
        // --- GNU AS STRUCTURE DEFINITION (using .equ) ---
        fasm_header += ".equ DynamicArray_pointer, 0" + Chr(10);
        fasm_header += ".equ DynamicArray_size, 8" + Chr(10);
        fasm_header += ".equ DynamicArray_capacity, 16" + Chr(10);
        fasm_header += ".equ INITIAL_CAPACITY, 4" + Chr(10);
    } else {
        // --- THIS IS YOUR EXISTING x86 CODE, UNCHANGED ---
        if (isDotCompile == 1) {
            fasm_header = "format ELF64" + Chr(10);
            fasm_header += "public _start" + Chr(10);
        } else {
            if (ring0 == 0) {
                fasm_header = "format ELF64 executable 3" + Chr(10);
                fasm_header += "entry _start" + Chr(10);
            } else {
                fasm_header = "use64" + Chr(10);
                fasm_header += "org 0x9400" + Chr(10);
                fasm_header += "jmp _start" + Chr(10);
            }
        }
        if (isDotCompile == 1) {
            fasm_header += "extrn compiler_c" + Chr(10);
            fasm_header += "extrn free_string_c" + Chr(10);
        }
        fasm_header += Chr(10);
        // --- FASM STRUCTURE DEFINITION ---
        fasm_header += "    DynamicArray.pointer  = 0" + Chr(10) + "    DynamicArray.size     = 8" + Chr(10) + "    DynamicArray.capacity = 16" + Chr(10) + "    sizeof.DynamicArray   = 24" + Chr(10) + Chr(10);
    }
    // Use simple section names WITH the correct ELF flags for object files.
    std::string upCode = "";
    // --- FASM BODY ---
    if (is_arm == 1) {
        // --- ARM GNU AS SECTIONS & DATA ---
        upCode = ".data" + Chr(10);
        upCode += "SCALE_FACTOR:      .quad 1000000" + Chr(10);
        upCode += "HTLL_dot:               .string " + Chr(34) + "." + Chr(34) + Chr(10);
        upCode += "HTLL_minus_sign:        .string " + Chr(34) + "-" + Chr(34) + Chr(10);
        upCode += "HTLL_nl:                .string " + Chr(34) + "\\n" + Chr(34) + Chr(10);
        upCode += dot_data;
        upCode += Chr(10) + ".bss" + Chr(10);
        upCode += ".lcomm HTLL_input_buffer, 256" + Chr(10);
        upCode += ".lcomm HTLL_file_read_buffer, 4096" + Chr(10);
        upCode += ".lcomm filename_ptr_size, 8" + Chr(10);
        upCode += ".lcomm source_ptr, 8" + Chr(10);
        upCode += ".lcomm source_ptr_size, 8" + Chr(10);
        upCode += ".lcomm args_array, 24" + Chr(10);
        upCode += ".lcomm filename_ptr, 8" + Chr(10);
        upCode += ".lcomm asm_code_ptr, 8" + Chr(10);
        upCode += arrBss + dot_bss;
        upCode += Chr(10) + ".text" + Chr(10);
        upCode += HTLL_Libs_x86_new;
    } else {
        if (isDotCompile == 1) {
            upCode = "section '.data' writeable" + Chr(10) + "    SCALE_FACTOR   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY = 2" + Chr(10) + "    print_buffer   rb 21" + Chr(10) + "    dot            db " + Chr(39) + "." + Chr(39) + Chr(10) + "    minus_sign     db " + Chr(39) + "-" + Chr(39) + " " + Chr(10) + "    nl   db 10" + Chr(10) + dot_data + Chr(10);
            upCode += "section '.bss' writeable" + Chr(10) + "    input_buffer rb 256" + Chr(10) + "    file_read_buffer rb 4096" + Chr(10) + "    input_len    rq 1" + Chr(10) + "    filename_ptr_size  rq 1" + Chr(10) + "    source_ptr      rq 1" + Chr(10) + "    source_ptr_size rq 1" + Chr(10) + "    args_array rq 3" + Chr(10) + "    filename_ptr      rq 1" + Chr(10) + "    asm_code_ptr    rq 1" + Chr(10) + "    print_buffer_n rb 20" + Chr(10) + arrBss + dot_bss;
            upCode += "section '.text' executable" + Chr(10) + HTLL_Libs_x86_new + Chr(10);
        } else {
            if (ring0 == 0) {
                upCode = "segment readable writeable" + Chr(10) + "    SCALE_FACTOR   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY = 2" + Chr(10) + "    print_buffer   rb 21" + Chr(10) + "    dot            db " + Chr(39) + "." + Chr(39) + "" + Chr(10) + "    minus_sign     db " + Chr(39) + "-" + Chr(39) + "" + Chr(10) + "    nl   db 10" + Chr(10) + dot_data + Chr(10);
                upCode += Chr(10) + Chr(10) + "    input_buffer rb 256" + Chr(10) + "    file_read_buffer rb 4096" + Chr(10) + "    input_len    rq 1" + Chr(10) + "    filename_ptr_size  rq 1" + Chr(10) + "    source_ptr      rq 1" + Chr(10) + "    source_ptr_size rq 1" + Chr(10) + "    args_array rq 3" + Chr(10) + "    filename_ptr      rq 1" + Chr(10) + "    asm_code_ptr    rq 1" + Chr(10) + "    print_buffer_n rb 20" + Chr(10) + arrBss + dot_bss;
                upCode += "segment readable executable" + Chr(10) + HTLL_Libs_x86_new + Chr(10);
            } else {
                upCode = Chr(10) + "    SCALE_FACTOR   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY = 2" + Chr(10) + "    print_buffer   rb 21" + Chr(10) + "    dot            db " + Chr(39) + "." + Chr(39) + "" + Chr(10) + "    minus_sign     db " + Chr(39) + "-" + Chr(39) + "" + Chr(10) + "    nl   db 10" + Chr(10) + dot_data + Chr(10);
                upCode += Chr(10) + Chr(10) + "    input_buffer rb 256" + Chr(10) + "    file_read_buffer rb 4096" + Chr(10) + "    input_len    rq 1" + Chr(10) + "    filename_ptr_size  rq 1" + Chr(10) + "    source_ptr      rq 1" + Chr(10) + "    source_ptr_size rq 1" + Chr(10) + "    args_array rq 3" + Chr(10) + "    filename_ptr      rq 1" + Chr(10) + "    asm_code_ptr    rq 1" + Chr(10) + "    print_buffer_n rb 20" + Chr(10) + arrBss + dot_bss + Chr(10) + "; Global variables for the 64-bit Runtime" + Chr(10) + "cursor_x dq 0" + Chr(10) + "cursor_y dq 0" + Chr(10) + "heap_ptr dq 0x40000" + Chr(10) + "; Minimal Scancode Map (QWERTY) - You can expand this" + Chr(10) + "scan_map:" + Chr(10) + "db 0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9" + Chr(10) + "db 'q','w','e','r','t','y','u','i','o','p','[',']',10,0,'a','s'" + Chr(10) + "db 'd','f','g','h','j','k','l',';',39,'" + Chr(96) + "',0,'\\','z','x','c','v'" + Chr(10) + "db 'b','n','m',',','.','/',0,'*',0,32" + Chr(10);
                upCode += Chr(10) + HTLL_Libs_x86_new + Chr(10);
            }
        }
    }
    // --- FASM FOOTER ---
    std::string downCode = "";
    if (is_arm == 1) {
        // --- ARM AArch64 EXIT SEQUENCE ---
        // NOTE: Your generated 'main' or other user code will go before this.
        // The _start label needs to wrap your main program logic.
        downCode += Chr(10) + "    mov x0, #0" + Chr(10);
        downCode += "    mov x8, #SYS_EXIT" + Chr(10);
        downCode += "    svc #0" + Chr(10);
    } else {
        // --- THIS IS YOUR EXISTING x86 CODE, UNCHANGED ---
        if (ring0 == 0) {
            downCode += Chr(10) + "    mov rsp, rbp" + Chr(10) + "    pop rbp" + Chr(10) + "    mov rax, 60" + Chr(10) + "    xor rdi, rdi" + Chr(10) + "    syscall" + Chr(10) + "";
        } else {
            downCode += Chr(10) + "mov rsp, rbp" + Chr(10) + "pop rbp" + Chr(10) + "jmp .program_end" + Chr(10) + ".program_end:" + Chr(10) + "jmp $" + Chr(10);
        }
    }
    // --- ASSEMBLE THE FINAL STRING ---
    std::string codeOUT = "";
    if (seenMain == 0) {
        // The 'main_syntax' variable will also need an 'if (is_arm)' block
        // to generate the correct function prologue for the target architecture.
        codeOUT = Chr(10) + fasm_header + Chr(10) + upCode + Chr(10) + main_syntax + Chr(10) + code + Chr(10) + downCode + Chr(10);
    } else {
        // The 'out' variable is the user's code. This must be generated as ARM or x86
        // based on the 'is_arm' flag *before* this assembly stage.
        codeOUT = Chr(10) + fasm_header + Chr(10) + upCode + Chr(10) + code + Chr(10) + downCode + Chr(10);
    }
    //;;;;;;;;;;;;;;;;;;;;;;;;;
    for (int A_Index190 = 0; A_Index190 < theIdNumOfThe34; A_Index190++) {
        if (theIdNumOfThe34 == A_Index190 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index190 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index190 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index190 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index190 + 1], keyWordEscpaeChar, "\\"));
        }
    }
    codeOUT = StrReplace(codeOUT, " [rax]", " rax");
    int is_1_optimize = 0;
    int is_2_optimize = 0;
    int is_3_optimize = 0;
    int is_4_optimize = 0;
    int is_5_optimize = 0;
    int is_6_optimize = 0;
    int is_7_optimize = 0;
    int is_8_optimize = 0;
    int is_9_optimize = 0;
    int is_10_optimize = 0;
    int is_11_optimize = 0;
    int is_12_optimize = 0;
    int is_13_optimize = 0;
    int is_14_optimize = 0;
    int is_15_optimize = 0;
    int is_16_optimize = 0;
    int is_17_optimize = 0;
    int is_18_optimize = 0;
    int is_19_optimize = 0;
    int is_20_optimize = 0;
    int is_21_optimize = 0;
    int is_22_optimize = 0;
    int is_in_under = 0;
    std::string allInOne_temp = "";
    std::vector<std::string> items191 = LoopParseFunc(codeOUT, "\n", "\r");
    for (size_t A_Index191 = 0; A_Index191 < items191.size(); A_Index191++) {
        std::string A_LoopField191 = items191[A_Index191 - 0];
        if (SubStr(Trim(A_LoopField191), 1, 2) != "; ") {
            allInOne_temp += Trim(A_LoopField191) + Chr(10);
            // Ignore definitions so they don't count as usage
            if (InStr(A_LoopField191, " db ") == 0 && InStr(A_LoopField191, " rb ") == 0 && InStr(A_LoopField191, " rq ") == 0 && InStr(A_LoopField191, " dq ") == 0 && InStr(A_LoopField191, " = ") == 0) {
                // CHANGED ALL "else if" TO "if" TO PREVENT OVERLAPPING REGEX BUGS
                if (RegExMatch(A_LoopField191, "\\bDynamicArray.pointer\\b")) {
                    is_1_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bDynamicArray.size\\b")) {
                    is_2_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bDynamicArray.capacity\\b")) {
                    is_3_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bsizeof.DynamicArray\\b")) {
                    is_4_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bSCALE" + Chr(95) + "FACTOR\\b")) {
                    is_5_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bINITIAL" + Chr(95) + "CAPACITY\\b")) {
                    is_6_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bprint" + Chr(95) + "buffer\\b")) {
                    is_7_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bdot\\b")) {
                    is_8_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bminus" + Chr(95) + "sign\\b")) {
                    is_9_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\b" + Chr(110) + Chr(108) + "\\b")) {
                    is_10_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\binput" + Chr(95) + "buffer\\b")) {
                    is_11_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bfile" + Chr(95) + "read" + Chr(95) + "buffer\\b")) {
                    is_12_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\binput" + Chr(95) + "len\\b")) {
                    is_13_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bfilename" + Chr(95) + "ptr" + Chr(95) + "size\\b")) {
                    is_14_optimize = 1;
                }
                // FIXED REGEX FOR SOURCE_PTR (is_15)
                if (RegExMatch(A_LoopField191, "\\bsource" + Chr(95) + "ptr\\b")) {
                    is_15_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bsource" + Chr(95) + "ptr" + Chr(95) + "size\\b")) {
                    is_16_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bargs" + Chr(95) + "array\\b")) {
                    is_17_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bfilename" + Chr(95) + "ptr\\b")) {
                    is_18_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\basm" + Chr(95) + "code" + Chr(95) + "ptr\\b")) {
                    is_19_optimize = 1;
                }
                if (RegExMatch(A_LoopField191, "\\bprint" + Chr(95) + "buffer" + Chr(95) + "n\\b")) {
                    is_20_optimize = 1;
                }
            }
        }
    }
    codeOUT = Trim(allInOne_temp);
    allInOne_temp = "";
    int aredy_int = 0;
    std::vector<std::string> items192 = LoopParseFunc(codeOUT, "\n", "\r");
    for (size_t A_Index192 = 0; A_Index192 < items192.size(); A_Index192++) {
        std::string A_LoopField192 = items192[A_Index192 - 0];
        aredy_int = 0;
        if (Trim(A_LoopField192) == "DynamicArray.pointer  = 0" && is_1_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "DynamicArray.size     = 8" && is_2_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "DynamicArray.capacity = 16" && is_3_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "sizeof.DynamicArray   = 24" && is_4_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "SCALE_FACTOR   dq 1000000" && is_5_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "INITIAL_CAPACITY = 2" && is_6_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "print_buffer   rb 21" && is_7_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "dot            db " + Chr(39) + "." + Chr(39) + "" && is_8_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "minus_sign     db " + Chr(39) + Chr(45) + Chr(39) && is_9_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == Chr(110) + Chr(108) + "   db 10" && is_10_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "input_buffer rb 256" && is_11_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "file_read_buffer rb 4096" && is_12_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "input_len    rq 1" && is_13_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "filename_ptr_size  rq 1" && is_14_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "source_ptr      rq 1" && is_15_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "source_ptr_size rq 1" && is_16_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "args_array rq 3" && is_17_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "filename_ptr      rq 1" && is_18_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "asm_code_ptr    rq 1" && is_19_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField192) == "print_buffer_n rb 20" && is_20_optimize == 0) {
            aredy_int = 1;
        }
        if (aredy_int == 0) {
            allInOne_temp += A_LoopField192 + Chr(10);
        }
    }
    codeOUT = Trim(allInOne_temp);
    return codeOUT;
}
int main(int argc, char* argv[]) {
    std::string params = Trim(GetParams());
    std::string paramsTemp = "";
    if (params == "") {
        print("Usage:" + Chr(10) + "./HTLL your_file.htll");
    } else {
        std::vector<std::string> items193 = LoopParseFunc(params, "\n", "\r");
        for (size_t A_Index193 = 0; A_Index193 < items193.size(); A_Index193++) {
            std::string A_LoopField193 = items193[A_Index193 - 0];
            paramsTemp = Trim(A_LoopField193);
            if (A_Index193 == 0) {
                FileDelete("finalASM_HTLL_ASM.s");
                FileAppend(compiler(FileRead(paramsTemp)), "finalASM_HTLL_ASM.s");
                print("Compilation finished: finalASM_HTLL_ASM.s generated.");
            }
        }
    }
    

    return 0;
}