#include <iostream>
using namespace std;

int main() {
	const int image_width = 256;
	const int image_height = 256;

	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	
	for (int i = image_height-1; i >= 0; --i) {

		cerr << "\rScanlines remaining: " << i << ' ' << flush;

		for (int j = 0; j < image_width; ++j) {

			auto r = double(j) / (image_width-1);
			auto g = double(i) / (image_height-1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	cerr << "\nDone.\n";
	//system("pause");
	return 0;
}