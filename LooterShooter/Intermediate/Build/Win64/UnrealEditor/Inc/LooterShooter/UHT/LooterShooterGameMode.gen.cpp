// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "LooterShooter/LooterShooterGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLooterShooterGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
LOOTERSHOOTER_API UClass* Z_Construct_UClass_ALooterShooterGameMode();
LOOTERSHOOTER_API UClass* Z_Construct_UClass_ALooterShooterGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_LooterShooter();
// End Cross Module References

// Begin Class ALooterShooterGameMode
void ALooterShooterGameMode::StaticRegisterNativesALooterShooterGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ALooterShooterGameMode);
UClass* Z_Construct_UClass_ALooterShooterGameMode_NoRegister()
{
	return ALooterShooterGameMode::StaticClass();
}
struct Z_Construct_UClass_ALooterShooterGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "LooterShooterGameMode.h" },
		{ "ModuleRelativePath", "LooterShooterGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALooterShooterGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ALooterShooterGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_LooterShooter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALooterShooterGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ALooterShooterGameMode_Statics::ClassParams = {
	&ALooterShooterGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ALooterShooterGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ALooterShooterGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ALooterShooterGameMode()
{
	if (!Z_Registration_Info_UClass_ALooterShooterGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ALooterShooterGameMode.OuterSingleton, Z_Construct_UClass_ALooterShooterGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ALooterShooterGameMode.OuterSingleton;
}
template<> LOOTERSHOOTER_API UClass* StaticClass<ALooterShooterGameMode>()
{
	return ALooterShooterGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ALooterShooterGameMode);
ALooterShooterGameMode::~ALooterShooterGameMode() {}
// End Class ALooterShooterGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ALooterShooterGameMode, ALooterShooterGameMode::StaticClass, TEXT("ALooterShooterGameMode"), &Z_Registration_Info_UClass_ALooterShooterGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ALooterShooterGameMode), 3633086196U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterGameMode_h_544460472(TEXT("/Script/LooterShooter"),
	Z_CompiledInDeferFile_FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
