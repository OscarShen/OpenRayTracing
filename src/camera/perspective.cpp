#include "perspective.h"
#include <util/strutil.h>
#include <sampler/sampler.h>
#include <sampler/sampling.h>
namespace orion {

	Ray PerspectiveCamera::generateRay(const Point2f &offset, const std::shared_ptr<Sampler> &sampler) const
	{
		Point2f rand_offset = offset + sampler->next2();

		CHECK_INFO(film.get() != nullptr, "Note : no render target in camera!");
		Float width = (Float)film->getWidth();
		Float height = (Float)film->getHeight();
		Float aspectRatio = width / height;

		Float xx = (2 * rand_offset.x / (Float)width - 1) * aspectRatio * tan_half_fov;
		Float yy = (1 - 2 * rand_offset.y / (Float)height) * tan_half_fov;
		Float zz = -1.0f;

		Vector3f dir = normalize(Vector3f(xx, yy, zz));
		Ray before(Point3f(0), dir);

		if (lensRadius > 0) { // if has aperture
			Point3f target = before(focalDistance);

			Point2f uv = uniformSampleDisk(sampler->next2());
			before.o = Point3f(uv.x * lensRadius, uv.y * lensRadius, 0);
			before.d = normalize(target - before.o);
		}

		Ray after = t(before);
		return after;
	}

	std::shared_ptr<Camera> createPerspectiveCamera(const Transform &camera2world, const ParamSet & param)
	{
		Vector2i filmSize = parseVector2i(param.getParam("filmsize"));
		Float fov = parseFloat(param.getParam("fov"));

		auto film = std::make_shared<RenderTarget>(filmSize.x, filmSize.y);
		Float lensRadius = parseFloat(param.getParam("lensRadius"));
		Float focalDistance = parseFloat(param.getParam("focalDistance"));
		return std::shared_ptr<Camera>(new PerspectiveCamera(camera2world, fov, film,focalDistance, lensRadius));
	}
}
