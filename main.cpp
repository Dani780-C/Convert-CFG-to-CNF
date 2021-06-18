#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
ifstream f("citire_chomsky.in");

unordered_map<string,vector<string>> multime;
vector<string> terminale_useless;
int index_x = 0;
int index_y = 0;

void citire(){
    string linie;
    while(getline(f,linie)){  /// se citesc linii sub forma "neterminal(litera mare) -> "productie1" | ...
        string neterminal = "";
        int i = 0;
        while(linie[i] != ' '){
            neterminal += linie[i++];
        }
        i += 4;
        while(i<linie.length()){
            string productie = "";
            while(linie[i] != ' ' and linie[i] != '\0') productie += linie[i++];
            if(productie != neterminal) multime[neterminal].push_back(productie);
            i+=3;
        }
    }
}

bool inaccesibil(string neterminal){
    unordered_map<string,vector<string>>::iterator i, j;
    for(i = multime.begin(); i != multime.end(); i++){
        for(j = multime.begin(); j != multime.end(); j++)
            if(i->first != j->first){
                for(int k = 0; k < (j->second).size(); k++){
                    string productie = (j->second)[k];
                    int l = 0;
                    while(l < productie.size() and productie[l] != neterminal[0]) l++;
                    if(l < productie.size()) return false;
                }
            }
    }
    return true;
}

bool neutilizabil(string neterminal){
    int nr_alte_caractere = 0, suma_lungimi_productii = 0;
    for(int j = 0; j < multime[neterminal].size(); j++){
        string productie = multime[neterminal][j];
        int k = 0;
        suma_lungimi_productii += productie.size();
        while(k < productie.size()){
            if(productie[k] >= 'A' and productie[k] <= 'Z'){
                if(productie[k] != neterminal[0]) return false;
            }
            else nr_alte_caractere++;
            k++;
        }
    }
    if(suma_lungimi_productii == nr_alte_caractere) return false;
    return true;
}

void sterge(string neterminal){
    unordered_map<string,vector<string>>::iterator j;
    vector<string> productii_ramase;
    for(j = multime.begin(); j != multime.end(); j++){
        productii_ramase.clear();
        for(int k = 0; k < (j->second).size(); k++){
            string productie = (j->second)[k];
            int idx = 0;
            while(idx < productie.size() and productie[idx] != neterminal[0]) idx++;
            if(idx == productie.size()) productii_ramase.push_back(productie);
        }
        if(productii_ramase.size() != (j->second).size()){
            (j->second).clear();
            for(int idx = 0; idx < productii_ramase.size(); idx++){
                (j->second).push_back(productii_ramase[idx]);
            }
        }
    }
}

void pas1(){       /// stergem neterminalele neutilizabile si inaccesibile
    unordered_map<string,vector<string>>::iterator i;
    for(i = multime.begin(); i != multime.end(); i++){
        string neterminal = (i->first);
        if(neterminal != "S" and inaccesibil(neterminal) == true) terminale_useless.push_back(neterminal);
        if(neterminal != "S" and neutilizabil(neterminal) == true) terminale_useless.push_back(neterminal);
    }
    for(int j = 0; j < terminale_useless.size(); j++){
        multime.erase(terminale_useless[j]); /// stergem terminalele si productiile lor din multimea de terminale -> productii
        sterge(terminale_useless[j]); /// functie care sterge din toate productiile terminalele care sunt neutilizabile
    }
}

void sterge_neterminal(string neterminal){
    unordered_map<string,vector<string>>::iterator j;
    vector<string> productii_ramase;
    for(j = multime.begin(); j != multime.end(); j++){
        productii_ramase.clear();
        for(int k = 0; k < (j->second).size(); k++){
            string productie = (j->second)[k];
            if(productie == neterminal){
                productii_ramase.push_back("#");
            }
            else{
                string productie_noua = "";
                int idx = 0;
                while(idx < productie.size()){
                    if(productie[idx] != neterminal[0]){
                        productie_noua += productie[idx];
                    }
                    idx++;
                }
                productii_ramase.push_back(productie_noua);
            }
        }
        (j->second).clear();
        for(int l = 0; l < productii_ramase.size(); l++){
            (j->second).push_back(productii_ramase[l]);
        }
    }
}

bool verifica_daca_apare(string neterminal, string productie){
    for(int i = 0; i < multime[neterminal].size(); i++){
        if(multime[neterminal][i] == productie) return false;
    }
    return true;
}

void sterge_si_compune(string neterminal){
    unordered_map<string,vector<string>>::iterator j;
    for(j = multime.begin(); j != multime.end(); j++){
        for(int k = 0; k < (j->second).size(); k++){
            string productie = (j->second)[k];
            if(productie == neterminal) (j->second)[k] = "#";
            else{
                int idx = 0;
                string cuv1 = "";
                string cuv2 = "";
                while(idx < productie.size()){
                    if(productie[idx] != neterminal[0]) cuv1 += productie[idx], cuv2 += productie[idx];
                    else{
                        int idx_1 = idx + 1;
                        string cuv3 = "";
                        cuv2 += productie[idx];
                        while(idx_1 < productie.size()) cuv3 += productie[idx_1++];
                        if(verifica_daca_apare(j->first, cuv1 + cuv3) == true and (cuv1 + cuv3) != j->first) (j->second).push_back(cuv1 + cuv3);
                        cuv1 = cuv2;
                    }
                    idx++;
                }
            }
        }
    }
}

void pas2(){
    unordered_map<string,vector<string>>::iterator i;
    vector<string> trebuie_sterse;
    vector<string> productii_ramase;
    bool ok = true;
    while(ok == true){
        ok = false;
        for( i = multime.begin(); i != multime.end(); i++){///verificam daca are lambda
            productii_ramase.clear();;
            int j = 0;
            while(j < (i->second).size() and (i->second)[j] != "#") j++;
            if(j < (i->second).size()){ /// are lambda-productie
                if(i->first != "S"){
                    ok = true;
                    string neterminal = i->first;
                    if((i->second).size() == 1){ /// productie de genul: N -> lambda
                        trebuie_sterse.push_back(i->first);
                        sterge_neterminal(neterminal);
                    }
                    else{
                        sterge_si_compune(neterminal);
                        int k = 0;
                        while(k < (i->second).size() and (i->second)[k] != "#") productii_ramase.push_back((i->second)[k++]);
                        k++;
                        while(k < (i->second).size()) productii_ramase.push_back((i->second)[k++]);
                        (i->second).clear();
                        for(int idx = 0; idx < productii_ramase.size(); idx++){
                            (i->second).push_back(productii_ramase[idx]);
                        }
                    }
                }
            }
        }
        for(int j = 0; j < trebuie_sterse.size(); j++){
            multime[trebuie_sterse[j]].clear();
            multime.erase(trebuie_sterse[j]);
        }
    }
    int j = 0;
    productii_ramase.clear();
    while(j < multime["S"].size() and multime["S"][j] != "#") productii_ramase.push_back(multime["S"][j++]);
    j++;
    while(j < multime["S"].size() and multime["S"][j] != "#") productii_ramase.push_back(multime["S"][j++]);
    if(productii_ramase.size() < multime["S"].size()){
        multime["S"].clear();
        for(int j = 0; j < productii_ramase.size(); j++) multime["S"].push_back(productii_ramase[j]);
        multime["S'"] = {"S", "#"};
    }
}

void elimina_redenumirile(string neterminal){
    vector<string> productii_ramase;
    for(int i = 0; i < multime[neterminal].size(); i++){
        string productie = multime[neterminal][i];
        if(productie.size() == 1 and productie[0] >= 'A' and productie[0] <= 'Z'){
            for(int l = 0; l < multime[productie].size(); l++){
                if(multime[productie][l] != neterminal) productii_ramase.push_back(multime[productie][l]);
            }
        }
        else productii_ramase.push_back(multime[neterminal][i]);
    }
    multime[neterminal].clear();
    for(int i = 0; i < productii_ramase.size(); i++){
        if(verifica_daca_apare(neterminal,productii_ramase[i])) multime[neterminal].push_back(productii_ramase[i]);
    }
    for(int i = 0; i < multime[neterminal].size(); i++){
        if(multime[neterminal][i].size() == 1 and multime[neterminal][i][0] >= 'A' and multime[neterminal][i][0] <= 'Z')
            elimina_redenumirile(neterminal);
    }
}

bool egale(string neterminal_1, string neterminal_2){
    for(int j = 0; j < multime[neterminal_1].size(); j++){
        int k = 0;
        while(k < multime[neterminal_2].size() and multime[neterminal_2][k] != multime[neterminal_1][j]) k++;
        if(k == multime[neterminal_2].size()) return false;
    }
    return true;
}

void schimba(string neterminal_1, string neterminal_2){
    vector<string> productii;
    unordered_map<string,vector<string>>::iterator nt;
    for(nt = multime.begin(); nt != multime.end(); nt++){
        productii.clear();
        for(int i = 0; i < (nt->second).size(); i++){
            string productie = (nt->second)[i];
            string productie_noua = "";
            for(int j = 0; j < productie.size(); j++){
                if(productie[j] == neterminal_2[0]){
                    productie_noua += neterminal_1[0];
                }else{
                    productie_noua += productie[j];
                }
            }
            productii.push_back(productie_noua);
        }
        (nt->second).clear();
        for(int i = 0; i < productii.size(); i++){
            (nt->second).push_back(productii[i]);
        }
    }
}

void elimina_egalitatile(){
    unordered_map<string,vector<string>>::iterator i,j;
    vector<string> trebuie_sterse;
    for(i = multime.begin(); i != multime.end(); i++){
        for(j = multime.begin(); j != multime.end(); j++)
            if(i->first != j->first and (i->second).size() == (j->second).size()){
                if(egale(i->first,j->first) == true){
                    if(i->first == "S") schimba("S",j->first), trebuie_sterse.push_back(j->first);
                    else if(j->first == "S") schimba("S",i->first), trebuie_sterse.push_back(i->first);
                        else schimba(i->first,j->first), trebuie_sterse.push_back(j->first);
                }
            }
    }
    for(int k = 0; k < trebuie_sterse.size(); k++){
        multime.erase(trebuie_sterse[k]);
    }
}

void pas3(){
    unordered_map<string,vector<string>>::iterator i;
    unordered_map<string,vector<string>> copie_multime = multime;
    for(i = copie_multime.begin(); i != copie_multime.end(); i++){
        string neterminal = i->first;
        if(i->first != "S'") elimina_redenumirile(neterminal);
    }
    elimina_egalitatile();
    if(multime.find("S'") != multime.end()) elimina_redenumirile("S'");
}

void pas5(){
    unordered_map<string,vector<string>>::iterator i;
    vector<string>  productii;
    for(i = multime.begin(); i != multime.end(); i++){
        productii.clear();
        for(int j = 0; j < (i->second).size(); j++){
            string productie = (i->second)[j];
            if(productie.size() > 1){
                string productie_noua = "";
                for(int k = 0; k < productie.size(); k++){
                    if(productie[k] >= 'a' and productie[k] <= 'z'){
                        if(multime.find(to_string(productie[k])) != multime.end()){
                            productie_noua += multime[to_string(productie[k])][0];
                        }
                        else{
                            index_x++;
                            multime[to_string(productie[k])].push_back("(X" + to_string(index_x) + ")");
                            productie_noua += multime[to_string(productie[k])][0];
                        }
                    }
                    else{
                        productie_noua += productie[k];
                    }
                }
                productii.push_back(productie_noua);
            }
            else{
                productii.push_back(productie);
            }
        }
        (i->second).clear();
        for(int idx = 0; idx < productii.size(); idx++) (i->second).push_back(productii[idx]);
    }
}

void pas6(){
    bool ok = false;
    unordered_map<string,vector<string>>::iterator i;
    for(i = multime.begin(); i != multime.end(); i++){
        string neterminal  = i->first;
        for(int j = 0; j < multime[neterminal].size(); j++){
            string productie = multime[neterminal][j];   ///descompunem productia in neterminale
            string neterminal_X = "";
            vector<string> neterminale;
            for(int l = 0; l < productie.size(); l++){
                neterminal_X += productie[l];
                if(neterminal_X[0] != '('){
                    neterminale.push_back(neterminal_X);
                    neterminal_X = "";
                }
                if(productie[l] == ')'){
                    neterminale.push_back(neterminal_X);
                    neterminal_X = "";
                }
            }
            if(neterminale.size() > 2){ /// trebuie facute alte neterminale
                string primul_neterminal = neterminale[0];
                string productie_noua = "";
                for(int l = 1; l < neterminale.size(); l++) productie_noua += neterminale[l];
                index_y++;
                multime.insert({"(Y" + to_string(index_y) + ")", {productie_noua}});
                string productie_veche = primul_neterminal + "(Y" + to_string(index_y) + ")";
                multime[neterminal][j] = productie_veche;
                ok = true;
            }
            neterminale.clear();
        }
    }
    if(ok == true) pas6();
}

void transformare(){
    pas1(); /// scapam de neterminalele neuitlizabile si inaccesibile
    pas2(); /// scapam de lambda-productii (in cod, lambda = #)
    pas3(); /// se elimina redenumirile ( N1 -> N2 | ...)
    pas1(); /// pas4() /// se aplica din nou algoritmul de reducere - pasul 1
    pas5(); /// inlocuim terminalele din cuvintele de lungime mai mare de 1 cu neterminale
    pas6(); /// final Chomsky
}

void afisare(){
    unordered_map<string,vector<string>>::iterator i;
    for(i = multime.begin(); i != multime.end(); i++){
        if((i->first)[0] >= '0' and (i->first)[0] <= '9'){
            int k = 0, nr = 0;
            while(k < (i->first).size()) nr = nr * 10 + ((i->first)[k++] - '0');
            cout<<(i->second)[0]<<" -> "<<char(nr)<<'\n';
        }
        else{
            cout<<i->first<<" -> ";
            for(int k = 0; k < (i->second).size(); k++){
                cout<<(i->second)[k]<<" ";
            }
            cout<<'\n';
        }
    }
}

int main()
{
    citire();   /// functia de citire din fisier pentru CFG
    transformare(); /// procesul de transformare din CFG in CNF
    afisare();  /// afisarea CNF
    return 0;
}
