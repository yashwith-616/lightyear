#pragma once

#include "EditorPanel.h"

class ConsolePanel : public EditorPanel {
public:
    ConsolePanel();
    void OnImGuiRender() override;

    void AddLogLine(const std::string& line);
    void Clear();

private:
    std::vector<std::string> m_LogBuffer;
    bool m_AutoScroll = true;
};