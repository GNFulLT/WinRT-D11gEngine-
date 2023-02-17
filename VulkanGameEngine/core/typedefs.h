#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <cmath>

// Argument to str
// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing

#ifndef _STR
#define _STR(str) #str
#define _STR_XDEF(str) _STR(str)
#endif // _STR
// Force compile to make inline the function, variable etc.

#ifndef _F_INLINE_
#if defined(__GNUC__)
#define _F_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _F_INLINE_ __forceinline
#else
#define _F_INLINE_ inline
#endif // __GNUC__
#endif // _F_INLINE_

// Use this instead of inline
#ifndef _INLINE_
#ifdef _DEBUG
#define _INLINE_ inline
#else
#define _INLINE_ _F_INLINE
#endif // _DEBUG
#endif // _INLINe_

// Inform the compiler that the return value of the function is important and shouldn't be ignored
#ifndef _IMP_RETURN_
#define _IMP_RETURN_ [[nodiscard]]
#endif
// Undef Win32 awkward defines
#ifdef _WIN32
#undef min // override standard definition and cause bugs some libraries like taskflow
#undef max // override standard definition and cause bugs some libraries like taskflow
#undef ERROR 
#undef DELETE 
#undef MessageBox
#undef Error
#undef OK
#undef CONNECT_DEFERRED
#endif // _WIN32

// Swap two value. Don't forget it calls default constructor 
#ifndef SWAP
#define SWAP(m_x, m_y) __swap_tmpl((m_x), (m_y))

template <class T>
_INLINE_ void __swap_tmpl(T& x, T& y) {
	T temp = x;
	x = y;
	y = temp;
}

// Pointer definition of swap
template <class T>
_INLINE_ void __swap_tmpl(T* x, T* y) {
	T* temp = x;
	x = y;
	y = temp;
}
#endif // SWAP

#ifndef _GARR_SIZE
#if defined(_MSC_VER) && !defined(USE_GARR_SIZE)
#define _GARR_SIZE(pArr) ARRAYSIZE(pArr)
#elif defined(USE_GARR_SIZE) && defined(USE_GARR_SIZE_TEMPLATED)
template<typename T, size_t N>
size_t arrlen(T(&)[N]){return N;}
#define _GARR_SIZE(pArr) (arrlen(pArr)) 
#else
#define _GARR_SIZE(pArr)  ((sizeof(pArr) / sizeof(*(pArr))) / static_cast<size_t>(!(sizeof(pArr) % sizeof(*(pArr)))))
#endif // _MSC_VER && USE_GARR_SIZE
#endif // _GARR_SIZE

#ifdef USE_DEBUG_ALLOCATION
#ifndef DEBUG_NEW
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif //_WIN32
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#ifdef USE_DEBUG_ALLOCATION_IMPLICIT
#define new DEBUG_NEW
#endif // USE_DEBUG_ALLOCATION_IMPLICIT
#endif // DEBUG_NEW
#endif // USE_DEBUG_ALLOCATION



#endif // TYPEDEFS_H