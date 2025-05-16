//metoda Rungego-Kutty RK4 system 3 rzędu 
#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>

// deklaracje zasięgów
using namespace std;

// stałe w programie
#define h 0.001 // krok obliczeń
#define T 50.0 // całkowity czas symulacji – przedział [0 , T]
#define L 2.5 // liczba okresów sygnału sinus w przedziale T
#define M 8.0 // amplituda sygnału sinus
#define PI 3.14159265 // liczba PI

int total;
double a3,a2,a1,a0,b1,b0;
double T1, T2, k1, k2;
double w;

// zmienne globalne w programie
double u[(int)(1.0*T/h)+1]; // sygnał wejściowy
double u1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wejściowego
double y[3] = {0.0, 0.0, 0.0};
//double y1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wyjściowego
//double y2p[(int)(1.0*T/h)+1]; // druga pochodna sygnału wyjściowego
//double y3p[(int)(1.0*T/h)+1]; // trzecia pochodna sygnału wyjściowego
double y_out[(int)(1.0*T/h)+1]; // sygnał wyjściowy

void compute_derivatives(double y[], double dydt[], double u, double u1p)
{
    dydt[0] = y[1];  // dy0/dt = y1
    dydt[1] = y[2];  // dy1/dt = y2
    dydt[2] = (1.0 / a3) * (-a2 * y[2] - a1 * y[1] - a0 * y[0] + b1 * u1p + b0 * u);  // dy2/dt
}

// program główny
int main()
{

    // wczytanie parametrów modelu
    cout << "\n T1 = "; cin >> T1;
    cout << "\n T2 = "; cin >> T2;
    cout << "\n k1 = "; cin >> k1;
    cout << "\n k2 = "; cin >> k2;
    cout << "\n\n";

    // współczynniki układu równania różniczkowego poczatkowego
    b1 = 1.0*T1*k1*k2;
    b0 = 1.0*k1*k2;
    a3 = 1.0*T1*T2;
    a2 = 1.0*T1;
    a1 = 1.0*T1*k1*k2;
    a0 = 1.0*k1*k2;

    total=sizeof(u)/sizeof(u[0]); // rozmiar wektorów danych
    double t;
    //w=2.0*PI*L/T; // częstotliwość sinusoidy


     // Skok jednostkowy jako wejście
    for (int i = 0; i < total; i++) {
        u[i] = 1.0;
        u1p[i] = 0.0;
    }


     for (int i = 0; i < total; i++) {
        t = i * h * 1.0;
        y_out[i] = y[0];  // zapisz wyjście!

        double k1[3], k2[3], k3[3], k4[3], y_temp[3];

        compute_derivatives(y, k1, u[i], u1p[i]);

        for (int j = 0; j < 3; j++)
            y_temp[j] = y[j] + 0.5 * h * k1[j];
        compute_derivatives(y_temp, k2, u[i], u1p[i]);

        for (int j = 0; j < 3; j++)
            y_temp[j] = y[j] + 0.5 * h * k2[j];
        compute_derivatives(y_temp, k3, u[i], u1p[i]);

        for (int j = 0; j < 3; j++)
            y_temp[j] = y[j] + h * k3[j];
        compute_derivatives(y_temp, k4, u[i], u1p[i]);

        for (int j = 0; j < 3; j++)
            y[j] += (h / 6.0) * (k1[j] + 2.0 * k2[j] + 2.0 * k3[j] + k4[j]);
    }


    ofstream fileU("D:\\PROJECTS\\projekt_MMM\\mmm_cpp_proba_1\\fileU.bin",ios::binary); // zapisanie wyników u(t) do pliku
    if (fileU.fail())
    {
        cout << "\n\n Saving u(t): error! Press a key ... \n\n";
        (void)getch();
        return(-1);
    }
    // Zapis całej tablicy u do pliku binarnego
    fileU.write(reinterpret_cast<const char*>(u), sizeof(u));
    fileU.close();

    ofstream fileY("D:\\PROJECTS\\projekt_MMM\\mmm_cpp_proba_1\\fileY.bin",ios::binary); // zapisanie wyników y(t) do pliku
    if (fileY.fail())
    {
        cout << "\n\n Saving y(t): error! Press a key ... \n\n";
        (void)getch();
        return(-2);
    }
    // Zapis całej tablicy y do pliku binarnego
    fileY.write(reinterpret_cast<const char*>(y_out), sizeof(y_out));
    fileY.close();

    return 0;
} 


