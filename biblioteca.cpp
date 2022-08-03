#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

struct Data
{
    int dia;
    int mes;
    int ano;
};

/* Recupera a data/hora atual da máquina */
void data_hora_atual(int &dia, int &mes, int &ano,
                     int &hora, int &min, int &seg)
{
    time_t t = time(NULL);
    struct tm lt = *localtime(&t);

    ano = lt.tm_year + 1900;
    mes = lt.tm_mon + 1;
    dia = lt.tm_mday;
    hora = lt.tm_hour;
    min = lt.tm_min;
    seg = lt.tm_sec;
}

/* Recupera a data atual da máquina */
Data data_atual()
{
    int h, m, s;
    Data dt;

    data_hora_atual(dt.dia, dt.mes, dt.ano, h, m, s);

    return dt;
}

/* Verifica se ano é bissexto */
bool eh_bissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/* Retorna o número de dias de um mês/ano */
int numero_dias_mes(int mes, int ano)
{
    int dias[] = {31, eh_bissexto(ano) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return mes >= 1 && mes <= 12 && ano > 0 ? dias[mes - 1] : -1;
}

/* Verifica se os valores de dia, mês e ano formam uma data válida */
bool eh_data_valida(int dia, int mes, int ano)
{
    return ano > 0 &&
           mes >= 1 && mes <= 12 &&
           dia >= 1 && dia <= numero_dias_mes(mes, ano);
}

/* Verifica se os valores de dia, mês e ano da struct Data formam uma data válida */
bool eh_data_valida(Data dt)
{
    return eh_data_valida(dt.dia, dt.mes, dt.ano);
}

/* Verifica se dt1 é maior que dt2 */
bool maior(Data dt1, Data dt2)
{
    return dt1.ano > dt2.ano ||
           (dt1.ano == dt2.ano && dt1.mes > dt2.mes) ||
           (dt1.ano == dt2.ano && dt1.mes == dt2.mes && dt1.dia > dt2.dia);
}

/* Verifica se dt1 é menor que dt2 */
bool menor(Data dt1, Data dt2)
{
    return dt1.ano < dt2.ano ||
           (dt1.ano == dt2.ano && dt1.mes < dt2.mes) ||
           (dt1.ano == dt2.ano && dt1.mes == dt2.mes && dt1.dia < dt2.dia);
}

/* Verifica se dt1 é igual a dt2 */
bool igual(Data dt1, Data dt2)
{
    return dt1.dia == dt2.dia &&
           dt1.mes == dt2.mes &&
           dt1.ano == dt2.ano;
}

/* Retorna a data dt incrementada em UM dia */
Data incrementa(Data dt)
{
    dt.dia++;
    if (dt.dia > numero_dias_mes(dt.mes, dt.ano))
    {
        dt.dia = 1;
        dt.mes++;
        if (dt.mes > 12)
        {
            dt.mes = 1;
            dt.ano++;
        }
    }

    return dt;
}

/* Retorna a data dt incrementada em N dias */
Data incrementa(Data dt, int n)
{
    for (int i = 0; i < n; i++)
        dt = incrementa(dt);

    return dt;
}

/* Retorna a data dt decrementada em UM dia */
Data decrementa(Data dt)
{
    dt.dia--;
    if (dt.dia < 1)
    {
        dt.mes--;
        if (dt.mes < 1)
        {
            dt.mes = 12;
            dt.ano--;
        }
        dt.dia = numero_dias_mes(dt.mes, dt.ano);
    }

    return dt;
}

/* Retorna a data dt decrementada em N dias */
Data decrementa(Data dt, int n)
{
    for (int i = 0; i < n; i++)
        dt = decrementa(dt);

    return dt;
}

/* Retorna a diferença, em dias, entre dt_ini e dt_fim
   Se dt_ini > dt_fim retorna um valor negativo */
int diferenca(Data dt_ini, Data dt_fim)
{
    int dias;

    if (dt_ini.mes == dt_fim.mes && dt_fim.ano == dt_fim.ano)
        dias = dt_fim.dia - dt_ini.dia;
    else if (dt_fim.ano == dt_fim.ano)
    {
        // Até o fim do mês inicial
        dias = numero_dias_mes(dt_ini.mes, dt_ini.ano) - dt_ini.dia;

        for (int mes = dt_ini.mes + 1; mes < dt_fim.mes; mes++)
            dias += numero_dias_mes(mes, dt_ini.ano);

        // Até o dia do mês final
        dias += dt_fim.dia;
    }
    else
    {
        dias = numero_dias_mes(dt_ini.mes, dt_ini.ano) - dt_ini.dia;

        for (int mes = dt_ini.mes + 1; mes <= 12; mes++)
            dias += numero_dias_mes(mes, dt_ini.ano);

        // Do ano inicial+1 até ano final-1
        for (int ano = dt_ini.ano + 1; ano < dt_fim.ano; ano++)
            dias += eh_bissexto(ano) ? 366 : 365;

        // Início do ano até mês final-1
        for (int mes = 1; mes < dt_fim.mes; mes++)
            dias += numero_dias_mes(mes, dt_fim.ano);

        dias += dt_fim.dia;
    }

    return dias;
}

// "retorna" uma string com caixa alta
void caixaAlta(char string[])
{
    for (int i = 0; string[i] != '\0'; i++)
        string[i] = toupper(string[i]);
}

bool apenasAlfa(char string[])
{
    for (int i = 0; string[i] != '\0'; i++)
        if (isdigit(string[i]))
            return false;
    return true;
}

void leNome(char nome[], bool usuario)
{
    while (true)
    {
        printf("Digite o nome do ");
        if (usuario)
            printf("usuario");
        else
            printf("autor");
        printf(" (entre 4 e 30 caracteres): ");
        char input[1000];
        fgets(input,sizeof(input), stdin);
        input[strlen(input)] = '\0';
        if (apenasAlfa(input) && strlen(input) <= 31 && strlen(input) >= 5)
        {
            input[strlen(input) - 1] = '\0'; // trocando \n por \0
            strcpy(nome, input);
            break;
        }
        puts("O nome nao tem numeros e precisa estar entre 4 e 30 caracteres. Tente novamente");
    }
}

long long leCpf()
{
    while (true)
    {
        printf("\nDigite o CPF: ");
        long long input;
        scanf("%lld", &input);
        if (input <= 99999999999 && input >= 10000000000)
        {
            return input;
            break;
        }
        puts("O CPF contem apenas numeros e precisa ter 11 digitos. Tente novamente");
    }
}

void leTitulo(char nome[])
{
    while (true)
    {
        char input[1000];
        printf("Digite o nome do livro (se o nome do livro tem numeros, escreva-os por extenso): ");
        fgets(input,sizeof(input), stdin);
        input[strlen(input)] = '\0';
        if (apenasAlfa(input) && strlen(input) <= 31 && strlen(input) >= 5)
        {
            input[strlen(input) - 1] = '\0'; // trocando \n por \0
            strcpy(nome, input);
            break;
        }
        puts("O titulo do livro precisa ter, pelo menos, 4 letras e, no maximo, 30.");
    }
}

int leCodigoLivro()
{
    while (true)
    {
        int input;
        printf("\nDigite um codigo de 6 digitos para o livro: ");
        scanf("%d", &input);
        if (input >= 100000 && input <= 999999)
            return input;
        puts("O codigo precisa ser positivo e ter 6 digitos.");
    }
}

int leAnoLivro()
{
    while (true)
    {
        int input;
        printf("Digite o ano de lancamento do livro (1900 a 2050): ");
        scanf("%d", &input);
        if (input >= 1900 && input <= 2050)
            return input;
        puts("O ano de lancamento deve estar entre 1900 e 2050.");
    }
}

struct Livro
{
    int codigo = -1, anoDePublicacao; // útil para a ordenação depois
    char titulo[30], autor[30];
    bool emprestado = false; // naturalmente
    Data emprestimo;
    Data devolucao;
};

Livro cadastraLivro(int codigoInput)
{
    Livro livro;
    char tit[30], aut[30];
    livro.codigo = codigoInput;
    getchar(); // consome enter
    leTitulo(tit);
    caixaAlta(tit);
    strcpy(livro.titulo, tit);
    leNome(aut, false);
    caixaAlta(aut);
    strcpy(livro.autor, aut);
    livro.anoDePublicacao = leAnoLivro();
    livro.emprestado = false; // naturalmente
    // dados do empresimo
    puts("\nLivro cadastrado com sucesso!");
    return livro;
}

struct Usuario
{
    char nome[30];
    long long cpf = -1; // útil para ordenação depois
    int codigo = -1;    // codigo do livro, útil para busca depois
};

Usuario cadastraUsuario(long long cpfInput)
{
    Usuario usuario;
    char nomeTemp[30], cpfTemp[30];
    usuario.cpf = cpfInput;
    getchar(); // consome enter
    leNome(nomeTemp, true);
    caixaAlta(nomeTemp);
    strcpy(usuario.nome, nomeTemp);
    /*
    todos os usuários são criados com um livro que não existe na biblioteca, cujo código é -1
    se pegar um livro emprestado, o livro.codigo terá o código do livro correspondente.
    */
    usuario.codigo = -1;
    puts("\nUsuario cadastrado com sucesso!");
    return usuario;
}

// funções serão definidas depois, mas nome são auto-explicativos
void menuCadastroDeUsuarios(Livro livros[], Usuario usuarios[]), menuCadastroDeLivros(Livro livros[], Usuario usuarios[]),
    menuEmprestimoDevolucao(Livro livros[], Usuario usuarios[]), ordenaLivrosCrescente(Livro livro[]), ordenaLivrosDecrescente(Livro livros[]),
    removeUsuario(Usuario usuarios[], int posicao), removeLivro(Livro livros[], int posicao), listaUsuarios(Usuario usuarios[], Livro livros[]),
    listaLivros(Livro livros[], Usuario usuarios[]), fazEmprestimo(Livro livros[], Usuario usuarios[]),
    fazDevolucao(Livro livros[], Usuario usuarios[], long long cpfInput), ordenaUsuariosCrescente(Usuario usuarios[]), ordenaUsuariosDecrescente(Usuario usuarios[]),
    listaEmprestimos(Livro livros[], Usuario usuarios[]);
int encontraUltimoLivro(Livro livros[]), encontraUltimoUsuario(Usuario usuarios[]), encontraLivro(Livro livros[], int codigoDigitado),
    encontraUsuario(Usuario usuarios[], long long cpfBuscado);

// recebe input ate usuario digitar data correta
Data leDataEntrega(Livro livros[], Usuario usuarios[], long long cpfUsuario)
{
    long input;
    int diaEntrega, mesEntrega, anoEntrega;
    int posicao = encontraUsuario(usuarios, cpfUsuario);
    Data d;
    Usuario usuario = usuarios[posicao];
    while (true)
    {
        printf("Digite a data de entrega no fomato DDMMAAAA: ");
        scanf("%ld", &input);
        if (input > 99999999 || input < 1000000)
            printf("Digite no formato DDMMAAAA\n");
        else
        {
            anoEntrega = input % 10000;
            mesEntrega = ((int)input / 10000) % 100;
            diaEntrega = (int)input / 1000000;
            d.ano = anoEntrega;
            d.mes = mesEntrega;
            d.dia = diaEntrega;
            if (eh_data_valida(d) && (maior(d, livros[encontraLivro(livros, usuario.codigo)].emprestimo) || igual(d, livros[encontraLivro(livros, usuario.codigo)].emprestimo)) &&
                (menor(d, data_atual()) || igual(d, data_atual()))) // válido e maior ou igual à data de retirada e menor ou igual à data atual
                break;
            puts("Data invalida. Tente novamente");
        }
    }
    return d;
}

void menuPrincipal(Livro livros[], Usuario usuarios[])
{
    printf("\nMenu Principal\n1-Cadastro de Usuarios\n2-Cadastro de livros\n3-Emprestimo/devolucao\n4-Fim\n");
    bool dentro;
    do
    {
        dentro = false;
        int input;
        printf("Digite o que voce quer fazer: ");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            menuCadastroDeUsuarios(livros, usuarios);
            break;
        case 2:
            menuCadastroDeLivros(livros, usuarios);
            break;
        case 3:
            menuEmprestimoDevolucao(livros, usuarios);
            break;
        case 4:
            puts("Adeus");
            break;
        default:
            puts("Voce digitou um numero incorreto. Tente novamente.");
            dentro = true;
            break;
        }
    } while (dentro);
}

void menuCadastroDeUsuarios(Livro livros[], Usuario usuarios[])
{
    printf("\nMenu de Cadastro de Usuarios\n1-Cadastrar usuario\n2-Excluir usuario\n3-Listar usuarios\n4-Voltar\n");
    int input;
    int pos;
    long long cpf;
    printf("Digite o que voce quer fazer: ");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        pos = encontraUltimoUsuario(usuarios);
        cpf = leCpf();
        if (encontraUsuario(usuarios, cpf) != -1)
            puts("Usuario ja cadastrado, digite outro CPF");
        else
            usuarios[pos] = cadastraUsuario(cpf);
        menuCadastroDeUsuarios(livros, usuarios);
        break;
    case 2:
        cpf = leCpf();
        pos = encontraUsuario(usuarios, cpf);
        removeUsuario(usuarios, pos);
        menuCadastroDeUsuarios(livros, usuarios);
        break;
    case 3:
        listaUsuarios(usuarios, livros);
        menuCadastroDeUsuarios(livros, usuarios);
        break;
    case 4:
        menuPrincipal(livros, usuarios);
        break;
    default:
        puts("Voce digitou um numero incorreto. Tente novamente.");
        menuCadastroDeUsuarios(livros, usuarios);
        break;
    }
}

void menuCadastroDeLivros(Livro livros[], Usuario usuarios[])
{
    printf("\nMenu de Cadastro de Livros\n1-Cadastrar livro\n2-Excluir livro\n3-Listar livros\n4-Voltar\n");
    int input;
    int pos;
    int codigo;
    printf("Digite o que voce quer fazer: ");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        pos = encontraUltimoLivro(livros);
        codigo = leCodigoLivro();
        if (encontraLivro(livros, codigo) != -1)
        {
            puts("Livro ja cadastrado, digite outro codigo");
        }
        else
            livros[pos] = cadastraLivro(codigo);
        menuCadastroDeLivros(livros, usuarios);
        break;
    case 2:
        codigo = leCodigoLivro();
        pos = encontraLivro(livros, codigo);
        removeLivro(livros, pos);
        menuCadastroDeLivros(livros, usuarios);
        break;
    case 3:
        listaLivros(livros, usuarios);
        menuCadastroDeLivros(livros, usuarios);
        break;
    case 4:
        menuPrincipal(livros, usuarios);
        break;
    default:
        puts("Voce digitou um numero incorreto. Tente novamente.");
        menuCadastroDeLivros(livros, usuarios);
        break;
    }
}

void menuEmprestimoDevolucao(Livro livros[], Usuario usuarios[])
{
    printf("\nMenu do Emprestimo/Devolucao\n1-Emprestar Livro\n2-Devolver livro\n3-Listar emprestimos\n4-Voltar\n");
    int input;
    printf("Digite o que voce quer fazer: ");
    scanf("%d", &input);
    int pos, codigo;
    long long cpf;
    switch (input)
    {
    case 1:
        fazEmprestimo(livros, usuarios);
        menuEmprestimoDevolucao(livros, usuarios);
        break;
    case 2:
        cpf = leCpf();
        fazDevolucao(livros, usuarios, cpf);
        menuEmprestimoDevolucao(livros, usuarios);
        break;
    case 3:
        listaEmprestimos(livros, usuarios);
        menuEmprestimoDevolucao(livros, usuarios);
        break;
    case 4:
        menuPrincipal(livros, usuarios);
        break;
    default:
        puts("Voce digitou um numero incorreto. Tente novamente.");
        menuEmprestimoDevolucao(livros, usuarios);
        break;
    }
}

int encontraUltimoLivro(Livro livros[])
{
    for (int i = 0; i < 100; i++)
        if (livros[i].codigo == -1)
            return i;
}

int encontraLivro(Livro livros[], int codigoDigitado)
{
    for (int i = 0; i < 100; i++)
        if (codigoDigitado == livros[i].codigo)
            return i;
    return -1;
}

void ordenaLivrosDecrescente(Livro livros[])
{
    for (int i = 0; i < 100; i++)
        for (int j = i + 1; j < 100; j++)
        {
            Livro temp;
            if (livros[i].codigo < livros[j].codigo)
            {
                temp = livros[i];
                livros[i] = livros[j];
                livros[j] = temp;
            }
        }
}

void ordenaLivrosCrescente(Livro livros[])
{
    for (int i = 0; i < encontraUltimoLivro(livros); i++)
        for (int j = i + 1; j < encontraUltimoLivro(livros); j++)
        {
            Livro temp;
            if (strcmp(livros[i].titulo, livros[j].titulo) == 1)
            {
                temp = livros[i];
                livros[i] = livros[j];
                livros[j] = temp;
            }
        }
}

void removeLivro(Livro livros[], int posicao)
{
    ordenaLivrosCrescente(livros);
    if (posicao == -1)
        puts("Nao ha este livro no sistema. Tente Outro");
    else if (livros[posicao].emprestado)
        puts("Nao eh possivel remover este livro pois ele esta emprestado");
    else
    {
        livros[posicao].codigo = -1;
        strcpy(livros[posicao].autor, "");
        strcpy(livros[posicao].titulo, "");
        ordenaLivrosCrescente(livros);
        puts("\nLivro removido com sucesso\n");
    }
}

void listaLivros(Livro livros[], Usuario usuarios[])
{
    ordenaLivrosDecrescente(livros);
    ordenaLivrosCrescente(livros);
    Data empresimoLivro, devolucaoLivro;
    puts("------------------------------------------------------------------------");
    puts("Codigo Titulo                         Autor                         Ano");
    puts("------------------------------------------------------------------------");
    for (int i = 0; i < encontraUltimoLivro(livros); i++)
    {
        printf("%d ", livros[i].codigo);
        printf("%s", livros[i].titulo);
        for (int j = 0; j < 31 - strlen(livros[i].titulo); j++)
            printf(" ");
        printf("%s", livros[i].autor);
        for (int j = 0; j < 31 - strlen(livros[i].autor); j++)
            printf(" ");
        printf("%d\n", livros[i].anoDePublicacao);
        for (int j = 0; j < encontraUltimoUsuario(usuarios); j++)
        {
            if (usuarios[j].codigo == livros[i].codigo)
            {
                empresimoLivro = livros[i].emprestimo;
                devolucaoLivro = livros[i].devolucao;
                printf("       ");
                printf("Emprestimo: %d/%d/%d a ", empresimoLivro.dia, empresimoLivro.mes, empresimoLivro.ano);
                printf("%d/%d/%d\n", devolucaoLivro.dia, devolucaoLivro.mes, devolucaoLivro.ano);
                printf("       ");
                printf("CPF: %lld\n", usuarios[j].cpf);
                printf("       ");
                printf("Nome: %s\n", usuarios[j].nome);
            }
        }
    }
    puts("-----------------------------------------------------------------------");
}

void ordenaUsuariosCrescente(Usuario usuarios[])
{
    for (int i = 0; i < encontraUltimoUsuario(usuarios); i++)
        for (int j = i + 1; j < encontraUltimoUsuario(usuarios); j++)
        {
            Usuario temp;
            if (usuarios[i].cpf > usuarios[j].cpf)
            {
                temp = usuarios[i];
                usuarios[i] = usuarios[j];
                usuarios[j] = temp;
            }
        }
}

void ordenaUsuariosDecrescente(Usuario usuarios[])
{
    for (int i = 0; i < 100; i++)
        for (int j = i + 1; j < 100; j++)
        {
            Usuario temp;
            if (usuarios[i].cpf < usuarios[j].cpf)
            {
                temp = usuarios[i];
                usuarios[i] = usuarios[j];
                usuarios[j] = temp;
            }
        }
}

int encontraUltimoUsuario(Usuario usuarios[])
{
    for (int i = 0; i < 100; i++)
        if (usuarios[i].cpf == -1)
            return i;
}

int encontraUsuario(Usuario usuarios[], long long cpfBuscado)
{
    for (int i = 0; i < 100; i++)
        if (cpfBuscado == usuarios[i].cpf)
            return i;
    return -1;
}

void removeUsuario(Usuario usuarios[], int posicao)
{
    if (posicao == -1)
        puts("Nao ha este usuario no sistema. Tente Outro");
    else if (usuarios[posicao].codigo != -1)
        puts("Nao eh possivel remover este usuario pois ele tem um livro emprestado");
    else
    {
        usuarios[posicao].cpf = -1;
        strcpy(usuarios[posicao].nome, "");
        ordenaUsuariosDecrescente(usuarios);
        ordenaUsuariosCrescente(usuarios);
        puts("\nUsuario removido com sucesso");
    }
}

void listaUsuarios(Usuario usuarios[], Livro livros[])
{
    int posicaoLivro;
    ordenaUsuariosDecrescente(usuarios); // just in case
    ordenaUsuariosCrescente(usuarios);
    puts("--------------------------------------------------");
    puts("CPF         Nome");
    puts("--------------------------------------------------");
    for (int i = 0; i < encontraUltimoUsuario(usuarios); i++)
    {
        printf("%lld %s\n", usuarios[i].cpf, usuarios[i].nome);
        if (usuarios[i].codigo != -1)
        {
            posicaoLivro = encontraLivro(livros, usuarios[i].codigo);
            printf("            ");
            printf("Emprestimo: %d/%d/%d a ", livros[posicaoLivro].emprestimo.dia, livros[posicaoLivro].emprestimo.mes, livros[posicaoLivro].emprestimo.ano);
            printf("%d/%d/%d\n", livros[posicaoLivro].devolucao.dia, livros[posicaoLivro].devolucao.mes, livros[posicaoLivro].devolucao.ano);
            printf("            ");
            printf("Titulo: %s\n", livros[posicaoLivro].titulo);
            printf("            ");
            printf("Autor: %s\n", livros[posicaoLivro].autor);
        }
    }
    puts("--------------------------------------------------");
}

void fazEmprestimo(Livro livros[], Usuario usuarios[])
{
    long long cpfInput = leCpf();
    if (encontraUsuario(usuarios, cpfInput) == -1)
    {
        puts("Erro: usuario nao cadastrado");
        return;
    }
    else if (usuarios[encontraUsuario(usuarios, cpfInput)].codigo != -1)
    {
        puts("Erro: este usuario ja tem um emprestimo.");
        return;
    }
    int codigoInput = leCodigoLivro();
    int posicaoLivro = encontraLivro(livros, codigoInput);
    int posicaoUsuario = encontraUsuario(usuarios, cpfInput);
    Data dataEntrega = incrementa(data_atual(), 7);
    if (posicaoLivro == -1)
        puts("Erro: livro nao encontrado. Cadastre-o no sistema ou digite o codigo corretamente");
    else if (livros[posicaoLivro].emprestado)
        puts("Erro: livro ja emprestado. Tente outro livro");
    else
    {
        livros[posicaoLivro].emprestado = true;
        livros[posicaoLivro].emprestimo = data_atual();
        livros[posicaoLivro].devolucao = dataEntrega;
        usuarios[posicaoUsuario].codigo = codigoInput;
        printf("%d\n", livros[posicaoLivro].devolucao.ano);
        usuarios[posicaoUsuario].codigo = codigoInput;
        puts("\nEmprestimo concedido!");
        printf("Nome: %s\nTitulo: %s\n", usuarios[posicaoUsuario].nome, livros[posicaoLivro].titulo);
        printf("Data do emprestimo: %d/%d/%d\n", livros[posicaoLivro].emprestimo.dia, livros[posicaoLivro].emprestimo.mes, livros[posicaoLivro].emprestimo.ano);
        printf("Data da devolucao: %d/%d/%d\n", livros[posicaoLivro].devolucao.dia, livros[posicaoLivro].devolucao.mes, livros[posicaoLivro].devolucao.ano);
    }
}

void fazDevolucao(Livro livros[], Usuario usuarios[], long long cpfInput)
{
    int posicaoLivro, posicaoUsuario = encontraUsuario(usuarios, cpfInput);
    if (posicaoUsuario == -1)
        puts("\nErro: usuario nao cadastrado. Tente novamente");
    else if (usuarios[posicaoUsuario].codigo == -1)
        puts("\nEste usuario nao tem emprestimos. Tente novamente");
    else
    {
        Data data;
        data = leDataEntrega(livros, usuarios, cpfInput);
        posicaoLivro = encontraLivro(livros, usuarios[posicaoUsuario].codigo);
        livros[posicaoLivro].emprestado = false;
        usuarios[posicaoUsuario].codigo = -1; // muito útil
        printf("\nDevolucao realizada com sucesso ");
        if (diferenca(data, livros[posicaoLivro].devolucao) >= 0)
            printf("dentro do prazo!\n");
        else
            printf("com atraso de %d dia(s)!\n", diferenca(livros[posicaoLivro].devolucao, data));
    }
}

void listaEmprestimos(Livro livros[], Usuario usuarios[])
{
    int posicaoLivro;
    ordenaUsuariosCrescente(usuarios);
    puts("--------------------------------------------------------------------------------------------------------------------------");
    printf("CPF         ");
    printf("Nome                          ");
    printf("Codigo");
    printf(" Titulo                        ");
    printf("Emprestimo ");
    printf("Devolucao\n");
    puts("--------------------------------------------------------------------------------------------------------------------------");
    for (int i = 0; i < encontraUltimoUsuario(usuarios); i++)
    {
        posicaoLivro = encontraLivro(livros, usuarios[i].codigo);
        Data emprestimoData, devolucaoData;
        if (livros[posicaoLivro].emprestado)
        {
            emprestimoData = livros[posicaoLivro].emprestimo;
            devolucaoData = livros[posicaoLivro].devolucao;
            printf("%lld", usuarios[i].cpf);
            printf(" %s", usuarios[i].nome);
            for (int j = 0; j < 30 - strlen(usuarios[i].nome); j++)
                printf(" ");
            printf("%d", livros[posicaoLivro].codigo);
            printf(" %s", livros[posicaoLivro].titulo);
            for (int j = 0; j < 30 - strlen(livros[posicaoLivro].titulo); j++)
                printf(" ");
            printf("%d/%d/%d  ", emprestimoData.dia, emprestimoData.mes, emprestimoData.ano);
            printf("%d/%d/%d", devolucaoData.dia, devolucaoData.mes, devolucaoData.ano);
            printf("\n");
        }
    }
    puts("--------------------------------------------------------------------------------------------------------------------------");
}

int main()
{
    Usuario usuarios[100];
    Livro livros[100];
    menuPrincipal(livros, usuarios);

    return 0;
}