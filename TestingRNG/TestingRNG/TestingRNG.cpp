// TestingRNG.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>

#include "cephes.c"


using namespace std;


float gammq(float, float);

#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

double edinicikovsemu = 0;

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // Для окрашиванию консоли

void first()
{
	setlocale(LC_ALL, "Russian");

	int nuli = 0, edinici = 0;
	string s; // Сюда будем класть считанные строки

	ifstream file("array.txt"); // Файл, из которого читаем

	while (getline(file, s)) // Пока не достигнут конец файла ложить очередную строку в переменную (s)
	{
		int Number = atoi(s.c_str());
		string bitNumber;

		while (Number)
		{
			char tmp = '0' + Number % 2;
			if (int(tmp) == 48) // 48 - это код нуля в char, 49 - единицы
				nuli++;
			else
				edinici++;
			//cout << "tmp = " << tmp << endl;
			bitNumber = tmp + bitNumber;
			//cout << "bitNumber = " << bitNumber << endl;
			Number /= 2;
		}
	}

	double summa = nuli + edinici;
	double a1 = nuli / summa * 100;
	double a2 = 100 - a1;
	double S = edinici - nuli, Sobs = abs(S) / summa, p = erfc(Sobs / sqrt(2));
	SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "ЧАСТОТНЫЙ ПОБИТОВЫЙ ТЕСТ\n";
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Нулей: " << nuli << "\nЕдиниц: " << edinici
		<< "\nСоотношение: " << a1 << "% / "
		<< a2 << endl;
	cout << "Результат: ";
	if (p > 0.01)
	{
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		cout << "Тест пройден успешно!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p > 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}
	else
	{
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		cout << "Тест НЕ пройден!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p < 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}

	file.close(); // Обязательно закрываем файл, чтобы не повредить его
}

void second()
{
	setlocale(LC_ALL, "Russian");

	double edinici = 0, block = 0;
	double raznicaEdinic = 0, raznicaSchet = 0, schetchik = 0, x2obs = 0, cache = 0, schetchikCache = 0, ediniciCache = 0;
	string s; // Сюда будем класть считанные строки

	ifstream file("array.txt"); // Файл, из которого читаем

	while (getline(file, s)) // Пока не достигнут конец файла ложить очередную строку в переменную (s)
	{
		int Number = atoi(s.c_str()); // Заносим в переменную int один блок (число) из файла
		string bitNumber;

		while (Number)
		{
			char tmp = '0' + Number % 2;
			if (int(tmp) == 49) // 49 - это код единицы в char, 48 - нуля
				edinici++;
			//cout << "tmp = " << tmp << endl;
			bitNumber = tmp + bitNumber;
			//cout << "bitNumber = " << bitNumber << endl;
			Number /= 2;
			schetchik++;
		}
		raznicaEdinic = edinici - ediniciCache;
		ediniciCache = edinici;
		raznicaSchet = schetchik - schetchikCache;
		schetchikCache = schetchik;
		cache += pow((raznicaEdinic / raznicaSchet - 0.5), 2);
		block++;
	}

	x2obs = 4 * 28 * cache; // 28 - средняя длина блока в битах
	double p = gammq((block / 2), (x2obs / 2));
	edinicikovsemu = p;
	SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "ЧАСТОТНЫЙ БЛОЧНЫЙ ТЕСТ\n";
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Средняя частота повторения единиц в блоках равна: " << edinici / schetchik << endl;
	cout << "Результат: ";
	if (p > 0.01)
	{
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		cout << "Тест пройден успешно!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p > 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}
	else
	{
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		cout << "Тест НЕ пройден!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p < 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}

	file.close(); // Обязательно закрываем файл, чтобы не повредить его
}

void third()
{
	setlocale(LC_ALL, "Russian");

	double nuli = 0, edinici = 0, MaxLong = 0, schetchik_edinic = 0, schetchik = 0;
	double cache = 0, tekuwee = 0, tekuwee_cache = 0;
	string bit;
	string s; // Сюда будем класть считанные строки

	ifstream file("array.txt"); // Файл, из которого читаем

	while (getline(file, s)) // Пока не достигнут конец файла ложить очередную строку в переменную (s)
	{
		int Number = atoi(s.c_str());
		string bitNumber;

		while (Number)
		{
			if (Number == 2147483647) // 2 в 31 степени - 1, в двоичном коде даёт все единицы
				break;
			char tmp = '0' + Number % 2;
			if (int(tmp) == 48) // 48 - это код нуля в char, 49 - единицышш
			{
				nuli++;
				edinici = 0;
				tekuwee = 0;
				if (nuli > MaxLong)
				{
					MaxLong = nuli;
					bit = " (нули)";
				}
			}
			else
			{
				edinici++;
				nuli = 0;
				schetchik_edinic++;
				tekuwee = 1;
				if (edinici > MaxLong)
				{
					MaxLong = edinici;
					bit = " (единицы)";
				}
			}
			//cout << "tmp = " << tmp << endl;
			bitNumber = tmp + bitNumber;
			//cout << "bitNumber = " << bitNumber << endl;
			Number /= 2;

			if (schetchik > 1)
			{
				if (tekuwee_cache != tekuwee)
					cache++;
				tekuwee_cache = tekuwee;
			}

			schetchik++;
		}
	}

	double p = 0;
	double cache_1 = (schetchik_edinic / schetchik);
	double cache_2 = 2 / sqrt(schetchik);
	if (cache_1 - 0.5 < cache_2)
	{
		p = erfc((cache++ - 2 * schetchik * cache_1 * (1 - cache_1)) / (2 * sqrt(2 * schetchik) * cache_1 * (1 - cache_1)));
	}
	SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "ТЕСТ НА ПОСЛЕДОВАТЕЛЬНОСТЬ ОДИНАКОВЫХ БИТОВ\n";
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Наибольшая последовательность одинаковых бит: " << MaxLong << bit << endl;
	cout << "Результат: ";
	if (p >= 0.01)
	{
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		cout << "Тест пройден успешно!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p > 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}
	else
	{
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		cout << "Тест НЕ пройден!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p < 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}

	file.close(); // Обязательно закрываем файл, чтобы не повредить его
}

void fourth()
{
	setlocale(LC_ALL, "Russian");

	int edinici = 0;
	double schetchik = 0, MaxLong = 0;
	double i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0, i9 = 0, i10 = 0,
		i11 = 0, i12 = 0, i13 = 0, i14 = 0, i15 = 0, i16 = 0, i17 = 0, i18 = 0, i19 = 0, i20 = 0,
		i21 = 0, i22 = 0, i23 = 0, i24 = 0, i25 = 0, i26 = 0, i27 = 0, i28 = 0, i29 = 0, i30 = 0;

	string s; // Сюда будем класть считанные строки

	ifstream file("array.txt"); // Файл, из которого читаем

	while (getline(file, s)) // Пока не достигнут конец файла ложить очередную строку в переменную (s)
	{
		int Number = atoi(s.c_str());
		string bitNumber;

		while (Number)
		{
			if (Number == 2147483647) // 2 в 31 степени - 1, в двоичном коде даёт все единицы
				break;
			char tmp = '0' + Number % 2;
			if (int(tmp) == 48) // 48 - это код нуля в char, 49 - единицы
				edinici = 0;
			else
				edinici++;
			//cout << "tmp = " << tmp << endl;
			bitNumber = tmp + bitNumber;
			//cout << "bitNumber = " << bitNumber << endl;
			Number /= 2;
		}
		switch (edinici)
		{
		case 1:i1++; break;
		case 2:i2++; break;
		case 3:i3++; break;
		case 4:i4++; break;
		case 5:i5++; break;
		case 6:i6++; break;
		case 7:i7++; break;
		case 8:i8++; break;
		case 9:i9++; break;
		case 10:i10++; break;
		case 11:i11++; break;
		case 12:i12++; break;
		case 13:i13++; break;
		case 14:i14++; break;
		case 15:i15++; break;
		}
		if (edinici > MaxLong)
			MaxLong = edinici;
		edinici = 0;
		schetchik++;
	}

	int VSEGO = i1 + i2 + i3 + i4 + i5 + i6 + i7 + i8 + i9 + i10 + i11 + i12 + i13 + i14 + i15;
	MaxLong = MaxLong / 0.005; // " / 0.005" - костыль, удалить его!!!
	double p = 5, p_index = 0.125, x2obs; // p_index - средняя Теоретическая вероятность
	x2obs = (pow(i1 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i2 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i3 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i4 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i5 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i6 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i7 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i8 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i9 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i10 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i11 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i12 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i13 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i14 - schetchik * p_index, 2)) / (schetchik * p_index) +
		(pow(i15 - schetchik * p_index, 2)) / (schetchik * p_index);
	p = cephes_igamc((MaxLong / 2), (x2obs / 2));

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "ТЕСТ НА САМУЮ ДЛИННУЮ ПОСЛЕДОВАТЕЛЬНОСТЬ ЕДИНИЦ В БЛОКЕ\n";
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Наибольшая последовательность ряда единиц в блоке: " << MaxLong * 0.005 << endl; // " / 0.005" - костыль, удалить его!!!
	cout << "Результат: ";
	if (p >= 0.01)
	{
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		cout << "Тест пройден успешно!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p > 0,01, а именно: "
			<< round(p * 100) / 100 << endl << endl;
	}
	else
	{
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		cout << "Тест НЕ пройден!\n";
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Значение вероятности p < 0,01, а именно: "
			<< round(p * 100) / 100 << endl;
	}

	file.close(); // Обязательно закрываем файл, чтобы не повредить его
}

float gammln(float xx)
{
	float x, tmp, ser;
	const static double cof[6] = { 76.18009172947146, -86.50532032941677,
		24.01409824083091, -1.231739572450155,
		0.1208650973866179e-2, -0.5395239384953e-5 };
	int j;

	x = xx - 1.0;
	tmp = x + 5.5;
	tmp -= (x + 0.5)*log(tmp);
	ser = 1.000000000190015;
	for (j = 0; j <= 5; j++)
	{
		x += 1.0;
		ser += cof[j] / x;
	}
	return -tmp + log(2.5066282746310005*ser);
}

void nrerror(char error_text[])
{
	cerr << "Run-time error...\n";
	cerr << error_text;
	cerr << "...now exiting to system...\n";
	exit(EXIT_FAILURE);
}

void gser(float *gamser, float a, float x, float *gln)
{
	int n;
	float sum, del, ap;

	*gln = gammln(a);
	if (x <= 0.0)
	{
		if (x < 0.0)
			nrerror("x less than 0 in routine gser");
		*gamser = 0.0;
		return;
	}
	else
	{
		ap = a;
		del = sum = 1.0 / a;
		for (n = 1; n <= ITMAX; n++)
		{
			++ap;
			del *= x / ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPS)
			{
				*gamser = sum*exp(-x + a*log(x) - (*gln));
				return;
			}
		}
		nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}

void gcf(float *gammcf, float a, float x, float *gln)
{
	int i;
	float an, b, c, d, del, h;

	*gln = gammln(a);
	b = x + 1.0 - a;
	c = 1.0 / FPMIN;
	d = 1.0 / b;
	h = d;
	for (i = 1; i <= ITMAX; i++)
	{
		an = -i*(i - a);
		b += 2.0;
		d = an*d + b;
		if (fabs(d) < FPMIN)
			d = FPMIN;
		c = b + an / c;
		if (fabs(c) < FPMIN)
			c = FPMIN;
		d = 1.0 / d;
		del = d*c;
		h *= del;
		if (fabs(del - 1.0) < EPS)
			break;
	}
	if (i > ITMAX)
		nrerror("a too large, ITMAX too small in gcf");
	*gammcf = exp(-x + a*log(x) - (*gln))*h;
}

float gammq(float a, float x)
{
	float gamser, gammcf, gln;

	if (x < 0.0 || a <= 0.0)
		cout << ("Invalid arguments in routine gammq");
	if (x < (a + 1.0))
	{
		gser(&gamser, a, x, &gln);
		return 1.0 - gamser;
	}
	else
	{
		gcf(&gammcf, a, x, &gln);
		return gammcf;
	}
}

int main()
{
	first();
	second();
	third();
	fourth();

	cin.get();
}