#pragma once

#include <imgui.h>

namespace Reflux::UI {

	class IMouseInputStrategy {
	public:
		// Trigger first
		virtual IMouseInputStrategy& OnMouseClicked(ImVec2 screenPosition) = 0;
		// Trigger second
		virtual IMouseInputStrategy& OnMouseReleased(ImVec2 screenPosition) = 0;
		// Third, either (mutually exclusive)
		virtual IMouseInputStrategy& OnMouseDragging(ImVec2 screenPosition) = 0;
		// or
		virtual IMouseInputStrategy& OnMouseDown(ImVec2 screenPosition) = 0;
		// or
		virtual IMouseInputStrategy& OnMouseHover(ImVec2 screenPosition) = 0;
	};

}