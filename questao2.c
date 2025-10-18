#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

/*
    ALUNO: EDSON GOMES DA SILVA FILHO
    MATRICULA: 221034982
*/


// --- Funções Auxiliares ---

/**
 * @brief Implementa o Algoritmo de Euclides para calcular o Máximo Divisor Comum (MDC) de dois números.
 * @param a Primeiro número.
 * @param b Segundo número.
 * @return O MDC de 'a' e 'b'.
 */
ll euclides_mdc(ll a, ll b) {
    ll temp;
    // Garante que a e b sejam positivos
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/**
 * @brief Calcula o Mínimo Múltiplo Comum (MMC) de dois números usando o MDC.
 * MMC(a, b) = (|a * b|) / MDC(a, b)
 * @param a Primeiro número.
 * @param b Segundo número.
 * @return O MMC de 'a' e 'b'.
 */
ll calcula_mmc(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    // O MMC é (a * b) / MDC(a, b). 
    // Para evitar overflow (apesar de a*b ser pequeno aqui), dividimos antes:
    return (a / euclides_mdc(a, b)) * b;
}


// --- Função Principal ---

int main() {
    int N; // Número de chaves
    const int LIMITE_ANO = 50;

    printf("  Questao 2: Chaves Periodicas (MMC)\n");
    printf("  Limite de Ano: %d\n", LIMITE_ANO);

    // 1. Entrada de N
    printf("Digite o numero de chaves N (1 <= N <= 10): ");
    if (scanf("%d", &N) != 1 || N < 1 || N > 10) {
        printf("Erro: Numero de chaves invalido.\n");
        return 1;
    }

    // Array para armazenar os ciclos
    ll ciclos[10];
    
    // 2. Entrada dos ciclos
    printf("Digite os %d ciclos C (2 <= C <= 20) separados por espaco:\n", N);
    for (int i = 0; i < N; i++) {
        if (scanf("%lld", &ciclos[i]) != 1) {
            printf("Erro: Entrada de ciclo invalida.\n");
            return 1;
        }
        if (ciclos[i] < 2 || ciclos[i] > 20) {
             printf("Erro: Ciclo C%d (%lld) fora do intervalo [2, 20].\n", i + 1, ciclos[i]);
             return 1;
        }
    }

    // 3. Cálculo iterativo do MMC
    // O MMC final é o primeiro ano X > 0 em que todas as chaves sincronizam.
    ll mmc_atual = ciclos[0]; 

    printf("\n[Processo de Calculo do MMC]\n");
    printf("  MMC inicial = %lld\n", mmc_atual);

    for (int i = 1; i < N; i++) {
        ll proximo_mmc = calcula_mmc(mmc_atual, ciclos[i]);
        
        printf("  MMC(%lld, %lld) = %lld\n", mmc_atual, ciclos[i], proximo_mmc);
        mmc_atual = proximo_mmc;

        // Verificação intermediária: se o MMC ultrapassar o limite de 50 anos,
        // não é necessário continuar, pois o resultado final também será maior que 50.
        if (mmc_atual > LIMITE_ANO) {
            break; 
        }
    }

    ll ano_sincronizado = mmc_atual;

    // 4. Verificação do Limite e Saída
    printf("\n[Resultado Final]\n");
    if (ano_sincronizado > LIMITE_ANO) {
        printf("Nao e possivel. O primeiro ano sincronizado (%lld) excede o limite de %d anos.\n", 
               ano_sincronizado, LIMITE_ANO);
    } else {
        printf("O primeiro ano sincronizado (X > 0) e: %lld\n", ano_sincronizado);
    }

    return 0;
}
