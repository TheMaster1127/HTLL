#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
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

size_t StrLen(const std::string& str) {
    return str.length();
}

double Sin(double angle) {
    return std::sin(angle);
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

double Floor(double num) {
    if (std::isnan(num)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return std::floor(num);
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

void ExitApp() {
    std::exit(0);
}


std::string MOVinB = "00000";
std::string ADDinB = "00001";
std::string SUBinB = "00010";
std::string MULinB = "00011";
std::string DEVinB = "00100";
std::string INCinB = "00101";
std::string DECinB = "00110";
std::string LABELinB = "00111";
std::string JMPinB = "01000";
std::string CMPinB = "01001";
std::string JEinB = "01010";
std::string JNEinB = "01011";
std::string JMinB = "01100";
std::string JLinB = "01101";
std::string JEMinB = "01110";
std::string JELinB = "01111";
std::string LOADinB = "10000";
std::string DRAWXinB = "10001";
std::string DRAWYinB = "10010";
std::string DRAWinB = "10011";
std::string PRINTinB = "10100";
std::string PRINTSinB = "10101";
std::string MODinB = "10110";
std::string HLTinB = "10111";
std::string XXX1inB = "11000";
std::string XXX2inB = "11001";
std::string XXX3inB = "11010";
std::string XXX4inB = "11011";
std::string XXX5inB = "11100";
std::string XXX6inB = "11101";
std::string XXX7inB = "11110";
std::string XXX8inB = "11111";
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
std::string getStringPosChar(std::string TheString, int pos) {
    std::vector<std::string> items1 = LoopParseFunc(TheString);
    for (size_t A_Index1 = 0; A_Index1 < items1.size() + 0; A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        if (pos == A_Index1 + 1) {
            return A_LoopField1;
        }
    }
    return "";
}
int powerOf(int base, int exp) {
    int res = 1;
    for (int A_Index2 = 0; A_Index2 < exp + 0; A_Index2++) {
        res *= base;
    }
    return res;
}
int binToDec(std::string binaryString) {
    int decimalValue = 0;
    int length = StrLen(binaryString);
    std::vector<std::string> items3 = LoopParseFunc(binaryString);
    for (size_t A_Index3 = 0; A_Index3 < items3.size() + 0; A_Index3++) {
        std::string A_LoopField3 = items3[A_Index3 - 0];
        std::string char123 = A_LoopField3;
        if (char123 == "1") {
            decimalValue += powerOf(2, length - A_Index3 - 1);
        }
    }
    return decimalValue;
}
std::string padBinary(std::string binaryString, int length) {
    while (StrLen(binaryString) < length) {
        binaryString = "0" + binaryString;
    }
    return binaryString;
}
std::string printBinary(int num, int totalBits) {
    std::string binaryString = "";
    int remainder = 0;
    while (num > 0) {
        remainder = Mod(num, 2);
        binaryString = STR(remainder) + binaryString;
        num = Floor(num / 2);
    }
    if (binaryString == "") {
        binaryString = "0";
    }
    binaryString = padBinary(binaryString, totalBits);
    return binaryString;
}
std::string decodeToBinary(std::string theCode) {
    std::string outInB = "";
    if (InStr(theCode, "r") || InStr(theCode, "l")) {
        outInB += "1";
        theCode = StringTrimLeft(theCode, 1);
    } else {
        outInB += "0";
    }
    std::vector<std::string> BinaryRepresentation;
    for (int A_Index4 = 0; A_Index4 < powerOf(2, 11) + 0; A_Index4++) {
        BinaryRepresentation.push_back(printBinary(A_Index4, 11));
    }
    int indexOfARR = INT(theCode);
    outInB += BinaryRepresentation[indexOfARR];
    return outInB;
}
std::string compiler(std::string code) {
    std::string outCodeInBinaryFIX = "";
    std::string outCodeInBinary = "";
    std::vector<std::string> items5 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index5 = 0; A_Index5 < items5.size() + 0; A_Index5++) {
        std::string A_LoopField5 = items5[A_Index5 - 0];
        outCodeInBinaryFIX += Trim(StrSplit(A_LoopField5, ";", 1)) + "\n";
    }
    code = StringTrimRight(outCodeInBinaryFIX, 1);
    std::vector<std::string> items6 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index6 = 0; A_Index6 < items6.size() + 0; A_Index6++) {
        std::string A_LoopField6 = items6[A_Index6 - 0];
        if (SubStr(A_LoopField6, 1, 1) == ";") {
            outCodeInBinary += "";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "MOV ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += MOVinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "ADD ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += ADDinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "SUB ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += SUBinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "MUL ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += MULinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "DEV ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += DEVinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "INC ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += INCinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "DEC ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += DECinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 6) == "LABEL ") {
            str1 = StringTrimLeft(A_LoopField6, 6);
            outCodeInBinary += LABELinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "JMP ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += JMPinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "CMP ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += CMPinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 3) == "JE ") {
            str1 = StringTrimLeft(A_LoopField6, 3);
            outCodeInBinary += JEinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "JNE ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += JNEinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 3) == "JM ") {
            str1 = StringTrimLeft(A_LoopField6, 3);
            outCodeInBinary += JMinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 3) == "JL ") {
            str1 = StringTrimLeft(A_LoopField6, 3);
            outCodeInBinary += JLinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "JEM ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += JEMinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "JEL ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            outCodeInBinary += JELinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 5) == "LOAD ") {
            str1 = StringTrimLeft(A_LoopField6, 5);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += LOADinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 6) == "DRAWX ") {
            str1 = StringTrimLeft(A_LoopField6, 6);
            outCodeInBinary += DRAWXinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 6) == "DRAWY ") {
            str1 = StringTrimLeft(A_LoopField6, 6);
            outCodeInBinary += DRAWYinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 5) == "DRAW ") {
            str1 = StringTrimLeft(A_LoopField6, 5);
            outCodeInBinary += DRAWinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 6) == "PRINT ") {
            str1 = StringTrimLeft(A_LoopField6, 6);
            outCodeInBinary += PRINTinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 7) == "PRINTS ") {
            str1 = StringTrimLeft(A_LoopField6, 7);
            outCodeInBinary += PRINTSinB + " " + decodeToBinary(str1) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 4) == "MOD ") {
            str1 = StringTrimLeft(A_LoopField6, 4);
            str2 = Trim(StrSplit(str1, ",", 2));
            outCodeInBinary += MODinB + " " + decodeToBinary(str1) + " " + decodeToBinary(str2) + "\n";
        }
        else if (SubStr(A_LoopField6, 1, 3) == "HLT") {
            str1 = StringTrimLeft(A_LoopField6, 3);
            outCodeInBinary += HLTinB + "\n";
        }
    }
    outCodeInBinary = StringTrimRight(outCodeInBinary, 1);
    return outCodeInBinary;
}
// how my asm will look like
/*
MOV r1, 2
MOV r2, 3
ADD r1, r2
// 17 since 1-16 is resereved for the registors
MOV l17, r1
PRINT l17
*/
std::vector<int> memLoc;
int PC = 0;
bool halted = false;
void ALU(std::string in1, std::string in2, std::string instruction) {
    int temp1 = 0;
    int temp2 = 0;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        temp1 = memLoc[binToDec(in1)];
    } else {
        in1 = StringTrimLeft(in1, 1);
        temp1 = binToDec(in1);
    }
    if (SubStr(in2, 1, 1) == "1") {
        in2 = StringTrimLeft(in2, 1);
        temp2 = memLoc[binToDec(in2)];
    } else {
        in2 = StringTrimLeft(in2, 1);
        temp2 = binToDec(in2);
    }
    int r1 = temp1;
    int r2 = temp2;
    if (instruction == "00001") {
        r1 = r1 + r2;
    }
    if (instruction == "00010") {
        r1 = r1 - r2;
    }
    if (instruction == "00011") {
        r1 = r1 * r2;
    }
    if (instruction == "00100") {
        r1 = r1 / r2;
    }
    if (instruction == "00101") {
        r1 = r1 + 1;
    }
    if (instruction == "00100") {
        r1 = r1 - 1;
    }
    memLoc[1] = r1;
}
void PRINTinst(std::string in1) {
    int temp1 = 0;
    int temp2 = 0;
    bool readFrom1 = false;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        readFrom1 = true;
        temp1 = binToDec(in1);
    } else {
        in1 = StringTrimLeft(in1, 1);
        readFrom1 = false;
        temp1 = binToDec(in1);
    }
    if (readFrom1) {
        temp2 = memLoc[temp1];
    } else {
        temp2 = temp1;
    }
    print(temp2);
}
void PRINTSinst(std::string in1) {
    int temp1 = 0;
    int temp2 = 0;
    bool readFrom1 = false;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        readFrom1 = true;
        temp1 = binToDec(in1);
    } else {
        in1 = StringTrimLeft(in1, 1);
        readFrom1 = false;
        temp1 = binToDec(in1);
    }
    if (readFrom1) {
        temp2 = memLoc[temp1];
    } else {
        temp2 = temp1;
    }
    print(Chr(temp2));
}
void MOVinst(std::string in1, std::string in2) {
    bool readFrom2 = false;
    int temp1 = 0;
    int temp2 = 0;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        temp1 = binToDec(in1);
    }
    if (SubStr(in2, 1, 1) == "1") {
        in2 = StringTrimLeft(in2, 1);
        readFrom2 = true;
        temp2 = binToDec(in2);
    } else {
        in2 = StringTrimLeft(in2, 1);
        readFrom2 = false;
        temp2 = binToDec(in2);
    }
    int temp3 = 0;
    if (readFrom2) {
        temp3 = memLoc[temp2];
    } else {
        temp3 = temp2;
    }
    memLoc[temp1] = temp3;
}
void LOADinst(std::string in1, std::string in2) {
    int temp1 = 0;
    int temp2 = 0;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        temp1 = binToDec(in1);
    }
    if (SubStr(in2, 1, 1) == "1") {
        in2 = StringTrimLeft(in2, 1);
        temp2 = binToDec(in2);
    } else {
        in2 = StringTrimLeft(in2, 1);
        temp2 = binToDec(in2);
    }
    memLoc[temp1] = memLoc[temp2];
}
void CMPinst(std::string in1, std::string in2) {
    int temp1 = 0;
    int temp2 = 0;
    if (SubStr(in1, 1, 1) == "1") {
        in1 = StringTrimLeft(in1, 1);
        temp1 = memLoc[binToDec(in1)];
    } else {
        in1 = StringTrimLeft(in1, 1);
        temp1 = binToDec(in1);
    }
    if (SubStr(in2, 1, 1) == "1") {
        in2 = StringTrimLeft(in2, 1);
        temp2 = memLoc[binToDec(in2)];
    } else {
        in2 = StringTrimLeft(in2, 1);
        temp2 = binToDec(in2);
    }
    // Clear all comparison flags
    memLoc[2] = 0;
    memLoc[3] = 0;
    memLoc[4] = 0;
    memLoc[5] = 0;
    memLoc[6] = 0;
    memLoc[7] = 0;
    if (temp1 == temp2) {
        memLoc[2] = 1;
    } else {
        memLoc[2] = 0;
    }
    if (temp1 != temp2) {
        memLoc[3] = 1;
    } else {
        memLoc[3] = 0;
    }
    if (temp1 > temp2) {
        memLoc[4] = 1;
    } else {
        memLoc[4] = 0;
    }
    if (temp1 < temp2) {
        memLoc[5] = 1;
    } else {
        memLoc[5] = 0;
    }
    if (temp1 >= temp2) {
        memLoc[6] = 1;
    } else {
        memLoc[6] = 0;
    }
    if (temp1 <= temp2) {
        memLoc[7] = 1;
    } else {
        memLoc[7] = 0;
    }
}
void JEinst(std::string in1) {
    if (memLoc[2] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JNEinst(std::string in1) {
    if (memLoc[3] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JMinst(std::string in1) {
    if (memLoc[4] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JLinst(std::string in1) {
    if (memLoc[5] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JEMinst(std::string in1) {
    if (memLoc[6] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JELinst(std::string in1) {
    if (memLoc[7] == 1) {
        PC = binToDec(in1) - 1;
    }
}
void JMPinst(std::string in1) {
    PC = binToDec(in1) - 1;
}
void decoder(std::string instrunction) {
    str1 = "";
    str2 = "";
    str3 = "";
    str1 = StrSplit(instrunction, " ", 1);
    str2 = StrSplit(instrunction, " ", 2);
    str3 = StrSplit(instrunction, " ", 3);
    if (str1 == "10111") {
        halted = true;
    }
    if (str1 == "00000") {
        MOVinst(str2, str3);
    }
    if (str1 == "10100") {
        PRINTinst(str2);
    }
    if (str1 == "10101") {
        PRINTSinst(str2);
    }
    if (str1 == "00001") {
        ALU(str2, str3, str1);
    }
    if (str1 == "00010") {
        ALU(str2, str3, str1);
    }
    if (str1 == "00011") {
        ALU(str2, str3, str1);
    }
    if (str1 == "00100") {
        ALU(str2, str3, str1);
    }
    if (str1 == "00101") {
        ALU(str2, str3, str1);
    }
    if (str1 == "00110") {
        ALU(str2, str3, str1);
    }
    if (str1 == "10000") {
        LOADinst(str2, str3);
    }
    if (str1 == "01001") {
        CMPinst(str2, str3);
    }
    if (str1 == "01000") {
        JMPinst(str2);
    }
    if (str1 == "01010") {
        JEinst(str2);
    }
    if (str1 == "01011") {
        JNEinst(str2);
    }
    if (str1 == "01100") {
        JMinst(str2);
    }
    if (str1 == "01101") {
        JLinst(str2);
    }
    if (str1 == "01110") {
        JEMinst(str2);
    }
    if (str1 == "01111") {
        JELinst(str2);
    }
}
void executeInst(std::string allInst, int thePC) {
    std::vector<std::string> items7 = LoopParseFunc(allInst, "\n", "\r");
    for (size_t A_Index7 = 0; A_Index7 < items7.size() + 0; A_Index7++) {
        std::string A_LoopField7 = items7[A_Index7 - 0];
        if (A_Index7 == thePC) {
            decoder(A_LoopField7);
            PC++;
            break;
        }
    }
}
void cpu(std::string htasm) {
    for (int A_Index8 = 0; A_Index8 < 2048 + 0; A_Index8++) {
        memLoc.push_back(0);
    }
    std::string BinaryInstructions = "";
    //print(compiler("MOV r3, 6\nMOV r4, 5\nADD r3, r4"))
    //BinaryInstructions := compiler("MOV r3, 6\nMOV r4, 5\nADD r3, r4\nPRINT r1\nHLT")
    //BinaryInstructions := compiler("MOV l17, 64\nLABEL 2\nINC l17\nMOV l17, r1\nPRINTS r1\nCMP r1, 70\nJNE 2\nHLT")
    //BinaryInstructions := compiler("MOV l21, 2\nMOV l22, 8\nMOV l20, 1\nMOV l19, 0\nLABEL 5\nMUL l20, l21\nMOV l20, r1\nINC l19\nMOV l19, r1\nCMP l19, l22\nJNE 5\nPRINT l20\nHLT")
    //BinaryInstructions := compiler("MOV l30, 69\nMOV l21, l30\nLOAD l50, l21\nPRINT l50\nHLT")
    BinaryInstructions = compiler(htasm);
    print(BinaryInstructions);
    int limitIndexDebug = 0;
    while (!halted) {
        limitIndexDebug++;
        if (limitIndexDebug > 2048) {
            print("it never halted");
            break;
        }
        executeInst(BinaryInstructions, PC);
    }
}
int main(int argc, char* argv[]) {
    std::string params = GetParams();
    if (params == "") {
        print("Usage:\nCPU-ASM-EMULATOR yourFile.htasm");
        ExitApp();
    }
    std::string ASMcode = FileRead(Trim(params));
    cpu(ASMcode);
    return 0;
}