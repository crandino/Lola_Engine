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
		glVertex3fv(edges[i].ptr());

	glEnd();
}

void DrawDebug::DrawOBB(const math::OBB &bbox) const
{
	bbox.ToEdgeList(edges);

	glBegin(GL_LINES);

	glLineWidth(2.0f);
	glColor3f(125.0f, 255.0f, 125.0f);

	for (int i = 0; i < 24; ++i)
		glVertex3fv(edges[i].ptr());

	glEnd();
}

void DrawDebug::DrawFrustum(const math::Frustum &frustrum) const
{
	frustrum.GetCornerPoints(corners);

	glBegin(GL_LINES);

	glLineWidth(3.5f);
	glColor3f(0.0f, 125.0f, 255.0f);

	// Near plane
	glVertex3fv(corners[0].ptr());
	glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[0].ptr());
	glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[3].ptr());
	glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[3].ptr());
							   
	// Far plane
	glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[5].ptr());
	glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[6].ptr());
	glVertex3fv(corners[6].ptr());
	glVertex3fv(corners[7].ptr());
	glVertex3fv(corners[5].ptr());
	glVertex3fv(corners[7].ptr());

	// Left plane
	glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[5].ptr());
	glVertex3fv(corners[3].ptr());
	glVertex3fv(corners[7].ptr());
			
	// Right pvlane
	glVertex3fv(corners[0].ptr());
	glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[6].ptr());

	glEnd();
}

