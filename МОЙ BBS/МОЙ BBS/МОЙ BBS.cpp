// МОЙ BBS.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <conio.h>
#include <windows.h>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

long long x0, y, M, p = 0, q = 0, n = 0, x = 0, cache, cache_zapis, qwe;

int modexp(long long x, int y, int M)
{
	if (y == 0) return 1;
	long long z = modexp(x, y / 2, M);
	if (y % 2 == 0)
		return (z*z) % M;
	else
		return (x*z*z) % M;
}

typedef unsigned long long ll;

ll pows[70];

void fil()
{
	pows[0] = 1ll;
	for (int i = 1; i <= 63; ++i)
		pows[i] = pows[i - 1] * 2ll;
}

ll mul_mod(ll a, ll b, ll mod)
{
	ll res = 0;
	while (b)
	{
		if (b & 1)
			res = (res + a) % mod;
		a = (a * 2ll) % mod;
		b >>= 1;
	}
	return res;
}

ll powm(ll p, ll n, ll mod)
{
	ll res = 1;
	while (n)
	{
		if (n & 1)
			res = mul_mod(res, p, mod);
		p = mul_mod(p, p, mod);
		n >>= 1;
	}
	return res;
}

bool mrt(ll p)
{
	if (p == 1)
		return false;
	ll s = 0, d = p - 1;
	while (!(d & 1))
	{
		d = d / 2;
		s++;
	}
	srand(time(NULL)); /// Генерируем действительно случайные и разные a
	for (int i = 1; i <= 5; ++i)
	{ /// 5 проверок, обеспечивающие вероятность ошибки всего 1/1024
		ll a = rand() % p;
		if (a == 0)
			a++;
		bool flag1 = false, flag2 = false;
		if (powm(a % p, d, p) == 1) /// Первая проверка
			flag1 = true;
		if (!flag1)
			for (int r = 0; r < s; ++r) /// Вторая проверка
				if (powm(a % p, pows[r] * d, p) == p - 1)
					flag2 = true;
		if (!flag1 && !flag2)
			return false;
	}
	return true;
}

int main()
{
	clock_t start, stop;

	setlocale(LC_ALL, "Russian");

	std::random_device rd; // Запускаем генератор больших чисел
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis;

	//int test10000 = 0;

	do
	{
		start = clock(); // запуск таймера выполнения программы
		cout << "Началась генерация случайного числа, пожалуйста, немного подождите...\n";
		do
		{
			do
			{
				do
				{
					qwe = dis(gen); // В "qwe" заносим сгенерированное число, которое может оказаться простым
				} while (mrt(qwe) == false); // Если сгенерированное число не простое, то повторяем генерацию
			} while (modexp(qwe, 1, 4) != 3); // Если остаток не 3 по модулю 4, то повторяем генерацию

			if (q != 0)
				p = qwe; // В "p" заносим первое простое число
			if (x != 0 && p == 0)
				q = qwe; // В "q" заносим второе простое число
			if (p == 0 && q == 0)
				x = qwe; // В "s" заносим третье простое число
			n = dis(gen);
		} while (p == 0);
		M = p * q;
		x0 = modexp(x, 2, M);
		cache = modexp(2, n, ((p - 1) * (q - 1)));
		cache_zapis = modexp(x0, cache, M);
		cout << "Сгенерированное число: " << cache_zapis << '\n';

		p = 0, q = 0, x = 0; // Зануляем переменные, чтобы заполнить их новыми значениями на следующем проходе

		ofstream zapis("array.txt", ios_base::app); // Создаём объект класса ofstream для записи и связываем его с файлом array.txt
		zapis << cache_zapis << "\n"; // Запись строки в конец файла, потому что ios_base::app
		zapis.close(); // Закрываем файл

		stop = clock(); // Остановка таймера выполнения программы
		cout << "Программа была выполнена за: " << (stop - start) / 1000.0 << " секунд!\n"; // Время выполнения программы

		//test10000++;
	} while (cin.get());
	//} while (test10000 < 100);
}