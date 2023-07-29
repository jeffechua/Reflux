#pragma once
#include "IMouseInputStrategy.h"
#include "Engine/include/Design/Design.h"

namespace Reflux::UI {

	class DesignEditorWindow;

	using namespace Reflux::Engine::Design;

	class DesignMouseInputStrategy : public IMouseInputStrategy {
	public:
		DesignMouseInputStrategy(DesignEditorWindow& editorWindow);
		IMouseInputStrategy& OnMouseClicked(ImVec2 screenPosition) override;
		IMouseInputStrategy& OnMouseReleased(ImVec2 screenPosition) override;
		IMouseInputStrategy& OnMouseDragging(ImVec2 screenPosition) override;
		IMouseInputStrategy& OnMouseDown(ImVec2 screenPosition) override;
		IMouseInputStrategy& OnMouseHover(ImVec2 screenPosition) override;
	private:
		DesignEditorWindow& editorWindow_;
		Design& design;
	};
}