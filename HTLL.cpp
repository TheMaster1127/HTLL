#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
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

// Print function for const char* specifically
void print(const char* value) {
    std::cout << std::string(value) << std::endl;  // Convert const char* to std::string
}
// Print function that converts all types to string if needed
template <typename T>
void print(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
        std::cout << value << std::endl;
    } else if constexpr (std::is_same_v<T, char>) {
        std::cout << value << std::endl;
    } else if constexpr (std::is_same_v<T, int>) {
        std::cout << std::to_string(value) << std::endl;
    } else if constexpr (std::is_same_v<T, float>) {
        std::cout << std::to_string(value) << std::endl;
    } else if constexpr (std::is_same_v<T, double>) {
        std::cout << std::to_string(value) << std::endl;
    } else if constexpr (std::is_same_v<T, size_t>) {
        std::cout << std::to_string(value) << std::endl;
    } else if constexpr (std::is_same_v<T, bool>) {
        std::cout << (value ? "1" : "0") << std::endl;
    } else {
        std::cout << "Unsupported type" << std::endl;
    }
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

double Exp(double value) {
    return std::exp(value);
}

std::string SubStr(const std::string& str, int startPos, int length = -1) {
    std::string result;
    size_t strLen = str.size();
    // Handle negative starting positions
    if (startPos < 0) {
        startPos += strLen;
        if (startPos < 0) startPos = 0;
    } else {
        if (startPos > static_cast<int>(strLen)) return ""; // Starting position beyond string length
        startPos -= 1; // Convert to 0-based index
    }
    // Handle length
    if (length < 0) {
        length = strLen - startPos; // Length to end of string
    } else if (startPos + length > static_cast<int>(strLen)) {
        length = strLen - startPos; // Adjust length to fit within the string
    }
    // Extract substring
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



// global vars
std::vector<std::string> allVars;
int varsDecNum = 16;
int labelNum = 1;
bool isNum(std::string theString) {
    bool isNum = false;
    int countNums = 0;
    int countNums2 = 0;
    std::vector<std::string> items1 = LoopParseFunc(theString);
    for (size_t A_Index1 = 0; A_Index1 < items1.size() + 0; A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        countNums++;
    }
    std::vector<std::string> items2 = LoopParseFunc(theString);
    for (size_t A_Index2 = 0; A_Index2 < items2.size() + 0; A_Index2++) {
        std::string A_LoopField2 = items2[A_Index2 - 0];
        if (A_LoopField2 == "0" || A_LoopField2 == "1" || A_LoopField2 == "2" || A_LoopField2 == "3" || A_LoopField2 == "4" || A_LoopField2 == "5" || A_LoopField2 == "6" || A_LoopField2 == "7" || A_LoopField2 == "8" || A_LoopField2 == "9") {
            countNums2++;
        }
    }
    if (countNums == countNums2) {
        isNum = true;
    }
    return isNum;
}
std::string getMemLocFromVarName(std::string varName) {
    for (int A_Index3 = 0; A_Index3 < allVars.size() + 0; A_Index3++) {
        if (Trim(allVars[A_Index3]) == Trim(varName)) {
            return STR(A_Index3 + 17);
        }
    }
    return "0";
}
std::string expresionCompiler(std::string TheExpresion) {
    TheExpresion = Trim(TheExpresion);
    if (!(InStr(TheExpresion, " "))) {
        return "false";
    }
    std::string expresionOut = "";
    if (isNum(StrSplit(TheExpresion, " ", 1))) {
        expresionOut = "MOV r1, " + StrSplit(TheExpresion, " ", 1);
    } else {
        expresionOut = "MOV r1, l" + getMemLocFromVarName(StrSplit(TheExpresion, " ", 1));
    }
    std::vector<std::string> TheExpresionARR;
    std::vector<std::string> items4 = LoopParseFunc(TheExpresion, " ");
    for (size_t A_Index4 = 0; A_Index4 < items4.size() + 0; A_Index4++) {
        std::string A_LoopField4 = items4[A_Index4 - 0];
        TheExpresionARR.push_back(A_LoopField4);
    }
    int onceFixLoopExpresion = 0;
    std::vector<std::string> items5 = LoopParseFunc(TheExpresion, " ");
    for (size_t A_Index5 = 0; A_Index5 < items5.size() + 0; A_Index5++) {
        std::string A_LoopField5 = items5[A_Index5 - 0];
        if (A_LoopField5 != "+" && A_LoopField5 != "-" && A_LoopField5 != "*") {
            onceFixLoopExpresion++;
            if (onceFixLoopExpresion != 1) {
                if (TheExpresionARR[A_Index5 - 1] == "+") {
                    if (isNum(A_LoopField5)) {
                        expresionOut += "\nADD r1, " + A_LoopField5;
                    } else {
                        expresionOut += "\nADD r1, l" + getMemLocFromVarName(A_LoopField5);
                    }
                }
                if (TheExpresionARR[A_Index5 - 1] == "-") {
                    if (isNum(A_LoopField5)) {
                        expresionOut += "\nSUB r1, " + A_LoopField5;
                    } else {
                        expresionOut += "\nSUB r1, l" + getMemLocFromVarName(A_LoopField5);
                    }
                }
                if (TheExpresionARR[A_Index5 - 1] == "*") {
                    if (isNum(A_LoopField5)) {
                        expresionOut += "\nMUL r1, " + A_LoopField5;
                    } else {
                        expresionOut += "\nMUL r1, l" + getMemLocFromVarName(A_LoopField5);
                    }
                }
            }
        }
    }
    return expresionOut;
}
void compiler() {
    std::string params = GetParams();
    if (params == "") {
        print("Usage:\nHTLL yourFile.htll");
        return;
    }
    std::string fileName = "";
    fileName = Trim(params);
    print(fileName);
    std::string HTLLcode = FileRead(Trim(fileName));
    std::string fileNameASM = "";
    fileNameASM = StringTrimRight(fileName, 4);
    fileNameASM = fileNameASM + "htasm";
    HTLLcode = StrReplace(HTLLcode, Chr(13), "");
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
    std::string HTLLout = "";
    std::string HTLLoutFix = "";
    std::vector<std::string> holdNextLines;
    std::vector<std::string> items6 = LoopParseFunc(HTLLcode, "\n", "\r");
    for (size_t A_Index6 = 0; A_Index6 < items6.size() + 0; A_Index6++) {
        std::string A_LoopField6 = items6[A_Index6 - 0];
        HTLLoutFix += Trim(A_LoopField6) + "\n";
        holdNextLines.push_back(Trim(A_LoopField6));
    }
    HTLLcode = StringTrimRight(HTLLoutFix, 1);
    std::string lastIfStatement = "";
    std::string lastIfStatementType = "";
    int curlyBracesDepth = 1;
    std::vector<std::string> curlyBracesHold;
    curlyBracesHold.push_back("");
    std::vector<std::string> lastIfHold;
    std::vector<std::string> lastIfHoldType;
    lastIfHold.push_back("");
    lastIfHoldType.push_back("");
    int loopNum = 0;
    int funcNum = 0;
    std::vector<std::string> allFuncs;
    int funcCallNum = 0;
    std::vector<std::string> items7 = LoopParseFunc(HTLLcode, "\n", "\r");
    for (size_t A_Index7 = 0; A_Index7 < items7.size() + 0; A_Index7++) {
        std::string A_LoopField7 = items7[A_Index7 - 0];
        if (SubStr(A_LoopField7, 1, 6) == "print(") {
            str1 = StringTrimLeft(A_LoopField7, 6);
            str1 = StringTrimRight(str1, 1);
            if (expresionCompiler(str1) == "false") {
                if (isNum(str1)) {
                    HTLLout += "PRINT " + str1 + "\n";
                } else {
                    HTLLout += "PRINT l" + getMemLocFromVarName(str1) + "\n";
                }
            } else {
                HTLLout += expresionCompiler(str1) + "\nPRINT r1\n";
            }
        }
        else if (SubStr(A_LoopField7, 1, 7) == "prints(") {
            str1 = StringTrimLeft(A_LoopField7, 7);
            str1 = StringTrimRight(str1, 1);
            if (expresionCompiler(str1) == "false") {
                if (isNum(str1)) {
                    HTLLout += "PRINTS " + str1 + "\n";
                } else {
                    HTLLout += "PRINTS l" + getMemLocFromVarName(str1) + "\n";
                }
            } else {
                HTLLout += expresionCompiler(str1) + "\nPRINTS r1\n";
            }
        }
        else if (InStr(A_LoopField7, " := ") && SubStr(StrLower(A_LoopField7), 1, 6) != "loop, ") {
            str1 = StrSplit(A_LoopField7, " := ", 1);
            if (getMemLocFromVarName(str1) == "0") {
                varsDecNum++;
                allVars.push_back(str1);
            }
            str2 = StrSplit(A_LoopField7, " := ", 2);
            if (expresionCompiler(str2) != "false") {
                str5 = expresionCompiler(str2);
                if (getMemLocFromVarName(str1) == "0") {
                    str4 = str5 + "\nMOV l" + STR(varsDecNum) + ", r1";
                } else {
                    str4 = str5 + "\nMOV l" + getMemLocFromVarName(str1) + ", r1";
                }
            } else {
                if (isNum(str2)) {
                    str3 = str2;
                } else {
                    str3 = "l" + getMemLocFromVarName(str2);
                }
                if (getMemLocFromVarName(str1) == "0") {
                    str4 = "MOV l" + STR(varsDecNum) + ", " + str3;
                } else {
                    str4 = "MOV l" + getMemLocFromVarName(str1) + ", " + str3;
                }
            }
            HTLLout += str4 + "\n";
        }
        else if (SubStr(A_LoopField7, 1, 3) == "if " && InStr(A_LoopField7, " = ")) {
            curlyBracesDepth++;
            str1 = StringTrimLeft(A_LoopField7, 3);
            str1 = StrReplace(str1, "(", "");
            str1 = StrReplace(str1, ")", "");
            str2 = Trim(StrSplit(str1, "=", 1));
            str3 = Trim(StrSplit(str1, "=", 2));
            lastIfStatementType = "=";
            if (isNum(str3)) {
                str4 = "CMP l" + getMemLocFromVarName(str2) + ", " + str3 + "\nJNE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_";
                lastIfStatement = "CMP l" + getMemLocFromVarName(str2) + ", " + str3;
            } else {
                str4 = "CMP l" + getMemLocFromVarName(str2) + ", l" + getMemLocFromVarName(str3) + "\nJNE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_";
                lastIfStatement = "CMP l" + getMemLocFromVarName(str2) + ", l" + getMemLocFromVarName(str3);
            }
            lastIfHold.push_back(lastIfStatement);
            lastIfHoldType.push_back(lastIfStatementType);
            curlyBracesHold.push_back(STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_");
            HTLLout += str4 + "\n";
        }
        else if (SubStr(A_LoopField7, 1, 3) == "if " && InStr(A_LoopField7, " != ")) {
            curlyBracesDepth++;
            str1 = StringTrimLeft(A_LoopField7, 3);
            str1 = StrReplace(str1, "(", "");
            str1 = StrReplace(str1, ")", "");
            str2 = Trim(StrSplit(str1, "!=", 1));
            str3 = Trim(StrSplit(str1, "!=", 2));
            lastIfStatementType = "!=";
            if (isNum(str3)) {
                str4 = "CMP l" + getMemLocFromVarName(str2) + ", " + str3 + "\nJE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_";
                lastIfStatement = "CMP l" + getMemLocFromVarName(str2) + ", " + str3;
            } else {
                str4 = "CMP l" + getMemLocFromVarName(str2) + ", l" + getMemLocFromVarName(str3) + "\nJE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_";
                lastIfStatement = "CMP l" + getMemLocFromVarName(str2) + ", l" + getMemLocFromVarName(str3);
            }
            lastIfHold.push_back(lastIfStatement);
            lastIfHoldType.push_back(lastIfStatementType);
            curlyBracesHold.push_back(STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_");
            HTLLout += str4 + "\n";
        }
        else if (StrLower(Trim(A_LoopField7)) == "else") {
            curlyBracesDepth++;
            str8 = lastIfHoldType[lastIfHoldType.size() - 1];
            str9 = lastIfHold[lastIfHold.size() - 1];
            if (str8 == "=") {
                HTLLout += str9 + "\nJE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_\n";
            }
            if (str8 == "!=") {
                HTLLout += str9 + "\nJNE " + STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_\n";
            }
            curlyBracesHold.push_back(STR(curlyBracesDepth - 1) + "_" + STR(labelNum) + "_");
            lastIfHoldType.pop_back();
            lastIfHold.pop_back();
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        }
        else if (SubStr(StrLower(A_LoopField7), 1, 6) == "loop, ") {
            curlyBracesDepth++;
            str1 = StringTrimLeft(A_LoopField7, 6);
            loopNum++;
            str2 = Trim(StrSplit(str1, ",", 1));
            str6 = Trim(StrSplit(str1, ",", 1));
            str7 = Trim(StrSplit(str1, ",", 2));
            lastIfHold.push_back("");
            lastIfHoldType.push_back("");
            if (getMemLocFromVarName(StrSplit(str2, " := ", 1)) == "0") {
                varsDecNum++;
                allVars.push_back(StrSplit(str2, " := ", 1));
            }
            str3 = StrSplit(str2, " := ", 2);
            if (expresionCompiler(str3) != "false") {
                str5 = expresionCompiler(str3);
                if (getMemLocFromVarName(str2) == "0") {
                    str4 = str5 + "\nMOV l" + STR(varsDecNum) + ", r1";
                } else {
                    str4 = str5 + "\nMOV l" + getMemLocFromVarName(str2) + ", r1";
                }
            } else {
                if (isNum(str3)) {
                    str5 = str3;
                } else {
                    str5 = "l" + getMemLocFromVarName(str3);
                }
                if (getMemLocFromVarName(str2) == "0") {
                    str4 = "MOV l" + STR(varsDecNum) + ", " + str5;
                } else {
                    str4 = "MOV l" + getMemLocFromVarName(str2) + ", " + str5;
                }
            }
            HTLLout += str4 + "\n";
            HTLLout += "LABEL loop_" + STR(loopNum) + "\n";
            // loopNum | count | indexNameVar
            curlyBracesHold.push_back(STR(loopNum) + "|" + Trim(StrSplit(str6, " := ", 1)) + "|" + str7);
        }
        else if (Trim(A_LoopField7) == "break") {
            for (int A_Index8 = 0; ; A_Index8++) {
                str8 = STR(curlyBracesHold.size());
                str9 = curlyBracesHold[INT(str8) - (A_Index8 + 1)];
                if (InStr(str9, "|")) {
                    break;
                }
            }
            HTLLout += "JMP loopexit_" + Trim(StrSplit(str9, "|", 1)) + "\n";
        }
        else if (SubStr(StrLower(A_LoopField7), 1, 5) == "func ") {
            str1 = StringTrimLeft(A_LoopField7, 5);
            funcNum++;
            allFuncs.push_back(str1 + "|" + STR(funcNum));
            lastIfHold.push_back("");
            lastIfHoldType.push_back("");
            HTLLout += "JMP funcClose_" + STR(funcNum) + "\nLABEL funcUp_" + STR(funcNum) + "\n";
            curlyBracesHold.push_back("~|" + STR(funcNum));
        }
        else if (!(SubStr(StrLower(A_LoopField7), 1, 5) == "func ") && InStr(A_LoopField7, "()")) {
            str1 = Trim(A_LoopField7);
            /*
            MOV r10, 11
            JMP 2
            LABEL 11
            */
            for (int A_Index9 = 0; A_Index9 < allFuncs.size() + 0; A_Index9++) {
                if (StrSplit(allFuncs[A_Index9], "|", 1) == str1) {
                    str3 = StrSplit(allFuncs[A_Index9], "|", 2);
                    break;
                }
            }
            funcCallNum++;
            HTLLout += "MOV r10, funcBack_" + STR(funcCallNum) + "\nJMP funcUp_" + str3 + "\nLABEL funcBack_" + STR(funcCallNum) + "\n";
        }
        else if (A_LoopField7 == "}") {
            if (holdNextLines[A_Index7 + 1] != "else") {
                lastIfHoldType.pop_back();
                lastIfHold.pop_back();
            }
            str9 = curlyBracesHold[curlyBracesHold.size() - 1];
            if (InStr(str9, "|") && !(InStr(str9, "~"))) {
                // loopNum | count | indexNameVar
                HTLLout += "INC l" + getMemLocFromVarName(Trim(StrSplit(str9, "|", 2))) + "\nMOV l" + getMemLocFromVarName(Trim(StrSplit(str9, "|", 2))) + ", r1\nCMP l" + getMemLocFromVarName(Trim(StrSplit(str9, "|", 2))) + ", " + Trim(StrSplit(str9, "|", 3)) + "\nJNE loop_" + StrSplit(str9, "|", 1) + "\nLABEL loopexit_" + StrSplit(str9, "|", 1) + "\n";
            }
            else if (InStr(str9, "~")) {
                HTLLout += "JMP r10\nLABEL funcClose_" + Trim(StrSplit(str9, "|", 2)) + "\n";
            } else {
                HTLLout += "LABEL _" + str9 + "\n";
            }
            curlyBracesHold.pop_back();
            curlyBracesDepth--;
            labelNum++;
        }
    }
    HTLLout += "HLT";
    // fix labels
    std::string fixLabels = "";
    print(HTLLout);
    std::vector<std::string> items10 = LoopParseFunc(HTLLout, "\n", "\r");
    for (size_t A_Index10 = 0; A_Index10 < items10.size() + 0; A_Index10++) {
        std::string A_LoopField10 = items10[A_Index10 - 0];
        if (SubStr(A_LoopField10, 1, 6) == "LABEL ") {
            if (SubStr(A_LoopField10, 1, 11) != "LABEL loop_") {
                str1 = StringTrimLeft(A_LoopField10, 6);
                str1 = Trim(str1);
                str2 = StrSplit(str1, "_", 2);
                str1 = Trim(StrSplit(str1, "_", 3));
                if (str2 == "0") {
                    str2 = "1";
                }
                if (str1 == "0") {
                    str1 = "1";
                }
                HTLLout = StrReplace(HTLLout, "LABEL _" + str2 + "_" + str1 + "_", "LABEL " +  STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JMP " + str2 + "_" + str1 + "_", "JMP " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JE " + str2 + "_" + str1 + "_", "JE " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JNE " + str2 + "_" + str1 + "_", "JNE " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JM " + str2 + "_" + str1 + "_", "JM " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JL " + str2 + "_" + str1 + "_", "JL " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JEM " + str2 + "_" + str1 + "_", "JEM " + STR(A_Index10 + 1));
                HTLLout = StrReplace(HTLLout, "JEL " + str2 + "_" + str1 + "_", "JEL " + STR(A_Index10 + 1));
            }
        }
        if (SubStr(A_LoopField10, 1, 11) == "LABEL loop_") {
            str1 = StringTrimLeft(A_LoopField10, 11);
            str1 = Trim(str1);
            HTLLout = StrReplace(HTLLout, "LABEL loop_" + str1, "LABEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JNE loop_" + str1, "JNE " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JL loop_" + str1, "JL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JM loop_" + str1, "JM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEM loop_" + str1, "JEM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEL loop_" + str1, "JNL " + STR(A_Index10 + 1));
        }
        if (SubStr(A_LoopField10, 1, 15) == "LABEL loopexit_") {
            str1 = StringTrimLeft(A_LoopField10, 15);
            str1 = Trim(str1);
            HTLLout = StrReplace(HTLLout, "LABEL loopexit_" + str1, "LABEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JMP loopexit_" + str1, "JMP " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JNE loopexit_" + str1, "JNE " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JL loopexit_" + str1, "JL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JM loopexit_" + str1, "JM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEM loopexit_" + str1, "JEM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEL loopexit_" + str1, "JEL " + STR(A_Index10 + 1));
        }
        if (SubStr(A_LoopField10, 1, 15) == "LABEL funcBack_") {
            str1 = StringTrimLeft(A_LoopField10, 15);
            str1 = Trim(str1);
            HTLLout = StrReplace(HTLLout, "LABEL funcBack_" + str1, "LABEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JMP funcBack_" + str1, "JMP " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JNE funcBack_" + str1, "JNE " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JL funcBack_" + str1, "JL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JM funcBack_" + str1, "JM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEM funcBack_" + str1, "JEM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEL funcBack_" + str1, "JEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "MOV r10, funcBack_" + str1, "MOV r10, " + STR(A_Index10 + 1));
        }
        if (SubStr(A_LoopField10, 1, 13) == "LABEL funcUp_") {
            str1 = StringTrimLeft(A_LoopField10, 13);
            str1 = Trim(str1);
            HTLLout = StrReplace(HTLLout, "LABEL funcUp_" + str1, "LABEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JMP funcUp_" + str1, "JMP " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JNE funcUp_" + str1, "JNE " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JL funcUp_" + str1, "JL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JM funcUp_" + str1, "JM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEM funcUp_" + str1, "JEM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEL funcUp_" + str1, "JEL " + STR(A_Index10 + 1));
        }
        if (SubStr(A_LoopField10, 1, 16) == "LABEL funcClose_") {
            str1 = StringTrimLeft(A_LoopField10, 16);
            str1 = Trim(str1);
            HTLLout = StrReplace(HTLLout, "LABEL funcClose_" + str1, "LABEL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JMP funcClose_" + str1, "JMP " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JNE funcClose_" + str1, "JNE " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JL funcClose_" + str1, "JL " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JM funcClose_" + str1, "JM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEM funcClose_" + str1, "JEM " + STR(A_Index10 + 1));
            HTLLout = StrReplace(HTLLout, "JEL funcClose_" + str1, "JEL " + STR(A_Index10 + 1));
        }
    }
    print(HTLLout);
    FileDelete(Trim(fileNameASM));
    FileAppend(HTLLout, Trim(fileNameASM));
}
int main(int argc, char* argv[]) {
    compiler();
    return 0;
}