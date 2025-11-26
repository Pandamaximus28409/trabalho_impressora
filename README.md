## 💻 `E1_Impressora_Exemplo_C`

**Exemplo de Aplicação em C para Comunicação com a DLL de Impressoras Elgin**

-----

### ✨ Sobre o Projeto

Este repositório contém um projeto de exemplo em **linguagem C** que demonstra como utilizar as funcionalidades da **DLL `E1_Impressora01.dll`** (fornecida pela Elgin) para comunicação e controle de impressoras fiscais e não fiscais (SAT).

O código-fonte (`main.c`) inclui:

  * Carregamento dinâmico da DLL.
  * Definição e carregamento dos ponteiros de função para todas as operações da impressora.
  * Uma interface de **menu de console** interativa para testar as diversas funções (abrir/fechar conexão, imprimir texto, QR Code, código de barras, XML SAT, abrir gaveta, sinal sonoro, etc.).

É um excelente ponto de partida para desenvolvedores que precisam integrar a comunicação com impressoras Elgin em aplicações legadas ou de baixo nível em ambientes **Windows**.

### 🛠️ Funcionalidades Principais

O exemplo demonstra o uso das seguintes funções da DLL:

| Função | Descrição |
| :--- | :--- |
| `AbreConexaoImpressora` | Abre a conexão com a impressora (USB, Serial/COM, Rede). |
| `FechaConexaoImpressora` | Fecha a conexão ativa. |
| `ImpressaoTexto` | Imprime uma string de texto formatada. |
| `Corte` | Executa o corte do papel. |
| `ImpressaoQRCode` | Imprime um QR Code. |
| `ImpressaoCodigoBarras` | Imprime um Código de Barras (diversos tipos). |
| `AvancaPapel` | Avança o papel. |
| `AbreGavetaElgin` | Abre a gaveta de dinheiro (para modelos Elgin). |
| `AbreGaveta` | Abre a gaveta de dinheiro (comando genérico). |
| `SinalSonoro` | Emite um sinal sonoro (beep). |
| `ImprimeXMLSAT` | Imprime o Extrato SAT a partir de um arquivo XML. |
| `ImprimeXMLCancelamentoSAT` | Imprime o Extrato de Cancelamento SAT. |

###  Como Compilar e Executar

#### Pré-requisitos

  * Um compilador C (como **GCC** ou o compilador do Visual Studio). Recomenda-se o uso do **MinGW** para compilação no Windows.
  * A biblioteca do Windows (`<windows.h>`) e as funções de carregamento de DLLs.

#### 1\. Obtenha a DLL

Você deve obter o arquivo **`E1_Impressora01.dll`** diretamente do pacote de SDK da Elgin. Coloque-o no mesmo diretório do executável final.

#### 2\. Compilação

Se estiver usando o GCC/MinGW, você pode compilar o código utilizando o seguinte comando no terminal:

```bash
gcc main.c -o printer_test.exe -lws2_32
```

> **Nota:** A flag `-lws2_32` pode ser necessária dependendo da versão do compilador e das dependências da DLL.

#### 3\. Execução

Execute o binário gerado:

```bash
./printer_test.exe
```

O programa exibirá um menu interativo no console.

### ⚙️ Configuração Inicial (Opção 1)

Antes de realizar qualquer impressão, você deve configurar os parâmetros de conexão.

| Parâmetro | Tipo no Código | Exemplo |
| :--- | :--- | :--- |
| **Tipo** | `int` (`g_tipo`) | `1` para USB, `3` para Serial/COM |
| **Modelo** | `char*` (`g_modelo`) | `"i9"`, `"VOX"` |
| **Conexao** | `char*` (`g_conexao`) | `"USB"`, `"COM3"`, `"192.168.1.100"` |
| **Parâmetro** | `int` (`g_parametro`) | `0` (geralmente não usado ou taxa de baud para serial) |

Após a configuração, use a **Opção 2 - Abrir Conexão** para iniciar a comunicação.

### Seções Chave do Código

#### Carregamento da DLL

O arquivo `E1_Impressora01.dll` é carregado dinamicamente usando a função `LoadLibraryA`:

```c
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    // ...
}
```

#### Resolução de Símbolos

Os ponteiros de função são resolvidos (associados) às funções exportadas pela DLL usando `GetProcAddress`. O macro `LOAD_FN` simplifica esse processo:

```c
#define LOAD_FN(h, name) /* ... */
// ...
LOAD_FN(g_hDll, AbreConexaoImpressora);
// ...
```

-----

#### Participantes

Arthur Augusto Mendes de Lacerda,
Arthur Pereira ,
Markson da Silva Ribeiro,
Guilherme Santos Gomes,
Tiago Dias de Oliveira 
