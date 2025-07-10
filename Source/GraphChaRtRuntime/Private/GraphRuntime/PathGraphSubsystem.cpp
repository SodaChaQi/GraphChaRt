// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphRuntime/PathGraphSubsystem.h"

#include "Graph/GraphChaRtSettings.h"

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
		for (const auto& Level : World->GetStreamingLevels())
		{
			if (GraphChaRtSettings->LevelPathGraphPaths.Find(Level->GetOutermost()->GetFName()))
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
	Super::Deinitialize();
	
}

void UPathGraphSubsystem::OnLevelLoaded(ULevel* Level, UWorld* World)
{
	if (const FPathGraphPaths* PathGraphPaths = GetDefault<UGraphChaRtSettings>()->LevelPathGraphPaths.Find(Level->GetOutermost()->GetFName()))
	{
		for (const auto& PathGraphPath : PathGraphPaths->PathGraphPaths)
		{
			//加载UPathGraph
			//PathGraphs.Add()
			TSoftObjectPtr<UPathGraph> SoftPathGraph(PathGraphPath.ToString());
			
			if (UPathGraph* PathGraph = SoftPathGraph.LoadSynchronous())
			{
				PathGraphs.Add(PathGraph);
			}
		}
	}
}

void UPathGraphSubsystem::OnLevelUnloaded(ULevel* Level, UWorld* World)
{
	if (Level->GetOutermost()->GetFName() == TEXT(""))
	{
		
	}
}
