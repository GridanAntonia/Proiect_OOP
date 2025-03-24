#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <random>

class Tara
{
private:
    std::string nume;
    std::string capitala;

public:
    Tara() {};

    ~Tara() {};

    explicit Tara(const std::string& nume, const std::string& capitala)
    {
        this->nume = nume;
        this->capitala = capitala;
    }

    Tara(const Tara& other)
    {
        this->nume = other.nume;
        this->capitala = other.capitala;
    }

    Tara& operator=(const Tara& other)
    {
        this->nume = other.nume;
        this->capitala = other.capitala;
        return *this;
    }

    /*void setNume(const std::string& nume)
    {
        this->nume = nume;
    }

    void setCapitala(const std::string& capitala)
    {
        this->capitala = capitala;
    }*/

    const std::string& getNume() const
    {
        return nume;
    }

    const std::string& getCapitala() const
    {
        return capitala;
    }

    bool operator==(const Tara& other) const
    {
        return nume == other.nume && capitala == other.capitala;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tara& tara)
    {
        os << "Tara: " << tara.nume << ", Capitala: " << tara.capitala;
        return os;
    }
};

class Scor
{
private:
    int puncte;

public:
    Scor()
    {
        puncte = 0;
    }

    explicit Scor(int puncte)
    {
        this->puncte = puncte;
    }

    void adaugaPuncte(int valoare)
    {
        puncte += valoare;
    }

    void reseteaza()
    {
        puncte = 0;
    }

    int getPuncte() const
    {
        return puncte;
    }

    friend std::ostream& operator<<(std::ostream& os, const Scor& scor)
    {
        os << "Scor: " << scor.puncte;
        return os;
    }
};

class Jucator
{
private:
    std::string NumeJucator;
    Scor scor;

public:
    explicit Jucator(const std::string& NumeJucator)
    {
        this->NumeJucator = NumeJucator;
    }

    void actualizeazaScor(int puncte)
    {
        scor.adaugaPuncte(puncte);
    }

    /*void reseteazaScor()
    {
        scor.reseteaza();
    }*/

    int getScor() const
    {
        return scor.getPuncte();
    }

    friend std::ostream& operator<<(std::ostream& os, const Jucator& jucator)
    {
        os << "Jucator: " << jucator.NumeJucator << ", " << jucator.scor;
        return os;
    }
};

class Joc
{
private:
    Jucator jucator;
    std::vector<Tara> tari;
    std::vector<Tara> tariIntrebate;

    void amestecaTari()
    {
        std::mt19937 rng(std::time(0));
        std::shuffle(tari.begin(), tari.end(), rng);
    }

    Tara getTaraAleatorie()
    {
        if (tariIntrebate.size() == tari.size())
        {
            std::cerr << "Eroare: Toate tarile au fost deja intrebate.\n";
            exit(1);
        }

        Tara taraAleatorie = tari[std::rand() % tari.size()];

        while (std::find(tariIntrebate.begin(), tariIntrebate.end(), taraAleatorie) != tariIntrebate.end())
        {
            taraAleatorie = tari[std::rand() % tari.size()];
        }

        tariIntrebate.push_back(taraAleatorie);
        return taraAleatorie;
    }

public:

    explicit Joc(const std::string& numeJucator, const std::vector<Tara>& listaTari)
        : jucator(numeJucator), tari(listaTari)
    {
        if (tari.empty())
        {
            std::cerr << "Eroare: Lista de tari este goala.\n";
            exit(1);
        }
        amestecaTari();
    }

    void start()
    {
        tariIntrebate.clear();
        std::cout << "Bun venit, " << jucator << "!\n";
        std::cout << "Ghiceste capitala fiecarei tari.\n";

        for (int i = 0; i < 10; ++i)
        {
            Tara taraCurenta = getTaraAleatorie();
            std::string capitalaJucator;

            std::cout << "\n--- Intrebarea " << (i + 1) << " ---\n";
            std::cout << "Care este capitala tarii " << taraCurenta.getNume() << "? ";
            std::getline(std::cin, capitalaJucator);

            if (capitalaJucator == taraCurenta.getCapitala())
            {
                jucator.actualizeazaScor(10);
                std::cout << "Corect! Ai acum " << jucator.getScor() << " puncte.\n";
            }
            else
            {
                std::cout << "Gresit! Capitala corecta este " << taraCurenta.getCapitala() << ".\n";
            }
        }

        std::cout << "Joc terminat. " << jucator << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Joc& joc)
    {
        os << "Jocul este in desfasurare. " << joc.jucator;
        return os;
    }
};

int main()
{
    std::vector<Tara> tari =
    {
        Tara("Afganistan", "Kabul"),
        Tara("Africa de Sud", "Pretoria"),
        Tara("Albania", "Tirana"),
        Tara("Algeria", "Alger"),
        Tara("Andorra", "Andorra la Vella"),
        Tara("Angola", "Luanda"),
        Tara("Antigua si Barbuda", "Saint John's"),
        Tara("Arabia Saudita", "Riyadh"),
        Tara("Argentina", "Buenos Aires"),
        Tara("Armenia", "Erevan"),
        Tara("Australia", "Canberra"),
        Tara("Austria", "Viena"),
        Tara("Azerbaidjan", "Baku"),
        Tara("Bahamas", "Nassau"),
        Tara("Bahrain", "Manama"),
        Tara("Bangladesh", "Dhaka"),
        Tara("Barbados", "Bridgetown"),
        Tara("Belarus", "Minsk"),
        Tara("Belgia", "Bruxelles"),
        Tara("Belize", "Belmopan"),
        Tara("Benin", "Porto-Novo"),
        Tara("Bhutan", "Thimphu"),
        Tara("Bolivia", "Sucre"),
        Tara("Bosnia si Hertegovina", "Sarajevo"),
        Tara("Botswana", "Gaborone"),
        Tara("Brazilia", "Brasilia"),
        Tara("Brunei", "Bandar Seri Begawan"),
        Tara("Bulgaria", "Sofia"),
        Tara("Burkina Faso", "Ouagadougou"),
        Tara("Burundi", "Bujumbura"),
        Tara("Cambodgia", "Phnom Penh"),
        Tara("Camerun", "Yaounde"),
        Tara("Canada", "Ottawa"),
        Tara("Capul Verde", "Praia"),
        Tara("Cehia", "Praga"),
        Tara("Chile", "Santiago"),
        Tara("China", "Beijing"),
        Tara("Cipru", "Nicosia"),
        Tara("Columbia", "Bogota"),
        Tara("Comoros", "Moroni"),
        Tara("Congo", "Brazzaville"),
        Tara("Congo (Republica Democrată)", "Kinshasa"),
        Tara("Coreea de Nord", "Pyongyang"),
        Tara("Coreea de Sud", "Seoul"),
        Tara("Costa Rica", "San Jose"),
        Tara("Croatia", "Zagreb"),
        Tara("Cuba", "Havana"),
        Tara("Danemarca", "Copenhaga"),
        Tara("Djibouti", "Djibouti"),
        Tara("Dominica", "Roseau"),
        Tara("Ecuador", "Quito"),
        Tara("Egipt", "Cairo"),
        Tara("El Salvador", "San Salvador"),
        Tara("Elvetria", "Bern"),
        Tara("Emiratele Arabe Unite", "Abu Dhabi"),
        Tara("Eritrea", "Asmara"),
        Tara("Estonia", "Tallinn"),
        Tara("Eswatini", "Mbabane"),
        Tara("Etiopia", "Addis Ababa"),
        Tara("Fiji", "Suva"),
        Tara("Filipine", "Manila"),
        Tara("Finlanda", "Helsinki"),
        Tara("Franta", "Paris"),
        Tara("Gabon", "Libreville"),
        Tara("Gambia", "Banjul"),
        Tara("Georgia", "Tbilisi"),
        Tara("Germania", "Berlin"),
        Tara("Ghana", "Accra"),
        Tara("Grecia", "Atena"),
        Tara("Grenada", "Saint George's"),
        Tara("Guatemala", "Guatemala City"),
        Tara("Guineea", "Conakry"),
        Tara("Guineea-Bissau", "Bissau"),
        Tara("Guineea Ecuatoriala", "Malabo"),
        Tara("Guyana", "Georgetown"),
        Tara("Haiti", "Port-au-Prince"),
        Tara("Honduras", "Tegucigalpa"),
        Tara("India", "New Delhi"),
        Tara("Indonezia", "Jakarta"),
        Tara("Iordania", "Amman"),
        Tara("Irak", "Baghdad"),
        Tara("Iran", "Teheran"),
        Tara("Irlanda", "Dublin"),
        Tara("Islanda", "Reykjavik"),
        Tara("Israel", "Ierusalim"),
        Tara("Italia", "Roma"),
        Tara("Jamaica", "Kingston"),
        Tara("Japonia", "Tokyo"),
        Tara("Kazahstan", "Nur-Sultan"),
        Tara("Kenya", "Nairobi"),
        Tara("Kiribati", "Tarawa"),
        Tara("Kuwait", "Kuwait City"),
        Tara("Kyrgyzstan", "Bishkek"),
        Tara("Laos", "Vientiane"),
        Tara("Letonia", "Riga"),
        Tara("Liban", "Beirut"),
        Tara("Lesotho", "Maseru"),
        Tara("Liberia", "Monrovia"),
        Tara("Libia", "Tripoli"),
        Tara("Liechtenstein", "Vaduz"),
        Tara("Lituania", "Vilnius"),
        Tara("Luxemburg", "Luxemburg"),
        Tara("Madagascar", "Antananarivo"),
        Tara("Malawi", "Lilongwe"),
        Tara("Malaysia", "Kuala Lumpur"),
        Tara("Maldives", "Male"),
        Tara("Mali", "Bamako"),
        Tara("Malta", "Valletta"),
        Tara("Maroc", "Rabat"),
        Tara("Marshall Islands", "Majuro"),
        Tara("Mauritania", "Nouakchott"),
        Tara("Mauritius", "Port Louis"),
        Tara("Mexic", "Mexico City"),
        Tara("Micronezia", "Palikir"),
        Tara("Moldova", "Chisinau"),
        Tara("Monaco", "Monaco"),
        Tara("Mongolia", "Ulaanbaatar"),
        Tara("Muntenegru", "Podgorica"),
        Tara("Mozambic", "Maputo"),
        Tara("Myanmar", "Naypyidaw"),
        Tara("Namibia", "Windhoek"),
        Tara("Nauru", "Yaren"),
        Tara("Nepal", "Kathmandu"),
        Tara("Nicaragua", "Managua"),
        Tara("Niger", "Niamey"),
        Tara("Nigeria", "Abuja"),
        Tara("Norvegia", "Oslo"),
        Tara("Noua Zeelanda", "Wellington"),
        Tara("Oman", "Muscat"),
        Tara("Pakistan", "Islamabad"),
        Tara("Palau", "Melekeok"),
        Tara("Panama", "Panama City"),
        Tara("Papua Noua Guinee", "Port Moresby"),
        Tara("Paraguay", "Asuncion"),
        Tara("Peru", "Lima"),
        Tara("Polonia", "Varsovia"),
        Tara("Portugalia", "Lisabona"),
        Tara("Qatar", "Doha"),
        Tara("Romania", "Bucuresti"),
        Tara("Rusia", "Moscova"),
        Tara("Rwanda", "Kigali"),
        Tara("Samoa", "Apia"),
        Tara("San Marino", "San Marino"),
        Tara("Sao Tome si Principe", "Sao Tome"),
        Tara("Senegal", "Dakar"),
        Tara("Serbia", "Belgrad"),
        Tara("Seychelles", "Victoria"),
        Tara("Sierra Leone", "Freetown"),
        Tara("Singapore", "Singapore"),
        Tara("Siria", "Damasc"),
        Tara("Slovacia", "Bratislava"),
        Tara("Slovenia", "Ljubljana"),
        Tara("Somalia", "Mogadishu"),
        Tara("Spania", "Madrid"),
        Tara("Sri Lanka", "Sri Jayawardenepura Kotte"),
        Tara("Statele Unite ale Americii", "Washington, D.C."),
        Tara("Sudan", "Khartoum"),
        Tara("Sudanul de Sud", "Juba"),
        Tara("Suedia", "Stockholm"),
        Tara("Surinam", "Paramaribo"),
        Tara("Tadjikistan", "Dushanbe"),
        Tara("Tanzania", "Dodoma"),
        Tara("Thailanda", "Bangkok"),
        Tara("Timorul de Est", "Dili"),
        Tara("Togo", "Lome"),
        Tara("Tonga", "Nuku'alofa"),
        Tara("Trinidad si Tobago", "Port of Spain"),
        Tara("Tunisia", "Tunis"),
        Tara("Turcia", "Ankara"),
        Tara("Turkmenistan", "Ashgabat"),
        Tara("Tuvalu", "Funafuti"),
        Tara("Ucraina", "Kiev"),
        Tara("Uganda", "Kampala"),
        Tara("Ungaria", "Budapesta"),
        Tara("Uruguay", "Montevideo"),
        Tara("Uzbekistan", "Tashkent"),
        Tara("Vanuatu", "Port Vila"),
        Tara("Vatican", "Vatican"),
        Tara("Venezuela", "Caracas"),
        Tara("Vietnam", "Hanoi"),
        Tara("Yemen", "Sanaa"),
        Tara("Zambia", "Lusaka"),
        Tara("Zimbabwe", "Harare")
    };

    std::string numeJucator;
    std::cout << "Introdu numele tau: ";
    std::getline(std::cin, numeJucator);

    Joc joc(numeJucator, tari);
    joc.start();
    return 0;
}
