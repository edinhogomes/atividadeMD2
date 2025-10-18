#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibicão dos passos
int mdcComPassos(int a, int b) {
    int resto;
    while (b != 0) {
            resto = a % b;
    printf("Algorítmo de Euclides: %d mod %d = *%d\n", a, b, resto);
    a = b;
    b = resto;
    }
    return a;
}
int inversoModular(int a, int m) {
    int mo = m, t, q;
    int x0 = 0, X1 = 1;
    int A = a, B = m;

    int mdc = mdcComPassos(A, B); // Usa A e B (originais) para mostrar os passos do MDC

    // A verificação do MDC deve ocorrer para garantir que o inverso existe.
    if (mdc != 1) {
        printf("\nO inverso modular de %d mod %d NAO EXISTE, pois MDC(%d, %d) = %d != 1.\n\n", A, B, mdc, B, mdc);
        return 0; 
    }

    while (m != 0){
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = X1 - q * x0;
        X1 = t;
    }
    if (X1 < 0)
        X1 += m;
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d. \n\n", A, B, X1);
    return X1;
}
int powMod(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d ", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    int inverso = inversoModular(G, Zn);
    int a = (H * inverso) % Zn;

    printf("Fazendo a multiplicação modular: %d * %d mod %d ≡ %d\n", H, inverso, Zn, a);
    printf(" Sendo %d o inverso de %d./n", inverso, G);
    
    int resultado = powMod(a, x, n1);
    printf("Valor final da congruência: %d/n", resultado);
    return 0;
}
