#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <random>

enum class Continent {
    EUROPA,
    AFRICA,
    ASIA,
    AMERICA,
    TOATE
};

enum class Dificultate {
    USOR,
    DIFICIL
};

class Tara
{
private:
    std::string nume;
    std::string capitala;
    Continent continent;
    bool populara;

public:
    Tara() {};

    ~Tara() {};

    explicit Tara(const std::string& nume, const std::string& capitala, Continent continent, bool populara = false)
    {
        this->nume = nume;
        this->capitala = capitala;
        this->continent = continent;
        this->populara = populara;
    }

    Tara(const Tara& other)
    {
        this->nume = other.nume;
        this->capitala = other.capitala;
        this->continent = other.continent;
        this->populara = other.populara;
    }

    Tara& operator=(const Tara& other)
    {
        this->nume = other.nume;
        this->capitala = other.capitala;
        this->continent = other.continent;
        this->populara = other.populara;
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

    Continent getContinent() const
    {
        return continent;
    }
    bool estePopulara() const
    {
        return populara;
    }

    bool operator==(const Tara& other) const
    {
        return nume == other.nume && capitala == other.capitala && continent == other.continent;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tara& tara)
    {
        os << "Tara: " << tara.nume << ", Capitala: " << tara.capitala << ", Continentul: ";
        switch(tara.continent) {
        case Continent::EUROPA: os << "Europa"; break;
        case Continent::AFRICA: os << "Africa"; break;
        case Continent::ASIA: os << "Asia"; break;
        case Continent::AMERICA: os << "America"; break;
        case Continent::TOATE: os << "Toate"; break;
        default: os << "Necunoscut";
    }
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

    /*void reseteaza()
    {
        puncte = 0;
    }*/

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
    Continent continentCurent;
    Dificultate dificultate;


    void amestecaTari()
    {
        std::mt19937 rng(std::time(0));
        std::shuffle(tari.begin(), tari.end(), rng);
    }

    Tara getTaraAleatorie(Continent continent) {
    std::vector<Tara> tariDisponibile;


    for (const auto& tara : tari) {
        bool esteNeintrebata = (std::find(tariIntrebate.begin(), tariIntrebate.end(), tara) == tariIntrebate.end());
        bool continentPotrivit = (continent == Continent::TOATE) || (tara.getContinent() == continent);

        if (esteNeintrebata && continentPotrivit) {
            tariDisponibile.push_back(tara);
        }
    }

    if (tariDisponibile.empty()) {
        std::cerr << "Eroare: Toate tarile din categoria selectata au fost deja intrebate.\n";
        exit(1);
    }


    std::mt19937 rng(std::time(0));
    std::uniform_int_distribution<int> dist(0, tariDisponibile.size() - 1);
    Tara taraAleatorie = tariDisponibile[dist(rng)];

    tariIntrebate.push_back(taraAleatorie);
    return taraAleatorie;
}


public:

    explicit Joc(const std::string& numeJucator, const std::vector<Tara>& listaTari, Continent continent, Dificultate dificultate)
        : jucator(numeJucator), tari(listaTari), continentCurent(continent), dificultate(dificultate)
    {
        if (tari.empty())
        {
            std::cerr << "Eroare: Lista de tari este goala.\n";
            exit(1);
        }
        amestecaTari();
    }

    void start() {
    tariIntrebate.clear();
    std::cout << "Bun venit, " << jucator << "!\n";
    std::cout << "Ghiceste capitala fiecarei tari.\n";

    for (int i = 0; i < 10; ++i) {
        Tara taraCurenta = getTaraAleatorie(continentCurent);
        std::string capitalaJucator;

        std::cout << "\n--- Intrebarea " << (i + 1) << " ---\n";
        std::cout << "Care este capitala tarii " << taraCurenta.getNume() << "?\n";
        std::cout << "Tasteaza -hint- pentru a descoperi prima litera: ";
        std::getline(std::cin, capitalaJucator);

        if (capitalaJucator == "hint" || capitalaJucator == "Hint") {
            getIndiciu(taraCurenta);
            std::cout << "Ai pierdut 2 puncte!\n";
            std::cout << "Acum, scrie numele capitalei -> ";
            std::getline(std::cin, capitalaJucator);
            jucator.actualizeazaScor(-2);
        }

        if (capitalaJucator == taraCurenta.getCapitala()) {
            jucator.actualizeazaScor(10);
            std::cout << "Corect! Ai acum " << jucator.getScor() << " puncte.\n";
        } else {
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

    void getIndiciu(const Tara& tara) const {
        if (tara.getCapitala().empty()) {
            std::cout << "Nu exista indiciu disponibil.\n";
        }
        std::cout << "Prima litera a capitalei este: " + tara.getCapitala().substr(0, 1)<<"\n";

    }

};

int main()
{
    std::vector<Tara> tari =
{
    Tara("Afganistan", "Kabul", Continent::ASIA, false),
    Tara("Africa de Sud", "Pretoria", Continent::AFRICA, false),
    Tara("Albania", "Tirana", Continent::EUROPA, true),
    Tara("Algeria", "Alger", Continent::AFRICA, false),
    Tara("Andorra", "Andorra la Vella", Continent::EUROPA, true),
    Tara("Angola", "Luanda", Continent::AFRICA, false),
    Tara("Antigua si Barbuda", "Saint John's", Continent::AMERICA, false),
    Tara("Arabia Saudita", "Riyadh", Continent::ASIA, false),
    Tara("Argentina", "Buenos Aires", Continent::AMERICA, false),
    Tara("Armenia", "Erevan", Continent::ASIA, false),
    Tara("Austria", "Viena", Continent::EUROPA, true),
    Tara("Azerbaidjan", "Baku", Continent::ASIA, false),
    Tara("Bahamas", "Nassau", Continent::AMERICA, false),
    Tara("Bahrain", "Manama", Continent::ASIA, false),
    Tara("Bangladesh", "Dhaka", Continent::ASIA, false),
    Tara("Barbados", "Bridgetown", Continent::AMERICA, false),
    Tara("Belarus", "Minsk", Continent::EUROPA, true),
    Tara("Belgia", "Bruxelles", Continent::EUROPA, true),
    Tara("Belize", "Belmopan", Continent::AMERICA, false),
    Tara("Benin", "Porto-Novo", Continent::AFRICA, false),
    Tara("Bhutan", "Thimphu", Continent::ASIA, false),
    Tara("Bolivia", "Sucre", Continent::AMERICA, false),
    Tara("Bosnia si Hertegovina", "Sarajevo", Continent::EUROPA, true),
    Tara("Botswana", "Gaborone", Continent::AFRICA, false),
    Tara("Brazilia", "Brasilia", Continent::AMERICA, false),
    Tara("Brunei", "Bandar Seri Begawan", Continent::ASIA, false),
    Tara("Bulgaria", "Sofia", Continent::EUROPA, true),
    Tara("Burkina Faso", "Ouagadougou", Continent::AFRICA, false),
    Tara("Burundi", "Bujumbura", Continent::AFRICA, false),
    Tara("Cambodgia", "Phnom Penh", Continent::ASIA, false),
    Tara("Camerun", "Yaounde", Continent::AFRICA, false),
    Tara("Canada", "Ottawa", Continent::AMERICA, false),
    Tara("Capul Verde", "Praia", Continent::AFRICA, false),
    Tara("Cehia", "Praga", Continent::EUROPA, true),
    Tara("Chile", "Santiago", Continent::AMERICA, false),
    Tara("China", "Beijing", Continent::ASIA, false),
    Tara("Cipru", "Nicosia", Continent::EUROPA, true),
    Tara("Columbia", "Bogota", Continent::AMERICA, false),
    Tara("Comoros", "Moroni", Continent::AFRICA, false),
    Tara("Congo", "Brazzaville", Continent::AFRICA, false),
    Tara("Congo (Republica Democrată)", "Kinshasa", Continent::AFRICA, false),
    Tara("Coreea de Nord", "Pyongyang", Continent::ASIA, false),
    Tara("Coreea de Sud", "Seoul", Continent::ASIA, false),
    Tara("Costa Rica", "San Jose", Continent::AMERICA, false),
    Tara("Croatia", "Zagreb", Continent::EUROPA, true),
    Tara("Cuba", "Havana", Continent::AMERICA, false),
    Tara("Danemarca", "Copenhaga", Continent::EUROPA, true),
    Tara("Djibouti", "Djibouti", Continent::AFRICA, false),
    Tara("Dominica", "Roseau", Continent::AMERICA, false),
    Tara("Ecuador", "Quito", Continent::AMERICA, false),
    Tara("Egipt", "Cairo", Continent::AFRICA, false),
    Tara("El Salvador", "San Salvador", Continent::AMERICA, false),
    Tara("Elvetria", "Bern", Continent::EUROPA, true),
    Tara("Emiratele Arabe Unite", "Abu Dhabi", Continent::ASIA, false),
    Tara("Eritrea", "Asmara", Continent::AFRICA, false),
    Tara("Estonia", "Tallinn", Continent::EUROPA, true),
    Tara("Eswatini", "Mbabane", Continent::AFRICA, false),
    Tara("Etiopia", "Addis Ababa", Continent::AFRICA, false),
    Tara("Filipine", "Manila", Continent::ASIA, false),
    Tara("Finlanda", "Helsinki", Continent::EUROPA, true),
    Tara("Franta", "Paris", Continent::EUROPA, true),
    Tara("Gabon", "Libreville", Continent::AFRICA, false),
    Tara("Gambia", "Banjul", Continent::AFRICA, false),
    Tara("Georgia", "Tbilisi", Continent::ASIA, false),
    Tara("Germania", "Berlin", Continent::EUROPA, true),
    Tara("Ghana", "Accra", Continent::AFRICA, false),
    Tara("Grecia", "Atena", Continent::EUROPA, true),
    Tara("Grenada", "Saint George's", Continent::AMERICA, false),
    Tara("Guatemala", "Guatemala City", Continent::AMERICA, false),
    Tara("Guineea", "Conakry", Continent::AFRICA, false),
    Tara("Guineea-Bissau", "Bissau", Continent::AFRICA, false),
    Tara("Guineea Ecuatoriala", "Malabo", Continent::AFRICA, false),
    Tara("Guyana", "Georgetown", Continent::AMERICA, false),
    Tara("Haiti", "Port-au-Prince", Continent::AMERICA, false),
    Tara("Honduras", "Tegucigalpa", Continent::AMERICA, false),
    Tara("India", "New Delhi", Continent::ASIA, false),
    Tara("Indonezia", "Jakarta", Continent::ASIA, false),
    Tara("Iordania", "Amman", Continent::ASIA, false),
    Tara("Irak", "Baghdad", Continent::ASIA, false),
    Tara("Iran", "Teheran", Continent::ASIA, false),
    Tara("Irlanda", "Dublin", Continent::EUROPA, true),
    Tara("Islanda", "Reykjavik", Continent::EUROPA, true),
    Tara("Israel", "Ierusalim", Continent::ASIA, false),
    Tara("Italia", "Roma", Continent::EUROPA, true),
    Tara("Jamaica", "Kingston", Continent::AMERICA, false),
    Tara("Japonia", "Tokyo", Continent::ASIA, false),
    Tara("Kazahstan", "Nur-Sultan", Continent::ASIA, false),
    Tara("Kenya", "Nairobi", Continent::AFRICA, false),
    Tara("Kuwait", "Kuwait City", Continent::ASIA, false),
    Tara("Kyrgyzstan", "Bishkek", Continent::ASIA, false),
    Tara("Laos", "Vientiane", Continent::ASIA, false),
    Tara("Letonia", "Riga", Continent::EUROPA, true),
    Tara("Liban", "Beirut", Continent::ASIA, false),
    Tara("Lesotho", "Maseru", Continent::AFRICA, false),
    Tara("Liberia", "Monrovia", Continent::AFRICA, false),
    Tara("Libia", "Tripoli", Continent::AFRICA, false),
    Tara("Liechtenstein", "Vaduz", Continent::EUROPA, true),
    Tara("Lituania", "Vilnius", Continent::EUROPA, true),
    Tara("Luxemburg", "Luxemburg", Continent::EUROPA, true),
    Tara("Madagascar", "Antananarivo", Continent::AFRICA, false),
    Tara("Malawi", "Lilongwe", Continent::AFRICA, false),
    Tara("Malaysia", "Kuala Lumpur", Continent::ASIA, false),
    Tara("Maldives", "Male", Continent::ASIA, false),
    Tara("Mali", "Bamako", Continent::AFRICA, false),
    Tara("Malta", "Valletta", Continent::EUROPA, true),
    Tara("Maroc", "Rabat", Continent::AFRICA, false),
    Tara("Mauritania", "Nouakchott", Continent::AFRICA, false),
    Tara("Mauritius", "Port Louis", Continent::AFRICA, false),
    Tara("Mexic", "Mexico City", Continent::AMERICA, false),
    Tara("Moldova", "Chisinau", Continent::EUROPA, true),
    Tara("Monaco", "Monaco", Continent::EUROPA, true),
    Tara("Mongolia", "Ulaanbaatar", Continent::ASIA, false),
    Tara("Muntenegru", "Podgorica", Continent::EUROPA, true),
    Tara("Mozambic", "Maputo", Continent::AFRICA, false),
    Tara("Myanmar", "Naypyidaw", Continent::ASIA, false),
    Tara("Namibia", "Windhoek", Continent::AFRICA, false),
    Tara("Nepal", "Kathmandu", Continent::ASIA, false),
    Tara("Nicaragua", "Managua", Continent::AMERICA, false),
    Tara("Niger", "Niamey", Continent::AFRICA, false),
    Tara("Nigeria", "Abuja", Continent::AFRICA, false),
    Tara("Norvegia", "Oslo", Continent::EUROPA, true),
    Tara("Oman", "Muscat", Continent::ASIA, false),
    Tara("Pakistan", "Islamabad", Continent::ASIA, false),
    Tara("Panama", "Panama City", Continent::AMERICA, false),
    Tara("Paraguay", "Asuncion", Continent::AMERICA, false),
    Tara("Peru", "Lima", Continent::AMERICA, false),
    Tara("Polonia", "Varsovia", Continent::EUROPA, true),
    Tara("Portugalia", "Lisabona", Continent::EUROPA, true),
    Tara("Qatar", "Doha", Continent::ASIA, false),
    Tara("Romania", "Bucuresti", Continent::EUROPA, true),
    Tara("Rusia", "Moscova", Continent::EUROPA, true),
    Tara("Rwanda", "Kigali", Continent::AFRICA, false),
    Tara("San Marino", "San Marino", Continent::EUROPA, true),
    Tara("Sao Tome si Principe", "Sao Tome", Continent::AFRICA, false),
    Tara("Senegal", "Dakar", Continent::AFRICA, false),
    Tara("Serbia", "Belgrad", Continent::EUROPA, true),
    Tara("Seychelles", "Victoria", Continent::AFRICA, false),
    Tara("Sierra Leone", "Freetown", Continent::AFRICA, false),
    Tara("Singapore", "Singapore", Continent::ASIA, false),
    Tara("Siria", "Damasc", Continent::ASIA, false),
    Tara("Slovacia", "Bratislava", Continent::EUROPA, true),
    Tara("Slovenia", "Ljubljana", Continent::EUROPA, true),
    Tara("Somalia", "Mogadishu", Continent::AFRICA, false),
    Tara("Spania", "Madrid", Continent::EUROPA, true),
    Tara("Sri Lanka", "Sri Jayawardenepura Kotte", Continent::ASIA, false),
    Tara("Statele Unite ale Americii", "Washington, D.C.", Continent::AMERICA, false),
    Tara("Sudan", "Khartoum", Continent::AFRICA, false),
    Tara("Sudanul de Sud", "Juba", Continent::AFRICA, false),
    Tara("Suedia", "Stockholm", Continent::EUROPA, true),
    Tara("Surinam", "Paramaribo", Continent::AMERICA, false),
    Tara("Tadjikistan", "Dushanbe", Continent::ASIA, false),
    Tara("Tanzania", "Dodoma", Continent::AFRICA, false),
    Tara("Thailanda", "Bangkok", Continent::ASIA, false),
    Tara("Timorul de Est", "Dili", Continent::ASIA, false),
    Tara("Togo", "Lome", Continent::AFRICA, false),
    Tara("Trinidad si Tobago", "Port of Spain", Continent::AMERICA, false),
    Tara("Tunisia", "Tunis", Continent::AFRICA, false),
    Tara("Turcia", "Ankara", Continent::ASIA, false),
    Tara("Turkmenistan", "Ashgabat", Continent::ASIA, false),
    Tara("Ucraina", "Kiev", Continent::EUROPA, true),
    Tara("Uganda", "Kampala", Continent::AFRICA, false),
    Tara("Ungaria", "Budapesta", Continent::EUROPA, true),
    Tara("Uruguay", "Montevideo", Continent::AMERICA, false),
    Tara("Uzbekistan", "Tashkent", Continent::ASIA, false),
    Tara("Vatican", "Vatican", Continent::EUROPA, true),
    Tara("Venezuela", "Caracas", Continent::AMERICA, false),
    Tara("Vietnam", "Hanoi", Continent::ASIA, false),
    Tara("Yemen", "Sanaa", Continent::ASIA, false),
    Tara("Zambia", "Lusaka", Continent::AFRICA, false),
    Tara("Zimbabwe", "Harare", Continent::AFRICA, false)
};

    std::string numeJucator;
    std::cout << "Introdu numele tau: ";
    std::getline(std::cin, numeJucator);

    std::cout << "\nSelecteaza dificultatea:\n";
    std::cout << "1. Usor (Doar tari populare - din Europa)\n";
    std::cout << "2. Dificil (Toate tarile)\n";
    std::cout << "Alegere (1-2): ";

    int optiuneDificultate;
    std::cin >> optiuneDificultate;
    std::cin.ignore();

    Dificultate dificultate;

    if(optiuneDificultate == 1)
        dificultate = Dificultate ::USOR;
    else
        dificultate = Dificultate::DIFICIL;


    std::vector<Tara> tariFiltrate;
    if (dificultate == Dificultate::USOR) {
        for (const auto& tara : tari) {
            if (tara.getContinent() == Continent::EUROPA && tara.estePopulara()) {
                tariFiltrate.push_back(tara);
            }
        }
    } else {
        tariFiltrate = tari;
    }


    Continent continent;

    if(dificultate == Dificultate::USOR)
        continent = Continent::EUROPA;
    else
        continent = Continent::TOATE;

    Joc joc(numeJucator, tariFiltrate, continent, dificultate);
    joc.start();

    return 0;
}
