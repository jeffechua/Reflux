#include "Application/pch.h"
#include "Engine/include/Design/Design.h"
#include "Application/include/ApplicationMain.h"

namespace Reflux::UI {

	ApplicationMain::ApplicationMain() {
		std::unique_ptr<Design> design = std::make_unique<Design>();
		std::unique_ptr<DesignEditorWindow> window = std::make_unique<DesignEditorWindow>(*design);
		designs.emplace(std::move(design), std::move(window));
	}

	bool ApplicationMain::show() {
		std::vector<const std::unique_ptr<Design>*> toRemove(0);
		for (auto& entry : designs) {
			std::unique_ptr<DesignEditorWindow>& window = entry.second;
			if (!window->show()) {
				toRemove.push_back(&entry.first);
			}
		}
		for (auto removed : toRemove) {
			designs.erase(*removed);
		}
		return true;
	}

}