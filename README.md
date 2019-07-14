# TestProblem
Solution of test problem
Решение собирал в visual studio 2017, стандарт С++17. 
Запускал через cmd файл .exe в папке Debug с параметрами вида
Examples of cmd commands:
			TestProblem.exe C:\check --out-prefix=sorted_ --content-type=i --sort-mode=a
			TestProblem.exe C:\check --out-prefix=sorted_ --content-type=s --sort-mode=a
			TestProblem.exe C:\check --out-prefix=sorted_ --content-type=i --sort-mode=d
			TestProblem.exe C:\check --out-prefix=sorted_ --content-type=s --sort-mode=d
Параметр --sort-mode отвечает за убывающую или возврастающую сортировку.
Параметр --content-type отвечает за тип сортируемых данных: int или string. 
Должен работать для int, для string не сделал. 
Исключения не ловил. 
Многое не успел сдеать/привести в порядок.
