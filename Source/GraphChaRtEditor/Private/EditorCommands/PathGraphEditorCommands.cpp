// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorCommands/PathGraphEditorCommands.h"


#define LOCTEXT_NAMESPACE "PathGraphEditorCommands"


void FPathGraphEditorCommands::RegisterCommands()
{
	// 注册保存命令
	UI_COMMAND(SaveGraph, "Save", "Save the current path graph", 
			   EUserInterfaceActionType::Button, 
			   FInputChord(EModifierKey::Control, EKeys::S));
    
	// 注册添加节点命令
	UI_COMMAND(AddNode, "Add Node", "Add a new path node", 
			   EUserInterfaceActionType::Button, 
			   FInputChord());
    
	// 注册删除节点命令
	UI_COMMAND(DeleteSelectedNodes, "Delete", "Delete selected nodes", 
			   EUserInterfaceActionType::Button, 
			   FInputChord(EKeys::Delete));
    
	// 注册刷新命令
	UI_COMMAND(RefreshGraph, "Refresh", "Refresh the path graph view", 
			   EUserInterfaceActionType::Button, 
			   FInputChord(EModifierKey::Control, EKeys::R));
    
	// 可以继续添加更多命令...
}




#undef LOCTEXT_NAMESPACE