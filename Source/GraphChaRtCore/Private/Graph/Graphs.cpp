// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"

#include "Graph/GraphChaRtSettings.h"
#include "UObject/ObjectSaveContext.h"

DEFINE_LOG_CATEGORY(GraphsLog);

UPathGraphSchema::UPathGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPathGraph::UPathGraph()
	: LevelContext(nullptr)
	, EdGraph(nullptr)
{
	Graph = MakeUnique<TGraphImpl<FPathNode, FPathEdge>>();
	
	if (Graph != nullptr)
	{
		// TSet<FName> Neighbors;
		// Neighbors.Add(FName("None"));
	}
}

UPathGraph::UPathGraph(const FName& InPathGraphID, const TSoftObjectPtr<ULevel>& InLevelContext)
	: PathGraphID(InPathGraphID), LevelContext(InLevelContext), EdGraph(nullptr)
{
}

void UPathGraph::AddNode(const FPathNodeData& InNodeData)
{
	if (Graph == nullptr) return;

	const FName NewNodeID = FGraphNode::NodeGUID();
	const FPathNode Node = FPathNode(NewNodeID, InNodeData);
	Graph->AddNode(Node);
}

bool UPathGraph::GetNode(const FName& NodeID, FPathNode& OutNode)
{
	if (Graph == nullptr) return false;

	return Graph->GetNode(NodeID, OutNode);
}

bool UPathGraph::RemoveNode(const FName& NodeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveNode(NodeID);
}

bool UPathGraph::AddEdge(const FPathEdge& Edge)
{
	if (Graph == nullptr) return false;
	
	return Graph->AddEdge(Edge);
}

bool UPathGraph::GetEdge(const FGraphEdgeID& NodeID, FPathEdge& OutEdge)
{
	if (Graph == nullptr) return false;

	return Graph->GetEdge(NodeID, OutEdge);
}

bool UPathGraph::RemoveEdge(const FGraphEdgeID& EdgeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveEdge(EdgeID);
}

TArray<FName> UPathGraph::GetNodeNeighbors(FName NodeID) const
{
	if (Graph == nullptr) return TArray<FName>();

	return Graph->GetNodeNeighbors(NodeID);
}

void UPathGraph::PostLoad()
{
	Super::PostLoad();

	BuildAdjacencyList();
}

void UPathGraph::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);

	if (Graph == nullptr) return;
	Graph->Serialize(Ar);
}

#if WITH_EDITOR

void UPathGraph::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	MarkPackageDirty();
}

void UPathGraph::PreSave(FObjectPreSaveContext SaveContext)
{
	UObject::PreSave(SaveContext);
}

void UPathGraph::PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext)
{
	UGraphChaRtSettings* GraphChaRtSettings = GetMutableDefault<UGraphChaRtSettings>();

	if (FPathGraphPaths* PathGraphPaths = GraphChaRtSettings->LevelPathGraphPaths.Find(LevelContext.ToSoftObjectPath()))
	{
		if (!PathGraphPaths->PathGraphPaths.Contains(this))
		{
			PathGraphPaths->PathGraphPaths.Add(this);
		}
	}
	else
	{
		GraphChaRtSettings->LevelPathGraphPaths.Add(LevelContext.ToSoftObjectPath(), FPathGraphPaths(TSet<FSoftObjectPath> { this }));
	}

	GraphChaRtSettings->SaveConfig(CPF_Config, *GraphChaRtSettings->GetDefaultConfigFilename());
	GraphChaRtSettings->PostEditChange();

	UE_LOG(LogTemp, Log, TEXT("GraphChaRt Settings updated and saved"));
}

#endif

void UPathGraph::BuildAdjacencyList()
{
	if (Graph == nullptr) return;

	Graph->BuildAdjacencyList();
}