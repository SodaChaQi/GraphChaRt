// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/PathGraphAssetFactory.h"

#include "EditorCommands/PathGraphEditorCommands.h"
#include "GraphEditor/SPathGraphEditor.h"

const FName FPathGraphAssetEditorToolkit::GraphCanvasTabID = FName("PathGraphEditor_GraphCanvas");
const FName FPathGraphAssetEditorToolkit::DetailsTabID("PathGraphEditor_Details");
const FName FPathGraphAssetEditorToolkit::GraphViewTabID("PathGraphEditor_GraphView");


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
					->SetSizeCoefficient(0.7f)
					->AddTab(GraphCanvasTabID, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.3f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->AddTab(DetailsTabID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->AddTab(GraphViewTabID, ETabState::OpenedTab)

				)

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
