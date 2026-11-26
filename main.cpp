// Praktine uzduotis Nr. 3 "Dviraciu parduotuves uzsakymo skaiciuokle"

// #####################################################################
// BENDRI KODAI: PROGRAMOS PRADZIA IR SKAICIAVIMAI (eilutes 1-71)
// (juos naudoja visi meniu punktai)
// #####################################################################
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 100;
const string KAT[3] = {"Dviraciai", "Dalys", "Aksesuarai"};

// Nuskaito duomenis is failo. Grazina true, jei viskas gerai.
bool nuskaityti(string kat[], string pav[], double kaina[], int kiekis[], int &n)
{
    ifstream f("uzsakymas.txt");
    if (!f.is_open())
    {
        cout << "KLAIDA: nepavyko atverti uzsakymas.txt" << endl;
        return false;
    }

    f >> n;
    if (f.fail() || n < 1 || n > MAX)
    {
        cout << "KLAIDA: prekiu skaicius turi buti nuo 1 iki 100" << endl;
        return false;
    }
    f.ignore(1000, '\n');

    for (int i = 0; i < n; i++)
    {
        getline(f, kat[i], ';');
        getline(f, pav[i], ';');
        f >> kaina[i];
        f.ignore();                         // praleidziam ;
        f >> kiekis[i];
        bool klaida = f.fail();             // ar pavyko nuskaityti eilute
        char po = f.peek();                 // po kiekio turi buti eilutes galas
        f.ignore(1000, '\n');

        bool geraKat = (kat[i] == KAT[0] || kat[i] == KAT[1] || kat[i] == KAT[2]);
        bool sveikas = (po == '\n' || po == '\r' || po == EOF);

        if (klaida || !geraKat || !sveikas || kaina[i] <= 0 || kiekis[i] <= 0)
        {
            cout << "KLAIDA: netinkami duomenys " << i + 1 << " prekes eiluteje" << endl;
            return false;
        }
    }
    return true;
}

// Prekes eilutes suma pries nuolaida.
double eilutesSuma(double kaina, int kiekis)
{
    return kaina * kiekis;
}

// Nuolaida: 10 %, jei kiekis >= 10.
double nuolaida(double suma, int kiekis)
{
    if (kiekis >= 10)
        return suma * 0.10;
    return 0;
}



// 1 MENIU KODAI: VISOS PREKES (eilutes 72-94)

// Prekiu lentele. k == "" -> visos prekes, kitaip tik kategorijos k.
void lentele(ostream &out, string kat[], string pav[], double kaina[], int kiekis[], int n, string k)
{
    out << fixed << setprecision(2);
    out << left << setw(12) << "Kategorija" << setw(32) << "Pavadinimas"
        << right << setw(9) << "Kaina" << setw(7) << "Kiekis"
        << setw(10) << "Suma" << setw(10) << "Nuolaida" << setw(10) << "Moketi" << endl;

    for (int i = 0; i < n; i++)
        if (k == "" || kat[i] == k)
        {
            double s = eilutesSuma(kaina[i], kiekis[i]);
            double nu = nuolaida(s, kiekis[i]);
            out << left << setw(12) << kat[i] << setw(32) << pav[i]
                << right << setw(9) << kaina[i] << setw(7) << kiekis[i]
                << setw(10) << s << setw(10) << nu << setw(10) << s - nu << endl;
        }
}



// 2 MENIU KODAI: PASIRINKTOS KATEGORIJOS PREKES (eilutes 95-133)

// Vienos kategorijos moketina suma po nuolaidu (kategorija - parametras k).
double kategorijosSuma(string kat[], double kaina[], int kiekis[], int n, string k)
{
    double s = 0;
    for (int i = 0; i < n; i++)
        if (kat[i] == k)
        {
            double es = eilutesSuma(kaina[i], kiekis[i]);
            s += es - nuolaida(es, kiekis[i]);
        }
    return s;
}

// 2 meniu punktas: pasirenkama kategorija, rodomos jos prekes ir suma.
void meniu2(string kat[], string pav[], double kaina[], int kiekis[], int n)
{
    int k;
    cout << "1. Dviraciai  2. Dalys  3. Aksesuarai" << endl << "Pasirinkite (1-3): ";
    while (!(cin >> k) || k < 1 || k > 3)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Netinkamas pasirinkimas. Bandykite dar karta: ";
    }
    string kategorija = KAT[k - 1];
    double s = kategorijosSuma(kat, kaina, kiekis, n, kategorija);
    if (s == 0)                             // kainos teigiamos -> 0 reiskia, kad prekiu nera
        cout << "Kategorijoje " << kategorija << " prekiu nera." << endl;
    else
    {
        lentele(cout, kat, pav, kaina, kiekis, n, kategorija);
        cout << kategorija << " moketina suma: " << s << " Eur" << endl;
    }
}



// 3 MENIU KODAI: VISO UZSAKYMO SUMOS (eilutes 134-154)

// Viso uzsakymo sumos: pries nuolaidas, nuolaidos, moketina suma.
void visoSumos(ostream &out, double kaina[], int kiekis[], int n)
{
    double viso = 0, nuol = 0;
    for (int i = 0; i < n; i++)
    {
        double s = eilutesSuma(kaina[i], kiekis[i]);
        viso += s;
        nuol += nuolaida(s, kiekis[i]);
    }
    out << fixed << setprecision(2);
    out << "VISO UZSAKYMO SUMOS" << endl;
    out << "Suma pries nuolaidas: " << viso << " Eur" << endl;
    out << "Nuolaidu suma:        " << nuol << " Eur" << endl;
    out << "Moketina suma:        " << viso - nuol << " Eur" << endl;
}



// 4 MENIU KODAI: SUMOS PAGAL KATEGORIJAS (eilutes 155-166)

// Kiekvienos is 3 kategoriju suma (ta pati funkcija kategorijosSuma).
void kategorijuSumos(ostream &out, string kat[], double kaina[], int kiekis[], int n)
{
    out << fixed << setprecision(2);
    for (int i = 0; i < 3; i++)
        out << KAT[i] << ": " << kategorijosSuma(kat, kaina, kiekis, n, KAT[i]) << " Eur" << endl;
}



// 5 MENIU KODAI: VISA ATASKAITA (eilutes 167-181)

// Visa ataskaita: prekiu lentele + viso sumos + kategoriju suvestine.
void ataskaita(ostream &out, string kat[], string pav[], double kaina[], int kiekis[], int n)
{
    out << "PREKES" << endl;
    lentele(out, kat, pav, kaina, kiekis, n, "");
    out << endl;
    visoSumos(out, kaina, kiekis, n);
    out << endl << "SUMOS PAGAL KATEGORIJAS" << endl;
    kategorijuSumos(out, kat, kaina, kiekis, n);
}



// 6 MENIU KODAI: ISSAUGOTI ATASKAITA I FAILA (eilutes 182-202)

// 6 meniu punktas: visa ataskaita irasoma i faila ataskaita.txt.
void meniu6(string kat[], string pav[], double kaina[], int kiekis[], int n)
{
    ofstream rez("ataskaita.txt");          // perraso sena faila
    if (!rez.is_open())
        cout << "KLAIDA: nepavyko atverti ataskaita.txt" << endl;
    else
    {
        ataskaita(rez, kat, pav, kaina, kiekis, n);
        rez.close();
        if (rez.fail())
            cout << "KLAIDA: nepavyko irasyti i ataskaita.txt" << endl;
        else
            cout << "Ataskaita issaugota faile ataskaita.txt" << endl;
    }
}



// MAIN: NUSKAITYMAS, MENIU IR 7 MENIU KODAI - ISEITI (eilutes 203-247)

int main()
{
    string kat[MAX], pav[MAX];
    double kaina[MAX];
    int kiekis[MAX];
    int n;

    if (!nuskaityti(kat, pav, kaina, kiekis, n))
    {
        cout << "Programa baigiama." << endl;
        return 1;
    }

    int p = 0;
    while (p != 7)                          // 7 MENIU: kai p == 7, ciklas baigiasi
    {
        cout << endl << "1. Visos prekes" << endl
             << "2. Pasirinktos kategorijos prekes" << endl
             << "3. Viso uzsakymo sumos" << endl
             << "4. Sumos pagal kategorijas" << endl
             << "5. Visa ataskaita" << endl
             << "6. Issaugoti ataskaita i faila" << endl
             << "7. Iseiti" << endl
             << "Pasirinkite (1-7): ";

        while (!(cin >> p) || p < 1 || p > 7)        // tikrinam pasirinkima
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Netinkamas pasirinkimas. Bandykite dar karta: ";
        }

        if (p == 1)      lentele(cout, kat, pav, kaina, kiekis, n, "");   // 1 MENIU
        else if (p == 2) meniu2(kat, pav, kaina, kiekis, n);              // 2 MENIU
        else if (p == 3) visoSumos(cout, kaina, kiekis, n);               // 3 MENIU
        else if (p == 4) kategorijuSumos(cout, kat, kaina, kiekis, n);    // 4 MENIU
        else if (p == 5) ataskaita(cout, kat, pav, kaina, kiekis, n);     // 5 MENIU
        else if (p == 6) meniu6(kat, pav, kaina, kiekis, n);              // 6 MENIU
    }
    cout << "Programa baigta." << endl;     // 7 MENIU: Iseiti
    return 0;
}
