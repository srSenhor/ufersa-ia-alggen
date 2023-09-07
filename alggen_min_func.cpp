#include <iostream>;
#include <string>;
#include <cmath>;
#include <bitset> //Sugestão para visualizar binário
#include <random>;

using namespace std;

//Função usada: raizquadrada(x^3 + 2*(y^4))
inline double f(int x, int y){ return sqrt(pow(x, 3) + 2 * pow(y, 4)); }


//O algoritmo genético binário deve minimizar f(x) com x e y sendo valores no intervalo [0, 7]
//Serão 3 bits para cada entrada.
int main(){

    //Setando constantes
    const int NUM_INDIVIDUOS = 16;
    const float TAXA_MUTACAO = 0.05f;
    const float PRECISAO = 1.0f;

    //Setando seed para criação de números aleatórios
    random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 7);

    //Inicializando a população
    int pop[NUM_INDIVIDUOS / 2][2]{};

    for (int i = 0; i < NUM_INDIVIDUOS; i++)
        for (int j = 0; j < 2; j++)
            pop[i][j] = dist(mt);
    
    //Avaliando os indivíduos
    double aval[NUM_INDIVIDUOS / 2]{};
    double total_aval = 0.0;

    for (int i = 0; i < NUM_INDIVIDUOS / 2; i++){
        aval[i] = 1 + f(pop[i][0], pop[i][1]);  //Previnindo que a função de avaliação retorne valores negativos ou nulos, como no exemplo da sala
        total_aval += 1 / aval[i]; //Invertendo o valor já que o objetivo é minimizar a função
    }

    //Repartindo os pedaços da roleta
    double intervalos_roleta[NUM_INDIVIDUOS / 2]{};
    for (int i = 0; i < NUM_INDIVIDUOS / 2; i++){
        intervalos_roleta[i] = ((1.0 / aval[i]) / (total_aval)) *  360;  
        intervalos_roleta[i] = round(intervalos_roleta[i]);     //FIXME: Em alguns casos, os pedaços somam 359. Buscar uma forma de contornar o problema.
    }
    
    //TODO: A partir daqui, encapsule em um laço que realizará os passos seguintes
    //TODO: Escrever o código do crossover dos pais selecionados aleatoriamente com base na roleta
    //TODO: Escrever o código da mutação dos filhos. Crie um novo "gerador" randomico de 0 a 360
    //TODO: Apague os membros da velha geração, mas não sei se são os pais selecionados ou o quê. Método ainda por definir.
    //TODO: Escrever o código de reavaliação dos indivíduos. Considerar componentizar o código de avaliação já feito.



    //Visualizando valores
    cout << "Populacao de individuos" << endl;
    for (int i = 0; i < NUM_INDIVIDUOS / 2; i++)
        cout << "X: " << pop[i][0] << ", Y:" << pop[i][1] << endl;
    cout << endl;

    cout << "Avaliacao dos individuos sob f(x,y)" << endl;
    for (int i = 0; i < NUM_INDIVIDUOS / 2; i++)
        cout << i + 1 << ":" << aval[i] << endl;
    cout << "Total: " << total_aval << endl << endl;

    cout << "Parte da roleta (inicia em 0)" << endl;
    for (int i = 0; i < NUM_INDIVIDUOS / 2; i++)
        cout << i + 1 << ":" << intervalos_roleta[i] <<  " pedacos posteriores" << endl;
    cout << "Total: " << total_aval << endl << endl;

    
    
    

    // Alguns testes para a visualização do binário
    /*
    int numero = 31;
    bitset<32> binario(numero);

    cout << "31: " << binario << endl;
    */
    /*
    string binarioString = "11111";
    cout << "Valor decimal:" << stoi(binarioString, nullptr, 2) << endl;
    */

    return 0;
}