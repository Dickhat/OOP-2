﻿#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

using namespace std;

HDC hdc;//Контекст устройства(содержит описание видеокарты и всех необходимых графических элементов)

//макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HWND GetConcolWindow()
{
	char str[128];
	// char title[128]="xxxxxxxxxxxxxxxxxx";
	LPWSTR title = (LPWSTR)"xxxxxxxxxxxxxxxxxx";	//новая версия Windows
	GetConsoleTitle((LPWSTR)str, sizeof((LPWSTR)str)); // получить заголовок окна
	SetConsoleTitle(title);						// установить новый заголовок окна
	Sleep(100);									// ждем смены заголовка окна (100 мс);

	HWND hwnd = FindWindow(NULL, (LPWSTR)title);// определяем дескриптор окна
	SetConsoleTitle((LPWSTR)str);				//возвращаем прежний заголовок

	return hwnd;//вернуть дескриптор окна
}//end GetConcolWindow()

//функция паузы до нажатия кнопки
void PressKey(int VkCode)
{
	while (1)
		if (KEY_DOWN(VkCode))
			break;
}

//Класс местоположение
class Location
{
protected:
	int X;//Координата X
	int Y;//Координата Y

public:
	//Конструктор
	Location(int ind_X, int ind_Y)
	{
		X = ind_X;
		Y = ind_Y;
	}

	//Возвращает X
	int Get_X()
	{
		return X;
	};

	//Возвращает Y
	int Get_Y()
	{
		return Y;
	}

	//Устанавливает X
	void Set_X(int ind_X)
	{
		X = ind_X;
	};

	//Устанавливает Y
	void Set_Y(int ind_Y)
	{
		Y = ind_Y;
	};
};

//Класс точка
class Point :public Location
{
protected:
	bool Visible;//Видимость точки

	struct heat_box
	{
		int start_X, end_X, start_Y, end_Y;	//Координаты фигуры(В форме квадрата)
	} boxheat;

public:
	//КОНСТРУКТОР
	Point(int X, int Y) :Location(X, Y)
	{ 
		Visible = false;
		boxheat.start_X = X;
		boxheat.end_X = X + 1;
		boxheat.start_Y = Y;
		boxheat.end_Y = Y + 1;
	};

	//Возвращает хитбоксы
	const heat_box & get()
	{
		return boxheat;
	}

	//Делает видимой точку
	virtual void set_visible()
	{
		Visible = true;
		SetPixel(hdc, X, Y, RGB(255, 0, 0));		//рисуем точку установленным цветом
		SetPixel(hdc, X + 1, Y, RGB(255, 0, 0));	//рисуем точку установленным цветом
		SetPixel(hdc, X, Y + 1, RGB(255, 0, 0));	//рисуем точку установленным цветом
		SetPixel(hdc, X + 1, Y + 1, RGB(255, 0, 0));//рисуем точку установленным цветом
		current_region(X, Y);						//Текущая область фигуры
	};

	//Делает невидимой точку
	virtual void set_invisible()
	{
		Visible = false;
		SetPixel(hdc, X, Y, RGB(0, 0, 0));			//рисуем точку установленным цветом
		SetPixel(hdc, X + 1, Y, RGB(0, 0, 0));		//рисуем точку установленным цветом
		SetPixel(hdc, X, Y + 1, RGB(0, 0, 0));		//рисуем точку установленным цветом
		SetPixel(hdc, X + 1, Y + 1, RGB(0, 0, 0));	//рисуем точку установленным цветом
	};

	//Текущая область фигуры
	virtual void current_region(int X, int Y)
	{
		boxheat.start_X = X;
		boxheat.end_X = X + 1;
		boxheat.start_Y = Y;
		boxheat.end_Y = Y + 1;
	};

	//Перемещает точку
	void Move_To(int X, int Y)
	{
		set_invisible();
		Set_X(X);
		Set_Y(Y);
		set_visible();
	}

	//Перетаскивание точки
	void Drag()
	{
		//while 7 - выход
		while (!KEY_DOWN(55))
		{
			// A - влево
			if (KEY_DOWN(65))
			{
				Move_To(Get_X() - 20, Get_Y());
			}
			// W - вверх
			else if (KEY_DOWN(87)) 
			{
				Move_To(Get_X(), Get_Y() - 20);
			}
			// D - вправо
			else if (KEY_DOWN(68))
			{
				Move_To(Get_X() + 20, Get_Y());
			}
			// S - Вниз
			else if (KEY_DOWN(83))
			{
				Move_To(Get_X(), Get_Y() + 20);
			}
			Sleep(10);
		}
	}
};

//Класс ядро
class ball :public Point
{
protected:
	int radius;
public:
	//Конструктор
	ball(int X, int Y, int Rad) :Point(X, Y) { radius = Rad; };

	void Paint_ball();

	void set_rad(int Rad) { radius = Rad; };
	int get_rad() { return radius; };

	void current_region(int X, int Y)
	{
		boxheat.start_X = X - radius;
		boxheat.end_X = X + radius;
		boxheat.start_Y = Y + radius;
		boxheat.end_Y = Y - radius;
	}

	void set_visible()
	{
		Visible = true;
		HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		//делаем перо активным 
		SelectObject(hdc, PenBlack);
		Paint_ball();
		current_region(X, Y);						//Текущая область фигуры

		DeleteObject(PenBlack);
	}
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenBlack);
		Paint_ball();

		DeleteObject(PenBlack);
	}
};

//Рисование ядра
void ball::Paint_ball()
{
	Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);
}

//Класс фигура (Башня)
class Tower :public Point
{
public:
	Tower(int X, int Y) :Point(X, Y) {
		
	};
	void virtual Paint();

	void current_region(int X, int Y)
	{
		boxheat.start_X = X - 30;
		boxheat.end_X = X + 95;
		boxheat.start_Y = Y + 125;
		boxheat.end_Y = Y - 60;
	};

	//Делает видимой точку
	void set_visible()
	{
		Visible = true;
		HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		//делаем перо активным 
		SelectObject(hdc, PenBlack);
		Paint();
		current_region(X, Y);

		DeleteObject(PenBlack);
	};

	//Делает невидимой точку
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenBlack);
		Paint();

		DeleteObject(PenBlack);
	};

	////Перемещает точку (static)
	//void Move_To(int X, int Y)
	//{
	//	set_invisible();
	//	Set_X(X);
	//	Set_Y(Y);
	//	set_visible();
	//}

	////Перетаскивание точки (static)
	//void Drag()
	//{
	//	//while 7 - выход
	//	while (!KEY_DOWN(55))
	//	{
	//		// A - влево
	//		if (KEY_DOWN(65)) Move_To(Get_X() - 20, Get_Y());
	//		// W - вверх
	//		else if (KEY_DOWN(87)) Move_To(Get_X(), Get_Y() - 20);
	//		// D - вправо
	//		else if (KEY_DOWN(68)) Move_To(Get_X() + 20, Get_Y());
	//		// S - Вниз
	//		else if (KEY_DOWN(83)) Move_To(Get_X(), Get_Y() + 20);
	//		Sleep(10);
	//	}
	//}
};

//Рисование Башни
void Tower::Paint()
{
	//Основание башни
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 125);
	LineTo(hdc, X + 40, Y + 125);
	LineTo(hdc, X + 40, Y + 80);
	LineTo(hdc, X + 20, Y + 80);
	LineTo(hdc, X + 20, Y + 125);
	LineTo(hdc, X + 40, Y + 125);
	LineTo(hdc, X, Y + 125);
	LineTo(hdc, X, Y);
	
	//Крыша башни
	LineTo(hdc, X - 30, Y);
	LineTo(hdc, X - 30, Y - 60);
	LineTo(hdc, X - 5, Y - 60);
	LineTo(hdc, X - 5, Y - 30);
	LineTo(hdc, X + 25, Y - 30);
	LineTo(hdc, X + 25, Y - 60);
	LineTo(hdc, X + 50, Y - 60);
	LineTo(hdc, X + 50, Y - 30);
	LineTo(hdc, X + 75, Y - 30);
	LineTo(hdc, X + 75, Y - 30);
	LineTo(hdc, X + 75, Y - 60);
	LineTo(hdc, X + 95, Y - 60);
	LineTo(hdc, X + 95, Y);
	LineTo(hdc, X, Y);
}

//Вертикальная иерархия 1
class vertical_Tower_1 :public Tower
{
public:
	//Конструктор
	vertical_Tower_1(int X, int Y) :Tower(X, Y) {};

	void current_region(int X, int Y)
	{
		boxheat.start_X = X - 30;
		boxheat.end_X = X + 95;
		boxheat.start_Y = Y + 125;
		boxheat.end_Y = Y - 60;
	};

	//Делает видимой башню
	void set_visible()
	{
		Visible = true;
		HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//делаем перо активным 
		SelectObject(hdc, PenGreen);
		Paint();
		current_region(X, Y);

		DeleteObject(PenGreen);
	};

	//Делает невидимой башню
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenWhite);
		Paint();
		DeleteObject(PenWhite);
	};
};

//Вертикальная иерархия 2
class vertical_Tower_2 :public vertical_Tower_1
{
public:
	//Конструктор
	vertical_Tower_2(int X, int Y) :vertical_Tower_1(X, Y) {};

	void print_construction();
	
	void current_region(int X, int Y)
	{
		boxheat.start_X = X;
		boxheat.end_X = X + 150;
		boxheat.start_Y = Y + 125;
		boxheat.end_Y = Y;
	};

	//Делает видимой башню
	void set_visible()
	{
		Visible = true;
		HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//делаем перо активным 
		SelectObject(hdc, PenGreen);
		Paint();
		print_construction();
		current_region(X, Y);

		DeleteObject(PenGreen);
	};

	//Делает невидимой башню
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenWhite);
		Paint();
		print_construction();

		DeleteObject(PenWhite);
	};
};

void vertical_Tower_2::print_construction()
{
	//Пристройка башни
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 63);
	LineTo(hdc, X + 150, Y + 63);
	LineTo(hdc, X + 150, Y + 125);
	LineTo(hdc, X + 60, Y + 125 );

	//Крыша пристройки
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 35);
	LineTo(hdc, X + 130, Y + 35);
	LineTo(hdc, X + 150, Y + 63);

	//Окно пристройки
	MoveToEx(hdc,X + 100, Y + 100, NULL);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 125, Y + 80);
	LineTo(hdc, X + 125, Y + 100);
	LineTo(hdc, X + 100, Y + 100);
	LineTo(hdc, X + 112, Y + 100);
	LineTo(hdc, X + 112, Y + 80);
}

//Веерная иерархия 1
class left_construction_Tower : public Tower
{
public:
	//Конструктор
	left_construction_Tower(int X, int Y) :Tower(X, Y) {};

	//Левая конструкция
	void left_print();

	void current_region(int X, int Y)
	{
		boxheat.start_X = X - 90;
		boxheat.end_X = X;
		boxheat.start_Y = Y + 125;
		boxheat.end_Y = Y;
	};


	//Делает видимой башню
	void set_visible()
	{
		Visible = true;
		HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//делаем перо активным 
		SelectObject(hdc, PenGreen);
		Paint();
		left_print();
		current_region(X, Y);

		DeleteObject(PenGreen);
	};

	//Делает невидимой башню
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenWhite);
		Paint();
		left_print();

		DeleteObject(PenWhite);
	};
};

//Левая пристройка
void left_construction_Tower::left_print()
{
	//Пристройка башни
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X , Y);
	LineTo(hdc, X , Y + 63);
	LineTo(hdc, X - 90, Y + 63);
	LineTo(hdc, X - 90, Y + 125);
	LineTo(hdc, X , Y + 125);

	//Крыша пристройки
	LineTo(hdc, X , Y);
	LineTo(hdc, X , Y + 35);
	LineTo(hdc, X - 70, Y + 35);
	LineTo(hdc, X - 90, Y + 63);
	
	//Окно пристройки
	MoveToEx(hdc, X - 40, Y + 100, NULL);
	LineTo(hdc, X - 40, Y + 80);
	LineTo(hdc, X - 40, Y + 80);
	LineTo(hdc, X - 65, Y + 80);
	LineTo(hdc, X - 65, Y + 100);
	LineTo(hdc, X - 40, Y + 100);
	LineTo(hdc, X - 53, Y + 100);
	LineTo(hdc, X - 53, Y + 80);
}

//Веерная иерархия 2
class right_construction_Tower : public Tower
{
public:
	//Конструктор
	right_construction_Tower(int X, int Y) :Tower(X, Y) {};

	//Правая конструкция
	void right_print();

	void current_region(int X, int Y)
	{
		boxheat.start_X = X;
		boxheat.end_X = X + 150;
		boxheat.start_Y = Y + 125;
		boxheat.end_Y = Y;
	};


	//Делает видимой башню
	void set_visible()
	{
		Visible = true;
		HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//делаем перо активным 
		SelectObject(hdc, PenGreen);
		Paint();
		right_print();
		current_region(X, Y);

		DeleteObject(PenGreen);
	};

	//Делает невидимой башню
	void set_invisible()
	{
		Visible = false;
		//Белый цвет (сейчас)
		HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		//делаем перо активным 
		SelectObject(hdc, PenWhite);
		Paint();
		right_print();

		DeleteObject(PenWhite);
	};
};

//Правая пристройка
void right_construction_Tower::right_print()
{
	//Пристройка башни
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 63);
	LineTo(hdc, X + 150, Y + 63);
	LineTo(hdc, X + 150, Y + 125);
	LineTo(hdc, X + 60, Y + 125);

	//Крыша пристройки
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 35);
	LineTo(hdc, X + 130, Y + 35);
	LineTo(hdc, X + 150, Y + 63);

	//Окно пристройки
	MoveToEx(hdc, X + 100, Y + 100, NULL);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 125, Y + 80);
	LineTo(hdc, X + 125, Y + 100);
	LineTo(hdc, X + 100, Y + 100);
	LineTo(hdc, X + 112, Y + 100);
	LineTo(hdc, X + 112, Y + 80);
}

//ПРоверка столкновений
void check(int current, vector <Tower*>& Tow, vector <ball*>& BALL);

int main()
{
	//получим дескриптор консольного окна
	HWND hwnd = GetConcolWindow();
	hdc = GetWindowDC(hwnd);    // контекст устройства консольного окна

	// Зададим перо и цвет пера
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, Pen);

	Tower* Pcur;				//Указатель на текущий элемент
	Point APoint(200, 200);

	vertical_Tower_1 green_tw_1(400, 200);
	vertical_Tower_2 green_tw_2(600, 200);
	green_tw_1.set_visible();
	green_tw_2.set_visible();

	left_construction_Tower cons_tw_1(200, 400);
	right_construction_Tower cons_tw_2(400, 400);
	cons_tw_1.set_visible();
	cons_tw_2.set_visible();

	ball bill_1(APoint.Get_X(), APoint.Get_Y(), 10);
	ball bill_2(800, 400, 40);
	ball bill_3(1000, 400, 70);
	bill_1.set_visible();
	bill_2.set_visible();
	bill_3.set_visible();

	//Массив указателей для определения коллизий
	vector <Tower*> Tow(4);
	Tow[0] = &green_tw_1;
	Tow[1] = &green_tw_2;
	Tow[2] = &cons_tw_1;
	Tow[3] = &cons_tw_2;

	vector <ball*> BALL(3);
	BALL[0] = &bill_1;
	BALL[1] = &bill_2;
	BALL[2] = &bill_3;

	PressKey(49);			//1
	Pcur = &green_tw_1;
	green_tw_1.Drag();
	check(0, Tow, BALL);
	Sleep(200);

	//APoint.Move_To(APoint.Get_X() + 100, APoint.Get_Y());//Перемещение точки

	PressKey(50);			//2
	Pcur = &green_tw_2;
	green_tw_2.Drag();
	check(1, Tow, BALL);
	//APoint.set_invisible();	//Исчезновение точки

	PressKey(51);			//3
	Pcur = &cons_tw_1;
	cons_tw_1.Drag();
	check(2, Tow, BALL);
	//vertical_Tower_2 bashna(APoint.Get_X(), APoint.Get_Y());//Создание объекта башни в координатах 200,200
	//bashna.Paint();			//Рисование башни
	//bashna.set_visible();   //Появление башни

	PressKey(52);			//4
	Pcur = &cons_tw_2;
	cons_tw_2.Drag();
	check(3, Tow, BALL);
	//bashna.Move_To(bashna.Get_X() + 100, bashna.Get_Y());//Перемещение башни

	//PressKey(53);			//5
	//bill.Drag();

	//bashna.set_invisible();	//Исчезновение башни

	//PressKey(54);			//6
	
	//bashna.Drag();			//Перетаскивание башни

	DeleteObject(Pen);

	return 0;
}

//ПРоверка столкновений
void check(int current,vector <Tower*>& Tow, vector <ball*>& BALL)
{
	for (int i = 0; i < BALL.size(); ++i)
	{
		//if Проверка, что объекты столкнулись
		if (Tow[current]->get().end_X < BALL[i]->get().start_X
			|| Tow[current]->get().start_X > BALL[i]->get().end_X
			|| Tow[current]->get().end_Y > BALL[i]->get().start_Y
			|| Tow[current]->get().start_Y < BALL[i]->get().end_Y)
			cout << "collision undetect\n";
		else
		{
			Tow[current]->set_invisible();	//Разрушить объект
			Tow[current]->Move_To(Tow[current]->Get_X() + 10000, Tow[current]->Get_Y() + 10000);
			BALL[i]->set_invisible();		//Разрушить объект
			BALL[i]->Move_To(BALL[i]->Get_X() + 15000, BALL[i]->Get_Y() + 15000);
			cout << "collision detect\n";
			break;
		}
	}
};