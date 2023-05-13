#pragma once

#include <unordered_map>
#include <memory>
#include "DesignEditorWindow.h"
#include "IRenderable.h"

using namespace Reflux::Engine::Design;

namespace Reflux::UI {

	class ApplicationMain : public IRenderable {
	public:
		ApplicationMain();
		bool show() override;

	private:
		std::unordered_map<std::unique_ptr<Design>, std::unique_ptr<Editor::DesignEditorWindow>> designs{};
	};

}