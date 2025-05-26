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
        // 하위 모터의 위치를 논리 위치로 변환
        return CPoint(subMotorPos + mainMotorPos);
    }
};