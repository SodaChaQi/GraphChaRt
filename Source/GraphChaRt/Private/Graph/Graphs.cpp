// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Graphs.h"

void UGraphBase::AddNode(const FGraphNode& Node)
{
	Nodes.AddUnique(Node);
}

bool UGraphBase::RemoveNode(const FName& NodeID)
{
	if (Nodes.Remove(NodeID) > 0) return true;
	
	return false;
}

bool UGraphBase::GetNode(FName NodeID, FGraphNode& OutNode)
{
	for (const FGraphNode& Node : Nodes)
	{
		if (Node.NodeID == NodeID)
		{
			OutNode = Node;
			return true;
		}
	}
	return false;
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



//UDirectedGraph
void UDirectedGraph::AddEdge(const FGraphEdge& Edge)
{
	Edges.Add(Edge);

	//需要测试Edges.Add(Edge);会不会触发PostEditChangeProperty(),若触发则删去该句
	//BuildAdjacencyList();
}

TArray<FName> UDirectedGraph::GetNodeNeighbors(FName NodeID) const
{
	if (const FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(NodeID))
	{
		return NodeNeighbors->Neighbors;
	}

	return TArray<FName>();
}

void UDirectedGraph::BuildAdjacencyList()
{
	AdjacencyList.Empty();

	for (const FGraphNode& Node : Nodes)
	{
		AdjacencyList.Add(Node.NodeID, FNodeNeighbors());
	}

	for (const FGraphEdge& Edge : Edges)
	{
		if (FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(Edge.StartNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.EndNodeID);
		}
	}
}
