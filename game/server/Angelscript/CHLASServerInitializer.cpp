#include "extdll.h"
#include "util.h"

#include "Angelscript/CHLASManager.h"

#include "Angelscript/ScriptAPI/ASGameRules.h"

#include "Angelscript/ScriptAPI/Extensions/CASGameRules.h"

#include "CHLASServerInitializer.h"

bool CHLASServerInitializer::RegisterCoreAPI( CASManager& manager )
{
	CHLASBaseInitializer::RegisterCoreAPI( manager );

	auto& engine = *manager.GetEngine();

	engine.RegisterInterface( "IGameRules" );

	RegisterScriptCGameRules( engine );
	RegisterScriptGameRules( engine );

	return true;
}