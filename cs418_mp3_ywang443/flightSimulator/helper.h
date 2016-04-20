#ifndef HELPER_H
#define HELPER_H
#include <vector>
#include <cstring>
#include "struct.h"


class Reader {
private:
	vector<point> vertices;
	vector<point> vertex_normals;
	vector<vector<int>> faces;
	vector<point> face_normals;
	vector<vector<int>> vertex_faces;
	void compute_normals();
	void read_triangles(string filename);
	void setup_cells();
public:
	Reader(string filename) {
		read_triangles(filename);
		setup_cells();
		compute_normals();
	};
	~Reader() {};
	void render();
};
#endif
