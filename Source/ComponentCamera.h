#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"

#include "MathGeoLib\MathGeoLib.h"

class ComponentCamera : public Component
{

private:

	bool camera_modified;

	float aspect_ratio;
	float FOV_h, FOV_v;
	float near_plane, far_plane;

	float near_plane_tmp, far_plane_tmp;

	float max_far_plane;

	void CalcHorizontalFOV();

public:

	math::Frustum cam_frustum;
	math::Frustum origin_cam_frustum;

	// Move that to Transform, like Unity
	math::float3 forward;
	math::float3 up;
	math::float3 left;

	ComponentCamera();
	bool Update();
	void SetComponent();

	void ShowEditorInfo();
	void UpdateFrustum();
	void ApplyTransformToFrustum();

};

#endif __COMPONENTTRANSFORM_H__

