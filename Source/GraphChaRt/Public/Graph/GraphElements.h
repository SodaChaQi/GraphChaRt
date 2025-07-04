#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GraphElements.generated.h"

////////////////////////////////////////////////////////////////////////////////////////Enum

UENUM(BlueprintType)
enum class EGraphEdgeType : uint8
{
	Directed,
	Undirected,
	WeightedDirected,
	WeightedUndirected
};

////////////////////////////////////////////////////////////////////////////////////////Struct

USTRUCT(BlueprintType)
struct FGraphNodeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FName NodeID;

	FGraphNodeInfo() = default;

	FGraphNodeInfo(const FName& NodeID)
		: NodeID(NodeID)
	{
		
	}

	FORCEINLINE static FName NodeGUID()
	{
		return  FName(FString::Printf(
			TEXT("GRAPH_NODE_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
	}
	

	bool operator==(const FGraphNodeInfo& Other) const
	{
		return NodeID == Other.NodeID;
	}

	bool operator!=(const FGraphNodeInfo& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FGraphNodeInfo& Node)
	{
		return GetTypeHash(Node.NodeID);
	}
};

USTRUCT(BlueprintType)
struct FGraphEdgeInfo
{
	GENERATED_BODY()

private:

	UPROPERTY()
	EGraphEdgeType EdgeType;
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName StartNodeID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName EndNodeID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge", meta = (DisplayName = "Weight"))
	TOptional<float> Weight;

	FGraphEdgeInfo()
		: EdgeType(EGraphEdgeType::Directed)
	{
		
	}

	FGraphEdgeInfo(const EGraphEdgeType& InEdgeType, const FName& InStartID, const FName& InEndID)
		: EdgeType(InEdgeType), StartNodeID(InStartID), EndNodeID(InEndID)
	{
		
	}

	FGraphEdgeInfo(const EGraphEdgeType& InEdgeType, const FName& InStartID, const FName& InEndID, const float& InWeight)
		: EdgeType(InEdgeType), StartNodeID(InStartID), EndNodeID(InEndID)
	{
		if (InEdgeType == EGraphEdgeType::WeightedDirected || InEdgeType == EGraphEdgeType::WeightedUndirected)
		{
			Weight = InWeight;
		}
	}
	
	bool operator == (const FGraphEdgeInfo& Other) const
	{
		if (EdgeType == Other.EdgeType)
		{
			const bool bSameWeight = (!Weight.IsSet() && !Other.Weight.IsSet()) ||
				(Weight.IsSet() && Other.Weight.IsSet() && FMath::IsNearlyEqual(Weight.GetValue(), Other.Weight.GetValue()));
			
			if (EdgeType == EGraphEdgeType::Directed || EdgeType == EGraphEdgeType::WeightedDirected)
			{
				return StartNodeID == Other.StartNodeID &&
					   EndNodeID == Other.EndNodeID &&
					   bSameWeight;
			}
			if (EdgeType == EGraphEdgeType::Undirected || EdgeType == EGraphEdgeType::WeightedUndirected)
			{
				return ((StartNodeID == Other.StartNodeID && EndNodeID == Other.EndNodeID) ||
						(StartNodeID == Other.EndNodeID && EndNodeID == Other.StartNodeID)) &&
						bSameWeight;
			}

			return false;
		}

		return false;
	}

	bool operator != (const FGraphEdgeInfo& Other) const
	{
		return !(*this == Other);
	}

private:

	TPair<FName, FName> GetNormalizeNodes() const
	{
		if ((EdgeType == EGraphEdgeType::Undirected || EdgeType == EGraphEdgeType::WeightedUndirected) &&
			StartNodeID.LexicalLess(EndNodeID))
		{
			return {StartNodeID, EndNodeID};
		}
		return {EndNodeID, StartNodeID};
	}
	

	friend uint32 GetTypeHash(const FGraphEdgeInfo& Edge)
	{
		const auto [First, Second] = Edge.GetNormalizeNodes();
		uint32 Hash = HashCombine(HashCombine(GetTypeHash(First), GetTypeHash(Second)), GetTypeHash(Edge.EdgeType));

		if (Edge.Weight.IsSet())
		{
			const int32 Quantized = FMath::RoundToInt32(Edge.Weight.GetValue() * 1000.f);
			Hash = HashCombine(Hash, GetTypeHash(Quantized));
		}

		return Hash;
	}
};

USTRUCT()
struct FNodeNeighbors
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FName> Neighbors;

	// operator TArray<FName>() const  { return Neighbors; }
	// void operator=(const TArray<FName>& Other) { Neighbors = Other; }
};

////////////////////////////////////////////////////////////////////////////////////////Interface

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGraphNodeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRAPHCHART_API IGraphNodeInterface
{
	GENERATED_BODY()

public:

	virtual FGraphNodeInfo GetNodeInfo() const = 0;
	
	virtual bool SetNodeInfo(const FGraphNodeInfo& GraphNodeInfo) = 0;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGraphEdgeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRAPHCHART_API IGraphEdgeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FGraphEdgeInfo GetEdgeInfo() const = 0;

	virtual bool SetEdgeInfo(const FGraphEdgeInfo& GraphEdgeInfo) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////UObject

UCLASS()
class GRAPHCHART_API UGraphNode : public UObject, public IGraphNodeInterface
{
	GENERATED_BODY()

	UPROPERTY()
	FGraphNodeInfo NodeInfo;

public:

	FORCEINLINE virtual FGraphNodeInfo GetNodeInfo() const override { return NodeInfo; }

	virtual bool SetNodeInfo(const FGraphNodeInfo& GraphNodeInfo) override;
	
};


UCLASS()
class GRAPHCHART_API UGraphEdge : public UObject, public IGraphEdgeInterface
{
	GENERATED_BODY()

	UPROPERTY()
	FGraphEdgeInfo EdgeInfo;

public:
	
	FORCEINLINE virtual FGraphEdgeInfo GetEdgeInfo() const override { return EdgeInfo; }

	virtual bool SetEdgeInfo(const FGraphEdgeInfo& GraphEdgeInfo) override;
	
};