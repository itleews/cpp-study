#include "pch.h"
#include "MotorManager.h"


Motor* MotorManager::AddMotor(
    bool isXDirection,
	CPoint strPos, CPoint endPos, CPoint motorPos,
    CSize motorSize)
{
    Motor* newMotor = new Motor(
        nextId++, isXDirection,
		strPos, endPos, motorPos,
        motorSize);

    motorList.push_back(newMotor);
    return newMotor;
}


//void MotorManager::MoveAxis(int id, CPoint dest) {
//	MotorAxis* axis = FindAxis(id);
//	if (axis) {
//		axis->MoveTo(dest);
//	}
//}

void MotorManager::SaveMotorData() {
    // ���� ���� ���̾�α� ����
    CFileDialog fileDialog(FALSE, _T("csv"), NULL, OFN_OVERWRITEPROMPT, _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

    if (fileDialog.DoModal() == IDOK) {
        CString filePath = fileDialog.GetPathName();

        // ������ ��η� ���� ����
        std::ofstream file(CT2A(filePath.GetString()));  // CString�� char*�� ��ȯ

        if (!file.is_open()) {
            AfxMessageBox(_T("������ ������ �� �����ϴ�."));
            return;
        }
        // ��� �ۼ�
        file << "ID,�� ����,���� X,���� Y,�� X,�� Y,���� X, ���� Y, ���� W, ���� H, ���� ����\n";

        // ���� ����Ʈ ����
        for (auto motor : motorList) {
            std::ostringstream oss;
            oss << motor->m_id << ","
                << (motor->isX ? "X" : "Y") << ","
                << motor->strPos.x << ","
                << motor->strPos.y << ","
                << motor->endPos.x << ","
                << motor->endPos.y << ","
                << motor->motorPos.x << ","
                << motor->motorPos.y << ","
                << motor->motorSize.cx << ","
                << motor->motorSize.cy;

            file << oss.str() << "\n";
        }

        file.close();

        AfxMessageBox(_T("���� ������ �Ϸ�Ǿ����ϴ�."));
	}
    else {
        AfxMessageBox(_T("���� ������ ��ҵǾ����ϴ�."));
    }
}

// CSV ������ �а� Motor ��ü�� ��ȯ�ϴ� �Լ�
Motor* MotorManager::ParseMotor(const std::string& line) {
    std::istringstream iss(line);
    std::string token;

    // CSV���� �� �ʵ带 ����
    std::getline(iss, token, ',');  // ID
    int id = std::stoi(token);

    std::getline(iss, token, ',');  // isX
    bool isX = token == "1";  // "1"�̸� true, "0"�̸� false

    std::getline(iss, token, ',');  // strPos.x
    int strPosX = std::stoi(token);

    std::getline(iss, token, ',');  // strPos.y
    int strPosY = std::stoi(token);

    std::getline(iss, token, ',');  // endPos.x
    int endPosX = std::stoi(token);

    std::getline(iss, token, ',');  // endPos.y
    int endPosY = std::stoi(token);

    std::getline(iss, token, ',');  // motorPos.x
    int motorPosX = std::stoi(token);

    std::getline(iss, token, ',');  // motorPos.y
    int motorPosY = std::stoi(token);

    std::getline(iss, token, ',');  // motorSize.cx
    int motorSizeX = std::stoi(token);

    std::getline(iss, token, ',');  // motorSize.cy
    int motorSizeY = std::stoi(token);

    // Motor ��ü ����
    Motor* motor = new Motor(id, isX ? _T("X") : _T("Y"),
        CPoint(strPosX, strPosY),
        CPoint(endPosX, endPosY),
        CPoint(motorPosX, motorPosY),
        CSize(motorSizeX, motorSizeY));

    return motor;
}

// �ҷ����� ��� (CSV �������� �ҷ�����)
void MotorManager::LoadMotorData() {
    // ���� ���� ���̾�α� ����
    CFileDialog fileDialog(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST, _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

    if (fileDialog.DoModal() == IDOK) {
        CString filePath = fileDialog.GetPathName();

        // ������ ���� ����
        std::ifstream file(CT2A(filePath.GetString())); // CString�� char*�� ��ȯ

        if (!file.is_open()) {
            AfxMessageBox(_T("������ �� �� �����ϴ�."));
            return;
        }

        std::string line;

        // ù ��° ���� ����̹Ƿ� �ǳʶٱ�
        std::getline(file, line);

        // �� �ٿ� ���� �Ľ�
        while (std::getline(file, line)) {
            motorList.push_back(ParseMotor(line));  // CSV �� ���� Motor ��ü�� ��ȯ
        }

        file.close();
    }
}