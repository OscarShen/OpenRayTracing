#include "directlighting.h"
#include <light/light.h>
#include <util/strutil.h>
namespace orion {
	void DirectLighting::preprocess(const Scene & scene, Sampler & sampler)
	{
		for (const auto &light : scene.lights)
			nLightSamples.push_back(light->nSamples);
	}
	Spectrum DirectLighting::Li(const Ray & ray, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Sampler>& sampler, int depth) const
	{
		Intersection isec;
		if (!scene->intersect(ray, &isec))
			return Spectrum(0.34f, 0.55f, 0.85f);

		Spectrum L;
		// if hit a emissive primitive
		L += isec.Le(-ray.d);

		for (const auto &light : scene->lights) {
			if (scene->lights.size() > 0) {
				L += uniformSampleAllLights(ray, isec, *scene, *sampler, nLightSamples);
			}
		}
		std::shared_ptr<BSDF> bsdf = isec.primitive->getMaterial()->getBSDF(&isec);
		if (depth + 1 < maxDepth) {
			L += specularReflect(ray, &isec, sampler, bsdf, scene, depth);
			L += specularTransmit(ray, &isec, sampler, bsdf, scene, depth);
		}
		return L;
	}
	std::shared_ptr<DirectLighting> createDirectLightingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, const ParamSet & param)
	{
		int maxDepth = parseInt(param.getParam("maxDepth"));
		return std::shared_ptr<DirectLighting>(new DirectLighting(camera, sampler, maxDepth));
	}
}
