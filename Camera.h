#ifndef GAME_CAMERA_h
#define GAME_CAMERA_h

// GLEW/GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
// OTHER
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

enum MOVE_DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Camera {
private:
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float zoom;

	float MovementSpeed;
	bool constrainPitch;
public:

	Camera(int w = 600, int h = 600,
		glm::vec3 position = glm::vec3(0.0f,0.0f,3.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float YAW = 90.0f, float PITCH = 0.0f):WINDOW_WIDTH(w), WINDOW_HEIGHT(h){
		
		cameraPos = position;
		cameraUp = up;

		// yaw is initialized to -90.0 degrees since a yaw of 0.0 
		// results in a direction vector pointing to the right 
		// so we initially rotate a bit to the left.
		yaw = YAW;
		pitch = PITCH;
		zoom = 45.0f;

		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		MovementSpeed = 2.5f;
		constrainPitch = true;

		updateVector();
	};

	void moveForward(float cameraSpeed) {
		cameraPos += cameraSpeed * this->cameraFront;
	};

	void moveBack(float cameraSpeed) {
		cameraPos -= cameraSpeed * this->cameraFront;
	};

	void moveLeft(float cameraSpeed) {
		cameraPos -= this->cameraRight * cameraSpeed;
	};

	void moveRight(float cameraSpeed) {
		cameraPos += this->cameraRight * cameraSpeed;
	};

	void ProcessKeyboard(MOVE_DIRECTION direction, float deltaTime) {
		float cameraSpeed = this->MovementSpeed * deltaTime;
		if (direction == UP) {
			this->moveForward(cameraSpeed);
		}
		if (direction == DOWN) {
			this->moveBack(cameraSpeed);
		}
		if (direction == LEFT) {
			this->moveLeft(cameraSpeed);
		}
		if (direction == RIGHT) {
			this->moveRight(cameraSpeed);
		}
		cameraPos.y = 5.0f;
	};

	void updateVector() {
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		
		this->cameraFront = glm::normalize(front);
		this->cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
		this->cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}

	void ProcessMouseMove(double xoffset, double yoffset) {
		
		// 设置敏感度
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		// 改变俯仰角
		this->yaw += xoffset;
		this->pitch += yoffset;

		if (this->constrainPitch) {
			if (this->pitch > 89.0f) {
				this->pitch = 89.0f;
			}
			if (this->pitch < -89.0f) {
				this->pitch = -89.0f;
			}
		}
		updateVector();
	};

	void ProcessMouseScroll(float yoffset) {
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
			this->zoom -= yoffset;
		if (this->zoom <= 1.0f)
			this->zoom = 1.0f;
		if (this->zoom >= 45.0f)
			this->zoom = 45.0f;
	}

	glm::mat4 getView() const {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::vec3 getPosition() const {
		return this->cameraPos;
	}

	float getZoom() const {
		return this->zoom;
	}
};

#endif