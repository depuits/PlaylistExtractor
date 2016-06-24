#pragma once

// Defines for UNICODE

#include <string>		
#include <sstream>
#include <iostream>
#include <fstream>
#include <tchar.h>
using namespace std;

#ifdef _UNICODE	// extra unicode defines
	#define tstring wstring
	#define tcin wcin
	#define tcout wcout
	#define tstringstream wstringstream
	#define tofstream wofstream
	#define tifstream wifstream
	#define tfstream wfstream
#else
	#define tstring string
	#define tcin cin
	#define tcout cout
	#define tstringstream stringstream
	#define tofstream ofstream
	#define tifstream ifstream
	#define tfstream fstream
#endif

// ASSERT macro

#ifndef NDEBUG
#define ASSERT \
if ( false ) {} \
else \
struct LocalAssert { \
	int mLine; \
	LocalAssert(int line=__LINE__) : mLine(line) {} \
	LocalAssert(bool isOK, const TCHAR* message=_T("")) { \
		if ( !isOK ) { \
			tstringstream buffer; \
			buffer << _T("ERROR!! Assert failed on line ") << LocalAssert().mLine << _T(" in file '") << __FILE__ << _T("'\nBoodschap: \"") << message << _T("\"\n"); \
			OutputDebugString(buffer.str().c_str()); \
			__asm { int 3 } \
		} \
	} \
} myAsserter = LocalAssert
#else
#define ASSERT \
if ( true ) {} else \
struct NoAssert { \
	NoAssert(bool isOK, const TCHAR* message=_T("")) {} \
} myAsserter = NoAssert
#endif

//engine
#include "Engine.h"

#define ENGINE (Engine::GetSingleton())


