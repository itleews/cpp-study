#include "pch.h"
#include "MotorPreviewPanel.h"

Motor MotorPreviewPanel::UpdatePreview(const MotorPreviewInputData& data)
{
    if (!data.isAddMotorMode && !data.isAddSubmotorMode) return {};

    CPoint start(_ttoi(data.sx), _ttoi(data.sy));
    CPoint end(_ttoi(data.ex), _ttoi(data.ey));
    CSize motorSize(_ttoi(data.width), _ttoi(data.height));
    int speed = _ttoi(data.speed);

    Motor* parentMotor = data.parentMotor;

    if (data.isAddSubmotorMode) {
        if (!parentMotor) {
            return {};
        }

        CPoint motorStart = parentMotor->motorPos - parentMotor->motorSize;
        CPoint motorEnd = parentMotor->motorPos + parentMotor->motorSize;

        start = SubToLogical(start, motorStart);
        end = SubToLogical(end, motorStart);

        if (start.x < motorStart.x || start.y < motorStart.y ||
            end.x > motorEnd.x || end.y > motorEnd.y) {
            AfxMessageBox(_T("하위 모터는 상위 모터의 영역 내에 있어야 합니다."));
            return {};
        }
    }

    int motorX = data.isXAxis ? (end.x - start.x) / 2 + start.x : end.x;
    int motorY = data.isXAxis ? start.y : (end.y - start.y) / 2 + start.y;

    Motor previewMotor;
    previewMotor.parentMotor = parentMotor;
	previewMotor.isX = data.isXAxis;
    previewMotor.motorPos = CPoint(motorX, motorY);
    previewMotor.motorSize = CSize(motorSize.cx / 2, motorSize.cy / 2);
    previewMotor.motorSpeed = speed;
    previewMotor.strPos = start;
    previewMotor.endPos = end;

    return previewMotor;
}
