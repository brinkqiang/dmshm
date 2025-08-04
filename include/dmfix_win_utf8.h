// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// cmake env
// add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#ifndef __DMFIX_WIN_UTF8_H_INCLUDE__
#define __DMFIX_WIN_UTF8_H_INCLUDE__

#ifdef _WIN32
#include <windows.h>
#else // Not _WIN32
#include <locale.h>
#endif // _WIN32

#if !defined(__cplusplus) // For C mode, include stdbool.h for 'bool'
#include <stdbool.h>
#endif // !__cplusplus

#define DMFIX_INTERNAL_CODE_PAGE_UTF8 65001

static inline bool dm_win_utf8_internal_do_setup(void) {
#ifdef _WIN32
    BOOL output_cp_set_ok = SetConsoleOutputCP(DMFIX_INTERNAL_CODE_PAGE_UTF8);
    BOOL input_cp_set_ok = SetConsoleCP(DMFIX_INTERNAL_CODE_PAGE_UTF8);
    return (output_cp_set_ok != 0 && input_cp_set_ok != 0);
#else // Not _WIN32
    /*
     * Attempts to set the program's locale to UTF-8.
     * Common locale strings for UTF-8 include "en_US.utf8", "C.UTF-8".
     * The specific string "en_US.utf8" might not be available on all systems.
     * setlocale(LC_ALL, "") would use the system's environment-defined locale.
     */
    if (setlocale(LC_ALL, "en_US.utf8") != NULL) {
        return true;
    }
    // Fallback attempt for other common UTF-8 locale string
    // else if (setlocale(LC_ALL, "C.UTF-8") != NULL) {
    // return true;
    // }
    // One could also try just "UTF-8" for some systems.
    // else if (setlocale(LC_ALL, "UTF-8") != NULL) {
    // return true;
    // }
    return false;
#endif // _WIN32
}


#ifdef __cplusplus

namespace DmInternal {
    struct DmWinUtf8Initializer {
        DmWinUtf8Initializer() {
            (void)dm_win_utf8_internal_do_setup();
        }
    };
    static inline DmWinUtf8Initializer g_dm_win_utf8_auto_initializer_instance;
} // namespace DmInternal

// Example of an explicit C-linkage function if needed
// extern "C" bool dm_win_utf8_init_explicitly() {
// return dm_win_utf8_internal_do_setup();
// }

#else // Pure C section below

// Public function for manual initialization in C
static inline bool dm_win_utf8_init(void) {
    return dm_win_utf8_internal_do_setup();
}

// Automatic C initialization (pre-main execution using compiler extensions)
// These static functions will call dm_win_utf8_internal_do_setup(), which handles
// OS-specific logic internally via its own #ifdef _WIN32.
#if defined(__GNUC__) || defined(__clang__)
// For GCC and Clang
__attribute__((constructor))
static void _dmfix_utf8_c_auto_init_constructor(void) {
    (void)dm_win_utf8_internal_do_setup(); // Cast to void indicates return value is intentionally ignored
}
#elif defined(_MSC_VER)
// For Microsoft Visual C++ (MSVC)
// This CRT initializer mechanism is specific to MSVC, typically for Windows.
// <windows.h> would have been included if _WIN32 is defined.

typedef int (*_PIFV)(void); // Pointer to Initializer Function (returning int, taking void)

static int _dmfix_utf8_msvc_c_auto_init_crt(void) {
    (void)dm_win_utf8_internal_do_setup(); // Cast to void
    return 0; // CRT initializers should return 0 on success.
}

#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) _PIFV _g_dmfix_utf8_c_initializer_ptr = _dmfix_utf8_msvc_c_auto_init_crt;

#endif // Compiler-specific auto-init (__GNUC__/__clang__ or _MSC_VER)

#endif // __cplusplus (end of C or C++ specific sections)

#endif // __DMFIX_WIN_UTF8_H_INCLUDE__