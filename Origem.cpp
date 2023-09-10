#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <bitset>

using namespace std;

// Fun��o de aptid�o (fitness)
double fitness(double x, double y) {
    return sqrt(pow(x, 3) + 2 * pow(y, 4));
}

// Fun��o para converter um n�mero decimal para bin�rio com 3 bits
string decimal_para_binario(int numero) {
    string binario = bitset<3>(numero).to_string();
    return binario;
}

// Fun��o para converter um cromossomo em formato de 6 bits para valores de x e y
pair<int, int> cromossomo_para_xy(const string& cromossomo) {
    int x = stoi(cromossomo.substr(0, 3), nullptr, 2);
    int y = stoi(cromossomo.substr(3, 3), nullptr, 2);
    return make_pair(x, y);
}

// Fun��o para converter um valor de x ou y em bin�rio de 3 bits
string valor_para_binario(int valor) {
    return decimal_para_binario(valor);
}

// Inicializa��o da popula��o de indiv�duos
vector<string> inicializar_populacao(int pop_size, int gene_length) {
    vector<string> populacao;
    default_random_engine gerador(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> distribuicao(0, 1);

    for (int i = 0; i < pop_size; ++i) {
        string cromossomo;
        for (int j = 0; j < gene_length; ++j) {
            int bit = distribuicao(gerador);
            cromossomo += to_string(bit);
        }
        populacao.push_back(cromossomo);
    }
    return populacao;
}

// C�lculo do fitness de cada indiv�duo na popula��o
vector<double> calcular_fitness_populacao(const vector<string>& populacao) {
    vector<double> fitness_populacao;
    for (const string& cromossomo : populacao) {
        pair<int, int> xy = cromossomo_para_xy(cromossomo);
        double resultado = fitness(xy.first, xy.second);
        fitness_populacao.push_back(resultado);
    }
    return fitness_populacao;
}

// Sele��o de pares com base na roleta
vector<string> selecao(const vector<string>& populacao, const vector<double>& fitness_populacao, int num_selecionados) {
    double total_fitness = 0;
    for (double aptidao : fitness_populacao) {
        total_fitness += aptidao;
    }

    vector<string> selecionados;
    default_random_engine gerador(static_cast<unsigned>(time(nullptr)));
    uniform_real_distribution<double> distribuicao(0, total_fitness);

    cout << "F(x,y)\t\t\tPeda�o da Roleta" << endl;
    double inicio_intervalo = 0.0;
    for (size_t i = 0; i < fitness_populacao.size(); ++i) {
        double probabilidade = fitness_populacao[i] / total_fitness;
        double fim_intervalo = inicio_intervalo + probabilidade;

        cout << fixed << fitness_populacao[i] << "\t\t" << fixed << inicio_intervalo << " - " << fim_intervalo << endl;

        inicio_intervalo = fim_intervalo;
    }

    while (selecionados.size() < static_cast<size_t>(num_selecionados)) {
        double r = distribuicao(gerador);
        double acumulado = 0;
        for (size_t i = 0; i < fitness_populacao.size(); ++i) {
            acumulado += fitness_populacao[i] / total_fitness;
            if (acumulado >= r) {
                selecionados.push_back(populacao[i]);
                break;
            }
        }
    }
    return selecionados;
}

// Cruzamento (crossover) entre dois pais
pair<string, string> crossover(const string& pai1, const string& pai2) {
    default_random_engine gerador(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> distribuicao(1, pai1.length() - 1);
    int ponto_corte = distribuicao(gerador);

    string filho1 = pai1.substr(0, ponto_corte) + pai2.substr(ponto_corte);
    string filho2 = pai2.substr(0, ponto_corte) + pai1.substr(ponto_corte);

    return make_pair(filho1, filho2);
}

// Muta��o em um cromossomo
string mutacao(const string& cromossomo, double taxa_mutacao) {
    default_random_engine gerador(static_cast<unsigned>(time(nullptr)));
    uniform_real_distribution<double> distribuicao(0, 1);

    string cromossomo_mutado = cromossomo;
    for (char& bit : cromossomo_mutado) {
        if (bit == '0' && distribuicao(gerador) < taxa_mutacao) {
            bit = '1';
        }
        else if (bit == '1' && distribuicao(gerador) < taxa_mutacao) {
            bit = '0';
        }
    }
    return cromossomo_mutado;
}

// Algoritmo Gen�tico
void algoritmo_genetico(int populacao_tamanho, int gene_length, double taxa_crossover, double taxa_mutacao, int geracoes) {
    vector<string> populacao = inicializar_populacao(populacao_tamanho, gene_length);
    vector<double> melhores_fitness;
    vector<string> melhores_individuos;

    for (int geracao = 0; geracao < geracoes; ++geracao) {
        vector<double> fitness_populacao = calcular_fitness_populacao(populacao);

        // Encontre o melhor indiv�duo da gera��o atual
        auto max_fitness = max_element(fitness_populacao.begin(), fitness_populacao.end());
        int melhor_indice = distance(fitness_populacao.begin(), max_fitness);
        string melhor_cromossomo = populacao[melhor_indice];
        pair<int, int> xy = cromossomo_para_xy(melhor_cromossomo);
        double resultado = fitness(xy.first, xy.second);

        // Adicione o melhor fitness e o melhor indiv�duo � lista de converg�ncia
        melhores_fitness.push_back(resultado);
        melhores_individuos.push_back(melhor_cromossomo);

        // Imprima a popula��o da gera��o atual
        cout << "Gera��o " << geracao + 1 << ":\n";
        for (size_t i = 0; i < populacao.size(); ++i) {
            pair<int, int> xy = cromossomo_para_xy(populacao[i]);
            double resultado = fitness(xy.first, xy.second);
            cout << "Indiv�duo " << i + 1 << "(" << populacao[i] << "): x=" << xy.first << ", y=" << xy.second << ", f(x, y)=" << resultado << "\n";
        }

        // Imprima o melhor indiv�duo da gera��o atual
        cout << "Melhor solu��o da gera��o " << geracao + 1 << " - x=" << xy.first << ", y=" << xy.second << ", f(x, y)=" << resultado << "\n";

        // Selecione indiv�duos para cruzamento
        vector<string> selecionados = selecao(populacao, fitness_populacao, populacao_tamanho);

        // Realize o crossover para criar a pr�xima gera��o
        vector<string> nova_populacao;
        while (nova_populacao.size() < static_cast<size_t>(populacao_tamanho)) {
            pair<string, string> pais = { selecionados[rand() % selecionados.size()], selecionados[rand() % selecionados.size()] };
            pair<string, string> filhos = crossover(pais.first, pais.second);
            filhos.first = mutacao(filhos.first, taxa_mutacao);
            filhos.second = mutacao(filhos.second, taxa_mutacao);
            nova_populacao.push_back(filhos.first);
            nova_populacao.push_back(filhos.second);
        }

        // Atualize a popula��o para a pr�xima gera��o
        populacao = nova_populacao;
    }

    // Encontre o melhor indiv�duo de toda a execu��o
    vector<double> fitness_populacao = calcular_fitness_populacao(populacao);
    auto max_fitness = max_element(fitness_populacao.begin(), fitness_populacao.end());
    int melhor_indice = distance(fitness_populacao.begin(), max_fitness);
    string melhor_cromossomo = populacao[melhor_indice];
    pair<int, int> xy = cromossomo_para_xy(melhor_cromossomo);
    double resultado = fitness(xy.first, xy.second);

    // Adicione o melhor fitness e o melhor indiv�duo � lista de converg�ncia
    melhores_fitness.push_back(resultado);
    melhores_individuos.push_back(melhor_cromossomo);

    // Imprima o resultado da melhor solu��o encontrada
    cout << "\nMelhor solu��o encontrada:\n";
    cout << "x=" << xy.first << ", y=" << xy.second << ", f(x, y)=" << resultado << "\n";
}

int main() {
    // Par�metros do algoritmo
    int populacao_tamanho = 8;
    int gene_length = 6;  // 3 bits para x, 3 bits para y
    double taxa_crossover = 0.7;
    double taxa_mutacao = 0.1;
    int geracoes = 10;

    // Executar o Algoritmo Gen�tico
    algoritmo_genetico(populacao_tamanho, gene_length, taxa_crossover, taxa_mutacao, geracoes);

    return 0;
}
