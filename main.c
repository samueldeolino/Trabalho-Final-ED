#include <stdio.h>
#include <stdlib.h>

typedef struct disciplina{
    int codigo;
    float notaFinal;
    char nomeDisciplina[50];
}disciplina;

typedef struct aluno{
    char * nome;
    char * email;
    int matricula;
    int qtdDis;
    char * telefone;
    disciplina ** subject;
    struct aluno *right;
    struct aluno *left;
}aluno;

aluno * raiz_arv = NULL;

aluno * busca(aluno * raiz, int matricula);
void menu_principal();
void menu_cadastrar();
void menu_remover();
void menu_alterar();
void menu_buscar();
disciplina ** ordenado(disciplina ** disc, int tam);
void addDisciplina(int matricula, int tamanho);
void flush_in();

void imprimirAluno(int matricula){
    aluno * AlunoImprimido = busca(raiz_arv, matricula);
    disciplina ** vet;
    if(AlunoImprimido == NULL){
        printf("\nAluno nao existe!\n");
        return;
    }
    if(AlunoImprimido->matricula == matricula){
        printf("\nAluno: %s\n", AlunoImprimido->nome);
        printf("Email: %s\n", AlunoImprimido->email);
        printf("Telefone: %s\n", AlunoImprimido->telefone);
        vet = ordenado(AlunoImprimido->subject, AlunoImprimido->qtdDis);
        printf("\nDisciplinas:\n");
        for (int i = 0; i < AlunoImprimido->qtdDis; i++){
            printf("\n");
            printf("%s", vet[i]->nomeDisciplina);
            printf("Codigo: %d\n", vet[i]->codigo);
            printf("Nota final: %.2f\n", vet[i]->notaFinal);
        }
        
    }else{
        printf("\nAluno nao existe!\n");
    }
}

disciplina ** ordenado(disciplina ** disc, int tam){ 
    disciplina * aux;
    for(int j=0; j<tam; j++){ 
        for(int i=0; i<tam; i++){
            if(disc[j]->notaFinal > disc[i]->notaFinal){
                aux = disc[j];
                disc[j] = disc[i];
                disc[i] = aux;
            }
        } 
    }
    
    return disc;
}

aluno * busca(aluno *raiz, int matricula){
    if(raiz == NULL){
        return NULL;
    }
    else if(matricula == raiz->matricula){
        return raiz;
    }
    else if(matricula < raiz->matricula){
        if(raiz->left != NULL)
            return busca(raiz->left, matricula);
        else{
            return raiz;
        }
    }
    else{
        if(raiz->right != NULL){
            return busca(raiz->right, matricula);
        }else{
            return raiz;
        }
    }
}

int add(int matricula, char * nome, char * email, char * telefone){
    aluno* resp = busca(raiz_arv, matricula);
    if(resp == NULL || resp->matricula != matricula){
        aluno* novo = malloc (sizeof(aluno));
        novo->matricula = matricula;
        novo->nome = nome;
        novo->email = email;
        novo->telefone = telefone;
        novo->left = NULL;
        novo->right = NULL;

        if(resp == NULL){
            raiz_arv = novo; 
        }else{
            if(matricula < resp->matricula){
                resp->left = novo;
            }else{
                resp->right = novo;
            }
        }
        int tam;
        printf("\nQuantas disciplinas serao acrescentadas? ");
        scanf("%d", &tam);
        novo->qtdDis = tam;
        flush_in();
        addDisciplina(matricula, tam);
        return 1;
    }else{
        printf("\nAluno nao pode ser cadastrado!\nMatricula ja existente!\n");
        return 0;
    }    
}

void flush_in() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

void addDisciplina(int matricula, int tamanho){
    aluno * Aluno = busca(raiz_arv, matricula);
    Aluno->qtdDis = tamanho;
    Aluno->subject = malloc(sizeof(disciplina*)*tamanho);
    for(int j = 0; j<tamanho; j++){
        Aluno->subject[j] = malloc(sizeof(disciplina));
    }

    if(Aluno->matricula == matricula){
        int codigo;
        float notaF;

        for(int i = 0; i<tamanho; i++){
            printf("\nNome da disciplina %d: \n", i+1);
            fgets(Aluno->subject[i]->nomeDisciplina, 50, stdin);
            printf("\nCodigo da disciplina %d: \n", i+1);
            scanf("%d", &codigo);
            Aluno->subject[i]->codigo = codigo;
            printf("\nNota da disciplina %d: \n", i+1);
            scanf("%f", &notaF);
            Aluno->subject[i]->notaFinal = notaF;
            printf("\nDisciplina cadastrada com sucesso!\n\n");
            flush_in();
        }
        printf("\n-_-_--_-_--_-_--_-_--_-_--_-_--_-_--_-_-\n\n");
        
    }else{
        printf("Aluno não cadastrado.\n");
    }

}

aluno *remover(aluno *raiz, int chave){

    if(raiz == NULL){
        printf("Valor nao encontrado");
        return NULL;
    }else{
        if(raiz->matricula == chave){
            if(raiz->left == NULL && raiz->right == NULL){
                for (int i = 0; i < raiz->qtdDis; i++){
                    free(raiz->subject[i]);
                }
                free(raiz->subject);
                free(raiz);
                return NULL;
            }else{
                if(raiz->left == NULL || raiz->right == NULL){
                    aluno *aux;
                    if(raiz->left != NULL){
                        aux = raiz->left;
                    }else{
                        aux = raiz->right;
                    }
                    for (int i = 0; i < raiz->qtdDis; i++){
                        free(raiz->subject[i]);
                    }
                    free(raiz->subject);
                    free(raiz);
                    return aux;
                }else{
                    aluno *aux = raiz->left;
                    aluno aux2;

                    aux2.matricula = raiz->matricula;
                    aux2.email = raiz->email;
                    aux2.nome = raiz->nome;
                    aux2.qtdDis = raiz->qtdDis;
                    aux2.telefone = raiz->telefone;
                    aux2.subject = raiz->subject;

                    while(aux->right != NULL){
                        aux = aux->right;
                    }

                    raiz->matricula = aux->matricula;
                    raiz->email = aux->email;
                    raiz->nome = aux->nome;
                    raiz->qtdDis = aux->qtdDis;
                    raiz->telefone = aux->telefone;
                    raiz->subject = aux->subject;

                    aux->matricula = aux2.matricula;
                    aux->email = aux2.email;
                    aux->nome = aux2.nome;
                    aux->qtdDis = aux2.qtdDis;
                    aux->telefone = aux2.telefone;
                    aux->subject = aux2.subject;

                    raiz->left = remover(raiz->left, chave);
                    return raiz;
                }
            }
        }else{
            if(chave < raiz->matricula){
                raiz->left = remover(raiz->left, chave);
            }else{
                raiz->right = remover(raiz->right, chave);
            }
            return raiz;
        }

    }
}

void modificar(int matricula){
    aluno * AlunoModificado = busca(raiz_arv, matricula);
    int op;
    char nome[100];
    char email[100];
    char telefone[50];
    if(AlunoModificado == NULL){
        printf("\nAluno Nao existe!");
        menu_principal();
    }else if(matricula != AlunoModificado->matricula){
        printf("\nAluno Nao existe!");
        menu_principal();
    }
    do{
        printf("\nNome: %s\nEmail: %s\nTelefone: %s", AlunoModificado->nome, AlunoModificado->email, AlunoModificado->telefone);
        printf("\n0 - Sair\n1 - Modificar nome\n2 - Modificar email\n3 - Modificar telefone\n");
        printf("Digite uma opcao: ");
        scanf("%d", &op);
    switch (op){
        case 0:
            menu_principal();
            break;
        case 1:
            printf("Informe novo nome: ");
            scanf(" %[^\n]", nome);
            AlunoModificado->nome = nome;
            break;
        case 2:
            printf("Informe novo email: ");
            scanf(" %[^\n]", email);
            AlunoModificado->email = email;
            break;
        case 3:
            printf("Informe novo telefone: ");
            scanf(" %[^\n]", &telefone);
            AlunoModificado->telefone = telefone;
            break;
        default:
            break;
        }
        flush_in();
    } while (op != 0);
}

void menu_principal(){
    int op;
    printf("\n");
    printf("\n1- Adicionar um Aluno\n2- Remover um Aluno\n3- Alterar dados de aluno\n4- Buscar dados de Aluno\n0- Fechar Programa\n");
    printf("selecione uma opcao: ");
    scanf("%d", &op);
    switch (op){
    case 0:
        exit(1);
        break;
    case 1:
        menu_cadastrar();
        break;
    case 2:
        menu_remover();
        break;
    case 3:
        menu_alterar();
        break;
    case 4:
        menu_buscar();
        break;
    default:
        printf("Opcao invalida!");
        menu_principal();
        break;
    }
}

void menu_cadastrar(){
    char nome[100];
    char email[100];
    int matricula;
    char telefone[50];

    printf("\n----Adicionar Aluno----\n");
    printf("\nDigite a Matricula: ");
    scanf("%d", &matricula);
    printf("Digite o nome do Aluno:");
    scanf(" %[^\n]", nome);
    printf("Digite o email do Aluno:");
    scanf(" %[^\n]", email);
    printf("Digite o telefone do Aluno:");
    scanf(" %[^\n]", &telefone);

    int status = add(matricula, nome, email, telefone);
    if(status == 0){
        menu_principal();
    }else{
        printf("\nAluno cadastrado com sucesso!\n");
        menu_principal();
    }
}

void menu_remover(){
    int matricula;
    int op;
    printf("\nDigite a Matricula do Aluno a ser removido: ");
    scanf("%d", &matricula);
    aluno* alun = busca(raiz_arv, matricula);
    if(alun == NULL){
        printf("Aluno nao encontrado!");
        menu_principal();
    }else if(alun->matricula == matricula){
        printf("\nTem certeza que deseja remover o(a) %s?\n", alun->nome );
        printf("1- Sim\n2- Nao\n");
        printf("Digite uma opcao: ");
        scanf("%d", &op);
        switch (op){
        case 1:
            raiz_arv = remover(raiz_arv, matricula);
            printf("\nAluno removido com sucesso!\n");
            menu_principal();
            break;
        case 2:
            menu_principal();
            break;
        default:
            printf("\nOpcao invalida!\n");
            menu_remover();
            break;
        }
    }else{
        printf("Aluno nao encontrado!");
        menu_principal();
    }
    
}

void menu_alterar(){
    int matricula;
    printf("Digite a matricula do Aluno a ser alterado: ");
    scanf("%d", &matricula);
    modificar(matricula);
}

void menu_buscar(){
    int matricula;
    int p;
    printf("Digite a matricula do Aluno a ser buscado: ");
    scanf("%d", &matricula);
    imprimirAluno(matricula);

    printf("\n\nDeseja pesquisar por outro Aluno?\n1- Sim\n2- Nao\n");
    printf("Digite uma opcao: ");
    scanf("%d", &p);
    switch (p){
    case 1:
        menu_buscar();
        break;
    case 2:
        menu_principal();
    default:
        printf("\nOpcao Invalida!\n");
        menu_principal();
        break;
    }
    
}

int main(){
    printf("Bem vindo ao Sistema de Gerencia de Alunos da UFC - Campus Russas!");
    // add(50, "conrado", "cc@gmail.com", 88925811);
    // add(25, "pedro", "pe@gmail.com", 8854236);
    // add(75, "samuel", "sa@gmail.com", 88795717);
    menu_principal();
    printf("\nTHANKSSSS-\n");
    return 0;
}
