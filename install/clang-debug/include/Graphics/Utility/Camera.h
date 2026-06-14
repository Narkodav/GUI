#pragma once
#include "Graphics/Common.h"

namespace Graphics::Utility {

	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	class CameraBase
	{
	protected:
		glm::vec3 m_position;
		glm::vec3 m_camFront; //points where the camera is looking
		glm::vec3 m_camUp; // points upwards from the camera, depends on pitch and yaw
		glm::vec3 m_camRight; // points to the right from the camera, can be made as cross product of world up and forward or cam up and front vectors
		glm::vec3 m_worldUpVector; // the upwards direction of the world space
		glm::vec3 m_camForward; // points in the forward direction depending on the world up vector and yaw		
		float m_pitch;
		float m_yaw;
		glm::mat4 m_view;
		glm::mat4 m_viewWithoutTransposition;
		glm::mat4 m_projection;
		
	public:

		CameraBase(glm::mat4 projection) noexcept;
		CameraBase(glm::vec3 worldUpVector, glm::vec3 position, float pitch, float yaw, glm::mat4 projection) noexcept;

		CameraBase(const CameraBase& other) noexcept
		{
			this->m_view = other.m_view;
			this->m_viewWithoutTransposition = other.m_viewWithoutTransposition;
			this->m_projection = other.m_projection;
			this->m_position = other.m_position;
			this->m_camFront = other.m_camFront;
			this->m_camRight = other.m_camRight;
			this->m_camUp = other.m_camUp;
			this->m_camForward = other.m_camForward;
			this->m_worldUpVector = other.m_worldUpVector;
			this->m_yaw = other.m_yaw;
			this->m_pitch = other.m_pitch;
		};

		CameraBase& operator=(const CameraBase& other) noexcept
		{
			if (this == &other)
				return *this;

			this->m_view = other.m_view;
			this->m_viewWithoutTransposition = other.m_viewWithoutTransposition;
			this->m_projection = other.m_projection;
			this->m_position = other.m_position;
			this->m_camFront = other.m_camFront;
			this->m_camRight = other.m_camRight;
			this->m_camUp = other.m_camUp;
			this->m_camForward = other.m_camForward;
			this->m_worldUpVector = other.m_worldUpVector;
			this->m_yaw = other.m_yaw;
			this->m_pitch = other.m_pitch;
			return *this;
		};

		CameraBase(CameraBase&& other) noexcept
		{
			this->m_view = std::exchange(other.m_view, glm::mat4(0.f));
			this->m_viewWithoutTransposition = std::exchange(other.m_viewWithoutTransposition, glm::mat4(0.f));
			this->m_projection = std::exchange(other.m_projection, glm::mat4(0.f));
			this->m_position = std::exchange(other.m_position, glm::vec3(0.f));
			this->m_camFront = std::exchange(other.m_camFront, glm::vec3(0.f));
			this->m_camRight = std::exchange(other.m_camRight, glm::vec3(0.f));
			this->m_camUp = std::exchange(other.m_camUp, glm::vec3(0.f));
			this->m_camForward = std::exchange(other.m_camForward, glm::vec3(0.f));
			this->m_worldUpVector = std::exchange(other.m_worldUpVector, glm::vec3(0.f));
			this->m_yaw = std::exchange(other.m_yaw, 0.f);
			this->m_pitch = std::exchange(other.m_pitch, 0.f);
		};

		CameraBase& operator=(CameraBase&& other) noexcept
		{
			if (this == &other)
				return *this;

			this->m_view = std::exchange(other.m_view, glm::mat4(0.f));
			this->m_viewWithoutTransposition = std::exchange(other.m_viewWithoutTransposition, glm::mat4(0.f));
			this->m_projection = std::exchange(other.m_projection, glm::mat4(0.f));
			this->m_position = std::exchange(other.m_position, glm::vec3(0.f));
			this->m_camFront = std::exchange(other.m_camFront, glm::vec3(0.f));
			this->m_camRight = std::exchange(other.m_camRight, glm::vec3(0.f));
			this->m_camUp = std::exchange(other.m_camUp, glm::vec3(0.f));
			this->m_camForward = std::exchange(other.m_camForward, glm::vec3(0.f));
			this->m_worldUpVector = std::exchange(other.m_worldUpVector, glm::vec3(0.f));
			this->m_yaw = std::exchange(other.m_yaw, 0.f);
			this->m_pitch = std::exchange(other.m_pitch, 0.f);

			return *this;
		};

		virtual ~CameraBase() = default;

		void move(glm::vec3 deltaPos);
		void rotate(float deltaPitch, float deltaYaw);

		//getters
		const glm::mat4& getView() const { return m_view; };
		const glm::mat4& getViewWithoutTransposition() const { return m_viewWithoutTransposition; };
		const glm::mat4& getProjection() const { return m_projection; };
		glm::mat3 getRotation() const { return glm::mat3(m_view); };
		const glm::vec3& getPosition() const { return m_position; };
		const glm::vec3& getCamFront() const { return m_camFront; };
		const glm::vec3& getCamRight() const { return m_camRight; };
		const glm::vec3& getCamUp() const { return m_camUp; };
		const glm::vec3& getCamForward() const { return m_camForward; };
		const glm::vec3& getWorldUp() const { return m_worldUpVector; };
		float getYaw() const { return m_yaw; };
		float getPitch() const { return m_pitch; };
	};

	template<ProjectionType projection>
	class Camera : public CameraBase {};

	template<>
	class Camera<ProjectionType::Perspective> : public CameraBase
	{
	protected:
		float m_fov;
		float m_aspectRatio;
		float m_near;
		float m_far;

		void updateProjection() {
			m_projection = glm::perspective(
				m_fov,
				m_aspectRatio,
				m_near,
				m_far
			);
			m_projection[1][1] *= -1;
		}

	public:
		Camera(float fov = glm::radians(90.f), float aspect = 16.0f / 9.0f,
			float nearPlane = 0.1f, float farPlane = 100.0f)
			: CameraBase(glm::perspective(fov, aspect, nearPlane, farPlane))
			, m_fov(fov)
			, m_aspectRatio(aspect)
			, m_near(nearPlane)
			, m_far(farPlane)
		{
		}

		Camera(glm::vec3 worldUpVector, glm::vec3 position, float pitch, float yaw,
			float fov = glm::radians(90.f), float aspect = 16.0f / 9.0f,
			float nearPlane = 0.1f, float farPlane = 100.0f)
			: CameraBase(worldUpVector, position, pitch, yaw,
				glm::perspective(fov, aspect, nearPlane, farPlane))
			, m_fov(fov)
			, m_aspectRatio(aspect)
			, m_near(nearPlane)
			, m_far(farPlane)
		{
		}

		void setFov(float fov) {
			m_fov = fov;
			updateProjection();
		}

		void setAspectRatio(float aspect) {
			m_aspectRatio = aspect;
			updateProjection();
		}

		void setNearPlane(float nearVal) {
			m_near = nearVal;
			updateProjection();
		}

		void setFarPlane(float farVal) {
			m_far = farVal;
			updateProjection();
		}

		float getFov() const { return m_fov; };
		float getAspectRatio() const { return m_aspectRatio; };
		float getNearPlane() const { return m_near; };
		float getFarPlane() const { return m_far; };
	};

	template<>
	class Camera<ProjectionType::Orthographic> : public CameraBase
	{
	protected:
		float m_left;
		float m_right;
		float m_bottom;
		float m_top;
		float m_near;
		float m_far;

		void updateProjection() {
			m_projection = glm::ortho(
				m_left, m_right,
				m_bottom, m_top,
				m_near, m_far
			);

			m_projection[1][1] *= -1;
		}

	public:

		Camera(glm::vec3 worldUpVector, glm::vec3 position, float pitch, float yaw,
			float left = -10.0f, float right = 10.0f,
			float bottom = -10.0f, float top = 10.0f,
			float nearPlane = 0.1f, float farPlane = 100.0f)
			: CameraBase(worldUpVector, position, pitch, yaw,
				glm::ortho(left, right, bottom, top, nearPlane, farPlane))
			, m_left(left)
			, m_right(right)
			, m_bottom(bottom)
			, m_top(top)
			, m_near(nearPlane)
			, m_far(farPlane)
		{
		}

		Camera(float left = -10.0f, float right = 10.0f,
			float bottom = -10.0f, float top = 10.0f,
			float nearPlane = 0.1f, float farPlane = 100.0f)
			: CameraBase(glm::ortho(left, right, bottom, top, nearPlane, farPlane))
			, m_left(left)
			, m_right(right)
			, m_bottom(bottom)
			, m_top(top)
			, m_near(nearPlane)
			, m_far(farPlane)
		{
		}

		void setOrthoSize(float width, float height) {
			m_left = -width / 2.0f;
			m_right = width / 2.0f;
			m_bottom = -height / 2.0f;
			m_top = height / 2.0f;
			updateProjection();
		}

		float getWidth() const { return m_right - m_left; }
		float getHeight() const { return m_top - m_bottom; }
	};

	using CameraPerspective = Camera<ProjectionType::Perspective>;
	using CameraOrtho = Camera<ProjectionType::Orthographic>;

} // namespace Graphics