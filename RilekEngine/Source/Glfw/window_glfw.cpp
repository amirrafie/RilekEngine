#include "stdafx.h"
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//
//#include "window_glfw.h"
//
//#include "Windows/window_system.h"
//
//namespace Rilek::Glfw
//{
//    window_glfw::window_glfw()
//        : mp_window(NULL)
//    {
//    }
//
//    bool window_glfw::init(Rilek::Window::window_system& t_window_system)
//    {
//        mp_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//        if (mp_window == NULL)
//        {
//            std::cout << "Failed to create GLFW window" << std::endl;
//            glfwTerminate();
//            return true;
//        }
//        glfwMakeContextCurrent(mp_window);
//        return true;
//    }
//
//	bool window_glfw::update(Rilek::Window::window_system& t_window_system)
//	{
//        if (glfwWindowShouldClose(mp_window))
//            return false;
//        glfwSwapBuffers(mp_window);
//        glfwPollEvents();
//        return true;
//	}
//
//}