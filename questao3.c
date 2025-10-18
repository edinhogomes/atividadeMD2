#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef long long ll;

/*
    ALUNO: EDSON GOMES DA SILVA FILHO
    MATRICULA: 221034982
*/

/**
 * @brief Calcula a potência (base^exp).
 * @param base A base.
 * @param exp O expoente (deve ser não-negativo).
 * @return O resultado da potência.
 */
ll calcular_potencia(ll base, int exp) {
    ll resultado = 1;
    for (int i = 0; i < exp; i++) {
        resultado *= base;
    }
    return resultado;
}

/**
 * @brief Implementa a fatoração prima por Trial Division (Divisão por Tentativa) e calcula tau(N) e sigma(N).
 * @param N O número a ser analisado (1 <= N <= 10^5).
 * @param tau Ponteiro para armazenar o número de divisores (tau(N)).
 * @param sigma Ponteiro para armazenar a soma dos divisores (sigma(N)).
 */
void fatorar_e_calcular_funcoes(ll N, ll *tau, ll *sigma) {
    ll temp_N = N;
    ll d = 2; // Começa a checar pelo menor primo
    *tau = 1;
    *sigma = 1;
    
    printf("\n[Fatoracao Prima de N = %lld]\n", N);
    
    if (N == 1) {
        printf("  N = 1 -> Fatoracao trivial.\n");
        printf("  Lista de Fatores Primos e Expoentes: (Nao ha fatores)\n");
        return; // tau(1)=1, sigma(1)=1. Ja inicializados.
    }

    printf("  Lista de Fatores Primos e Expoentes (p^a):\n");

    // Loop principal de Trial Division
    while (d * d <= temp_N) {
        if (temp_N % d == 0) {
            int a = 0; // Expoente do fator p_i
            
            // 1. Encontra o expoente 'a' e remove o fator 'p_i' completamente de temp_N
            while (temp_N % d == 0) {
                temp_N /= d;
                a++;
            }
            
            printf("  - Fator: %lld, Expoente: %d\n", d, a);
            
            // 2. Cálculo de tau(N): multiplicando (a_i + 1)
            *tau *= (a + 1);
            
            // 3. Cálculo de sigma(N): multiplicando a formula (p^(a+1) - 1) / (p - 1)
            // Calculo do termo da serie geometrica (p^(a+1) + ... + 1)
            // O calculo (p^(a+1) - 1) / (p - 1) e seguro pois d >= 2
            ll termo_sigma = (calcular_potencia(d, a + 1) - 1) / (d - 1);
            *sigma *= termo_sigma;
        }
        
        // Otimização: Apos 2, checa apenas impares
        if (d == 2) d++;
        else d += 2;
    }

    // Se sobrou algum fator maior que sqrt(N), ele é primo (com expoente a=1)
    if (temp_N > 1) {
        ll d_restante = temp_N;
        int a_restante = 1;
        
        printf("  - Fator: %lld, Expoente: %d\n", d_restante, a_restante);

        // Atualiza tau(N)
        *tau *= (a_restante + 1);
        
        // Atualiza sigma(N)
        ll termo_sigma = (calcular_potencia(d_restante, a_restante + 1) - 1) / (d_restante - 1);
        *sigma *= termo_sigma;
    }
}


int main() {
    ll N;
    ll tau_N = 0;
    ll sigma_N = 0;

    printf(" Questao 3: A Razao de Eficiencia de um Numero N: tau(N) / sigma(N)\n");

    // Entrada de N
    printf("Digite um inteiro N (1 <= N <= 105): ");
    if (scanf("%lld", &N) != 1 || N < 1 || N > 105) {
        printf("Erro: Entrada invalida. N deve estar entre 1 e 105.\n");
        return 1;
    }

    // Fatoração e Cálculo de Tau e Sigma
    fatorar_e_calcular_funcoes(N, &tau_N, &sigma_N);

    printf("\n[Calculos Intermediarios]\n");
    printf("  Funcao tau(N) (Numero de divisores) = %lld\n", tau_N);
    printf("  Funcao sigma(N) (Soma dos divisores) = %lld\n", sigma_N);

    // Cálculo da Razão de Eficiência
    double razao_eficiencia;
    if (sigma_N == 0) { // Deve ocorrer apenas para N=0, mas por seguranca
        printf("Erro: Soma dos divisores (sigma(N)) e zero. Impossivel calcular a razao.\n");
        return 1;
    }
    
    // Casting para double para realizar a divisao real
    razao_eficiencia = (double)tau_N / sigma_N;

    printf("\n[Resultado Final]\n");
    printf("  Razao de Eficiencia = tau(N) / sigma(N)\n");
    printf("  Razao de Eficiencia = %lld / %lld\n", tau_N, sigma_N);
    // Impressao com duas casas decimais de precisao
    printf("  Razao de Eficiencia de %lld (Precisao 2 casas decimais) = %.2f\n", N, razao_eficiencia);

    return 0;
}
