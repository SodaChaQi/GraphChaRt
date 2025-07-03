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
class GRAPHCHART_API UGraphBase : public UDataAsset
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Graph")
	TArray<FGraphNode> Nodes;

public:

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual void AddNode(const FGraphNode& Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool RemoveNode(const FName& NodeID);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	virtual bool GetNode(const FName& NodeID, FGraphNode& OutNode);

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

UCLASS()
class GRAPHCHART_API UDirectedGraph : public UGraphBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TArray<FDirectedEdge> Edges;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool AddEdge(const FDirectedEdge& Edge);

	virtual TArray<FName> GetNodeNeighbors(FName NodeID) const override;

protected:

	virtual void BuildAdjacencyList() override;
};
