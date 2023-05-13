#pragma once

namespace Reflux::UI {

	class IRenderable {
		/// <summary>
		/// Show some ImGui content.
		/// </summary>
		/// <returns>False to send an exit signal e.g. closing a window, deleting an item.</returns>
		virtual bool show() = 0;
	};

}