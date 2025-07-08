// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Graph/GraphElements.h"

// #include "GraphImpls.generated.h"


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

	TSet<TGraphNode> Nodes;
	TSet<TGraphEdge> Edges;
	
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

protected:

	TMap<FName, FNodeNeighbors> AdjacencyList;

	FORCEINLINE const TMap<FName, FNodeNeighbors>& GetAdjacencyList() { return AdjacencyList; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////



