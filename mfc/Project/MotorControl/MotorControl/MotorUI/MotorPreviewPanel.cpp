#include "pch.h"
#include "MotorPreviewPanel.h"

Motor MotorPreviewPanel::UpdatePreview(const MotorPreviewInputData& data)
{
    Motor previewMotor;
    if (!data.isAddMotorMode && !data.isAddSubmotorMode && !data.isAddRotatingMotorMode) return {};
    if (data.isAddRotatingMotorMode) {
        previewMotor = PreviewRotatingmotor(data);
        return previewMotor;
	}

    m_start = CPoint(_ttoi(data.sx), _ttoi(data.sy));
    m_end = CPoint(_ttoi(data.ex), _ttoi(data.ey));
    m_motorSize = CSize(_ttoi(data.width), _ttoi(data.height));
    int speed = _ttoi(data.speed);

    Motor* parentMotor = data.parentMotor;

    if (data.isAddSubmotorMode) {
        if(!PreviewSubmotor(data, m_start, m_end))
            return {};
    }

    int motorX = (data.axis == X) ? (m_end.x - m_start.x) / 2 + m_start.x : m_end.x;
    int motorY = (data.axis == X) ? m_start.y : (m_end.y - m_start.y) / 2 + m_start.y;

    previewMotor.parentMotor = parentMotor;
	previewMotor.axis = data.axis;
    previewMotor.motorPos = CPoint(motorX, motorY);
    previewMotor.motorSize = CSize(m_motorSize.cx / 2, m_motorSize.cy / 2);
    previewMotor.motorSpeed = speed;
    previewMotor.strPos = m_start;
    previewMotor.endPos = m_end;
	previewMotor.isValid = true;

    return previewMotor;
}

bool MotorPreviewPanel::PreviewSubmotor(const MotorPreviewInputData& data, CPoint& start, CPoint& end)
{
    if (!data.parentMotor) return false;

    CPoint motorStart = data.parentMotor->motorPos - data.parentMotor->motorSize;
    CPoint motorEnd = data.parentMotor->motorPos + data.parentMotor->motorSize;

    start = SubToLogical(start, motorStart);
    end = SubToLogical(end, motorStart);

    if (start.x < motorStart.x || start.y < motorStart.y ||
        end.x > motorEnd.x || end.y > motorEnd.y) {
        AfxMessageBox(_T("하위 모터는 상위 모터의 영역 내에 있어야 합니다."));
        return false;
    }
	return true;
}

Motor MotorPreviewPanel::PreviewRotatingmotor(const MotorPreviewInputData& data)
{
	CPoint motorPos(_ttoi(data.sx), _ttoi(data.sy));
	CPoint motorSize(_ttoi(data.width), _ttoi(data.width));
	double angularSpeed = _ttof(data.ex);
    
    if (angularSpeed <= 0 || angularSpeed > 360) {
        AfxMessageBox(_T("회전 속도는 0°/s보다 크고 360°/s 이하이어야 합니다."));
        return {};
    }

    Motor rotatingMotor;
	rotatingMotor.parentMotor = data.parentMotor;
    rotatingMotor.motorPos = motorPos;
    rotatingMotor.motorSize = motorSize;
    rotatingMotor.rotationSpeed = angularSpeed;
    rotatingMotor.isValid = true;

    return rotatingMotor;
}