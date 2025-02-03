## [문제 1] 키워드 const의 의미

 const int num = 10; → 변수 num을 상수화!  
 const int * prt1 = &val1; → 포인터 prt1을 이용해서 val1의 값을 변경할 수 없음  
 int * const prt2 = &val2; → 포인터 prt2가 상수화 됨  
 const int * const prt3 = &val3; → 포인터 prt3가 상수화되었으며, prt3를 이용해서 val3의 값ㅇ르 변경할 수 없음  

## [문제 2] 실행중인 프로그램의 메모리 공간
```
 실행중인 프로그램은 운영체제로부터 메모리 공간을 할당받는데, 이는 크게 데이터, 스택, 힙 영역으로 나뉜다.
 각각의 영역에는 어떠한 형태의 변수가 할당되는지 설명해보자. 특히 C언어의 malloc과 free 함수에 관련해서도 설명해보자.
```

 → 데이터 : 전역변수가 저장되는 영역  
 → 스택 : 지역변수 및 매개변수가 저장되는 영역  
 → 힙 : malloc 함수호출에 의해 프로그램이 실행되는 과정에서 동적으로 할당이 이루어지는 영역  
 → malloc & free : malloc 함수호출에 의해 할당된 메모리 공간은 free 함수호출을 통해서 소멸하지 않으면 해제되지 않음.  

## [문제 3] Call-by-value vs. Call-by-reference
```
void SwapByValue(int num1, int num2) {
	int temp = num1;
	num1 = num2;
	num2 = temp;
} // Call-by-value
```

```
void SwapByRef(int * ptr1, int * ptr2) {
	int temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
} // Call-by-reference
```
