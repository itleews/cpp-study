#pragma once
#include "../Motor/MotorManager.h"
#include "../Motor/MotorTransform.h"

struct MotorPreviewInputData {
    bool isAddMotorMode;
    bool isAddSubmotorMode;

    CString sx, sy, ex, ey;
    CString width, height, speed;

    AxisType axis;

    Motor* parentMotor;
};


class MotorPreviewPanel {
public:
	MotorTransform m_motorTransform;
    Motor UpdatePreview(const MotorPreviewInputData& data);
private:
    CPoint SubToLogical(CPoint subMotorPos, CPoint mainMotorPos) {
        // mainMotorPos ��ġ�� �̵��ϴ� translation ��� ����
        Matrix3x3 translationMatrix = m_motorTransform.translation(mainMotorPos.x, mainMotorPos.y);

        // subMotorPos�� ��� ��ȯ (transformPoint�� double x, y�� �����Ƿ�)
        POINT transformedPoint = translationMatrix.transformPoint(subMotorPos.x, subMotorPos.y);

        // ��ȯ�� ����� CPoint�� ��ȯ
        return CPoint(transformedPoint.x, transformedPoint.y);
    }
};