#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"

struct GLFWwindow;

namespace ly {

class LIGHTYEAR_API WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    virtual void OnUpdate() override;
    inline unsigned int GetWidth() const override { return m_Data.Width; }
    inline unsigned int GetHeight() const override { return m_Data.Height; }

    // Window Attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }

    virtual void SetVSync(bool isEnabled) override;
    virtual bool IsVSync() const override;
    virtual void* GetNativeWindow() const override { return static_cast<void*>(m_Window); }

protected:
    virtual void Init(const WindowProps& props);
    virtual void ShutDown();

private:
    GLFWwindow* m_Window;

    struct WindowsData {
        std::string Title{ "Demo" };
        unsigned int Height{ 0 };
        unsigned int Width{ 0 };
        bool VSync{ true };

        EventCallbackFn EventCallback;
    };

    WindowsData m_Data{};
};

}  // namespace ly
