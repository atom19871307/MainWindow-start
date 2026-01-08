#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

CONST CHAR g_sz_CLASS_NAME[] = "CalcProjectClass";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Հաշվիչի փոփոխականներ / Переменные калькулятора
double g_a = 0, g_b = 0;   // Թվերը պահելու համար / Для хранения чисел
int g_operation = 0;       // Գործողության տեսակը / Тип операции (1:+, 2:-, 3:*, 4:/)
BOOL g_is_second_num = FALSE; // Արդյոք սկսել ենք գրել 2-րդ թիվը / Начинаем ли ввод 2-го числа
// Главная функция программы
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, INT nShow) 
{
    WNDCLASSEX wc = { 0 };                          //Структура класса окна
    wc.cbSize = sizeof(WNDCLASSEX);                 // Կառուցվածքի չափը։
    wc.lpfnWndProc = WndProc;                       // Функция обработки сообщений
    wc.hInstance = hInst;                           // Ծրագրի նույնացուցիչը։
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Մկնիկի սլաքի տեսքը։
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Պատուհանի ֆոնի գույնը / Цвет фона окна  //  (սպիտակ)։
    wc.lpszClassName = g_sz_CLASS_NAME;             // Имя класса

    //Регистрируем класс
    if (!RegisterClassEx(&wc)) return 0;

    // Էկրանի 25% և Կենտրոն
    //  25% экрана и запуск по центру
    INT sw = GetSystemMetrics(SM_CXSCREEN);     // Էկրանի լայնություն / Ширина экрана
    INT sh = GetSystemMetrics(SM_CYSCREEN);     // Էկրանի բարձրություն / Высота экрана
    INT ww = sw * 0.25;                         // Պատուհանի լայնություն / Ширина окна
    INT wh = sh * 0.45;                         // Պատուհանի բարձրություն / Высота окна

    // Կոորդինատների հաշվարկ կենտրոնի համար / Расчет координат для центра
    INT x_pos = (sw - ww) / 2;
    INT y_pos = (sh - wh) / 2;

    HWND hwnd = CreateWindowEx
    (
        0, g_sz_CLASS_NAME, "CalcHW - Windows Calculator", WS_OVERLAPPEDWINDOW,
        (sw - ww) / 2, (sh - wh) / 2, ww, wh, NULL, NULL, hInst, NULL

    );
    ShowWindow(hwnd, nShow);                    // Ցույց տալ պատուհանը էկրանին։
    MSG msg;
    // Հաղորդագրությունների մշակման ցիկլ / Цикл обработки сообщений
    while (GetMessage(&msg, NULL, 0, 0)) 
    { 
        TranslateMessage(&msg);                 // Թարգմանում է ստեղնաշարի սեղմումները։
        DispatchMessage(&msg);                  // Ուղարկում է հաղորդագրությունը WndProc-ին։
    }
    return (int)msg.wParam;
}

//WndProc ev Elementneri steghtsum (WM_CREATE)
//Aystegh steghtsvum en teqstayin dasht@ (Edit)ev kotchakner@:
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) 
{
    static HWND hEdit;                  // Հաշվիչի էկրանը / Дисплей калькулятора
    static HFONT hFont;                 //Шрифт

    switch (msg) 
    {
    case WM_CREATE: 
    {
        // Ստեղծում ենք տառատեսակ էլեմենտների համար / Создаем шрифт для элементов
        hFont = CreateFont
        (
            24, 0, 0, 0,
            FW_BOLD,
            0, 0, 0,
            ANSI_CHARSET,
            0, 0, 0, 0,
            "Arial"
        );

        // Հաշվիչի էկրանը / Дисплей калькулятора (Edit)
        hEdit = CreateWindowEx
        (
            WS_EX_CLIENTEDGE,
            "Edit", "0",
            WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
            20, 20,
            320, 50,
            hwnd, 
            (HMENU)200,
            GetModuleHandle(NULL), 
            NULL
        );
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);        // Կիրառում ենք շրիֆտը էկրանի վրա։

        // Թվային կոճակներ (1-9) ցիկլով / Цикл для создания кнопок (1-9)
         // Умножение// 1-ից 9 թվերի կոճակների ստեղծում երկու ցիկլով (3x3 ցանց)
        char buf[2];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++) 
            {
                int num = (i * 3 + j + 1);                          // Հաշվում ենք թիվը (1, 2, 3...)
                sprintf(buf, "%d", num);
                HWND hBtn = CreateWindow
                (
                    "Button",
                    buf, 
                    WS_CHILD | WS_VISIBLE,
                    20 + j * 65,
                    80 + i * 65, 
                    60, 60, 
                    hwnd,
                    (HMENU)(100 + num), 
                    NULL,
                    NULL
                );
                SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
            }
        }
        // (Այստեղ ստեղծվում են նաև 0, C և գործողությունների կոճակները նույն ձևով)
        // 0, C և Գործողություններ / 0, C и Операции
        HWND hBtn0 = CreateWindow
        (
            "Button",
            "0",
            WS_CHILD | WS_VISIBLE, 
            20, 275,
            125, 60, 
            hwnd,
            (HMENU)100,
            NULL,
            NULL
        );
        HWND hBtnC = CreateWindow
        (
            "Button", 
            "C", 
            WS_CHILD | WS_VISIBLE, 
            150, 275, 
            60, 60, 
            hwnd, 
            (HMENU)400,
            NULL, 
            NULL
        );

        // Գործողության կոճակներ / Кнопки операций
        const char* ops[] = { "+", "-", "*", "/", "=" };
        int opIDs[] = { 301, 302, 303, 304, 305 };
        for (int i = 0; i < 5; i++)
        {
            HWND hOp = CreateWindow
            (
                "Button",
                ops[i],
                WS_CHILD | WS_VISIBLE, 
                215 + (i == 4 ? 0 : 0), 
                80 + (i * 65), 60, 60,
                hwnd, 
                (HMENU)opIDs[i],
                NULL, 
                NULL
            );
            if 
                (i == 4) SetWindowPos
                (
                    hOp,
                    NULL,
                    280, 80, 
                    60, 255, 
                    SWP_NOZORDER
                ); // "=" կոճակի դիրքը
            SendMessage
            (
                hOp, 
                WM_SETFONT, 
                (WPARAM)hFont,
                TRUE);
        }
        break;
    }
    case WM_COMMAND: 
    {
        int id = LOWORD(wp);                                 // Получить ID кнопки
                                                            
        if (id >= 100 && id <= 109)                          // Եթե սեղմվել է թիվ (100-109) / Если нажата цифра
        {
            char current[256], digit[2];
            GetWindowText(hEdit, current, 256);             // Прочитать текст  // Վերցնում ենք այն, ինչ հիմա գրված է էկրանին։
            sprintf(digit, "%d", id - 100);                 // Превратить ID в число  // Կոճակի ID-ից ստանում ենք թիվը (օրինակ՝ 105-ից ստանում ենք "5")։

            if (strcmp(current, "0") == 0 || g_is_second_num)
            {
                SetWindowText(hEdit, digit);                //  Написать новое число  // Եթե "0" է կամ նոր թիվ ենք սկսում, ուղղակի գրում ենք թիվը։
                g_is_second_num = FALSE;
            }
            else 
            {
                strcat(current, digit);                         // Добавить цифру к имеющейся  // Եթե արդեն թիվ կա, նոր թիվը կպցնում ենք կողքից։
                SetWindowText(hEdit, current);
            }
        }
        //Обработка операций (+, -, *, /)
        // Գործողությունների մշակում  (+, -, *, /)
        if (id >= 301 && id <= 304)
        {
            char val[256]; GetWindowText(hEdit, val, 256);
            g_a = atof(val);                            // Պահում ենք առաջին թիվը / Сохраняем первое число
            g_operation = id - 300;                     // Ֆիքսում ենք գործողությունը / Фиксируем операцию
            g_is_second_num = TRUE;                     // Նշում ենք, որ հաջորդ թիվը սկսելիս էկրանը մաքրվի։
        }
        // Հավասարման նշան (=) / Знак равно
        // Հավասարման նշան (=) և 0-ի բաժանման ստուգում
        if (id == 305) 
        {
            char val[256]; GetWindowText(hEdit, val, 256);
            g_b = atof(val); // Պահում ենք երկրորդ թիվը / Сохраняем второе число
            double res = 0;
            if (g_operation == 4 && g_b == 0) // Ստուգում ենք՝ արդյո՞ք բաժանում է 0-ի։
            {
                SetWindowText(hEdit, "ERROR, ОШИБКА "); // Ձեր խնդրած հաղորդագրությունը։
            }
            else
            {
                switch (g_operation)
                {
                case 1: res = g_a + g_b; break;                     // Сложение	    (Плюс)
                case 2: res = g_a - g_b; break;                     // Вычитание    (Минус)
                case 3: res = g_a * g_b; break;                     // Умножение    (Умножить)
                //Делим a на b, но проверяем на ноль                // (Բաժանում ենք a - ն b - ի վրա, բայց ստուգում ենք զրոյի դեպքը)
                case 4: res = (g_b != 0) ? g_a / g_b : 0; break;    // 0-ի բաժանման ստուգում

                }
                sprintf(val, "%g", res);                            //Конвертация в текст // Արդյունքը դարձնում ենք տեքստ։
                SetWindowText(hEdit, val);
            }
            g_is_second_num = TRUE;
        }
        // Մաքրում (C) / Очистка
        if (id == 400) SetWindowText(hEdit, "0");
        break;
    }
    case WM_SIZE:
    case WM_MOVE: // Երբ պատուհանը շարժվում է կամ չափը փոխվում է։
    {
        //  Չափսերի և դիրքի արտացոլում վերնագրում
        //  Отображение координат и размера в заголовке
        RECT r; GetWindowRect(hwnd, &r);  // Ստանում ենք պատուհանի նոր չափերն ու դիրքը։              
        char t[256];
        sprintf
        (
            t,
            "CalcHW - Pos: %dx%d, Size: %dx%d",
            r.left, r.top, r.right - r.left, r.bottom - r.top
        );
        SetWindowText(hwnd, t);             // Գրում ենք այդ տվյալները պատուհանի վերնագրում։
        break;
    }
    case WM_DESTROY:                        // Երբ ծրագիրը փակվում է։
        DeleteObject(hFont);                // Ջնջում ենք շրիֆտը հիշողությունից։
        PostQuitMessage(0);                 // Ծրագրի ավարտ։
        break;
    default: return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
