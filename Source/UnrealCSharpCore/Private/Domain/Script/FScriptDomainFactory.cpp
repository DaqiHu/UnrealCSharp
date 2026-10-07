#include "Domain/Script/FScriptDomainFactory.h"
#include "Misc/CommandLine.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#if WITH_MONO
#include "Domain/Mono/FMonoDomain.h"
#endif
#if WITH_CORECLR
#include "Domain/CoreCLR/FCoreCLRDomain.h"
#endif
#if WITH_LEANCLR
#include "Domain/LeanCLR/FLeanCLRDomain.h"
#endif

namespace
{
IScriptDomain* RecordBenchDomain(IScriptDomain* Domain, const TCHAR* Backend)
{
	FString OutputPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("BenchRuntimeTelemetry="), OutputPath))
	{
		// Record the branch that actually constructed the domain, outside timed loops.
		FFileHelper::SaveStringToFile(FString::Printf(
			TEXT("{\"backend\":\"%s\",\"source\":\"FScriptDomainFactory::Create constructed domain\"}"), Backend),
			*OutputPath);
	}
	return Domain;
}
}

EScriptDomainType FScriptDomainFactory::GetScriptDomainType()
{
	if (const auto UnrealCSharpSetting = GetDefault<UUnrealCSharpSetting>())
	{
		return UnrealCSharpSetting->GetScriptDomainType(FPlatformProperties::IniPlatformName());
	}

	return EScriptDomainType::Mono;
}

IScriptDomain* FScriptDomainFactory::Create()
{
	if (const auto ScriptDomainType = GetScriptDomainType();
		ScriptDomainType == EScriptDomainType::Mono)
	{
#if WITH_MONO
		return RecordBenchDomain(new FMonoDomain(), TEXT("Mono"));
#endif
	}
	else if (ScriptDomainType == EScriptDomainType::CoreCLR)
	{
#if WITH_CORECLR
		return RecordBenchDomain(new FCoreCLRDomain(), TEXT("CoreCLR"));
#endif
	}
	else if (ScriptDomainType == EScriptDomainType::LeanCLR)
	{
#if WITH_LEANCLR
		return RecordBenchDomain(new FLeanCLRDomain(), TEXT("LeanCLR"));
#endif
	}

	return nullptr;
}

void FScriptDomainFactory::Destroy(IScriptDomain* InScriptDomain)
{
	if (InScriptDomain != nullptr)
	{
		if (InScriptDomain->IsInitialized())
		{
			InScriptDomain->Deinitialize();
		}

		delete InScriptDomain;

		IScriptDomain::Set(nullptr);
	}
}
