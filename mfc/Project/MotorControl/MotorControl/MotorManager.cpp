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
    // 파일 저장 다이얼로그 띄우기
    CFileDialog fileDialog(FALSE, _T("csv"), NULL, OFN_OVERWRITEPROMPT, _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

    if (fileDialog.DoModal() == IDOK) {
        CString filePath = fileDialog.GetPathName();

        // 선택한 경로로 파일 저장
        std::ofstream file(CT2A(filePath.GetString()));  // CString을 char*로 변환

        if (!file.is_open()) {
            AfxMessageBox(_T("파일을 저장할 수 없습니다."));
            return;
        }
        // 헤더 작성
        file << "ID,축 방향,시작 X,시작 Y,끝 X,끝 Y,모터 X, 모터 Y, 모터 W, 모터 H, 하위 모터\n";

        // 모터 리스트 저장
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

        AfxMessageBox(_T("모터 저장이 완료되었습니다."));
	}
    else {
        AfxMessageBox(_T("파일 저장이 취소되었습니다."));
    }
}

// CSV 파일을 읽고 Motor 객체로 변환하는 함수
Motor* MotorManager::ParseMotor(const std::string& line) {
    std::istringstream iss(line);
    std::string token;

    // CSV에서 각 필드를 추출
    std::getline(iss, token, ',');  // ID
    int id = std::stoi(token);

    std::getline(iss, token, ',');  // isX
    bool isX = token == "1";  // "1"이면 true, "0"이면 false

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

    // Motor 객체 생성
    Motor* motor = new Motor(id, isX ? _T("X") : _T("Y"),
        CPoint(strPosX, strPosY),
        CPoint(endPosX, endPosY),
        CPoint(motorPosX, motorPosY),
        CSize(motorSizeX, motorSizeY));

    return motor;
}

// 불러오기 기능 (CSV 형식으로 불러오기)
void MotorManager::LoadMotorData() {
    // 파일 열기 다이얼로그 띄우기
    CFileDialog fileDialog(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST, _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

    if (fileDialog.DoModal() == IDOK) {
        CString filePath = fileDialog.GetPathName();

        // 선택한 파일 열기
        std::ifstream file(CT2A(filePath.GetString())); // CString을 char*로 변환

        if (!file.is_open()) {
            AfxMessageBox(_T("파일을 열 수 없습니다."));
            return;
        }

        std::string line;

        // 첫 번째 줄은 헤더이므로 건너뛰기
        std::getline(file, line);

        // 각 줄에 대해 파싱
        while (std::getline(file, line)) {
            motorList.push_back(ParseMotor(line));  // CSV 한 줄을 Motor 객체로 변환
        }

        file.close();
    }
}