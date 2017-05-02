/************************************************************************ 
 * @description :  ���Դ�����������
 * @author		:  Oscar Shen
 * @creat 		:  2017��4��18��10:22:33
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LINALG_H_
#define ORION_LINALG_H_
#include <orion.h>
#include <core/geometry.h>
#include <util/logmanager.h>

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min


namespace orion {
	// dot
	template <typename T>
	inline T dot(const Vector3<T> &v1, const Vector3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	template <typename T>
	inline T dot(const Vector3<T> &v1, const Normal3<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	// cross
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}
	template <typename T>
	inline Vector3<T> cross(const Normal3<T> &v1, const Vector3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}
	template <typename T>
	inline Vector3<T> cross(const Vector3<T> &v1, const Normal3<T> &v2) {
		Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
		Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}


	// length square
	template <typename T>
	inline T lengthSquare(const Vector3<T> &v) { return dot(v, v); }
	template <typename T>
	inline T lengthSquare(const Normal3<T> &v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

	// length
	template <typename T>
	inline T length(const Vector3<T> &v) { return std::sqrt(lengthSquare(v)); }
	template <typename T>
	inline T length(const Normal3<T> &v) { return std::sqrt(lengthSquare(v)); }

	// normalize
	template <typename T>
	inline Vector3<T> normalize(const Vector3<T> &v) { return v / length(v); }
	template <typename T>
	inline Normal3<T> normalize(const Normal3<T> &n) { return n / length(n); }

	// clamp between 0 and 1
	inline Float clamp(Float f) {
		return f < 0 ? 0 : (f > 1.0f ? 1.0f : f);
	}
	// clamp between _min_ and _max_
	inline Float clamp(Float f, Float min, Float max) {
		CHECK_INFO(min <= max, "clamp() : min should less than max!");
		return f < min ? min : (f > max ? max : f);
	}
	// clamp between _min_ and _max_
	inline int clamp(int f, int min, int max) {
		CHECK_INFO(min <= max, "clamp() : min should less than max!");
		return f < min ? min : (f > max ? max : f);
	}

	// inverse of 255, convenient to convert between float and char
	constexpr Float inv255 = 0.0039215686274509803921568627451f;

	// float to char
	inline char color_FloatToChar(Float f) {
		f = clamp(f);
		return (char)(f * 255);
	}

	// char to float
	inline Float color_CharToFloat(unsigned char c) {
		return c * inv255;
	}

	// degrees to radians
	constexpr Float radians(Float degrees) {
		return degrees * static_cast<Float>(0.01745329251994329576923690768489);
	}
	// radians to degrees
	constexpr Float degrees(Float radians) {
		return radians * static_cast<Float>(57.295779513082320876798154814105);

	}

	inline Float cosTheta(const Vector3f &w) { return w.y; }
	inline Float absCosTheta(const Vector3f &w) { return std::abs(w.y); }
	inline Float sinTheta2(const Vector3f &w) { return std::max(0.0f, 1.0f - cosTheta(w) * cosTheta(w)); }
	inline Float sinTheta(const Vector3f &w) { return std::sqrt(sinTheta2(w)); }
	inline Float cosPhi(const Vector3f &w) {
		Float sinTh = sinTheta(w);
		if (sinTh == 0) return 1.0f;
		return clamp(w.x / sinTh, -1.0f, 1.0f);
	}
	inline Float sinPhi(const Vector3f &w) {
		Float sinTh = sinTheta(w);
		if (sinTh == 0) return 0.0f;
		return clamp(-w.z / sinTh, -1.0f, 1.0f);
	}
	inline bool sameHemisphere(const Vector3f &wi, const Vector3f &wo) { return wi.y * wo.y > 0; }
	inline Float sphericalTheta(const Vector3f &v) { return std::acos(clamp(v.y, -1.0f, 1.0f)); }
	inline Float sphericalPhi(const Vector3f &v) {
		Float p = std::atan2(-v.z, v.x);
		return (p < 0) ? p + 2 * pi : p;
	}
}

#endif // !ORT_LINALG_H_
