// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/PathGraphAssetFactory.h"



void FPathGraphAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                 TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (UObject* Obj : InObjects)
	{
		if (UPathGraph* Asset = Cast<UPathGraph>(Obj))
		{
			if (GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(Asset, /*bFocusIfOpen=*/true))
			{
				continue;
			}
			// 创建 Toolkit 实例
			TSharedRef<FPathGraphAssetEditorToolkit> EditorToolkit =
				MakeShareable(new FPathGraphAssetEditorToolkit());

			// 初始化编辑器
			EditorToolkit->InitPathGraphAssetEditor(Mode, EditWithinLevelEditor, Asset);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void FPathGraphAssetEditorToolkit::InitPathGraphAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UPathGraph* InAsset)
{
	EditedPathGraph = InAsset;
	EditedObjects = { EditedPathGraph };

	const TSharedRef<FTabManager::FLayout> StandaloneLayout = FTabManager::NewLayout("Standalone_PathGraphEditorLayout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Horizontal)
		->Split(
			FTabManager::NewStack()
			->AddTab(PathGraphMainTabID, ETabState::OpenedTab)
			->SetHideTabWell(true)
		)
	);

	
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		FName("PathGraphEditorApp"),
		StandaloneLayout,
		true,
		true,
		InAsset,
		true,
		false,
		TOptional<EAssetOpenMethod>()
		);
}

void FPathGraphAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(PathGraphMainTabID, FOnSpawnTab::CreateSP(this, &FPathGraphAssetEditorToolkit::SpawnPathGraphMainTab))
	.SetDisplayName(NSLOCTEXT("PathGraphEditor", "MainTab", "Path Graph"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

TSharedRef<SDockTab> FPathGraphAssetEditorToolkit::SpawnPathGraphMainTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Path Graph Editor")))
		]
		+ SVerticalBox::Slot()
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					return FText::FromString(FString::Printf(TEXT("节点数: %d"), EditedPathGraph->GetPathNodes().Num()));
				})
			]
		]
		
	];
}



////////////////////////////////////////////////////////////////////////////////////////

UPathGraphAssetFactory::UPathGraphAssetFactory()
{
	SupportedClass = UPathGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UPathGraphAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UPathGraph>(InParent, InClass, InName, Flags, Context);
}
