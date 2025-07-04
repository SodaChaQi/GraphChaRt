// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"


bool UGraphBase::AddNode(const FGraphNodeInfo& Node)
{
	NewObject<UGraphNode>(this)->SetNodeInfo(Node);
	return true;
}

// bool UGraphBase::RemoveNode(const FName& NodeID)
// {
// 	return
// 	Nodes.RemoveAll([&](const FGraphNodeInfo& Node)
// 	{
// 		return Node.NodeID == NodeID;
// 	}) > 0;
// }
//
// bool UGraphBase::GetNode(const FName& NodeID, FGraphNodeInfo& OutNode)
// {
// 	for (FGraphNodeInfo& Node : Nodes)
// 	{
// 		if (Node.NodeID == NodeID)
// 		{
// 			OutNode = Node;
// 			return true;
// 		}
// 	}
// 	return false;
// }
//
// void UGraphBase::PostLoad()
// {
// 	Super::PostLoad();
//
// 	BuildAdjacencyList();
// }
//
// #if WITH_EDITOR
//
// void UGraphBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	BuildAdjacencyList();
// }
//
// #endif
//
// bool UGraphBase::AddEdge(const FGraphEdgeInfo& Edge)
// {
// 	if (Nodes.Contains(Edge.StartNodeID) && Nodes.Contains(Edge.EndNodeID))
// 	{
// 		Edges.Add(Edge);
// 		return true;
// 	}
// 	
// 	return false;
// }
//
// TArray<FName> UDirectedGraph::GetNodeNeighbors(FName NodeID) const
// {
// 	if (const FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(NodeID))
// 	{
// 		return NodeNeighbors->Neighbors;
// 	}
//
// 	return TArray<FName>();
// }
//
// void UDirectedGraph::BuildAdjacencyList()
// {
// 	AdjacencyList.Empty();
//
// 	for (const FGraphNodeInfo& Node : Nodes)
// 	{
// 		AdjacencyList.Add(Node.NodeID, FNodeNeighbors());
// 	}
//
// 	for (const FGraphEdgeInfo& Edge : Edges)
// 	{
// 		if (FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(Edge.StartNodeID))
// 		{
// 			NodeNeighbors->Neighbors.AddUnique(Edge.EndNodeID);
// 		}
// 	}
// }
