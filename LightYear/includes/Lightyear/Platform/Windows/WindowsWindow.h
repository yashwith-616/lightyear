#include "../Window.h"

class GLFWwindow;

namespace ly {

class LIGHTYEAR_API WindowsWindow : public Window {
public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;
	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// Window Attributes
	inline void SetEventCallback(const EventCallbackFn& callback) override {
		m_Data.EventCallback = callback;
	}

	void SetVSync(bool isEnabled) override;
	bool IsVSync() const override;

private:
	virtual void Init(const WindowProps& props);
	virtual void ShutDown();

private:
	GLFWwindow* m_Window;

	struct WindowsData {
		std::string Title;
		unsigned int Height, Width;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowsData m_Data;
};

}
