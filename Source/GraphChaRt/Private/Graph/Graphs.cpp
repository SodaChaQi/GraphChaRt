// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"


template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddNode(const TGraphNode& Node)
{
	if (Nodes.AddUnique(Node) != INDEX_NONE)
	{
		AdjacencyList.Add(Node.NodeID);
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
	
	const int32 RemoveCount = Nodes.RemoveAll([&](const TGraphNode& Node)
	{
		return Node.NodeID == NodeID;
	});

	if (RemoveCount > 0)
	{
		AdjacencyList.Remove(NodeID);
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddEdge(const TGraphEdge& Edge)
{
	if (Nodes.Contains(Edge.EdgeID.StartNodeID) && Nodes.Contains(Edge.EdgeID.EndNodeID) && Edges.AddUnique(Edge) != INDEX_NONE)
	{
		if (const auto NodeNeighbors = AdjacencyList.Find(Edge.EdgeID.StartNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.EdgeID.EndNodeID);
		}
		if (const auto NodeNeighbors = AdjacencyList.Find(Edge.EdgeID.EndNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.EdgeID.StartNodeID);
		}
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

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::RemoveEdge(const FGraphEdgeID& EdgeID)
{
	const int32 RemoveCount = Edges.RemoveAll([&](const TGraphEdge& Edge)
	{
		return Edge.EdgeID == EdgeID;
	});

	if (RemoveCount > 0)
	{
		if (const auto NodeNeighbors = AdjacencyList.Find(EdgeID.StartNodeID))
		{
			NodeNeighbors->Neighbors.RemoveAll([&](const FName& NodeID)
			{
				return EdgeID.EndNodeID == NodeID;
			});
		}
		if (const auto NodeNeighbors = AdjacencyList.Find(EdgeID.EndNodeID))
		{
			NodeNeighbors->Neighbors.RemoveAll([&](const FName& NodeID)
			{
				return EdgeID.StartNodeID == NodeID;
			});
		}
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
TArray<FName> TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetNodeNeighbors(FName NodeID)
{
	if (const FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(NodeID))
	{
		return NodeNeighbors->Neighbors;
	}

	return TArray<FName>();
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

bool UGraphBase::AddEdge(const FDirectedEdge& Edge)
{
	if (Graph == nullptr) return false;
	
	return Graph->AddEdge(Edge);
}

bool UGraphBase::GetEdge(const FGraphEdgeID& NodeID, FDirectedEdge& OutEdge)
{
	if (Graph == nullptr) return false;

	return Graph->GetEdge(NodeID, OutEdge);
}

bool UGraphBase::RemoveEdge(const FGraphEdgeID& EdgeID)
{
	if (Graph == nullptr) return false;

	return Graph->RemoveEdge(EdgeID);
}

TArray<FName> UGraphBase::GetNodeNeighbors(FName NodeID)
{
	if (Graph == nullptr) return TArray<FName>();

	return Graph->GetNodeNeighbors(NodeID);
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
