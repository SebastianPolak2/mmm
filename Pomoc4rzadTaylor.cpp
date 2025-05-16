// System z pliku pomocniczego 4 rzędu Taylor'a
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
#define M 8.0 // amplituda sygnału sinus
#define PI 3.14159265 // liczba PI


// zmienne globalne w programie
double u[(int)(1.0*T/h)+1]; // sygnał wejściowy
double u1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wejściowego
double u2p[(int)(1.0*T/h)+1]; // druga pochodna sygnału wejściowego
double u3p[(int)(1.0*T/h)+1]; // trzecia pochodna sygnału wejściowego
double y[(int)(1.0*T/h)+1]; // sygnał wyjściowy
double y1p[(int)(1.0*T/h)+1]; // pierwsza pochodna sygnału wyjściowego
double y2p[(int)(1.0*T/h)+1]; // druga pochodna sygnału wyjściowego
double y3p[(int)(1.0*T/h)+1]; // trzecia pochodna sygnału wyjściowego
double y4p[(int)(1.0*T/h)+1]; // czwarta pochodna sygnału wyjściowego

// program główny
int main()
{
    int i,j,total;
    double a3,a2,a1,a0,b3,b2,b1,b0,w;

    // wczytanie parametrów modelu
    cout << "\n a3 = "; cin >> a3;
    cout << "\n a2 = "; cin >> a2;
    cout << "\n a1 = "; cin >> a1;
    cout << "\n a0 = "; cin >> a0;
    cout << "\n b3 = "; cin >> b3;
    cout << "\n b2 = "; cin >> b2;
    cout << "\n b1 = "; cin >> b1;
    cout << "\n b0 = "; cin >> b0;
    cout << "\n\n";

    total=sizeof(u)/sizeof(u[0]); // rozmiar wektorów danych
    w=2.0*PI*L/T; // częstotliwość sinusoidy

    /*
    for(i=0; i<total; i++) // sygnał wejściowy i jego pochodne
    {
        u[i]=M*sin(w*i*h); // sygnał wejściowy: u=M*sin(w*t) , t=i*h
        u1p[i]=M*w*cos(w*i*h); // pochodna 1: u’(t)
        u2p[i]=-M*w*w*sin(w*i*h); // pochodna 2: u’’(t)
        u3p[i]=-M*w*w*w*cos(w*i*h); // pochodna 3: u’’’(t)
    }*/

    // wejście skokowe
    for(i=0; i<total; i++) // sygnał wejściowy i jego pochodne
    {
        u[i] = 1.0; // sygnał wejściowy 
        u1p[i] = 0.0; // pochodna 1: u’(t)
        u2p[i] = 0.0; 
        u3p[i] = 0.0; 
    }

        y[0]=0; y1p[0]=0; y2p[0]=0; y3p[0]=0; // zerowe warunki początkowe

    for(i=0; i<total-1; i++) // główna pętla obliczeń
    {
        y4p[i]=-a3*y3p[i]-a2*y2p[i]-a1*y1p[i]-a0*y[i]+b3*u3p[i]+b2*u2p[i]+b1*u1p[i]+b0*u[i];
        y3p[i+1]=y3p[i]+h*y4p[i];
        y2p[i+1]=y2p[i]+h*y3p[i]+(h*h/2.0)*y4p[i];
        y1p[i+1]=y1p[i]+h*y2p[i]+(h*h/2.0)*y3p[i]+(h*h*h/6.0)*y4p[i];
        y[i+1]=y[i]+h*y1p[i]+(h*h/2.0)*y2p[i]+(h*h*h/6.0)*y3p[i]+(h*h*h*h/24.0)*y4p[i];
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
    fileY.write(reinterpret_cast<const char*>(y), sizeof(y));
    fileY.close();

    return 0;
} 
