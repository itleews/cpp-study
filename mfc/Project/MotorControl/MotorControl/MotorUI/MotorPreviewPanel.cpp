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

    if (m_motorSize.cx == 0 || m_motorSize.cy == 0) {
        AfxMessageBox(_T("��ȿ���� ���� ���Դϴ�. ���� ������� 0���� Ŀ�� �մϴ�."));
        return {};
    }

    CRect curRect(m_start.x, m_start.y, m_end.x, m_end.y);
    curRect.NormalizeRect();  // �»��-���ϴ� ����

    if (curRect.Width() > m_motorTransform.m_logicalBounds.Width() || curRect.Height() > m_motorTransform.m_logicalBounds.Height()) {
        CRect newRect(0, 0, max(curRect.Width() + 10, m_motorTransform.m_logicalBounds.Width() + 10), max(curRect.Height() + 10, m_motorTransform.m_logicalBounds.Height() + 10));
        m_motorTransform.SetLogicalBounds(newRect);
    }

    Motor* parentMotor = data.parentMotor;

    if (data.isAddSubmotorMode && !PreviewSubmotor(data, m_start, m_end)) {
        return {};
    }

    int motorX = 0, motorY = 0;
    switch (data.axis) {
    case (X):
        motorX = (m_end.x - m_start.x) / 2 + m_start.x;
        motorY = m_start.y;
        break;
    case (Y):
        motorX = m_end.x;
        motorY = (m_end.y - m_start.y) / 2 + m_start.y;
        break;
    default:
        break;
    }

    previewMotor.parentMotor = parentMotor;
	previewMotor.axis = data.axis;
    previewMotor.motorPos = CPoint(motorX, motorY);
    previewMotor.motorSize = CSize(m_motorSize.cx / 2, m_motorSize.cy / 2);
    previewMotor.motorSpeed = speed;
    previewMotor.strPos = m_start;
    previewMotor.endPos = m_end;
    previewMotor.isValid = true;
    if (parentMotor) {
        previewMotor.isRotating = parentMotor->isRotating;
        previewMotor.rotationAngle = (parentMotor->axis == T) ? 0.0 : parentMotor->rotationAngle;
    }

    return previewMotor;
}

// TODO: ȸ�� ������ ���� ���Ϳ� ���� ���͸� �߰��� ��, ȸ������ �ʴ� ���� �ذ� �ʿ�
bool MotorPreviewPanel::PreviewSubmotor(const MotorPreviewInputData& data, CPoint& start, CPoint& end)
{
    if (!data.parentMotor) return false;

    CPoint motorStart = data.parentMotor->motorPos - data.parentMotor->motorSize;
    CPoint motorEnd = data.parentMotor->motorPos + data.parentMotor->motorSize;

	CPoint rotationCenter = GetRotationCenter(data.parentMotor);

    start = m_motorTransform.SubToLogical(start, motorStart);
    end = m_motorTransform.SubToLogical(end, motorStart);

    if (start.x < motorStart.x || start.y < motorStart.y ||
        end.x > motorEnd.x || end.y > motorEnd.y) {
        return false;
    }
	return true;
}

Motor MotorPreviewPanel::PreviewRotatingmotor(const MotorPreviewInputData& data)
{
	CPoint motorPos(_ttoi(data.sx), _ttoi(data.sy));
	CPoint motorSize(_ttoi(data.width), _ttoi(data.width));
	double angularSpeed = _ttof(data.ex);
    
    if (angularSpeed <= 0 || angularSpeed > 3600) {
        AfxMessageBox(_T("ȸ�� �ӵ��� �ּ� 0��/s ~ �ִ� 3600��/s ���� ������ �մϴ�."));
        return {};
    }

    if (motorSize.x == 0) {
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

CPoint MotorPreviewPanel::GetRotationCenter(Motor* motor) {
    // T���̸� �� ��ġ�� ȸ�� �߽�
    if (motor->axis == T) {
        return motor->motorPos;
    }

    // ȸ�� ���̸�, �θ��� �θ� Ÿ�� �ٽ� Ž��
    if (motor->axis != T && motor->isRotating) {
        return GetRotationCenter(motor->parentMotor);
    }

    return CPoint(0, 0); // �⺻��
}