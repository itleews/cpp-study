#pragma once
#include <vector>
#include "../Motor/MotorManager.h"

struct DisplayRow{
    CString id;
    CString type;
    CString strPos;
    CString endPos;
    CString relMotorPos;
    CString absMotorPos;
};

class MotorListPanel {
public:
    void DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors);
    void BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows);
};