// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "GraphChaRtEditorStyle.h"
#include "Factories/Factory.h"
#include "Graph/Graphs.h"
#include "PathGraphAssetFactory.generated.h"

/**
 * 
 */

class FPathGraphAssetTypeActions : public FAssetTypeActions_Base
{
public:

	FORCEINLINE virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "FGraphAsset", "Path Graph"); }
	FORCEINLINE virtual FColor GetTypeColor() const override { return FColor::Purple; }
	FORCEINLINE virtual UClass* GetSupportedClass() const override { return UPathGraph::StaticClass(); }
	FORCEINLINE virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	FORCEINLINE virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override { return FGraphChaRtEditorStyle::Get().GetBrush("ClassIcon.PathGraph"); }
	FORCEINLINE virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override { return FGraphChaRtEditorStyle::Get().GetBrush("ClassIcon.PathGraph"); }

	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()
		) override;
};

class FPathGraphAssetEditorToolkit : public FAssetEditorToolkit
{
public:

	

	void InitPathGraphAssetEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UPathGraph* InAsset
		);

	virtual FName GetToolkitFName() const override { return FName("PathNodeAssetEditor"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("PathNodeAssetEditor", "AppLabel", "Path Node Asset Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("MyCustom"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }
	virtual const TArray<UObject*>* GetObjectsCurrentlyBeingEdited() const override { return &EditedObjects; }
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:

	void BindCommands();
	void FillToolbarExtents(FToolBarBuilder& ToolbarBuilder);

	void SaveGraph();
	bool CanSaveGraph();
	void AddNode();
	bool CanAddNode();
	void DeleteSelectedNodes();
	bool CanDeleteNodes();
	void Check();
	bool CanCheck();

private:

	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_GraphView(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_CheckResult(const FSpawnTabArgs& Args);

	TSharedPtr<FExtender> ToolbarExtender;
	
	
	UPathGraph* EditedPathGraph = nullptr;
	TArray<UObject*> EditedObjects;

	static const FName GraphCanvasTabID;
	static const FName DetailsTabID;
	static const FName GraphViewTabID;
	static const FName CheckResultTabID;
};

UCLASS()
class GRAPHCHARTEDITOR_API UPathGraphAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UPathGraphAssetFactory();

	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn,
		FName CallingContext
		) override;
};
