#include "DrawDebug.h"

#include "openGL.h"

#include "Globals.h"

DrawDebug::DrawDebug()
{
	edges = new math::float3[24];
	corners = new math::float3[8];
}

DrawDebug::~DrawDebug()
{
	RELEASE(edges);
	RELEASE(corners);
}

void DrawDebug::DrawAABB(const math::AABB &bbox) const
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

void DrawDebug::DrawOBB(const math::OBB &bbox) const
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

void DrawDebug::DrawFrustum(const math::Frustum &frustrum) const
{
	frustrum.GetCornerPoints(corners);

	glBegin(GL_LINES);

	glLineWidth(2.0f);
	glColor3f(0.0f, 125.0f, 255.0f);

	// Near plane
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	// Far plane
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	// Left plane
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	// Left plane
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);

	glColor3f(0.0f, 0.0f, 0.0f);

	glEnd();
}

