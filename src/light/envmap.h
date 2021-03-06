/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LIGHT_INFINITE_H_
#define ORION_LIGHT_INFINITE_H_

#include <core/light.h>
#include <texture/imagetexture.h>
ORION_NAMESPACE_BEGIN
class Distribution2D;
class Envmap : public Light
{
private:
	std::unique_ptr<Texture> Lmap;
	Spectrum aveSpectrum;			// average spectrum of infinite area light
	Point3f worldCenter;
	Float worldRadius;
	std::unique_ptr<Distribution2D> distribution;

public:
	Envmap(const Transform &light2world, const Spectrum &L,
		int nSamples, const std::string &path);

	void preprocess(const Scene &scene) override {
		scene.worldbound().boundingSphere(&worldCenter, &worldRadius);
	}

	Spectrum Le(const Ray &ray) const override;

	Spectrum power() const override;

	Spectrum sample_Li(const Intersection &isec, const Point2f &rnd, Vector3f *wi,
		Float *pdf, ShadowTester *sdt, Point3f *samplePoint = nullptr) const override;
	Float pdf_Li(const Intersection &isec, const Vector3f &wi, const Scene &scene) const override;

	Spectrum sample_Le(const Point2f &rand1, const Point2f &rand2,
		Ray *ray, Normal3f *nLight, Float *pdfPos, Float *pdfDir) const override;
	void pdf_Le(const Ray &ray, const Normal3f &n, Float *pdfPos,
		Float *pdfDir) const override;
};

std::shared_ptr<Envmap> createEnvMap(const Transform &transform, const ParamSet &param);

ORION_NAMESPACE_END
#endif // !ORION_LIGHT_INFINITE_H_
