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
#include "FGraphPanelFactorys.h"
#include "ChaRtEditor/PathGraph/PathGraphAssetFactory.h"


static const FName GraphChaRtEditorTabName("GraphChaRtEditor");

#define LOCTEXT_NAMESPACE "FGraphChaRtEditorModule"

void FGraphChaRtEditorModule::StartupModule()
{
	FGraphChaRtEditorStyle::Initialize();
	FGraphChaRtEditorStyle::ReloadTextures();

	//注册节点工厂
	PathEdGraphNodeFactory = MakeShareable( new FGraphChaRtNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(PathEdGraphNodeFactory);

	//注册命令
	FGraphChaRtEditorCommands::Register();
	FPathGraphEditorCommands::Register();

	//绑定Plugins层级命令
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGraphChaRtEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGraphChaRtEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGraphChaRtEditorModule::RegisterMenus));

	//注册Plugins按钮
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

	//释放节点工厂
	FEdGraphUtilities::UnregisterVisualNodeFactory(PathEdGraphNodeFactory);
	PathEdGraphNodeFactory.Reset();
	
	//释放命令
	FGraphChaRtEditorCommands::Unregister();
	FPathGraphEditorCommands::Unregister();

	//释放Plugins按钮面板
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