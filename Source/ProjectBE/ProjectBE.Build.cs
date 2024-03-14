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
                "GACore",           // 音量管理 のため
                "GFCore",           // 基本アクター のため
                "GCItem",           // アイテム実装 のため
                "GSCore",           // ゲーム設定 のため
                "GCSave",           // ロードアウト保存のため
                "GCLoading",        // ロード画面作成のため
                "GCOnline",         // オンラインサービス利用のため

                // Game XXX Extension
                "GAExt",            // アビリティ実装 のため
                "GCExt",            // キャラクターセットアップ のため
                "GFXExt",           // ContextEffectInterface のため
                "GEEquip",          // 装備関係の機能実装 のため
                "GEExt",            // ゲームモードの作成 のため
                "GEInput",          // 入力関係の設定や処理ついか のため  
                "GLExt",            // LocomotionCharacter 及び LocomotionComponent のため
                "GTExt",            // TeamMemberComponentInterface のため
                "GUIExt",           // UI実装 のため
                "GVExt",            // FOV設定付きの視点の追加 のため
                "GEPhase",          // ゲームモードの進行制御 のため
               
                // Game XXX: YYY Addon
                "GAHAddon",         // ヘルス管理 のため
                "GLHAddon",         // 人型キャラクターのアニメーション制御のため
                "GAUIDialog",       // チュートリアルダイアログ用
                "GAUISetting",      // 設定メニュー作成のため
                
                // Game XXX: YYY Integration
                "GAUIIntg",         // アビリティ関係のUI作成 のため
                "GIEquipAbility",   // 装備関係のアビリティ追加 のため
                "GHFXIntg",         // 人型キャラクター関係のエフェクト表現 のため
                "GIAIntg",          // アビリティの入力処理 のため
                "GLAIntg",          // 移動関係のアビリティ機能利用 のため
                "GIUIEquip",        // 装備関係のUI実装 のため

                // Project
                "BEDataBase",
            }
        );

        SetupIrisSupport(Target);
    }
}
