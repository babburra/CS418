//http://www.cplusplus.com/forum/general/87738/
#include <GL/glut.h>
#include <fstream>
#include <string>
#include <GL/glut.h>
#include <cmath>
#include "helper.h"

#define PI 3.14159265359

static float scale = 0.5;
static float ymax = -9999;
using namespace std;

void Reader::read_triangles(string filename)
{
	//read the vertices
	ifstream file;
	file.open(filename);
	string str;
	while (!file.eof())
	{
		getline(file, str);
		if (str[0] == 'v') break;
	}
	while (str[0] == 'v')
	{
		int i = 0;
		point v;

		while (true)
		{
			while (str[i] == ' ')
			{
				i++; //move a space over
			}
			i++;
			i++;
			int j = i, k = i;
			while (str[i] != ' ') {
				i++;
				k = i;
			}
			v.x = scale*atof(str.substr(j, k - j).c_str());

			//moving on to the y coord

			while (str[i] == ' ') {
				i++;
			}

			int q = i, w = i;
			while (str[i] != ' ') {
				i++;
				w = i;
			}
			v.y = scale*atof(str.substr(q, w - q).c_str());

			while (str[i] == ' ') {
				i++;
			}

			int a = i, s = i;
			while (i<str.size() && str[i] != ' ') {
				i++;
				s = i;
			}
			v.z = scale*atof(str.substr(a, s - a).c_str());

			break;
		}
		//get the ymax 
		if (v.y>ymax) ymax = v.y;
		vertices.push_back(v);
		getline(file, str);
		while (str[0] == '#' || str[0] == 'g')
		{
			getline(file, str);
		}
	}
	//reade the triangles
	int i = 0;

	while (str[0] == 'f')
	{

		while (str[i] == 'f') i++;
		while (str[i] == ' ') i++;
		int j = i, k = i;

		while (str[i] != ' ') {
			i++;
			k = i;
		}

		int one = atoi(str.substr(j, k - j).c_str());

		i += 1;

		j = i;
		k = i;

		while (str[i] != ' ') {
			i++;
			k = i;
		}

		int two = atoi(str.substr(j, k - j).c_str());

		i += 1;

		j = i;
		k = i;

		while (i<str.size() && str[i] != ' ') {
			i++;
			k = i;
		}

		int three = atoi(str.substr(j, k - j).c_str());

		vector<int> v;
		v.push_back(one - 1);
		v.push_back(two - 1);
		v.push_back(three - 1);
		faces.push_back(v);
		getline(file, str);
		while (str[0] == '#' || str[0] == 'g')
		{
			getline(file, str);
		}
		i = 0;
	}
}

void Reader::setup_cells()
{
	int num_vertices = vertices.size();
	for (int i = 0; i<num_vertices; i++)
	{
		vector<int> v;
		vertex_faces.push_back(v);
	}
	int num_triangles = faces.size();
	for (int i = 0; i<num_triangles; i++)
	{
		for (int j = 0; j<3; j++)
		{
			int index = faces[i][j];
			vertex_faces[index].push_back(i);
		}
	}
}

void Reader::compute_normals()
{
	int num_triangles = faces.size();
	int num_vertices = vertices.size();
	for (int i = 0; i<num_triangles; i++)
	{
		point v0 = vertices[faces[i][0]];
		point v1 = vertices[faces[i][1]];
		point v2 = vertices[faces[i][2]];
		point norm = v0.getnormal(v1, v2);
		face_normals.push_back(norm);
	}
	for (int i = 0; i<num_vertices; i++)
	{
		vector<int> f = vertex_faces[i];
		point norm;
		if (f.size()>0)
		{
			for (unsigned int j = 0; j<f.size(); j++)
			{
				norm = norm + face_normals[f[j]];
			}
			norm = norm / faces.size();
			norm.norm();
		}
		else
		{
			norm.set(0.0, 1.0, 0.0);
		}
		vertex_normals.push_back(norm);
	}
}

void Reader::render()
{
	point n;
	point v;
	float theta;

	for (unsigned int i = 0; i<faces.size(); i++)
	{
		glBegin(GL_POLYGON);
		for (int j = 0; j < 3; j++)
		{
			point v = vertices[faces[i][j]];
			point n = vertex_normals[faces[i][j]];
			theta = atan2(v.z, v.x);
			float s = (theta + PI) / (2 * PI);
			float t = v.y / ymax;
			glTexCoord2f(s, t);
			glNormal3f(n.x, n.y, n.z);
			glVertex3d(v.x, v.y, v.z);
		}
		glEnd();
	}
}