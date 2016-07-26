#include <vulkan\vulkan.h>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <glfw3.h>

using namespace std;

class TringleApplication
{
public:

	const int WIDTH = 800;
	const int HEIGHT = 600;

	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}

private:

	GLFWwindow* window;

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Learning", nullptr, nullptr);

	}

	void initVulkan()
	{

	}
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
};

template <typename T>
class VDeleter {
public:
	VDeleter() : VDeleter([](T _) {}) {}

	VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef) {
		object = VK_NULL_HANDLE;
		this->deleter = [=](T obj) { deletef(obj, nullptr); };
	}

	VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef) {
		object = VK_NULL_HANDLE;
		this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
	}

	VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef) {
		object = VK_NULL_HANDLE;
		this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
	}

	~VDeleter() {
		cleanup();
	}

	T* operator &() {
		cleanup();
		return &object;
	}

	operator T() const {
		return object;
	}

private:
	T object;
	std::function<void(T)> deleter;

	void cleanup() {
		if (object != VK_NULL_HANDLE) {
			deleter(object);
		}
		object = VK_NULL_HANDLE;
	}
};

int main()
{
	TringleApplication app;

	try{
		app.run();
	}
	catch (runtime_error& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

	
}