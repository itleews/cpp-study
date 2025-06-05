#pragma once
#include "../Motor/MotorManager.h"
#include "../Motor/MotorTransform.h"

struct MotorPreviewInputData {
    bool isAddMotorMode;
    bool isAddSubmotorMode;
	bool isAddRotatingMotorMode;

    CString sx, sy, ex, ey;
    CString width, height, speed;

    AxisType axis;

    Motor* parentMotor;

    bool isValid;
};


class MotorPreviewPanel {
public:
	MotorTransform m_motorTransform;
    Motor UpdatePreview(const MotorPreviewInputData& data);
    CPoint GetRotationCenter(Motor* motor);
private:
    CPoint m_start, m_end;
    CSize m_motorSize;
    bool PreviewSubmotor(const MotorPreviewInputData& data, CPoint& start, CPoint& end);
    Motor PreviewRotatingmotor(const MotorPreviewInputData& data);
};