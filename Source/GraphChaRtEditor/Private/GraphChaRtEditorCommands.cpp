// Copyright Epic Games, Inc. All Rights Reserved.

#include "GraphChaRtEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGraphChaRtEditorModule"

void FGraphChaRtEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GraphChaRtEditor", "Bring up GraphChaRtEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
