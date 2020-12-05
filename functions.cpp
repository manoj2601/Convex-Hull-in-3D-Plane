#include <bits/stdc++.h>
#include "header.h"

using namespace std;

float min(float a, float b) {if(a<b) return a; else return b;}
float max(float a, float b) {if(a>b) return a; else return b;}

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Point* Point::sub(Point* c)
{
	Point* ret = new Point(this->x - c->x, this->y - c->y, this->z - c->z);
	return ret;
}

Point* Point::add(Point* c)
{
	Point* ret = new Point(this->x + c->x, this->y + c->y, this->z + c->z);
	return ret;
}
float Point::getLength()
{
	return sqrt(pow(this->x, 2)+pow(this->y, 2)+pow(this->z, 2));
}
Point* Point::crossProduct(Point* p)
{
	float a = this->y*p->z - this->z*p->y;
	float b = this->z*p->x - this->x*p->z;
	float c = this->x*p->y - this->y*p->x;
	return (new Point(a,b,c));
}
float Point::dotProduct(Point* p)
{
	float a = this->x*p->x;
	float b = this->y*p->y;
	float c = this->z*p->z;
	return (a+b+c);
}
string Point::toString()
{
	return to_string(this->x)+" "+to_string(this->y)+" "+to_string(this->z);
}


Edge::Edge (Point* A, Point* B)
{
	this->A = A;
	this->B = B;
}
string Edge::toString() {
	return "["+this->A->toString()+", "+this->B->toString()+"]";
}



Face::Face(Point* A, Point* B, Point* C) {
	this->A = A;
	this->B = B;
	this->C = C;
	this->normal = (this->A->sub(this->B))->crossProduct(this->B->sub(this->C));
	float l = this->normal->getLength();
	this->normal->x = this->normal->x/l;
	this->normal->y = this->normal->y/l;
	this->normal->z = this->normal->z/l;
}
Edge* Face::getCommonEdge(Face* p)
{
	vector<Point*> common;
	if((this->A == p->A) || (this->A == p->B) || (this->A == p->C))
	{
		common.push_back(this->A);
	}
	if((this->B == p->A) || (this->B == p->B) || (this->B == p->C))
	{
		common.push_back(this->B);
	}
	if((this->C == p->A) || (this->C == p->B) || (this->C == p->C))
	{
		common.push_back(this->C);
	}
	if(common.size() == 3)
	{
		cout<<"Both planes are same.";
		return NULL;
	}
	else if(common.size() != 2)
	{
		cout<<"No common edge";
		return NULL;
	}
	Edge* ret = new Edge(common[0], common[1]);
	return ret;
}
bool Face::isCommonEdge(Face* p)
{
	vector<Point*> common;
	if((this->A == p->A) || (this->A == p->B) || (this->A == p->C))
	{
		common.push_back(this->A);
	}
	if((this->B == p->A) || (this->B == p->B) || (this->B == p->C))
	{
		common.push_back(this->B);
	}
	if((this->C == p->A) || (this->C == p->B) || (this->C == p->C))
	{
		common.push_back(this->C);
	}
	if(common.size() ==2)
		return true;
		return false;
}
void Face::toggleNormal() {
	this->normal->x = -1*this->normal->x;
	this->normal->y = -1*this->normal->y;
	this->normal->z = -1*this->normal->z;
}
void Face::updateNormal(vector<Point*> internal_points) {
	for(int i=0; i<internal_points.size(); i++) {
		float d = this->distFromFace(this, internal_points[i]);
		if(d>pow(10, -5)) { //pow(10, -5) is used instead of zero bcoz float has 6 digit precision. 
			this->toggleNormal();
			return;
		}
	}
}
void Face::updateFrontPoints(vector<Point*> points) {
	for (int i=0; i<points.size(); i++) {
		float d = this->distFromFace(this, points[i]);
		if(d > pow(10, -5)) //pow(10, -5) is used instead of zero bcoz float has 6 digit precision.
			this->frontPoints.push_back(points[i]);
	}
}
vector<Edge*> Face::edges()
{
	vector<Edge*> ret;
	Edge* e1 = new Edge(this->A, this->B);
	ret.push_back(e1);
	Edge* e2 = new Edge(this->A, this->C);
	ret.push_back(e2);
	Edge* e3 = new Edge(this->B, this->C);
	ret.push_back(e3);
	return ret;
}
string Face::toString()
{
	return "["+this->A->toString()+", "+this->B->toString()+", "+this->C->toString()+"]\n"+"normal: "+this->normal->toString();
}

float Face::distFromFace(Face* f, Point* p) {
	if(f->A == p || f->B == p || f->C == p)
		return 0;
	Point* normal = f->normal;
	return normal->dotProduct(p->sub(f->A));
}

float distFromFace(Face* f, Point* p) {
	if(f->A == p || f->B == p || f->C == p)
		return 0;
	Point* normal = f->normal;
	return normal->dotProduct(p->sub(f->A));
}

int getIndex(Face* p, vector<Face*> &convexFaces)
{
	for(int i=0; i<convexFaces.size(); i++)
	{
		if(convexFaces[i] == p)
			return i;
	}
	return -1;
}

vector<Point*> findExtremePoints(vector<Point*> points)
{
	float minx,miny,minz;
	float maxx, maxy, maxz;
	minx = INT_MAX;
	miny = INT_MAX;
	minz = INT_MAX;
	maxx = (-1)*INT_MAX;
	maxy = (-1)*INT_MAX;
	maxz = (-1)*INT_MAX;

	for(int i=0; i<points.size(); i++)
	{
		minx = min(minx, points[i]->x);
		miny = min(miny, points[i]->y);
		minz = min(minz, points[i]->z);
		maxx = max(maxx, points[i]->x);
		maxy = max(maxy, points[i]->y);
		maxz = max(maxz, points[i]->z);
	}
	vector<Point*> ret(6);
	for(int i=points.size()-1; i>=0; i--)
	{
		if(points[i]->x == minx)
			ret[0] = points[i];
		if(points[i]->y == miny)
			ret[1] = points[i];
		if(points[i]->z == minz)
			ret[2] = points[i];
		if(points[i]->x == maxx)
			ret[3] = points[i];
		if(points[i]->y == maxy)
			ret[4] = points[i];
		if(points[i]->z == maxz)
			ret[5] = points[i];
	}
	return ret;
}

float Distance(Point* p1, Point* p2) {
	float a1 = (p1->x - p2->x)*(p1->x - p2->x);
	float a2 = (p1->y - p2->y)*(p1->y - p2->y);
	float a3 = (p1->z - p2->z)*(p1->z - p2->z);
	float ret = a1+a2+a3;
	return float(sqrt(ret));
}

Edge* maxDistanceLine(vector<Point*> extr) {
	pair<Point*, Point*> ret;
	float max1 = -1;
	for(int i=0; i<extr.size(); i++) {
		for (int j=i+1; j<extr.size(); j++) {
			float d = Distance(extr[i], extr[j]);
			if(d>max1) {
				max1 = d;
				ret.first = extr[i];
				ret.second = extr[j];
			}
		}
	}
	Edge* e = new Edge(ret.first, ret.second);
	return e;
}

Point* farthestPointFromLine(Edge* f, vector<Point*> &points) {
	float max1 = -1;
	int maxPoint;
	for(int i=0; i<points.size(); i++)
	{
		if(f->A != points[i] && f->B != points[i])
		{
			Point* v1 = points[i]->sub(f->A);
			Point* v2 = points[i]->sub(f->B);
			Point* v3 = f->B->sub(f->A);

			if(v3->getLength() == 0)
			{
				cout<<"Wrong"<<endl;
				exit(1);
				return NULL;
			}
			else
			{
				float height = abs(((v1->crossProduct(v2))->getLength()))/(v3->getLength());
				if(height > max1)
				{
					max1 = height;
					maxPoint = i;
				}
			}
		}
	}
	return points[maxPoint];
}

Point* farthestPointFromFace(Face* f, vector<Point*> &points) {
	float max1 = -1;
	int maxPoint=-1;
	for(int i=0; i<points.size(); i++)
	{
		float dist = abs(distFromFace(f, points[i]));
		if(dist > max1)
		{
			max1 = dist;
			maxPoint = i;
		}
	}
	return points[maxPoint];
}

vector<Face*> neighbourFace(Face* p, vector<Face*> planes)
{
	vector<Face*> ret;
	for (int i=0; i<planes.size(); i++)
	{
		if(planes[i] == p)
			continue;
		if(p->isCommonEdge(planes[i]))
			ret.push_back(planes[i]);
	}
	return ret;
}

bool augmentedDFS(Face* p, Point* f, set<Face*> &visitedp, set<Edge*> &boundaryedges, vector<Face*> &convexFaces)
{
	if(distFromFace(p, f) <=0)
		return false;
	visitedp.insert(p);
	vector<Face*> neighbourplanes = neighbourFace(p, convexFaces);
	for(int k=0; k<neighbourplanes.size(); k++)
	{
		Face* curr = neighbourplanes[k];
		if(!(visitedp.find(curr) != visitedp.end()))
		{
			bool done = augmentedDFS(curr, f, visitedp, boundaryedges, convexFaces);
			if(!done)
				boundaryedges.insert(p->getCommonEdge(curr));
		}
	}
	return true;
}