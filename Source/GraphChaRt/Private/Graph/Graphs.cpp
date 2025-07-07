// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"


template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddNode(const TGraphNode& Node)
{
	if (Nodes.AddUnique(Node) != INDEX_NONE)
	{
		OnNodeAdded.ExecuteIfBound(Node);
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetNode(const FName& NodeID, TGraphNode& OutNode)
{
	if (const TGraphNode* Node = Nodes.FindByKey(NodeID))
	{
		OutNode = *Node;
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::RemoveNode(const FName& NodeID)
{
	return 
	Nodes.RemoveAll([&](const FGraphNode& Node){
					return Node.NodeID == NodeID;
	}) > 0;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddEdge(const TGraphEdge& Edge)
{
	if (Nodes.Contains(Edge.EdgeID.StartNodeID) && Nodes.Contains(Edge.EdgeID.EndNodeID) && Edges.AddUnique(Edge) != INDEX_NONE)
	{
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetEdge(const FGraphEdgeID& NodeID, TGraphEdge& OutEdge)
{
	if (const TGraphEdge* Edge = Edges.FindByKey(NodeID))
	{
		OutEdge = *Edge;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

UGraphBase::UGraphBase()
{
	Graph = MakeUnique<TGraphImpl<FGraphNode, FDirectedEdge>>();
	
	if (Graph != nullptr)
	{
		
	}
}

void UGraphBase::AddNode()
{
	if (Graph == nullptr) return;

	const FGraphNode Node = FGraphNode(FGraphNode::NodeGUID());
	Graph->AddNode(Node);
}

bool UGraphBase::GetNode(const FName& NodeID, FGraphNode& OutNode)
{
	if (Graph == nullptr) return false;

	return Graph->GetNode(NodeID, OutNode);
}

bool UGraphBase::RemoveNode(const FName& NodeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveNode(NodeID);
}

void UGraphBase::PostLoad()
{
	Super::PostLoad();

	BuildAdjacencyList();
}

#if WITH_EDITOR

void UGraphBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BuildAdjacencyList();
}

#endif

bool UGraphBase::AddEdge(const FDirectedEdge& Edge)
{
	if (Graph == nullptr) return false;

	return Graph->AddEdge(Edge);
}

bool UGraphBase::GetEdge(const FName& StartNodeID, const FName& EndNodeID, FDirectedEdge& OutEdge)
{
	return false;
}

bool UGraphBase::RemoveEdge(const FName& StartNodeID, const FName& EndNodeID)
{
	return false;
}

TArray<FName> UGraphBase::GetNodeNeighbors(FName NodeID)
{
	// if (const FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(NodeID))
	// {
	// 	return NodeNeighbors->Neighbors;
	// }

	return TArray<FName>();
}

void UGraphBase::BuildAdjacencyList()
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
