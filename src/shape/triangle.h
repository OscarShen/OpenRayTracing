/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TRIANGLE_H_
#define ORION_TRIANGLE_H_
#include <orion.h>
#include <core/geometry.h>
#include <core/transform.h>
#include <io/meshio.h>
#include <math/linalg.h>
#include "shape.h"
namespace orion {

	// TODO : Add transform
	struct TriangleMesh
	{
		// number of triangles, number of vertices
		int numTri, numVer;
		std::vector<int> vertexIndices;
		std::unique_ptr<Point3f[]> p;		// position
		std::unique_ptr<Normal3f[]> n;		// normal
		std::unique_ptr<Point2f[]> uv;		// uv

		TriangleMesh(const Transform &local2world, const MeshData &meshdata);
	};

	class Triangle : public Shape
	{
	private:
		std::shared_ptr<TriangleMesh> mesh;
		const int *v;

	public:
		Triangle(const Transform *local2world, const Transform *world2local, 
			const std::shared_ptr<TriangleMesh> &mesh, int triNumber)
			: Shape(local2world, world2local), mesh(mesh) {
			v = &mesh->vertexIndices[3 * triNumber];
		}

		virtual bool intersect(const Ray &ray, Intersection *isec) const override;
		virtual Bounds3f worldBound() const override;
		virtual Bounds3f localBound() const override;
		virtual Float area() const override;
		virtual Intersection sample(const Point2f &u, Float *pdf) const override;

	private:
		void _getUVs(Point2f uv[3]) const;
		void _getNormals(Normal3f n[3]) const;
	};
	
	std::vector<std::shared_ptr<Shape>> createTriangleMesh(const Transform *local2world, const Transform *world2local, 
		const MeshData &meshdata);

}

#endif // !ORION_TRIANGLE_H_
