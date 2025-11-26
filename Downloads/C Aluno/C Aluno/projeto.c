#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configuração ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funções para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funções a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{   // TODO: implementar exibição do menu principal com as opções de impressão
    printf("\n===== MENU PRINCIPAL =====\n");
    printf("1 - Configurar conexao\n");
    printf("2 - Abrir conexao\n");
    printf("3 - Imprimir texto\n");
    printf("4 - Imprimir QRCode\n");
    printf("5 - Imprimir codigo de barras\n");
    printf("6 - Imprimir XML SAT\n");
    printf("7 - Imprimir XML Cancelamento SAT\n");
    printf("8 - Abrir gaveta Elgin\n");
    printf("9 - Abrir gaveta generica\n");
    printf("10 - Emitir sinal sonoro\n");
    printf("0 - fechar conexao e Sair\n");
    printf("===========================\n");
    printf("Escolha uma opcao: ");
 
}

static void configurarConexao(void){
    // TODO: pedir ao usuário tipo, modelo, conexão e parâmetro
    printf("Digite o tipo (ex: 1 para USB): \n");
    scanf("%d", &g_tipo);
    
    flush_entrada();
    
    printf("Digite o modelo (ex: i9): \n");
    scanf("%s", &g_modelo);
    
    printf("Digite a conexao (ex: USB ou COM3): \n");
    scanf("%s", &g_conexao);
    
    printf("Digite o parametro: \n");
    scanf("%d", &g_parametro);
    
    flush_entrada();
    
    printf("Dados salvos com sucesso\n\n");
    printf("tipo=%d", g_tipo);
    printf("tipo=%s", g_modelo);
    printf("tipo=%s", g_conexao);
    printf("tipo=%d", g_parametro);
}

static void abrirConexao(void){
    
     if(g_conectada == 0){
    	
    	int retorno = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
    	
    	if(retorno == 0){
    		printf("Conexao aberta com sucesso\n\n");
    		g_conectada = 1;
		}else{
			printf("Deu erro ao abrir conexao. Retorno %d\n", retorno);
		}
    	
	}else{
		printf("Ja tem uma conexao em aberto\n");
	}
}

static void fecharConexao(void){
	int retorno = -1;
	
    if(g_conectada == 0){
		printf("Ja esta fechada \n");
	}
	else{ 
	
	retorno = FechaConexaoImpressora();
	
		if(retorno == 0){
    		printf("Conexao fechada com sucesso\n\n");
    		g_conectada = 0;
		}else{
			printf("Deu erro ao fechar conexao. Retorno %d\n", retorno);
		}
	
	}
}

static void imprimirTexto(void){
     if(g_conectada == 1){
     	
     	int retorno = ImpressaoTexto ("Teste de impressao", 1, 4, 0);
     	
     	AvancaPapel(5);
     	
	 if(retorno == 0){
    		printf("texto imprimido com sucesso\n\n");
    		Corte(3);
		}else{
			printf("Deu erro ao imprimi texto. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }

}

static void imprimirQRCode(void){
     if(g_conectada == 1){
     	
     	int retorno = ImpressaoQRCode ("Teste de impressao", 6, 4);
     	

     	
	 if(retorno == 0){
    		printf("QRCode imprimido com sucesso\n\n");
    		AvancaPapel(2);
    		Corte(2);
		}else{
			printf("Deu erro ao imprimi QRCode. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }
}

static void imprimirCodigoBarras(void){
    if(g_conectada == 1){
     	
     	int retorno = ImpressaoCodigoBarras (8, "{A012345678912", 100, 2, 3);
     	

     	
	 if(retorno == 0){
    		printf("Codigo de barras imprimido com sucesso\n\n");
    		AvancaPapel(2);
    		Corte(2);
		}else{
			printf("Deu erro ao imprimi Codigo de barras. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }
}

static void imprimirXMLSAT(void){
	if(g_conectada == 1){
		char *xml = ("path=C:/Users/arthur.lacerda/Downloads/C Aluno/C Aluno/XMLSAT.xml");
		if(xml){
			printf("sucesso em ler o arquivo XMLSAT\n");
			
			int retorno = ImprimeXMLSAT (xml, 0);
			if(retorno == 0){
    		printf("XMLSAT imprimido com sucesso\n\n");
    		AvancaPapel(2);
    		Corte(2);
		}
		else{
		printf("Deu erro ao imprimi XMLSAT. Retorno %d\n", retorno);}
			
		}
	}

	 else{
	 	printf("Conexao fechada");
	 }

}

static void imprimirXMLCancelamentoSAT(void){
	
		if(g_conectada == 1){
			
		char *canc_xml = ("path=C:/Users/arthur.lacerda/Downloads/C Aluno/C Aluno/CANC_SAT.xml");
		
		const char *assQRCode = "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
		"jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
		"SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
		"Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
		"p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
		"YVFCDtYR9Hi5qgdk31v23w==";
        
		if(canc_xml ){
		
			int retorno = ImprimeXMLCancelamentoSAT (canc_xml, assQRCode, 0);
			
			if(retorno == 0){
    		printf("Cancelamento com sucesso\n\n");
    		AvancaPapel(2);
    		Corte(2);
		}
		else{
		printf("Deu erro ao imprimi Cancelamento SAT. Retorno %d\n", retorno);}
			
		}
	}

	 else{
	 	printf("Conexao fechada");
	 }
        
}

static void abrirGavetaElginOpc(void){
 if(g_conectada == 1){
     	
     	int retorno = AbreGavetaElgin(1, 50, 50);
     	
     	
	 if(retorno == 0){
    		printf("Gaveta abriu comsucesso\n\n");
		}else{
			printf("Deu erro ao abir gaveta. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }

}

static void abrirGavetaOpc(void){

 if(g_conectada == 1){
     	
     	int retorno = AbreGaveta(1, 5, 10);
     	

     	
	 if(retorno == 0){
    		printf("Gaveta abriu com sucesso\n\n");
		}else{
			printf("Deu erro ao abir gaveta. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }
}

static void emitirSinalSonoro(void){
if(g_conectada == 1){
     	
     	int retorno = SinalSonoro(4, 5, 5);
     	

     	
	 if(retorno == 0){
    		printf("sinal sonoro emitido com sucesso\n\n");
		}else{
			printf("Deu erro ao emitir sinal sonoro. Retorno %d\n", retorno);
		}
	 }
	 else{
	 	printf("Conexao fechada");
	 }
}

/* ======================= Função principal ======================= */
int main(void){
	
	
   if (!carregarFuncoes()) {
        return 1;
    }

    int opcao = 0;
     
    while (1) {
    	
        exibirMenu(); 
        
        printf("Digite sua escolha: ");
        scanf("%d", &opcao);
        
        switch (opcao)
		{
			case 0:
			 fecharConexao(); 
			 liberarBiblioteca();
			 printf("conexao fechada e saindo do programa");
			 return 0;
			 break;
			
        	case 1: configurarConexao();
			 break;
        	
        	case 2: abrirConexao(); 
			 break;
        	
        	case 3: imprimirTexto(); 
			 break;
        	
        	case 4: imprimirQRCode();
			 break;
        	
        	case 5: imprimirCodigoBarras();
			 break;
        	
			case 6: imprimirXMLSAT(); 
			break;
			
			case 7: imprimirXMLCancelamentoSAT();
			 break;
			
			case 8: abrirGavetaElginOpc();
			 break;
			
			case 9: abrirGavetaOpc(); 
			 break;
			
			case 10: emitirSinalSonoro(); 
			 break;
			 
			 default: printf("opcao infalida\n");
    	} 
    } 
}

