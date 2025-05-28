#pragma once

#include "EditorPanel.h"

class SceneViewportPanel : public EditorPanel {
public:
	~SceneViewportPanel();

	void OnImGuiRender();
};