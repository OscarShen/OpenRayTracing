/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SHAPE_DISK_H_
#define ORION_SHAPE_DISK_H_
#include <orion.h>
#include "shape.h"
namespace orion {

	class Disk : public Shape
	{
	private:
		const Float radius;

	public:
		Disk(const Transform *local2world, const Transform *world2local,
			Float radius) : Shape(local2world, world2local),
			radius(radius) {}

		virtual bool intersect(const Ray &ray, Intersection *isec) const override;
		virtual Bounds3f localBound() const override;
		virtual Float area() const override;
		virtual Intersection sample(const Point2f &u, Float *pdf) const override;
	};

	std::shared_ptr<Disk> createDisk(const Transform *local2world, const Transform *world2local, const ParamSet &param);
}

#endif // !ORION_SHAPE_DISK_H_
