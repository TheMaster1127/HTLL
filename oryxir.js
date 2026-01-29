

function LoopParseFunc(varString, delimiter1="", delimiter2="") {
    let items;
    if (!delimiter1 && !delimiter2) {
        // If no delimiters are provided, return an array of characters
        items = [...varString];
    } else {
        // Construct the regular expression pattern for splitting the string
        let pattern = new RegExp('[' + delimiter1.replace(/[.*+?^${}()|[\]\\]/g, '\\$&') + delimiter2.replace(/[.*+?^${}()|[\]\\]/g, '\\$&') + ']+');
        // Split the string using the constructed pattern
        items = varString.split(pattern);
    }
    return items;
}

function print(value) {
    console.log(value)
}

// Convert value to string
function STR(value) {
    if (value === null || value === undefined) {
        return ""; // Return a string for null or undefined
    } else if (typeof value === 'number') {
        return value.toString();
    } else if (typeof value === 'boolean') {
        return value ? "1" : "0";
    } else if (typeof value === 'string') {
        return value; // Return the string as is
    } else {
        // Handle any unexpected types gracefully
        return String(value); // Convert any other type to a string
    }
}

// Convert value to integer
function INT(value) {
    const intValue = parseInt(value, 10);
    if (Number.isNaN(intValue)) {
        console.warn(`Invalid input: ${value} cannot be converted to an integer.`);
        return 0;  // Or any other default value
    }
    return intValue;
}

// Convert value to float
function FLOAT(value) {
    const floatValue = parseFloat(value);
    if (isNaN(floatValue)) {
        throw new TypeError("Cannot convert to float");
    }
    return floatValue;
}

// Function to find the position of needle in haystack (string overload)
function InStr(haystack, needle) {
    const pos = haystack.indexOf(needle);
    return (pos !== -1) ? pos + 1 : 0;
}

function Sleep(milliseconds) {
    // Sleep for the specified number of milliseconds
    return new Promise(resolve => setTimeout(resolve, milliseconds));
}

// Function to simulate input() in JavaScript
function input(promptText) {
    // Display the prompt and get user input
    return prompt(promptText);
}

function StrLen(s) {
    // Return the length of the given string
    return s.length;
}

function Chr(number) {
    // Return the character corresponding to the Unicode code point, or an empty string if out of range
    return (number >= 0 && number <= 0x10FFFF) ? String.fromCharCode(number) : "";
}

function Asc(s) {
    return s.length > 0 ? s.charCodeAt(0) : -1;
}

function SubStr(str, startPos, length = -1) {
    let result = "";
    let strLen = str.length;
    // Handle negative starting positions (counting from the end)
    if (startPos < 0) {
        startPos = strLen + startPos;
        if (startPos < 0) startPos = 0;  // Ensure it doesn't go beyond the start of the string
    } else {
        startPos -= 1; // Convert to 0-based index for internal operations
    }
    // Handle length
    if (length < 0) {
        length = strLen - startPos; // Length to the end of the string
    } else if (startPos + length > strLen) {
        length = strLen - startPos; // Adjust length to fit within the string
    }
    // Extract the substring
    result = str.substr(startPos, length);
    return result;
}

function Trim(inputString) {
    return inputString ? inputString.trim() : "";
}

function StrReplace(originalString, find, replaceWith) {
    return originalString.split(find).join(replaceWith);
}

function StringTrimLeft(input, numChars) {
    return (numChars <= input.length) ? input.substring(numChars) : input;
}

function StringTrimRight(input, numChars) {
    return (numChars <= input.length) ? input.substring(0, input.length - numChars) : input;
}

function StrSplit(inputStr, delimiter, num) {
    const parts = inputStr.split(delimiter);
    return (num > 0 && num <= parts.length) ? parts[num - 1] : "";
}

function Mod(dividend, divisor) {
    return dividend % divisor;
}

// RegExMatch
function RegExMatch(haystack, needle) {
    const regex = new RegExp(needle);
    const match = haystack.match(regex);
    return match ? match.index + 1 : 0; // 1-based index or 0 if no match
}

function HTVM_getLang_HTVM() {
    return "js";
}

function HTVM_Append(arr, value) {
    arr.push(value);
}

function HTVM_Pop(arr) {
    arr.pop();
}

function HTVM_Size(arr) {
    return arr.length;
}

function HTVM_Remove(arr, index) {
    if (index >= 0 && index < arr.length) arr.splice(index, 1);
}



// start of HT-Lib.htvm
// global vars NEEDED
var HT_LIB_theIdNumOfThe34 = 0;
let HT_Lib_theIdNumOfThe34theVar = [];
//;;;;;;;;;;;;;;;;;;;;;;;;;
function preserveStrings(code, keyWordEscpaeChar = "\\") {
    HT_LIB_theIdNumOfThe34 = 0;
    HT_Lib_theIdNumOfThe34theVar = [];
    let getAllCharForTheFurtureSoIcanAddEscapeChar = [];
    var ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = "";
    var str21 = "";
    var htCodeOUT754754 = "";
    var OutFixDoubleQuotesInsideDoubleQuotes = "";
    var fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
    var removeNexFixkeyWordEscpaeChar = 0;
    var areWEinSome34sNum = 0;
    let items1 = LoopParseFunc(code);
    for (let A_Index1 = 0; A_Index1 < items1.length; A_Index1++) {
        const A_LoopField1 = items1[A_Index1 - 0];
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
    }
    let items2 = LoopParseFunc(code);
    for (let A_Index2 = 0; A_Index2 < items2.length; A_Index2++) {
        const A_LoopField2 = items2[A_Index2 - 0];
        HT_Lib_theIdNumOfThe34theVar[A_Index2] = HT_Lib_theIdNumOfThe34theVar[A_Index2] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField2);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "--" + str21 + "asds" + str21 + "as--" + str21 + "theuhtuwaesphoutr" + Chr(34);
    let items3 = LoopParseFunc(code);
    for (let A_Index3 = 0; A_Index3 < items3.length; A_Index3++) {
        const A_LoopField3 = items3[A_Index3 - 0];
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
        let items4 = LoopParseFunc(code);
        for (let A_Index4 = 0; A_Index4 < items4.length; A_Index4++) {
            const A_LoopField4 = items4[A_Index4 - 0];
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
        let items5 = LoopParseFunc(code);
        for (let A_Index5 = 0; A_Index5 < items5.length; A_Index5++) {
            const A_LoopField5 = items5[A_Index5 - 0];
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
    for (let A_Index6 = 0; A_Index6 < HT_LIB_theIdNumOfThe34; A_Index6++) {
        HT_Lib_theIdNumOfThe34theVar[A_Index6] = HT_Lib_theIdNumOfThe34theVar[A_Index6] + Chr(34);
    }
    HTVM_Append(HT_Lib_theIdNumOfThe34theVar, Chr(34));
    return code;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;
function restoreStrings(codeOUT, keyWordEscpaeChar = Chr(92)) {
    for (let A_Index7 = 0; A_Index7 < HT_LIB_theIdNumOfThe34; A_Index7++) {
        if (HT_LIB_theIdNumOfThe34 == A_Index7 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\"));
        }
    }
    return codeOUT;
}
function cleanUpFirst(code) {
    code = StrReplace(code, Chr(13), "");
    var out = "";
    let items8 = LoopParseFunc(code, "\n", "\r");
    for (let A_Index8 = 0; A_Index8 < items8.length; A_Index8++) {
        const A_LoopField8 = items8[A_Index8 - 0];
        out += Trim(A_LoopField8) + Chr(10);
    }
    out = StringTrimRight(out, 1);
    return out;
}
function getLangParams(binName, langExtension, extra = "") {
    var params = Trim(GetParams());
    if (params == "") {
        if (isWindows()) {
            print("Usage:" + Chr(10) + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        } else {
            print("Usage:" + Chr(10) + "./" + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        }
        return "";
    } else {
        // Loop through the parameters and return the VERY FIRST one.
        let items9 = LoopParseFunc(params, "\n", "\r");
        for (let A_Index9 = 0; A_Index9 < items9.length; A_Index9++) {
            const A_LoopField9 = items9[A_Index9 - 0];
            var first_param = Trim(A_LoopField9);
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
function saveOutput(outCode, fileName) {
    FileDelete(Trim(fileName));
    FileAppend(Trim(outCode), Trim(fileName));
    print("Generation finished: " + Trim(fileName) + " generated.");
}
function beginning(line, what) {
    if (SubStr(line, 1, StrLen(what)) == what) {
        return true;
    }
    return false;
}
function formatCurlyBracesForParsing(code, openBrace = "{", closeBrace = "}") {
    code = StrReplace(code, openBrace, Chr(10) + "{" + Chr(10));
    code = StrReplace(code, closeBrace, Chr(10) + "}" + Chr(10));
    code = cleanUpFirst(code);
    return code;
}
function handleComments(code, commentKeyword = ";") {
    var str1 = "";
    let items10 = LoopParseFunc(code, "\n", "\r");
    for (let A_Index10 = 0; A_Index10 < items10.length; A_Index10++) {
        const A_LoopField10 = items10[A_Index10 - 0];
        str1 += StrSplit(A_LoopField10, commentKeyword, 1) + Chr(10);
    }
    code = StringTrimRight(str1, 1);
    return code;
}
// Define the function to check odd spaces at the beginning
function CheckOddLeadingSpaces(string123) {
    // Initialize a variable to count the spaces
    var spaceCount = 0;
    // Loop through the string one character at a time
    let items11 = LoopParseFunc(string123);
    for (let A_Index11 = 0; A_Index11 < items11.length; A_Index11++) {
        const A_LoopField11 = items11[A_Index11 - 0];
        // Check if the current character is a space
        if (A_LoopField11 == Chr(32)) {
            spaceCount++;
        } else {
            // When we hit a non-space character, break the loop
            break;
        }
    }
    // Return true if the number of spaces is odd, false otherwise
    var sdsfawasd = STR(Mod(spaceCount, 2) == 1);
    //MsgBox, % sdsfawasd
    return sdsfawasd;
}
function RepeatSpaces(count) {
    var spaces = "";
    for (let A_Index12 = 0; A_Index12 < count; A_Index12++) {
        spaces += Chr(32);
    }
    return spaces;
}
// if you wanna convert to python, nim etc... indentation style we set modeCurlyBracesOn to 0
function indent_nested_curly_braces(input_string, modeCurlyBracesOn = 1) {
    var indent_size = 4;
    var current_indent = 0;
    var result = "";
    var trimmed_line = "";
    var resultOut = "";
    var culyOpenFix = "{";
    var culyCloseFix = "}";
    //MsgBox, % input_string
    let items13 = LoopParseFunc(input_string, "\n", "\r");
    for (let A_Index13 = 0; A_Index13 < items13.length; A_Index13++) {
        const A_LoopField13 = items13[A_Index13 - 0];
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
        var resultOut = "";
        let items14 = LoopParseFunc(result, "\n", "\r");
        for (let A_Index14 = 0; A_Index14 < items14.length; A_Index14++) {
            const A_LoopField14 = items14[A_Index14 - 0];
            if (Trim(A_LoopField14) != "{" && Trim(A_LoopField14) != "}") {
                resultOut += A_LoopField14 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    } else {
        // format curly braces in a K&R style
        let lookIntoFurture = [];
        let items15 = LoopParseFunc(result, "\n", "\r");
        for (let A_Index15 = 0; A_Index15 < items15.length; A_Index15++) {
            const A_LoopField15 = items15[A_Index15 - 0];
            HTVM_Append(lookIntoFurture, Trim(A_LoopField15));
        }
        HTVM_Append(lookIntoFurture, " ");
        var resultOut = "";
        var skipNext = 0;
        let items16 = LoopParseFunc(result, "\n", "\r");
        for (let A_Index16 = 0; A_Index16 < items16.length; A_Index16++) {
            const A_LoopField16 = items16[A_Index16 - 0];
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
        let lookIntoFurture2 = [];
        let items17 = LoopParseFunc(result, "\n", "\r");
        for (let A_Index17 = 0; A_Index17 < items17.length; A_Index17++) {
            const A_LoopField17 = items17[A_Index17 - 0];
            HTVM_Append(lookIntoFurture2, Trim(A_LoopField17));
        }
        HTVM_Append(lookIntoFurture2, " ");
        resultOut = "";
        skipNext = 0;
        var addSpacesAtTheBegginig;
        let items18 = LoopParseFunc(result, "\n", "\r");
        for (let A_Index18 = 0; A_Index18 < items18.length; A_Index18++) {
            const A_LoopField18 = items18[A_Index18 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(A_LoopField18) == "}" && Trim(lookIntoFurture2[A_Index18 + 1]) == "else {") {
                skipNext = 2;
                addSpacesAtTheBegginig = "";
                let items19 = LoopParseFunc(A_LoopField18);
                for (let A_Index19 = 0; A_Index19 < items19.length; A_Index19++) {
                    const A_LoopField19 = items19[A_Index19 - 0];
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
    var ALoopField;
    let items20 = LoopParseFunc(result, "\n", "\r");
    for (let A_Index20 = 0; A_Index20 < items20.length; A_Index20++) {
        const A_LoopField20 = items20[A_Index20 - 0];
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
let Official_Oryx_VM_symbol_table = [];
let Official_Oryx_VM_reg_array = [];
// --- API STATE CACHE ---
// These globals store the state of the *last* completed interpreter run.
let Official_Oryx_VM_API_symbol_table_cache = [];
function print_raw(value) {
    
    
            process.stdout.write(String(value));
}
// --- HELPER: SAFE TOKEN GETTER ---
// Replaces StrSplit(str, delim, index) to avoid crashes on missing parts.
function get_token(input, delim, index) {
    var current = 1;
    let items21 = LoopParseFunc(input, delim);
    for (let A_Index21 = 0; A_Index21 < items21.length; A_Index21++) {
        const A_LoopField21 = items21[A_Index21 - 0];
        if (current == index) {
            return A_LoopField21;
        }
        current++;
    }
    return "";
}
// --- HELPER FUNCTION: GET VALUE ---
function get_value(operand) {
    if (RegExMatch(operand, "^r\\d+$")) {
        var reg_index = INT(StringTrimLeft(operand, 1));
        return Official_Oryx_VM_reg_array[reg_index];
    }
    else if (RegExMatch(operand, "^-?\\d+(\\.\\d+)?$")) {
        return operand;
    }
    else if (SubStr(operand, 1, 1) == Chr(34)) {
        var val = StringTrimLeft(operand, 1);
        val = StringTrimRight(val, 1);
        val = StrReplace(val, "\\n", Chr(10));
        val = StrReplace(val, "\\r", Chr(13));
        val = StrReplace(val, "\\t", Chr(9));
        return val;
    } else {
        var i = 0;
        for (let A_Index22 = 0; A_Index22 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index22++) {
            var entry = Official_Oryx_VM_symbol_table[i];
            if (get_token(entry, Chr(254), 1) == operand) {
                return get_token(entry, Chr(254), 3);
            }
            i++;
        }
    }
    return "";
}
// --- HELPER FUNCTION: SET VALUE ---
function set_value(dest_operand, new_value) {
    if (RegExMatch(dest_operand, "^r\\d+$")) {
        var reg_index = INT(StringTrimLeft(dest_operand, 1));
        Official_Oryx_VM_reg_array[reg_index] = new_value;
    } else {
        var i = 0;
        for (let A_Index23 = 0; A_Index23 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index23++) {
            var entry = Official_Oryx_VM_symbol_table[i];
            var var_name = get_token(entry, Chr(254), 1);
            if (var_name == dest_operand) {
                var var_type = get_token(entry, Chr(254), 2);
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
function OryxAPI_GetVariable(operand) {
    if (RegExMatch(operand, "^r\\d+$")) {
        var reg_index = INT(StringTrimLeft(operand, 1));
        return Official_Oryx_VM_reg_array[reg_index];
    }
    else if (RegExMatch(operand, "^-?\\d+(\\.\\d+)?$")) {
        return operand;
    }
    else if (SubStr(operand, 1, 1) == Chr(34)) {
        var val = StringTrimLeft(operand, 1);
        val = StringTrimRight(val, 1);
        val = StrReplace(val, "\\n", Chr(10));
        val = StrReplace(val, "\\r", Chr(13));
        val = StrReplace(val, "\\t", Chr(9));
        return val;
    } else {
        var i = 0;
        for (let A_Index24 = 0; A_Index24 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index24++) {
            var entry = Official_Oryx_VM_symbol_table[i];
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
function OryxAPI_GetArray(arr_name) {
    let result_array = [];
    var content_string = "";
    var i = 0;
    for (let A_Index25 = 0; A_Index25 < HTVM_Size(Official_Oryx_VM_API_symbol_table_cache); A_Index25++) {
        var entry = Official_Oryx_VM_API_symbol_table_cache[i];
        // Check if it's an array and the name matches
        if (get_token(entry, Chr(254), 1) == arr_name && get_token(entry, Chr(254), 2) == "arr") {
            content_string = get_token(entry, Chr(254), 3);
            break;
        }
        i++;
    }
    if (content_string != "") {
        let items26 = LoopParseFunc(content_string, Chr(253));
        for (let A_Index26 = 0; A_Index26 < items26.length; A_Index26++) {
            const A_LoopField26 = items26[A_Index26 - 0];
            HTVM_Append(result_array, A_LoopField26);
        }
    }
    return result_array;
}
// This is the gatekeeper for all external string-to-variable operations.
// It checks the destination type and formats the data accordingly.
function set_value_type_aware(dest_operand, new_value) {
    if (RegExMatch(dest_operand, "^r\\d+$")) {
        // Registers are typeless. Set the value directly.
        var reg_index = INT(StringTrimLeft(dest_operand, 1));
        Official_Oryx_VM_reg_array[reg_index] = new_value;
        return;
    }
    // Find the destination variable's type from the symbol table.
    var dest_type = "";
    var i = 0;
    for (let A_Index27 = 0; A_Index27 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index27++) {
        var entry = Official_Oryx_VM_symbol_table[i];
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
        var numeric_content = "";
        let items28 = LoopParseFunc(new_value);
        for (let A_Index28 = 0; A_Index28 < items28.length; A_Index28++) {
            const A_LoopField28 = items28[A_Index28 - 0];
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
function Oryx_interpreter(code) {
    var outState = "success";
    var str1 = "";
    var str2 = "";
    var str3 = "";
    var str4 = "";
    var ALoopField = "";
    var current_line = "";
    let all_instr = [];
    let label_table = [];
    var zero_flag = 0;
    var sign_flag = 0;
    let call_stack = [];
    let main_stack = [];
    for (let A_Index29 = 0; A_Index29 < 100; A_Index29++) {
        HTVM_Append(Official_Oryx_VM_reg_array, "");
    }
    let items30 = LoopParseFunc(code, "\n", "\r");
    for (let A_Index30 = 0; A_Index30 < items30.length; A_Index30++) {
        const A_LoopField30 = items30[A_Index30 - 0];
        HTVM_Append(all_instr, A_LoopField30);
    }
    // --- POPULATE ARGS_ARRAY (Corrected to mimic x86 byte buffer) ---
    var args_init_content = "";
    var sys_params = GetParams();
    let items31 = LoopParseFunc(sys_params, "\n", "\r");
    for (let A_Index31 = 0; A_Index31 < items31.length; A_Index31++) {
        const A_LoopField31 = items31[A_Index31 - 0];
        // Skip the first argument (the script file itself)
        if (A_Index31 == 0) {
            continue;
        }
        var current_arg = Trim(A_LoopField31);
        if (current_arg != "") {
            // --- UNPACK BYTES ---
            // Loop through each character of the argument string
            let items32 = LoopParseFunc(current_arg);
            for (let A_Index32 = 0; A_Index32 < items32.length; A_Index32++) {
                const A_LoopField32 = items32[A_Index32 - 0];
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
    var i = 0;
    for (let A_Index33 = 0; A_Index33 < HTVM_Size(all_instr); A_Index33++) {
        ALoopField = Trim(all_instr[i]);
        if (SubStr(ALoopField, -1) == ":") {
            var label_name = StringTrimRight(ALoopField, 1);
            HTVM_Append(label_table, label_name + Chr(254) + STR(i));
        }
        i++;
    }
    var pc = 0;
    for (let A_Index34 = 0; ; A_Index34++) {
        if (pc >= HTVM_Size(all_instr)) {
            break;
        }
        current_line = Trim(all_instr[pc]);
        var pc_was_modified = false;
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
            var first_colon = InStr(str1, ":");
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
            var first_colon = InStr(str1, ":");
            str2 = Trim(SubStr(str1, 1, first_colon - 1));
            str3 = Trim(SubStr(str1, first_colon + 1));
            HTVM_Append(Official_Oryx_VM_symbol_table, str2 + Chr(254) + "int" + Chr(254) + str3);
        }
        if (SubStr(current_line, 1, 6) == "float ") {
            str1 = StringTrimLeft(current_line, 6);
            var first_colon = InStr(str1, ":");
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
            var op1 = "";
            var op2 = "";
            // --- FIX: Manually parse to respect commas in string literals ---
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1 = Trim(SubStr(str1, 1, first_comma - 1));
                op2 = Trim(SubStr(str1, first_comma + 1));
            } else {
                // This would be a syntax error, but we'll handle it gracefully
                op1 = Trim(str1);
            }
            // --- END FIX ---
            if (op2 != "") {
                var value_to_move = get_value(op2);
                // --- REPLACE THIS LINE ---
                // set_value(op1, value_to_move)
                // --- WITH THIS LINE ---
                set_value_type_aware(op1, value_to_move);
            }
        }
        if (SubStr(current_line, 1, 4) == "add ") {
            str1 = StringTrimLeft(current_line, 4);
            var op1 = Trim(get_token(str1, ",", 1));
            var op2 = Trim(get_token(str1, ",", 2));
            var val1 = FLOAT(get_value(op1));
            var val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 + val2));
        }
        if (SubStr(current_line, 1, 4) == "sub ") {
            str1 = StringTrimLeft(current_line, 4);
            var op1 = Trim(get_token(str1, ",", 1));
            var op2 = Trim(get_token(str1, ",", 2));
            var val1 = FLOAT(get_value(op1));
            var val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 - val2));
        }
        if (SubStr(current_line, 1, 4) == "mul ") {
            str1 = StringTrimLeft(current_line, 4);
            var op1 = Trim(get_token(str1, ",", 1));
            var op2 = Trim(get_token(str1, ",", 2));
            var val1 = FLOAT(get_value(op1));
            var val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 * val2));
        }
        if (SubStr(current_line, 1, 4) == "div ") {
            str1 = StringTrimLeft(current_line, 4);
            var op1 = Trim(get_token(str1, ",", 1));
            var op2 = Trim(get_token(str1, ",", 2));
            var val1 = FLOAT(get_value(op1));
            var val2 = FLOAT(get_value(op2));
            set_value(op1, STR(val1 / val2));
        }
        if (SubStr(current_line, 1, 8) == "add_str ") {
            str1 = StringTrimLeft(current_line, 8);
            var op1 = "";
            var op2 = "";
            // --- FIX: Manually parse to respect commas in string literals ---
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1 = Trim(SubStr(str1, 1, first_comma - 1));
                op2 = Trim(SubStr(str1, first_comma + 1));
            } else {
                op1 = Trim(str1);
            }
            // --- END FIX ---
            if (op2 != "") {
                var s1 = get_value(op1);
                var s2 = get_value(op2);
                set_value(op1, s1 + s2);
            }
        }
        if (SubStr(current_line, 1, 4) == "inc ") {
            var op1 = Trim(StringTrimLeft(current_line, 4));
            var val = FLOAT(get_value(op1));
            set_value(op1, STR(val + 1));
        }
        if (SubStr(current_line, 1, 4) == "dec ") {
            var op1 = Trim(StringTrimLeft(current_line, 4));
            var val = FLOAT(get_value(op1));
            set_value(op1, STR(val - 1));
        }
        if (SubStr(current_line, 1, 4) == "cmp ") {
            str1 = StringTrimLeft(current_line, 4);
            var op1_str = "";
            var op2_str = "";
            // --- FIX: Use manual parsing to handle potential commas in string literals ---
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                op1_str = Trim(SubStr(str1, 1, first_comma - 1));
                op2_str = Trim(SubStr(str1, first_comma + 1));
            } else {
                // This would be a syntax error, but we'll handle it gracefully
                op1_str = Trim(str1);
            }
            var s_val1 = get_value(op1_str);
            var s_val2 = get_value(op2_str);
            // --- FIX: Check if we are comparing numbers or strings ---
            if (RegExMatch(s_val1, "^-?\\d+(\\.\\d+)?$") && RegExMatch(s_val2, "^-?\\d+(\\.\\d+)?$")) {
                // --- Numeric Comparison ---
                var val1 = FLOAT(s_val1);
                var val2 = FLOAT(s_val2);
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
            var label_name = Trim(StringTrimLeft(current_line, 4));
            var j = 0;
            for (let A_Index35 = 0; A_Index35 < HTVM_Size(label_table); A_Index35++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 3));
                var j = 0;
                for (let A_Index36 = 0; A_Index36 < HTVM_Size(label_table); A_Index36++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 4));
                var j = 0;
                for (let A_Index37 = 0; A_Index37 < HTVM_Size(label_table); A_Index37++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 3));
                var j = 0;
                for (let A_Index38 = 0; A_Index38 < HTVM_Size(label_table); A_Index38++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 3));
                var j = 0;
                for (let A_Index39 = 0; A_Index39 < HTVM_Size(label_table); A_Index39++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 4));
                var j = 0;
                for (let A_Index40 = 0; A_Index40 < HTVM_Size(label_table); A_Index40++) {
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
                var label_name = Trim(StringTrimLeft(current_line, 4));
                var j = 0;
                for (let A_Index41 = 0; A_Index41 < HTVM_Size(label_table); A_Index41++) {
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
            var dest = "";
            var prompt_source = "";
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                dest = Trim(SubStr(str1, 1, first_comma - 1));
                prompt_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                dest = Trim(str1);
            }
            var prompt_msg = get_value(prompt_source);
            var user_input = input(prompt_msg);
            set_value_type_aware(dest, user_input);
        }
        if (SubStr(current_line, 1, 10) == "file.read ") {
            str1 = StringTrimLeft(current_line, 10);
            var dest = "";
            var path_source = "";
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                dest = Trim(SubStr(str1, 1, first_comma - 1));
                path_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                dest = Trim(str1);
            }
            var filepath = get_value(path_source);
            var content = FileRead(filepath);
            // --- THE FIX: CHECK DESTINATION TYPE ---
            var dest_type = "";
            var sym_idx = 0;
            for (let A_Index42 = 0; A_Index42 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index42++) {
                if (get_token(Official_Oryx_VM_symbol_table[sym_idx], Chr(254), 1) == dest) {
                    dest_type = get_token(Official_Oryx_VM_symbol_table[sym_idx], Chr(254), 2);
                    break;
                }
                sym_idx++;
            }
            if (dest_type == "arr") {
                // Destination is an array. Convert content to delimited ASCII values.
                var numeric_content = "";
                let items43 = LoopParseFunc(content);
                for (let A_Index43 = 0; A_Index43 < items43.length; A_Index43++) {
                    const A_LoopField43 = items43[A_Index43 - 0];
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
            var path_source = "";
            var content_source = "";
            var first_comma = InStr(str1, ",");
            if (first_comma > 0) {
                path_source = Trim(SubStr(str1, 1, first_comma - 1));
                content_source = Trim(SubStr(str1, first_comma + 1));
            } else {
                path_source = Trim(str1);
            }
            var filepath = get_value(path_source);
            var content = get_value(content_source);
            FileAppend(content, filepath);
        }
        if (SubStr(current_line, 1, 12) == "file.delete ") {
            str1 = StringTrimLeft(current_line, 12);
            var path_source = Trim(str1);
            var filepath = get_value(path_source);
            FileDelete(filepath);
        }
        if (SubStr(current_line, 1, 12) == "num_to_char ") {
            str1 = StringTrimLeft(current_line, 12);
            var src_reg = Trim(get_token(str1, ",", 1));
            var dest_reg = Trim(get_token(str1, ",", 2));
            var num_str = get_value(src_reg);
            var char_val = Chr(INT(num_str));
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
                var label_name = str1;
                var j = 0;
                for (let A_Index44 = 0; A_Index44 < HTVM_Size(label_table); A_Index44++) {
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
            var return_address = call_stack[HTVM_Size(call_stack) - 1];
            HTVM_Pop(call_stack);
            pc = return_address;
        }
        if (SubStr(current_line, 1, 5) == "push ") {
            var op1 = Trim(StringTrimLeft(current_line, 5));
            var val = get_value(op1);
            HTVM_Append(main_stack, op1 + Chr(254) + val);
        }
        if (SubStr(current_line, 1, 4) == "pop ") {
            var op1 = Trim(StringTrimLeft(current_line, 4));
            var target_prefix = op1 + Chr(254);
            var found_index = -1;
            var found_val = "";
            var stack_idx = HTVM_Size(main_stack) - 1;
            for (let A_Index45 = 0; ; A_Index45++) {
                if (stack_idx < 0) {
                    break;
                }
                var entry = main_stack[stack_idx];
                if (SubStr(entry, 1, StrLen(target_prefix)) == target_prefix) {
                    found_index = stack_idx;
                    found_val = StringTrimLeft(entry, StrLen(target_prefix));
                    break;
                }
                stack_idx--;
            }
            if (found_index != -1) {
                var reg_idx = INT(StringTrimLeft(op1, 1));
                Official_Oryx_VM_reg_array[reg_idx] = found_val;
                HTVM_Remove(main_stack, found_index);
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.add ") {
            str1 = StringTrimLeft(current_line, 8);
            var arr_name = Trim(get_token(str1, ",", 1));
            var val_source = Trim(get_token(str1, ",", 2));
            var val = get_value(val_source);
            var j = 0;
            for (let A_Index46 = 0; A_Index46 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index46++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    var current_content = get_token(entry, Chr(254), 3);
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
            var arr_name = Trim(StringTrimLeft(current_line, 8));
            var j = 0;
            for (let A_Index47 = 0; A_Index47 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index47++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    var content = get_token(entry, Chr(254), 3);
                    if (content != "") {
                        // Calculate number of items
                        var item_count = 0;
                        let items48 = LoopParseFunc(content, Chr(253));
                        for (let A_Index48 = 0; A_Index48 < items48.length; A_Index48++) {
                            const A_LoopField48 = items48[A_Index48 - 0];
                            item_count++;
                        }
                        // Rebuild string excluding the last item
                        var new_content = "";
                        var current_idx = 0;
                        let items49 = LoopParseFunc(content, Chr(253));
                        for (let A_Index49 = 0; A_Index49 < items49.length; A_Index49++) {
                            const A_LoopField49 = items49[A_Index49 - 0];
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
            var arr_name = Trim(get_token(str1, ",", 1));
            var index_source = Trim(get_token(str1, ",", 2));
            var dest_reg_str = Trim(get_token(str1, ",", 3));
            var index = INT(get_value(index_source));
            var j = 0;
            for (let A_Index50 = 0; A_Index50 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index50++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    var content = get_token(entry, Chr(254), 3);
                    var val = get_token(content, Chr(253), index + 1);
                    var reg_idx = INT(StringTrimLeft(dest_reg_str, 1));
                    Official_Oryx_VM_reg_array[reg_idx] = val;
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 8) == "arr.set ") {
            str1 = StringTrimLeft(current_line, 8);
            var arr_name = Trim(get_token(str1, ",", 1));
            var index_source = Trim(get_token(str1, ",", 2));
            var val_source = Trim(get_token(str1, ",", 3));
            var index = INT(get_value(index_source));
            var new_val = get_value(val_source);
            var j = 0;
            for (let A_Index51 = 0; A_Index51 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index51++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    var content = get_token(entry, Chr(254), 3);
                    var new_content = "";
                    var current_idx = 0;
                    let items52 = LoopParseFunc(content, Chr(253));
                    for (let A_Index52 = 0; A_Index52 < items52.length; A_Index52++) {
                        const A_LoopField52 = items52[A_Index52 - 0];
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
            var arr_name = Trim(get_token(str1, ",", 1));
            var dest_reg_str = Trim(get_token(str1, ",", 2));
            var j = 0;
            for (let A_Index53 = 0; A_Index53 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index53++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    var content = get_token(entry, Chr(254), 3);
                    // --- THE FINAL, TRUE FIX ---
                    var size = 0;
                    // This check IS needed, but the loop logic was wrong.
                    if (content != "") {
                        let items54 = LoopParseFunc(content, Chr(253));
                        for (let A_Index54 = 0; A_Index54 < items54.length; A_Index54++) {
                            const A_LoopField54 = items54[A_Index54 - 0];
                            // Simply increment for each item found.
                            size++;
                        }
                    }
                    var reg_idx = INT(StringTrimLeft(dest_reg_str, 1));
                    Official_Oryx_VM_reg_array[reg_idx] = STR(size);
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 10) == "arr.clear ") {
            var arr_name = Trim(StringTrimLeft(current_line, 10));
            var j = 0;
            for (let A_Index55 = 0; A_Index55 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index55++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == arr_name) {
                    Official_Oryx_VM_symbol_table[j] = arr_name + Chr(254) + "arr" + Chr(254) + "";
                    break;
                }
                j++;
            }
        }
        if (SubStr(current_line, 1, 9) == "arr.copy ") {
            str1 = StringTrimLeft(current_line, 9);
            var src_name = Trim(get_token(str1, ",", 1));
            var dest_name = Trim(get_token(str1, ",", 2));
            var content_to_copy = "";
            var j = 0;
            for (let A_Index56 = 0; A_Index56 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index56++) {
                var entry = Official_Oryx_VM_symbol_table[j];
                if (get_token(entry, Chr(254), 1) == src_name) {
                    content_to_copy = get_token(entry, Chr(254), 3);
                    break;
                }
                j++;
            }
            j = 0;
            for (let A_Index57 = 0; A_Index57 < HTVM_Size(Official_Oryx_VM_symbol_table); A_Index57++) {
                var entry = Official_Oryx_VM_symbol_table[j];
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
            var str_name = Trim(get_token(str1, ",", 1));
            var index_source = Trim(get_token(str1, ",", 2));
            var dest_reg_str = Trim(get_token(str1, ",", 3));
            var index = INT(get_value(index_source));
            var content = get_value(str_name);
            var charr = SubStr(content, index + 1, 1);
            set_value(dest_reg_str, charr);
        }
        if (SubStr(current_line, 1, 8) == "str.set ") {
            str1 = StringTrimLeft(current_line, 8);
            var str_name = Trim(get_token(str1, ",", 1));
            var index_source = Trim(get_token(str1, ",", 2));
            var char_source = Trim(get_token(str1, ",", 3));
            var index = INT(get_value(index_source));
            var new_val = get_value(char_source);
            var old_str = get_value(str_name);
            // --- HARD RULE: overwrite ONE character only ---
            var repl_char = SubStr(new_val, 1, 1);
            var part1 = "";
            if (index > 0) {
                part1 = SubStr(old_str, 1, index);
            }
            var part2 = SubStr(old_str, index + 2);
            var new_str = part1 + repl_char + part2;
            set_value(str_name, new_str);
        }
        if (SubStr(current_line, 1, 8) == "str.len ") {
            str1 = StringTrimLeft(current_line, 8);
            var str_name = Trim(get_token(str1, ",", 1));
            var dest_reg_str = Trim(get_token(str1, ",", 2));
            var content = get_value(str_name);
            var len = StrLen(content);
            set_value(dest_reg_str, STR(len));
        }
        if (!pc_was_modified) {
            pc++;
        }
    }
    Official_Oryx_VM_API_symbol_table_cache = Official_Oryx_VM_symbol_table;
    return outState;
}
function Oryx_VM(code) {
    var outState = "";
    code = Trim(code);
    code = cleanUpFirst(code);
    code = preserveStrings(code);
    code = handleComments(code, ";");
    code = restoreStrings(code);
    outState = Oryx_interpreter(Trim(code));
    return outState;
}
async function main() {
    var outState = "";
    var code = "";
    var params = "";
    if (HTVM_getLang_HTVM() != "js") {
        params = getLangParams("oryxir", "oryxir");
        if (params != "") {
            code = FileRead(params);
            outState = Oryx_VM(code);
            print(outState);
        }
    }
    

}
main();