#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <crtdbg.h>
#include <deque>
#include <CommCtrl.h>

#include "CallerPtr.h"
#include "Callable.h"

#include "ProjUtil.h"

#include "Control.h"

#include "resource.h"

#define MAINAPP (PlaylistExtractor::GetSingleton())
