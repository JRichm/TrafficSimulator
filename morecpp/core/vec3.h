#pragma once


class Vector3 {
public:
	float x, y, z;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	float dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3 cross(const Vector3& other) const {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	Vector3 normalized() const {
		float len = length();
		if (len <= 0.0f) return Vector3();
		return Vector3(x / len, y / len, z / len);
	}

	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}
};