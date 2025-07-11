// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"

class FPathGraphEditorCommands : public TCommands<FPathGraphEditorCommands>
{
public:
	FPathGraphEditorCommands()
	: TCommands<FPathGraphEditorCommands>(TEXT("PathGraphEditor"), NSLOCTEXT("Contexts", "PathGraphEditor", "Path Graph Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > SaveGraph;
	TSharedPtr<FUICommandInfo> AddNode;
	TSharedPtr<FUICommandInfo> DeleteSelectedNodes;
	TSharedPtr<FUICommandInfo> RefreshGraph;

};