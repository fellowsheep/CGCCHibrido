int numero = 1, total = 0, positivos = 0, negativos = 0, divisiveisPor2 = 0, divisiveisPor5 = 0;

while (numero != 0)
{
    printf("Digite um número (0 para encerrar): ");
    scanf("%d", &numero);

    if (numero != 0)
    {
        total++;

        if (numero > 0)
        {
            positivos++;
        }
        else if (numero < 0)
        {
            negativos++;
        }

        if (numero % 2 == 0)
        {
            divisiveisPor2++;
        }

        if (numero % 5 == 0)
        {
            divisiveisPor5++;
        }
    }
}

if (total > 0)
{
    printf("Porcentagem de positivos: %.2f%%\n", positivos / (float)total * 100);
    printf("Porcentagem de negativos: %.2f%%\n", negativos / (float)total * 100);
    printf("Porcentagem de divisíveis por 2: %.2f%%\n", divisiveisPor2 / (float)total * 100);
    printf("Porcentagem de divisíveis por 5: %.2f%%\n", divisiveisPor5 / (float)total * 100);
}
else
{
    printf("Nenhum número foi digitado.\n");
}

int a = rand() % (670 - 130 + 1) + 130;
int b = rand() % (-5 - (-20) + 1) - 20;
int c = rand() % (10 - (-10) + 1) - 10;

int e = (rand() % 21) * 5;
char f = rand() % ('Z' - 'A' + 1) + 'A';

// Multiplicando os limites por 10 para trabalhar com inteiros
int minimo = 105; // 10.5 * 10
int maximo = 502; // 50.2 * 10

// Sorteando um número inteiro entre 105 e 502
int numeroSorteado = rand() % (maximo - minimo + 1) + minimo;

// Dividindo por 10 para obter o valor com uma casa decimal
float resultado = numeroSorteado / 10.0;

// Explicação:
// O caractere 'A' tem valor ASCII 65, e 'Z' tem 90.
// A fórmula rand() % (90 - 65 + 1) + 65 sorteia um valor numérico no intervalo de ASCII para 'A' a 'Z'
// Depois, esse valor é convertido de número para caractere pelo printf.
char f = rand() % ('Z' - 'A' + 1) + 'A';
printf("f. Caractere entre 'A' e 'Z': %c\n", f);

// Existem 21 múltiplos de 5 entre 0 e 100 (inclusive).
// Sorteamos um número entre 0 e 20 com rand() % 21, e depois multiplicamos por 5.
// Isso garante que o valor sorteado será um múltiplo de 5.
int e = (rand() % 21) * 5;

int meses;
float populacao;

printf("Digite a população inicial da colmeia: ");
scanf("%f", &populacao);
printf("Digite o número de meses: ");
scanf("%d", &meses);

for (int i = 1; i <= meses; i++)
{
    // Dobrar a população
    populacao = populacao * 2;

    // Subtrair os 3% de abelhas migradas
    populacao = populacao - (populacao * 0.03);

    printf("Mês %d: População = %.2f\n", i, populacao);
}

float umidade;

// Processa um sensor de cada vez
for (int i = 1; i <= 5; i++)
{
    printf("Digite a umidade do solo para o sensor %d: ", i);
    scanf("%f", &umidade);

    // Verifica se a umidade está abaixo de 30%
    if (umidade < 30.0)
    {
        printf("Irrigação ativada pelo sensor %d (Umidade: %.2f%%)\n", i, umidade);
    }
}

int numero, palpite;

srand(time(NULL));
numero = rand() % 100 + 1;

do
{
    printf("Tente adivinhar o número (entre 1 e 100): ");
    scanf("%d", &palpite);

    if (palpite > numero)
    {
        printf("Muito alto!\n");
    }
    else if (palpite < numero)
    {
        printf("Muito baixo!\n");
    }
    else
    {
        printf("Correto! O número era %d.\n", numero);
    }
} while (palpite != numero);

char caractere;

for (int i = 0; i < 10; i++)
{
    printf("Digite um caractere: ");
    scanf(" %c", &caractere);

    if (isalpha(caractere))
    {
        printf("É uma letra.\n");
    }
    else if (isdigit(caractere))
    {
        printf("É um número.\n");
    }
    else
    {
        printf("É um símbolo especial.\n");
    }
}

float tempo, total = 0, maior = 0;
int diaMaior = 0;

for (int i = 1; i <= 7; i++)
{
    printf("Digite o tempo de exercício no dia %d (minutos): ", i);
    scanf("%f", &tempo);

    // Acumula o tempo total
    total += tempo;

    // Verifica se este é o maior tempo
    if (tempo > maior)
    {
        maior = tempo;
        diaMaior = i;
    }
}

// Calcula e exibe os resultados
printf("Total de tempo exercitado na semana: %.2f minutos\n", total);
printf("Média diária: %.2f minutos\n", total / 7);
printf("Dia com maior tempo de exercício: Dia %d (%.2f minutos)\n", diaMaior, maior);

float chuva, total = 0, maior = 0;
int diaMaior = 0;

for (int i = 1; i <= 7; i++)
{
    printf("Digite a quantidade de chuva no dia %d (mm): ", i);
    scanf("%f", &chuva);

    // Acumula o total de chuva
    total += chuva;

    // Verifica se é o maior valor de chuva
    if (chuva > maior)
    {
        maior = chuva;
        diaMaior = i;
    }

    // Alerta de seca se a chuva for menor que 5 mm
    if (chuva < 5.0)
    {
        printf("Alerta de seca no dia %d: %.2f mm de chuva\n", i, chuva);
    }
}

// Exibe os resultados
printf("Total de chuva acumulada: %.2f mm\n", total);
printf("Dia com maior quantidade de chuva: Dia %d (%.2f mm)\n", diaMaior, maior);
