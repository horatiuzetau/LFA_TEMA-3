#include <iostream>
#include "cstring"
#include "vector"
#include "fstream"
#include "stack"
#include "queue"

using namespace std;

string cuv = "baaaa∉";


//reg = regula
//et  = eticheta(adica neterminal)
//prod= vectorul in caer se afla productiile

struct reg{
    char et;
    vector<string> prod;
};


vector<reg> citireFisier(const string& locatie){
    //deschidem fisierul
    ifstream f;
    f.open(locatie);

    vector<reg> reguli; //vectorul care va fi transmis in programul principal
    string str;
    char *cstr = nullptr;//folosit la tokenuri si el pentru ca pe string nu se poate face split
    char * p;//folosit la split in tokenuri
    reg aux;//folosit ca placeholder pentr informatia fiecarei productii

    while(getline(f, str)){

        //CA SA PUTEM SA FOLOSIM STRTOK AM CONVERTIT STRING IN CHAR
        cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        p  = strtok(cstr, "- ");
        //PUNEM NETERMINALUL CA ETICHETA IN AUX
        aux.et = *p;

        //PUNEM PRODUCTIILE DE LA ACEL NETERMINAL IN AUX
        while( p != nullptr) {
            p = strtok(nullptr, " ");
            if(p) {
                aux.prod.emplace_back(p);
            }
        }

        //PUNEM AUX IN VECTOR
        reguli.push_back(aux);
        //GOLIM PRODUCTIILE DE LA ULTIMA PRODUCTIE PENTRU A REUSI
        //SA ADAUGAM ALTELE NOI
        aux.prod.clear();
    }

    delete[] cstr;
    f.close(); // inchidem fisierul

    return reguli;
}

void afisareGramatica(const vector<reg>& a){
    for (auto &i : a) {
        cout << i.et << "->";
        for(auto &j : i.prod){
            cout << j << " ";
        }
        cout << endl;
    }
}

int gasesteIndex(const vector<reg>& a, const char& x){
    for(int i = 0; i < a.size(); i++){
        if(a[i].et == x){
            return i;
        }
    }

    return -1;
}

bool verifica(const string& a, const string& b){
    //b = cuvantul nostru
    //a = cuvantul verificat
    if(a.size() > b.size() + 1){
        return false;
    }

    for(int k = 0; k < b.size(); k++){
        if( a[k] != b[k] ) {
            return isupper(a[k]) != 0;
        }
    }

    return true;
}

bool exista(const vector<reg>& gram, const char& start){
    int index;
    index = gasesteIndex(gram, start);
    queue<string> stiva;
    string a, auxstr;
    int iaux;
    bool gasit = false;

    if(index != -1) {
        //bagam prima data astea in stiva
        for (const auto &i : gram[index].prod) {
            stiva.push(i);
        }


        while(!stiva.empty()){
            a = stiva.front();
            stiva.pop();

            for(int i = 0; i < a.size(); i++){ //for pentru toate literele din cuvantul actual
                if(isupper(a[i])){//daca se gaseste litera mare (neterminal)
                    gasit = true;
                    iaux = gasesteIndex(gram, a[i]); //extrage randul cu acest neterminal

                    for(int j = 0; j < gram[iaux].prod.size(); j++){ //pentru fiecare productie din acesta inlocuim in actualul nostru
                        auxstr = a; //folosim un string aux ca sa nu schimbam valoarea
                        if(gram[iaux].prod[j] == "√") //daca este LAMBDA atunci dispare NETERMINALUL
                            auxstr.replace(i, 1, "");
                        else                            //daca nu este lambda se inlocuieste cu productia
                            auxstr.replace(i, 1, gram[iaux].prod[j]);

                        cout << a << " : "; // se arata de unde se pleaca

                        if(verifica(auxstr, cuv)){  // si unde se ajugne
                            cout << auxstr << " ";
                            stiva.push(auxstr); // acestea se baga ins tiva ca mai apoi sa lucram cu ele cum am procedat panacum
                        }
                        if(auxstr == cuv){ //daca am gasit cuvantul ne intoarcem
                            return true;
                        }
                        cout << endl;

                    }
                    cout << endl;
                }
            }
        }
    }
    return false; //daca nu gasim cuvantul return false
}

int main() {

    char start = 'S';
    string target = "C:\\Work\\Real work\\Tema3\\date.in";

    vector<reg> gram = citireFisier(target); // gramatica noastra
    afisareGramatica(gram);
    cout << endl;

    if(exista(gram, start))
        cout << "\nMerge";
    else
        cout << "\nNu merge";





    return 0;
}