#include "util_functions.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <fstream>
#include <string>

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
	// Toggle if want the ppm to be automatically generated
	bool write_ppm = true;
	std::string image_name = "hittable";

	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	
	std::ofstream outfile;
	if (write_ppm) {
		outfile.open("images/" + image_name + ".ppm");
		outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	}
	else {
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	}
	
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	for (int j = image_height-1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

			color pixel_color = ray_color(r, world);

			if (write_ppm)
				write_color(outfile, pixel_color);
			else
				write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";

	if(write_ppm)
		outfile.close();
}