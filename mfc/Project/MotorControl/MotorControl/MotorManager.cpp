#include "pch.h"
#include "MotorManager.h"

MotorAxis* MotorManager::AddAxis(
    bool isXDirection,
    double strXRatio, double strYRatio,
    double endXRatio, double endYRatio,
    double motorXRatio, double motorYRatio,
    CSize motorSize)
{
    MotorAxis* newAxis = new MotorAxis(
        nextId++, isXDirection,
        strXRatio, strYRatio,
        endXRatio, endYRatio,
        motorXRatio, motorYRatio,
        motorSize);

    axisList.push_back(newAxis);
    return newAxis;
}


//void MotorManager::MoveAxis(int id, CPoint dest) {
//	MotorAxis* axis = FindAxis(id);
//	if (axis) {
//		axis->MoveTo(dest);
//	}
//}

MotorAxis* MotorManager::FindAxis(int id) {
	for (MotorAxis* axis : axisList) {
		if (axis->m_id == id) {
			return axis;
		}
	}
	return nullptr;
}

void MotorManager::UpdateAllMotorRatios()
{
    for (auto& axis : axisList)
    {
        if (axis == nullptr) continue;

        // 1. 이전 기준(baseRect)으로 실제 좌표 복원
        int oldW = max(prevBaseRect.Width(), 1);
        int oldH = max(prevBaseRect.Height(), 1);

        int strX = static_cast<int>(axis->strXRatio * oldW) + prevBaseRect.left;
        int strY = static_cast<int>(axis->strYRatio * oldH) + prevBaseRect.top;
        int endX = static_cast<int>(axis->endXRatio * oldW) + prevBaseRect.left;
        int endY = static_cast<int>(axis->endYRatio * oldH) + prevBaseRect.top;
        int motX = static_cast<int>(axis->motorXRatio * oldW) + prevBaseRect.left;
        int motY = static_cast<int>(axis->motorYRatio * oldH) + prevBaseRect.top;

        // 2. 새로운 기준(baseRect)으로 비율 재계산
        int newW = max(baseRect.Width(), 1);
        int newH = max(baseRect.Height(), 1);

        axis->strXRatio = (strX - baseRect.left) / (double)newW;
        axis->strYRatio = (strY - baseRect.top) / (double)newH;
        axis->endXRatio = (endX - baseRect.left) / (double)newW;
        axis->endYRatio = (endY - baseRect.top) / (double)newH;
        axis->motorXRatio = (motX - baseRect.left) / (double)newW;
        axis->motorYRatio = (motY - baseRect.top) / (double)newH;
    }
}
