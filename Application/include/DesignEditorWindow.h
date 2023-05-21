#pragma once

#include <imgui.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Geometry/transform.h"
#include "Engine/include/Geometry/vector2.h"
#include "IRenderable.h"

using namespace Reflux::Engine::Design;
using namespace Reflux::Engine::Geometry;

namespace Reflux::UI::Editor {

	class DesignEditorWindow : public IRenderable {
	public:
		Design& design;
		vector2<double> gridStep;
		transform<ImVec2, vector2<double>> view;

		DesignEditorWindow(Design& design);
		bool show() override;
	private:
		ImVec2 canvasP0;
		ImVec2 canvasP1;
		ImVec2 to_screen_space(vector2<double> designPosition);
		vector2<double> to_design_space(ImVec2 screenPosition);
		void draw_content(ImDrawList& drawList);
	};

}