#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
	public:
		sphere() {}
		sphere(point3 cen, double r) : center(cen), radius(r) {};

		virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

	public:
		point3 center;
		double radius;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	// If the given ray intersects the sphere at at least one point:
	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		// Checks first root
		auto temp = (-half_b - root) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}
		// Second root
		temp = (-half_b + root) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}
	}
	return false;
}

#endif
