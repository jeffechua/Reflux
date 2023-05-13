#include "Application/pch.h"
#include "imgui.h"
#include "Application/include/DesignEditorWindow.h"

namespace Reflux::UI::Editor {
	
	DesignEditorWindow::DesignEditorWindow(Design& design): design(design) {
	}
	bool DesignEditorWindow::show() {
		ImGui::Begin(design.name.c_str());
		ImGui::End();
		return true;
	}

}