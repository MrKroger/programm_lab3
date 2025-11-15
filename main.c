#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define SIZE_MENU 7
#define MAX_INPUT 100

// Структура для хранения данных интеграла
typedef struct {
    double a;          // нижняя граница
    double b;          // верхняя граница
    double step;       // шаг интегрирования
    double result;     // результат вычисления
    double accuracy;   // точность
    int calculated;    // флаг вычисления
} IntegralData;

// Прототипы функций
void Menu(const char** list_menu, int point);
void InputBounds(IntegralData* data);
void InputStep(IntegralData* data);
void CalculateIntegral(IntegralData* data);
double F(double a);
double Calcul_Accuracy(double a, double b);
void DetermineAccuracy(IntegralData* data);
void PrintResult(IntegralData* data);
void AboutProgram();
double Function(double x);
void ClearInputBuffer();

int main() {
    short is_exit = 0;
    int point = 0;

    // Подключение русского языка
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Выделение памяти для меню
    const char** list_menu = malloc(SIZE_MENU * sizeof(char*));
    list_menu[0] = "Ввод границ интеграла";
    list_menu[1] = "Ввод шага интегрирования";
    list_menu[2] = "Расчёт интеграла";
    list_menu[3] = "Определить погрешность";
    list_menu[4] = "Вывод результата";
    list_menu[5] = "О программе";
    list_menu[6] = "Выход";

    // Инициализация данных интеграла
    IntegralData data = {0, 0, -1, 0, 0.001, 0};

    while (!is_exit) {
        system("cls");
        Menu(list_menu, point);

        int key = _getch();
        switch(key) {
            case 72:    // стрелка вверх
                if (point==0) point=SIZE_MENU;
                if (point > 0) point--;
                break;
            case 80:    // стрелка вниз
                if (point==SIZE_MENU) point=0;
                if (point < SIZE_MENU - 1) point++;
                break;
            case 13:    // Enter - выбор пункта
                system("cls");
                switch(point) {
                    case 0: InputBounds(&data); break;
                    case 1: InputStep(&data); break;
                    case 2: CalculateIntegral(&data); break;
                    case 3: DetermineAccuracy(&data); break;
                    case 4: PrintResult(&data); break;
                    case 5: AboutProgram(); break;
                    case 6: is_exit = 1; break;
                }
                if (point != 6) {
                    printf("\nНажмите любую клавишу для возврата в меню...");
                    _getch();
                }
                break;
            case 27:    // Escape - выход
                is_exit = 1;
                break;
        }
    }

    free(list_menu);
    printf("Программа завершена.\n Спасибо, что воспользовались моей программой\n");
    return 0;
}

void Menu(const char** list_menu, int point) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║           ВЫЧИСЛЕНИЕ ИНТЕГРАЛА               ║\n");
    printf("║          (1-я и 2-я четверти)                ║\n");
    printf("╠══════════════════════════════════════════════╣\n");

    for (int i = 0; i < SIZE_MENU; i++) {
        if (point == i) {printf("║ → %-40s  \n", list_menu[i]);}
        else {printf("║   %-40s  \n", list_menu[i]);}
    }

    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ Управление: ↑↓ - выбор, Enter - выполнить    ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Текущий выбор: %d\n", point+1);
}

void InputBounds(IntegralData* data) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║             ВВОД ГРАНИЦ ИНТЕГРАЛА            ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    printf("Введите нижнюю границу a: ");
    double a,b;
    char c;
    while (1) {
        int result = scanf("%lf%c", &a, &c);
        if (result == 2 && (c == '\n' || c==' \n')) {break;}
        else {printf("\nОшибка! Введите одно число: ");
            ClearInputBuffer();
        }
    }

    printf("Введите верхнюю границу b: ");
    while (1) {
        int result = scanf("%lf%c", &b, &c);
        if (result == 2 && (c == '\n' || c==' \n')) {break;}
        else {printf("\nОшибка! Введите одно число: ");
            ClearInputBuffer();
        }
    }
    data->a=a; data->b=b;
    // Проверка корректности границ
    if (data->a >= data->b) {
        printf("Ошибка: нижняя граница должна быть меньше верхней!\n");
        printf("Автоматическая замена значений...\n");
        double temp = data->a;
        data->a = data->b;
        data->b = temp;
    }

    data->calculated = 0;  // Сбрасываем флаг вычисления
    printf("\nГраницы установлены: a = %.3lf, b = %.3lf\n", data->a, data->b);
}

void InputStep(IntegralData* data) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║           ВВОД ШАГА ИНТЕГРИРОВАНИЯ           ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    printf("Текущий шаг: Не установлен\n");
    printf("Введите новый шаг интегрирования (10^-10<=step>=1): ");

    double new_step;
    char c;
    while (1) {
        int result=scanf("%lf%c",&new_step,&c);
        if (result == 2 && (c == '\n' || c==' \n')) {
            if (new_step > 0.00000000001 && new_step<=1) {data->step = new_step;break;}
            else {printf("Шаг должен быть: (10^(-10)<=step>=1) ! Введите снова: ");}
        } else {
            // Ошибка: либо не число, либо лишние символы
            printf("Ошибка! Введите только одно положительное число: ");
            ClearInputBuffer();
        }
    }
    data->calculated = 0;  // Сбрасываем флаг вычисления
    printf("Шаг установлен: %.10lf\n", data->step);
}

double Function(double x) {
    double y= pow(x,3)- pow(x,2)+ x+7;
    if (y>=0){return y;}
    else return 0;
}

void CalculateIntegral(IntegralData* data) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║              РАСЧЁТ ИНТЕГРАЛА                ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    if ((data->step == -1) || (data->b - data->a)==0) {
        printf("Ошибка: шаг интегрирования не установлен, либо границы совпадают! Вернись и исправь!\n");
        return;
    }

    printf("Вычисление интеграла методом Симпсона...\n");
    printf("Границы: [%.3lf, %.3lf]\n", data->a, data->b);
    printf("Шаг: %.11lf\n\n", data->step);
    printf("|..");Sleep(500);printf("..");Sleep(500);printf("..");
    Sleep(500);printf("..");Sleep(500);printf("..");Sleep(500);printf("..");Sleep(500);printf("..|\n");Sleep(300);

    int n = (int)((data->b - data->a) / data->step);  // количество отрезков

    if (n % 2 != 0) {n++;}

    double h = (data->b - data->a) / n;  // пересчитываем шаг
    double sum = Function(data->a) + Function(data->b);

    for (int i = 1; i < n; i += 2) {sum += 4 * Function(data->a + i * h);}

    for (int i = 2; i < n; i += 2) {sum += 2 * Function(data->a + i * h);}

    data->result = (h/3)*(sum);
    data->calculated = 1;

    printf("\n✅ Интеграл успешно вычислен!\n");
    printf("Количество шагов: %d\n", n);
    printf("Результат: %.8lf\n", data->result);
}

double F(double a) {
  return (pow(a,4))/4.0 - (pow(a,3))/3.0 +(pow(a,2))/2.0+7*(a);
}
double Calcul_Accuracy(double a, double b) {
  double zero_fun =2.10487;
  if (a <= zero_fun && b >= zero_fun) return F(b) - F(zero_fun);
  else if (a <= zero_fun && b <= zero_fun) return 0;
  else if (a >= zero_fun && b >= zero_fun) return F(b) - F(a);
  return 0;
}

void DetermineAccuracy(IntegralData* data) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║             ОПРЕДЕЛЕНИЕ ПОГРЕШНОСТИ          ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    printf("|..");Sleep(500);printf("..");Sleep(500);printf("..");
    Sleep(500);printf("..");Sleep(500);printf("..");Sleep(500);printf("..");Sleep(500);printf("..|\n");Sleep(300);

    double H_L=Calcul_Accuracy(data->a,data->b);
    printf("Абсолютно значение: %lf\n",H_L);
    double new_accuracy=(abs((data->result)-H_L)/H_L)*100;
    printf("Погрешность установлена: %lf %\n", data->accuracy);
}

void PrintResult(IntegralData* data) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║               РЕЗУЛЬТАТЫ                     ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    printf("Границы интегрирования:\n");
    printf(" ▷ a = %.6lf\n", data->a);
    printf(" ▷ b = %.6lf\n", data->b);
    printf(" ▷ Длина интервала: %.6lf\n\n", data->b - data->a);

    printf("Параметры вычисления:\n");
    printf(" ▷ Шаг интегрирования: %.6lf\n", data->step);

    if (data->calculated) {
        printf("РЕЗУЛЬТАТ ВЫЧИСЛЕНИЙ:\n");
        printf(" ▷ ∫f(x)dx = %.8lf\n", data->result);
        printf(" ▷ Относительная погрешность: %.8lf\n\n", data->accuracy);
    } else {
        printf("❌ Интеграл ещё не вычислен!\n");
        printf("Выберите пункт 'Расчёт интеграла' ;)\n");
    }
}

void AboutProgram() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                О ПРОГРАММЕ                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    printf("ПРОГРАММА ДЛЯ ВЫЧИСЛЕНИЯ ИНТЕГРАЛОВ\n\n");
    printf("Возможности:\n");
    printf("• Вычисление определённых интегралов\n");
    printf("• Работа с функциями в 1-й и 2-й четвертях\n");
    printf("• Настройка точности вычислений\n");
    printf("• Метод Симпсона\n\n");

    printf("Пример функции: f(x) = x^3- x^2+ x+7;\n");
    printf("Управление: стрелки ↑↓ и Enter\n\n");

    printf("Версия: 1.1\n");
    printf("Разработчик: Куделькин Егор\n");
}

void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

