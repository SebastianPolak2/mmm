// Taylor rząd 3                //fileUYstep.bin
#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>

// deklaracje zasięgów
using namespace std;

// stałe w programie
#define h 0.001 // krok obliczeń
#define T 50.0 // całkowity czas symulacji – przedział [0 , T]
#define L 5.0 // liczba okresów sygnału sinus w przedziale T
#define M 1.0 // amplituda sygnału sinus
#define PI 3.14159265 // liczba PI


// zmienne globalne w programie
double u[(int)(1.0*T/h)+1]; // sygnał wejściowy
double u1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wejściowego
double y[(int)(1.0*T/h)+1]; // sygnał wyjściowy
double y1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wyjściowego
double y2p[(int)(1.0*T/h)+1]; // druga pochodna sygnału wyjściowego
double y3p[(int)(1.0*T/h)+1]; // trzecia pochodna sygnału wyjściowego

// program główny
int main()
{
    int i,j,total;
    double a3,a2,a1,a0,b1,b0,w;
    double c0,c1,c2,d0,d1; 
    double T1, T2, k1, k2;
    
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
    
    /*
    //test
    b1 = 2.0;
    b0 = 1.0;

    a3 = 1.0;
    a2 = 6.0;
    a1 = 11.0;
    a0 = 6.0;
    //*/  

    // współczynniki układu równań różniczkowych
    c2 = 1.0*a2/a3;
    c1 = 1.0*a1/a3;
    c0 = 1.0*a0/a3;
    d1 = 1.0*b1/a3;
    d0 = 1.0*b0/a3;

    total=sizeof(u)/sizeof(u[0]); // rozmiar wektorów danych
    w=2.0*PI*L/T; // częstotliwość sinusoidy

    /*
    // wejscie sinusoidalne
    for(i=0; i<total; i++) // sygnał wejściowy i jego pochodne
    {
        u[i]=M*sin(w*i*h); // sygnał wejściowy: u=M*sin(w*t) , t=i*h
        u1p[i]=M*w*cos(w*i*h); // pochodna 1: u’(t)
    }   */


    // wejscie skokowe
    for(i=0; i<total; i++) // sygnał wejściowy i jego pochodne
    {
        u[i] = 1.0; // sygnał wejściowy 
        u1p[i] = 1/sqrt(2*PI*0.0005)*exp(-i*i/(2.0)*0.0005); // pochodna 1: u’(t)
    }  

        y[0] = 0.0; y1p[0] = 0.0; y2p[0] = 0.0; // zerowe warunki początkowe

    for(i=0; i<total-1; i++) // główna pętla obliczeń
    {
        y3p[i] = - c2*y2p[i] - c1*y1p[i] - c0*y[i] + d1*u1p[i] + d0*u[i];
        y2p[i+1] = y2p[i] + h*y3p[i];
        y1p[i+1] = y1p[i] + h*y2p[i] + (h*h/2.0)*y3p[i];
        y[i+1] = y[i] + h*y1p[i] + (h*h/2.0)*y2p[i] + (h*h*h/6.0)*y3p[i];

        if(std::isnan(y[i+1]) || std::isinf(y[i+1])) y[i+1] = y[i]; // zatrzymanie wartości na ostatniej poprawnej

        if(i % 1000 == 0)cout << i*h << " " << y[i] << "\n"; // wypisanie wyników na ekran
    }

    ofstream fileU("D:\\PROJECTS\\projekt_MMM\\mmm_cpp_proba_1\\fileUstep.bin",ios::binary); // zapisanie wyników u(t) do pliku
    if (fileU.fail())
    {
        cout << "\n\n Saving u(t): error! Press a key ... \n\n";
        (void)getch();
        return(-1);
    }
    // Zapis całej tablicy u do pliku binarnego
    fileU.write(reinterpret_cast<const char*>(u), sizeof(u));
    fileU.close();

    ofstream fileY("D:\\PROJECTS\\projekt_MMM\\mmm_cpp_proba_1\\fileYstep.bin",ios::binary); // zapisanie wyników y(t) do pliku
    if (fileY.fail())
    {
        cout << "\n\n Saving y(t): error! Press a key ... \n\n";
        (void)getch();
        return(-2);
    }
    // Zapis całej tablicy y do pliku binarnego
    fileY.write(reinterpret_cast<const char*>(y), sizeof(y));
    fileY.close();

    //sprawdzam coś
    cout << "\n\n";
    cout << c2 << " " << c1 << " " << c0 << " " << d1 << " " << d0 << "\n";
    cout << "\n\n";
    return 0;
} 
