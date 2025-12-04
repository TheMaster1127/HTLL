#include <string>   // Required for std::string
#include <cstring>  // Required for memcpy
#include <new>      // Required for std::nothrow

// ============================================================================
// FORWARD DECLARATION
// ============================================================================
// This tells the compiler that a function named "compiler" exists somewhere
// else in your project. This is necessary for the wrapper to work.
std::string compiler(std::string source_code);


// ============================================================================
// HELPER FUNCTION
// ============================================================================
// This is the new, robust helper that safely creates a C-style string
// from a C++ std::string using the correct C++ memory operators.
char* create_c_string_from_std_string(const std::string& s) {
    // Use the C++ array allocation operator 'new[]'.
    // std::nothrow ensures it returns nullptr on failure instead of crashing.
    char* c_str = new (std::nothrow) char[s.length() + 1];

    // If allocation was successful, copy the data.
    if (c_str) {
        // memcpy is a fast and safe way to copy the raw bytes.
        memcpy(c_str, s.c_str(), s.length());
        
        // Manually add the final null terminator. This is critical.
        c_str[s.length()] = '\0';
    }
    
    return c_str;
}


// ============================================================================
// C-STYLE WRAPPER INTERFACE
// ============================================================================
// The 'extern "C"' block prevents C++ name mangling, allowing your
// assembly code to find these functions by their simple names.
extern "C" {

    /**
     * @brief   Takes a C-string, passes it to the C++ compiler function,
     *          and returns the result as a new C-string.
     * @param c The input source code as a null-terminated C-string.
     * @return  A pointer to a new C-string with the compiled code.
     *          The caller is responsible for freeing this with free_string_c.
     */
    char* compiler_c(const char* c) {
        // Make sure the input pointer is not null before using it.
        if (!c) {
            return nullptr;
        }
        return create_c_string_from_std_string(compiler(c));
    }


    /**
     * @brief   Safely deallocates a C-string that was created by this library.
     * @param p A pointer to the memory that should be freed.
     */
    void free_string_c(void* p) {
        if (p) {
            // Use 'delete[]' because the memory was allocated with 'new[]'.
            // This is the C++ equivalent of free() and solves the crash.
            delete[] static_cast<char*>(p);
        }
    }

} // end extern "C"