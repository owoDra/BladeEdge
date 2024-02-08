// Copyright (C) 2024 owoDra

#include "BESettingUICustomRegister_Keybind.h"

#include "GameSetting/Resolver/BESettingUITypeResolver_Keybind.h"
#include "GameSetting/Keybind/BEUserKeybindSettings.h"
#include "GameSetting/Keybind/BEKeybindSettings.h"

#include "SettingUISubsystem.h"

#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUICustomRegister_Keybind)


UBESettingUICustomRegister_Keybind::UBESettingUICustomRegister_Keybind(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FRegisteredSettingUITable UBESettingUICustomRegister_Keybind::CreateTable(USettingUISubsystem* Subsystem) const
{
	check(Subsystem);

	// 新しいテーブルを用意

	FRegisteredSettingUITable NewTable;
	
	const auto* LocalPlayer{ Subsystem->GetLocalPlayer() };
	ensure(LocalPlayer);
	const auto* EISubsystem{ ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer) };
	ensure(EISubsystem);
	auto* UserSettings{ EISubsystem ? EISubsystem->GetUserSettings() : nullptr };
	ensure(UserSettings);
	const auto* CurrentProfile{ UserSettings ? UserSettings->GetCurrentKeyProfile() : nullptr };

	if (ensure(CurrentProfile) && ensure(ResolverClass) && ensure(HeaderClass))
	{
		// 項目登録時にカテゴリーごとのマッピングデータのソートに使用するマッピングリストを用意

		struct FMappings
		{
			FText Category;
			TArray<FKeyMappingRow> Rows;
		};

		TMap<FName, FMappings> CategoryMappingsMap;

		auto AddOrCreateForCategory
		{
			[&CategoryMappingsMap](const FText& InCategory, const FKeyMappingRow& InMapping)
			{
				const auto CategoryName{ FName(InCategory.ToString()) };
				auto& Entry{ CategoryMappingsMap.FindOrAdd(CategoryName) };

				Entry.Category = InCategory;
				Entry.Rows.Emplace(InMapping);
			}
		};

		// 現在のプロファイルに登録されているマッピングデータをイテレート

		for (const auto& KVP : CurrentProfile->GetPlayerMappingRows())
		{
			const auto& MappingName{ KVP.Key };
			const auto& MappingRow{ KVP.Value };

			if (MappingRow.HasAnyMappings())
			{
				const auto& Category{ MappingRow.Mappings.begin()->GetDisplayCategory() };
				const auto& DefaultKey{ MappingRow.Mappings.begin()->GetDefaultKey() };

				if ((DefaultKey.IsGamepadKey() && bShowGamepad) || (!DefaultKey.IsGamepadKey() && bShowKeyboardAndMouse))
				{
					AddOrCreateForCategory(Category, MappingRow);
				}

				///
				/// @TODO:
				///		現在 FPlayerKeyMapping からマッピングごとに設定した PlayerMappableKeySettings を取得できないので
				///		現在はデフォルトのキータイプから判定する
				/// 

				/*const auto& Setting{ Cast<UBEKeybindSettings>(MappingRow.Mappings.begin()->GetAssociatedInputAction()->GetPlayerMappableKeySettings()) };
				if (ensure(Setting))
				{
					const auto bCanExposeGamepad{ Setting->CanExposeGamepad() };
					const auto bCanExposeKeyboardAndMouse{ Setting->CanExposeKeyboardAndMouse() };
					
					if ((bShowGamepad && bCanExposeGamepad) || (bShowKeyboardAndMouse && bCanExposeKeyboardAndMouse))
					{
						AddOrCreateForCategory(Category, MappingRow);
					}
				}*/
			}
		}

		// キャッシュしたデータをもとに設定項目を作成

		for (const auto& SortedKVP : CategoryMappingsMap)
		{
			const auto& CategoryDevName{ SortedKVP.Key };
			const auto& CategoryDisplayName{ SortedKVP.Value.Category };
			const auto& Mappings{ SortedKVP.Value.Rows };

			// ヘッダーを作成

			auto* NewHeader{ NewObject<USettingUITypeResolver>(Subsystem, HeaderClass) };

			FSettingUIOption HeaderOption;
			HeaderOption.Name = CategoryDisplayName;
			HeaderOption.Category = CategoryDisplayName;

			NewHeader->InitializeResolver(Subsystem, CategoryDevName, HeaderOption);

			NewTable.Row.Add(CategoryDevName, NewHeader);

			// 項目を作成

			for (const auto& Mapping : Mappings)
			{
				const auto& MappingName{ Mapping.Mappings.begin()->GetMappingName() };

				auto* NewOption{ NewObject<UBESettingUITypeResolver_Keybind>(Subsystem, ResolverClass) };

				FSettingUIOption OptionOption;
				OptionOption.Name = Mapping.Mappings.begin()->GetDisplayName();
				OptionOption.Description = Mapping.Mappings.begin()->GetAssociatedInputAction()->ActionDescription;
				//OptionOption.Description = CastChecked<UBEKeybindSettings>(Mapping.Mappings.begin()->GetAssociatedInputAction()->GetPlayerMappableKeySettings())->GetTooltipText();
				OptionOption.Category = CategoryDisplayName;

				NewOption->InitializeResolver(Subsystem, MappingName, OptionOption);
				NewOption->SetInputSetting(UserSettings);

				NewTable.Row.Add(MappingName, NewOption);
			}
		}
	}

	return NewTable;
}
