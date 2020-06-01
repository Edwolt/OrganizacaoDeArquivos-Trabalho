/*
Abaixo seguem funções que fazem a escrita do binário em "stdout" (tela) pra poder ser comparado no run.codes.

Funciona assim: você faz tudo o que tiver que fazer na funcionalidade no arquivo em disco, assim como ensinado nas aulas da disciplina.
Ao fim da funcionalidade, use a função "binarioNaTela" e a função já cuida de tudo para você. É só chamar a função.

Note que ao usar a binarioNaTela, o fclose no arquivo binário já deve ter sido feito anteriormente. Você passa o nome do arquivo binário ("arquivoTrabX.bin") pra ela e ela vai ler tudo e escrever na tela.

Você pode colocar isso num módulo .h separado, ou incluir as funções no próprio código .c: como preferir.
VOCÊ NÃO PRECISA ENTENDER ESSAS FUNÇÕES. É só usar elas da forma certa depois de acabar a funcionalidade.
NÃO MODIFIQUE ESSAS FUNÇÕES. Da forma como elas estão aqui, já estão funcionando para o propósito delas.

Tá tudo testado e funcionando, mas qualquer dúvida acerca dessas funções, falar com o monitor Matheus (mcarvalhor@usp.br).
*/

#ifndef H_BINARIONATELA_
#define H_BINARIONATELA_

/**
 * Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
 * Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). 
 */
void binarioNaTela(char *nomeArquivoBinario);

/**
 * Essa função arruma uma string de entrada "str".
 * Manda pra ela uma string que tem '\r' e ela retorna sem.
 * Ela remove do início e do fim da string todo tipo de espaçamento (\r, \n, \t, espaço, ...).
 * 
 * Por exemplo:
 * char minhaString[] = "    \t TESTE  DE STRING COM BARRA R     \t  \r\n ";
 * trim(minhaString);
 * printf("[%s]", minhaString); // vai imprimir "[TESTE  DE STRING COM BARRA R]"
 */
void trim(char *str);

/**
 * Use essa função para ler um campo string delimitado entre aspas (").
 * Chame ela na hora que for ler tal campo. Por exemplo:
 *
 * A entrada está da seguinte forma:
 *   nomeDoCampo "MARIA DA SILVA"
 *
 * Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
 *   scanf("%s", str1); // Vai salvar nomeDoCampo em str1
 *   scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
 *
 */
void scan_quote_string(char *str);

#endif

// Acabou o código que vai no .h