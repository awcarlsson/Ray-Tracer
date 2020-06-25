#include "util_functions.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>
#include <string>

color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;
	// Once the recursive ray count is exceeded, return black (no more color info gathered)
	if (depth <= 0)
		return color(0, 0, 0);
	// If an object in the world is hit, shoot another ray in a random direction to gather more
	// color data
	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	// If nothing is hit, return a background color
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
	// Toggle if want the ppm to be automatically generated
	bool write_ppm = true;
	std::string image_name = "cam";

	// Sets up the image properties
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100; // Number of rays to shoot per pixel
	const int max_depth = 50; // Number of times a ray can recurse to gather color data
	
	// Sets up the output
	std::ofstream outfile;
	if (write_ppm) {
		outfile.open("images/" + image_name + ".ppm");
		outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	}
	else {
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	}

	// Creates the objects populating the world
	hittable_list world;

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(color(.1, .2, .5))));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(color(.8, .8, 0.))));
	world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(.8, .6, .2), 0.3)));
	world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(point3(-1, 0, -1), -0.45, make_shared<dielectric>(1.5)));

	camera cam(110, double(image_width)/image_height);

	// Cycles through each pixel of the image
	for (int j = image_height-1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			// Anti-aliasing step:
			// Shoots samples_per_pixel rays into the pixel at random and averages the colors
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = double(i + random_double()) / (image_width - 1);
				auto v = double(j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);

				pixel_color += ray_color(r, world, max_depth);
			}

			if (write_ppm)
				write_color(outfile, pixel_color, samples_per_pixel);
			else
				write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";

	if(write_ppm)
		outfile.close();
}