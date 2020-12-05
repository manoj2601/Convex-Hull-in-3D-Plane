/*
This is an assignment by Prof. Ben Leong (NUS)

Implemented by: 
Manoj Kumar (manoj2601)
www.cse.iitd.ac.in/~cs5180411
*/
#include <bits/stdc++.h>
#include "header.h"
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	ifstream input;
	ofstream output;
	if(argc == 1)
	{
		input.open("CONVEX.IN");
		output.open("CONVEX.OUT");
	}
	else if(argc == 2)
	{
		input.open(argv[1]);
		output.open("CONVEX.OUT");
	}
	else {
		input.open(argv[1]);
		output.open(argv[2]);
	}
	//Reading Inputs
	int N;
	input>>N;
	vector<Point*> points; //array of all points
	for(int i=0;i<N;i++)
	{
		float x,y,z;
		input>>x>>y>>z;
		Point* obj = new Point(x,y,z);
		points.push_back(obj);
	}
	input.close();

	//Input Done

	if(N<4)
	{
		cout<<"Not enough points for a valid 3D convex hull"<<endl;
		return 0;
	}
	//Creating the first tetrahedrone
	vector<Point*> extr = findExtremePoints(points);
	//vector extr contains the points with maximum or minimum values of x, y or z coordinates.
	Edge* f = maxDistanceLine(extr); //line segment between two points with maximum distance.
	Face* p = new Face(f->A, f->B, farthestPointFromLine(f, points)); //plane with maximum area.
	Point* lp = farthestPointFromFace(p, points);
	Face *p1 = p;
	Face* p2 = new Face(p->A, p->B, lp);
	Face* p3 = new Face(p->B, p->C, lp);
	Face* p4 = new Face(p->A, p->C, lp);

	//planes p1, p2, p3 and p4 creates a tetrahedron with maximum volume.
	vector<Point*> internal_points; //Internal points to set the normal in outside of the polygon.
	internal_points.push_back(p->A);
	internal_points.push_back(p->B);
	internal_points.push_back(p->C);
	internal_points.push_back(lp);
	p1->updateNormal(internal_points);
	p2->updateNormal(internal_points);
	p3->updateNormal(internal_points);
	p4->updateNormal(internal_points);
	p1->updateFrontPoints(points);
	p2->updateFrontPoints(points);
	p3->updateFrontPoints(points);
	p4->updateFrontPoints(points);
	vector<Face*> convexFaces;
	convexFaces.push_back(p1);
	convexFaces.push_back(p2);
	convexFaces.push_back(p3);
	convexFaces.push_back(p4);

	while(true)
	{
		bool isDone = true; //boolean to check if no changes occur in one iteration.
		set<Face*> tobeRemovedFaces;
		for(int k=0; k<convexFaces.size(); k++)
		{
			Face* p = convexFaces[k];
			if(p->frontPoints.size() > 0)
			{
				isDone = false;
				Point* f = farthestPointFromFace(p, p->frontPoints);
				set<Edge*> boundaryedges;
				augmentedDFS(p, f, tobeRemovedFaces, boundaryedges, convexFaces);
				vector<Point*> allfrontPoints;
				//storing allfrontPoints to update frontPoints of newly created faces
				for(Face* t : tobeRemovedFaces)
				{
					for(int j=0; j<t->frontPoints.size(); j++)
						allfrontPoints.push_back(t->frontPoints[j]);
				}

				//adding new faces
				for(Edge* e : boundaryedges)
				{
					Face* newp = new Face(e->A, e->B, f);
					newp->updateNormal(internal_points);
					newp->updateFrontPoints(allfrontPoints);
					convexFaces.push_back(newp);
				}
				break;
			}
		}
		//erasing internal faces
		for (Face* p : tobeRemovedFaces)
			convexFaces.erase(convexFaces.begin()+getIndex(p, convexFaces));
		if(isDone)
			break;
	}
	set<Point*> ret;
	for(int i=0; i<convexFaces.size(); i++) {
		ret.insert(convexFaces[i]->A);
		ret.insert(convexFaces[i]->B);
		ret.insert(convexFaces[i]->C);
	}

	output<<ret.size()<<endl;
	for(Point* p : ret)
		output<<p->x<<" "<<p->y<<" "<<p->z<<endl;
	output.close();
	return 0;
}