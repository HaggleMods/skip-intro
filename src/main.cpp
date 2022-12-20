#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>
#include <MinHook.h>

HMODULE self{};

void init()
{
	MH_Initialize();

	callbacks::on(callbacks::type::main_loop, []()
	{
		static bool once = false;
		if (!once)
		{
			static int timer = 100;
			if (timer > 0)
			{
				--timer;
			}
			else if (timer <= 0)
			{
				Sexy::ThunderballApp::DoToMenu();
				once = true;
			}
		}
	});

	MH_EnableHook(MH_ALL_HOOKS);
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		self = hModule;
		DisableThreadLibraryCalls(self);
		CreateThread(nullptr, 0, OnAttach, self, 0, nullptr);
		return true;
	}

	return false;
}
