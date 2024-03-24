// Copyright (C) 2024 owoDra

#include "BESettingUITypeResolver_Language.h"

#include "GameSetting/GameplaySettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include "Internationalization/Culture.h"
#include "Internationalization/TextLocalizationManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUITypeResolver_Language)


UBESettingUITypeResolver_Switch_Language::UBESettingUITypeResolver_Switch_Language(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


// Initialization

void UBESettingUITypeResolver_Switch_Language::OnInitialized()
{
	OptionValues.Reset();
	OptionDisplayTexts.Reset();

	// 言語の設定値候補と表示名候補をキャッシュ

	const auto AllCultureNames{ FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game) };
	for (const auto& CultureName : AllCultureNames)
	{
		if (FInternationalization::Get().IsCultureAllowed(CultureName))
		{
			OptionValues.Add(CultureName);

			auto Culture{ FInternationalization::Get().GetCulture(CultureName) };
			if (ensureMsgf(Culture, TEXT("Unable to find Culture '%s'!"), *CultureName))
			{
				const auto CultureDisplayName{ Culture->GetDisplayName() };
				const auto CultureNativeName{ Culture->GetNativeName() };

				// 言語名と表示名が違う場合のみ両方表示する

				auto Entry
				{
					!CultureNativeName.Equals(CultureDisplayName, ESearchCase::CaseSensitive) ? 
						FString::Printf(TEXT("%s (%s)"), *CultureNativeName, *CultureDisplayName) : CultureNativeName
				};

				OptionDisplayTexts.Add(FText::FromString(Entry));
			}
		}
	}

	// 言語のデフォルト設定を挿入

	OptionValues.Insert(TEXT(""), UBESettingUITypeResolver_Switch_Language::SettingSystemDefaultLanguageIndex);

	const auto SystemDefaultCulture{ FInternationalization::Get().GetDefaultCulture().ToSharedPtr() };
	if (ensure(SystemDefaultCulture))
	{
		const auto& DefaultCultureDisplayName{ SystemDefaultCulture->GetDisplayName() };
		auto LocalizedSystemDefault{ FText::Format(NSLOCTEXT("SettingUI", "SystemDefaultLanguage", "System Default ({0})"), FText::FromString(DefaultCultureDisplayName)) };

		OptionDisplayTexts.Insert(MoveTemp(LocalizedSystemDefault), UBESettingUITypeResolver_Switch_Language::SettingSystemDefaultLanguageIndex);
	}

	Super::OnInitialized();
}


// Setting

void UBESettingUITypeResolver_Switch_Language::SetSettingValue(int32 NewValue)
{
	if (ensure(OptionValues.IsValidIndex(NewValue)))
	{
		if (auto* Settings{ UGSCGameUserSettings::GetSettingSubsystem<UGameplaySettingSubsystem>() })
		{
			// デフォルト言語の場合はリセット

			if (NewValue == UBESettingUITypeResolver_Switch_Language::SettingSystemDefaultLanguageIndex)
			{
				Settings->ResetToDefaultCulture();
			}

			// デフォルト言語でない場合は PendingCulture を設定

			else
			{
				Settings->SetPendingCulture(OptionValues[NewValue]);
			}
		}
	}
}

int32 UBESettingUITypeResolver_Switch_Language::GetSettingValue() const
{
	if (auto* Settings{ UGSCGameUserSettings::GetSettingSubsystem<UGameplaySettingSubsystem>() })
	{
		if (Settings->ShouldResetToDefaultCulture())
		{
			return UBESettingUITypeResolver_Switch_Language::SettingSystemDefaultLanguageIndex;
		}

		// 現在の設定値を取得

		auto PendingCulture{ Settings->GetPendingCulture() };
		if (PendingCulture.IsEmpty())
		{
			if (Settings->IsUsingDefaultCulture())
			{
				return SettingSystemDefaultLanguageIndex;
			}

			PendingCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
		}

		// キャッシュから一致する設定値のインデクスを返す

		const auto ExactMatchIndex{ OptionValues.IndexOfByKey(PendingCulture) };
		if (ExactMatchIndex != INDEX_NONE)
		{
			return ExactMatchIndex;
		}

		// キャッシュから見つからなかった場合は PrioritizedCulture からインデクスを返す

		const auto PrioritizedPendingCultures{ FInternationalization::Get().GetPrioritizedCultureNames(PendingCulture) };
		for (auto i{ 0 }; i < OptionValues.Num(); ++i)
		{
			if (PrioritizedPendingCultures.Contains(OptionValues[i]))
			{
				return i;
			}
		}
	}

	return 0;
}


TArray<FText> UBESettingUITypeResolver_Switch_Language::GetOptionDisplayTexts() const
{
	return OptionDisplayTexts;
}
