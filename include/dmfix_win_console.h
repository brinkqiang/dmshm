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

#ifndef __DMFIX_WIN_CONSOLE_H_INCLUDE__
#define __DMFIX_WIN_CONSOLE_H_INCLUDE__

#ifdef _WIN32
#include <windows.h>
// For C mode (when __cplusplus is not defined) on Windows, include stdbool.h for 'bool'
#if !defined(__cplusplus)
#include <stdbool.h>
#endif // !__cplusplus
#endif // _WIN32

static inline bool dm_win_console_internal_do_setup(void) {
#ifdef _WIN32
  bool any_console_handle_successfully_configured = false;
  bool at_least_one_console_handle_targeted = false;
  HANDLE handles_to_check[2] = { GetStdHandle(STD_OUTPUT_HANDLE), GetStdHandle(STD_ERROR_HANDLE) };

  for (int i = 0; i < 2; ++i) {
    HANDLE hCurrent = handles_to_check[i];
    if (hCurrent != INVALID_HANDLE_VALUE && GetFileType(hCurrent) == FILE_TYPE_CHAR) {
      at_least_one_console_handle_targeted = true;
      DWORD dwOriginalMode = 0;
      if (GetConsoleMode(hCurrent, &dwOriginalMode)) {
        if (!(dwOriginalMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
          if (!SetConsoleMode(hCurrent, dwOriginalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
            // According to MSDN, SetConsoleMode can return 0 on success if dwOriginalMode was 0.
            // However, a more robust check might involve GetLastError() if needed.
            // For this common use case, if it fails to set the new mode, we return false.
            return false;
          }
        }
        any_console_handle_successfully_configured = true;
      }
      else {
        // Failed to get console mode
        return false;
      }
    }
  }

  // If no console handles (like stdout/stderr redirected to file),
  // this setup might not be applicable or considered successful by some definitions.
  // The original logic returns false if no char handles targeted, which is preserved.
  if (!at_least_one_console_handle_targeted) {
    return false;
  }

  return any_console_handle_successfully_configured;
#else // Not _WIN32
  return true; // On non-Windows platforms, assume success or not applicable
#endif // _WIN32
}


#ifdef __cplusplus

namespace DmInternal {
  struct DmWinConsoleInitializer {
    DmWinConsoleInitializer() {
      // In C++, the static object's constructor calls the setup function.
      (void)dm_win_console_internal_do_setup();
    }
  };

  // This static inline variable (C++17 feature for headers) ensures its constructor
  // is called once for the program, triggering the console setup.
  // If pre-C++17, 'static' alone in a header would mean one per translation unit.
  // 'inline' helps ensure ODR (One Definition Rule) compliance.
  static inline DmWinConsoleInitializer g_dm_win_console_auto_initializer_instance;

} // namespace DmInternal

// Public C-linkage function for manual initialization if needed from C++
extern "C" inline bool dm_win_console_init() {
  return dm_win_console_internal_do_setup();
}

#else // Pure C section below

// Public function for manual initialization in C
static inline bool dm_win_console_init(void) {
  return dm_win_console_internal_do_setup();
}

// Automatic C initialization (pre-main execution using compiler extensions)
// This section is active only for C compilation paths on Windows.
#ifdef _WIN32

#if defined(__GNUC__) || defined(__clang__)
// For GCC and Clang (including MinGW-w64 on Windows)
// The __attribute__((constructor)) causes this static function to be called before main().
__attribute__((constructor))
static void _dmfix_win_console_c_auto_init_constructor(void) {
  (void)dm_win_console_internal_do_setup(); // Cast to void indicates return value is intentionally ignored
}
#elif defined(_MSC_VER)
// For Microsoft Visual C++ (MSVC)
// <windows.h> is already included under the _WIN32 guard.

// Define the function pointer type for C initializers if not available through CRT headers.
// _PIFV is a pointer to a function returning int and taking no arguments.
typedef int (*_PIFV)(void);

// This static function will be called by the C Runtime (CRT) before main.
// It must match the _PIFV signature (return int, take void).
static int _dmfix_win_console_msvc_c_auto_init_crt(void) {
  (void)dm_win_console_internal_do_setup(); // Cast to void
  return 0; // CRT initializers typically return 0 on success.
}

// This pragma directs the linker to place a pointer to our initializer function
// into a special section of the executable (.CRT$XCU). The CRT startup code
// iterates through function pointers in this section and calls them before main.
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) _PIFV _g_dmfix_win_console_c_initializer_ptr = _dmfix_win_console_msvc_c_auto_init_crt;

#endif // Compiler-specific auto-init for Windows C (__GNUC__/__clang__ or _MSC_VER)

#endif // _WIN32 (for C automatic initialization logic)

#endif // __cplusplus (end of C or C++ specific sections)

#endif // __DMFIX_WIN_CONSOLE_H_INCLUDE__