#pragma once

#ifndef NOEXPORT

#ifdef CPPACELIB_EXPORTS
#define DLLEXPORT _declspec(dllexport)
#else
#define DLLEXPORT _declspec(dllimport)
#endif

#else
#define DLLEXPORT
#endif // !NOEXPORT
