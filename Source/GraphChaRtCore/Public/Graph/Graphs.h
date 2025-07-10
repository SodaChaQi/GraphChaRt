#pragma once


#include "CoreMinimal.h"
#include "GraphImpls.h"

#include "Graphs.generated.h"



/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(GraphsLog, Log, All);

USTRUCT(BlueprintType)
struct FPathNodeData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FName NodeName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FTransform WorldTransform = FTransform::Identity;

	friend FArchive& operator<<(FArchive& Ar, FPathNodeData& NodeData)
	{
		Ar << NodeData.NodeName;
		Ar << NodeData.WorldTransform;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct FPathNode : public FGraphNode
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FPathNodeData Data;

	FPathNode()
		: FGraphNode(NAME_None)
	{}
	
	FPathNode(const FName& InNodeID)
		: FGraphNode(InNodeID)
	{}

	FPathNode(const FName& InNodeID, const FPathNodeData& InData)
		: FGraphNode(InNodeID), Data(InData)
	{}
	
	bool operator == (const FPathNode& Other) const
	{
		return NodeID == Other.NodeID;
	}

	bool operator != (const FPathNode& Other) const
	{
		return !(*this == Other);
	}

	friend FArchive& operator<<(FArchive& Ar, FPathNode& Node)
	{
		Ar << Node.NodeID;
		Ar << Node.Data;
		return Ar;
	}

private:

	friend uint32 GetTypeHash(const FPathNode& Node)
	{
		return GetTypeHash(Node.NodeID);
	}
	
};

USTRUCT(BlueprintType)
struct FPathEdge : public FDirectedWeightedEdge
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	int32 LaneCount = 1;

	FPathEdge()
		: FDirectedWeightedEdge(NAME_None, NAME_None)
	{}

	FPathEdge(const FName& InStartID, const FName& InEndID, const float InWeight, const int32 InLaneCount)
		: FDirectedWeightedEdge(InStartID, InEndID, InWeight), LaneCount(InLaneCount)
	{}

	FPathEdge(const FGraphEdgeID& InEdgeID)
		: FDirectedWeightedEdge(InEdgeID.StartNodeID, InEdgeID.EndNodeID)
	{}
	
	bool operator == (const FPathEdge& Other) const
	{
		return EdgeID == Other.EdgeID &&
				FixedWeight == Other.FixedWeight &&
					LaneCount == Other.LaneCount;
	}

	bool operator != (const FPathEdge& Other) const
	{
		return !(*this == Other);
	}

	friend FArchive& operator<<(FArchive& Ar, FPathEdge& Edge)
	{
		Ar << Edge.EdgeID;
		Ar << Edge.FixedWeight;
		Ar << Edge.LaneCount;
		return Ar;
	}

private:

	friend uint32 GetTypeHash(const FPathEdge& Edge)
	{
		return HashCombine(HashCombine(GetTypeHash(Edge.EdgeID),
			GetTypeHash(Edge.FixedWeight)),
			GetTypeHash(Edge.LaneCount));
	}
};

UCLASS(BlueprintType)
class GRAPHCHARTCORE_API UPathGraph : public UObject
{
	GENERATED_BODY()

public:

	UPathGraph();

	UPathGraph(const FName& InPathGraphID, const TSoftObjectPtr<ULevel>& InLevelContext);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FName PathGraphID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Path")
	TSoftObjectPtr<ULevel> LevelContext;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void AddNode(const FPathNodeData& InNodeData);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool GetNode(const FName& NodeID, FPathNode& OutNode);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool RemoveNode(const FName& NodeID);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool AddEdge(const FPathEdge& Edge);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool GetEdge(const FGraphEdgeID& NodeID, FPathEdge& OutEdge);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool RemoveEdge(const FGraphEdgeID& EdgeID);

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<FName> GetNodeNeighbors(FName NodeID) const;

	virtual void PostLoad() override;

	virtual void Serialize(FArchive& Ar) override;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

protected:

	TUniquePtr<TGraphImpl<FPathNode, FPathEdge>> Graph;

	void BuildAdjacencyList();

public:

	FORCEINLINE TSet<FPathNode> GetPathNodes() const { if (Graph == nullptr) return TSet<FPathNode>(); return Graph->GetNodes(); }
	
};