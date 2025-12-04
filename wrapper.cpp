#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

// --- All Function Declarations from your original HTLL.cpp ---
std::string StrReplace(const std::string&, const std::string&, const std::string&);
std::string Trim(const std::string&);
std::string SubStr(const std::string&, int, int);
int InStr(const std::string&, const std::string&);
std::string FileRead(const std::string&);
bool FileAppend(const std::string&, const std::string&);
bool FileDelete(const std::string&);
size_t StrLen(const std::string&);
std::string SubStrLastChars(std::string, int);
std::string compiler(std::string);

// Helper function
char* create_c_string_from_std_string(const std::string& s) {
    char* c_str = (char*)malloc(s.length() + 1);
    if (c_str) { strcpy(c_str, s.c_str()); }
    return c_str;
}

// extern "C" block
extern "C" {
    char* StrReplace_c(const char* o, const char* f, const char* r) { return create_c_string_from_std_string(StrReplace(o, f, r)); }
    char* Trim_c(const char* i) { return create_c_string_from_std_string(Trim(i)); }
    char* SubStr_c(const char* i, int s, int l) { return create_c_string_from_std_string(SubStr(i, s, l)); }
    char* FileRead_c(const char* p) { return create_c_string_from_std_string(FileRead(p)); }
    char* SubStrLastChars_c(const char* t, int n) { return create_c_string_from_std_string(SubStrLastChars(t, n)); }
    char* compiler_c(const char* c) { return create_c_string_from_std_string(compiler(c)); }
    int InStr_c(const char* h, const char* n) { return InStr(h, n); }
    bool FileAppend_c(const char* p, const char* c) { return FileAppend(c, p); }
    bool FileDelete_c(const char* p) { return FileDelete(p); }
    size_t StrLen_c(const char* s) { return StrLen(s); }
    void free_string_c(void* p) { if (p) { free(p); } }
}
