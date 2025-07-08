#pragma once


#include "CoreMinimal.h"
#include "GraphImpls.h"

#include "Graphs.generated.h"



/**
 * 
 */



UCLASS()
class GRAPHCHARTCORE_API UPathGraph : public UObject
{
	GENERATED_BODY()

public:

	UPathGraph();

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