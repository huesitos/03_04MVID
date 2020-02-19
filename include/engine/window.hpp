#ifndef __WINDOW_H__
#define __WINDOW_H__

class GLFWwindow;

class Window {
public:
	static Window* instance();

	bool alive() const;
	void frame() const;

private:
	Window();
	~Window();

	Window(const Window&) = default;
	Window(Window&&) = default;
	Window& operator=(const Window&) = default;
	Window& operator=(Window&&) = default;

	GLFWwindow* window_;
};

#endif