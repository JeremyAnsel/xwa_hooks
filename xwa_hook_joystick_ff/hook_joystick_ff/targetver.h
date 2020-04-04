#pragma once

#include <winsdkver.h>

#undef NTDDI_VERSION
#undef _WIN32_WINNT
#undef WINVER

#define NTDDI_VERSION		NTDDI_WINXPSP2
#define _WIN32_WINNT		_WIN32_WINNT_WINXP
#define WINVER				_WIN32_WINNT

#define DIRECTINPUT_VERSION  0x0500
