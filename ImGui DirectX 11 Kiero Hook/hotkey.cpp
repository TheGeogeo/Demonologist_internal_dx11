#include "includes.h"

namespace hotkey
{
	void AsyncHotkey()
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) CFG.b_MenuShow = !CFG.b_MenuShow;
	}
}