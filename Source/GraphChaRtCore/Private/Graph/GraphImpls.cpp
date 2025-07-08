// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphImpls.h"


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
	if (Nodes.Contains(Edge.EdgeID.StartNodeID) && Nodes.Contains(Edge.EdgeID.EndNodeID) && Edges.Add(Edge).IsValidId())
	{
		if (const auto NodeNeighbors = AdjacencyList.Find(Edge.EdgeID.StartNodeID))
		{
			NodeNeighbors->Neighbors.AddUnique(Edge.EdgeID.EndNodeID);
		}
		if (!Edge.EdgeID.GetIsOrderSensitive())
		{
			if (const auto NodeNeighbors = AdjacencyList.Find(Edge.EdgeID.EndNodeID))
			{
				NodeNeighbors->Neighbors.AddUnique(Edge.EdgeID.StartNodeID);
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
		if (auto* StartNeighbors = AdjacencyList.Find(EdgeCopy.EdgeID.StartNodeID))
		{
			StartNeighbors->Neighbors.Remove(EdgeCopy.EdgeID.EndNodeID);
		}
        
		// 如果是无向边，更新反向连接
		if (!EdgeCopy.EdgeID.GetIsOrderSensitive())
		{
			if (auto* EndNeighbors = AdjacencyList.Find(EdgeCopy.EdgeID.EndNodeID))
			{
				EndNeighbors->Neighbors.Remove(EdgeCopy.EdgeID.StartNodeID);
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

//////////////////////////////////////////////////////////////////////////////////////////////////


