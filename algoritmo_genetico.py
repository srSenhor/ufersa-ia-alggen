import random
import math
import matplotlib.pyplot as plt

# Função de aptidão (fitness)
def fitness(x, y):
    return math.sqrt(x**3 + 2*y**4)

# Função para converter um número decimal para binário com 3 bits
def decimal_para_binario(numero):
    return bin(numero)[2:].zfill(3)

# Função para converter um cromossomo em formato de 6 bits para valores de x e y
def cromossomo_para_xy(cromossomo):
    cromossomo_x = cromossomo[:3]
    cromossomo_y = cromossomo[3:]
    x = int(cromossomo_x, 2)
    y = int(cromossomo_y, 2)
    return x, y

# Função para converter um valor de x ou y em binário de 3 bits
def valor_para_binario(valor):
    return decimal_para_binario(valor).zfill(3)

# Inicialização da população de indivíduos
def inicializar_populacao(pop_size, gene_length):
    populacao = [''.join([str(random.randint(0, 1)) for _ in range(gene_length)]) for _ in range(pop_size)]
    return populacao

# Cálculo do fitness de cada indivíduo na população
def calcular_fitness_populacao(populacao):
    return [fitness(*cromossomo_para_xy(cromossomo)) for cromossomo in populacao]

# Seleção de pares com base na roleta
def selecao(populacao, fitness_populacao, num_selecionados):
    total_fitness = sum(fitness_populacao)
    selecionados = []
    while len(selecionados) < num_selecionados:
        r = random.uniform(0, total_fitness)
        acumulado = 0
        for i, aptidao in enumerate(fitness_populacao):
            acumulado += aptidao
            if acumulado >= r:
                selecionados.append(populacao[i])
                break
    return selecionados

# Cruzamento (crossover) entre dois pais
def crossover(pai1, pai2):
    ponto_corte = random.randint(1, len(pai1) - 1)
    filho1 = pai1[:ponto_corte] + pai2[ponto_corte:]
    filho2 = pai2[:ponto_corte] + pai1[ponto_corte:]
    return filho1, filho2

# Mutação em um cromossomo
def mutacao(cromossomo, taxa_mutacao):
    cromossomo_mutado = ''.join(['1' if bit == '0' and random.random() < taxa_mutacao else
                                 '0' if bit == '1' and random.random() < taxa_mutacao else bit for bit in cromossomo])
    return cromossomo_mutado

# Algoritmo Genético
def algoritmo_genetico(populacao_tamanho, gene_length, taxa_crossover, taxa_mutacao, geracoes):
    populacao = inicializar_populacao(populacao_tamanho, gene_length)
    melhores_fitness = []
    melhores_individuos = []

    for geracao in range(geracoes):
        fitness_populacao = calcular_fitness_populacao(populacao)

        # Encontre o melhor indivíduo da geração atual
        melhor_indice = fitness_populacao.index(max(fitness_populacao))
        melhor_cromossomo = populacao[melhor_indice]
        x, y = cromossomo_para_xy(melhor_cromossomo)
        resultado = fitness(x, y)

        # Adicione o melhor fitness e o melhor indivíduo à lista de convergência
        melhores_fitness.append(resultado)
        melhores_individuos.append(melhor_cromossomo)

        # Imprima a população da geração atual
        print(f'Geração {geracao + 1}:')
        for i, cromossomo in enumerate(populacao):
            x, y = cromossomo_para_xy(cromossomo)
            resultado = fitness(x, y)
            print(f'Indivíduo {i + 1}({cromossomo}): x={x}, y={y}, f(x, y)={resultado}')

        # Imprima o melhor indivíduo da geração atual
        print(f'Melhor solução da geração {geracao + 1} - x={x}, y={y}, f(x, y)={resultado}')

        # Selecione indivíduos para cruzamento
        selecionados = selecao(populacao, fitness_populacao, populacao_tamanho)

        # Realize o crossover para criar a próxima geração
        nova_populacao = []
        while len(nova_populacao) < populacao_tamanho:
            pai1, pai2 = random.sample(selecionados, 2)
            filho1, filho2 = crossover(pai1, pai2)
            filho1 = mutacao(filho1, taxa_mutacao)
            filho2 = mutacao(filho2, taxa_mutacao)
            nova_populacao.extend([filho1, filho2])

        # Atualize a população para a próxima geração
        populacao = nova_populacao

    # Encontre o melhor indivíduo de toda a execução
    fitness_populacao = calcular_fitness_populacao(populacao)
    melhor_indice = fitness_populacao.index(max(fitness_populacao))
    melhor_cromossomo = populacao[melhor_indice]
    x, y = cromossomo_para_xy(melhor_cromossomo)
    resultado = fitness(x, y)

    # Adicione o melhor fitness e o melhor indivíduo à lista de convergência
    melhores_fitness.append(resultado)
    melhores_individuos.append(melhor_cromossomo)

    # Imprima o resultado da melhor solução encontrada
    print("\nMelhor solução encontrada:")
    print(f"x={x}, y={y}, f(x, y)={resultado}")

    # Plotar gráfico de convergência
    plt.figure()
    plt.plot(melhores_fitness)
    plt.xlabel('Gerações')
    plt.ylabel('Melhor Fitness')
    plt.title('Convergência do Algoritmo Genético')
    plt.grid()
    plt.show()

# Parâmetros do algoritmo
populacao_tamanho = 8
gene_length = 6  # 3 bits para x, 3 bits para y
taxa_crossover = 0.7
taxa_mutacao = 0.1
geracoes = 10

# Executar o Algoritmo Genético
algoritmo_genetico(populacao_tamanho, gene_length, taxa_crossover, taxa_mutacao, geracoes)
