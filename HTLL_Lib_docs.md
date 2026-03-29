# HTLL Standard String Library

This document outlines the built-in string and array manipulation functions available in `HTLL_Lib.htll`. 

> **Important Notes:**
> * HTLL arrays and strings are **0-indexed**.
> * HTLL does not use negative numbers. Functions that search for an index (like `InStr` or `RegExMatch`) will return the **length of the haystack** if the target is not found.

---

## Table of Contents
1. [StrLen](#1-strlenarr-string)
2. [Chr](#2-chrnum)
3. [StringTrimRight](#3-stringtrimrightarr-string-n)
4. [StringTrimLeft](#4-stringtrimleftarr-string-n)
5. [countChars](#5-countcharsarr-s-c)
6. [SubStr](#6-substrarr-s-start-len)
7. [Trim](#7-trimarr-s)
8. [InStr](#8-instrarr-haystack-arr-needle-startpos)
9. [StrLower](#9-strlowerarr-s)
10. [INT_To_STR](#10-int_to_strnum)
11. [StrReplace](#11-strreplacearr-haystack-arr-needle-arr-replacement)
12. [StrSplit](#12-strsplitarr-inputstr-arr-delimiter-num)
13. [LoopParseFunc](#13-loopparsefuncarr-var-arr-delim1-arr-delim2-num)
14. [RegExMatch](#14-regexmatcharr-haystack-arr-needle-startpos)
15. [RegExReplace](#15-regexreplacearr-input-arr-pattern-arr-replacement)

---

## 1. `StrLen(arr string)`
Returns the length (number of elements) of an array/string.
* **Parameters**: 
  * `string` (arr): The array to measure.
* **Returns**: Integer (Length of the array).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Normal String
arr s1
arradd s1 hello
int len1 := 0
StrLen(s1)
len1 := rax
print(len1) ; Prints 5

; Example 2: String with spaces
arr s2
arradd s2 hello world
int len2 := 0
StrLen(s2)
len2 := rax
print(len2) ; Prints 11

; Example 3: Empty Array
arr s3
int len3 := 0
StrLen(s3)
len3 := rax
print(len3) ; Prints 0
```

## 2. `Chr(num)`
Converts an ASCII integer code into an array containing that single character.
* **Parameters**:
  * `num` (int): The ASCII decimal code (e.g., 65 for 'A').
* **Returns**: Array containing the character.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Capital Letter
arr c1
c1 := Chr(65) ; 'A'
c1.index 0
print_rax_as_char
print("")

; Example 2: Lowercase Letter
arr c2
c2 := Chr(122) ; 'z'
c2.index 0
print_rax_as_char
print("")

; Example 3: Number Symbol
arr c3
c3 := Chr(48) ; '0'
c3.index 0
print_rax_as_char
print("")
```

## 3. `StringTrimRight(arr string, n)`
Removes `n` elements from the end (right side) of the array.
* **Parameters**:
  * `string` (arr): The array to modify.
  * `n` (int): Number of elements to remove.
* **Returns**: A new truncated Array.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Trim 2 chars
arr str1
arradd str1 hello
arr res1
res1 := StringTrimRight(str1, 2)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "hel"

; Example 2: Trim all chars
arr str2
arradd str2 hello
arr res2
res2 := StringTrimRight(str2, 5)
res2.size
print(rax) ; Prints 0 (Empty array)
print("")

; Example 3: Trim 0 chars
arr str3
arradd str3 hello
arr res3
res3 := StringTrimRight(str3, 0)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "hello"
```

## 4. `StringTrimLeft(arr string, n)`
Removes `n` elements from the beginning (left side) of the array.
* **Parameters**:
  * `string` (arr): The array to modify.
  * `n` (int): Number of elements to remove.
* **Returns**: A new truncated Array.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Trim 2 chars
arr str1
arradd str1 hello
arr res1
res1 := StringTrimLeft(str1, 2)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "llo"

; Example 2: Trim all chars
arr str2
arradd str2 hello
arr res2
res2 := StringTrimLeft(str2, 5)
res2.size
print(rax) ; Prints 0 (Empty array)
print("")

; Example 3: Trim 0 chars
arr str3
arradd str3 hello
arr res3
res3 := StringTrimLeft(str3, 0)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "hello"
```

## 5. `countChars(arr s, c)`
Counts how many times a specific byte/character appears in the array.
* **Parameters**:
  * `s` (arr): The array to search.
  * `c` (int): The ASCII value of the character to count.
* **Returns**: Integer (Total occurrences).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Count 'l' (108)
arr s1
arradd s1 hello
int c1 := 0
countChars(s1, 108)
c1 := rax
print(c1) ; Prints 2

; Example 2: Count 'z' (122)
arr s2
arradd s2 hello
int c2 := 0
countChars(s2, 122)
c2 := rax
print(c2) ; Prints 0

; Example 3: Count spaces (32)
arr s3
arradd s3 a b c
int c3 := 0
countChars(s3, 32)
c3 := rax
print(c3) ; Prints 2
```

## 6. `SubStr(arr s, start, len)`
Extracts a chunk of an array starting at `start`.
* **Parameters**:
  * `s` (arr): The source array.
  * `start` (int): The 0-based starting index.
  * `len` (int): The number of characters to extract. If `0`, extracts everything to the end of the string.
* **Returns**: Array (The extracted substring).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Extract middle
arr s1
arradd s1 hello
arr res1
res1 := SubStr(s1, 1, 3)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "ell"

; Example 2: Extract to the end
arr s2
arradd s2 hello
arr res2
res2 := SubStr(s2, 2, 0)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "llo"

; Example 3: Extract first char
arr s3
arradd s3 hello
arr res3
res3 := SubStr(s3, 0, 1)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "h"
```

## 7. `Trim(arr s)`
Removes leading and trailing whitespace (spaces, tabs, newlines, carriage returns) from an array.
* **Parameters**:
  * `s` (arr): The string/array to clean.
* **Returns**: Array (The cleaned string).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Standard spaces
arr s1
s1.add 32
arradd s1 hello
s1.add 32
arr res1
res1 := Trim(s1)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "hello"

; Example 2: Tabs and Newlines
arr s2
s2.add 9
arradd s2 world
s2.add 10
arr res2
res2 := Trim(s2)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "world"

; Example 3: No spaces to trim
arr s3
arradd s3 nospaces
arr res3
res3 := Trim(s3)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "nospaces"
```

## 8. `InStr(arr haystack, arr needle, startPos)`
Searches for a sequence of characters (`needle`) inside another (`haystack`).
* **Parameters**:
  * `haystack` (arr): The string to search inside.
  * `needle` (arr): The string to look for.
  * `startPos` (int): The 0-based index to begin the search.
* **Returns**: Integer. The 0-based index where the needle begins. If not found, returns `haystack.size`.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Find word
arr hay1
arradd hay1 hello world
arr ndl1
arradd ndl1 world
int pos1 := 0
InStr(hay1, ndl1, 0)
pos1 := rax
print(pos1) ; Prints 6

; Example 2: Find starting later
arr hay2
arradd hay2 foo bar foo
arr ndl2
arradd ndl2 foo
int pos2 := 0
InStr(hay2, ndl2, 1)
pos2 := rax
print(pos2) ; Prints 8 (skips the first "foo")

; Example 3: Not Found
arr hay3
arradd hay3 hello
arr ndl3
arradd ndl3 zzz
int pos3 := 0
InStr(hay3, ndl3, 0)
pos3 := rax
print(pos3) ; Prints 5 (Length of "hello")
```

## 9. `StrLower(arr s)`
Converts all uppercase ASCII characters (A-Z) in the array to lowercase (a-z).
* **Parameters**:
  * `s` (arr): The string to convert.
* **Returns**: Array (The lowercase string).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: All Caps
arr s1
arradd s1 HELLO
arr res1
res1 := StrLower(s1)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "hello"

; Example 2: Mixed Case
arr s2
arradd s2 HeLlO WoRlD
arr res2
res2 := StrLower(s2)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "hello world"

; Example 3: Numbers and Symbols
arr s3
arradd s3 TEST 123!
arr res3
res3 := StrLower(s3)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "test 123!"
```

## 10. `INT_To_STR(num)`
Converts an integer value into its string/character array representation.
* **Parameters**:
  * `num` (int): The number to convert (e.g., 12345).
* **Returns**: Array (e.g., `['1', '2', '3', '4', '5']`).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Normal Number
int n1 := 12345
arr res1
res1 := INT_To_STR(n1)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "12345"

; Example 2: Zero
int n2 := 0
arr res2
res2 := INT_To_STR(n2)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "0"

; Example 3: Single Digit
int n3 := 7
arr res3
res3 := INT_To_STR(n3)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "7"
```

## 11. `StrReplace(arr haystack, arr needle, arr replacement)`
Finds all instances of `needle` in `haystack` and replaces them with `replacement`.
* **Parameters**:
  * `haystack` (arr): The original string.
  * `needle` (arr): The string to find.
  * `replacement` (arr): The string to insert in place of the needle.
* **Returns**: Array (The newly formatted string).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Replace Word
arr h1
arradd h1 foo bar foo
arr n1
arradd n1 foo
arr r1
arradd r1 baz
arr res1
res1 := StrReplace(h1, n1, r1)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "baz bar baz"

; Example 2: Replace Char
arr h2
arradd h2 hello
arr n2
arradd n2 l
arr r2
arradd r2 X
arr res2
res2 := StrReplace(h2, n2, r2)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "heXXo"

; Example 3: Remove Char (Empty Replacement)
arr h3
arradd h3 test
arr n3
arradd n3 t
arr r3 ; Left empty
arr res3
res3 := StrReplace(h3, n3, r3)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "es"
```

## 12. `StrSplit(arr inputStr, arr delimiter, num)`
Splits a string by a specific delimiter sequence and returns the *Nth* token.
* **Parameters**:
  * `inputStr` (arr): The string to split.
  * `delimiter` (arr): The sequence separating the tokens (e.g., `--`).
  * `num` (int): The 1-based token number to retrieve (e.g., `2` gets the second item).
* **Returns**: Array (The requested token). Returns empty if the token doesn't exist.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Get 2nd Token
arr data1
arradd data1 a--b--c
arr delim1
arradd delim1 --
arr res1
res1 := StrSplit(data1, delim1, 2)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "b"

; Example 2: Get Last Token
arr data2
arradd data2 a--b--c
arr delim2
arradd delim2 --
arr res2
res2 := StrSplit(data2, delim2, 3)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "c"

; Example 3: Out of Bounds (Invalid Token)
arr data3
arradd data3 a--b--c
arr delim3
arradd delim3 --
arr res3
res3 := StrSplit(data3, delim3, 5)
res3.size
print(rax) ; Prints 0 (Empty array)
```

## 13. `LoopParseFunc(arr var, arr delim1, arr delim2, num)`
Advanced splitting. Treats every character in `delim1` and `delim2` as split points. Consecutive delimiters are treated as a single boundary.
* **Parameters**:
  * `var` (arr): The string to parse.
  * `delim1` (arr): First set of delimiter characters.
  * `delim2` (arr): Second set of delimiter characters.
  * `num` (int): The 1-based token to retrieve.
* **Returns**: Array (The requested token).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Cloud Delimiters
arr d_in
arradd d_in first,,second...third
arr d1
arradd d1 ,
arr d2
arradd d2 .
arr res1
res1 := LoopParseFunc(d_in, d1, d2, 2)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "second"

; Example 2: Single Delimiter Array
arr d_in2
arradd d_in2 a,b,c
arr d_comma
arradd d_comma ,
arr d_empty
arr res2
res2 := LoopParseFunc(d_in2, d_comma, d_empty, 3)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "c"

; Example 3: No Delimiters (Gets Nth character)
arr d_in3
arradd d_in3 hello
arr res3
res3 := LoopParseFunc(d_in3, d_empty, d_empty, 2)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "e"
```

## 14. `RegExMatch(arr haystack, arr needle, startPos)`
Searches a string using lightweight Regular Expressions.
* **Supported Patterns**:
  * `\d` - Matches any digit (0-9).
  * `\b` - Word boundary (transitions between alphanumeric and non-alphanumeric).
* **Parameters**:
  * `haystack` (arr): The string to search.
  * `needle` (arr): The pattern to find.
  * `startPos` (int): The 0-based index to begin the search.
* **Returns**: Integer. The 0-based index of the match. Returns `haystack.size` if not found.

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Find Digit (\d)
arr h1
arradd h1 Agent 007
arr pat1
pat1.add 92 ; \
pat1.add 100 ; d
int pos1 := 0
RegExMatch(h1, pat1, 0)
pos1 := rax
print(pos1) ; Prints 6

; Example 2: Find Word Boundary (\b)
arr h2
arradd h2 Hi!
arr pat2
pat2.add 92 ; \
pat2.add 98 ; b
pat2.add 33 ; !
int pos2 := 0
RegExMatch(h2, pat2, 0)
pos2 := rax
print(pos2) ; Prints 2

; Example 3: Not Found
arr h3
arradd h3 NoDigitsHere
arr pat3
pat3.add 92 ; \
pat3.add 100 ; d
int pos3 := 0
RegExMatch(h3, pat3, 0)
pos3 := rax
print(pos3) ; Prints 12 (length of string)
```

## 15. `RegExReplace(arr input, arr pattern, arr replacement)`
Finds all occurrences of a RegEx pattern and replaces them.
* **Parameters**:
  * `input` (arr): The original string.
  * `pattern` (arr): The RegEx pattern to find (supports `\d`, `\b`).
  * `replacement` (arr): The string to insert in place of the matched pattern.
* **Returns**: Array (The newly formatted string).

**Examples:**
```ahk
include "HTLL_Lib.htll"
main

; Example 1: Replace Digits
arr h1
arradd h1 Agent 007
arr pat1
pat1.add 92 ; \
pat1.add 100 ; d
arr rep1
arradd rep1 X
arr res1
res1 := RegExReplace(h1, pat1, rep1)
int res1_len := 0
res1.size
res1_len := rax
Loop, res1_len {
    res1.index A_Index
    print_rax_as_char
}
print("") ; Prints "Agent XXX"

; Example 2: Replace to Empty
arr h2
arradd h2 A1B2C3
arr pat2
pat2.add 92 ; \
pat2.add 100 ; d
arr rep2 ; Empty array
arr res2
res2 := RegExReplace(h2, pat2, rep2)
int res2_len := 0
res2.size
res2_len := rax
Loop, res2_len {
    res2.index A_Index
    print_rax_as_char
}
print("") ; Prints "ABC"

; Example 3: Complex literal + RegEx Replace
arr h3
arradd h3 item1, item2
arr pat3
arradd pat3 m
pat3.add 92 ; \
pat3.add 100 ; d
arr rep3
arradd rep3 mX
arr res3
res3 := RegExReplace(h3, pat3, rep3)
int res3_len := 0
res3.size
res3_len := rax
Loop, res3_len {
    res3.index A_Index
    print_rax_as_char
}
print("") ; Prints "itemX, itemX"
```

***

## 📝 Important HTLL Architecture Notes

To write stable code with HTLL and this library, you must keep the following compiler rules in mind:

1. **0-Indexed Arrays**: All strings and arrays in HTLL begin at index `0`. The first character of `"hello"` is at index `0`, and the last is at index `4`.
2. **No Negative Numbers**: HTLL does not support negative numbers. Because of this, functions like `InStr` and `RegExMatch` cannot return `-1` to indicate a failed search. Instead, **they return the total length of the haystack array** (which is an out-of-bounds index).
3. **RAX Volatility**: The `rax` register is volatile. If a function or method returns an integer (like `StrLen`, `InStr`, or `arr.size`), you must immediately copy `rax` into an `int` variable (`pos := rax`) before executing any other commands. Do not use `rax` directly inside `.add` or mathematical operations.
4. **Parameter Purity**: The HTLL Standard Library never modifies your input variables. Every array passed into a function is duplicated locally via `.copy` before manipulation. Your source data is always safe. 
5. **RegEx Escape Characters**: When building pattern arrays for `RegExMatch` and `RegExReplace`, you cannot easily type the backslash `\` or special keywords via `arradd`. You must push the raw ASCII values into the array: `arr.add 92` (for `\`), `arr.add 100` (for `d`), and `arr.add 98` (for `b`).

***