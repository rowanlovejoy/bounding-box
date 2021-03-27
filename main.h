#pragma once

struct GLFWwindow;

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void keyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
