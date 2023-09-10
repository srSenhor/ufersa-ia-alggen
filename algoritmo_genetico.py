import random
import math

# Defina a função de aptidão (fitness)
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

# Crie uma função para inicializar a população de indivíduos
def inicializar_populacao(pop_size, gene_length):
    populacao = []
    for _ in range(pop_size):
        cromossomo = ''.join([str(random.randint(0, 1)) for _ in range(gene_length)])
        populacao.append(cromossomo)
    return populacao

# Crie uma função para calcular o fitness de cada indivíduo na população
def calcular_fitness_populacao(populacao):
    fitness_populacao = []
    for cromossomo in populacao:
        x, y = cromossomo_para_xy(cromossomo)
        aptidao = fitness(x, y)
        fitness_populacao.append(aptidao)
    return fitness_populacao

# Crie uma função para selecionar indivíduos para cruzamento com base na roleta
def selecao(populacao, fitness_populacao, num_selecionados):
    selecionados = []
    total_fitness = sum(fitness_populacao)
    while len(selecionados) < num_selecionados:
        acumulado = 0
        r = random.uniform(0, total_fitness)
        for i, aptidao in enumerate(fitness_populacao):
            acumulado += aptidao
            if acumulado >= r:
                selecionados.append(populacao[i])
                break
    return selecionados

# Crie uma função para realizar o cruzamento (crossover) entre dois pais
def crossover(pai1, pai2):
    ponto_corte = random.randint(1, len(pai1) - 1)
    filho1 = pai1[:ponto_corte] + pai2[ponto_corte:]
    filho2 = pai2[:ponto_corte] + pai1[ponto_corte:]
    return filho1, filho2

# Crie uma função para realizar a mutação em um cromossomo
def mutacao(cromossomo, taxa_mutacao):
    cromossomo_mutado = ''
    for bit in cromossomo:
        if random.random() < taxa_mutacao:
            cromossomo_mutado += '1' if bit == '0' else '0'
        else:
            cromossomo_mutado += bit
    return cromossomo_mutado

# Defina hiperparâmetros
populacao_tamanho = 8
gene_length = 6  # 3 bits para x, 3 bits para y
taxa_crossover = 0.7
taxa_mutacao = 0.1
geracoes = 10

# Inicialize a população
populacao = inicializar_populacao(populacao_tamanho, gene_length)

# Execute o algoritmo genético por um número fixo de gerações
for geracao in range(geracoes):
    # Calcule o fitness da população atual
    fitness_populacao = calcular_fitness_populacao(populacao)

    # Imprima todos os indivíduos da geração atual
    print(f'Geração {geracao + 1}:')
    for i, cromossomo in enumerate(populacao):
        x, y = cromossomo_para_xy(cromossomo)
        resultado = fitness(x, y)
        print(f'Indivíduo {i + 1}: x={x}, y={y}, f(x, y)={resultado}')

    # Encontre o melhor indivíduo da geração atual
    melhor_indice = fitness_populacao.index(max(fitness_populacao))
    melhor_cromossomo = populacao[melhor_indice]
    x, y = cromossomo_para_xy(melhor_cromossomo)
    resultado = fitness(x, y)

    # Imprima o resultado da melhor solução da geração atual
    print(f'Melhor solução - x={x}, y={y}, f(x, y)={resultado}')

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

# Imprima o resultado da melhor solução encontrada
print("\nMelhor solução encontrada:")
print(f"x={x}, y={y}, f(x, y)={resultado}")


