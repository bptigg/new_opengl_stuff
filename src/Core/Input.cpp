#include "Input.h"

#include "Application.h" 

bool Input::Is_Key_Pressed(const int key)
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().Get_window().Get_Native_Window()); 
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS;
}

bool Input::Is_MouseButton_Pressed(int button)
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().Get_window().Get_Native_Window()); 
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::Get_Mouse_Position()
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().Get_window().Get_Native_Window());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float Input::Get_Mouse_X()
{
    return Get_Mouse_Position().x;
}

float Input::Get_Mouse_Y()
{
    return Get_Mouse_Position().y;
}

glm::vec2 Input::Get_Window_Size()
{
    float height = Application::Get().Get_window().Get_Height();
    float width = Application::Get().Get_window().Get_Width();

    return { width, height };
}
