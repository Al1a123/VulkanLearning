#include <vulkan\vulkan.h>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <glfw3.h>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

using namespace std;

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

class TringleApplication
{
public:

	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}

private:
    VDeleter<VkInstance> instance{ vkDestroyInstance };

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
		createInstance();
	}
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void createInstance()
	{
		VkApplicationInfo appinfo = {};
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pApplicationName = "Hello Tringle";
		appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.pEngineName = "No Engine";
		appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.apiVersion = VK_API_VERSION;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appinfo;
 
		unsigned int glfwextensioncount = 0;
		const char** glfwextensionnames = glfwGetRequiredInstanceExtensions(&glfwextensioncount);
		createInfo.enabledExtensionCount = glfwextensioncount;
		createInfo.ppEnabledExtensionNames = glfwextensionnames;
		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw runtime_error("vulkan couldn't create instance");

        uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		vector<VkExtensionProperties> _vk_extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, _vk_extensions.data());
		cout << "Required vulkan extensions :" << endl;
		for (const auto& extension : _vk_extensions  )
			cout << "\t" << extension.extensionName << endl;



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