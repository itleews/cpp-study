#include "pch.h"
#include <cmath>
#include "MotorTransform.h"

const double PI = 3.14159265358979323846;

Matrix3x3 MotorTransform::translation(double x, double y) {
    return Matrix3x3{ {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::rotation(double theta) {
    return Matrix3x3{ {
        {cos(theta), -sin(theta), 0},
        {sin(theta),  cos(theta), 0},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::scaling(double sx, double sy) {
    return Matrix3x3{ {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::makeTransform(double x, double y, double theta, double sx, double sy) {
    return translation(x, y) * rotation(theta) * scaling(sx, sy);
}

// Logical ��ǥ ���� ȸ���� �簢���� ȭ�� ��ǥ 4�� ��ȯ
void MotorTransform::GetRotatedRectScreenPoints(CPoint centerLogical, CSize sizeLogical, CPoint rotationCenter, double rotationAngleDeg, CPoint outPts[4])
{
    int halfW = sizeLogical.cx;
    int halfH = sizeLogical.cy;

    // �簢�� ������ (Logical ����, ȸ�� ��)
    CPoint ptsLogical[4] = {
        {centerLogical.x - halfW, centerLogical.y - halfH}, // �»�
        {centerLogical.x + halfW, centerLogical.y - halfH}, // ���
        {centerLogical.x + halfW, centerLogical.y + halfH}, // ����
        {centerLogical.x - halfW, centerLogical.y + halfH}  // ����
    };

    // ȸ�� �����Ͽ� ȭ�� ��ǥ�� ��ȯ
    for (int i = 0; i < 4; ++i) {
        outPts[i] = LogicalToScreen(ptsLogical[i], rotationCenter, rotationAngleDeg);
    }
}

CPoint MotorTransform::LogicalToScreen(CPoint logical, CPoint rotationCenter, double rotationAngle)
{
	double rotationAngleRad = rotationAngle * PI / 180.0;

	// ��, ���� �̵�
	Matrix3x3 transform = makeTransform(
		-m_logicalBounds.left,
		-m_logicalBounds.top,
		0.0, // ȸ�� ���� ��/�̵���
		m_zoomFactor,
		m_zoomFactor
	);

	// ȸ�� �߽� �������� ȸ�� ��Ʈ���� �����
	Matrix3x3 toCenter = translation(-rotationCenter.x, -rotationCenter.y);
	Matrix3x3 rotate = rotation(rotationAngleRad);
	Matrix3x3 back = translation(rotationCenter.x, rotationCenter.y);

	Matrix3x3 rotationTransform = back * rotate * toCenter;

	// �� ��ȯ
	Matrix3x3 pan = translation(m_panOffset.x, m_panOffset.y);

	// ��ü ��ȯ: �� * �ܽ����� * �����̵�
	Matrix3x3 viewTransform = pan * transform * rotationTransform;

	// ��ǥ ��ȯ
	POINT screen = viewTransform.transformPoint(logical.x, logical.y);
	return CPoint(screen.x, screen.y);
}

CPoint MotorTransform::ScreenToLogical(CPoint screen)
{
	Matrix3x3 invPan = translation(-m_panOffset.x, -m_panOffset.y);

	double invZoom = 1.0 / m_zoomFactor;
	Matrix3x3 invScale = scaling(invZoom, invZoom);

	Matrix3x3 invOrigin = translation(m_logicalBounds.left, m_logicalBounds.top);

	Matrix3x3 inverseTransform = invOrigin * invScale * invPan;

	POINT logical = inverseTransform.transformPoint(screen.x, screen.y);
	return CPoint(logical.x, logical.y);
}

CPoint MotorTransform::SubToLogical(CPoint subMotorPos, CPoint mainMotorPos, CPoint rotationCenter, double rotationAngle)
{
    double rotationAngleRad = rotationAngle * PI / 180.0;

    // 1. translation: ���� ���� ��ġ��ŭ �̵�
    Matrix3x3 translationMatrix = translation(mainMotorPos.x, mainMotorPos.y);
    POINT translatedPoint = translationMatrix.transformPoint(subMotorPos.x, subMotorPos.y);

    //// 2. ȸ�� �߽� �������� ȸ�� ��� ����
    //Matrix3x3 toCenter = translation(-rotationCenter.x, -rotationCenter.y);
    //Matrix3x3 rotate = rotation(rotationAngleRad);
    //Matrix3x3 back = translation(rotationCenter.x, rotationCenter.y);
    //Matrix3x3 rotationTransform = back * rotate * toCenter;

    //// 3. ȸ�� ����� ������ ����Ʈ�� ����
    //POINT finalPoint = rotationTransform.transformPoint(translatedPoint.x, translatedPoint.y);

    return CPoint(translatedPoint.x, translatedPoint.y);
}