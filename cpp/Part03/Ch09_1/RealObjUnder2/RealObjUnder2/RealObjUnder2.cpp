#include <iostream>
using namespace std;

// 클래스 Data를 흉내 낸 영역
typedef struct Data {
	int data;
	void (*ShowData)(Data*);
	void (*AddData)(Data*, int);
} Data;

void ShowData(Data* THIS) {
	cout << "Data: " << THIS->data << endl;
}
void Add(Data* THIS, int num) {
	THIS->data += num;
}

// 적절히 변경된 main 함수
int main(void) {
	Data obj1 = { 15, ShowData, Add };
	Data obj2 = { 7, ShowData, Add };

	obj1.AddData(&obj1, 17);
	obj2.AddData(&obj2, 9);
	obj1.ShowData(&obj1);
	obj2.ShowData(&obj2);
	return 0;
}