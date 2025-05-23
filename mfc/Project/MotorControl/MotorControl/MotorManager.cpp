#include "pch.h"
#include "MotorManager.h"
#include "ChildView.h"
#include <unordered_set>

Motor* MotorManager::AddMotor(
    Motor* parentMotor,
    bool isXDirection,
    CPoint strPos, CPoint endPos, CPoint motorPos,
    CSize motorSize, int motorSpeed)
{
    nextId = GetMaxId() + 1;

    Motor* newMotor = new Motor(
        nextId, isXDirection,
        strPos, endPos, motorPos,
        motorSize, motorSpeed);

    motorMap[newMotor->m_id] = newMotor;
	newMotor->parentMotor = parentMotor; // ⭐ 부모 설정

    if (parentMotor)
    {
        parentMotor->children.push_back(newMotor); // ⭐ 부모에 연결
    }
    else
    {
        rootMotors.push_back(newMotor); // ⭐ 최상위 Motor 리스트에 추가
    }

    return newMotor;
}

int MotorManager::GetMaxId() {
    int maxId = 0;
    for (auto it = motorMap.begin(); it != motorMap.end(); ++it) {
        int id = it->first;
        if (id > maxId) maxId = id;
    }
    return maxId;
}

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
        file << "ID,축,시작 위치,끝 위치,모터 현재 위치,모터 절대 위치,모터 크기,모터 속도,상위 모터\n";

        // 모터 리스트 저장 (재귀적으로 모든 모터 저장)
        for (auto motor : rootMotors) {
            SaveMotorRecursive(file, motor);
        }

        file.close();
    }
}

void MotorManager::SaveMotorRecursive(std::ofstream& file, Motor* motor, CPoint parentOffset) {  
    CPoint relativeStr = motor->strPos - parentOffset;
    CPoint relativeEnd = motor->endPos - parentOffset;
    CPoint relativeMotor = motor->motorPos - parentOffset;

    std::ostringstream oss;
    oss << motor->m_id << ","                         // ID
        << (motor->isX ? "X" : "Y") << ","           // 축
        << "\"" << relativeStr.x << ";" << relativeStr.y << "\","  // 시작 위치
        << "\"" << relativeEnd.x << ";" << relativeEnd.y << "\","  // 끝 위치
        << "\"" << relativeMotor.x << ";" << relativeMotor.y << "\","  // 모터 상대 위치
        << "\"" << motor->motorPos.x << ";" << motor->motorPos.y << "\","  // 모터 절대 위치
        << "\"" << motor->motorSize.cx << ";" << motor->motorSize.cy << "\","  // 모터 사이즈
        << motor->motorSpeed << ","                   // 속도
        << (motor->parentMotor ? std::to_string(motor->parentMotor->m_id) : "");  // 부모 ID

    file << oss.str() << "\n";  

    for (auto child : motor->children) {  
        SaveMotorRecursive(file, child, motor->motorPos - motor->motorSize);  
    }  
}


// 불러오기 기능 (CSV 형식으로 불러오기)
void MotorManager::LoadMotorData() {
    CFileDialog fileDialog(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST, _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

    if (fileDialog.DoModal() == IDOK) {
        rootMotors.clear();

        CString filePath = fileDialog.GetPathName();
        std::ifstream file(CT2A(filePath.GetString()));

        if (!file.is_open()) {
            AfxMessageBox(_T("파일을 열 수 없습니다."));
            return;
        }

        std::string line;
        std::getline(file, line);  // 헤더 건너뛰기

        while (std::getline(file, line)) {
            int parentId = -1;
            Motor* motor = ParseMotor(line, parentId);
            motorMap[motor->m_id] = motor;
            motorsWithParents.emplace_back(motor, parentId);
        }
        file.close();

        rootMotors.clear();
        for (auto& pair : motorsWithParents) {
            Motor* motor = pair.first;
            int parentId = pair.second;

            if (parentId == -1) {
                motor->parentMotor = nullptr;
                rootMotors.push_back(motor);
            }
            else {
                auto it = motorMap.find(parentId);
                if (it != motorMap.end()) {
                    motor->parentMotor = it->second;
                    it->second->children.push_back(motor);
                }
                else {
                    motor->parentMotor = nullptr;
                    rootMotors.push_back(motor);
                }
            }
        }

    }
}

CPoint ParsePoint(const std::string& s) {
    int x = 0, y = 0;
    size_t semicolonPos = s.find(';');
    if (semicolonPos != std::string::npos) {
        x = std::stoi(s.substr(0, semicolonPos));
        y = std::stoi(s.substr(semicolonPos + 1));
    }
    return CPoint(x, y);
}

Motor* MotorManager::ParseMotor(const std::string& line, int& outParentId) {
    std::istringstream iss(line);
    std::string token;

    std::getline(iss, token, ',');  // ID
    int id = std::stoi(token);

    std::getline(iss, token, ',');  // isX ("X" or "Y")
    bool isX = (token == "X");

    std::getline(iss, token, ',');  // 시작 위치 "(x;y)"
    token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    CPoint strPos = ParsePoint(token);

    std::getline(iss, token, ',');  // 끝 위치
    token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    CPoint endPos = ParsePoint(token);

    std::getline(iss, token, ',');  // 상대 위치
    token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    CPoint relativeMotorPos = ParsePoint(token);

    std::getline(iss, token, ',');  // 절대 위치
    token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    CPoint motorAbsPos = ParsePoint(token);

    std::getline(iss, token, ',');  // 크기
    token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    CSize motorSize = ParsePoint(token);

    std::getline(iss, token, ',');  // 속도
    int motorSpeed = std::stoi(token);

    std::getline(iss, token, ',');  // 부모 ID
    outParentId = token.empty() ? -1 : std::stoi(token);

    // 부모 ID가 있으면 모터 좌표 수정
    if (outParentId != -1) {
        auto it = motorMap.find(outParentId);
        if (it != motorMap.end()) {
            Motor* parentMotor = it->second;
            CPoint parentOffset = parentMotor->motorPos - parentMotor->motorSize;
            strPos += parentOffset;
            endPos += parentOffset;
            relativeMotorPos += parentOffset;
		}
    }

    Motor* motor = new Motor(id, isX ? _T("X") : _T("Y"),
        strPos, endPos, relativeMotorPos, motorSize, motorSpeed);
    motor->motorPos = motorAbsPos;

    return motor;
}
