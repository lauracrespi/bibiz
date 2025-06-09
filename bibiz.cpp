#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_BIBLIOS = 100;
const int MAX_PALAVRAS = 10;

struct informacoesBibliografia {
    string titulo;
    string autor;
    string tipo;
    string nivel;
    string palavrasChave[MAX_PALAVRAS];
    int qtdPalavras;
};

informacoesBibliografia banco[MAX_BIBLIOS];
int total = 0;

void separarPalavras(string entrada, string palavras[], int &qtd) {
    string palavra = "";
    qtd = 0;
    for (int i = 0; i < (int)entrada.length(); i++) {
        char c = entrada[i];
        if (c == ',') {

            int start = 0;
            int end = (int)palavra.length() - 1;
            while (start <= end && palavra[start] == ' ') start++;
            while (end >= start && palavra[end] == ' ') end--;
            if (start <= end) {
                palavras[qtd] = palavra.substr(start, end - start + 1);
                qtd++;
            }
            palavra = "";
        } else {
            palavra += c;
        }
    }

    int start = 0;
    int end = (int)palavra.length() - 1;
    while (start <= end && palavra[start] == ' ') start++;
    while (end >= start && palavra[end] == ' ') end--;
    if (start <= end) {
        palavras[qtd] = palavra.substr(start, end - start + 1);
        qtd++;
    }
}

void salvarBanco() {
    ofstream arq("bibliografias.txt");
    if (!arq) {
        cout << "Erro ao abrir arquivo para salvar.\n";
        return;
    }
    arq << total << "\n";
    for (int i = 0; i < total; i++) {
        arq << banco[i].titulo << "\n"
            << banco[i].autor << "\n"
            << banco[i].tipo << "\n"
            << banco[i].nivel << "\n"
            << banco[i].qtdPalavras << "\n";
        for (int j = 0; j < banco[i].qtdPalavras; j++) {
            arq << banco[i].palavrasChave[j];
            if (j < banco[i].qtdPalavras - 1) arq << ",";
        }
        arq << "\n";
    }
    arq.close();
}

void carregarBanco() {
    ifstream arq("bibliografias.txt");
    if (!arq) {
        total = 0;
        return;
    }
    arq >> total;
    arq.ignore();
    for (int i = 0; i < total; i++) {
        getline(arq, banco[i].titulo);
        getline(arq, banco[i].autor);
        getline(arq, banco[i].tipo);
        getline(arq, banco[i].nivel);
        arq >> banco[i].qtdPalavras;
        arq.ignore();
        string linhaPalavras;
        getline(arq, linhaPalavras);
        separarPalavras(linhaPalavras, banco[i].palavrasChave, banco[i].qtdPalavras);
    }
    arq.close();
}

string escolherOpcao(string opcoes[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "[" << i + 1 << "] " << opcoes[i] << "\n";
    }
    cout << "\n[0] Voltar\n";
    int escolha;
    cout << "Selecione uma opcao: ";
    cin >> escolha;
    while (escolha < 0 || escolha > n) {
        cout << "Opcao invalida. Tente novamente: ";
        cin >> escolha;
    }
    if (escolha == 0) return "";
    return opcoes[escolha - 1];
}

void adicionarBibliografia() {

    if (total >= MAX_BIBLIOS) {
        cout << "AVISO: Limite de armazenamento de bibliografias atingido.\n";
        return;
    }
    cin.ignore();
    informacoesBibliografia novaBibliografia;
    cout << "Adicionando nova bibliografia...\n\n";
    cout << "Titulo: ";
    getline(cin, novaBibliografia.titulo);
    if (novaBibliografia.titulo == "") {
        cout << "\nProcedimento cancelado. Retornando ao MENU PRINCIPAL...\n";
        return;
    }

    cout << "Autor: ";
    getline(cin, novaBibliografia.autor);
    if (novaBibliografia.autor == "") {
        cout << "\nProcedimento cancelado. Retornando ao MENU PRINCIPAL...\n";
        return;
    }

    string opcoesTipo[] = {"Livro", "Artigo", "Video", "Outro"};
    cout << "Tipo:\n";
    novaBibliografia.tipo = escolherOpcao(opcoesTipo, 4);
    if (novaBibliografia.tipo == "") {
        cout << "\nProcedimento cancelado. Retornando ao MENU PRINCIPAL...\n";
        return;
    }

    string opcoesNivel[] = {"Iniciante", "Intermediario", "Avancado"};
    cout << "Nivel:\n";
    novaBibliografia.nivel = escolherOpcao(opcoesNivel, 3);
    if (novaBibliografia.nivel == "") {
        cout << "\nProcedimento cancelado. Retornando ao MENU PRINCIPAL...\n";
        return;
    }

    string entradaPalavrasChave;
    cout << "Palavras-chave (separadas por virgula): ";
    cin.ignore();
    getline(cin, entradaPalavrasChave);
    cout << entradaPalavrasChave;
    if (entradaPalavrasChave.empty() == true) {
        cout << "\nProcedimento cancelado. Retornando ao MENU PRINCIPAL...\n";
        return;
    }
    separarPalavras(entradaPalavrasChave, novaBibliografia.palavrasChave, novaBibliografia.qtdPalavras);

    banco[total] = novaBibliografia;
    total++;
    salvarBanco();
    cout << "\n\nBibliografia adicionada com sucesso!\n";
}

void listar() {
    if (total == 0) {
        cout << "Nenhuma bibliografia cadastrada\n";
        return;
    }
    for (int i = 0; i < total; i++) {
        cout << "\nTitulo: " << banco[i].titulo
             << "\nAutor: " << banco[i].autor
             << "\nTipo: " << banco[i].tipo
             << "\nNivel: " << banco[i].nivel
             << "\nPalavras-chave: ";
        for (int j = 0; j < banco[i].qtdPalavras; j++) {
            cout << banco[i].palavrasChave[j];
            if (j < banco[i].qtdPalavras - 1) cout << ", ";
        }
        cout << "\n";
    }
}

bool contemPalavra(string palavra, string lista[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (palavra == lista[i]) return true;
    }
    return false;
}

bool correspondeFiltro(informacoesBibliografia b, string titulo, string autor, string tipo, string nivel, string palavras[], int qtdPalavras) {
    if (titulo != "" && b.titulo != titulo) return false;
    if (autor != "" && b.autor != autor) return false;
    if (tipo != "" && b.tipo != tipo) return false;
    if (nivel != "" && b.nivel != nivel) return false;
    for (int i = 0; i < qtdPalavras; i++) {
        if (!contemPalavra(palavras[i], b.palavrasChave, b.qtdPalavras)) return false;
    }
    return true;
}


void buscarMenu() {
    string filtroTitulo = "";
    string filtroAutor = "";
    string filtroTipo = "";
    string filtroNivel = "";
    string filtroPalavras[MAX_PALAVRAS];
    int qtdFiltroPalavras = 0;

    while (true) {
        cout << "\n====== MENU DE BUSCA ======\n";
        cout << "Filtros atuais:\n";
        cout << "Titulo: " << (filtroTitulo == "" ? "[nenhum]" : filtroTitulo) << "\n";
        cout << "Autor: " << (filtroAutor == "" ? "[nenhum]" : filtroAutor) << "\n";
        cout << "Tipo: " << (filtroTipo == "" ? "[nenhum]" : filtroTipo) << "\n";
        cout << "Nivel: " << (filtroNivel == "" ? "[nenhum]" : filtroNivel) << "\n";
        cout << "Palavras-chave: ";
        if (qtdFiltroPalavras == 0) cout << "[nenhum]";
        else {
            for (int i = 0; i < qtdFiltroPalavras; i++) {
                cout << filtroPalavras[i];
                if (i < qtdFiltroPalavras - 1) cout << ", ";
            }
        }
        cout << "\n\n";

        cout << "[1] Filtrar por Titulo\n";
        cout << "[2] Filtrar por Autor\n";
        cout << "[3] Filtrar por Tipo\n";
        cout << "[4] Filtrar por Nivel\n";
        cout << "[5] Filtrar por Palavras-chave\n";
        cout << "[6] Mostrar resultados\n";
        cout << "[7] Limpar todos os filtros\n";
        cout << "\n[0] Voltar\n";

        cout << "\nSelecione uma opcao: ";
        int opcao;
        cin >> opcao;
        cin.ignore();

        if (opcao == 0) break;

        if (opcao == 1) {
            cout << "Digite o titulo: ";
            getline(cin, filtroTitulo);
        } else if (opcao == 2) {
            cout << "Digite o autor: ";
            getline(cin, filtroAutor);
        } else if (opcao == 3) {
            string opcoesTipo[] = {"Livro", "Artigo", "Video", "Outro"};
            cout << "Selecione o tipo:\n";
            string selecionado = escolherOpcao(opcoesTipo, 4);
            filtroTipo = selecionado;
        } else if (opcao == 4) {
            string opcoesNivel[] = {"Iniciante", "Intermediario", "Avancado"};
            cout << "Selecione o nivel:\n";
            string selecionado = escolherOpcao(opcoesNivel, 3);
            filtroNivel = selecionado;
        } else if (opcao == 5) {
            cout << "Digite palavras-chave (separadas por virgula): ";
            string entrada;
            getline(cin, entrada);
            if (entrada == "") {
                qtdFiltroPalavras = 0;
            } else {
                separarPalavras(entrada, filtroPalavras, qtdFiltroPalavras);
            }
        } else if (opcao == 6) {
            int count = 0;
            for (int i = 0; i < total; i++) {
                if (correspondeFiltro(banco[i], filtroTitulo, filtroAutor, filtroTipo, filtroNivel, filtroPalavras, qtdFiltroPalavras)) {

                    cout << "\nResultados encontrados:\n";
                    cout << "\nTitulo: " << banco[i].titulo
                         << "\nAutor: " << banco[i].autor
                         << "\nTipo: " << banco[i].tipo
                         << "\nNivel: " << banco[i].nivel
                         << "\nPalavras-chave: ";
                    for (int j = 0; j < banco[i].qtdPalavras; j++) {
                        cout << banco[i].palavrasChave[j];
                        if (j < banco[i].qtdPalavras - 1) cout << ", ";
                    }
                    cout << "\n";
                    count++;
                }
            }
            if (count == 0) {
                cout << "Nenhum resultado encontrado.\n";
            } else {
                cout << "\nTotal de resultados: " << count << "\n";
            }
        } else if (opcao == 7) {
            filtroTitulo = "";
            filtroAutor = "";
            filtroTipo = "";
            filtroNivel = "";
            qtdFiltroPalavras = 0;
            cout << "Filtros limpos.\n";
        } else {
            cout << "AVISO: Opcao invalida\n";
        }
    }
}

int main() {
    cout << "\n====================== BIBIZ ======================\n";

    carregarBanco();

    while (true) {
        cout << "\n====== MENU PRINCIPAL ======\n";
        cout << "[1] Adicionar bibliografia\n";
        cout << "[2] Buscar bibliografia\n";
        cout << "[3] Listar todas as bibliografias\n";
        cout << "[0] Sair\n";
        cout << "\nSelecione uma opcao: ";
        int opcao;
        cin >> opcao;
        cout << "\n";

        if (opcao == 0) break;

        if (opcao == 1) {
            adicionarBibliografia();
        } else if (opcao == 2) {
            buscarMenu();
        } else if (opcao == 3) {
            listar();
        } else {
            cout << "AVISO: Opcao invalida\n";
        }
    }

    cout << "Programa finalizado!\n";
    return 0;
}
