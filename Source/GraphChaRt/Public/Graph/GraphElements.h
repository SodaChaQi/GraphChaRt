#pragma once

#include "CoreMinimal.h"
#include "GraphElements.generated.h"

USTRUCT(BlueprintType)
struct FGraphNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FName NodeID;

	FGraphNode() = default;

	FGraphNode(const FName& NodeID)
		: NodeID(NodeID)
	{
		
	}

	FORCEINLINE static FName NodeGUID()
	{
		return  FName(FString::Printf(
			TEXT("NODE_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
	}
	

	bool operator==(const FGraphNode& Other) const
	{
		return NodeID == Other.NodeID;
	}

	bool operator!=(const FGraphNode& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FGraphNode& Node)
	{
		return GetTypeHash(Node.NodeID);
	}
};

USTRUCT(BlueprintType)
struct FGraphEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName StartNodeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName EndNodeID;

	FGraphEdge() = default;

	FGraphEdge(const FName& InStartID, const FName& InEndID)
		: StartNodeID(InStartID), EndNodeID(InEndID)
	{
		
	}

	bool operator==(const FGraphEdge& Other) const
	{
		return StartNodeID == Other.StartNodeID && EndNodeID == Other.EndNodeID;
	}

	bool operator!=(const FGraphEdge& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FGraphEdge& Edge)
	{
		return HashCombine(
			GetTypeHash(Edge.StartNodeID),
			GetTypeHash(Edge.EndNodeID)
			);
	}
};

USTRUCT(BlueprintType)
struct FWeightedEdge : public FGraphEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	float Weight = 0.0f;
	
	FWeightedEdge() = default;

	FWeightedEdge(const FName& InStartID, const FName& InEndID, float InWeight)
		: FGraphEdge(InStartID, InEndID), Weight(InWeight)
	{
		
	}

	bool operator==(const FWeightedEdge& Other) const
	{
		return StartNodeID == Other.StartNodeID &&
			EndNodeID == Other.EndNodeID &&
				FMath::IsNearlyEqual(Weight, Other.Weight);
	}

	friend uint32 GetTypeHash(const FWeightedEdge& Edge)
	{
		const int32 QuantizedWeight = FMath::RoundToInt32(Edge.Weight * 1000.f);
		
		return HashCombine(
			HashCombine(GetTypeHash(Edge.StartNodeID), GetTypeHash(Edge.EndNodeID)),
			GetTypeHash(QuantizedWeight)
			);
	}
};

USTRUCT(BlueprintType)
struct FDirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FDirectedEdge() = default;

	FDirectedEdge(const FName& InStartID, const FName& InEndID)
		: FGraphEdge(InStartID, InEndID)
	{
		
	}
};

USTRUCT(BlueprintType)
struct FUndirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FUndirectedEdge() = default;

	FUndirectedEdge(const FName& InStartID, const FName& InEndID)
		: FGraphEdge(InStartID, InEndID)
	{
		
	}

	bool operator==(const FUndirectedEdge& Other) const
	{
		return (StartNodeID == Other.StartNodeID && EndNodeID == Other.EndNodeID) ||
			(StartNodeID == Other.EndNodeID && EndNodeID == Other.StartNodeID);
	}

	friend uint32 GetTypeHash(const FUndirectedEdge& Edge)
	{
		const bool bOrdered = Edge.StartNodeID.LexicalLess(Edge.EndNodeID);
		const FName& First = bOrdered ? Edge.StartNodeID : Edge.EndNodeID;
		const FName& Second = bOrdered ? Edge.EndNodeID : Edge.StartNodeID;

		return HashCombine(GetTypeHash(First), GetTypeHash(Second));
	}
};

USTRUCT(BlueprintType)
struct FWeightedDirectedEdge : public FWeightedEdge
{
	GENERATED_BODY()

	FWeightedDirectedEdge() = default;

	FWeightedDirectedEdge(const FName& InStartID, const FName& InEndID, float InWeight)
		: FWeightedEdge(InStartID, InEndID, InWeight)
	{
		
	}
};

USTRUCT(BlueprintType)
struct FWeightedUndirectedEdge : public FWeightedEdge
{
	GENERATED_BODY()

	FWeightedUndirectedEdge() = default;

	FWeightedUndirectedEdge(const FName& InStartID, const FName& InEndID, float InWeight)
		: FWeightedEdge(InStartID, InEndID, InWeight)
	{
		
	}

	bool operator==(const FWeightedUndirectedEdge& Other) const
	{
		return (StartNodeID == Other.StartNodeID && EndNodeID == Other.EndNodeID && FMath::IsNearlyEqual(Weight, Other.Weight)) ||
			   (StartNodeID == Other.EndNodeID && EndNodeID == Other.StartNodeID && FMath::IsNearlyEqual(Weight, Other.Weight));
	}

	friend uint32 GetTypeHash(const FWeightedUndirectedEdge& Edge)
	{
		const bool bOrdered = Edge.StartNodeID.LexicalLess(Edge.EndNodeID);
		const FName& First = bOrdered ? Edge.StartNodeID : Edge.EndNodeID;
		const FName& Second = bOrdered ? Edge.EndNodeID : Edge.StartNodeID;

		const int32 QuantizedWeight = FMath::RoundToInt32(Edge.Weight * 1000.f);

		return HashCombine(
			HashCombine(GetTypeHash(First), GetTypeHash(Second)),
			GetTypeHash(QuantizedWeight)
			);
	}
};

USTRUCT()
struct FNodeNeighbors
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FName> Neighbors;

	operator TArray<FName>() const  { return Neighbors; }
	void operator=(const TArray<FName>& Other) { Neighbors = Other; }
};