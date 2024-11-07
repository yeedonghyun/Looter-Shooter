// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "LooterShooterProjectile.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UPrimitiveComponent;
struct FHitResult;
#ifdef LOOTERSHOOTER_LooterShooterProjectile_generated_h
#error "LooterShooterProjectile.generated.h already included, missing '#pragma once' in LooterShooterProjectile.h"
#endif
#define LOOTERSHOOTER_LooterShooterProjectile_generated_h

#define FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnHit);


#define FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesALooterShooterProjectile(); \
	friend struct Z_Construct_UClass_ALooterShooterProjectile_Statics; \
public: \
	DECLARE_CLASS(ALooterShooterProjectile, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/LooterShooter"), NO_API) \
	DECLARE_SERIALIZER(ALooterShooterProjectile) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ALooterShooterProjectile(ALooterShooterProjectile&&); \
	ALooterShooterProjectile(const ALooterShooterProjectile&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ALooterShooterProjectile); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ALooterShooterProjectile); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ALooterShooterProjectile) \
	NO_API virtual ~ALooterShooterProjectile();


#define FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_12_PROLOG
#define FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_INCLASS_NO_PURE_DECLS \
	FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> LOOTERSHOOTER_API UClass* StaticClass<class ALooterShooterProjectile>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Looter_Shooter_LooterShooter_Source_LooterShooter_LooterShooterProjectile_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
