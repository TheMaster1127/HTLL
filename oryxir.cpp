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
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
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

// Convert std::string to int
int INT(const std::string& str) {
    std::istringstream iss(str);
    int value;
    iss >> value;
    return value;
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

// Convert std::string to float
float FLOAT(const std::string& str) {
    std::istringstream iss(str);
    float value;
    iss >> value;
    return value;
}

// Function to find the position of needle in haystack (std::string overload)
int InStr(const std::string& haystack, const std::string& needle) {
    size_t pos = haystack.find(needle);
    return (pos != std::string::npos) ? static_cast<int>(pos) + 1 : 0;
}

// Function to sleep for a specified number of milliseconds
void Sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Function to get input from the user, similar to Python's input() function
std::string input(const std::string& prompt) {
    std::string userInput;
    std::cout << prompt; // Display the prompt to the user
    std::getline(std::cin, userInput); // Get the entire line of input
    return userInput;
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

int Asc(const std::string& str) {
    if (!str.empty()) {
        return static_cast<int>(str[0]);
    }
    return -1; // Return -1 if the string is empty
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

std::string HTVM_getLang_HTVM() {
    return "cpp";
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

void HTVM_Pop(std::vector<std::string>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<int>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<float>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<bool>& arr) {
    if (!arr.empty()) arr.pop_back();
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

// Remove by index for std::vector<std::string>
void HTVM_Remove(std::vector<std::string>& arr, size_t index) {
    if (index < arr.size()) {
        arr.erase(arr.begin() + index);
    }
}
// Remove by index for std::vector<int>
void HTVM_Remove(std::vector<int>& arr, size_t index) {
    if (index < arr.size()) {
        arr.erase(arr.begin() + index);
    }
}
// Remove by index for std::vector<float>
void HTVM_Remove(std::vector<float>& arr, size_t index) {
    if (index < arr.size()) {
        arr.erase(arr.begin() + index);
    }
}
// Remove by index for std::vector<bool>
void HTVM_Remove(std::vector<bool>& arr, size_t index) {
    if (index < arr.size()) {
        arr.erase(arr.begin() + index);
    }
}



// start of HT-Lib.htvm
// global vars NEEDED
int HT_LIB_theIdNumOfThe34 = 0;
std::vector<std::string> HT_Lib_theIdNumOfThe34theVar;
//;;;;;;;;;;;;;;;;;;;;;;;;;
std::string preserveStrings(std::string code, std::string keyWordEscpaeChar = "\\") {
    HT_LIB_theIdNumOfThe34 = 0;
    HT_Lib_theIdNumOfThe34theVar = {};
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
    if (params == "") {
        if (isWindows()) {
            print("Usage:" + Chr(10) + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        } else {
            print("Usage:" + Chr(10) + "./" + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        }
        return "";
    } else {
        // Loop through the parameters and return the VERY FIRST one.
        std::vector<std::string> items9 = LoopParseFunc(params, "\n", "\r");
        for (size_t A_Index9 = 0; A_Index9 < items9.size(); A_Index9++) {
            std::string A_LoopField9 = items9[A_Index9 - 0];
            std::string first_param = Trim(A_LoopField9);
            if (first_param != "") {
                return first_param;
            }
        }
        // If all lines were empty for some reason, return nothing.
        return "";
    }
    // This is now unreachable, but good practice to keep.
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
    std::vector<std::string> items10 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index10 = 0; A_Index10 < items10.size(); A_Index10++) {
        std::string A_LoopField10 = items10[A_Index10 - 0];
        str1 += StrSplit(A_LoopField10, commentKeyword, 1) + Chr(10);
    }
    code = StringTrimRight(str1, 1);
    return code;
}
// Define the function to check odd spaces at the beginning
std::string CheckOddLeadingSpaces(std::string string123) {
    // Initialize a variable to count the spaces
    int spaceCount = 0;
    // Loop through the string one character at a time
    std::vector<std::string> items11 = LoopParseFunc(string123);
    for (size_t A_Index11 = 0; A_Index11 < items11.size(); A_Index11++) {
        std::string A_LoopField11 = items11[A_Index11 - 0];
        // Check if the current character is a space
        if (A_LoopField11 == Chr(32)) {
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
    for (int A_Index12 = 0; A_Index12 < count; A_Index12++) {
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
    std::vector<std::string> items13 = LoopParseFunc(input_string, "\n", "\r");
    for (size_t A_Index13 = 0; A_Index13 < items13.size(); A_Index13++) {
        std::string A_LoopField13 = items13[A_Index13 - 0];
        trimmed_line = Trim(A_LoopField13);
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
        std::vector<std::string> items14 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index14 = 0; A_Index14 < items14.size(); A_Index14++) {
            std::string A_LoopField14 = items14[A_Index14 - 0];
            if (Trim(A_LoopField14) != "{" && Trim(A_LoopField14) != "}") {
                resultOut += A_LoopField14 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    } else {
        // format curly braces in a K&R style
        std::vector<std::string> lookIntoFurture;
        std::vector<std::string> items15 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index15 = 0; A_Index15 < items15.size(); A_Index15++) {
            std::string A_LoopField15 = items15[A_Index15 - 0];
            HTVM_Append(lookIntoFurture, Trim(A_LoopField15));
        }
        HTVM_Append(lookIntoFurture, " ");
        std::string resultOut = "";
        int skipNext = 0;
        std::vector<std::string> items16 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index16 = 0; A_Index16 < items16.size(); A_Index16++) {
            std::string A_LoopField16 = items16[A_Index16 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(lookIntoFurture[A_Index16 + 1]) == "{") {
                resultOut += A_LoopField16 + " " + culyOpenFix + Chr(10);
                skipNext = 2;
            }
            if (skipNext == 0) {
                resultOut += A_LoopField16 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
        std::vector<std::string> lookIntoFurture2;
        std::vector<std::string> items17 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index17 = 0; A_Index17 < items17.size(); A_Index17++) {
            std::string A_LoopField17 = items17[A_Index17 - 0];
            HTVM_Append(lookIntoFurture2, Trim(A_LoopField17));
        }
        HTVM_Append(lookIntoFurture2, " ");
        resultOut = "";
        skipNext = 0;
        std::string addSpacesAtTheBegginig = "";
        std::vector<std::string> items18 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index18 = 0; A_Index18 < items18.size(); A_Index18++) {
            std::string A_LoopField18 = items18[A_Index18 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(A_LoopField18) == "}" && Trim(lookIntoFurture2[A_Index18 + 1]) == "else {") {
                skipNext = 2;
                addSpacesAtTheBegginig = "";
                std::vector<std::string> items19 = LoopParseFunc(A_LoopField18);
                for (size_t A_Index19 = 0; A_Index19 < items19.size(); A_Index19++) {
                    std::string A_LoopField19 = items19[A_Index19 - 0];
                    if (A_LoopField19 == " ") {
                        if (A_LoopField19 != " ") {
                            break;
                        }
                        addSpacesAtTheBegginig += StrReplace(A_LoopField19, "}", culyCloseFix);
                    }
                }
                resultOut += addSpacesAtTheBegginig + culyCloseFix + " else " + culyOpenFix + Chr(10);
            }
            if (skipNext == 0) {
                resultOut += A_LoopField18 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    }
    resultOut = "";
    std::string ALoopField = "";
    std::vector<std::string> items20 = LoopParseFunc(result, "\n", "\r");
    for (size_t A_Index20 = 0; A_Index20 < items20.size(); A_Index20++) {
        std::string A_LoopField20 = items20[A_Index20 - 0];
        if (CheckOddLeadingSpaces(A_LoopField20) == "1") {
            ALoopField = StringTrimLeft(A_LoopField20, 1);
            resultOut += ALoopField + Chr(10);
        } else {
            resultOut += A_LoopField20 + Chr(10);
        }
    }
    result = StringTrimRight(resultOut, 1);
    // Return the result
    return result;
}
// end of HT-Lib.htvm
std::vector<std::string> Official_Oryx_VM_symbol_table;
std::vector<std::string> Official_Oryx_VM_reg_array;
// --- API STATE CACHE ---
// These globals store the state of the *last* completed interpreter run.
std::vector<std::string> Official_Oryx_VM_API_symbol_table_cache;
void print_raw(std::string value) {
            std::cout << value;
    
    
}
// --- HELPER: SAFE TOKEN GETTER ---
// Replaces StrSplit(str, delim, index) to avoid crashes on missing parts.
std::string get_token(std::string input, std::string delim, int index) {
    int current = 1;
    std::vector<std::string> items21 = LoopParseFunc(input, delim);
    for (size_t A_Index21 = 0; A_Index21 < items21.size(); A_Index21++) {
        std::string A_LoopField21 = items21[A_Index21 - 0];
        if (current == index) {
            return A_LoopField21;
        }
        current++;
    }
    return "";
}
// --- HELPER FUNCTION: GET VALUE ---
std::string get_value(std::string operand) {
    if (RegExMatch(operand, "^r\\d+$")) {
        int reg_index = INT(StringTrimLeft(operand, 1));
        return Official_Oryx_VM_reg_array[reg_index];
    }
    else if (RegExMatch(operand, "^-?\\d+(\\.\\d+)?$")) {
        return operand;
    }
    else if (SubStr(operand, 1, 1) == Chr(34)) {
        std::string val = StringTrimLeft(operand, 1);
        val = StringTrimRight(val, 1);
        val = StrReplace(val, "\\n", Chr(10));
        val = StrReplace(val, "\\r", Chr(13));
        val = StrReplace(val, "\\t", Chr(9));
        return val;
    } else {
        int i = 0;
        for (int A_Index22 = 0; A_Index22 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index22++) {
            std::string entry = Official_Oryx_VM_symbol_table[i];
            if (get_token(entry, Chr(254), 1) == operand) {
                return get_token(entry, Chr(254), 3);
            }
            i++;
        }
    }
    return "";
}
// --- HELPER FUNCTION: SET VALUE ---
void set_value(std::string dest_operand, std::string new_value) {
    if (RegExMatch(dest_operand, "^r\\d+$")) {
        int reg_index = INT(StringTrimLeft(dest_operand, 1));
        Official_Oryx_VM_reg_array[reg_index] = new_value;
    } else {
        int i = 0;
        for (int A_Index23 = 0; A_Index23 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index23++) {
            std::string entry = Official_Oryx_VM_symbol_table[i];
            std::string var_name = get_token(entry, Chr(254), 1);
            if (var_name == dest_operand) {
                std::string var_type = get_token(entry, Chr(254), 2);
                Official_Oryx_VM_symbol_table[i] = var_name + Chr(254) + var_type + Chr(254) + new_value;
                return;
            }
            i++;
        }
    }
}
// --- NEW API FUNCTION: GET VARIABLE ---
// Retrieves the final value of a variable from the last interpreter run.
// @param str var_name The name of the variable (e.g., "my_var").
// @return str The final value of the variable as a string. Returns "" if not found.
std::string OryxAPI_GetVariable(std::string operand) {
    if (RegExMatch(operand, "^r\\d+$")) {
        int reg_index = INT(StringTrimLeft(operand, 1));
        return Official_Oryx_VM_reg_array[reg_index];
    }
    else if (RegExMatch(operand, "^-?\\d+(\\.\\d+)?$")) {
        return operand;
    }
    else if (SubStr(operand, 1, 1) == Chr(34)) {
        std::string val = StringTrimLeft(operand, 1);
        val = StringTrimRight(val, 1);
        val = StrReplace(val, "\\n", Chr(10));
        val = StrReplace(val, "\\r", Chr(13));
        val = StrReplace(val, "\\t", Chr(9));
        return val;
    } else {
        int i = 0;
        for (int A_Index24 = 0; A_Index24 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index24++) {
            std::string entry = Official_Oryx_VM_symbol_table[i];
            if (get_token(entry, Chr(254), 1) == operand) {
                return get_token(entry, Chr(254), 3);
            }
            i++;
        }
    }
    return "";
}
// --- NEW API FUNCTION: GET ARRAY ---
// Retrieves the final contents of an array from the last interpreter run.
// @param str arr_name The name of the array (e.g., "my_list").
// @return arr str An array containing all the elements. Returns an empty array if not found.
std::vector<std::string> OryxAPI_GetArray(std::string arr_name) {
    std::vector<std::string> result_array;
    std::string content_string = "";
    int i = 0;
    for (int A_Index25 = 0; A_Index25 < HTVM_Size(Official_Oryx_VM_API_symbol_table_cache); A_Index25++) {
        std::string entry = Official_Oryx_VM_API_symbol_table_cache[i];
        // Check if it's an array and the name matches
        if (get_token(entry, Chr(254), 1) == arr_name && get_token(entry, Chr(254), 2) == "arr") {
            content_string = get_token(entry, Chr(254), 3);
            break;
        }
        i++;
    }
    if (content_string != "") {
        std::vector<std::string> items26 = LoopParseFunc(content_string, Chr(253));
        for (size_t A_Index26 = 0; A_Index26 < items26.size(); A_Index26++) {
            std::string A_LoopField26 = items26[A_Index26 - 0];
            HTVM_Append(result_array, A_LoopField26);
        }
    }
    return result_array;
}
// This is the gatekeeper for all external string-to-variable operations.
// It checks the destination type and formats the data accordingly.
void set_value_type_aware(std::string dest_operand, std::string new_value) {
    if (RegExMatch(dest_operand, "^r\\d+$")) {
        // Registers are typeless. Set the value directly.
        int reg_index = INT(StringTrimLeft(dest_operand, 1));
        Official_Oryx_VM_reg_array[reg_index] = new_value;
        return;
    }
    // Find the destination variable's type from the symbol table.
    std::string dest_type = "";
    int i = 0;
    for (int A_Index27 = 0; A_Index27 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index27++) {
        std::string entry = Official_Oryx_VM_symbol_table[i];
        if (get_token(entry, Chr(254), 1) == dest_operand) {
            dest_type = get_token(entry, Chr(254), 2);
            break;
        }
        i++;
    }
    if (dest_type == "arr") {
        // --- ARRAY-AWARE LOGIC ---
        // The destination is an array. Convert the raw string `new_value`
        // into a Chr(253)-delimited string of ASCII values.
        std::string numeric_content = "";
        std::vector<std::string> items28 = LoopParseFunc(new_value);
        for (size_t A_Index28 = 0; A_Index28 < items28.size(); A_Index28++) {
            std::string A_LoopField28 = items28[A_Index28 - 0];
            if (numeric_content != "") {
                numeric_content += Chr(253);
            }
            numeric_content += STR(Asc(A_LoopField28));
        }
        set_value(dest_operand, numeric_content);
    } else {
        // --- DEFAULT LOGIC ---
        // The destination is a string, int, or float.
        // Use the original "type-blind" setter.
        set_value(dest_operand, new_value);
    }
}
std::string Oryx_interpreter(std::string code) {
    std::string outState = "success";
    std::string str1 = "";
    std::string str2 = "";
    std::string str3 = "";
    std::string str4 = "";
    std::string ALoopField = "";
    std::string current_line = "";
    std::vector<std::string> all_instr;
    std::vector<std::string> label_table;
    int zero_flag = 0;
    int sign_flag = 0;
    std::vector<int> call_stack;
    std::vector<std::string> main_stack;
    for (int A_Index29 = 0; A_Index29 < 100; A_Index29++) {
        HTVM_Append(Official_Oryx_VM_reg_array, "");
    }
    std::vector<std::string> items30 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index30 = 0; A_Index30 < items30.size(); A_Index30++) {
        std::string A_LoopField30 = items30[A_Index30 - 0];
        HTVM_Append(all_instr, A_LoopField30);
    }
    // --- POPULATE ARGS_ARRAY (Corrected to mimic x86 byte buffer) ---
    std::string args_init_content = "";
    std::string sys_params = GetParams();
    std::vector<std::string> items31 = LoopParseFunc(sys_params, "\n", "\r");
    for (size_t A_Index31 = 0; A_Index31 < items31.size(); A_Index31++) {
        std::string A_LoopField31 = items31[A_Index31 - 0];
        // Skip the first argument (the script file itself)
        if (A_Index31 == 0) {
            continue;
        }
        std::string current_arg = Trim(A_LoopField31);
        if (current_arg != "") {
            // --- UNPACK BYTES ---
            // Loop through each character of the argument string
            std::vector<std::string> items32 = LoopParseFunc(current_arg);
            for (size_t A_Index32 = 0; A_Index32 < items32.size(); A_Index32++) {
                std::string A_LoopField32 = items32[A_Index32 - 0];
                if (args_init_content != "") {
                    args_init_content += Chr(253);
                }
                // Add the character's ASCII value to the array content
                args_init_content += STR(Asc(A_LoopField32));
            }
            // --- APPEND SEPARATOR ---
            // After each full argument, append a newline character (ASCII 10)
            // to match the x86 behavior.
            if (args_init_content != "") {
                args_init_content += Chr(253);
            }
            args_init_content += "10";
        }
    }
    HTVM_Append(Official_Oryx_VM_symbol_table, "args_array" + Chr(254) + "arr" + Chr(254) + args_init_content);
    int i = 0;
    for (int A_Index33 = 0; A_Index33 < HTVM_Size(all_instr); A_Index33++) {
        ALoopField = Trim(all_instr[i]);
        if (SubStr(ALoopField, -1) == ":") {
            std::string label_name = StringTrimRight(ALoopField, 1);
            HTVM_Append(label_table, label_name + Chr(254) + STR(i));
        }
        i++;
    }
    int pc = 0;
    for (int A_Index34 = 0; ; A_Index34++) {
        if (pc >= HTVM_Size(all_instr)) {
            break;
        }
        current_line = Trim(all_instr[pc]);
        bool pc_was_modified = false;
        if (current_line == "" || SubStr(current_line, -1) == ":") {
            pc++;
            continue;
        }
        if (SubStr(current_line, 1, 5) == "meta ") {
            // The meta instruction is a NOP for the VM.
            // Its payload is for external debuggers.
            // Simply skip it and move to the next instruction.
            pc++;
            continue;
        }
        if (SubStr(current_line, 1, 7) == "string ") {
            str1 = StringTrimLeft(current_line, 7);
            int first_colon = InStr(str1, ":");
            str2 = Trim(SubStr(str1, 1, first_colon - 1));
            str3 = Trim(SubStr(str1, first_colon + 1));
            str3 = StringTrimLeft(str3, 1);
            str3 = StringTrimRight(str3, 1);
            str3 = StrReplace(str3, "\\n", Chr(10));
            str3 = StrReplace(str3, "\\r", Chr(13));
            str3 = StrReplace(str3, "\\t", Chr(9));
            HTVM_Append(Official_Oryx_VM_symbol_table, str2 + Chr(254) + "string" + Chr(254) + str3);
        }
        if (SubStr(current_line, 1, 4) == "int ") {
            str1 = StringTrimLeft(current_line, 4);
            int first_colon = InStr(str1, ":");
            str2 = Trim(SubStr(str1, 1, first_colon - 1));
            str3 = Trim(SubStr(str1, first_colon + 1));
            HTVM_Append(Official_Oryx_VM_symbol_table, str2 + Chr(254) + "int" + Chr(254) + str3);
        }
        if (SubStr(current_line, 1, 6) == "float ") {
            str1 = StringTrimLeft(current_line, 6);
            int first_colon = InStr(str1, ":");
            str2 = Trim(SubStr(str1, 1, first_colon - 1));
            str3 = Trim(SubStr(str1, first_colon + 1));
            HTVM_Append(Official_Oryx_VM_symbol_table, str2 + Chr(254) + "float" + Chr(254) + str3);
        }
        if (SubStr(current_line, 1, 4) == "arr ") {
            str1 = StringTrimLeft(current_line, 4);
            HTVM_Append(Official_Oryx_VM_symbol_table, str1 + Chr(254) + "arr" + Chr(254) + "");
        }
        if (SubStr(current_line, 1, 4) == "mov ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1 = "";
            std::string op2 = "";
            // --- FIX: Manually parse to respect commas in string literals ---
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1 = Trim(SubStr(str1, 1, first_comma - 1));
                op2 = Trim(SubStr(str1, first_comma + 1));
            } else {
                // This would be a syntax error, but we'll handle it gracefully
                op1 = Trim(str1);
            }
            // --- END FIX ---
            if (op2 != "") {
                std::string value_to_move = get_value(op2);
                // --- REPLACE THIS LINE ---
                // set_value(op1, value_to_move)
                // --- WITH THIS LINE ---
                set_value_type_aware(op1, value_to_move);
            }
        }
        if (SubStr(current_line, 1, 4) == "add ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1 = Trim(get_token(str1, ",", 1));
            std::string op2 = Trim(get_token(str1, ",", 2));
            float val1 = FLOAT(get_value(op1));
            float val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 + val2));
        }
        if (SubStr(current_line, 1, 4) == "sub ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1 = Trim(get_token(str1, ",", 1));
            std::string op2 = Trim(get_token(str1, ",", 2));
            float val1 = FLOAT(get_value(op1));
            float val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 - val2));
        }
        if (SubStr(current_line, 1, 4) == "mul ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1 = Trim(get_token(str1, ",", 1));
            std::string op2 = Trim(get_token(str1, ",", 2));
            float val1 = FLOAT(get_value(op1));
            float val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 * val2));
        }
        if (SubStr(current_line, 1, 4) == "div ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1 = Trim(get_token(str1, ",", 1));
            std::string op2 = Trim(get_token(str1, ",", 2));
            float val1 = FLOAT(get_value(op1));
            float val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 / val2));
        }
        if (SubStr(current_line, 1, 8) == "add_str ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string op1 = "";
            std::string op2 = "";
            // --- FIX: Manually parse to respect commas in string literals ---
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1 = Trim(SubStr(str1, 1, first_comma - 1));
                op2 = Trim(SubStr(str1, first_comma + 1));
            } else {
                op1 = Trim(str1);
            }
            // --- END FIX ---
            if (op2 != "") {
                std::string s1 = get_value(op1);
                std::string s2 = get_value(op2);
                set_value(op1, s1 + s2);
            }
        }
        if (SubStr(current_line, 1, 4) == "inc ") {
            std::string op1 = Trim(StringTrimLeft(current_line, 4));
            float val = FLOAT(get_value(op1));
            set_value(op1, STR(val + 1));
        }
        if (SubStr(current_line, 1, 4) == "dec ") {
            std::string op1 = Trim(StringTrimLeft(current_line, 4));
            float val = FLOAT(get_value(op1));
            set_value(op1, STR(val - 1));
        }
        if (SubStr(current_line, 1, 4) == "cmp ") {
            str1 = StringTrimLeft(current_line, 4);
            std::string op1_str = "";
            std::string op2_str = "";
            // --- FIX: Use manual parsing to handle potential commas in string literals ---
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1_str = Trim(SubStr(str1, 1, first_comma - 1));
                op2_str = Trim(SubStr(str1, first_comma + 1));
            } else {
                // This would be a syntax error, but we'll handle it gracefully
                op1_str = Trim(str1);
            }
            std::string s_val1 = get_value(op1_str);
            std::string s_val2 = get_value(op2_str);
            // --- FIX: Check if we are comparing numbers or strings ---
            if (RegExMatch(s_val1, "^-?\\d+(\\.\\d+)?$") && RegExMatch(s_val2, "^-?\\d+(\\.\\d+)?$")) {
                // --- Numeric Comparison ---
                float val1 = FLOAT(s_val1);
                float val2 = FLOAT(s_val2);
                if (val1 == val2) {
                    zero_flag = 1;
                } else {
                    zero_flag = 0;
                }
                if (val1 < val2) {
                    sign_flag = 1;
                } else {
                    sign_flag = 0;
                }
            } else {
                // --- String Comparison ---
                if (s_val1 == s_val2) {
                    zero_flag = 1;
                } else {
                    zero_flag = 0;
                }
                if (s_val1 < s_val2) {
                    sign_flag = 1;
                } else {
                    sign_flag = 0;
                }
            }
        }
        if (SubStr(current_line, 1, 4) == "jmp ") {
            pc_was_modified = true;
            std::string label_name = Trim(StringTrimLeft(current_line, 4));
            int j = 0;
            for (int A_Index35 = 0; A_Index35 < HTVM_Size(label_table); A_Index35++) {
                if (get_token(label_table[j], Chr(254), 1) == label_name) {
                    pc = INT(get_token(label_table[j], Chr(254), 2));
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 3) == "je ") {
            if (zero_flag == 1) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 3));
                int j = 0;
                for (int A_Index36 = 0; A_Index36 < HTVM_Size(label_table); A_Index36++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 4) == "jne ") {
            if (zero_flag == 0) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 4));
                int j = 0;
                for (int A_Index37 = 0; A_Index37 < HTVM_Size(label_table); A_Index37++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 3) == "jg ") {
            if (sign_flag == 0 && zero_flag == 0) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 3));
                int j = 0;
                for (int A_Index38 = 0; A_Index38 < HTVM_Size(label_table); A_Index38++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 3) == "jl ") {
            if (sign_flag == 1) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 3));
                int j = 0;
                for (int A_Index39 = 0; A_Index39 < HTVM_Size(label_table); A_Index39++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 4) == "jge ") {
            if (sign_flag == 0) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 4));
                int j = 0;
                for (int A_Index40 = 0; A_Index40 < HTVM_Size(label_table); A_Index40++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 4) == "jle ") {
            if (sign_flag == 1 || zero_flag == 1) {
                pc_was_modified = true;
                std::string label_name = Trim(StringTrimLeft(current_line, 4));
                int j = 0;
                for (int A_Index41 = 0; A_Index41 < HTVM_Size(label_table); A_Index41++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (SubStr(current_line, 1, 6) == "input ") {
            str1 = StringTrimLeft(current_line, 6);
            std::string dest = "";
            std::string prompt_source = "";
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                dest = Trim(SubStr(str1, 1, first_comma - 1));
                prompt_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                dest = Trim(str1);
            }
            std::string prompt_msg = get_value(prompt_source);
            std::string user_input = input(prompt_msg);
            set_value_type_aware(dest, user_input);
        }
        if (SubStr(current_line, 1, 10) == "file.read ") {
            str1 = StringTrimLeft(current_line, 10);
            std::string dest = "";
            std::string path_source = "";
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                dest = Trim(SubStr(str1, 1, first_comma - 1));
                path_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                dest = Trim(str1);
            }
            std::string filepath = get_value(path_source);
            std::string content = FileRead(filepath);
            // --- THE FIX: CHECK DESTINATION TYPE ---
            std::string dest_type = "";
            int sym_idx = 0;
            for (int A_Index42 = 0; A_Index42 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index42++) {
                if (get_token(Official_Oryx_VM_symbol_table[sym_idx], Chr(254), 1) == dest) {
                    dest_type = get_token(Official_Oryx_VM_symbol_table[sym_idx], Chr(254), 2);
                    break;
                }
                sym_idx++;
            }
            if (dest_type == "arr") {
                // Destination is an array. Convert content to delimited ASCII values.
                std::string numeric_content = "";
                std::vector<std::string> items43 = LoopParseFunc(content);
                for (size_t A_Index43 = 0; A_Index43 < items43.size(); A_Index43++) {
                    std::string A_LoopField43 = items43[A_Index43 - 0];
                    if (numeric_content != "") {
                        numeric_content += Chr(253);
                    }
                    numeric_content += STR(Asc(A_LoopField43));
                }
                set_value_type_aware(dest, content);
            } else {
                // Destination is a string or other type. Use old behavior.
                set_value_type_aware(dest, content);
            }
        }
        if (SubStr(current_line, 1, 12) == "file.append ") {
            str1 = StringTrimLeft(current_line, 12);
            std::string path_source = "";
            std::string content_source = "";
            int first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                path_source = Trim(SubStr(str1, 1, first_comma - 1));
                content_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                path_source = Trim(str1);
            }
            std::string filepath = get_value(path_source);
            std::string content = get_value(content_source);
            FileAppend(content, filepath);
        }
        if (SubStr(current_line, 1, 12) == "file.delete ") {
            str1 = StringTrimLeft(current_line, 12);
            std::string path_source = Trim(str1);
            std::string filepath = get_value(path_source);
            FileDelete(filepath);
        }
        if (SubStr(current_line, 1, 12) == "num_to_char ") {
            str1 = StringTrimLeft(current_line, 12);
            std::string src_reg = Trim(get_token(str1, ",", 1));
            std::string dest_reg = Trim(get_token(str1, ",", 2));
            std::string num_str = get_value(src_reg);
            std::string char_val = Chr(INT(num_str));
            set_value(dest_reg, char_val);
        }
        if (SubStr(current_line, 1, 5) == "call ") {
            str1 = Trim(StringTrimLeft(current_line, 5));
            if (str1 == "print") {
                str2 = Official_Oryx_VM_reg_array[1];
                print(str2);
            }
            else if (str1 == "print_char") {
                str2 = Official_Oryx_VM_reg_array[1];
                // Convert value in r1 to int, then to Char, then print
                print_raw(Chr(INT(str2)));
            }
            else if (str1 == "sleep") {
                str2 = Official_Oryx_VM_reg_array[1];
                // Sleep for milliseconds specified in r1
                Sleep(INT(str2));
            } else {
                pc_was_modified = true;
                HTVM_Append(call_stack, pc + 1);
                std::string label_name = str1;
                int j = 0;
                for (int A_Index44 = 0; A_Index44 < HTVM_Size(label_table); A_Index44++) {
                    if (get_token(label_table[j], Chr(254), 1) == label_name) {
                        pc = INT(get_token(label_table[j], Chr(254), 2));
                        break;
                    }
                    j++;
                }
            }
        }
        if (current_line == "ret") {
            if (HTVM_Size(call_stack) == 0) {
                print("FATAL ERROR: Return ('ret') called with empty call stack! PC: " + STR(pc));
                break;
            }
            pc_was_modified = true;
            int return_address = call_stack[HTVM_Size(call_stack) - 1];
            HTVM_Pop(call_stack);
            pc = return_address;
        }
        if (SubStr(current_line, 1, 5) == "push ") {
            std::string op1 = Trim(StringTrimLeft(current_line, 5));
            std::string val = get_value(op1);
            HTVM_Append(main_stack, op1 + Chr(254) + val);
        }
        if (SubStr(current_line, 1, 4) == "pop ") {
            std::string op1 = Trim(StringTrimLeft(current_line, 4));
            std::string target_prefix = op1 + Chr(254);
            int found_index = -1;
            std::string found_val = "";
            int stack_idx = HTVM_Size(main_stack) - 1;
            for (int A_Index45 = 0; ; A_Index45++) {
                if (stack_idx < 0) {
                    break;
                }
                std::string entry = main_stack[stack_idx];
                if (SubStr(entry, 1, StrLen(target_prefix)) == target_prefix) {
                    found_index = stack_idx;
                    found_val = StringTrimLeft(entry, StrLen(target_prefix));
                    break;
                }
                stack_idx--;
            }
            if (found_index != -1) {
                int reg_idx = INT(StringTrimLeft(op1, 1));
                Official_Oryx_VM_reg_array[reg_idx] = found_val;
                HTVM_Remove(main_stack, found_index);
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.add ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string arr_name = Trim(get_token(str1, ",", 1));
            std::string val_source = Trim(get_token(str1, ",", 2));
            std::string val = get_value(val_source);
            int j = 0;
            for (int A_Index46 = 0; A_Index46 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index46++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    std::string current_content = get_token(entry, Chr(254), 3);
                    if (current_content == "") {
                        current_content = val;
                    } else {
                        current_content = current_content + Chr(253) + val;
                    }
                    Official_Oryx_VM_symbol_table[j] = arr_name + Chr(254) + "arr" + Chr(254) + current_content;
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.pop ") {
            std::string arr_name = Trim(StringTrimLeft(current_line, 8));
            int j = 0;
            for (int A_Index47 = 0; A_Index47 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index47++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    std::string content = get_token(entry, Chr(254), 3);
                    if (content != "") {
                        // Calculate number of items
                        int item_count = 0;
                        std::vector<std::string> items48 = LoopParseFunc(content, Chr(253));
                        for (size_t A_Index48 = 0; A_Index48 < items48.size(); A_Index48++) {
                            std::string A_LoopField48 = items48[A_Index48 - 0];
                            item_count++;
                        }
                        // Rebuild string excluding the last item
                        std::string new_content = "";
                        int current_idx = 0;
                        std::vector<std::string> items49 = LoopParseFunc(content, Chr(253));
                        for (size_t A_Index49 = 0; A_Index49 < items49.size(); A_Index49++) {
                            std::string A_LoopField49 = items49[A_Index49 - 0];
                            current_idx++;
                            if (current_idx < item_count) {
                                if (new_content != "") {
                                    new_content += Chr(253);
                                }
                                new_content += A_LoopField49;
                            }
                        }
                        Official_Oryx_VM_symbol_table[j] = arr_name + Chr(254) + "arr" + Chr(254) + new_content;
                    }
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.get ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string arr_name = Trim(get_token(str1, ",", 1));
            std::string index_source = Trim(get_token(str1, ",", 2));
            std::string dest_reg_str = Trim(get_token(str1, ",", 3));
            int index = INT(get_value(index_source));
            int j = 0;
            for (int A_Index50 = 0; A_Index50 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index50++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    std::string content = get_token(entry, Chr(254), 3);
                    std::string val = get_token(content, Chr(253), index + 1);
                    int reg_idx = INT(StringTrimLeft(dest_reg_str, 1));
                    Official_Oryx_VM_reg_array[reg_idx] = val;
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.set ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string arr_name = Trim(get_token(str1, ",", 1));
            std::string index_source = Trim(get_token(str1, ",", 2));
            std::string val_source = Trim(get_token(str1, ",", 3));
            int index = INT(get_value(index_source));
            std::string new_val = get_value(val_source);
            int j = 0;
            for (int A_Index51 = 0; A_Index51 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index51++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    std::string content = get_token(entry, Chr(254), 3);
                    std::string new_content = "";
                    int current_idx = 0;
                    std::vector<std::string> items52 = LoopParseFunc(content, Chr(253));
                    for (size_t A_Index52 = 0; A_Index52 < items52.size(); A_Index52++) {
                        std::string A_LoopField52 = items52[A_Index52 - 0];
                        if (current_idx == index) {
                            if (new_content != "") {
                                new_content = new_content + Chr(253);
                            }
                            new_content = new_content + new_val;
                        } else {
                            if (new_content != "") {
                                new_content = new_content + Chr(253);
                            }
                            new_content = new_content + A_LoopField52;
                        }
                        current_idx++;
                    }
                    Official_Oryx_VM_symbol_table[j] = arr_name + Chr(254) + "arr" + Chr(254) + new_content;
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 9) == "arr.size ") {
            str1 = StringTrimLeft(current_line, 9);
            std::string arr_name = Trim(get_token(str1, ",", 1));
            std::string dest_reg_str = Trim(get_token(str1, ",", 2));
            int j = 0;
            for (int A_Index53 = 0; A_Index53 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index53++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    std::string content = get_token(entry, Chr(254), 3);
                    // --- THE FINAL, TRUE FIX ---
                    int size = 0;
                    // This check IS needed, but the loop logic was wrong.
                    if (content != "") {
                        std::vector<std::string> items54 = LoopParseFunc(content, Chr(253));
                        for (size_t A_Index54 = 0; A_Index54 < items54.size(); A_Index54++) {
                            std::string A_LoopField54 = items54[A_Index54 - 0];
                            // Simply increment for each item found.
                            size++;
                        }
                    }
                    int reg_idx = INT(StringTrimLeft(dest_reg_str, 1));
                    Official_Oryx_VM_reg_array[reg_idx] = STR(size);
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 10) == "arr.clear ") {
            std::string arr_name = Trim(StringTrimLeft(current_line, 10));
            int j = 0;
            for (int A_Index55 = 0; A_Index55 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index55++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    Official_Oryx_VM_symbol_table[j] = arr_name + Chr(254) + "arr" + Chr(254) + "";
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 9) == "arr.copy ") {
            str1 = StringTrimLeft(current_line, 9);
            std::string src_name = Trim(get_token(str1, ",", 1));
            std::string dest_name = Trim(get_token(str1, ",", 2));
            std::string content_to_copy = "";
            int j = 0;
            for (int A_Index56 = 0; A_Index56 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index56++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == src_name) {
                    content_to_copy = get_token(entry, Chr(254), 3);
                    break;
                }
                j++;
            }
            j = 0;
            for (int A_Index57 = 0; A_Index57 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index57++) {
                std::string entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == dest_name) {
                    Official_Oryx_VM_symbol_table[j] = dest_name + Chr(254) + "arr" + Chr(254) + content_to_copy;
                    break;
                }
                j++;
            }
        }
        // --- NEW: String Manipulation ---
        if (SubStr(current_line, 1, 8) == "str.get ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string str_name = Trim(get_token(str1, ",", 1));
            std::string index_source = Trim(get_token(str1, ",", 2));
            std::string dest_reg_str = Trim(get_token(str1, ",", 3));
            int index = INT(get_value(index_source));
            std::string content = get_value(str_name);
            std::string charr = SubStr(content, index + 1, 1);
            set_value(dest_reg_str, charr);
        }
        if (SubStr(current_line, 1, 8) == "str.set ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string str_name = Trim(get_token(str1, ",", 1));
            std::string index_source = Trim(get_token(str1, ",", 2));
            std::string char_source = Trim(get_token(str1, ",", 3));
            int index = INT(get_value(index_source));
            std::string new_val = get_value(char_source);
            std::string old_str = get_value(str_name);
            // --- HARD RULE: overwrite ONE character only ---
            std::string repl_char = SubStr(new_val, 1, 1);
            std::string part1 = "";
            if (index > 0) {
                part1 = SubStr(old_str, 1, index);
            }
            std::string part2 = SubStr(old_str, index + 2);
            std::string new_str = part1 + repl_char + part2;
            set_value(str_name, new_str);
        }
        if (SubStr(current_line, 1, 8) == "str.len ") {
            str1 = StringTrimLeft(current_line, 8);
            std::string str_name = Trim(get_token(str1, ",", 1));
            std::string dest_reg_str = Trim(get_token(str1, ",", 2));
            std::string content = get_value(str_name);
            int len = StrLen(content);
            set_value(dest_reg_str, STR(len));
        }
        if (!pc_was_modified) {
            pc++;
        }
    }
    Official_Oryx_VM_API_symbol_table_cache = Official_Oryx_VM_symbol_table;
    return outState;
}
std::string Oryx_VM(std::string code) {
    std::string outState = "";
    code = Trim(code);
    code = cleanUpFirst(code);
    code = preserveStrings(code);
    code = handleComments(code, ";");
    code = restoreStrings(code);
    outState = Oryx_interpreter(Trim(code));
    return outState;
}
int main(int argc, char* argv[]) {
    std::string outState = "";
    std::string code = "";
    std::string params = "";
    if (HTVM_getLang_HTVM() != "js") {
        params = getLangParams("oryxir", "oryxir");
        if (params != "") {
            code = FileRead(params);
            outState = Oryx_VM(code);
            print(outState);
        }
    }
    

    return 0;
}