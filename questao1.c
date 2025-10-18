#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h> // Para toupper()

/*
    ALUNO: EDSON GOMES DA SILVA FILHO
    MATRICULA: 221034982
*/


// Definição de tipos para maior clareza e suporte a números grandes
typedef long long ll;
#define MAX_MSG_LEN 100
#define MAX_CODIGO_LEN (MAX_MSG_LEN * 2 + 1) // O dobro do tamanho da mensagem + '\0'

// =======================================================================
// Funções Auxiliares de Aritmética Modular
// =======================================================================

/**
 * @brief Implementa o Algoritmo de Euclides para calcular o Máximo Divisor Comum (MDC).
 * @param a Primeiro número.
 * @param b Segundo número.
 * @return O MDC de 'a' e 'b'.
 */
ll euclides_mdc(ll a, ll b) {
    ll temp;
    // Garante que a e b sejam positivos
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;

    printf("  [MDC: Algoritmo de Euclides]\n");
    while (b != 0) {
        printf("    MDC(%lld, %lld) -> %lld mod %lld = %lld\n", a, b, a, b, a % b);
        temp = b;
        b = a % b;
        a = temp;
    }
    printf("  [MDC: Resultado -> %lld]\n", a);
    return a;
}

/**
 * @brief Implementa o Algoritmo Estendido de Euclides para encontrar o inverso modular.
 * Encontra x tal que a*x + b*y = mdc(a, b). Para o inverso, queremos a*x + z*y = 1.
 * O inverso modular é x mod b (se mdc(a, b) = 1).
 * @param a O número para o qual queremos o inverso (o expoente E).
 * @param b O módulo (o totiente z(n)).
 * @return O inverso modular de a mod b. Retorna -1 se não houver inverso.
 */
ll euclides_estendido(ll a, ll b) {
    ll r0 = a, r1 = b;
    ll x0 = 1, x1 = 0;
    ll y0 = 0, y1 = 1;

    printf("\n  [Inverso Modular (D) via Euclides Estendido]\n");
    printf("  Buscando D tal que D * %lld = 1 mod %lld\n", a, b);
    printf("  -------------------------------------------------\n");
    printf("  | r | q | x | y | (r = a*x + b*y)\n");
    printf("  -------------------------------------------------\n");
    printf("  | %lld | - | %lld | %lld |\n", r0, x0, y0);
    printf("  | %lld | - | %lld | %lld |\n", r1, x1, y1);

    while (r1 != 0) {
        ll q = r0 / r1;

        ll r2 = r0 - q * r1;
        ll x2 = x0 - q * x1;
        ll y2 = y0 - q * y1;

        printf("  | %lld | %lld | %lld | %lld |\n", r2, q, x2, y2);

        r0 = r1; r1 = r2;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }

    printf("  -------------------------------------------------\n");
    ll mdc = r0;
    ll x = x0;
    
    if (mdc != 1) {
        printf("  MDC(%lld, %lld) = %lld. O inverso nao existe.\n", a, b, mdc);
        return -1;
    }

    // Garante que o resultado seja positivo no intervalo [0, b-1]
    ll inverso = x % b;
    if (inverso < 0) {
        inverso += b;
    }

    printf("  MDC = 1. Inverso D = %lld mod %lld = %lld\n", x, b, inverso);
    return inverso;
}

/**
 * @brief Verifica se um número é primo (Teste de primalidade simplificado).
 * @param n O número a ser testado.
 * @return 1 se primo, 0 se composto.
 */
int e_primo(ll n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (ll i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

/**
 * @brief Implementa a exponenciação modular otimizada (quadrática) com aplicação de teoremas.
 * Calcula (base^exp) mod modN.
 * @param base A base M ou C.
 * @param exp O expoente E ou D.
 * @param modN O módulo n.
 * @param totient O totiente z(n).
 * @return O resultado da potência modular.
 */
ll potencia_modular(ll base, ll exp, ll modN, ll totient) {
    ll resultado = 1;
    ll M = base;
    ll E = exp;
    ll n = modN;

    printf("\n  [Potencia Modular: %lld ^ %lld mod %lld]\n", M, E, n);
    printf("  --------------------------------------------------\n");

    // 1. Redução do expoente (se aplicável)
    ll E_reduzido = E;
    char* teorema_aplicado = "Divisao Euclidiana (Padrao)";

    if (e_primo(n)) {
        // Pequeno Teorema de Fermat: a^(p-1) = 1 mod p, se p nao divide a.
        // Como RSA usa n=pq, esta condicao NUNCA DEVE SER ATENDIDA para o n do RSA.
        // No entanto, se o programa fosse usar um n primo, seria a opcao.
        // Vamos checar apenas se for um modulo primo (o que pode ocorrer em outros contextos)
        // Se n for primo, totient = n - 1
        if (totient == n - 1 && M % n != 0) {
             E_reduzido = E % (n - 1);
             teorema_aplicado = "Pequeno Teorema de Fermat";
             printf("  APLICANDO: %s (n=%lld primo, mdc(M,n)=1)\n", teorema_aplicado, n);
             printf("  Novo Expoente: %lld mod %lld = %lld\n", E, n - 1, E_reduzido);
        }
    } else {
        // Teorema de Euler: a^phi(n) = 1 mod n, se mdc(a, n) = 1.
        if (euclides_mdc(M, n) == 1) {
            E_reduzido = E % totient;
            teorema_aplicado = "Teorema de Euler";
            printf("  APLICANDO: %s (mdc(M,n)=1)\n", teorema_aplicado);
            printf("  Totiente z(n) = %lld\n", totient);
            printf("  Novo Expoente: %lld mod %lld = %lld\n", E, totient, E_reduzido);
        }
    }
    
    // Default: Divisão Euclidiana (e se os teoremas acima não se aplicarem)
    // Se nenhum teorema for aplicado, E_reduzido = E.
    if (strcmp(teorema_aplicado, "Divisao Euclidiana (Default)") == 0) {
        printf("  APLICANDO: %s (Exponenciacao Quadratica)\n", teorema_aplicado);
    }
    
    E = E_reduzido;

    // 2. Exponenciação binária (quadrática)
    // (a * b) mod n = ((a mod n) * (b mod n)) mod n
    M = M % n;

    printf("  --------------------------------------------------\n");
    printf("  Passos da Exponenciacao Quadratica:\n");
    ll base_atual = M;
    ll exp_restante = E;
    int passo = 1;

    while (exp_restante > 0) {
        printf("  Passo %d (Exp=%lld): ", passo++, exp_restante);
        if (exp_restante & 1) {
            // Se o bit menos significativo for 1, multiplica o resultado
            resultado = (resultado * base_atual) % n;
            printf("resultado = (%lld * %lld) mod %lld = %lld (Multiplica)\n", resultado, base_atual, n, resultado);
        } else {
            printf("resultado = %lld (Pula)\n", resultado);
        }
        
        // Quadra a base
        base_atual = (base_atual * base_atual) % n;
        printf("  Base Quad: %lld^2 mod %lld = %lld\n", base_atual, n, base_atual);
        
        // Move para o próximo bit
        exp_restante >>= 1;
    }
    printf("  --------------------------------------------------\n");

    return resultado;
}


// =======================================================================
// Funções de Codificação e Decodificação de Caracteres (do código anterior)
// =======================================================================

/**
 * @brief Converte uma letra maiúscula ou espaço para o código numérico de 2 dígitos.
 * @param letra A letra a ser codificada (A-Z ou ' ').
 * @return String (código) correspondente (ex: "11", "36", "00"). O retorno é uma string estática (deve ser copiada se necessário).
 */
char* codifica_letra(char letra) {
    static char codigo_str[3]; 

    if (letra >= 'A' && letra <= 'Z') {
        int codigo = letra - 'A' + 11;
        snprintf(codigo_str, 3, "%d", codigo);
    } else if (letra == ' ') {
        snprintf(codigo_str, 3, "00");
    } else {
        snprintf(codigo_str, 3, "??"); 
    }
    return codigo_str;
}

/**
 * @brief Converte um código numérico de 2 dígitos para a letra maiúscula ou espaço correspondente.
 * @param codigo_str A string do código (ex: "11", "36", "00").
 * @return O caractere correspondente ('A'-'Z' ou ' '). Retorna '?' para código inválido.
 */
char decodifica_codigo(const char* codigo_str) {
    if (strlen(codigo_str) != 2) return '?';

    int codigo = atoi(codigo_str);

    if (codigo >= 11 && codigo <= 36) {
        return 'A' + (codigo - 11);
    } else if (codigo == 0) {
        return ' ';
    } else {
        return '?';
    }
}


// =======================================================================
// Etapa 1: Método rho de Pollard (para fatorar N)
// *Reciclado do código anterior*
// =======================================================================

/**
 * @brief Implementa o método rho de Pollard para encontrar um fator não trivial de um número composto N.
 * @param n O número composto a ser fatorado.
 * @return Um fator primo não trivial de N.
 */
ll pollard_rho(ll n) {
    if (n <= 1 || n % 2 == 0) return (n % 2 == 0) ? 2 : n;

    ll x_rapido = 2; // Tartaruga
    ll x_lento = 2;  // Lebre
    ll d = 1;        // Fator encontrado

    printf("\n[Metodo rho de Pollard para N = %lld]\n", n);
    printf("Funcao de iteracao: g(x) = (x^2 + 1) mod %lld\n", n);
    
    int i = 1;
    do {
        // x_i = (x_{i-1}^2 + 1) mod n
        x_rapido = (x_rapido * x_rapido) % n;
        x_rapido = (x_rapido + 1) % n;

        // x_2i = g(g(x_{2i-2}))
        x_lento = (x_lento * x_lento) % n;
        x_lento = (x_lento + 1) % n;
        x_lento = (x_lento * x_lento) % n;
        x_lento = (x_lento + 1) % n;
        
        ll diferenca = x_lento - x_rapido;
        if (diferenca < 0) { diferenca = -diferenca; } // |x2 - x1|

        printf("\n[Iteracao %d]\n", i);
        printf("  x(i) = %lld, x(2i) = %lld\n", x_rapido, x_lento);
        printf("  Diferenca |x(2i) - x(i)| = %lld\n", diferenca);
        
        d = euclides_mdc(diferenca, n);

        i++;

        if (d > 1 && d < n) {
            printf("\n[Fator Encontrado] - Sucesso na Iteracao %d.\n", i - 1);
            return d;
        }
        if (d == n) {
            printf("\n[Falha Temporaria] - MDC igual a N. Tentando proxima iteracao.\n");
            // Se d=n, o ciclo se repetiu. Para fins do exercicio, retornamos -1.
            return -1; 
        }

    } while (d == 1);

    return d;
}


// =======================================================================
// Função Principal (main)
// =======================================================================
int main() {
    // Variáveis RSA
    ll p, q, N, z;
    ll E, D;
    
    // --- ETAPA 1: Fatoração Interativa (Método rho de Pollard) ---
    printf("========================================================\n");
    printf("  Sistema RSA: Etapa 1 - Fatoracao com Metodo rho de Pollard\n");
    printf("========================================================\n\n");

    ll N1, N2;
    do {
        printf("Digite o primeiro numero composto N1 (100-9999): ");
        if (scanf("%lld", &N1) != 1) { while(getchar() != '\n'); continue; }
    } while (N1 < 100 || N1 > 9999);
    do {
        printf("Digite o segundo numero composto N2 (100-9999, distinto de N1): ");
        if (scanf("%lld", &N2) != 1) { while(getchar() != '\n'); continue; }
    } while (N2 < 100 || N2 > 9999 || N2 == N1);
    
    // Fatoração de N1 (define p)
    p = pollard_rho(N1);
    if (p == -1) { printf("Falha ao fatorar N1. Encerrando.\n"); return 1; }
    ll p_complemento = N1 / p;
    // Escolhe o menor fator como p para ter N menor
    if (p > p_complemento) { p = p_complemento; } 
    printf("\nFatores de N1=%lld encontrados: %lld e %lld\n", N1, p, N1 / p);
    printf("Definindo p = %lld\n", p);

    // Fatoração de N2 (define q)
    q = pollard_rho(N2);
    if (q == -1) { printf("Falha ao fatorar N2. Encerrando.\n"); return 1; }
    ll q_complemento = N2 / q;
    if (q > q_complemento) { q = q_complemento; }
    printf("\nFatores de N2=%lld encontrados: %lld e %lld\n", N2, q, N2 / q);
    printf("Definindo q = %lld\n", q);
    
    // --- ETAPA 2: Geração das Chaves RSA ---
    printf("\n\n========================================================\n");
    printf("  Sistema RSA: Etapa 2 - Geracao das Chaves\n");
    printf("========================================================\n");
    
    // Cálculo do Módulo e Totiente
    N = p * q;
    z = (p - 1) * (q - 1); // Totiente de Euler z(n)
    
    printf("1. Modulo N = p * q = %lld * %lld = %lld\n", p, q, N);
    printf("2. Totiente z(n) = (p-1) * (q-1) = %lld * %lld = %lld\n", p - 1, q - 1, z);

    // Escolha do Expoente Público E
    E = 3; // Começa a busca pelo menor E > 1
    printf("\n3. Escolha do Expoente Publico E (mdc(E, z(n))=1):\n");
    while (E < z) {
        printf("  Testando E = %lld: ", E);
        ll resultado_mdc = euclides_mdc(E, z);
        if (resultado_mdc == 1) {
            printf("  E = %lld escolhido (MDC=1).\n", E);
            break;
        }
        E++; // Incrementa E
    }

    // Cálculo do Expoente Privado D
    printf("\n4. Calculo do Expoente Privado D (D * E = 1 mod z):\n");
    D = euclides_estendido(E, z);
    if (D == -1) { printf("Falha ao encontrar o inverso modular D. Encerrando.\n"); return 1; }
    
    // Impressão das Chaves
    printf("\n[CHAVES RSA GERADAS]\n");
    printf("  CHAVE PUBLICA: (N=%lld, E=%lld)\n", N, E);
    printf("  CHAVE PRIVADA: (N=%lld, D=%lld)\n", N, D);

    // --- ETAPA 3: Codificação e Decodificação ---
    printf("\n\n========================================================\n");
    printf("  Sistema RSA: Etapa 3 - Codificacao e Decodificacao\n");
    printf("========================================================\n");
    
    char msg_original[MAX_MSG_LEN];
    printf("Digite a mensagem a ser criptografada (A-Z e espaco, max %d chars): ", MAX_MSG_LEN - 1);
    // Limpa o buffer
    while(getchar() != '\n'); 
    if (fgets(msg_original, MAX_MSG_LEN, stdin) == NULL) { return 1; }

    // Pré-processamento: Upper-case e remoção de nova linha
    int len_msg = 0;
    for (int i = 0; msg_original[i] != '\0'; i++) {
        if (msg_original[i] == '\n' || msg_original[i] == '\r') {
            msg_original[i] = '\0';
            break;
        }
        msg_original[i] = toupper(msg_original[i]);
        len_msg++;
    }
    printf("\nMensagem Padronizada: \"%s\"\n", msg_original);


    // 3.1 Pré-Codificação (Texto para Blocos Numéricos)
    printf("\n[3.1 PRE-CODIFICACAO: Texto -> Numeros (Blocos de 2 digitos)]\n");
    char codigo_concatenado[MAX_CODIGO_LEN] = ""; 
    ll blocos_M[MAX_MSG_LEN];
    for (int i = 0; i < len_msg; i++) {
        char letra = msg_original[i];
        char* codigo_str = codifica_letra(letra);
        // Assume que cada bloco M é formado por 2 digitos (M < 100)
        blocos_M[i] = (ll)atoi(codigo_str); 
        strcat(codigo_concatenado, codigo_str);
        printf("  %c -> %s (M = %lld)\n", letra, codigo_str, blocos_M[i]);
    }
    printf("Sequencia de Blocos M: %s\n", codigo_concatenado);


    // 3.2 Codificação (Criptografia) C = M^E mod N
    printf("\n[3.2 CODIFICACAO: C = M ^ E mod N]\n");
    ll blocos_C[MAX_MSG_LEN];
    
    for (int i = 0; i < len_msg; i++) {
        printf("\n--- Criptografando Bloco %d (M = %lld) ---\n", i + 1, blocos_M[i]);
        blocos_C[i] = potencia_modular(blocos_M[i], E, N, z);
        printf("  Bloco Cifrado C = %lld\n", blocos_C[i]);
    }

    printf("\n--- RESULTADO DA CRIPTOGRAFIA ---\n");
    printf("  Blocos Cifrados C: ");
    for (int i = 0; i < len_msg; i++) {
        printf("%lld ", blocos_C[i]);
    }
    printf("\n");

    // 3.3 Decodificação (Descriptografia) M_final = C^D mod N
    printf("\n[3.3 DECODIFICACAO: M_final = C ^ D mod N]\n");
    ll blocos_M_final[MAX_MSG_LEN];
    
    for (int i = 0; i < len_msg; i++) {
        printf("\n--- Descriptografando Bloco %d (C = %lld) ---\n", i + 1, blocos_C[i]);
        blocos_M_final[i] = potencia_modular(blocos_C[i], D, N, z);
        printf("  Bloco Decifrado M' = %lld\n", blocos_M_final[i]);
    }


    // 3.4 Pós-Decodificação (Números para Texto)
    printf("\n[3.4 POS-DECODIFICACAO: Numeros -> Texto]\n");
    char msg_decifrada[MAX_MSG_LEN];
    char codigo_decifrado_concatenado[MAX_CODIGO_LEN] = ""; 
    int sucesso = 1;

    for (int i = 0; i < len_msg; i++) {
        char codigo_str[3];
        snprintf(codigo_str, 3, "%02lld", blocos_M_final[i]); // Formata como 2 digitos (ex: 00)
        
        char letra_decodificada = decodifica_codigo(codigo_str);
        msg_decifrada[i] = letra_decodificada;
        strcat(codigo_decifrado_concatenado, codigo_str);

        printf("  M' = %s -> %c\n", codigo_str, letra_decodificada);

        if (letra_decodificada != msg_original[i]) {
            sucesso = 0; // Se houver discrepância
        }
    }
    msg_decifrada[len_msg] = '\0'; // Finaliza a string

    printf("\n------------------------------------------------\n");
    printf("Mensagem Original:     \"%s\"\n", msg_original);
    printf("Mensagem Decifrada:    \"%s\"\n", msg_decifrada);
    printf("------------------------------------------------\n");

    if (sucesso) {
        printf("CONFIRMACAO: A mensagem decifrada e identica a mensagem original.\n");
    } else {
        printf("ERRO: A mensagem decifrada NAO e identica a mensagem original.\n");
    }
    printf("========================================================\n");


    return 0;
}
