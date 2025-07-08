// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"



UPathGraph::UPathGraph()
{
	Graph = MakeUnique<TGraphImpl<FGraphNode, FDirectedEdge>>();
	
	if (Graph != nullptr)
	{
		// TSet<FName> Neighbors;
		// Neighbors.Add(FName("None")) == ;
	}
}

void UPathGraph::AddNode()
{
	if (Graph == nullptr) return;

	const FGraphNode Node = FGraphNode(FGraphNode::NodeGUID());
	Graph->AddNode(Node);
}

bool UPathGraph::GetNode(const FName& NodeID, FGraphNode& OutNode)
{
	if (Graph == nullptr) return false;

	return Graph->GetNode(NodeID, OutNode);
}

bool UPathGraph::RemoveNode(const FName& NodeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveNode(NodeID);
}

bool UPathGraph::AddEdge(const FDirectedEdge& Edge)
{
	if (Graph == nullptr) return false;
	
	return Graph->AddEdge(Edge);
}

bool UPathGraph::GetEdge(const FGraphEdgeID& NodeID, FDirectedEdge& OutEdge)
{
	if (Graph == nullptr) return false;

	return Graph->GetEdge(NodeID, OutEdge);
}

bool UPathGraph::RemoveEdge(const FGraphEdgeID& EdgeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveEdge(EdgeID);
}

TArray<FName> UPathGraph::GetNodeNeighbors(FName NodeID)
{
	if (Graph == nullptr) return TArray<FName>();

	return Graph->GetNodeNeighbors(NodeID);
}

void UPathGraph::PostLoad()
{
	Super::PostLoad();

	BuildAdjacencyList();
}

#if WITH_EDITOR

void UPathGraph::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// BuildAdjacencyList();
}

#endif

void UPathGraph::BuildAdjacencyList()
{
	// AdjacencyList.Empty();
	//
	// for (const FGraphNodeInfo& Node : Nodes)
	// {
	// 	AdjacencyList.Add(Node.NodeID, FNodeNeighbors());
	// }
	//
	// for (const FGraphEdgeInfo& Edge : Edges)
	// {
	// 	if (FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(Edge.StartNodeID))
	// 	{
	// 		NodeNeighbors->Neighbors.AddUnique(Edge.EndNodeID);
	// 	}
	// }
}