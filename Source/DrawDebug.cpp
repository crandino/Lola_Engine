#include "DrawDebug.h"

#include "openGL.h"

#include "Globals.h"

DrawDebug::DrawDebug()
{
	edges = new math::float3[24];
}

DrawDebug::~DrawDebug()
{
	RELEASE(edges);
}

void DrawDebug::DrawAABB(const math::AABB &bbox)
{
	bbox.ToEdgeList(edges);

	glBegin(GL_LINES);

	glLineWidth(2.0f);
	glColor3f(0.0f, 255.0f, 0.0f);

	for (int i = 0; i < 24; ++i)
		glVertex3f(edges[i].x, edges[i].y, edges[i].z);

	glColor3f(0.0f, 0.0f, 0.0f);

	glEnd();
}

void DrawDebug::DrawOBB(const math::OBB &bbox)
{
	bbox.ToEdgeList(edges);

	glBegin(GL_LINES);

	glLineWidth(2.0f);
	glColor3f(125.0f, 255.0f, 125.0f);

	for (int i = 0; i < 24; ++i)
		glVertex3f(edges[i].x, edges[i].y, edges[i].z);

	glColor3f(0.0f, 0.0f, 0.0f);

	glEnd();
}

