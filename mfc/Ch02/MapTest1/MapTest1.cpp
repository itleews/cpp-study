// MapTest1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "framework.h"
#include "MapTest1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 애플리케이션 개체입니다.

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 여기에 애플리케이션 동작을 코딩합니다.
            wprintf(L"심각한 오류: MFC 초기화 실패\n");
            nRetCode = 1;
        }
        else
        {
            _tsetlocale(LC_ALL, _T("")); // 유니코드 한국어 출력에 필요

            // 맵 객체를 생성하고 초기화한다.
            CMapStringToString map;
			map[_T("사과")] = _T("Apple");
			map[_T("딸기")] = _T("Strawberry");
			map[_T("포도")] = _T("Grape");
			map[_T("우유")] = _T("Milk");
            // 특정 키 값을 가진 데이터를 검색한다.
            CString str;
            if (map.Lookup(_T("딸기"), str))
                _tprintf(_T("딸기 -> %s\n"), (LPCTSTR)str);
            
			_tprintf(_T("\n"));
            // 맵을 순회하면서 모든 키와 데이터값을 출력한다.
            POSITION pos = map.GetStartPosition();
            while (pos != NULL) {
                CString strKey, strValue;
                map.GetNextAssoc(pos, strKey, strValue);
				_tprintf(_T("%s -> %s\n"), (LPCTSTR)strKey, (LPCTSTR)strValue);
            }

            _tprintf(_T("\n"));
            map.RemoveKey(_T("우유")); // 키값 "우유"에 해당하는 데이터를 삭제한다.
			map[_T("수박")] = _T("Watermelon"); // map.SetAt(_T("수박"), _T("Watermelon"));
            // 맵을 순회하면서 모든 키와 데이터값을 출력한다.
            pos = map.GetStartPosition();
            while (pos != NULL) {
                CString strKey, strValue;
                map.GetNextAssoc(pos, strKey, strValue);
                _tprintf(_T("%s -> %s\n"), (LPCTSTR)strKey, (LPCTSTR)strValue);
            }

        }
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }

    return nRetCode;
}
