#include "framework.h"
#include "FV2.h"
#include <string>

#define MAX_LOADSTRING 500

// Идентификаторы кнопок
enum { OnMenuClicked1,
	   OnMenuClicked2,
	   OnMenuClicked3,
	   OnExit,
	   OnCLEAR,
	   OnRESULT,
	   OnCOPY };

// Идентификаторы полей для ввода
enum { DlgIndexNumberD = 100,
	   DlgIndexNumberF,
	   DlgIndexNumberV };

// Переменные окна (виджетов)
HWND hDiameter; 
HWND hDiameterStatic;
HWND hFlow;
HWND hVelocity;

// Глобальные переменные
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
TCHAR tchBuf[MAX_LOADSTRING];

// Входные параметры
double inputDiameter;
double inputFlow;

// Прототипы
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Идентификаторы кнопок
HWND hButton1, hButton2, hButton3;

// Функция меню
void MainWndAddMenus(HWND hWnd);

// Функция виджетов
void MainWndAddWidgets(HWND hWnd);

// Результат вычислений
double RESULT(double Diameter, double Flow);

// Функция для ввода дробного числа 
double GetDlgItemDouble(HWND hWnd, UINT uId);

// main
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FV2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FV2));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 100;
    wcex.cbWndExtra     = 100;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FV2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FV2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
       CW_USEDEFAULT, 0, 470, 370, nullptr, nullptr, hInstance, nullptr); // CW_USEDEFAULT, 0

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND: // действия с виджетами, кнопками, меню
        {
            // Выбор в меню
            switch (wParam)
            {
            case OnCLEAR:
                SetWindowTextA(hDiameter, "");
                SetWindowTextA(hFlow, "");
                SetWindowTextA(hVelocity, "");
                break;
            case OnRESULT:
				inputDiameter = GetDlgItemDouble(hWnd, DlgIndexNumberD);
				inputFlow = GetDlgItemDouble(hWnd, DlgIndexNumberF);
				if (inputDiameter == NULL || inputFlow == NULL)
                {
                    MessageBoxA(hWnd, "Не введены данные для расчета!", "Ошибка!", MB_OK);
                    break;
                }
				if (inputDiameter < 0.01 || inputDiameter > 100)
                {
                    MessageBoxA(hWnd, "Значение диаметра должно быть от 0.01 до 100!", "Ошибка!", MB_OK);
					break;
                }
				if (inputFlow < 0.01 || inputFlow > 9999999)
                {
                    MessageBoxA(hWnd, "Значение расхода должно быть от 0.01 до 9 999 999!", "Ошибка!", MB_OK);
					break;
                }
                else
                {
					SetWindowTextA(hVelocity, std::to_string(RESULT(inputDiameter, inputFlow)).c_str());
                }
               break;
            case OnExit:
                DestroyWindow(hWnd);
                break;
            case OnMenuClicked3:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

        }
        break;
    case WM_CREATE:
        MainWndAddMenus(hWnd);
        MainWndAddWidgets(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// функция создания меню
void MainWndAddMenus(HWND hWnd) 
{
    HMENU RootMenu = CreateMenu(); 
    HMENU SubMenu = CreateMenu(); 
    HMENU SubMenu2 = CreateMenu(); 

    AppendMenu(SubMenu, MF_STRING, OnCLEAR, L"Очистить");
    AppendMenu(SubMenu, MF_STRING, OnRESULT, L"Рассчитать");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, OnExit, L"Выход");

    AppendMenu(SubMenu2, MF_STRING, OnMenuClicked3, L"О программе");

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Файл"); // выпадающее меню
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu2, L"О программе");

    SetMenu(hWnd, RootMenu); // присваиваем меню нашему окну (hWnd)
}

// виджеты
void MainWndAddWidgets(HWND hWnd) 
{
    CreateWindowA("STATIC", "Введите значения диаметра и расхода.", WS_VISIBLE | WS_CHILD, 5, 5, 450, 50, hWnd, NULL, NULL, NULL);

    CreateWindowA("STATIC", "Для обозначения дробной части используйте точку.", WS_VISIBLE | WS_CHILD, 5, 25, 450, 50, hWnd, NULL, NULL, NULL);

    CreateWindowA("STATIC", "Диаметр трубопровода, м.", WS_VISIBLE | WS_CHILD, 5, 70, 590, 50, hWnd, NULL, NULL, NULL);

    hDiameter = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD, 5, 90, 100, 20, hWnd, (HMENU)DlgIndexNumberD, NULL, NULL);

    CreateWindowA("STATIC", "Расход в рабочих условиях, м3/ч.", WS_VISIBLE | WS_CHILD, 5, 120, 590, 50, hWnd, NULL, NULL, NULL);

    hFlow = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD, 5, 140, 100, 20, hWnd, (HMENU)DlgIndexNumberF, NULL, NULL);

    CreateWindowA("STATIC", "Скорость потока, м/с.", WS_VISIBLE | WS_CHILD, 5, 170, 590, 50, hWnd, NULL, NULL, NULL);

    hVelocity = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | ES_READONLY, 5, 190, 150, 20, hWnd, NULL, NULL, NULL);

    CreateWindowA("BUTTON", "ОЧИСТИТЬ", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 250, 200, 50, hWnd, (HMENU)OnCLEAR, NULL, NULL);

    CreateWindowA("BUTTON", "РАССЧИТАТЬ", WS_VISIBLE | WS_CHILD | ES_CENTER, 250, 250, 200, 50, hWnd, (HMENU)OnRESULT, NULL, NULL);
}

// функция расчета
double RESULT(double Diameter, double Flow) 
{
    double result = Flow / (3600 * 3.14159 * (Diameter / 2) * (Diameter / 2));

    return result;
}

// функция возврата значений типа double из виджетов (преобразование char в double)
double GetDlgItemDouble(HWND hWnd, UINT uId) 
{
    TCHAR tchBuf[500];
    GetDlgItemText(hWnd, uId, tchBuf, 500);
    return _ttof(tchBuf);
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
