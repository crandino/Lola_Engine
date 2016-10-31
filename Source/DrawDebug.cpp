#include "DrawDebug.h"

#include "openGL.h"

#include "Globals.h"

#include <stack>

bool DrawDebug::show_aabb = false;
bool DrawDebug::show_frustum = false;
bool DrawDebug::show_octree = false;

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
	if (show_aabb)
	{
		bbox.ToEdgeList(edges);

		glBegin(GL_LINES);

		glLineWidth(LINE_WIDTH);
		glColor3f(0.0f, 1.0f, 0.0f);

		for (int i = 0; i < 24; ++i)
			glVertex3fv(edges[i].ptr());

		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(DEFAULT_LINE_WIDTH);

		glEnd();
	}	
}

void DrawDebug::DrawOBB(const math::OBB &bbox) const
{
	bbox.ToEdgeList(edges);

	glBegin(GL_LINES);

	glLineWidth(LINE_WIDTH);
	glColor3f(0.5f, 1.0f, 0.5f);

	for (int i = 0; i < 24; ++i)
		glVertex3fv(edges[i].ptr());

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(DEFAULT_LINE_WIDTH);

	glEnd();
}

void DrawDebug::DrawFrustum(const math::Frustum &frustrum) const
{
	if (show_frustum)
	{
		frustrum.GetCornerPoints(corners);

		glBegin(GL_LINES);

		glLineWidth(LINE_WIDTH);
		glColor3f(0.0f, 0.5f, 1.0f);

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

		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(DEFAULT_LINE_WIDTH);

		glEnd();
	}	
}

void DrawDebug::DrawOcTree(const OcTree &quad) const
{	
	if (show_octree)
	{
		std::vector<OcTreeNode*> nodes;
		quad.CollectRects(nodes);

		glBegin(GL_LINES);

		glLineWidth(LINE_WIDTH);
		glColor3f(1.0f, 0.7f, 0.0f);

		for (uint i = 0; i < nodes.size();++i)
		{
			nodes[i]->box.ToEdgeList(edges);			

			for (int i = 0; i < 24; ++i)
				glVertex3fv(edges[i].ptr());			
		}

		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(DEFAULT_LINE_WIDTH);

		glEnd();
	}	
}

