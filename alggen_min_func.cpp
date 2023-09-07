#include <iostream>;
#include <cmath>;
#include <random>;

using namespace std;

//Função usada: raizquadrada(x^3 + 2*(y^4))
inline double f(double x, double y){ return sqrt(pow(x, 3.0) + 2 * pow(y, 4.0)); }


//O algoritmo genético binário deve minimizar f(x) com x e y sendo valores no intervalo [0, 7]
//Serão 3 bits para cada entrada.
int main(){
    //TODO: Definir qual função de geração de algoritmos aleatórios (provavelmente o mersennetwister)
    //TODO: Definir o número de indivíduos da população
    //TODO: Definir critérios para seleção de pares (provavelmente usando o método da roleta)     
    //TODO: Definir taxa de mutação  


    return 0;
}