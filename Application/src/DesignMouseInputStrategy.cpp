#include "Application/pch.h"
#include "../include/DesignMouseInputStrategy.h"
#include "../include/DesignEditorWindow.h"
#include "Engine/include/Design/Design.h"

using namespace Reflux::Engine::Design;

namespace Reflux::UI {
	DesignMouseInputStrategy::DesignMouseInputStrategy(DesignEditorWindow& editorWindow) : editorWindow_(editorWindow), design(editorWindow.design) {}
	IMouseInputStrategy& DesignMouseInputStrategy::OnMouseClicked(ImVec2 screenPosition) {
		return *this;
	}
	IMouseInputStrategy& DesignMouseInputStrategy::OnMouseReleased(ImVec2 screenPosition) {
		return *this;
	}
	IMouseInputStrategy& DesignMouseInputStrategy::OnMouseDragging(ImVec2 screenPosition) {
		return *this;
	}
	IMouseInputStrategy& DesignMouseInputStrategy::OnMouseDown(ImVec2 screenPosition) {
		return *this;
	}
	IMouseInputStrategy& DesignMouseInputStrategy::OnMouseHover(ImVec2 screenPosition) {
		return *this;
	}
}