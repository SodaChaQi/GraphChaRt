// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "GraphChaRtEditorStyle.h"

class FGraphChaRtEditorCommands : public TCommands<FGraphChaRtEditorCommands>
{
public:

	FGraphChaRtEditorCommands()
		: TCommands<FGraphChaRtEditorCommands>(TEXT("GraphChaRtEditor"), NSLOCTEXT("Contexts", "GraphChaRtEditor", "GraphChaRtEditor Plugin"), NAME_None, FGraphChaRtEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};