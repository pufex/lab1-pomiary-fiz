#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <time.h>

// Opracowanie wynik�w pomiar�w laboratorium 1 - zad 1.06

// Program na podstawie 'i' pomiar�w ci�nienia w butli wzgl�dem otoczenia
// (na manometrze jako ro�nica wysoko�ci s�upk�w cieczy) liczy nast�puj�ce
// wymagane do tabelki wielko�ci dla wszystkich lub kazdego pomiaru z osobna:
// 1. Warto�� wsp�czynnika k = Cp/Cv
// 2. �redni� warto�� wsp�lczynika k.
// 3. Dla ka�dego pomiaru niepewno�� standardow� typu B.
// 4. Niepewno�� standardow� typu A dla �redniego K.
// 5. Niepewno�� standardow� ca�kowit�.

int ile_pomiarow();
double* inicjacja_pomiarow(int);
double* losowe_pomiary(int);
double wylosuj_pomiar(double, double);
void wyswietl_wyniki(double*, int);

int main() {
	srand(time(0));
	setlocale(LC_CTYPE, "Polish");
	int n;
	double* pomiary;
	n = ile_pomiarow();
	pomiary = losowe_pomiary(n);
	if (pomiary == 0) {
		printf("B��d przy alokowaniu pami�ci dla pomiar�w. Program zako�czy dzia�anie. \n");
		exit(0);
	}
	wyswietl_wyniki(pomiary, n);
	free(pomiary);
}

void wyswietl_wyniki(double* pomiary, int ilosc_pomiarow) {
	int i;
	double h1, h2, h1_avg = 0, h2_avg = 0, k, k_avg = 0, UB, UB_max = 0, UA, S = 0, UC;
	double a, b, c;
	printf("Warto�ci wsp�czynnika K dla poszczeg�lnych pomiar�w: \n");
	for (i = 0; i < 2*ilosc_pomiarow; i+=2) {
		h1 = *(pomiary + i);
		h1_avg += h1;
		h2 = *(pomiary + i + 1);
		h2_avg += h2;
		k = h1 / (h1 - h2);
		k_avg += k;
		printf("K%d = %.4lf\n", i / 2 + 1, k);
	}
	h1_avg /= ilosc_pomiarow;
	h2_avg /= ilosc_pomiarow;
	k_avg /= ilosc_pomiarow;

	printf("�rednia warto�� wsp�czynnika K: %lf\n", k_avg);

	printf("Niepewno�� standardowa typu B dla ka�dego pomiaru: \n");
	for (i = 0; i < 2*ilosc_pomiarow; i += 2) {
		h1 = *(pomiary + i);
		h2 = *(pomiary + i + 1);
		k = h1 / (h1 - h2);
		S += (k_avg - k) * (k_avg - k);
		a = 1 / (h1 - h2) * (h1 - h2);
		b = sqrt((h2 * h2 * (h1_avg - h1) * (h1_avg - h1) + h1 * h1 * (h2_avg - h2) * (h2_avg - h2))/3);
		UB = a * b;
		if (UB > UB_max) {
			UB_max = UB;
		}
		printf("UB(%d) = %lf\n", i / 2 + 1, UB);
	}

	UA = sqrt(S / (ilosc_pomiarow * (ilosc_pomiarow - 1)));
	printf("Niepewno�� standardowa typu A dla warto�ci �redniej k: %lf\n", UA);
	UC = sqrt(UA * UA + UB_max * UB_max);
	printf("Niepewno�� standardowa ca�kowita: %lf\n", UC);
	printf("Wynik ko�cowyL K = %.3lf(%.3lf)\n", k_avg, UC);
}

int ile_pomiarow() {
	int n;
	while (1) {
		printf("Prosz� wprowadzi� ilo�� pomiar�w: ");
		scanf_s("%d", &n);
		system("CLS");

		if (n < 0) {
			printf("Wprowadzona liczba pomiar�w jest nieprawid�owa. \n");
			continue;
		}

		break;
	}
	return n;
}

double* inicjacja_pomiarow(int n) {
	double* pomiary = (long double*)malloc(sizeof(double) * n * 2);
	double wynik = 0;
	if (pomiary == 0) {
		return 0;
	}
	for (int i = 0; i < 2 * n; i++) {
		if (i % 2 == 0) {
			printf("Prosz� wprowadzi� warto�� h1 dla %d-tego pomiaru: ", i/2 + 1);
		}
		else {
			printf("Prosze wprowadzi� warto�� h2 dla %d-tego pomiaru: ", i / 2 + 1);
		}
		scanf_s("%lf", &wynik);
		system("CLS");
		*(pomiary + i) = wynik;
	}
	return pomiary;
}

double* losowe_pomiary(int n) {
	double* pomiary = (long double*)malloc(sizeof(double) * n * 2);
	double h1, h2;
	if (pomiary == 0) {
		return 0;
	}
	for (int i = 0; i < 2 * n; i+=2) {
		h1 = wylosuj_pomiar(29.5, 30.5);
		h2 = wylosuj_pomiar(0.39 / 1.39 * h1, 0.41 / 1.41 * h1);
		*(pomiary + i) = h1;
		*(pomiary + i + 1) = h2;
	}
	return pomiary;
}

double wylosuj_pomiar(double min, double max) {
	double diff = fabs(max - min);
	double rand1 = rand(), rand2 = rand();
	if (rand1 > rand2) {
		return (rand2 / rand1) * diff + min;
	}
	else {
		return (rand1 / rand2) * diff + min;
	}
}
