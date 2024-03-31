// Copyright (C) 2024 owoDra

using UnrealBuildTool;

public class ProjectBE : ModuleRules
{
	public ProjectBE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                ModuleDirectory,
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "CoreUObject", "Engine",

                "Slate", "SlateCore", "RenderCore", "UMG",

                "AIModule", "ApplicationCore", "PhysicsCore", "NetCore",

                "InputCore", "EnhancedInput",

                "AudioMixer", "AudioModulation",

                "CommonUI", "CommonInput",

                "GameplayTags", "GameplayAbilities", "ModularGameplay", 
                
                "GameFeatures", "GameplayTasks",

                "DeveloperSettings", "PropertyPath",

                "OnlineServicesInterface", "CoreOnline",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // Game XXX Core
                "GACore",           // Game Audio Core      音量管理 のため
                "GFCore",           // Game Framework       Core 基本アクター のため
                "GCItem",           // Game Item Core       アイテム実装 のため
                "GSCore",           // Game Setting Core    ゲーム設定 のため
                "GCSave",           // Game Save Core       ロードアウト保存のため
                "GCLoading",        // Game Loading Core    ロード画面作成のため
                "GCOnline",         // Game Online Core     オンラインサービス利用のため

                // Game XXX Extension
                "GAExt",            // Game Ability Extension       アビリティ実装 のため
                "GCExt",            // Game Character Extension     キャラクターセットアップ のため
                "GFXExt",           // Game Effect Extension        ContextEffectInterface のため
                "GEEquip",          // Game Equipment Extension     装備関係の機能実装 のため
                "GEExt",            // Game Experience Extension    ゲームモードの作成 のため
                "GEInput",          // Game Input Extension         入力関係の設定や処理ついか のため  
                "GLExt",            // Game Locomotion Extension    LocomotionCharacter 及び LocomotionComponent のため
                "GTExt",            // Game Team Extension          TeamMemberComponentInterface のため
                "GUIExt",           // Game UI Extension            UI実装 のため
                "GVExt",            // Game View Extension          FOV設定付きの視点の追加 のため
                "GEPhase",          // Game Phase Extension         ゲームモードの進行制御 のため
               
                // Game XXX: YYY Addon
                "GAHAddon",         // Game Ability: Health Addon       ヘルス管理 のため
                "GLHAddon",         // Game Locomotion: Human Addon     人型キャラクターのアニメーション制御のため
                "GAUIDialog",       // Game UI: Dialog Addon            チュートリアルダイアログ用
                "GAUISetting",      // Game UI: Setting Addon           設定メニュー作成のため
                
                // Game XXX: YYY Integration
                "GAUIIntg",         // Game Ability: UI Integration             アビリティ関係のUI作成 のため
                "GIEquipAbility",   // Game Equipment: Ability Integration      装備関係のアビリティ追加 のため
                "GHFXIntg",         // Game Human: Effect Integration           人型キャラクター関係のエフェクト表現 のため
                "GIAIntg",          // Game Input: Ability Integration          アビリティの入力処理 のため
                "GLAIntg",          // Game Locomotion: Ability Integration     移動関係のアビリティ機能利用 のため
                "GIUIEquip",        // Game UI: Equipment Integration           装備関係のUI実装 のため

                // Project 専用
                "BEDataBase",       // ゲームデータ・パラメータの管理のため
            }
        );

        SetupIrisSupport(Target);
    }
}
