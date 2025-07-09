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

	FGraphEdgeID(const FName& InStartID, const FName& InEndID, const bool bInIsOrderSensitive)
		: StartNodeID(InStartID), EndNodeID(InEndID), bIsOrderSensitive(bInIsOrderSensitive)
	{}

	FGraphEdgeID(const FName& InStartID, const FName& InEndID)
		: StartNodeID(InStartID), EndNodeID(InEndID)
	{}
	
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

	bool operator != (const FGraphEdgeID& Other) const
	{
		return !(*this == Other);
	}

	FGraphEdgeID GetNormalizeEdgeID() const
	{
		if (bIsOrderSensitive) return *this;

		if (StartNodeID.LexicalLess(EndNodeID)) return *this;

		return this->Reverse();
	}

	FORCEINLINE bool GetIsOrderSensitive() const { return bIsOrderSensitive; }

	friend FArchive& operator<<(FArchive& Ar, FGraphEdgeID& Element)
	{
		Ar << Element.StartNodeID << Element.EndNodeID << Element.bIsOrderSensitive;
		return Ar;
	}

protected:
	bool bIsOrderSensitive = false;

private:

	friend uint32 GetTypeHash(const FGraphEdgeID& EdgeID)
	{
		return HashCombine(HashCombine(GetTypeHash(EdgeID.StartNodeID), GetTypeHash(EdgeID.EndNodeID)),
			GetTypeHash(EdgeID.bIsOrderSensitive));
	}
};

////////////////////////////////////////////////////////////////////////////////////////
///以下USTRUCT都不可实例化，若要使用请按需继承
///
///若要继承后实例化结构体：
///至少实现默认构造函数和隐式转换构造函数
///需重载 == 与 != 实现查找
///需实现 GetTypeHash()，以便 Hash 容器寻找对比
///需重载<<操作符实现序列化
///

USTRUCT(BlueprintType)
struct FGraphNode
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Node")
	FName NodeID;

	FGraphNode()
		: NodeID(NAME_None)
	{}

protected:

	FGraphNode(const FName& InNodeID)
		: NodeID(InNodeID)
	{}
	
public:

	FORCEINLINE static FName NodeGUID()
	{
		return  FName(FString::Printf(
			TEXT("GRAPH_NODE_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
	}
};

USTRUCT(BlueprintType)
struct FGraphEdge
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	FGraphEdgeID EdgeID;

	FGraphEdge(const FName& InStartID, const FName& InEndID, const bool bInIsOrderSensitive)
		: EdgeID(InStartID, InEndID, bInIsOrderSensitive)
	{}

public:
	
	FGraphEdge()
		: EdgeID(NAME_None, NAME_None)
	{}

	FGraphEdgeID GetID() const { return EdgeID; }

	bool IsConnectTo(const FName NodeID) const
	{
		return EdgeID.StartNodeID == NodeID || EdgeID.EndNodeID == NodeID;
	}
};

USTRUCT(BlueprintType)
struct FUndirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FUndirectedEdge()
		: FGraphEdge(NAME_None, NAME_None, false)
	{}

protected:

	FUndirectedEdge(const FName InStartID, const FName& InEndID)
		: FGraphEdge(InStartID, InEndID, false)
	{
	}
};

USTRUCT(BlueprintType)
struct FDirectedEdge : public FGraphEdge
{
	GENERATED_BODY()

	FDirectedEdge()
		: FGraphEdge(NAME_None, NAME_None, true)
	{}

protected:
	
	FDirectedEdge(const FName& InStartID, const FName& InEndID)
		: FGraphEdge(InStartID, InEndID, true)
	{}
};

USTRUCT(BlueprintType)
struct FUndirectedWeightedEdge : public FUndirectedEdge
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	float Weight = 1.f;

	FUndirectedWeightedEdge()
		: FUndirectedEdge(NAME_None, NAME_None)
	{}

protected:

	FUndirectedWeightedEdge(const FName& InStartID, const FName& InEndID, const float& InWeight)
		: FUndirectedEdge(InStartID, InEndID), Weight(InWeight)
	{}
};

USTRUCT(BlueprintType)
struct FDirectedWeightedEdge : public FDirectedEdge
{
	GENERATED_BODY()

	FDirectedWeightedEdge()
	: FDirectedEdge(NAME_None, NAME_None)
	{}

	FORCEINLINE void SetWeight(const float InWeight) { FixedWeight = FMath::RoundToInt64(InWeight * FIXED_WEIGHT_SCALE); }
	FORCEINLINE float GetWeight() const { return static_cast<float>(FixedWeight) / static_cast<float>(FIXED_WEIGHT_SCALE); }
	FORCEINLINE double GetPreciseWeight() const { return static_cast<double>(FixedWeight) / FIXED_WEIGHT_SCALE; }

protected:

	static constexpr int64 FIXED_WEIGHT_SCALE = 1LL << 16;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph|Edge")
	int64 FixedWeight = 1;

	FDirectedWeightedEdge(const FName& InStartID, const FName& InEndID, const float& InWeight)
		: FDirectedEdge(InStartID, InEndID), FixedWeight(FMath::RoundToInt64(InWeight * FIXED_WEIGHT_SCALE))
	{}

	FDirectedWeightedEdge(const FName& InStartID, const FName& InEndID)
		: FDirectedEdge(InStartID, InEndID)
	{}
};



////////////////////////////////////////////////////////////////////////////////////////
///以上USTRUCT都不可实例化，若要使用请按需继承
///
///若要继承后实例化结构体：
///至少实现默认构造函数和隐式转换构造函数
///需重载 == 与 != 实现查找
///需实现 GetTypeHash()，以便 Hash 容器寻找对比
///需重载<<操作符实现序列化
///

