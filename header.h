#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>

using namespace std;


float min(float a, float b);
float max(float a, float b);


class Point 
{
public:
	float x,y,z;
	Point(float x, float y, float z);
	Point* sub(Point* c);
	Point* add(Point* c);
	float getLength();
	Point* crossProduct(Point* p);
	float dotProduct(Point* p);
	string toString();
};

class Edge{
public:
	Point* A;
	Point* B;
	Edge (Point* A, Point* B);
	string toString();
};

class Face {
public:
	Point* A;
	Point* B;
	Point* C;
	Point* normal;
	vector<Point*> frontPoints;
	
	Face(Point* A, Point* B, Point* C);
	Edge* getCommonEdge(Face* p);
	bool isCommonEdge(Face* p);
	void toggleNormal();
	void updateNormal(vector<Point*> internal_points);
	void updateFrontPoints(vector<Point*> points);
	vector<Edge*> edges();
	string toString();
	float distFromFace(Face* f, Point* p);
};

float distFromFace(Face* f, Point* p);

int getIndex(Face* p, vector<Face*> &convexFaces);

vector<Point*> findExtremePoints(vector<Point*> points);

float Distance(Point* p1, Point* p2);

Edge* maxDistanceLine(vector<Point*> extr);

Point* farthestPointFromLine(Edge* f, vector<Point*> &points);

Point* farthestPointFromFace(Face* f, vector<Point*> &points);

vector<Face*> neighbourFace(Face* p, vector<Face*> planes);

bool augmentedDFS(Face* p, Point* f, set<Face*> &visitedp, set<Edge*> &boundaryedges, vector<Face*> &convexFaces);

#endif
