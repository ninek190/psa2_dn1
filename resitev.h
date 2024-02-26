#include <string>
#include <vector>
#include <tuple>
#include <utility>


using std::vector;
using std::string;
using std::pair;
using std::tuple;


class Resitev {

    void povecaj(long long emso_1, long long emso_2, long bonboni); 

protected:
    long long n_vozlisc;

    long long n_notranjih_vozlisc;

    vector<pair<pair<long long, long long>, pair<pair<long long, long long>, long long>>> vozlisca;
    vector<long long> listi_poddrevesa;


    tuple<long long, long long, vector<long long>, vector<long long>, long long, pair<vector<long long>, vector<long long>>>  izracunaj_poti_itd(long long i, long long j);
    tuple<long long, long long, vector<long long>, vector<long long>, long long, pair<vector<long long>, vector<long long>>>  izracunaj_poti_itd_2(long long i, long long j);
    long long najdi_list(long long i);
    long long n_listov_poddrevo(long long i);
    bool je_notranje(long long i);

    void posodobi_beta(long long i);

    int odgovor_vzdolz_obeh_poti(long long i, long long j);
    

    static pair<long long, bool> posodobitev_z_notranjim(int i, vector<long long> & pot, long long j, long long node);
    static vector<long long> pot_iz_lista(long long i_list);

    static pair<long long, pair<vector<long long>, vector<long long>>> skupni_prednik_in_poti_pod_njim(vector<long long> & pot_levo, vector<long long> & pot_desno);
    static vector<long long> notranji_koreni(vector<long long> & pot_levo, vector<long long> & pot_desno);

    static pair<long long, long long> izracunaj_velikost_polnega_drevesa(long long n);
    static long long izracunaj_starsa(long long i);
    static long long izracunaj_levega(long long i);
    static long long izracunaj_desnega(long long i);

public:
    // potrebujemo (tudi) tak konstruktor
    Resitev(vector<long long> emsoOtrok);
    

    // in tako metodo obdelaj
    int obdelaj(const string & ukaz);
};