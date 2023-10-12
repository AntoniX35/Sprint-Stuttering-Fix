/* 
 * https://github.com/Starfield-Reverse-Engineering/CommonLibSF
 * This plugin template links against CommonLibSF
 */

#include "DKUtil/Hook.hpp"

namespace SprintStutteringFix
{
	void Install()
	{
		REL::Relocation<std::uintptr_t> SprintLimit{ REL::ID(498736) };
		
		float limit = 2000.0f;

		REL::safe_write(SprintLimit.address(), &limit, sizeof(float));

		INFO("Installed");
	}
}

namespace
{
	void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
	{
		switch (a_msg->type) {
		case SFSE::MessagingInterface::kPostLoad:
			{
				SprintStutteringFix::Install();
			}
			break;
		default:
			break;
		}
	}
}

/**
// for preload plugins
void SFSEPlugin_Preload(SFSE::LoadInterface* a_sfse);
/**/

DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", Plugin::NAME.data(), NULL);
#endif

	SFSE::Init(a_sfse, false);
	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);
	
	// do stuff
	// this allocates 1024 bytes for development builds, you can
	// adjust the value accordingly with the log result for release builds
	//SFSE::AllocTrampoline(1 << 7);

	SFSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}
