#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm> // za transform

#include "resitev.h"

#include <stdio.h>


using std::vector;
using std::string;
using std::cout;
using std::endl;


vector<string> preberiVrstice(const string & pot){
    vector<string> vrstice;

    std::fstream datoteka;
    datoteka.open(pot, std::ios::in);
    if (datoteka.is_open()){
        string vrstica;
        while(getline(datoteka, vrstica)){
            vrstice.push_back(vrstica);
        }
        datoteka.close();
    } else {
        cout << "Nisem mogel prebrati datoteke " << pot << "\nLep pozdrav, tvoj c++" << endl;
    }
    return vrstice;
}


void preveri(const string & naloga, long long maxTrajanjeMS){
    cout << "Preverjam " << naloga << endl; 

    auto ukazi = preberiVrstice(naloga + ".in");
    auto emsoOtrokSTR = preberiVrstice(naloga + ".vpisna");
    vector<long long> emsoOtrok(emsoOtrokSTR.size());
    std::transform(emsoOtrokSTR.begin(), emsoOtrokSTR.end(), emsoOtrok.begin(), [](string x){return std::stoll(x); });
    auto resitve = preberiVrstice(naloga + ".out");
    vector<long long> odgovori(resitve.size());

    auto t0 = std::chrono::high_resolution_clock::now();
    int iPoizvedba = 0;
    Resitev R {emsoOtrok};
    //Slaba R {emsoOtrok};

    for (const auto & ukaz : ukazi){
        if (ukaz[0] == 'o'){
            odgovori[iPoizvedba] = R.obdelaj(ukaz);
            iPoizvedba++;
        } else{
            R.obdelaj(ukaz);
        }
        // R.toString();
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    // milisekunde
    auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
    int napaka = -1;
    int nNapak = 0;
    for (int i = 0; i < resitve.size(); i++){
        if (std::stoll(resitve[i]) != odgovori[i]){
            if (nNapak == 0){
                // najdi poizvedbo z danim indeksom
                iPoizvedba = 0;
                string poizvedba;
                for (const auto & ukaz : ukazi){
                    if (ukaz[0] == 'o'){
                        if (iPoizvedba == i){
                            poizvedba = ukaz;
                            break;
                        }
                        iPoizvedba++;
                    }
                }
                cout << "Prva napaka se zgodi pri poizvedbi st. " << i << " (>= 0): " << poizvedba << "."
                     << " Pravilen odgovor = " << resitve[i] << " != " << odgovori[i] << endl;
            }
            nNapak++;
        }
    }
    if (nNapak == 0){
        cout << "Vsi odgovori so pravilni." << endl;
    } else{
        cout << "Skupno stevilo napak: " << nNapak << endl;
    }
    string jeNi = trajanje <= maxTrajanjeMS ? "je" : "ni";
    cout << "Resitev " << jeNi << " dovolj hitra: " << trajanje << " ms (dovoljeno: " << maxTrajanjeMS << " ms)" << endl;
}


long long izracunajMaxT(){
    auto t0 = std::chrono::high_resolution_clock::now();
    int vsota = 0;
    for (int i = 0; i < 25300; i++){
        for (int j = i; j >= -i; j--){
            vsota += (i + j) / 2;
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
    return trajanje;
}

int main(){
    long long maxT = izracunajMaxT();
    vector<string> naloge = {
        "nalogaA0", "nalogaA1", "nalogaA2",  
        "nalogaB0", "nalogaB1", "nalogaB2" 
    };
       
    for (auto const & naloga : naloge){
        preveri(naloga, maxT);
        cout << endl;
    }
    return 0;
}