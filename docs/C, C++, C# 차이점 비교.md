## C언어, C++와 C#의 차이점

## 1. 개요
C, C++, C#은 모두 강력한 프로그래밍 언어이지만, 각각의 목적과 특징이 다릅니다. C는 하드웨어와 밀접하게 연관된 시스템 프로그래밍에 적합하며, C++는 객체 지향 프로그래밍(OOP)을 지원하는 언어입니다. C#은 .NET 프레임워크에서 주로 사용되며, 객체 지향 프로그래밍을 지원하고, 많은 고수준 기능을 제공합니다. 아래는 C, C++, C#의 주요 차이점입니다.

---

## 2. 주요 차이점 비교

| 항목         | C 언어                          | C++ 언어                           | C# 언어                               |
|-------------|--------------------------------|----------------------------------|--------------------------------------|
| **패러다임** | 절차 지향 프로그래밍 (Procedural) | 객체 지향 프로그래밍 (OOP) + 절차 지향 | 객체 지향 프로그래밍 (OOP)             |
| **클래스/객체** | 지원하지 않음                   | 지원 (클래스, 객체, 상속, 다형성 등) | 지원 (클래스, 객체, 상속, 다형성 등)   |
| **함수 오버로딩** | 지원하지 않음                   | 지원 (같은 이름의 함수 여러 개 정의 가능) | 지원 (같은 이름의 함수 여러 개 정의 가능) |
| **네임스페이스** | 없음                            | 있음 (`namespace` 키워드 사용) | 있음 (`namespace` 키워드 사용)        |
| **기본 입력/출력** | `printf`, `scanf` 사용        | `cout`, `cin` 사용 (C 스타일도 사용 가능) | `Console.WriteLine`, `Console.ReadLine` 사용 |
| **메모리 관리** | `malloc`, `free` 사용         | `new`, `delete` 사용 가능 (RAII 지원) | 자동 메모리 관리 (GC: Garbage Collection) |
| **예외 처리** | 없음                            | 있음 (`try-catch` 문법) | 있음 (`try-catch-finally` 문법)         |
| **템플릿** | 없음                            | 있음 (제네릭 프로그래밍 지원) | 있음 (제네릭 프로그래밍 지원)           |
| **STL 지원** | 없음                            | 있음 (벡터, 리스트, 맵 등 표준 라이브러리 제공) | 있음 (LINQ, List, Dictionary 등 제공)  |
| **언어 수준** | 저수준 언어 (하드웨어와 밀접)   | 중수준 언어 (저수준 및 고수준 모두 지원) | 고수준 언어 (주로 애플리케이션 개발)    |
| **기타 특징** | 운영체제에 가까운 프로그래밍에 적합 | 시스템 및 애플리케이션 프로그래밍에 적합 | Windows 애플리케이션 및 웹 애플리케이션 개발에 최적화 |

---

## 3. 코드 비교

### C 언어 예제 (절차 지향)
```c
#include <stdio.h>

void sayHello() {
    printf("Hello, C!\n");
}

int main() {
    sayHello();
    return 0;
}
```

### C++ 언어 예제 (객체 지향)
```cpp
#include <iostream>

class Greeter {
public:
    void sayHello() {
        std::cout << "Hello, C++!" << std::endl;
};

int main() {
    Greeter greeter;
    greeter.sayHello();
    return 0;
}
```

### C# 언어 예제 (객체 지향)
``` cs
using System;

class Greeter {
    public void SayHello() {
        Console.WriteLine("Hello, C#!");
    }
}

class Program {
    static void Main() {
        Greeter greeter = new Greeter();
        greeter.SayHello();
    }
}
```

---

## 4. 결론
- **C**는 절차 지향적이며 하드웨어와 밀접한 저수준 프로그래밍에 적합합니다.
- **C++**는 객체 지향적이며 시스템 프로그래밍과 고급 애플리케이션 개발에 적합합니다.
- **C#**은 고수준 언어로, 주로 Windows 애플리케이션과 웹 애플리케이션 개발에 사용됩니다. 자동 메모리 관리 및 풍부한 라이브러리와 함께 개발 효율성을 높이는 데 유리합니다.
