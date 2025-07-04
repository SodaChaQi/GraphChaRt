// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphElements.h"


bool UGraphNode::SetNodeInfo(const FGraphNodeInfo& GraphNodeInfo)
{
	NodeInfo = GraphNodeInfo;
	return true;
}

bool UGraphEdge::SetEdgeInfo(const FGraphEdgeInfo& GraphEdgeInfo)
{
	EdgeInfo = GraphEdgeInfo;
	return true;
}
