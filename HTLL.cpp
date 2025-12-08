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
std::string SubStrLastChars(std::string text, int numOfChars) {
    std::string LastOut = "";
    int NumOfChars = 0;
    std::vector<std::string> items1 = LoopParseFunc(text);
    for (size_t A_Index1 = 0; A_Index1 < items1.size(); A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        NumOfChars++;
    }
    for (int A_Index2 = 0; A_Index2 < numOfChars; A_Index2++) {
        NumOfChars--;
    }
    std::vector<std::string> items3 = LoopParseFunc(text);
    for (size_t A_Index3 = 0; A_Index3 < items3.size(); A_Index3++) {
        std::string A_LoopField3 = items3[A_Index3 - 0];
        if (A_Index3 >= NumOfChars) {
            LastOut += A_LoopField3;
        }
    }
    return LastOut;
}
std::vector<std::string> nintArr;
bool isNint(std::string name) {
    for (int A_Index4 = 0; A_Index4 < HTVM_Size(nintArr); A_Index4++) {
        if (Trim(name) == Trim(nintArr[A_Index4])) {
            return true;
        }
    }
    return false;
}
std::string compiler(std::string code) {
    std::string out = "";
    std::string main_syntax = "";
    if (RegExMatch(code, "\\bargs_array\\b")) {
        main_syntax = Chr(10) + "_start:" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "and rsp, -16 " + Chr(10) + "mov rdi, args_array" + Chr(10) + "call array_clear" + Chr(10) + "mov r12, [rbp+8]" + Chr(10) + "lea r13, [rbp+16]" + Chr(10) + "mov r14, 1" + Chr(10) + ".arg_loop:" + Chr(10) + "cmp r14, r12" + Chr(10) + "jge .args_done" + Chr(10) + "mov rsi, [r13 + r14*8]" + Chr(10) + "mov rdi, args_array" + Chr(10) + "call array_unpack_from_bytes" + Chr(10) + "mov rdi, args_array" + Chr(10) + "mov rsi, 10" + Chr(10) + "call array_append" + Chr(10) + "inc r14" + Chr(10) + "jmp .arg_loop" + Chr(10) + ".args_done:" + Chr(10) + Chr(10);
    } else {
        main_syntax = Chr(10) + "_start:" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "and rsp, -16 " + Chr(10) + Chr(10);
    }
    std::string HTLL_Libs_x86 = FileRead("HTLL_Libs_x86.txt");
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
    std::vector<std::string> items5 = LoopParseFunc(code);
    for (size_t A_Index5 = 0; A_Index5 < items5.size(); A_Index5++) {
        std::string A_LoopField5 = items5[A_Index5 - 0];
        HTVM_Append(theIdNumOfThe34theVar, "");
        HTVM_Append(theIdNumOfThe34theVar, "");
    }
    std::vector<std::string> items6 = LoopParseFunc(code);
    for (size_t A_Index6 = 0; A_Index6 < items6.size(); A_Index6++) {
        std::string A_LoopField6 = items6[A_Index6 - 0];
        theIdNumOfThe34theVar[A_Index6] = theIdNumOfThe34theVar[A_Index6] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField6);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "--" + str21 + "asds" + str21 + "as--" + str21 + "theuhtuwaesphoutr" + Chr(34);
    std::vector<std::string> items7 = LoopParseFunc(code);
    for (size_t A_Index7 = 0; A_Index7 < items7.size(); A_Index7++) {
        std::string A_LoopField7 = items7[A_Index7 - 0];
        if (A_LoopField7 == keyWordEscpaeChar && getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index7 + 1] == Chr(34)) {
            fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 1;
            OutFixDoubleQuotesInsideDoubleQuotes += ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes;
        } else {
            if (fixOutFixDoubleQuotesInsideDoubleQuotesFIXok != 1) {
                OutFixDoubleQuotesInsideDoubleQuotes += A_LoopField7;
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
        std::vector<std::string> items8 = LoopParseFunc(code);
        for (size_t A_Index8 = 0; A_Index8 < items8.size(); A_Index8++) {
            std::string A_LoopField8 = items8[A_Index8 - 0];
            if (A_LoopField8 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField8 != Chr(34)) {
                    if (A_LoopField8 == keyWordEscpaeChar) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField8;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField8 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField8;
                }
                areWEinSome34sNum = 0;
            }
        }
    } else {
        std::vector<std::string> items9 = LoopParseFunc(code);
        for (size_t A_Index9 = 0; A_Index9 < items9.size(); A_Index9++) {
            std::string A_LoopField9 = items9[A_Index9 - 0];
            if (A_LoopField9 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField9 != Chr(34)) {
                    if (A_LoopField9 == keyWordEscpaeChar && keyWordEscpaeChar == getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index9 + 1]) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + keyWordEscpaeChar;
                        removeNexFixkeyWordEscpaeChar = 1;
                    }
                    else if (A_LoopField9 == keyWordEscpaeChar) {
                        if (removeNexFixkeyWordEscpaeChar != 1) {
                            theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                        } else {
                            removeNexFixkeyWordEscpaeChar = 0;
                        }
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField9;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField9 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField9;
                }
                areWEinSome34sNum = 0;
            }
        }
    }
    code = htCodeOUT754754;
    for (int A_Index10 = 0; A_Index10 < theIdNumOfThe34; A_Index10++) {
        theIdNumOfThe34theVar[A_Index10] = theIdNumOfThe34theVar[A_Index10] + Chr(34);
    }
    HTVM_Append(theIdNumOfThe34theVar, Chr(34));
    code = StrReplace(code, "{", Chr(10) + "{" + Chr(10));
    code = StrReplace(code, "}", Chr(10) + "}" + Chr(10));
    std::vector<std::string> items11 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index11 = 0; A_Index11 < items11.size(); A_Index11++) {
        std::string A_LoopField11 = items11[A_Index11 - 0];
        out += Trim(A_LoopField11) + Chr(10);
    }
    code = StringTrimRight(out, 1);
    str1 = "";
    out = "";
    std::vector<std::string> items12 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index12 = 0; A_Index12 < items12.size(); A_Index12++) {
        std::string A_LoopField12 = items12[A_Index12 - 0];
        std::vector<std::string> items13 = LoopParseFunc(A_LoopField12);
        for (size_t A_Index13 = 0; A_Index13 < items13.size(); A_Index13++) {
            std::string A_LoopField13 = items13[A_Index13 - 0];
            if (A_LoopField13 != ";") {
                str1 += A_LoopField13;
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
    std::vector<std::string> items14 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index14 = 0; A_Index14 < items14.size(); A_Index14++) {
        std::string A_LoopField14 = items14[A_Index14 - 0];
        if (Trim(A_LoopField14) == "main") {
            seenMain = 1;
            out += main_syntax + Chr(10);
        } else {
            out += A_LoopField14 + Chr(10);
        }
    }
    code = StringTrimRight(out, 1);
    out = "";
    std::vector<std::string> items15 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index15 = 0; A_Index15 < items15.size(); A_Index15++) {
        std::string A_LoopField15 = items15[A_Index15 - 0];
        if (SubStr(A_LoopField15, 1, 7) == "arradd ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 7));
            str2 = StrSplit(str1, " ", 1);
            str3 = StringTrimLeft(str1, StrLen(str2) + 1);
            str4 = "";
            std::vector<std::string> items16 = LoopParseFunc(str3);
            for (size_t A_Index16 = 0; A_Index16 < items16.size(); A_Index16++) {
                std::string A_LoopField16 = items16[A_Index16 - 0];
                str4 += "mov rsi, " + Chr(39) + A_LoopField16 + Chr(39) + Chr(10) + "mov rdi, " + str2 + Chr(10) + "call array_append" + Chr(10);
            }
            out += str4 + Chr(10);
        }
        else if (SubStr(A_LoopField15, 1, 4) == "int ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
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
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            if (InStr(A_LoopField15, " += ")) {
                str2 = Trim(StrSplit(str1, "+=", 1));
                str3 = Trim(StrSplit(str1, "+=", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    out += "add qword [" + str2 + "], " + str3 + Chr(10);
                } else {
                    out += "mov rdi, [" + str3 + "]" + Chr(10);
                    out += "add qword [" + str2 + "], rdi" + Chr(10);
                }
            }
            else if (InStr(A_LoopField15, " *= ")) {
                str2 = Trim(StrSplit(str1, "*=", 1));
                str3 = Trim(StrSplit(str1, "*=", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    // *= constant
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, " + str3 + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                } else {
                    // *= another variable
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, [" + str3 + "]" + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                }
            }
            else if (InStr(A_LoopField15, " -= ")) {
                str2 = Trim(StrSplit(str1, "-=", 1));
                str3 = Trim(StrSplit(str1, "-=", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    out += "sub qword [" + str2 + "], " + str3 + Chr(10);
                } else {
                    out += "mov rdi, [" + str3 + "]" + Chr(10);
                    out += "sub qword [" + str2 + "], rdi" + Chr(10);
                }
            }
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        }
        else if (SubStr(A_LoopField15, 1, 5) == "nint ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 5));
            if (InStr(str1, " ")) {
                HTVM_Append(nintArr, StrSplit(str1, " ", 1));
            } else {
                HTVM_Append(nintArr, Trim(str1));
            }
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
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            if (InStr(A_LoopField15, " += ")) {
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
            else if (InStr(A_LoopField15, " *= ")) {
                str2 = Trim(StrSplit(str1, "*=", 1));
                str3 = Trim(StrSplit(str1, "*=", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    // *= constant
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, " + str3 + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                    out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                } else {
                    // *= another variable
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, [" + str3 + "]" + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                    out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                }
            }
            else if (InStr(A_LoopField15, " -= ")) {
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
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        }
        else if (SubStr(A_LoopField15, 1, 4) == "str ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str2 = Trim(StrSplit(str1, "[", 2));
            str2 = Trim(StrSplit(str2, "]", 1));
            str1 = Trim(StrSplit(str1, "[", 1));
            dot_bss_str += str1 + " rb " + str2 + Chr(10);
        }
        else if (InStr(A_LoopField15, " := ") || InStr(A_LoopField15, " += ") || InStr(A_LoopField15, " -= ") || InStr(A_LoopField15, " *= ")) {
            if (InStr(A_LoopField15, " := ")) {
                str1 = Trim(A_LoopField15);
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
            else if (InStr(A_LoopField15, " += ")) {
                str1 = Trim(A_LoopField15);
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
            else if (InStr(A_LoopField15, " *= ")) {
                str1 = Trim(A_LoopField15);
                str2 = Trim(StrSplit(str1, "*=", 1));
                str3 = Trim(StrSplit(str1, "*=", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    // *= constant
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, " + str3 + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                    if (isNint(str2)) {
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    }
                } else {
                    // *= another variable
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "imul rax, [" + str3 + "]" + Chr(10);
                    out += "mov [" + str2 + "], rax" + Chr(10);
                    if (isNint(str2)) {
                        out += "lea rdi, [" + str2 + "]" + Chr(10) + "lea rsi, [" + str2 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
                    }
                }
            }
            else if (InStr(A_LoopField15, " -= ")) {
                str1 = Trim(A_LoopField15);
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
        else if (InStr(A_LoopField15, "++")) {
            str1 = Trim(A_LoopField15);
            str1 = StringTrimRight(str1, 2);
            out += "inc qword [" + Trim(str1) + "]" + Chr(10);
            if (isNint(str1)) {
                out += "lea rdi, [" + str1 + "]" + Chr(10) + "lea rsi, [" + str1 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
            }
        }
        else if (InStr(A_LoopField15, "--")) {
            str1 = Trim(A_LoopField15);
            str1 = StringTrimRight(str1, 2);
            out += "dec qword [" + Trim(str1) + "]" + Chr(10);
            if (isNint(str1)) {
                out += "lea rdi, [" + str1 + "]" + Chr(10) + "lea rsi, [" + str1 + "_is_negative]" + Chr(10) + "call is_nint_negative" + Chr(10);
            }
        }
        else if (SubStr(A_LoopField15, 1, 6) == "print(") {
            str1 = StringTrimLeft(A_LoopField15, 6);
            str1 = StringTrimRight(str1, 1);
            if (RegExMatch(str1, "^\\d+$")) {
                out += "mov rdi, " + Trim(str1) + Chr(10) + "mov rsi, 0" + Chr(10) + "call print_number" + Chr(10);
            }
            else if (InStr(str1, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC")) {
                dot_data_print_temp_strings_count++;
                dot_data_print_temp_strings += "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + " db " + Trim(str1) + ", 10" + Chr(10) + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len = $-" + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10);
                out += "mov rsi, ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10) + "mov rdx, " + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len" + Chr(10) + "call print_str" + Chr(10);
            } else {
                if (isNint(str1)) {
                    out += "mov rdi, [" + Trim(str1) + "]" + Chr(10) + "mov rsi, [" + str2 + "_is_negative]" + Chr(10) + "call print_number" + Chr(10);
                } else {
                    out += "mov rdi, [" + Trim(str1) + "]" + Chr(10) + "mov rsi, 0" + Chr(10) + "call print_number" + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField15) == "print_rax_as_char") {
            out += "mov rdi, rax" + Chr(10) + "push rcx" + Chr(10) + "call print_char" + Chr(10) + "pop rcx" + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue" || StrLower(Trim(A_LoopField15)) == "continue1") {
            out += "jmp .cloop1_end" + STR(loopCount1) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue2") {
            out += "jmp .cloop2_end" + STR(loopCount2) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue3") {
            out += "jmp .cloop3_end" + STR(loopCount3) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue4") {
            out += "jmp .cloop4_end" + STR(loopCount4) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue5") {
            out += "jmp .cloop5_end" + STR(loopCount5) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue6") {
            out += "jmp .cloop6_end" + STR(loopCount6) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue7") {
            out += "jmp .cloop7_end" + STR(loopCount7) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue8") {
            out += "jmp .cloop8_end" + STR(loopCount8) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "continue9") {
            out += "jmp .cloop9_end" + STR(loopCount9) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break" || StrLower(Trim(A_LoopField15)) == "break1") {
            out += "jmp .loop1_end" + STR(loopCount1) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break2") {
            out += "jmp .loop2_end" + STR(loopCount2) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break3") {
            out += "jmp .loop3_end" + STR(loopCount3) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break4") {
            out += "jmp .loop4_end" + STR(loopCount4) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break5") {
            out += "jmp .loop5_end" + STR(loopCount5) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break6") {
            out += "jmp .loop6_end" + STR(loopCount6) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break7") {
            out += "jmp .loop7_end" + STR(loopCount7) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break8") {
            out += "jmp .loop8_end" + STR(loopCount8) + Chr(10);
        }
        else if (StrLower(Trim(A_LoopField15)) == "break9") {
            out += "jmp .loop9_end" + STR(loopCount9) + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 6) == "loop, " || SubStr(StrLower(A_LoopField15), 1, 7) == "loop1, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 6));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop1_" + STR(loopCount1) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop1_end" + STR(loopCount1) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop1_" + STR(loopCount1) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop1_end" + STR(loopCount1) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend" || Trim(A_LoopField15) == "endloop" || Trim(A_LoopField15) == "loopend1" || Trim(A_LoopField15) == "endloop1") {
            out += ".cloop1_end" + STR(loopCount1) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop1_" + STR(loopCount1) + Chr(10) + ".loop1_end" + STR(loopCount1) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount1++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop2, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop2_" + STR(loopCount2) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop2_end" + STR(loopCount2) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop2_" + STR(loopCount2) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop2_end" + STR(loopCount2) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend2" || Trim(A_LoopField15) == "endloop2") {
            out += ".cloop2_end" + STR(loopCount2) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop2_" + STR(loopCount2) + Chr(10) + ".loop2_end" + STR(loopCount2) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount2++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop3, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop3_" + STR(loopCount3) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop3_end" + STR(loopCount3) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop3_" + STR(loopCount3) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop3_end" + STR(loopCount3) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend3" || Trim(A_LoopField15) == "endloop3") {
            out += ".cloop3_end" + STR(loopCount3) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop3_" + STR(loopCount3) + Chr(10) + ".loop3_end" + STR(loopCount3) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount3++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop4, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop4_" + STR(loopCount4) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop4_end" + STR(loopCount4) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop4_" + STR(loopCount4) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop4_end" + STR(loopCount4) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend4" || Trim(A_LoopField15) == "endloop4") {
            out += ".cloop4_end" + STR(loopCount4) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop4_" + STR(loopCount4) + Chr(10) + ".loop4_end" + STR(loopCount4) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount4++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop5, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop5_" + STR(loopCount5) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop5_end" + STR(loopCount5) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop5_" + STR(loopCount5) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop5_end" + STR(loopCount5) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend5" || Trim(A_LoopField15) == "endloop5") {
            out += ".cloop5_end" + STR(loopCount5) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop5_" + STR(loopCount5) + Chr(10) + ".loop5_end" + STR(loopCount5) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount5++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop6, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop6_" + STR(loopCount6) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop6_end" + STR(loopCount6) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop6_" + STR(loopCount6) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop6_end" + STR(loopCount6) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend6" || Trim(A_LoopField15) == "endloop6") {
            out += ".cloop6_end" + STR(loopCount6) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop6_" + STR(loopCount6) + Chr(10) + ".loop6_end" + STR(loopCount6) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount6++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop7, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop7_" + STR(loopCount7) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop7_end" + STR(loopCount7) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop7_" + STR(loopCount7) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop7_end" + STR(loopCount7) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend7" || Trim(A_LoopField15) == "endloop7") {
            out += ".cloop7_end" + STR(loopCount7) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop7_" + STR(loopCount7) + Chr(10) + ".loop7_end" + STR(loopCount7) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount7++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop8, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop8_" + STR(loopCount8) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop8_end" + STR(loopCount8) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop8_" + STR(loopCount8) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop8_end" + STR(loopCount8) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend8" || Trim(A_LoopField15) == "endloop8") {
            out += ".cloop8_end" + STR(loopCount8) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop8_" + STR(loopCount8) + Chr(10) + ".loop8_end" + STR(loopCount8) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount8++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "loop9, ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            if (RegExMatch(str1, "^\\d+$")) {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, " + str1 + Chr(10) + ".loop9_" + STR(loopCount9) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop9_end" + STR(loopCount9) + Chr(10);
            } else {
                out += "push r12" + Chr(10) + "push r13" + Chr(10) + "xor r13, r13" + Chr(10) + "mov r12, [" + str1 + "]" + Chr(10) + ".loop9_" + STR(loopCount9) + ":" + Chr(10) + "cmp r12, 0" + Chr(10) + "je .loop9_end" + STR(loopCount9) + Chr(10);
            }
        }
        else if (Trim(A_LoopField15) == "loopend9" || Trim(A_LoopField15) == "endloop9") {
            out += ".cloop9_end" + STR(loopCount9) + ":" + Chr(10) + "inc r13" + Chr(10) + "dec r12" + Chr(10) + "jmp .loop9_" + STR(loopCount9) + Chr(10) + ".loop9_end" + STR(loopCount9) + ":" + Chr(10) + "pop r13" + Chr(10) + "pop r12" + Chr(10);
            loopCount9++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 3) == "if " || SubStr(StrLower(A_LoopField15), 1, 4) == "if1 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 3));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            isNum = 0;
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                str3 = Trim(StrSplit(str1, " = ", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                str3 = Trim(StrSplit(str1, " != ", 2));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jle .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jle .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jge .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jge .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jl .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jl .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jg .end_if1_" + STR(ifCount1) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jg .end_if1_" + STR(ifCount1) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField15) == "ifend" || Trim(A_LoopField15) == "endif" || Trim(A_LoopField15) == "ifend1" || Trim(A_LoopField15) == "endif1") {
            out += ".end_if1_" + STR(ifCount1) + ":" + Chr(10);
            ifCount1++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if2 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
            // --- IF2 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jle .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jle .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jge .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jge .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jl .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jl .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jg .end_if2_" + STR(ifCount2) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jg .end_if2_" + STR(ifCount2) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField15) == "ifend2" || Trim(A_LoopField15) == "endif2") {
            out += ".end_if2_" + STR(ifCount2) + ":" + Chr(10);
            ifCount2++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if3 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
            // --- IF3 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jle .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jle .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jge .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jge .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jl .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jl .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, " + str3 + Chr(10);
                    out += "jg .end_if3_" + STR(ifCount3) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10);
                    out += "cmp rax, [" + str3 + "]" + Chr(10);
                    out += "jg .end_if3_" + STR(ifCount3) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField15) == "ifend3" || Trim(A_LoopField15) == "endif3") {
            out += ".end_if3_" + STR(ifCount3) + ":" + Chr(10);
            ifCount3++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if4 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if4_" + STR(ifCount4) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if4_" + STR(ifCount4) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if4_" + STR(ifCount4) + Chr(10);
                }
            }
            // --- IF4 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
        else if (Trim(A_LoopField15) == "ifend4" || Trim(A_LoopField15) == "endif4") {
            out += ".end_if4_" + STR(ifCount4) + ":" + Chr(10);
            ifCount4++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if5 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if5_" + STR(ifCount5) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if5_" + STR(ifCount5) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if5_" + STR(ifCount5) + Chr(10);
                }
            }
            // --- IF5 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
        else if (Trim(A_LoopField15) == "ifend5" || Trim(A_LoopField15) == "endif5") {
            out += ".end_if5_" + STR(ifCount5) + ":" + Chr(10);
            ifCount5++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if6 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if6_" + STR(ifCount6) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if6_" + STR(ifCount6) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if6_" + STR(ifCount6) + Chr(10);
                }
            }
            // --- IF6 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
        else if (Trim(A_LoopField15) == "ifend6" || Trim(A_LoopField15) == "endif6") {
            out += ".end_if6_" + STR(ifCount6) + ":" + Chr(10);
            ifCount6++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if7 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if7_" + STR(ifCount7) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if7_" + STR(ifCount7) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if7_" + STR(ifCount7) + Chr(10);
                }
            }
            // --- IF7 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
        else if (Trim(A_LoopField15) == "ifend7" || Trim(A_LoopField15) == "endif7") {
            out += ".end_if7_" + STR(ifCount7) + ":" + Chr(10);
            ifCount7++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if8 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if8_" + STR(ifCount8) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if8_" + STR(ifCount8) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if8_" + STR(ifCount8) + Chr(10);
                }
            }
            // --- IF8 ---
            if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                str3 = Trim(StrSplit(str1, " > ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                str3 = Trim(StrSplit(str1, " < ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                str3 = Trim(StrSplit(str1, " >= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                str3 = Trim(StrSplit(str1, " <= ", 2));
                isNum = 0;
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
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
        else if (Trim(A_LoopField15) == "ifend8" || Trim(A_LoopField15) == "endif8") {
            out += ".end_if8_" + STR(ifCount8) + ":" + Chr(10);
            ifCount8++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "if9 ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 4));
            str1 = StringTrimLeft(str1, 1);
            str1 = StringTrimRight(str1, 1);
            if (InStr(str1, " = ")) {
                str2 = Trim(StrSplit(str1, " = ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jne .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jne .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
            else if (InStr(str1, " != ")) {
                str2 = Trim(StrSplit(str1, " != ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "je .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "je .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
            else if (InStr(str1, " > ")) {
                str2 = Trim(StrSplit(str1, " > ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jle .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jle .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
            else if (InStr(str1, " < ")) {
                str2 = Trim(StrSplit(str1, " < ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jge .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jge .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
            else if (InStr(str1, " >= ")) {
                str2 = Trim(StrSplit(str1, " >= ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jl .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jl .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
            else if (InStr(str1, " <= ")) {
                str2 = Trim(StrSplit(str1, " <= ", 1));
                if (RegExMatch(str3, "^\\d+$")) {
                    isNum = 1;
                }
                if (isNum == 1) {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, " + str3 + Chr(10) + "jg .end_if9_" + STR(ifCount9) + Chr(10);
                } else {
                    out += "mov rax, [" + str2 + "]" + Chr(10) + "cmp rax, [" + str3 + "]" + Chr(10) + "jg .end_if9_" + STR(ifCount9) + Chr(10);
                }
            }
        }
        else if (Trim(A_LoopField15) == "ifend9" || Trim(A_LoopField15) == "endif9") {
            out += ".end_if9_" + STR(ifCount9) + ":" + Chr(10);
            ifCount9++;
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 5) == "func ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 5));
            str2 = "";
            std::vector<std::string> items17 = LoopParseFunc(str1);
            for (size_t A_Index17 = 0; A_Index17 < items17.size(); A_Index17++) {
                std::string A_LoopField17 = items17[A_Index17 - 0];
                if (A_LoopField17 == "(") {
                    break;
                } else {
                    str2 += A_LoopField17;
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
                std::vector<std::string> items18 = LoopParseFunc(str1);
                for (size_t A_Index18 = 0; A_Index18 < items18.size(); A_Index18++) {
                    std::string A_LoopField18 = items18[A_Index18 - 0];
                    if (A_LoopField18 == ")") {
                        break;
                    }
                    if (int1 == 1 && A_LoopField18 != "," && A_LoopField18 != " ") {
                        funcArgsArr[int2] = funcArgsArr[int2] + A_LoopField18;
                    }
                    if (A_LoopField18 == ",") {
                        HTVM_Append(funcArgsArr, "");
                        int2++;
                    }
                    if (A_LoopField18 == "(") {
                        HTVM_Append(funcArgsArr, "");
                        int1 = 1;
                    }
                }
            }
            funcName = str2;
            out += str2 + ":" + Chr(10) + "push rbp" + Chr(10) + "mov rbp, rsp" + Chr(10) + "sub rsp, " + STR(8 + (localVarNum * 8)) + Chr(10);
        }
        else if (Trim(A_LoopField15) == "funcend" || Trim(A_LoopField15) == "funcend") {
            out += "." + funcName + "_return:" + Chr(10) + "add rsp, " + STR(8 + (localVarNum * 8)) + Chr(10) + "pop rbp" + Chr(10) + "ret" + Chr(10);
            funcCount++;
        }
        else if (Trim(A_LoopField15) == "return") {
            out += "jmp ." + funcName + "_return" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "return ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 7));
            str2 = str1;
            if (RegExMatch(str1, "^\\d+$")) {
                out += "mov rax, " + str2 + Chr(10) + "jmp ." + funcName + "_return" + Chr(10);
            } else {
                out += "mov rax, [" + str2 + "]" + Chr(10) + "jmp ." + funcName + "_return" + Chr(10);
            }
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 4) == "arr ") {
            str1 = Trim(StringTrimLeft(Trim(A_LoopField15), 4));
            arrBss += "    " + Trim(str1) + " rq 3" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".add ")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StrSplit(A_LoopField15, ".add", 2));
            if (RegExMatch(str2, "^\\d+$")) {
                str3 = "mov rsi, " + str2 + Chr(10) + "mov rdi, " + str1 + Chr(10);
            } else {
                str3 = "mov rsi, [" + str2 + "]" + Chr(10) + "mov rdi, " + str1 + Chr(10);
            }
            out += str3 + Chr(10) + "call array_append" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".pop")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StrSplit(A_LoopField15, ".pop", 2));
            str3 = "mov rdi, " + str1 + Chr(10);
            out += str3 + Chr(10) + "call array_pop" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".clear")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            out += "mov rdi, " + str1 + Chr(10) + "call array_clear" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".copy ")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StrSplit(A_LoopField15, ".copy", 2));
            out += "mov rdi, " + str1 + Chr(10) + "mov rsi, " + str2 + Chr(10) + "call array_copy" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".size")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            out += "mov rax, [" + str1 + " + DynamicArray.size]" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".index ")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StrSplit(A_LoopField15, ".index", 2));
            if (RegExMatch(str2, "^\\d+$")) {
                str3 = "mov rcx, " + str2 + Chr(10);
            } else {
                str3 = "mov rcx, [" + str2 + "]" + Chr(10);
            }
            out += str3 + Chr(10) + "mov rbx, [" + str1 + " + DynamicArray.pointer]" + Chr(10) + "mov rax, [rbx + rcx*8]" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".compile ")) {
            // arrName.compile outArr
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StrSplit(A_LoopField15, ".compile", 2));
            isDotCompile = 1;
            out += "mov rdi, " + str1 + Chr(10) + "call array_pack_to_bytes" + Chr(10) + "mov [source_ptr], rax" + Chr(10) + "mov rdi, [source_ptr]" + Chr(10) + "call compiler_c" + Chr(10) + "mov [asm_code_ptr], rax" + Chr(10) + "mov rdi, " + str2 + Chr(10) + "mov rsi, [asm_code_ptr]" + Chr(10) + "call array_unpack_from_bytes" + Chr(10) + "mov rdi, [source_ptr]" + Chr(10) + "mov rsi, [source_ptr_size]" + Chr(10) + "call free_packed_string" + Chr(10) + "mov rdi, [asm_code_ptr]" + Chr(10) + "call free_string_c" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 5) == "goto ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 5));
            out += "jmp .__HTLL_HTLL_" + str1 + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 5) == "togo ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 5));
            out += ".__HTLL_HTLL_" + str1 + ":" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 7) == "sleep, ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 7));
            out += "mov rdi, " + str1 + Chr(10) + "call sleep_ms" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 13) == "fileread_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 13));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            // --- THE PURE ASSEMBLY GENERATION ---
            // Pack the FILENAME array (str3) into a C-string.
            out += "mov rdi, " + str3 + Chr(10);
            out += "call array_pack_to_bytes" + Chr(10);
            // -- THE FIX IS HERE --
            out += "mov [filename_ptr], rax" + Chr(10);
            out += "mov [filename_ptr_size], rdx" + Chr(10);
            // Call fileread_from_ptr with the DESTINATION array (str2) and the new pointer.
            out += "mov rdi, " + str2 + Chr(10);
            out += "mov rsi, [filename_ptr]" + Chr(10);
            out += "call fileread_from_ptr" + Chr(10);
            // Clean up the temporary C-string pointer using YOUR PURE FUNCTION.
            // -- THE FIX IS HERE --
            out += "mov rdi, [filename_ptr]" + Chr(10);
            out += "mov rsi, [filename_ptr_size]" + Chr(10);
            out += "call free_packed_string" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 15) == "fileappend_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 15));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            // --- THE PURE ASSEMBLY GENERATION ---
            // Pack the FILENAME array (str2) into a C-string.
            out += "mov rdi, " + str2 + Chr(10);
            out += "call array_pack_to_bytes" + Chr(10);
            // -- THE FIX IS HERE --
            out += "mov [filename_ptr], rax" + Chr(10);
            out += "mov [filename_ptr_size], rdx" + Chr(10);
            // Call fileappend_from_ptr with the new pointer and the CONTENT array (str3).
            out += "mov rdi, [filename_ptr]" + Chr(10);
            out += "mov rsi, " + str3 + Chr(10);
            out += "call fileappend_from_ptr" + Chr(10);
            // Clean up the temporary C-string pointer using YOUR PURE FUNCTION.
            // -- THE FIX IS HERE --
            out += "mov rdi, [filename_ptr]" + Chr(10);
            out += "mov rsi, [filename_ptr_size]" + Chr(10);
            out += "call free_packed_string" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 15) == "filedelete_arr ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 15));
            // --- THE PURE ASSEMBLY GENERATION ---
            // Pack the FILENAME array (str1) into a C-string.
            out += "mov rdi, " + str1 + Chr(10);
            out += "call array_pack_to_bytes" + Chr(10);
            // -- THE FIX IS HERE --
            out += "mov [filename_ptr], rax" + Chr(10);
            out += "mov [filename_ptr_size], rdx" + Chr(10);
            // Call filedelete_from_ptr with the new pointer.
            out += "mov rdi, [filename_ptr]" + Chr(10);
            out += "call filedelete_from_ptr" + Chr(10);
            // Clean up the temporary C-string pointer using YOUR PURE FUNCTION.
            // -- THE FIX IS HERE --
            out += "mov rdi, [filename_ptr]" + Chr(10);
            out += "mov rsi, [filename_ptr_size]" + Chr(10);
            out += "call free_packed_string" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 9) == "fileread ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 9));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            dot_data_print_temp_strings += path_label + " db " + str3 + ", 0" + Chr(10);
            out += "mov rdi, " + str2 + Chr(10) + "mov rsi, " + path_label + Chr(10) + "call file_read" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 11) == "fileappend ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 11));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            dot_data_print_temp_strings += path_label + " db " + str2 + ", 0" + Chr(10);
            out += "mov rdi, " + path_label + Chr(10) + "mov rsi, " + str3 + Chr(10) + "call file_append" + Chr(10);
        }
        else if (SubStr(StrLower(A_LoopField15), 1, 11) == "filedelete ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 11));
            dot_data_print_temp_strings_count++;
            path_label = "FILE_PATH_" + STR(dot_data_print_temp_strings_count);
            dot_data_print_temp_strings += path_label + " db " + str1 + ", 0" + Chr(10);
            out += "mov rdi, " + path_label + Chr(10) + "call file_delete" + Chr(10);
        }
        else if (SubStr(A_LoopField15, 1, 6) == "input ") {
            str1 = Trim(StringTrimLeft(A_LoopField15, 6));
            str2 = Trim(StrSplit(str1, ",", 1));
            str3 = Trim(StrSplit(str1, ",", 2));
            // Generate the assembly call
            out += "mov rdi, " + str2 + Chr(10) + "mov rsi, " + str3 + Chr(10) + "call get_user_input" + Chr(10);
        }
        else if (InStr(A_LoopField15, ".set ")) {
            str1 = Trim(StrSplit(A_LoopField15, ".", 1));
            str2 = Trim(StringTrimLeft(A_LoopField15, StrLen(str1) + 5));
            str_index = Trim(StrSplit(str2, ",", 1));
            str_value = Trim(StrSplit(str2, ",", 2));
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
        else if (SubStrLastChars(Trim(A_LoopField15), 1) == ")") {
            str1 = StringTrimRight(Trim(A_LoopField15), 1);
            str2 = StrSplit(str1, "(", 1);
            std::vector<std::string> items19 = LoopParseFunc(str2, " ");
            for (size_t A_Index19 = 0; A_Index19 < items19.size(); A_Index19++) {
                std::string A_LoopField19 = items19[A_Index19 - 0];
                str3 = A_LoopField19;
            }
            str2 = str3;
            str3 = StrSplit(str1, "(", 2);
            str4 = "";
            int5 = 0;
            int1 = 0;
            if (Trim(str3) == "") {
                int1 = 1;
            }
            std::vector<std::string> items20 = LoopParseFunc(str3, ", ");
            for (size_t A_Index20 = 0; A_Index20 < items20.size(); A_Index20++) {
                std::string A_LoopField20 = items20[A_Index20 - 0];
                if (RegExMatch(A_LoopField20, "^\\d+$")) {
                    str4 += "push " + A_LoopField20 + Chr(10);
                } else {
                    str4 += "push qword [" + A_LoopField20 + "]" + Chr(10);
                }
                int5++;
            }
            if (int1 == 0) {
                out += str4 + "call " + str2 + Chr(10) + "add rsp, " + STR(8 * int5) + Chr(10);
            } else {
                out += str4 + "call " + str2 + Chr(10);
            }
        } else {
            out += A_LoopField15 + Chr(10);
        }
        for (int A_Index21 = 0; A_Index21 < HTVM_Size(funcArgsArr); A_Index21++) {
            out = RegExReplace(out, "\\b" + Trim(funcArgsArr[A_Index21]) + "\\b", "rbp + " + STR(8 + ( (HTVM_Size(funcArgsArr) - A_Index21) * 8 )));
        }
        out = StrReplace(out, "']", "'");
        out = StrReplace(out, "['", "'");
        out = StrReplace(out, "push qword []", "");
        out = StrReplace(out, "[A_Index]", "r13");
    }
    code = StringTrimRight(out, 1);
    std::vector<std::string> allFuncCALLS;
    std::vector<std::string> allFuncCALLS_alredy;
    std::string code_TEMP = code + Chr(10) + main_syntax;
    std::vector<std::string> items22 = LoopParseFunc(code_TEMP, "\n", "\r");
    for (size_t A_Index22 = 0; A_Index22 < items22.size(); A_Index22++) {
        std::string A_LoopField22 = items22[A_Index22 - 0];
        if (SubStr(Trim(A_LoopField22), 1, 5) == "call " || SubStr(Trim(A_LoopField22), 1, 4) == "jmp ") {
            HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(A_LoopField22), 4)));
        }
    }
    std::string HTLL_Libs_x86_new = Chr(10);
    std::string temp_funcName = "";
    int temp_in_funcName = 0;
    int canWeGetFunc = 0;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    std::vector<std::string> items23 = LoopParseFunc(HTLL_Libs_x86, "\n", "\r");
    for (size_t A_Index23 = 0; A_Index23 < items23.size(); A_Index23++) {
        std::string A_LoopField23 = items23[A_Index23 - 0];
        if (InStr(A_LoopField23, "$$$$") == false && InStr(A_LoopField23, "%%%%") == false && temp_in_funcName == 1 && canWeGetFunc == 1) {
            HTLL_Libs_x86_new += A_LoopField23 + Chr(10);
        }
        if (InStr(A_LoopField23, "$$$$")) {
            temp_funcName = StringTrimLeft(StringTrimRight(Trim(A_LoopField23), 4), 4);
            temp_in_funcName = 1;
            canWeGetFunc = 0;
            for (int A_Index24 = 0; A_Index24 < HTVM_Size(allFuncCALLS); A_Index24++) {
                if (temp_funcName == allFuncCALLS[A_Index24]) {
                    canWeGetFunc = 1;
                    HTVM_Append(allFuncCALLS_alredy, temp_funcName);
                    break;
                }
            }
        }
        else if (InStr(A_LoopField23, "%%%%")) {
            temp_in_funcName = 0;
        }
    }
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    allFuncCALLS = {};
    int anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 0;
    std::string ALoopField = "";
    std::vector<std::string> items25 = LoopParseFunc(HTLL_Libs_x86_new, "\n", "\r");
    for (size_t A_Index25 = 0; A_Index25 < items25.size(); A_Index25++) {
        std::string A_LoopField25 = items25[A_Index25 - 0];
        ALoopField = A_LoopField25;
        if (SubStr(Trim(A_LoopField25), 1, 5) == "call " || SubStr(Trim(A_LoopField25), 1, 4) == "jmp ") {
            anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 1;
            for (int A_Index26 = 0; A_Index26 < HTVM_Size(allFuncCALLS_alredy); A_Index26++) {
                if (allFuncCALLS_alredy[A_Index26] == Trim(StringTrimLeft(Trim(ALoopField), 4))) {
                    anotherVar_INT_HELP_BUILD_IN_FUNCS_INT = 0;
                }
            }
            if (anotherVar_INT_HELP_BUILD_IN_FUNCS_INT == 1) {
                HTVM_Append(allFuncCALLS, Trim(StringTrimLeft(Trim(ALoopField), 4)));
            }
        }
    }
    temp_funcName = "";
    temp_in_funcName = 0;
    canWeGetFunc = 0;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    std::vector<std::string> items27 = LoopParseFunc(HTLL_Libs_x86, "\n", "\r");
    for (size_t A_Index27 = 0; A_Index27 < items27.size(); A_Index27++) {
        std::string A_LoopField27 = items27[A_Index27 - 0];
        if (InStr(A_LoopField27, "$$$$") == false && InStr(A_LoopField27, "%%%%") == false && temp_in_funcName == 1 && canWeGetFunc == 1) {
            HTLL_Libs_x86_new += A_LoopField27 + Chr(10);
        }
        if (InStr(A_LoopField27, "$$$$")) {
            temp_funcName = StringTrimLeft(StringTrimRight(Trim(A_LoopField27), 4), 4);
            temp_in_funcName = 1;
            canWeGetFunc = 0;
            for (int A_Index28 = 0; A_Index28 < HTVM_Size(allFuncCALLS); A_Index28++) {
                if (temp_funcName == allFuncCALLS[A_Index28]) {
                    canWeGetFunc = 1;
                    break;
                }
            }
        }
        else if (InStr(A_LoopField27, "%%%%")) {
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
    std::string fasm_header = "";
    // --- FASM OBJECT FILE HEADER & LINKER DIRECTIVES ---
    if (isDotCompile == 1) {
        fasm_header = "format ELF64" + Chr(10);
        fasm_header += "public _start" + Chr(10);
    } else {
        fasm_header = "format ELF64 executable 3" + Chr(10);
        fasm_header += "entry _start" + Chr(10);
    }
    if (isDotCompile == 1) {
        fasm_header += "extrn compiler_c" + Chr(10);
        fasm_header += "extrn free_string_c" + Chr(10);
    }
    fasm_header += Chr(10);
    // --- FASM STRUCTURE DEFINITION ---
    fasm_header += "    DynamicArray.pointer  = 0" + Chr(10) + "    DynamicArray.size     = 8" + Chr(10) + "    DynamicArray.capacity = 16" + Chr(10) + "    sizeof.DynamicArray   = 24" + Chr(10) + Chr(10);
    // #################### THE FINAL TRUTH ####################
    // Use simple section names WITH the correct ELF flags for object files.
    // #########################################################
    std::string upCode = "";
    // --- FASM BODY ---
    if (isDotCompile == 1) {
        upCode = "section '.data' writeable" + Chr(10) + "    SCALE_FACTOR   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY = 2" + Chr(10) + "    print_buffer   rb 21" + Chr(10) + "    dot            db " + Chr(39) + "." + Chr(39) + Chr(10) + "    minus_sign     db " + Chr(39) + "-" + Chr(39) + " " + Chr(10) + "    nl   db 10" + Chr(10) + dot_data + Chr(10);
        upCode += "section '.bss' writeable" + Chr(10) + "    input_buffer rb 256" + Chr(10) + "    file_read_buffer rb 4096" + Chr(10) + "    input_len    rq 1" + Chr(10) + "    filename_ptr_size  rq 1" + Chr(10) + "    source_ptr      rq 1" + Chr(10) + "    source_ptr_size rq 1" + Chr(10) + "    args_array rq 3" + Chr(10) + "    filename_ptr      rq 1" + Chr(10) + "    asm_code_ptr    rq 1" + Chr(10) + "    print_buffer_n rb 20" + Chr(10) + arrBss + dot_bss;
        upCode += "section '.text' executable" + Chr(10) + HTLL_Libs_x86_new + Chr(10);
    } else {
        upCode = "segment readable writeable" + Chr(10) + "    SCALE_FACTOR   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY = 2" + Chr(10) + "    print_buffer   rb 21" + Chr(10) + "    dot            db " + Chr(39) + "." + Chr(39) + "" + Chr(10) + "    minus_sign     db " + Chr(39) + "-" + Chr(39) + "" + Chr(10) + "    nl   db 10" + Chr(10) + dot_data + Chr(10);
        upCode += Chr(10) + Chr(10) + "    input_buffer rb 256" + Chr(10) + "    file_read_buffer rb 4096" + Chr(10) + "    input_len    rq 1" + Chr(10) + "    filename_ptr_size  rq 1" + Chr(10) + "    source_ptr      rq 1" + Chr(10) + "    source_ptr_size rq 1" + Chr(10) + "    args_array rq 3" + Chr(10) + "    filename_ptr      rq 1" + Chr(10) + "    asm_code_ptr    rq 1" + Chr(10) + "    print_buffer_n rb 20" + Chr(10) + arrBss + dot_bss;
        upCode += "segment readable executable" + Chr(10) + HTLL_Libs_x86_new + Chr(10);
    }
    // --- FASM FOOTER ---
    std::string downCode = Chr(10) + "    mov rsp, rbp" + Chr(10) + "    pop rbp" + Chr(10) + "    mov rax, 60" + Chr(10) + "    xor rdi, rdi" + Chr(10) + "    syscall" + Chr(10) + "";
    // --- ASSEMBLE THE FINAL STRING ---
    std::string codeOUT = "";
    if (seenMain == 0) {
        codeOUT = fasm_header + upCode + main_syntax + out + downCode;
    } else {
        codeOUT = fasm_header + upCode + out + downCode;
    }
    //;;;;;;;;;;;;;;;;;;;;;;;;;
    for (int A_Index29 = 0; A_Index29 < theIdNumOfThe34; A_Index29++) {
        if (theIdNumOfThe34 == A_Index29 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index29 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index29 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index29 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index29 + 1], keyWordEscpaeChar, "\\"));
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
    std::vector<std::string> items30 = LoopParseFunc(codeOUT, "\n", "\r");
    for (size_t A_Index30 = 0; A_Index30 < items30.size(); A_Index30++) {
        std::string A_LoopField30 = items30[A_Index30 - 0];
        if (SubStr(Trim(A_LoopField30), 1, 2) != "; ") {
            allInOne_temp += Trim(A_LoopField30) + Chr(10);
            // Ignore definitions so they don't count as usage
            if (InStr(A_LoopField30, " db ") == 0 && InStr(A_LoopField30, " rb ") == 0 && InStr(A_LoopField30, " rq ") == 0 && InStr(A_LoopField30, " dq ") == 0 && InStr(A_LoopField30, " = ") == 0) {
                // CHANGED ALL "else if" TO "if" TO PREVENT OVERLAPPING REGEX BUGS
                if (RegExMatch(A_LoopField30, "\\bDynamicArray.pointer\\b")) {
                    is_1_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bDynamicArray.size\\b")) {
                    is_2_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bDynamicArray.capacity\\b")) {
                    is_3_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bsizeof.DynamicArray\\b")) {
                    is_4_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bSCALE" + Chr(95) + "FACTOR\\b")) {
                    is_5_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bINITIAL" + Chr(95) + "CAPACITY\\b")) {
                    is_6_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bprint" + Chr(95) + "buffer\\b")) {
                    is_7_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bdot\\b")) {
                    is_8_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bminus" + Chr(95) + "sign\\b")) {
                    is_9_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\b" + Chr(110) + Chr(108) + "\\b")) {
                    is_10_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\binput" + Chr(95) + "buffer\\b")) {
                    is_11_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bfile" + Chr(95) + "read" + Chr(95) + "buffer\\b")) {
                    is_12_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\binput" + Chr(95) + "len\\b")) {
                    is_13_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bfilename" + Chr(95) + "ptr" + Chr(95) + "size\\b")) {
                    is_14_optimize = 1;
                }
                // FIXED REGEX FOR SOURCE_PTR (is_15)
                if (RegExMatch(A_LoopField30, "\\bsource" + Chr(95) + "ptr\\b")) {
                    is_15_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bsource" + Chr(95) + "ptr" + Chr(95) + "size\\b")) {
                    is_16_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bargs" + Chr(95) + "array\\b")) {
                    is_17_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bfilename" + Chr(95) + "ptr\\b")) {
                    is_18_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\basm" + Chr(95) + "code" + Chr(95) + "ptr\\b")) {
                    is_19_optimize = 1;
                }
                if (RegExMatch(A_LoopField30, "\\bprint" + Chr(95) + "buffer" + Chr(95) + "n\\b")) {
                    is_20_optimize = 1;
                }
            }
        }
    }
    codeOUT = Trim(allInOne_temp);
    allInOne_temp = "";
    int aredy_int = 0;
    std::vector<std::string> items31 = LoopParseFunc(codeOUT, "\n", "\r");
    for (size_t A_Index31 = 0; A_Index31 < items31.size(); A_Index31++) {
        std::string A_LoopField31 = items31[A_Index31 - 0];
        aredy_int = 0;
        if (Trim(A_LoopField31) == "DynamicArray.pointer  = 0" && is_1_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "DynamicArray.size     = 8" && is_2_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "DynamicArray.capacity = 16" && is_3_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "sizeof.DynamicArray   = 24" && is_4_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "SCALE_FACTOR   dq 1000000" && is_5_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "INITIAL_CAPACITY = 2" && is_6_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "print_buffer   rb 21" && is_7_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "dot            db " + Chr(39) + "." + Chr(39) + "" && is_8_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "minus_sign     db " + Chr(39) + Chr(45) + Chr(39) && is_9_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == Chr(110) + Chr(108) + "   db 10" && is_10_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "input_buffer rb 256" && is_11_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "file_read_buffer rb 4096" && is_12_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "input_len    rq 1" && is_13_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "filename_ptr_size  rq 1" && is_14_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "source_ptr      rq 1" && is_15_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "source_ptr_size rq 1" && is_16_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "args_array rq 3" && is_17_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "filename_ptr      rq 1" && is_18_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "asm_code_ptr    rq 1" && is_19_optimize == 0) {
            aredy_int = 1;
        }
        else if (Trim(A_LoopField31) == "print_buffer_n rb 20" && is_20_optimize == 0) {
            aredy_int = 1;
        }
        if (aredy_int == 0) {
            allInOne_temp += A_LoopField31 + Chr(10);
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
        std::vector<std::string> items32 = LoopParseFunc(params, "\n", "\r");
        for (size_t A_Index32 = 0; A_Index32 < items32.size(); A_Index32++) {
            std::string A_LoopField32 = items32[A_Index32 - 0];
            paramsTemp = Trim(A_LoopField32);
            if (A_Index32 == 0) {
                FileDelete("finalASM_HTLL_ASM.s");
                FileAppend(compiler(FileRead(paramsTemp)), "finalASM_HTLL_ASM.s");
                print("Compilation finished: finalASM_HTLL_ASM.s generated.");
            }
        }
    }
    

    return 0;
}