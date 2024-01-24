// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IconHandler/Public/IconHandlerSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeIconHandlerSettings() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
	ICONHANDLER_API UClass* Z_Construct_UClass_UIconHandlerSettings();
	ICONHANDLER_API UClass* Z_Construct_UClass_UIconHandlerSettings_NoRegister();
	ICONHANDLER_API UScriptStruct* Z_Construct_UScriptStruct_FClassIconEntry();
	UPackage* Z_Construct_UPackage__Script_IconHandler();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_ClassIconEntry;
class UScriptStruct* FClassIconEntry::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_ClassIconEntry.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_ClassIconEntry.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FClassIconEntry, (UObject*)Z_Construct_UPackage__Script_IconHandler(), TEXT("ClassIconEntry"));
	}
	return Z_Registration_Info_UScriptStruct_ClassIconEntry.OuterSingleton;
}
template<> ICONHANDLER_API UScriptStruct* StaticStruct<FClassIconEntry>()
{
	return FClassIconEntry::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FClassIconEntry_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClassThumbnail_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ClassThumbnail;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClassIcon_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ClassIcon;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FClassIconEntry_Statics::Struct_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Entry used to configure the icons for a single class. */" },
#endif
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Entry used to configure the icons for a single class." },
#endif
	};
#endif
	void* Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FClassIconEntry>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassThumbnail_MetaData[] = {
		{ "AllowedClasses", "/Script/Engine.Texture2D" },
		{ "Category", "ClassIconEntry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Icon used to display in the windows like the content browser. (64x64)*/" },
#endif
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Icon used to display in the windows like the content browser. (64x64)" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassThumbnail = { "ClassThumbnail", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FClassIconEntry, ClassThumbnail), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassThumbnail_MetaData), Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassThumbnail_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassIcon_MetaData[] = {
		{ "AllowedClasses", "/Script/Engine.Texture2D" },
		{ "Category", "ClassIconEntry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Icon used to display in tabs and/or anything small. (16x16) */" },
#endif
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Icon used to display in tabs and/or anything small. (16x16)" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassIcon = { "ClassIcon", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FClassIconEntry, ClassIcon), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassIcon_MetaData), Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassIcon_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FClassIconEntry_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassThumbnail,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewProp_ClassIcon,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FClassIconEntry_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_IconHandler,
		nullptr,
		&NewStructOps,
		"ClassIconEntry",
		Z_Construct_UScriptStruct_FClassIconEntry_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FClassIconEntry_Statics::PropPointers),
		sizeof(FClassIconEntry),
		alignof(FClassIconEntry),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FClassIconEntry_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FClassIconEntry_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FClassIconEntry_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FClassIconEntry()
	{
		if (!Z_Registration_Info_UScriptStruct_ClassIconEntry.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_ClassIconEntry.InnerSingleton, Z_Construct_UScriptStruct_FClassIconEntry_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_ClassIconEntry.InnerSingleton;
	}
	void UIconHandlerSettings::StaticRegisterNativesUIconHandlerSettings()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UIconHandlerSettings);
	UClass* Z_Construct_UClass_UIconHandlerSettings_NoRegister()
	{
		return UIconHandlerSettings::StaticClass();
	}
	struct Z_Construct_UClass_UIconHandlerSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Enable_MetaData[];
#endif
		static void NewProp_Enable_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_Enable;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Icons_ValueProp;
		static const UECodeGen_Private::FClassPropertyParams NewProp_Icons_Key_KeyProp;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Icons_MetaData[];
#endif
		static const UECodeGen_Private::FMapPropertyParams NewProp_Icons;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UIconHandlerSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_IconHandler,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UIconHandlerSettings_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Settings used to configure the Icon handler plugin itself. */" },
#endif
		{ "IncludePath", "IconHandlerSettings.h" },
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Settings used to configure the Icon handler plugin itself." },
#endif
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable_MetaData[] = {
		{ "Category", "IconHandler" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Master toggle wether to enable the icon changes or not. */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Master toggle wether to enable the icon changes or not." },
#endif
	};
#endif
	void Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable_SetBit(void* Obj)
	{
		((UIconHandlerSettings*)Obj)->Enable = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable = { "Enable", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UIconHandlerSettings), &Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable_MetaData), Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_ValueProp = { "Icons", nullptr, (EPropertyFlags)0x0000000000004001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FClassIconEntry, METADATA_PARAMS(0, nullptr) }; // 2624714862
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_Key_KeyProp = { "Icons_Key", nullptr, (EPropertyFlags)0x0000000000004001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_MetaData[] = {
		{ "AllowAbstract", "" },
		{ "Category", "IconHandler" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09 * Texture for this tutorial, used when presented to the user in the tutorial browser.\n\x09 * @note Will only work with C++ created classes.\n\x09 */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "ModuleRelativePath", "Public/IconHandlerSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Texture for this tutorial, used when presented to the user in the tutorial browser.\n@note Will only work with C++ created classes." },
#endif
	};
#endif
	const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons = { "Icons", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UIconHandlerSettings, Icons), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_MetaData), Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_MetaData) }; // 2624714862
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UIconHandlerSettings_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Enable,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_ValueProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons_Key_KeyProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UIconHandlerSettings_Statics::NewProp_Icons,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UIconHandlerSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UIconHandlerSettings>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UIconHandlerSettings_Statics::ClassParams = {
		&UIconHandlerSettings::StaticClass,
		"Editor",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UIconHandlerSettings_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::PropPointers),
		0,
		0x001000A6u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UIconHandlerSettings_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UIconHandlerSettings_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UIconHandlerSettings()
	{
		if (!Z_Registration_Info_UClass_UIconHandlerSettings.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UIconHandlerSettings.OuterSingleton, Z_Construct_UClass_UIconHandlerSettings_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UIconHandlerSettings.OuterSingleton;
	}
	template<> ICONHANDLER_API UClass* StaticClass<UIconHandlerSettings>()
	{
		return UIconHandlerSettings::StaticClass();
	}
	UIconHandlerSettings::UIconHandlerSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UIconHandlerSettings);
	UIconHandlerSettings::~UIconHandlerSettings() {}
	struct Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ScriptStructInfo[] = {
		{ FClassIconEntry::StaticStruct, Z_Construct_UScriptStruct_FClassIconEntry_Statics::NewStructOps, TEXT("ClassIconEntry"), &Z_Registration_Info_UScriptStruct_ClassIconEntry, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FClassIconEntry), 2624714862U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UIconHandlerSettings, UIconHandlerSettings::StaticClass, TEXT("UIconHandlerSettings"), &Z_Registration_Info_UClass_UIconHandlerSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UIconHandlerSettings), 2382305785U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_251071354(TEXT("/Script/IconHandler"),
		Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_20_Projects_Unreal_ProjectBE_Plugins_ThirdParty_IconHandler_Source_IconHandler_Public_IconHandlerSettings_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
