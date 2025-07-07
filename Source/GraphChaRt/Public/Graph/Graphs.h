// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Graph/GraphElements.h"

#include "Graphs.generated.h"


/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnNodeAdded, const FGraphNode&);
DECLARE_DELEGATE_OneParam(FOnNodeRemoved, FGraphNode);

template<	typename TGraphNode,
			typename TGraphEdge,
			typename = typename TEnableIf<TIsDerivedFrom<TGraphNode, FGraphNode>::Value, void>::Type,
			typename = typename TEnableIf<TIsDerivedFrom<TGraphEdge, FGraphEdge>::Value, void>::Type
		>
class TGraphImpl
{
public:

	virtual ~TGraphImpl() = default;
	
	TArray<TGraphNode> Nodes;

	TArray<TGraphEdge> Edges;

	FOnNodeAdded OnNodeAdded;
	FOnNodeRemoved OnNodeRemoved;

	virtual bool AddNode(const TGraphNode& Node);
	virtual bool GetNode(const FName& NodeID, TGraphNode& OutNode);
	virtual bool RemoveNode(const FName& NodeID);

	virtual bool AddEdge(const TGraphEdge& Edge);
	virtual bool GetEdge(const FGraphEdgeID& NodeID, TGraphEdge& OutEdge);
	virtual bool RemoveEdge(const FGraphEdgeID& EdgeID);

	TArray<FName> GetNodeNeighbors(FName NodeID);

protected:

	TMap<FName, FNodeNeighbors> AdjacencyList;

	FORCEINLINE const TMap<FName, FNodeNeighbors>& GetAdjacencyList() { return AdjacencyList; }
};

UCLASS()
class GRAPHCHART_API UGraphBase : public UObject
{
	GENERATED_BODY()

public:

	UGraphBase();

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void AddNode();

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool GetNode(const FName& NodeID, FGraphNode& OutNode);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool RemoveNode(const FName& NodeID);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool AddEdge(const FDirectedEdge& Edge);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool GetEdge(const FGraphEdgeID& NodeID, FDirectedEdge& OutEdge);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool RemoveEdge(const FGraphEdgeID& EdgeID);

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<FName> GetNodeNeighbors(FName NodeID);

	virtual void PostLoad() override;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

protected:

	TUniquePtr<TGraphImpl<FGraphNode, FDirectedEdge>> Graph;

	void BuildAdjacencyList();
	
};
