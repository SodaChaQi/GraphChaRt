#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GraphElements.generated.h"



////////////////////////////////////////////////////////////////////////////////////////Struct


USTRUCT()
struct FNodeNeighbors
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FName> Neighbors;

	// operator TArray<FName>() const  { return Neighbors; }
	// void operator=(const TArray<FName>& Other) { Neighbors = Other; }
};

USTRUCT(BlueprintType)
struct FGraphEdgeID
{
	GENERATED_BODY()

	FGraphEdgeID() = default;

	FGraphEdgeID(const FName& InStartID, const FName& InEndID, const bool bIsDirected)
		: StartNodeID(InStartID), EndNodeID(InEndID), bIsOrderSensitive(bIsDirected)
	{
		
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName StartNodeID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FName EndNodeID;

	FGraphEdgeID Reverse() const
	{
		return FGraphEdgeID(EndNodeID, StartNodeID, bIsOrderSensitive);
	}

	bool operator == (const FGraphEdgeID& Other) const
	{
		if (bIsOrderSensitive != Other.bIsOrderSensitive) return false;

		if (bIsOrderSensitive && Other.bIsOrderSensitive)
			return StartNodeID == Other.StartNodeID &&
				EndNodeID == Other.EndNodeID;
		
		return (StartNodeID == Other.StartNodeID && EndNodeID == Other.EndNodeID) ||
				(StartNodeID == Other.EndNodeID && EndNodeID == Other.StartNodeID);
	}

	FGraphEdgeID GetNormalizeEdgeID() const
	{
		if (bIsOrderSensitive) return *this;

		if (StartNodeID.LexicalLess(EndNodeID)) return *this;

		return this->Reverse();
	}

	FORCEINLINE bool GetIsOrderSensitive() const { return bIsOrderSensitive; }

	friend struct FGraphEdge;

protected:
	bool bIsOrderSensitive;

private:

	friend uint32 GetTypeHash(const FGraphEdgeID& EdgeID)
	{
		return HashCombine(HashCombine(GetTypeHash(EdgeID.StartNodeID), GetTypeHash(EdgeID.EndNodeID)),
			GetTypeHash(EdgeID.bIsOrderSensitive));
	}
};

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
			TEXT("GRAPH_NODE_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
	}
	
	bool operator == (const FGraphNode& Other) const
	{
		return NodeID == Other.NodeID;
	}

	bool operator != (const FGraphNode& Other) const
	{
		return !(*this == Other);
	}

private:

	friend uint32 GetTypeHash(const FGraphNode& Node)
	{
		return GetTypeHash(Node.NodeID);
	}
};

USTRUCT(BlueprintType)
struct FGraphEdge
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FGraphEdgeID EdgeID;
	
	FGraphEdge() = default;

	FGraphEdge(const FGraphEdgeID& InEdgeID)
		: EdgeID(InEdgeID)
	{
		
	}

	bool IsConnectTo(const FName NodeID) const
	{
		return EdgeID.StartNodeID == NodeID || EdgeID.EndNodeID == NodeID;
	}
};

USTRUCT(BlueprintType)
struct FUndirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FUndirectedEdge() = default;

	FUndirectedEdge(const FName& StartNodeID, const FName& EndNodeID)
	{
		EdgeID = FGraphEdgeID(StartNodeID, EndNodeID, false);
	}

	FUndirectedEdge(const FGraphEdgeID& InEdgeID)
	{
		EdgeID = FGraphEdgeID(InEdgeID.StartNodeID, InEdgeID.EndNodeID, false);
	}

	bool operator == (const FGraphEdge& Other) const
	{
		return EdgeID == Other.EdgeID;
	}

	bool operator != (const FGraphEdge& Other) const
	{
		return !(*this == Other);
	}

private:

	friend uint32 GetTypeHash(const FGraphEdge& Edge)
	{
		const auto ID = Edge.EdgeID.GetNormalizeEdgeID();

		return GetTypeHash(ID);
	}
};

USTRUCT(BlueprintType)
struct FDirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FDirectedEdge() = default;

	FDirectedEdge(const FName& InStartNodeID, const FName& InEndNodeID)
	{
		EdgeID = FGraphEdgeID(InStartNodeID, InEndNodeID, true);
	}

	FDirectedEdge(const FGraphEdgeID& InEdgeID)
	{
		EdgeID = FGraphEdgeID(InEdgeID.StartNodeID, InEdgeID.EndNodeID, true);
	}

	bool operator == (const FDirectedEdge& Other) const
	{
		return EdgeID == Other.EdgeID;
	}

	bool operator == (const FGraphEdgeID& Other) const
	{
		return EdgeID == Other;
	}

	bool operator != (const FDirectedEdge& Other) const
	{
		return !(*this == Other);
	}

private:

	friend uint32 GetTypeHash(const FDirectedEdge& Edge)
	{
		return GetTypeHash(Edge.EdgeID);
	}
};

USTRUCT(BlueprintType)
struct FUndirectedWeightedEdge : public FUndirectedEdge
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	float Weight = 0.f;

	FUndirectedWeightedEdge() = default;

	FUndirectedWeightedEdge(const FName& InStartNodeID, const FName& InEndNodeID, const float InWeight)
		: Weight(InWeight)
	{
		EdgeID = FGraphEdgeID(InStartNodeID, InEndNodeID, false);
	}

	FUndirectedWeightedEdge(const FGraphEdgeID& InEdgeID)
	{
		EdgeID = FGraphEdgeID(InEdgeID.StartNodeID, InEdgeID.EndNodeID, false);
		Weight = 1.0f;
	}

	bool operator == (const FUndirectedWeightedEdge& Other) const
	{
		return EdgeID == Other.EdgeID &&
			FMath::IsNearlyEqual(Weight, Other.Weight);
	}
	
	bool operator != (const FUndirectedWeightedEdge& Other) const
	{
		return !(*this == Other);
	}

private:

	friend uint32 GetTypeHash(const FUndirectedWeightedEdge& Edge)
	{
		const auto ID = Edge.EdgeID.GetNormalizeEdgeID();
		const int32 Quantized = FMath::RoundToInt32(Edge.Weight * 1000.f);
		
		return HashCombine(GetTypeHash(ID), GetTypeHash(Quantized));
	}
};

USTRUCT(BlueprintType)
struct FDirectedWeightedEdge : public FDirectedEdge
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	float Weight = 0.f;
	
	FDirectedWeightedEdge() = default;

	FDirectedWeightedEdge(const FName& InStartNodeID, const FName& InEndNodeID, const float InWeight)
		: Weight(InWeight)
	{
		EdgeID = FGraphEdgeID(InStartNodeID, InEndNodeID, true);
	}

	FDirectedWeightedEdge(const FGraphEdgeID& InEdgeID)
	{
		EdgeID = FGraphEdgeID(InEdgeID.StartNodeID, InEdgeID.EndNodeID, true);
		Weight = 1.0f;
	}

	bool operator == (const FDirectedWeightedEdge& Other) const
	{
		return EdgeID == Other.EdgeID &&
				FMath::IsNearlyEqual(Weight, Other.Weight);
	}

	bool operator != (const FDirectedWeightedEdge& Other) const
	{
		return !(*this == Other);
	}

private:

	friend uint32 GetTypeHash(const FDirectedWeightedEdge& Edge)
	{
		const int32 Quantized = FMath::RoundToInt32(Edge.Weight * 1000.f);
		
		return HashCombine(GetTypeHash(Edge.EdgeID), GetTypeHash(Quantized));
	}
};



////////////////////////////////////////////////////////////////////////////////////////Custom Node/Edge
///
///

USTRUCT(BlueprintType)
struct FPathNode : public FGraphNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	TWeakObjectPtr<AActor> PathNode;
	
};



