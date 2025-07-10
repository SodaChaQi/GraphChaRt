// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Graph/GraphElements.h"


/**
 * 
 */


template<	typename TGraphNode,
			typename TGraphEdge,
			typename = typename TEnableIf<TIsDerivedFrom<TGraphNode, FGraphNode>::Value, void>::Type,
			typename = typename TEnableIf<TIsDerivedFrom<TGraphEdge, FGraphEdge>::Value, void>::Type
		>
class TGraphImpl
{

	DECLARE_DELEGATE_OneParam(FOnNodeAdded, const TGraphNode&);
	DECLARE_DELEGATE_OneParam(FOnNodeRemoved, const TGraphNode&);
	DECLARE_DELEGATE_OneParam(FOnEdgeAdded, const TGraphEdge&);
	DECLARE_DELEGATE_OneParam(FOnEdgeRemoved, const TGraphEdge&);

protected:
	
	TSet<TGraphNode> Nodes;
	TSet<TGraphEdge> Edges;

	TMap<FName, FNodeNeighbors> AdjacencyList;
	
public:

	virtual ~TGraphImpl() = default;

	FOnNodeAdded OnNodeAdded;
	FOnNodeRemoved OnNodeRemoved;
	FOnEdgeAdded OnEdgeAdded;
	FOnEdgeRemoved OnEdgeRemoved;

	virtual bool AddNode(const TGraphNode& Node);
	virtual bool GetNode(const FName& NodeID, TGraphNode& OutNode);
	virtual bool RemoveNode(const FName& NodeID);

	virtual bool AddEdge(const TGraphEdge& Edge);
	virtual bool GetEdge(const FGraphEdgeID& NodeID, TGraphEdge& OutEdge);
	virtual bool RemoveEdge(const FGraphEdgeID& EdgeID);

	const TArray<FName>& GetNodeNeighbors(FName NodeID);

	void BuildAdjacencyList();

	FORCEINLINE const TMap<FName, FNodeNeighbors>& GetAdjacencyList() { return AdjacencyList; }

	TSet<TGraphNode> GetNodes() const { return Nodes; }
	TSet<TGraphEdge> GetEdges() const { return Edges; }


	void Serialize(FArchive& Ar);
	
};


///////////////////////////////////////////////////////////////////////////////////////////////////


template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddNode(const TGraphNode& Node)
{
	if (Nodes.Add(Node).IsValidId())
	{
		AdjacencyList.Add(Node.NodeID, FNodeNeighbors());
		OnNodeAdded.ExecuteIfBound(Node);
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetNode(const FName& NodeID, TGraphNode& OutNode)
{
	if (const TGraphNode* Node = Nodes.Find(NodeID))
	{
		OutNode = *Node;
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::RemoveNode(const FName& NodeID)
{
	if (const TGraphNode* Node = Nodes.Find(NodeID))
	{
		TGraphNode NodeCopy = *Node;
		Nodes.Remove(NodeCopy);
		OnNodeRemoved.ExecuteIfBound(NodeCopy);

		AdjacencyList.Remove(NodeID);
		for (auto& Pair : AdjacencyList)
		{
			Pair.Value.Neighbors.Remove(NodeCopy.NodeID);
		}
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::AddEdge(const TGraphEdge& Edge)
{
	if (Nodes.Contains(Edge.GetID().StartNodeID) && Nodes.Contains(Edge.GetID().EndNodeID) && Edges.Add(Edge).IsValidId())
	{
		if (const auto NodeNeighbors = AdjacencyList.Find(Edge.GetID().StartNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.GetID().EndNodeID);
		}
		if (!Edge.GetID().GetIsOrderSensitive())
		{
			if (const auto NodeNeighbors = AdjacencyList.Find(Edge.GetID().EndNodeID))
			{
				NodeNeighbors->Neighbors.AddUnique(Edge.GetID().StartNodeID);
			}
		}
		OnEdgeAdded.ExecuteIfBound(Edge);
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetEdge(const FGraphEdgeID& NodeID, TGraphEdge& OutEdge)
{
	if (const TGraphEdge* Edge = Edges.Find(NodeID))
	{
		OutEdge = *Edge;
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
bool TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::RemoveEdge(const FGraphEdgeID& EdgeID)
{
	if (const TGraphEdge* Edge = Edges.Find(EdgeID))
	{
		TGraphEdge EdgeCopy = *Edge; // 复制边数据（用于后续操作）
		Edges.Remove(EdgeCopy);      // 高效移除边
        
		// 更新邻接表
		if (auto* StartNeighbors = AdjacencyList.Find(EdgeCopy.GetID().StartNodeID))
		{
			StartNeighbors->Neighbors.Remove(EdgeCopy.GetID().EndNodeID);
		}
        
		// 如果是无向边，更新反向连接
		if (!EdgeCopy.GetID().GetIsOrderSensitive())
		{
			if (auto* EndNeighbors = AdjacencyList.Find(EdgeCopy.GetID().EndNodeID))
			{
				EndNeighbors->Neighbors.Remove(EdgeCopy.GetID().StartNodeID);
			}
		}
        
		// 触发事件通知
		OnEdgeRemoved.ExecuteIfBound(EdgeCopy);
        
		return true;
	}
	return false;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
const TArray<FName>& TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::GetNodeNeighbors(FName NodeID)
{
	if (const FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(NodeID))
	{
		return NodeNeighbors->Neighbors;
	}

	static TArray<FName> EmptyArray;
	return EmptyArray;
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
void TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::BuildAdjacencyList()
{
	AdjacencyList.Empty();
	
	for (const TGraphNode& Node : Nodes)
	{
		AdjacencyList.Add(Node.NodeID, FNodeNeighbors());
	}
	
	for (const TGraphEdge& Edge : Edges)
	{
		if (FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(Edge.GetID().StartNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.GetID().EndNodeID);
		}
		if (!Edge.GetID().GetIsOrderSensitive())
		{
			if (FNodeNeighbors* NodeNeighbors = AdjacencyList.Find(Edge.GetID().EndNodeID))
			{
				NodeNeighbors->Neighbors.AddUnique(Edge.GetID().StartNodeID);
			}
		}
	}
}

template <typename TGraphNode, typename TGraphEdge, typename T0, typename T1>
void TGraphImpl<TGraphNode, TGraphEdge, T0, T1>::Serialize(FArchive& Ar)
{
	Ar << Nodes;
	Ar << Edges;
}

