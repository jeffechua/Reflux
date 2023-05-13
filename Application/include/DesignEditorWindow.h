#pragma once

#include "Engine/include/Design/Design.h"
#include "IRenderable.h"

using namespace Reflux::Engine::Design;

namespace Reflux::UI::Editor {

	class DesignEditorWindow : public IRenderable {
	public:
		Design& design;

		DesignEditorWindow(Design& design);
		bool show() override;
	};

}