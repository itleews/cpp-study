#pragma once
#include "../Motor/MotorManager.h"

struct MotorPreviewInputData {
    bool isAddMotorMode;
    bool isAddSubmotorMode;

    CString sx, sy, ex, ey;
    CString width, height, speed;

    bool isXAxis;

    Motor* parentMotor;
};


class MotorPreviewPanel {
public:
    Motor UpdatePreview(const MotorPreviewInputData& data);
private:
    CPoint SubToLogical(CPoint subMotorPos, CPoint mainMotorPos) {
        // ���� ������ ��ġ�� �� ��ġ�� ��ȯ
        return CPoint(subMotorPos + mainMotorPos);
    }
};