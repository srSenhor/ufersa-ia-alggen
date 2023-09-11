import numpy as np
import matplotlib.pyplot as plt
import random

# Setando constantes
NUM_PARES = 8
MAX_ITERACOES = 50
TAXA_MUTACAO = 0.05

# Função usada: raizquadrada(x^3 + 2*(y^4))
def f(x, y):
    return np.sqrt(np.power(x, 3) + 2 * np.power(y, 4))

def seletor_pais(intervalos):
    num = random.randint(0, 360)
    lim_sup = intervalos[0]

    for i in range(NUM_PARES):
        if num <= lim_sup:
            return i
        else:
            lim_sup += intervalos[i + 1]

    return -1

def converte_bitset(x, y):
    bin_x = np.binary_repr(x, width=3)
    bin_y = np.binary_repr(y, width=3)
    bin_str = bin_x + bin_y
    return int(bin_str, 2)

def crossover(pais):
    num = random.randint(0, 5)

    for i in range(0, NUM_PARES, 2):
        m1 = [0, 0]  # Máscaras do primeiro pai
        m2 = [0, 0]  # Máscaras do segundo pai

        if num == 0:
            m1[0] = 32
            m1[1] = 31
            m2[0] = 32
            m2[1] = 31
        elif num == 1:
            m1[0] = 48
            m1[1] = 15
            m2[0] = 48
            m2[1] = 15
        elif num == 2:
            m1[0] = 56
            m1[1] = 7
            m2[0] = 56
            m2[1] = 7
        elif num == 3:
            m1[0] = 60
            m1[1] = 3
            m2[0] = 60
            m2[1] = 3
        elif num == 4:
            m1[0] = 62
            m1[1] = 1
            m2[0] = 62
            m2[1] = 1
        elif num == 5:
            m1[0] = 63
            m1[1] = 0
            m2[0] = 63
            m2[1] = 0

        m1[0] &= pais[i]
        m1[1] &= pais[i]
        m2[0] &= pais[i + 1]
        m2[1] &= pais[i + 1]

        pais[i] = m1[0] | m2[1]
        pais[i + 1] = m2[0] | m1[1]
        mutacao(pais[i])
        mutacao(pais[i + 1])

def mutacao(individuo):
    x = random.randint(0, 4)
    if x < TAXA_MUTACAO:
        bit_alterado = random.randint(0, 5)
        individuo ^= (1 << bit_alterado)

# Setando seed para criação de números aleatórios
pop = np.array([random.randint(0, 7) for _ in range(2 * NUM_PARES)]).reshape(NUM_PARES, 2)

# Avaliando os indivíduos
aval = np.zeros(NUM_PARES)
total_aval = 0.0
melhor_aval = 100
melhor = np.zeros(2)

melhores_x = [melhor[0]]
melhores_y = [melhor[1]]

for i in range(NUM_PARES):
    aval[i] = 1 + f(pop[i][0], pop[i][1])
    if aval[i] < melhor_aval:
        melhor_aval = aval[i]
        melhor[0] = pop[i][0]
        melhor[1] = pop[i][1]
        melhores_x.append(melhor[0])
        melhores_y.append(melhor[1])
    total_aval += 1 / aval[i]

# Repartindo os pedaços da roleta
intervalos_roleta = np.zeros(NUM_PARES)
for i in range(NUM_PARES):
    intervalos_roleta[i] = ((1.0 / aval[i]) / total_aval) * 360
    intervalos_roleta[i] = round(intervalos_roleta[i])

i = 0
print(f"Melhor individuo da geracao {i}: x = {melhor[0]}, y = {melhor[1]}")
while melhor_aval > 1.0 and i < MAX_ITERACOES:
    # Selecionando pais
    selecionados = [seletor_pais(intervalos_roleta) for _ in range(NUM_PARES)]

    # Convertendo para bitset
    pais = [converte_bitset(pop[selecionado][0], pop[selecionado][1]) for selecionado in selecionados]

    # Realizando o crossover
    crossover(pais)

    # Reavaliando os indivíduos
    total_aval = 0.0

    for j in range(NUM_PARES):
        aux_x = pais[j] >> 3
        aux_y = pais[j] & 7

        pop[j][0] = aux_x
        pop[j][1] = aux_y

    for j in range(NUM_PARES):
        aval[j] = 1 + f(pop[j][0], pop[j][1])
        if aval[j] < melhor_aval:
            melhor_aval = aval[j]
            melhor[0] = pop[j][0]
            melhor[1] = pop[j][1]
            melhores_x.append(melhor[0])
            melhores_y.append(melhor[1])
        total_aval += 1 / aval[j]

    # Repartindo os pedaços da roleta de novo
    for j in range(NUM_PARES):
        intervalos_roleta[j] = ((1.0 / aval[j]) / total_aval) * 360
        intervalos_roleta[j] = round(intervalos_roleta[j])

    i += 1
    print(f"Melhor individuo da geracao {i}: x = {melhor[0]}, y = {melhor[1]}")

# Visualizando valores
print("Populacao de individuos")
for i in range(NUM_PARES):
    print(f"X: {pop[i][0]}, Y: {pop[i][1]}")

print("\nAvaliacao dos individuos sob f(x,y)")
for i in range(NUM_PARES):
    print(f"{i + 1}: {aval[i]}")

print(f"Total: {total_aval}")

melhores_x = [melhor[0]]
melhores_y = [melhor[1]]

plt.plot(i, melhores_x, label='Melhor X', marker='o', linestyle='-')
plt.plot(i, melhores_y, label='Melhor Y', marker='o', linestyle='-')
plt.title('Evolução dos Melhores Indivíduos')
plt.xlabel('Geração')
plt.ylabel('Melhor Indivíduo')
plt.legend()
plt.grid(True)
plt.show()