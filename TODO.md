# TODO

### HTLL Upgrade Plan

1. **Better array loops** – `Loop, arr.size {` automatically handles indexing inside.  
2. **Infinite loops** – `Loop {` runs forever.  
3. **`include`** – Include other HTLL source files.  
4. **Inline assembly / Programming blocks** – Embed target‑specific assembly or target language code.  
5. **Array parameters** – Functions can take arrays (passed by copy).  
6. **Return types** – `func int name() { return 5 }`.  
7. **Local scope** – Variables inside functions are truly local.  
8. **Bitwise operators** – `&`, `|`, `^`, `~`, `<<`, `>>`.  
9. **Basic expressions** – `z := x + y - 5` left to right.  

---

## For build-ins in HTLL

### List (Sorted by Difficulty)

1.  `StrLen`
2.  `Chr`
3.  `StringTrimRight`
4.  `countChars`
5.  `StringTrimLeft`
6.  `StrLower`
7.  `SubStr`
8.  `Trim`
9.  `InStr`
10. `INT_To_STR`
11. `StrSplit`
12. `RegExMatch`
13. `StrReplace`
14. `LoopParseFunc`
15. `RegExReplace`

---

### Implementation Signatures

```ahk
func StrLen(str)
funcend

func Chr(val)
funcend

func StringTrimRight(str, count)
funcend

func countChars(str, needle)
funcend

func StringTrimLeft(str, count)
funcend

func StrLower(str)
funcend

func SubStr(str, start_pos, length)
funcend

func Trim(str)
funcend

func InStr(haystack, needle)
funcend

func INT_To_STR(val)
funcend

func StrSplit(input, delim, num)
funcend

func RegExMatch(str, pattern)
funcend

func StrReplace(orig, find, replace)
funcend

func LoopParseFunc(var, delim1, delim2)
funcend

func RegExReplace(input, pattern, replace)
funcend
```