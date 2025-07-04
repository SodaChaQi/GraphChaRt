// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Graph/GraphElements.h"

#include "Graphs.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class GRAPHCHART_API UGraphBase : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Graph")
	TMap<UObject*, TScriptInterface<IGraphNodeInterface>> Nodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Graph")
	TMap<UObject*, TScriptInterface<IGraphEdgeInterface>> Edges;

public:

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool AddNode(const FGraphNodeInfo& Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool RemoveNode(const FName& NodeID);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool GetNode(const FName& NodeID, FGraphNodeInfo& OutNode);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool AddEdge(const FGraphEdgeInfo& Edge);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool RemoveEdge(const FName& StartNodeID, const FName& EndNodeID);

	virtual bool RemoveEdge(const FName& StartNodeID, const FName& EndNodeID, const float& Weight);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool GetEdge(const FName& StartNodeID, const FName& EndNodeID, FGraphEdgeInfo& OutEdge);

	UFUNCTION(BlueprintPure, Category = "Graph")
	virtual TArray<FName> GetNodeNeighbors(FName NodeID) const PURE_VIRTUAL(UGraphBase::GetNodeNeighbors, return TArray<FName>(););

	virtual void PostLoad() override;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

protected:
	
	UPROPERTY(Transient)
	TMap<FName, FNodeNeighbors> AdjacencyList;

	virtual void BuildAdjacencyList() PURE_VIRTUAL(UGraphBase::BuildAdjacencyList);
	
};
