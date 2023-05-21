#include "Application/pch.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Application/include/DesignEditorWindow.h"

namespace Reflux::UI::Editor {

	DesignEditorWindow::DesignEditorWindow(Design& design) : design(design), gridStep({ 1.0f, 1.0f }), view({ {0.0f, 0.0f}, {0.1f, 0.1f} }
	) {}

	bool DesignEditorWindow::show() {
		ImGui::Begin(design.name.c_str());

		static ImVector<ImVec2> points;
		static ImVec2 scrolling(0.0f, 0.0f);
		static bool adding_line = false;

		ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

		// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
		// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
		// To use a child window instead we could use, e.g:
		//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
		//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
		//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
		//      ImGui::PopStyleColor();
		//      ImGui::PopStyleVar();
		//      [...]
		//      ImGui::EndChild();

		// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
		canvasP0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
		if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;
		canvasP1 = ImVec2(canvasP0.x + canvasSize.x, canvasP0.y + canvasSize.y);

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(canvasP0, canvasP1, IM_COL32(50, 50, 50, 255));
		drawList->AddRect(canvasP0, canvasP1, IM_COL32(255, 255, 255, 255));

		// This will catch our interactions
		ImGui::InvisibleButton("canvas", canvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held

		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
			view.offset -= vector2<float>{io.MouseDelta.x, io.MouseDelta.y};
		}

		if (is_hovered) {
			vector2<double> oldMouseDesignPos = to_design_space(io.MousePos);
			view.scale /= exp(io.MouseWheel / 10);
			if (view.scale.x > 1) view.scale = { 1.0f, 1.0f };
			if (view.scale.y < 0.01) view.scale = { 0.01f, 0.01f };
			vector2<double> newMouseDesignPos = to_design_space(io.MousePos);
			vector2<double> designSpaceDelta = newMouseDesignPos - oldMouseDesignPos;
			ImVec2 screenSpaceDelta = view.inv_transform_vector(designSpaceDelta);
			view.offset -= vector2<float>{screenSpaceDelta.x, screenSpaceDelta.y};
		}

		// Context menu (under default mouse threshold)
		/*ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
			ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopup("context")) {
			if (adding_line)
				points.resize(points.size() - 2);
			adding_line = false;
			if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
			if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
			ImGui::EndPopup();
		}*/

		// Draw content
		draw_content(*drawList);

		drawList->PopClipRect();


		ImGui::End();
		return true;
	}

	ImVec2 DesignEditorWindow::to_screen_space(vector2<double> designPosition) {
		return view.inv_transform_position(designPosition) + (canvasP0 + canvasP1) / 2;
	}
	vector2<double> DesignEditorWindow::to_design_space(ImVec2 screenPosition) {
		return view.transform_position(screenPosition - (canvasP0 + canvasP1) / 2);
	}

	void DesignEditorWindow::draw_content(ImDrawList& drawList) {
		drawList.PushClipRect(canvasP0, canvasP1, true);
		const vector2<double> designP0 = to_design_space(canvasP0);
		const vector2<double> designP1 = to_design_space(canvasP1);
		for (double x = ceil(designP0.x); x < designP1.x; x += gridStep.x) {
			const float gridLineScreenX = to_screen_space({ x, 0 }).x;
			drawList.AddLine(ImVec2(gridLineScreenX, canvasP0.y), ImVec2(gridLineScreenX, canvasP1.y), IM_COL32(200, 200, 200, 40));
		}
		for (double y = ceil(designP0.y); y < designP1.y; y += gridStep.y) {
			const float gridLineScreenY = to_screen_space({ 0, y }).y;
			drawList.AddLine(ImVec2(canvasP0.x, gridLineScreenY), ImVec2(canvasP1.x, gridLineScreenY), IM_COL32(200, 200, 200, 40));
		}

		drawList.AddRectFilled(to_screen_space({ 1, 1 }), to_screen_space({ 5, 8 }), IM_COL32(255, 255, 255, 255));
	}



}