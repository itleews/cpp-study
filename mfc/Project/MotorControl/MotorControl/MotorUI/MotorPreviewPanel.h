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
private:
    CPoint m_start, m_end;
    CSize m_motorSize;
    bool PreviewSubmotor(const MotorPreviewInputData& data, CPoint& start, CPoint& end);
    Motor PreviewRotatingmotor(const MotorPreviewInputData& data);
    CPoint SubToLogical(CPoint subMotorPos, CPoint mainMotorPos) {
        // mainMotorPos 위치로 이동하는 translation 행렬 생성
        Matrix3x3 translationMatrix = m_motorTransform.translation(mainMotorPos.x, mainMotorPos.y);

        // subMotorPos를 행렬 변환 (transformPoint는 double x, y를 받으므로)
        POINT transformedPoint = translationMatrix.transformPoint(subMotorPos.x, subMotorPos.y);

        // 변환된 결과를 CPoint로 반환
        return CPoint(transformedPoint.x, transformedPoint.y);
    }
};