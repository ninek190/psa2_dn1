#include <algorithm>
#include <iostream>
#include <set>
#include <cmath>
#include "resitev.h"

using std::cout;
using std::string;
using std::endl;

Resitev::Resitev(vector<long long> emsoOtrok) {
    auto n = (long) emsoOtrok.size();
    auto [n_vsa, n_noter] = izracunaj_velikost_polnega_drevesa(n);
    n_vozlisc = n_vsa;
    n_notranjih_vozlisc = n_noter;
    vozlisca = vector<pair<pair<long long, long long>, pair<pair<long long, long long>, long long>>>(n_vsa);
    std::fill(vozlisca.begin(), vozlisca.end(), std::make_pair(std::make_pair(0, 0), std::make_pair(std::make_pair(0, 0), 0)));
    listi_poddrevesa = vector<long long>(n_vsa);
    std::fill(listi_poddrevesa.begin(), listi_poddrevesa.end(), 0);
    for (long long i = 0; i < n; i++){
        vozlisca[i + n_notranjih_vozlisc].first.first = emsoOtrok[i];
    };
};

int Resitev::obdelaj(const string & ukaz){

    std::string ukaz_2 = ukaz.substr(2, ukaz.size() - 2);
    int i = ukaz_2.find(' ');
    long long prvi_emso = std::stoll(ukaz_2.substr(0, i));
    std::string ukaz_3 = ukaz_2.substr(i + 1, sizeof(ukaz_2));


    if (ukaz[0] == 'o') {
        long long drugi_emso = std::stoll(ukaz_2.substr(i, sizeof(ukaz_2)));
        return odgovor_vzdolz_obeh_poti(prvi_emso, drugi_emso);
    }
    else {
        int j = ukaz_3.find(' ');
        long long drugi_emso = std::stoll(ukaz_3.substr(0, j));
        long bonbonov = std::stol(ukaz_3.substr(j, sizeof(ukaz_3)));
        povecaj(prvi_emso, drugi_emso, bonbonov);
        return 0;
    }
};

// najdi_list da INDEX lista ki ima specifičen EMŠO, najde s bisekcijo

long long Resitev::najdi_list(long long el) {
    long long i = n_notranjih_vozlisc;
    long long x = n_vozlisc;
    if(vozlisca[i].first.first == el){
        return i;
    } else if(vozlisca[x].first.first == el){
        return x;
    } else {
        while(i < x)
        {
            long long mid = (i + x) / 2;
            if (el == vozlisca[mid].first.first && (mid == 0 || vozlisca[mid-1].first.first < el))
                return mid;
            if (el <= vozlisca[mid].first.first)
                x = mid;
            else
                i = mid + 1;
        }
    }
    return -1;
}
// torej če je index levega otroka manjši ko je število vseh vozlišč, basically če otrok sploh obstaja
// sprejme index vozlisca
bool Resitev::je_notranje(long long i){
    return izracunaj_levega(i) < n_vozlisc;
}

//najde pot od lista do korena, iList je index lista v drevesu

vector<long long> Resitev::pot_iz_lista(long long iList){
    long trenutni = iList;
    vector<long long> pot {trenutni};
    while (trenutni > 0){
        trenutni = izracunaj_starsa(trenutni);
        pot.push_back(trenutni);
    }
    return pot;
}

/*izracunaj_poti_itd sprejme emšo prvega pa emšo drugega otroka na intervalu med katerima nas stvari zanimajo
      - i_levo/desno: indeksa vozlisc, ki pripadada robovoma intervala [i, j]
      - pot levo/desno: pot od i_levo (oz. i_desno) do korena (indeksi vozlisc) od lista do korena (prvi element je i_levo/i_desno, zadnji pa 0)
      - i_skupni: indeks prvega skupnega prednika vozlisc i_levo in i_desno (če sta poti [23, 11, 5, 2, 0] in [25, 12, 5, 2, 0], je i_skupni = 5)
      - poti_pod_skupnim: če sta poti kot zgoraj, je to par ([23, 11], [25, 12]). */
// long i, long j sta emšota, torej vozlisca[nekaj].first.first, z najdi_list(i oz. j) iščemo ta nekaj
tuple<long long, long long, vector<long long>, vector<long long>, long long, pair<vector<long long>, vector<long long>>> Resitev::izracunaj_poti_itd(long long i, long long j) {
    auto i_levo = najdi_list(i);
    auto i_desno = najdi_list(j);
    auto pot_levo = pot_iz_lista(i_levo);
    auto pot_desno = pot_iz_lista(i_desno);
    auto [i_skupni, poti_pod_skupnim] = skupni_prednik_in_poti_pod_njim(pot_levo, pot_desno);
    return std::make_tuple(i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim);
}
tuple<long long, long long, vector<long long>, vector<long long>, long long, pair<vector<long long>, vector<long long>>> Resitev::izracunaj_poti_itd_2(long long i, long long j) {
    auto i_levo = najdi_list(i);
    auto i_desno = najdi_list(j) - 1;
    auto pot_levo = pot_iz_lista(i_levo);
    auto pot_desno = pot_iz_lista(i_desno);
    auto [i_skupni, poti_pod_skupnim] = skupni_prednik_in_poti_pod_njim(pot_levo, pot_desno);
    return std::make_tuple(i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim);
}
long long Resitev::izracunaj_starsa(long long i){
    return (i - 1) >> 1;
}
long long Resitev::izracunaj_levega(long long i){
    return (i << 1) + 1;
}
long long Resitev::izracunaj_desnega(long long i){
    return (i << 1) + 2;
}


void Resitev::povecaj(long long i, long long j, long x){

    auto [i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim] = izracunaj_poti_itd(i, j);
    auto notranja_vozlisca = notranji_koreni(poti_pod_skupnim.first, poti_pod_skupnim.second);

    // povecaj alfe
    if (i_levo > n_notranjih_vozlisc){
        vozlisca[i_levo - 1].second.first.second += x;
        posodobi_beta(i_levo - 1);
    } else if (i_levo == n_notranjih_vozlisc && i_levo != i_desno) {
        vozlisca[i_levo].second.first.second += 1;
        posodobi_beta(i_levo);
    }
    if (i_levo == i_desno){
        vozlisca[i_levo].second.first.second -= x;
        posodobi_beta(i_levo);
    }
    if (i_levo != i_desno && i_desno < n_vozlisc) {
        vozlisca[i_desno].second.first.second += (-x - i_desno + i_levo);
        posodobi_beta(i_desno);
    } else if (i_desno == n_vozlisc) {
        vozlisca[i_desno].second.first.second = 0;

    }
    for (auto const & koren : notranja_vozlisca){
        vozlisca[koren].second.first.second += 1;
        posodobi_beta(koren);
    }
}


long long Resitev::n_listov_poddrevo(long long i) {
    if (listi_poddrevesa[i] > 0){
        return listi_poddrevesa[i];
    }
    long long i_levo = i;
    long long i_desno = i;
    // pojdi levo
    while (je_notranje(i_levo)){
        i_levo = izracunaj_levega(i_levo);
    }
    // pojdi desno
    while (je_notranje(i_desno)){
        i_desno = izracunaj_desnega(i_desno);
    }
    listi_poddrevesa[i] = i_desno - i_levo + 1;
    return listi_poddrevesa[i];
}

int Resitev::odgovor_vzdolz_obeh_poti(long long i, long long j){
    auto [i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim] = izracunaj_poti_itd_2(i, j);
    int koncna_vrednost;
    int koncna_vrednost_abs;
    vector<long long> vrednost_levo_desno {0, 0};
    vector<long long> vrednost_levo_desno_abs {0, 0};
    // izracunaj vzdolz leve in desne poti
    if (i_levo == i_desno){
        koncna_vrednost = vozlisca[i_levo].second.first.second;
        koncna_vrednost_abs = vozlisca[i_levo].second.first.second;

        for(int x; x < pot_levo.size(); x++){
            koncna_vrednost += vozlisca[pot_levo[x]].second.first.second;
            koncna_vrednost_abs += vozlisca[pot_levo[x]].second.first.second;
        }
        //odgovor_vzdolz_ene_poti(0, pot_levo, vrednost_levo_desno, vrednost_levo_desno_abs, vozlisca_levo_desno, vozlisca_levo_desno_abs);
    /*} else if (izracunaj_starsa(i_levo) == izracunaj_starsa(i_desno)){
        koncna_vrednost = std::max(vozlisca[i_levo].second.first.first + vozlisca[i_levo].second.first.second, vozlisca[i_desno].second.first.first + vozlisca[i_desno].second.first.second);
        koncna_vrednost_abs = std::abs(std::min(vozlisca[i_levo].second.second + vozlisca[i_levo].second.first.second, vozlisca[i_desno].second.second + vozlisca[i_desno].second.first.second));
        koncna_vrednost = std::max(koncna_vrednost_abs, koncna_vrednost);*/
    } else{
        auto n = (int) pot_levo.size();
        for (int k = 0; k < n; k++){
            if (k == 0){
                vrednost_levo_desno[0] = vozlisca[i_levo].second.first.second;
                vrednost_levo_desno_abs[0] = vozlisca[i_desno].second.first.second;
                vrednost_levo_desno[1] = vozlisca[i_levo].second.first.second;
                vrednost_levo_desno_abs[1] = vozlisca[i_desno].second.first.second;
            }
            if(pot_levo[k] > i_skupni && k > 0){
                auto [koren_1, posodobi_z_notranjim_1] = posodobitev_z_notranjim(0, pot_levo, k, pot_levo[k]);
                if (posodobi_z_notranjim_1){
                    vrednost_levo_desno[0] = std::max(vozlisca[koren_1].second.first.first + vozlisca[koren_1].second.first.second, vrednost_levo_desno[0]);
                    vrednost_levo_desno_abs[0] = std::min(vozlisca[koren_1].second.second + vozlisca[koren_1].second.first.second, vrednost_levo_desno_abs[0]);
                } 
            }
            if(pot_desno[k] > i_skupni){
                auto [koren_2, posodobi_z_notranjim_2] = posodobitev_z_notranjim(1, pot_desno, k, pot_desno[k]);
                if (posodobi_z_notranjim_2){
                    vrednost_levo_desno[1] = std::max(vozlisca[koren_2].second.first.first + vozlisca[koren_2].second.first.second, vrednost_levo_desno[1]);
                    vrednost_levo_desno_abs[1] = std::min(vozlisca[koren_2].second.second + vozlisca[koren_2].second.first.second, vrednost_levo_desno_abs[1]);
                }
            }
            if(pot_levo[k] == i_skupni){
                koncna_vrednost = std::max(vrednost_levo_desno[0] + vozlisca[i_skupni].second.first.second, vrednost_levo_desno[1] + vozlisca[i_skupni].second.first.second);
                koncna_vrednost_abs = std::max(vrednost_levo_desno_abs[0] + vozlisca[i_skupni].second.first.second, vrednost_levo_desno_abs[1] + vozlisca[i_skupni].second.first.second);
            }
            if(pot_levo[k] < i_skupni){
            koncna_vrednost += vozlisca[pot_levo[k]].second.first.second;
            koncna_vrednost_abs += vozlisca[pot_levo[k]].second.first.second;
            }
                /*else {
                    vrednost_levo_desno[i] = vozlisca[pot[j]].second.first.first;
                    vrednost_levo_desno_abs[i] = vozlisca[pot[j]].second.second;
                }*/
        }
        koncna_vrednost_abs = std::abs(koncna_vrednost_abs),
        koncna_vrednost = std::max(koncna_vrednost, koncna_vrednost_abs);            


        /*
        odgovor_vzdolz_ene_poti(0, pot_levo, vrednost_levo_desno, vrednost_levo_desno_abs, i_skupni);
        odgovor_vzdolz_ene_poti(1, pot_desno, vrednost_levo_desno, vrednost_levo_desno_abs, i_skupni);
        koncna_vrednost = std::max(vrednost_levo_desno[0], vrednost_levo_desno[1]);
        koncna_vrednost_abs = std::abs(std::min(vrednost_levo_desno_abs[0], vrednost_levo_desno_abs[1]));*/

        }

    return std::abs(koncna_vrednost);
}

//

pair<long long, bool> Resitev::posodobitev_z_notranjim(int i_poti, vector<long long> & pot, long long j, long long vozlisce) {
    long long koren = -1;
    bool posodobi = false;
    if (j == 0){
        koren = vozlisce;
    } else if (i_poti == 0 && izracunaj_desnega(vozlisce) != pot[j - 1]){
        // notranje na levi poti
        koren = izracunaj_desnega(vozlisce);
        posodobi = true;
    } else if (i_poti == 1 && izracunaj_levega(vozlisce) != pot[j - 1]){
        // notranje na desni poti
        koren = izracunaj_levega(vozlisce);
        posodobi = true;
    }
    return std::make_pair(koren, posodobi);
}

pair<long long, pair<vector<long long>, vector<long long>>> Resitev::skupni_prednik_in_poti_pod_njim(vector<long long> & pot_levo, vector<long long> & pot_desno){
    auto poti_pod_skupnim = std::make_pair(vector<long long> {}, vector<long long> {});
    poti_pod_skupnim.first.push_back(pot_levo[0]);
    poti_pod_skupnim.second.push_back(pot_desno[0]);
    long long i_skupni = -1;
    auto dolzina = (int) pot_levo.size();
    for (long long i = 0; i < dolzina; i++){
        long long levo = pot_levo[i];
        long long desno = pot_desno[i];
        if (levo == desno){
            i_skupni = i;
            break;
        }
    }
    for(long long i = 1; i < i_skupni; i++){
        poti_pod_skupnim.first.push_back(pot_levo[i]);
        poti_pod_skupnim.second.push_back(pot_desno[i]);
    }
    return std::make_pair(pot_levo[i_skupni], poti_pod_skupnim);
}

vector<long long> Resitev::notranji_koreni(vector<long long> & pot_levo, vector<long long> & pot_desno){
    vector<long long> internal;
    auto dolzina = (int) pot_levo.size();
    for (int i = 1; i < dolzina; i++){
        long long levo = pot_levo[i];
        long long desno = pot_desno[i];
        if (izracunaj_levega(levo) == pot_levo[i - 1]){
            internal.push_back(izracunaj_desnega(levo));
        }
        if (izracunaj_desnega(desno) == pot_desno[i - 1]){
            internal.push_back(izracunaj_levega(desno));
        }
    }
    return internal;
}

pair<long long, long long> Resitev::izracunaj_velikost_polnega_drevesa(long long n){
    long long globina = 1;
    long long listi = 1;
    while (listi < n){
        listi = listi << 1;
        globina += 1;
    }
    long long vsa = (1 << globina) - 1;
    long long n_notranjih_vozlisc = vsa - (1 << (globina - 1));
    return std::make_pair(vsa, n_notranjih_vozlisc);
}



void Resitev::posodobi_beta(long long i){
    //long l_beta = vozlisca[izracunaj_levega(i)].first.second;
    //long d_beta = vozlisca[izracunaj_desnega(i)].first.second;
    //long alfa = vozlisca[i].first.first;
    //vozlisca[i].first.second = l_beta + d_beta + alfa * n_listov_poddrevo(i);
    while (i > 0){
        long long stars = izracunaj_starsa(i);
        long long alfa_zac = vozlisca[stars].second.first.first;
        //long beta_zac = vozlisca[stars].second.first.second;
        long long alfa_zac_abs = vozlisca[stars].second.second;

        long long levi = izracunaj_levega(stars);
        long long desni = izracunaj_desnega(stars);
        long long alfa = std::max(vozlisca[levi].second.first.first + vozlisca[levi].second.first.second, vozlisca[desni].second.first.first + vozlisca[desni].second.first.second);
        vozlisca[stars].second.first.first = alfa;
        long long alfa_abs = std::min(vozlisca[levi].second.second + vozlisca[levi].second.first.second, vozlisca[desni].second.second + vozlisca[desni].second.first.second);
        vozlisca[stars].second.second = alfa_abs;
        if(alfa_zac == alfa && alfa_zac_abs == alfa_abs){
            break;                
        }
        i = stars;
    }
}

