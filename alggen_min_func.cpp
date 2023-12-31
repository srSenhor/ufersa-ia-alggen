#include <iostream>
#include <cmath>
#include <bitset> //Sugestão para visualizar binário
#include <random>


using namespace std;

 //Setando constantes
const int NUM_PARES = 8; //Número de individuos. FIXME: Por ora funciona apenas com pares de pares
const int MAX_ITERACOES = 100;
const float TAXA_MUTACAO = 0.05f;


//Função usada: raizquadrada(x^3 + 2*(y^4))
inline double f(int x, int y){ return sqrt(pow(x, 3) + 2 * pow(y, 4)); }

int seletorPais(double *, random_device&);
void crossover(bitset<6>*, random_device&);
void mutacao(bitset<6>&, random_device&);
bitset<6> converteBitset(int, int);

//O algoritmo genético binário deve minimizar f(x) com x e y sendo valores no intervalo [0, 7]
//Serão 3 bits para cada entrada.
int main(){

    //Setando seed para criação de números aleatórios
    random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 7);
    std::uniform_int_distribution<int> roleta(0, 360);

    //Inicializando a população
    int pop[NUM_PARES][2]{};

    for (int i = 0; i < NUM_PARES; i++)
        for (int j = 0; j < 2; j++)
            pop[i][j] = dist(mt);
    
    //Avaliando os indivíduos
    double aval[NUM_PARES]{};
    double total_aval = 0.0;
    double melhor_aval = 100;   //melhor avaliação da iteração
    double melhor[2]{}; // melhor individuo

    for (int i = 0; i < NUM_PARES; i++){
        aval[i] = 1 + f(pop[i][0], pop[i][1]);  //Previnindo que a função de avaliação retorne valores negativos ou nulos, como no exemplo da sala
        if(aval[i] < melhor_aval){
            melhor_aval = aval[i];
            melhor[0] = pop[i][0];
            melhor[1] = pop[i][1];
        }
        total_aval += 1 / aval[i]; //Invertendo o valor já que o objetivo é minimizar a função
    }

    //Repartindo os pedaços da roleta
    double intervalos_roleta[NUM_PARES]{};
    for (int i = 0; i < NUM_PARES; i++){
        intervalos_roleta[i] = ((1.0 / aval[i]) / (total_aval)) *  360;  
        intervalos_roleta[i] = round(intervalos_roleta[i]);     //FIXME: Em alguns casos, os pedaços somam 359. Buscar uma forma de contornar o problema.
    }

    int i = 0;

    //Visualizando valores
    cout << "Populacao de individuos" << endl;
    for (int i = 0; i < NUM_PARES; i++)
        cout << "X: " << pop[i][0] << ", Y: " << pop[i][1] << endl;
    cout << endl;

    cout << "Avaliacao dos individuos sob f(x,y)" << endl;
    for (int i = 0; i < NUM_PARES; i++)
        cout << i + 1 << ":" << aval[i] << endl;
    cout << "Total: " << total_aval << endl << endl;

    cout << "Melhor individuo da geracao " << i << ": x = " << melhor[0] << ", y =" << melhor[1] << endl;
    cout << "------------------------------------------------------" << endl;
    do{
        //Selecionando pais
        int selecionados[NUM_PARES]{};
        for (int i = 0; i < NUM_PARES; i++){
            selecionados[i] = seletorPais(intervalos_roleta, rd);  
        }
        
        //Convertendo para bitset
        bitset<6> pais[NUM_PARES]{};

        for (int i = 0; i < NUM_PARES; i++){
            pais[i] = converteBitset(pop[selecionados[i]][0], pop[selecionados[i]][1]);
        }
        
        //Realizando o crossover
        crossover(pais, rd);

        //Reavaliando os indivíduos
        //TODO: Testar se isso aqui tá funcional
        total_aval = 0.0;

        for (int i = 0; i < NUM_PARES; i++){
            bitset<6> aux_x = (pais[i] >>= 3);
            bitset<6> aux_y = (pais[i] &= bitset<6>("000111"));
            
            pop[i][0] = aux_x.to_ulong();
            pop[i][1] = aux_y.to_ulong();
        }


        for (int i = 0; i < NUM_PARES; i++){
            aval[i] = 1 + f(pop[i][0], pop[i][1]);
            if(aval[i] < melhor_aval){
                melhor_aval = aval[i];
                melhor[0] = pop[i][0];
                melhor[1] = pop[i][1];
            }
            total_aval += 1 / aval[i]; 
        }

        //Repartindo os pedaços da roleta de novo
        for (int i = 0; i < NUM_PARES; i++){
            intervalos_roleta[i] = ((1.0 / aval[i]) / (total_aval)) *  360;  
            intervalos_roleta[i] = round(intervalos_roleta[i]);     //FIXME: Em alguns casos, os pedaços somam 359. Buscar uma forma de contornar o problema.
        }
        i++;

        //Visualizando valores
        cout << "Populacao de individuos" << endl;
        for (int i = 0; i < NUM_PARES; i++)
            cout << "X: " << pop[i][0] << ", Y: " << pop[i][1] << endl;
        cout << endl;

        cout << "Avaliacao dos individuos sob f(x,y)" << endl;
        for (int i = 0; i < NUM_PARES; i++)
            cout << i + 1 << ":" << aval[i] << endl;
        cout << "Total: " << total_aval << endl << endl;


        cout << "Melhor individuo da geracao " << i << ": x = " << melhor[0] << ", y =" << melhor[1] << endl;
        cout << "------------------------------------------------------" << endl;
    } while(melhor_aval > 1.0 && i < MAX_ITERACOES);
    
    //TODO: Procurar uma biblioteca para plotar o grafico em cpp. Sugerido o "matplotlib for cpp".  
    //TODO: Limpar e organizar o código.


 

    return 0;
}

int seletorPais(double * intervalos, random_device& rd){
    
    mt19937 mt(rd());
    uniform_int_distribution<int> roleta(0, 360);

    double limSup = intervalos[0];
    int num = roleta(mt);

    for (int i = 0; i < NUM_PARES; i++){
        if(num <= limSup)
            return i;
        else 
            limSup += intervalos[i + 1];
    }
    
    return -1;
}

bitset<6> converteBitset(int x, int y){
    bitset<6> binx(x), biny(y), ret(0);

    ret |= binx;
    ret <<= 3;
    ret |= biny;

    return ret;
}

void crossover(bitset<6> * pais, random_device& rd){
    
    mt19937 mt(rd());
    uniform_int_distribution<int> bitAlterado(0, 5);

    int num;

    for (int i = 0; i < NUM_PARES; i += 2){
        num = bitAlterado(mt);

        bitset<6> m1[2]{(0), (0)};   //Máscaras do primeiro pai
        bitset<6> m2[2]{(0), (0)};   //Máscaras do segundo pai

        switch (num){
        case 0:
                m1[0] |= 32, m1[1] |= 31;
                m2[0] |= 32, m2[1] |= 31;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        case 1:
                m1[0] |= 48, m1[1] |= 15;
                m2[0] |= 48, m2[1] |= 15;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        case 2:
                m1[0] |= 56, m1[1] |= 7;
                m2[0] |= 56, m2[1] |= 7;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        case 3:
                m1[0] |= 60, m1[1] |= 3;
                m2[0] |= 60, m2[1] |= 3;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        case 4:
                m1[0] |= 62, m1[1] |= 1;
                m2[0] |= 62, m2[1] |= 1;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        case 5:
                m1[0] |= 63, m1[1] |= 0;
                m2[0] |= 63, m2[1] |= 0;
                m1[0] &= pais[i];
                m1[1] &= pais[i];
                m2[0] &= pais[i + 1];
                m2[1] &= pais[i + 1];
                
                pais[i] = (m1[0] |= m2[1]);
                pais[i + 1] = (m2[0] |= m1[1]);
                mutacao(pais[i], rd);
                mutacao(pais[i + 1], rd);
            break;
        default:
            cout << "Algo deu errado no crossover..." << endl;
            break;
        }
        
    }
}

void mutacao(bitset<6> & individuo, random_device& rd){
    
    mt19937 mt(rd());
    uniform_int_distribution<int> mutar(0, 99);

    if(mutar(mt) < (TAXA_MUTACAO * 100)){
        uniform_int_distribution<int> bitAlterado(0, 5);
        int num = bitAlterado(mt);
        individuo.flip(num);    //Inverte o bit na posição escolhida, da direita pra esquerda
    }
}
