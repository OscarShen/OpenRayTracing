/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLE_METHOD_H_
#define ORION_SAMPLE_METHOD_H_
#include <orion.h>
#include <core/geometry.h>
#include <math/linalg.h>
#include <math/mathutil.h>
namespace orion {

	Point2f	uniformSampleDisk(const Point2f &rand);
	Point2f concentricSampleDisk(const Point2f &rand);

	Point2f uniformSampleTriangle(const Point2f &rand);

	Vector3f uniformSampleSphere(const Point2f &rand);

	inline Float uniformConePdf(Float cosThetaMax) {
		return 1 / (2 * pi * (1 - cosThetaMax));
	}

	inline Vector3f cosineSampleHemisphere(const Point2f &rand) {
		Point2f sample = concentricSampleDisk(rand);
		Float y = std::sqrt(std::max(0.0f, 1 - sample[0] * sample[0] - sample[1] * sample[1]));
		return Vector3f(sample[0], y, sample[1]);
	}

	inline Float powerHeuristic(int nf, Float fPdf, int ng, Float gPdf) {
		Float f = nf * fPdf, g = ng * gPdf;
		return (f * f) / (f * f + g * g);
	}

	struct Distribution1D
	{
		std::vector<Float> func, cdf;
		Float funcInt;

		Distribution1D(const Float *f, int n) : func(f, f + n), cdf(n + 1) {
			cdf[0] = 0;
			for (int i = 1; i < n + 1; ++i)
				cdf[i] = cdf[i - 1] + func[i - 1] / n;

			funcInt = cdf[n];
			if (funcInt == 0) {
				for (int i = 1; i < n + 1; ++i)
					cdf[i] = Float(i) / Float(n);
			}
			else {
				for (int i = 1; i < n + 1; ++i)
					cdf[i] /= funcInt;
			}
		}
		int count() const { return (int)func.size(); }
		Float sampleContinuous(Float rand, Float *pdf, int *off = nullptr) const {
			// Find surrounding CDF segments and _offset_
			int offset = findInterval((int)cdf.size(),
				[&](int index) { return cdf[index] <= rand; });
			if (off) *off = offset;
			// Compute offset along CDF segment
			Float du = rand - cdf[offset];
			if ((cdf[offset + 1] - cdf[offset]) > 0) {
				du /= (cdf[offset + 1] - cdf[offset]);
			}
			CHECK_INFO(!std::isnan(du), "du should not be nan!");

			// Compute PDF for sampled offset
			if (pdf) *pdf = (funcInt > 0) ? func[offset] / funcInt : 0;

			// Return $x\in{}[0,1)$ corresponding to sample
			return (offset + du) / count();
		}
		int SampleDiscrete(Float u, Float *pdf = nullptr,
			Float *uRemapped = nullptr) const {
			// Find surrounding CDF segments and _offset_
			int offset = findInterval((int)cdf.size(),
				[&](int index) { return cdf[index] <= u; });
			if (pdf) *pdf = (funcInt > 0) ? func[offset] / (funcInt * count()) : 0;
			if (uRemapped)
				*uRemapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
			if (uRemapped) CHECK(*uRemapped >= 0.f && *uRemapped <= 1.f);
			return offset;
		}
		Float discretePdf(int index) const {
			CHECK(index >= 0 && index < count());
			return func[index] / (funcInt * count());
		}
	};
}

#endif // !ORION_SAMPLE_METHOD_H_
