// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaRtEditor/PathGraph/PathGraphAssetFactory.h"

#include "EditorCommands/PathGraphEditorCommands.h"
#include "ChaRtEditor/PathGraph/Slate/SPathGraphCheckResult.h"
#include "ChaRtEditor/PathGraph/Slate/SPathGraphEditor.h"

const FName FPathGraphAssetEditorToolkit::GraphCanvasTabID = FName("PathGraphEditor_GraphCanvas");
const FName FPathGraphAssetEditorToolkit::DetailsTabID("PathGraphEditor_Details");
const FName FPathGraphAssetEditorToolkit::GraphViewTabID("PathGraphEditor_GraphView");
const FName FPathGraphAssetEditorToolkit::CheckResultTabID("PathGraphEditor_CheckResult");


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

#define LOCTEXT_NAMESPACE "PathGraphAssetEditorToolkit"

void FPathGraphAssetEditorToolkit::InitPathGraphAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UPathGraph* InAsset)
{
	EditedPathGraph = InAsset;

	EditedObjects = { EditedPathGraph };

	ToolbarExtender = MakeShared<FExtender>();
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateRaw(this, &FPathGraphAssetEditorToolkit::FillToolbarExtents)
		);
	AddToolbarExtender(ToolbarExtender);

	const TSharedRef<FTabManager::FLayout> PathGraphEditorLayout = FTabManager::NewLayout("PathGraphEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(GraphViewTabID, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.4f)
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab(GraphCanvasTabID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(CheckResultTabID, ETabState::OpenedTab)
				)

			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(DetailsTabID, ETabState::OpenedTab)
			)
		)
	);

	
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		FName("PathGraphEditorApp"),
		PathGraphEditorLayout,
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

	InTabManager->RegisterTabSpawner(GraphCanvasTabID, FOnSpawnTab::CreateSP(this, &FPathGraphAssetEditorToolkit::SpawnTab_GraphCanvas))
	.SetDisplayName(NSLOCTEXT("PathGraphEditor", "GraphCanvasTab", "Graph Canvas"));

	InTabManager->RegisterTabSpawner(GraphViewTabID, FOnSpawnTab::CreateSP(this, &FPathGraphAssetEditorToolkit::SpawnTab_GraphView))
	.SetDisplayName(NSLOCTEXT("PathGraphEditor", "GraphViewTab", "Graph View"));
	
	InTabManager->RegisterTabSpawner(DetailsTabID, FOnSpawnTab::CreateSP(this, &FPathGraphAssetEditorToolkit::SpawnTab_Details))
	.SetDisplayName(NSLOCTEXT("PathGraphEditor", "GraphDetailsTab", "Details"));

	InTabManager->RegisterTabSpawner(CheckResultTabID, FOnSpawnTab::CreateSP(this, &FPathGraphAssetEditorToolkit::SpawnTab_CheckResult))
	.SetDisplayName(NSLOCTEXT("PathGraphEditor", "CheckResultTab", "CheckResult"));

	
	// .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FPathGraphAssetEditorToolkit::BindCommands()
{
	const FPathGraphEditorCommands& Commands = FPathGraphEditorCommands::Get();
    
	ToolkitCommands->MapAction(
		Commands.SaveGraph,
		FExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::SaveGraph),
		FCanExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::CanSaveGraph)
	);
    
	ToolkitCommands->MapAction(
		Commands.AddNode,
		FExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::AddNode),
		FCanExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::CanAddNode)
	);
    
	ToolkitCommands->MapAction(
		Commands.DeleteSelectedNodes,
		FExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FPathGraphAssetEditorToolkit::CanDeleteNodes)
	);
}

void FPathGraphAssetEditorToolkit::FillToolbarExtents(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection(TEXT("Graph Editor Toolbar"));
	{
		ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateRaw(this, &FPathGraphAssetEditorToolkit::Check),
				FCanExecuteAction::CreateRaw(this, &FPathGraphAssetEditorToolkit::CanCheck)),
				NAME_None,
				LOCTEXT("MyButtonLabel", "Check"),
				LOCTEXT("MyButtonTooltip", "Check All Node and Edge"),
				FSlateIcon(FGraphChaRtEditorStyle::GetStyleSetName(), "GraphChaRtEditor.OpenPluginWindow")
				);
	}
	ToolbarBuilder.EndSection();
}

void FPathGraphAssetEditorToolkit::SaveGraph()
{
}

bool FPathGraphAssetEditorToolkit::CanSaveGraph()
{
	return true;
}

void FPathGraphAssetEditorToolkit::AddNode()
{
}

bool FPathGraphAssetEditorToolkit::CanAddNode()
{
	return true;
}

void FPathGraphAssetEditorToolkit::DeleteSelectedNodes()
{
}

bool FPathGraphAssetEditorToolkit::CanDeleteNodes()
{
	return true;
}

void FPathGraphAssetEditorToolkit::Check()
{
}

bool FPathGraphAssetEditorToolkit::CanCheck()
{
	return true;
}

TSharedRef<SDockTab> FPathGraphAssetEditorToolkit::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
{
	check(IsValid(EditedPathGraph));
	
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
		SNew(SPathGraphEditor)
		.EditedPathGraph(EditedPathGraph)
				
		];
}

TSharedRef<SDockTab> FPathGraphAssetEditorToolkit::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab);

}

TSharedRef<SDockTab> FPathGraphAssetEditorToolkit::SpawnTab_GraphView(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab);
}

TSharedRef<SDockTab> FPathGraphAssetEditorToolkit::SpawnTab_CheckResult(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SPathGraphCheckResult)
		];
}


#undef LOCTEXT_NAMESPACE

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
