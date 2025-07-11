// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphRuntime/PathGraphSubsystem.h"

#include "Graph/GraphChaRtSettings.h"
#include "Graph/Graphs.h"


bool UPathGraphSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	
		
	if (const UWorld* World = Outer->GetWorld())
	{
		
#if WITH_EDITOR
		
		if (GIsEditor)
		{
			// 在编辑器中检查是否处于PIE模式
			if (!(World->IsPlayInEditor() || World->IsGameWorld()))
				return false;
		}
		
#endif

		const UGraphChaRtSettings* GraphChaRtSettings = GetDefault<UGraphChaRtSettings>();

		if (GraphChaRtSettings->LevelPathGraphPaths.Contains(World->PersistentLevel->GetOutermost()))
			return true;
		
		for (const auto& LevelStreaming : World->GetStreamingLevels())
		{
			if (GraphChaRtSettings->LevelPathGraphPaths.Find(LevelStreaming->GetWorldAssetPackageName()))
			{
				return  true;
			}
		}
	}
	
	return false;
}

void UPathGraphSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 监听关卡加载/卸载事件
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UPathGraphSubsystem::OnLevelLoaded);
	FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UPathGraphSubsystem::OnLevelUnloaded);
}

void UPathGraphSubsystem::Deinitialize()
{
	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);
	FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
	PathGraphs.Empty();

	Super::Deinitialize();
}

void UPathGraphSubsystem::OnLevelLoaded(ULevel* Level, UWorld* World)
{
	if (const FPathGraphPaths* PathGraphPaths = GetDefault<UGraphChaRtSettings>()->LevelPathGraphPaths.Find(Level))
	{
		for (const FSoftObjectPath& PathGraphPath : PathGraphPaths->PathGraphPaths)
		{
			TSoftObjectPtr<UPathGraph> SoftPathGraph(PathGraphPath);
			
			if (UPathGraph* PathGraph = SoftPathGraph.LoadSynchronous())
			{
				PathGraphs.FindOrAdd(Level).Graphs.Add(PathGraph);
			}
		}
	}
}

void UPathGraphSubsystem::OnLevelUnloaded(ULevel* Level, UWorld* World)
{
	if (GetDefault<UGraphChaRtSettings>()->LevelPathGraphPaths.Contains(Level))
	{
		PathGraphs.Remove(Level);
	}
}
