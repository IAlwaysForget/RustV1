#include "Pch.h"
#include "Globals.h"
#include "Visuals.h"

#include "context.h"
std::atomic<ViewMatrix> CurrentMatrix;

Vector2 WorldToScreen(const Vector3& position)
{
	Vector3 transform(CurrentMatrix.load().matrix[0][3], CurrentMatrix.load().matrix[1][3], CurrentMatrix.load().matrix[2][3]);
	Vector3 right(CurrentMatrix.load().matrix[0][0], CurrentMatrix.load().matrix[1][0], CurrentMatrix.load().matrix[2][0]);
	Vector3 up(CurrentMatrix.load().matrix[0][1], CurrentMatrix.load().matrix[1][1], CurrentMatrix.load().matrix[2][1]);

	float w = Vector3::Dot(transform, position) + CurrentMatrix.load().matrix[3][3];

	if (w < 1.f)
		return Vector2(0, 0);
	float x = Vector3::Dot(right, position) + CurrentMatrix.load().matrix[3][0];
	float y = Vector3::Dot(up, position) + CurrentMatrix.load().matrix[3][1];
	if (ctx.m_settings.Overlay_OverRideResolution)
		return Vector2((ctx.m_settings.Overlay_ScreenWidth / 2) * (1.f + x / w), (ctx.m_settings.Overlay_ScreenHeight / 2) * (1.f - y / w));
	Vector2 output = Vector2((GetSystemMetrics(SM_CXSCREEN) / 2) * (1.f + x / w), (GetSystemMetrics(SM_CYSCREEN) / 2) * (1.f - y / w));
	if (output.x > GetSystemMetrics(SM_CXSCREEN) || output.y > GetSystemMetrics(SM_CYSCREEN))
		return Vector2(0, 0);
	if (output.x < 0 || output.y < 0)
		return Vector2(0, 0);
	if (output.x < 0 || output.y < 0)
		return Vector2(0, 0);

	/*if (WindowPos.x != 0 || WindowPos.y != 0)
	{
		output.x += WindowPos.x;
		output.y += WindowPos.y;
	}*/

	return output;
}

void RefreshViewMatrix()
{
	CurrentMatrix.store(Camera.load()->GetViewMatrix());
}
