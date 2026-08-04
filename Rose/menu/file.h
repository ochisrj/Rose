#pragma once
#include "imgui.h"

class FileMenu
{
public:
	static void DrawMenu();
private:
	static bool LoadFile;
	static bool SaveFile;
	static bool SaveAsFile;

	static bool NewFile;
	static bool NewFolderFile;

	static bool ExitFile;
};
