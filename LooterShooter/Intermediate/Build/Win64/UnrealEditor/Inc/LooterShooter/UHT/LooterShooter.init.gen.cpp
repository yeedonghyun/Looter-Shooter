// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLooterShooter_init() {}
	LOOTERSHOOTER_API UFunction* Z_Construct_UDelegateFunction_LooterShooter_OnPickUp__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_LooterShooter;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_LooterShooter()
	{
		if (!Z_Registration_Info_UPackage__Script_LooterShooter.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_LooterShooter_OnPickUp__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/LooterShooter",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0xF3D972C8,
				0xF3B0F759,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_LooterShooter.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_LooterShooter.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_LooterShooter(Z_Construct_UPackage__Script_LooterShooter, TEXT("/Script/LooterShooter"), Z_Registration_Info_UPackage__Script_LooterShooter, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xF3D972C8, 0xF3B0F759));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
