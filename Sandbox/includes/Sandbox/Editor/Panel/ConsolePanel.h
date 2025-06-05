#pragma once

#include "IEditorPanel.h"

class ConsolePanel : public IEditorPanel {
public:
    ~ConsolePanel();
    void OnImGuiRender() override;
};