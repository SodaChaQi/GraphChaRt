// Copyright Epic Games, Inc. All Rights Reserved.

#include "GraphChaRtEditor.h"

#include "AssetToolsModule.h"
#include "GraphChaRtEditorStyle.h"
#include "EditorCommands/GraphChaRtEditorCommands.h"
#include "EditorCommands/PathGraphEditorCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "GraphEditor/PathGraphAssetFactory.h"



static const FName GraphChaRtEditorTabName("GraphChaRtEditor");

#define LOCTEXT_NAMESPACE "FGraphChaRtEditorModule"

void FGraphChaRtEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGraphChaRtEditorStyle::Initialize();
	FGraphChaRtEditorStyle::ReloadTextures();

	FGraphChaRtEditorCommands::Register();
	FPathGraphEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGraphChaRtEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGraphChaRtEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGraphChaRtEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GraphChaRtEditorTabName, FOnSpawnTab::CreateRaw(this, &FGraphChaRtEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGraphChaRtEditorTabTitle", "GraphChaRtEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// 注册资产类型到 AssetTools
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	UPathGraphAssetFactory* Factory = NewObject<UPathGraphAssetFactory>();
	Factory->AddToRoot();
	
	TSharedRef<IAssetTypeActions> Action = MakeShareable(new FPathGraphAssetTypeActions());
	AssetTools.RegisterAssetTypeActions(Action);

	RegisteredAssetTypeActions.Add(Action);
}

void FGraphChaRtEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGraphChaRtEditorStyle::Shutdown();

	FGraphChaRtEditorCommands::Unregister();
	FPathGraphEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GraphChaRtEditorTabName);

	//取消注册资产
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (auto& Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
	RegisteredAssetTypeActions.Empty();
}

TSharedRef<SDockTab> FGraphChaRtEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FGraphChaRtEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("GraphChaRtEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FGraphChaRtEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GraphChaRtEditorTabName);
}

void FGraphChaRtEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGraphChaRtEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGraphChaRtEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGraphChaRtEditorModule, GraphChaRtEditor)