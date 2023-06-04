#include "classes.h"

extern HDC hdc;//�������� ����������(�������� �������� ���������� � ���� ����������� ����������� ���������)

//������ ������� �����
void Point::set_visible()
{
	Visible = true;
	SetPixel(hdc, X, Y, RGB(255, 0, 0));		//������ ����� ������������� ������
	SetPixel(hdc, X + 1, Y, RGB(255, 0, 0));	//������ ����� ������������� ������
	SetPixel(hdc, X, Y + 1, RGB(255, 0, 0));	//������ ����� ������������� ������
	SetPixel(hdc, X + 1, Y + 1, RGB(255, 0, 0));//������ ����� ������������� ������
	current_region(X, Y);						//������� ������� ������
};

//������ ��������� �����
void Point::set_invisible()
{
	Visible = false;
	SetPixel(hdc, X, Y, RGB(0, 0, 0));			//������ ����� ������������� ������
	SetPixel(hdc, X + 1, Y, RGB(0, 0, 0));		//������ ����� ������������� ������
	SetPixel(hdc, X, Y + 1, RGB(0, 0, 0));		//������ ����� ������������� ������
	SetPixel(hdc, X + 1, Y + 1, RGB(0, 0, 0));	//������ ����� ������������� ������
};

//������� ������� ������
void Point::current_region(int X, int Y)
{
	boxheat.start_X = X;
	boxheat.end_X = X + 1;
	boxheat.start_Y = Y;
	boxheat.end_Y = Y + 1;
};

//���������� �����
void Point::Move_To(int X, int Y)
{
	set_invisible();
	Set_X(X);
	Set_Y(Y);
	set_visible();
}

//�������������� �����
void Point::Drag()
{
	//while 7 - �����
	while (!KEY_DOWN(55))
	{
		// A - �����
		if (KEY_DOWN(65))
		{
			Move_To(Get_X() - 20, Get_Y());
		}
		// W - �����
		else if (KEY_DOWN(87))
		{
			Move_To(Get_X(), Get_Y() - 20);
		}
		// D - ������
		else if (KEY_DOWN(68))
		{
			Move_To(Get_X() + 20, Get_Y());
		}
		// S - ����
		else if (KEY_DOWN(83))
		{
			Move_To(Get_X(), Get_Y() + 20);
		}
		Sleep(10);
	}
}

//��������� ����
void ball::Paint_ball()
{
	Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);
}

//���������� �������������� ������
void ball::current_region(int X, int Y)
{
	boxheat.start_X = X - radius;
	boxheat.end_X = X + radius;
	boxheat.start_Y = Y + radius;
	boxheat.end_Y = Y - radius;
}

void ball::set_visible()
{
	Visible = true;
	HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenBlack);
	Paint_ball();
	current_region(X, Y);						//������� ������� ������

	DeleteObject(PenBlack);
}

void ball::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenBlack);
	Paint_ball();

	DeleteObject(PenBlack);
}

//��������� �����
void Tower::Paint()
{
	//��������� �����
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

	//����� �����
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

void Tower::current_region(int X, int Y)
{
	boxheat.start_X = X - 30;
	boxheat.end_X = X + 95;
	boxheat.start_Y = Y + 125;
	boxheat.end_Y = Y - 60;
};

//������ ������� �����
void Tower::set_visible()
{
	Visible = true;
	HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenBlack);
	Paint();
	current_region(X, Y);

	DeleteObject(PenBlack);
};

//������ ��������� �����
void Tower::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenBlack = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenBlack);
	Paint();

	DeleteObject(PenBlack);
};

void vertical_Tower_1::current_region(int X, int Y)
{
	boxheat.start_X = X - 30;
	boxheat.end_X = X + 95;
	boxheat.start_Y = Y + 125;
	boxheat.end_Y = Y - 60;
};

//������ ������� �����
void vertical_Tower_1::set_visible()
{
	Visible = true;
	HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenGreen);
	Paint();
	current_region(X, Y);

	DeleteObject(PenGreen);
};

//������ ��������� �����
void vertical_Tower_1::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenWhite);
	Paint();
	DeleteObject(PenWhite);
};

//��������� ����� � ������������ ��������
void vertical_Tower_2::print_construction()
{
	//���������� �����
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 63);
	LineTo(hdc, X + 150, Y + 63);
	LineTo(hdc, X + 150, Y + 125);
	LineTo(hdc, X + 60, Y + 125);

	//����� ����������
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 35);
	LineTo(hdc, X + 130, Y + 35);
	LineTo(hdc, X + 150, Y + 63);

	//���� ����������
	MoveToEx(hdc, X + 100, Y + 100, NULL);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 125, Y + 80);
	LineTo(hdc, X + 125, Y + 100);
	LineTo(hdc, X + 100, Y + 100);
	LineTo(hdc, X + 112, Y + 100);
	LineTo(hdc, X + 112, Y + 80);
}

void vertical_Tower_2::current_region(int X, int Y)
{
	boxheat.start_X = X;
	boxheat.end_X = X + 150;
	boxheat.start_Y = Y + 125;
	boxheat.end_Y = Y;
};

//������ ������� ����� � ������������ �������� 2
void vertical_Tower_2::set_visible()
{
	Visible = true;
	HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenGreen);
	Paint();
	print_construction();
	current_region(X, Y);

	DeleteObject(PenGreen);
};

//������ ��������� ����� � ������������ �������� 2
void vertical_Tower_2::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenWhite);
	Paint();
	print_construction();

	DeleteObject(PenWhite);
};

//����� ���������� � ������� ��������
void left_construction_Tower::left_print()
{
	//���������� �����
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X, Y);
	LineTo(hdc, X, Y + 63);
	LineTo(hdc, X - 90, Y + 63);
	LineTo(hdc, X - 90, Y + 125);
	LineTo(hdc, X, Y + 125);

	//����� ����������
	LineTo(hdc, X, Y);
	LineTo(hdc, X, Y + 35);
	LineTo(hdc, X - 70, Y + 35);
	LineTo(hdc, X - 90, Y + 63);

	//���� ����������
	MoveToEx(hdc, X - 40, Y + 100, NULL);
	LineTo(hdc, X - 40, Y + 80);
	LineTo(hdc, X - 40, Y + 80);
	LineTo(hdc, X - 65, Y + 80);
	LineTo(hdc, X - 65, Y + 100);
	LineTo(hdc, X - 40, Y + 100);
	LineTo(hdc, X - 53, Y + 100);
	LineTo(hdc, X - 53, Y + 80);
}

void left_construction_Tower::current_region(int X, int Y)
{
	boxheat.start_X = X - 90;
	boxheat.end_X = X;
	boxheat.start_Y = Y + 125;
	boxheat.end_Y = Y;
};

//������ ������� ����� ����� � ������� ��������
void left_construction_Tower::set_visible()
{
	Visible = true;
	HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenGreen);
	Paint();
	left_print();
	current_region(X, Y);

	DeleteObject(PenGreen);
};

//������ ��������� ����� ����� � ������� ��������
void left_construction_Tower::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenWhite);
	Paint();
	left_print();

	DeleteObject(PenWhite);
};

//������ ����������
void right_construction_Tower::right_print()
{
	//���������� �����
	MoveToEx(hdc, X, Y, NULL);
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 63);
	LineTo(hdc, X + 150, Y + 63);
	LineTo(hdc, X + 150, Y + 125);
	LineTo(hdc, X + 60, Y + 125);

	//����� ����������
	LineTo(hdc, X + 60, Y);
	LineTo(hdc, X + 60, Y + 35);
	LineTo(hdc, X + 130, Y + 35);
	LineTo(hdc, X + 150, Y + 63);

	//���� ����������
	MoveToEx(hdc, X + 100, Y + 100, NULL);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 100, Y + 80);
	LineTo(hdc, X + 125, Y + 80);
	LineTo(hdc, X + 125, Y + 100);
	LineTo(hdc, X + 100, Y + 100);
	LineTo(hdc, X + 112, Y + 100);
	LineTo(hdc, X + 112, Y + 80);
}

void right_construction_Tower::current_region(int X, int Y)
{
	boxheat.start_X = X;
	boxheat.end_X = X + 150;
	boxheat.start_Y = Y + 125;
	boxheat.end_Y = Y;
};

//������ ������� ������ ����� � ������� ��������
void right_construction_Tower::set_visible()
{
	Visible = true;
	HPEN PenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	//������ ���� �������� 
	SelectObject(hdc, PenGreen);
	Paint();
	right_print();
	current_region(X, Y);

	DeleteObject(PenGreen);
};

//������ ��������� ������ ����� � ������� ��������
void right_construction_Tower::set_invisible()
{
	Visible = false;
	//����� ���� (������)
	HPEN PenWhite = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	//������ ���� �������� 
	SelectObject(hdc, PenWhite);
	Paint();
	right_print();

	DeleteObject(PenWhite);
};
