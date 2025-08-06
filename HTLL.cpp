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
#include <cstdint>
#include <filesystem>
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
    std::ifstream file;
    std::filesystem::path full_path;
    // Check if the file path is an absolute path
    if (std::filesystem::path(path).is_absolute()) {
        full_path = path;
    } else {
        // If it's not a full path, prepend the current working directory
        full_path = std::filesystem::current_path() / path;
    }
    // Open the file
    file.open(full_path);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file.");
    }
    // Read the file content into a string
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
    std::filesystem::path file_path(path);
    // Check if the file exists
    if (!std::filesystem::exists(file_path)) {
        return false;
    }
    // Attempt to remove the file
    if (!std::filesystem::remove(file_path)) {
        return false;
    }
    return true;
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
    std::vector<std::string> params;
    for (int i = 1; i < ARGC; ++i) {
        std::string arg = ARGV[i];
        if (std::filesystem::exists(arg)) {
            arg = std::filesystem::absolute(arg).string();
        }
        params.push_back(arg);
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
std::string main_syntax = "" + Chr(10) + "; =============================================================================" + Chr(10) + "; MAIN PROGRAM ENTRY POINT" + Chr(10) + "; =============================================================================" + Chr(10) + "_start:" + Chr(10) + "";
std::string compiler(std::string code) {
    std::string out = "";
    std::string dot_data = Chr(10);
    std::string dot_data_print_temp_strings = Chr(10);
    int dot_data_print_temp_strings_count = 0;
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
    std::vector<std::string> items1 = LoopParseFunc(code);
    for (size_t A_Index1 = 0; A_Index1 < items1.size() + 0; A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        HTVM_Append(theIdNumOfThe34theVar, "");
        HTVM_Append(theIdNumOfThe34theVar, "");
    }
    std::vector<std::string> items2 = LoopParseFunc(code);
    for (size_t A_Index2 = 0; A_Index2 < items2.size() + 0; A_Index2++) {
        std::string A_LoopField2 = items2[A_Index2 - 0];
        theIdNumOfThe34theVar[A_Index2] = theIdNumOfThe34theVar[A_Index2] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField2);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "--" + str21 + "asds" + str21 + "as--" + str21 + "theuhtuwaesphoutr" + Chr(34);
    std::vector<std::string> items3 = LoopParseFunc(code);
    for (size_t A_Index3 = 0; A_Index3 < items3.size() + 0; A_Index3++) {
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
        for (size_t A_Index4 = 0; A_Index4 < items4.size() + 0; A_Index4++) {
            std::string A_LoopField4 = items4[A_Index4 - 0];
            if (A_LoopField4 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField4 != Chr(34)) {
                    if (A_LoopField4 == keyWordEscpaeChar) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField4;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
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
        for (size_t A_Index5 = 0; A_Index5 < items5.size() + 0; A_Index5++) {
            std::string A_LoopField5 = items5[A_Index5 - 0];
            if (A_LoopField5 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField5 != Chr(34)) {
                    if (A_LoopField5 == keyWordEscpaeChar && keyWordEscpaeChar == getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index5 + 1]) {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + keyWordEscpaeChar;
                        removeNexFixkeyWordEscpaeChar = 1;
                    }
                    else if (A_LoopField5 == keyWordEscpaeChar) {
                        if (removeNexFixkeyWordEscpaeChar != 1) {
                            theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + Chr(92);
                        } else {
                            removeNexFixkeyWordEscpaeChar = 0;
                        }
                    } else {
                        theIdNumOfThe34theVar[theIdNumOfThe34] = theIdNumOfThe34theVar[theIdNumOfThe34] + A_LoopField5;
                    }
                } else {
                    theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(theIdNumOfThe34) + Chr(65) + Chr(65);
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
    for (int A_Index6 = 0; A_Index6 < theIdNumOfThe34 + 0; A_Index6++) {
        theIdNumOfThe34theVar[A_Index6] = theIdNumOfThe34theVar[A_Index6] + Chr(34);
    }
    HTVM_Append(theIdNumOfThe34theVar, Chr(34));
    code = StrReplace(code, "{", Chr(10) + "{" + Chr(10));
    code = StrReplace(code, "}", Chr(10) + "}" + Chr(10));
    std::vector<std::string> items7 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index7 = 0; A_Index7 < items7.size() + 0; A_Index7++) {
        std::string A_LoopField7 = items7[A_Index7 - 0];
        out += Trim(A_LoopField7) + Chr(10);
    }
    code = StringTrimRight(out, 1);
    str1 = "";
    out = "";
    std::vector<std::string> items8 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index8 = 0; A_Index8 < items8.size() + 0; A_Index8++) {
        std::string A_LoopField8 = items8[A_Index8 - 0];
        std::vector<std::string> items9 = LoopParseFunc(A_LoopField8);
        for (size_t A_Index9 = 0; A_Index9 < items9.size() + 0; A_Index9++) {
            std::string A_LoopField9 = items9[A_Index9 - 0];
            if (A_LoopField9 != ";") {
                str1 += A_LoopField9;
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
    std::vector<std::string> items10 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index10 = 0; A_Index10 < items10.size() + 0; A_Index10++) {
        std::string A_LoopField10 = items10[A_Index10 - 0];
        if (Trim(A_LoopField10) == "main") {
            seenMain = 1;
            out += main_syntax + Chr(10);
        } else {
            out += A_LoopField10 + Chr(10);
        }
    }
    code = StringTrimRight(out, 1);
    out = "";
    std::vector<std::string> items11 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index11 = 0; A_Index11 < items11.size() + 0; A_Index11++) {
        std::string A_LoopField11 = items11[A_Index11 - 0];
        if (SubStr(A_LoopField11, 1, 6) == "print(") {
            str1 = StringTrimLeft(A_LoopField11, 6);
            str1 = StringTrimRight(str1, 1);
            if (RegExMatch(str1, "^\\d+$")) {
                out += "mov rdi, " + str1 + Chr(10) + "mov rsi, 0" + Chr(10) + "call print_number" + Chr(10);
            }
            else if (InStr(str1, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC")) {
                dot_data_print_temp_strings_count++;
                dot_data_print_temp_strings += "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + ": db " + Trim(str1) + ", 10" + Chr(10) + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len: equ $-" + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10);
                out += "mov rsi, ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + Chr(10) + "mov rdx, " + "ASM_STR_TEMP_PRINT_" + STR(dot_data_print_temp_strings_count) + "_len" + Chr(10) + "call print_str" + Chr(10);
            }
        }
    }
    code = StringTrimRight(out, 1);
    dot_data += dot_data_print_temp_strings + Chr(10);
    std::string upCode = "; This defines a " + Chr(34) + "struct" + Chr(34) + " to make our code readable." + Chr(10) + "struc DynamicArray" + Chr(10) + "    .pointer:   resq 1  ; Offset 0: Holds the pointer to the heap memory" + Chr(10) + "    .size:      resq 1  ; Offset 8: Holds the current number of elements" + Chr(10) + "    .capacity:  resq 1  ; Offset 16: Holds how many elements the block can store" + Chr(10) + "endstruc" + Chr(10) + "" + Chr(10) + "section .data" + Chr(10) + "    SCALE_FACTOR:   dq 1000000" + Chr(10) + "    INITIAL_CAPACITY equ 2  ; Let's use a smaller capacity to test resizing sooner" + Chr(10) + "    print_buffer:   times 21 db 0" + Chr(10) + "    dot:            db '.'" + Chr(10) + "    minus_sign:     db '-'" + Chr(10) + "    nl:   db 10" + Chr(10) + dot_data + Chr(10) + "" + Chr(10) + "section .bss" + Chr(10) + "    print_buffer_n: resb 20" + Chr(10) + "" + Chr(10) + "" + Chr(10) + "section .text" + Chr(10) + "global _start" + Chr(10) + "" + Chr(10) + "; =============================================================================" + Chr(10) + "; UTILITY FUNCTIONS (Correct and Properly Formatted)" + Chr(10) + "; =============================================================================" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "; divide_and_store: Performs fixed-point division and stores results in memory." + Chr(10) + "; [In] rdi: dividend_int, rsi: dividend_dec" + Chr(10) + ";      rdx: divisor_int,  rcx: divisor_dec" + Chr(10) + ";      r8:  ptr to result_int, r9: ptr to result_dec" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "divide_and_store:" + Chr(10) + "    ; Prologue" + Chr(10) + "    push rbx" + Chr(10) + "    push r12" + Chr(10) + "    push r13" + Chr(10) + "    push r14" + Chr(10) + "    push r15" + Chr(10) + "" + Chr(10) + "    ; --- THE FIX: Save all volatile input arguments immediately ---" + Chr(10) + "    mov r12, rdi            ; Save dividend_int" + Chr(10) + "    mov r13, rsi            ; Save dividend_dec" + Chr(10) + "    mov r14, rdx            ; Save divisor_int (THIS IS THE CRITICAL FIX)" + Chr(10) + "    mov r15, rcx            ; Save divisor_dec" + Chr(10) + "    " + Chr(10) + "    ; --- Step 1: Scale the Dividend ---" + Chr(10) + "    mov rax, r12            ; Use the saved dividend_int" + Chr(10) + "    mov rbx, [SCALE_FACTOR]" + Chr(10) + "    mul rbx" + Chr(10) + "    add rax, r13            ; Add the saved dividend_dec" + Chr(10) + "    mov r12, rax            ; r12 now holds the final scaled dividend" + Chr(10) + "" + Chr(10) + "    ; --- Step 2: Scale the Divisor ---" + Chr(10) + "    mov rax, r14            ; Use the saved divisor_int" + Chr(10) + "    mov rbx, [SCALE_FACTOR]" + Chr(10) + "    mul rbx" + Chr(10) + "    add rax, r15            ; Add the saved divisor_dec" + Chr(10) + "    mov r13, rax            ; r13 now holds the final scaled divisor" + Chr(10) + "" + Chr(10) + "    ; --- Step 3: Perform the main 128-bit division ---" + Chr(10) + "    mov rax, r12            ; Load the scaled dividend" + Chr(10) + "    mov rbx, [SCALE_FACTOR]" + Chr(10) + "    mul rbx" + Chr(10) + "    div r13                 ; Divide by the scaled divisor" + Chr(10) + "    " + Chr(10) + "    ; --- Step 4: De-scale the final result ---" + Chr(10) + "    mov rbx, [SCALE_FACTOR]" + Chr(10) + "    xor rdx, rdx" + Chr(10) + "    div rbx" + Chr(10) + "" + Chr(10) + "    ; --- Step 5: Store the results using the pointers from r8 and r9 ---" + Chr(10) + "    mov [r8], rax           ; Store the integer result" + Chr(10) + "    mov [r9], rdx           ; Store the decimal result" + Chr(10) + "" + Chr(10) + "    ; Epilogue" + Chr(10) + "    pop r15" + Chr(10) + "    pop r14" + Chr(10) + "    pop r13" + Chr(10) + "    pop r12" + Chr(10) + "    pop rbx" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "; print_div: (Unchanged logic, but now properly formatted)" + Chr(10) + "; [In] rdi: integer part, rsi: decimal part" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "print_div:" + Chr(10) + "    ; Prologue" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    push rbx" + Chr(10) + "    push r12" + Chr(10) + "    push rcx" + Chr(10) + "    push rsi" + Chr(10) + "" + Chr(10) + "    ; --- Print integer part ---" + Chr(10) + "    mov rax, rdi" + Chr(10) + "    lea r12, [print_buffer + 19]" + Chr(10) + "    mov byte [r12], 0" + Chr(10) + "    dec r12" + Chr(10) + "    mov rbx, 10" + Chr(10) + ".int_convert_loop:" + Chr(10) + "    xor rdx, rdx" + Chr(10) + "    div rbx" + Chr(10) + "    add rdx, '0'" + Chr(10) + "    mov [r12], dl" + Chr(10) + "    dec r12" + Chr(10) + "    cmp rax, 0" + Chr(10) + "    jne .int_convert_loop" + Chr(10) + "" + Chr(10) + "    inc r12" + Chr(10) + "    mov rsi, r12" + Chr(10) + "    lea rdx, [print_buffer + 19]" + Chr(10) + "    sub rdx, rsi" + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; --- Print decimal point ---" + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    mov rsi, dot" + Chr(10) + "    mov rdx, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; --- Print decimal part ---" + Chr(10) + "    pop rax" + Chr(10) + "    lea r12, [print_buffer + 5]" + Chr(10) + "    mov rbx, 10" + Chr(10) + "    mov rcx, 6" + Chr(10) + ".dec_convert_loop:" + Chr(10) + "    xor rdx, rdx" + Chr(10) + "    div rbx" + Chr(10) + "    add rdx, '0'" + Chr(10) + "    mov [r12], dl" + Chr(10) + "    dec r12" + Chr(10) + "    loop .dec_convert_loop" + Chr(10) + "" + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    lea rsi, [print_buffer]" + Chr(10) + "    mov rdx, 6" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; Print a final newline." + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    mov rsi, nl" + Chr(10) + "    mov rdx, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; Epilogue" + Chr(10) + "    pop rcx" + Chr(10) + "    pop r12" + Chr(10) + "    pop rbx" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "; print_number: Prints a number, prepending a '-' sign if requested." + Chr(10) + ";" + Chr(10) + "; [In]" + Chr(10) + ";   rdi - The number to print (as an unsigned, positive magnitude)." + Chr(10) + ";   rsi - The " + Chr(34) + "is_negative" + Chr(34) + " flag. 0 for positive, 1 for negative." + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "print_number:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    push rbx" + Chr(10) + "    push r12" + Chr(10) + "    push rdi        ; Save the number itself (the value from rdi) on the stack." + Chr(10) + "" + Chr(10) + "    ; Check the is_negative flag passed in rsi" + Chr(10) + "    cmp rsi, 0" + Chr(10) + "    je .skip_sign_print" + Chr(10) + "" + Chr(10) + "    ; Flag is 1, so print a minus sign." + Chr(10) + "    ; This is safe because the number is preserved on the stack." + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    mov rsi, minus_sign" + Chr(10) + "    mov rdx, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + ".skip_sign_print:" + Chr(10) + "    ; Restore the saved number from the stack directly into RAX," + Chr(10) + "    ; which is the register we need for the division loop." + Chr(10) + "    pop rax" + Chr(10) + "" + Chr(10) + "    ; The conversion loop, which now operates on the correct number in rax." + Chr(10) + "    lea r12, [print_buffer + 19]" + Chr(10) + "    mov byte [r12], 0" + Chr(10) + "    dec r12" + Chr(10) + "    mov rbx, 10" + Chr(10) + ".conversion_loop:" + Chr(10) + "    xor rdx, rdx" + Chr(10) + "    div rbx" + Chr(10) + "    add rdx, '0'" + Chr(10) + "    mov [r12], dl" + Chr(10) + "    dec r12" + Chr(10) + "    cmp rax, 0" + Chr(10) + "    jne .conversion_loop" + Chr(10) + "" + Chr(10) + "    inc r12" + Chr(10) + "    mov rsi, r12" + Chr(10) + "    lea rdx, [print_buffer + 20]" + Chr(10) + "    sub rdx, rsi" + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; Print a final newline." + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    mov rsi, nl" + Chr(10) + "    mov rdx, 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    ; The stack is balanced because we pushed RDI and popped it into RAX." + Chr(10) + "    pop r12" + Chr(10) + "    pop rbx" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + "print_str:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    mov rax, 1" + Chr(10) + "    mov rdi, 1" + Chr(10) + "    ; rsi and rdx are expected to be set by the caller" + Chr(10) + "    syscall" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "; input: Reads a line from stdin, handles empty input, removes the trailing" + Chr(10) + ";        newline, and stores the raw string and its length." + Chr(10) + ";" + Chr(10) + "; [In]" + Chr(10) + ";   rdi - Pointer to the buffer where the string should be stored." + Chr(10) + ";   rsi - Pointer to a qword where the length of the clean string will be stored." + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "input:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    push r12" + Chr(10) + "    push r13" + Chr(10) + "" + Chr(10) + "    ; Save the pointers passed as arguments immediately" + Chr(10) + "    mov r12, rdi ; r12 holds pointer to the buffer" + Chr(10) + "    mov r13, rsi ; r13 holds pointer to the length variable" + Chr(10) + "" + Chr(10) + "    ; Correctly set up all registers for the sys_read syscall" + Chr(10) + "    mov rax, 0" + Chr(10) + "    mov rdi, 0" + Chr(10) + "    mov rsi, r12" + Chr(10) + "    mov rdx, 100" + Chr(10) + "    syscall" + Chr(10) + "    ; The program waits here. rax will return with bytes read." + Chr(10) + "" + Chr(10) + "    ; Check if the user entered anything. If they just press Enter, rax will be 1." + Chr(10) + "    ; If they press Ctrl+D, rax will be 0. Both cases mean empty input." + Chr(10) + "    cmp rax, 1" + Chr(10) + "    jle .read_empty     ; Jump if Less or Equal (handles 0 and 1)" + Chr(10) + "" + Chr(10) + "    ; This code only runs for valid input (rax " + Chr(62) + " 1)." + Chr(10) + "    ; The length of the clean string is (bytes_read - 1)." + Chr(10) + "    dec rax" + Chr(10) + "    jmp .store_length   ; Jump to the end to store the calculated length" + Chr(10) + "" + Chr(10) + ".read_empty:" + Chr(10) + "    ; === THE FIX IS HERE ===" + Chr(10) + "    ; If the input was empty, explicitly set the length to 0." + Chr(10) + "    xor rax, rax" + Chr(10) + "" + Chr(10) + ".store_length:" + Chr(10) + "    ; Store the final calculated length (either from dec rax or xor rax)" + Chr(10) + "    ; into the memory location pointed to by r13." + Chr(10) + "    mov [r13], rax" + Chr(10) + "" + Chr(10) + "    pop r13" + Chr(10) + "    pop r12" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + "" + Chr(10) + "; =============================================================================" + Chr(10) + "; DYNAMIC ARRAY LIBRARY FUNCTIONS (Final, Corrected Version)" + Chr(10) + "; =============================================================================" + Chr(10) + "array_append:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    push rbx" + Chr(10) + "    push r12" + Chr(10) + "    push r13" + Chr(10) + "    push r14" + Chr(10) + "" + Chr(10) + "    mov r12, rdi             ; R12 = pointer to our array struct" + Chr(10) + "    mov r13, rsi             ; R13 = value we want to add" + Chr(10) + "" + Chr(10) + "    mov rcx, [r12 + DynamicArray.size]" + Chr(10) + "    cmp rcx, [r12 + DynamicArray.capacity]" + Chr(10) + "    jl .has_space" + Chr(10) + "" + Chr(10) + ".resize:" + Chr(10) + "    mov rbx, [r12 + DynamicArray.capacity]" + Chr(10) + "    cmp rbx, 0" + Chr(10) + "    jne .double_it" + Chr(10) + "    mov rbx, INITIAL_CAPACITY" + Chr(10) + "    jmp .capacity_calculated" + Chr(10) + ".double_it:" + Chr(10) + "    shl rbx, 1" + Chr(10) + ".capacity_calculated:" + Chr(10) + "    mov [r12 + DynamicArray.capacity], rbx" + Chr(10) + "" + Chr(10) + "    push r12" + Chr(10) + "    push r13" + Chr(10) + "" + Chr(10) + "    mov r14, rbx" + Chr(10) + "    imul r14, 8" + Chr(10) + "" + Chr(10) + "    mov rax, 9" + Chr(10) + "    mov rdi, 0" + Chr(10) + "    mov rsi, r14" + Chr(10) + "    mov rdx, 3" + Chr(10) + "    mov r10, 34" + Chr(10) + "    mov r8, -1" + Chr(10) + "    mov r9, 0" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    cmp rax, 0" + Chr(10) + "    jl .mmap_failed" + Chr(10) + "" + Chr(10) + "    mov r14, rax" + Chr(10) + "" + Chr(10) + "    pop r13" + Chr(10) + "    pop r12" + Chr(10) + "" + Chr(10) + "    mov rsi, [r12 + DynamicArray.pointer]" + Chr(10) + "    cmp rsi, 0" + Chr(10) + "    je .skip_copy" + Chr(10) + "" + Chr(10) + "    mov rcx, [r12 + DynamicArray.size]" + Chr(10) + "    mov rdi, r14" + Chr(10) + "    rep movsq" + Chr(10) + "" + Chr(10) + ".skip_copy:" + Chr(10) + "    mov [r12 + DynamicArray.pointer], r14" + Chr(10) + "    jmp .has_space" + Chr(10) + "" + Chr(10) + ".mmap_failed:" + Chr(10) + "    pop r13" + Chr(10) + "    pop r12" + Chr(10) + "    jmp .append_epilogue" + Chr(10) + "" + Chr(10) + ".has_space:" + Chr(10) + "    mov rbx, [r12 + DynamicArray.pointer]" + Chr(10) + "    mov rcx, [r12 + DynamicArray.size]" + Chr(10) + "" + Chr(10) + "    mov [rbx + rcx * 8], r13" + Chr(10) + "    inc qword [r12 + DynamicArray.size]" + Chr(10) + "" + Chr(10) + ".append_epilogue:" + Chr(10) + "    pop r14" + Chr(10) + "    pop r13" + Chr(10) + "    pop r12" + Chr(10) + "    pop rbx" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + "array_pop:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    push rbx" + Chr(10) + "    push rcx" + Chr(10) + "    push r8" + Chr(10) + "" + Chr(10) + "    mov r8, rdi" + Chr(10) + "    mov rcx, [r8 + DynamicArray.size]" + Chr(10) + "    cmp rcx, 0" + Chr(10) + "    je .pop_empty" + Chr(10) + "" + Chr(10) + "    dec rcx" + Chr(10) + "    mov [r8 + DynamicArray.size], rcx" + Chr(10) + "    mov rbx, [r8 + DynamicArray.pointer]" + Chr(10) + "    mov rax, [rbx + rcx * 8]" + Chr(10) + "    jmp .pop_done" + Chr(10) + "" + Chr(10) + ".pop_empty:" + Chr(10) + "    xor rax, rax" + Chr(10) + "" + Chr(10) + ".pop_done:" + Chr(10) + "    pop r8" + Chr(10) + "    pop rcx" + Chr(10) + "    pop rbx" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "; array_clear: Clears a dynamic array by setting its size to 0." + Chr(10) + "; [In] rdi: Pointer to the DynamicArray struct." + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "array_clear:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "" + Chr(10) + "    ; Set the .size field of the struct pointed to by rdi to 0." + Chr(10) + "    mov qword [rdi + DynamicArray.size], 0" + Chr(10) + "" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + ";-----------------------------------------------------------------------------" + Chr(10) + "" + Chr(10) + "print_char:" + Chr(10) + "    push rbp" + Chr(10) + "    mov rbp, rsp" + Chr(10) + "    sub rsp, 1                  ; reserve 1 byte for char storage" + Chr(10) + "" + Chr(10) + "    mov byte [rsp], dil         ; store char at rsp (safe because reserved)" + Chr(10) + "" + Chr(10) + "    mov rax, 1                 ; sys_write" + Chr(10) + "    mov rdi, 1                 ; stdout" + Chr(10) + "    mov rsi, rsp               ; pointer to char on stack" + Chr(10) + "    mov rdx, 1                 ; length = 1" + Chr(10) + "    syscall" + Chr(10) + "" + Chr(10) + "    add rsp, 1                 ; deallocate local stack space" + Chr(10) + "    pop rbp" + Chr(10) + "    ret" + Chr(10) + "" + Chr(10) + "" + Chr(10) + "";
    std::string downCode = "" + Chr(10) + "    ; --- Exit cleanly ---" + Chr(10) + "    mov rax, 60" + Chr(10) + "    xor rdi, rdi" + Chr(10) + "    syscall" + Chr(10) + "";
    std::string codeOUT = "";
    if (seenMain == 0) {
        codeOUT = upCode + Chr(10) + main_syntax +  Chr(10) + out + Chr(10) + downCode;
    } else {
        codeOUT = upCode + Chr(10) + out + Chr(10) + downCode;
    }
    for (int A_Index12 = 0; A_Index12 < theIdNumOfThe34 + 0; A_Index12++) {
        if (theIdNumOfThe34 == A_Index12 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index12 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index12 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index12 + 1) + Chr(65) + Chr(65), StrReplace(theIdNumOfThe34theVar[A_Index12 + 1], keyWordEscpaeChar, "\\"));
        }
    }
    return codeOUT;
}
int main(int argc, char* argv[]) {
    std::string params = Trim(GetParams());
    std::string paramsTemp = "";
    if (params == "") {
        print("Usage:" + Chr(10) + "./HTLL your_file.htll");
    } else {
        std::vector<std::string> items13 = LoopParseFunc(params, "\n", "\r");
        for (size_t A_Index13 = 0; A_Index13 < items13.size() + 0; A_Index13++) {
            std::string A_LoopField13 = items13[A_Index13 - 0];
            paramsTemp = Trim(A_LoopField13);
            if (A_Index13 == 0) {
                FileDelete("finalASM_HTLL_ASM.s");
                FileAppend(compiler(FileRead(paramsTemp)), "finalASM_HTLL_ASM.s");
                print("Compilation finished: finalASM_HTLL_ASM.s generated.");
            }
        }
    }
    

    return 0;
}