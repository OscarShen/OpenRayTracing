/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_LIGHT_H_
#define ORION_CORE_LIGHT_H_
#include <orion.h>
#include "transform.h"
#include "scene.h"
#include "intersection.h"
ORION_NAMESPACE_BEGIN

class ShadowTester
{
private:
	Intersection p0, p1;

public:
	ShadowTester() {}
	ShadowTester(const Intersection &isec0, const Intersection &isec1)
		: p0(isec0), p1(isec1) {}
	bool unoccluded(const Scene & scene) const {
		Intersection isec;
		return !scene.intersect(p0.spawnRay(p1), &isec);
	}
};

enum class LightType : int
{
	DeltaPosition = 1,
	DeltaDirection = 2,
	Area = 4,
	Infinite = 8
};

inline bool isDeltaLight(int type) {
	return type & (int)LightType::DeltaPosition ||
		type & (int)LightType::DeltaDirection;
}

class Light
{
public:
	const int nSamples; // samples needed for one light
	const int type;
protected:
	Transform local2world, world2local;

public:
	Light(int type, const Transform &light2world, int nSamples = 1)
		: type(type), local2world(light2world), world2local(inverse(light2world)), nSamples(std::max(1, nSamples)) {}
	virtual ~Light() {}
	virtual void preprocess(const Scene &scene) {}

	virtual Spectrum power() const = 0;

	// used for env map, usual light need not to override
	virtual Spectrum Le(const Ray &ray) const { return 0; }

	virtual Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi,
		Float *pdf, ShadowTester *sdt, Point3f *samplePoint = nullptr) const = 0;
	virtual Float pdf_Li(const Intersection &isec, const Vector3f &wi, const Scene &scene) const = 0;

	virtual Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const = 0;
	virtual void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const = 0;
};

class AreaLight : public Light
{
public:
	AreaLight(const Transform &light2world, int nSamples)
		: Light((int)LightType::Area, light2world, nSamples) {}
	virtual Spectrum L(const Intersection &intr, const Vector3f &w) const = 0;
};


ORION_NAMESPACE_END
#endif // !ORION_CORE_LIGHT_H_
