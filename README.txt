Universidade Federal da Fronteira Sul
Campus Chapecó, SC
Curso de Ciência da Computação
Redes de Computadores – 2022.1
Profº Marco Aurélio Spohn

Programação Socket e Roteamento

Simulação de protocolo de roteamento entre processos roteadores com sockets UDP.

Instruções
Compilar os arquivos "terminalid1.c" e "terminalid2.c" e executa-los simultaneamente.
Ao executar, será apresentado um menu de opções:
    [1] - Enviar mensagem
    [2] - Listar destinos
    [9] - Receive
    [0] - Encerar

Na implementação atual, os dois processos atuam exclusivamente como servidor ou excludivamente como cliente.
Digitando "1" o processo entrara em modo cliente, será solicitado o identificador de um rotedor destino (Não implementado) e em seguida uma mensagem a ser transmitida ao processo atualndo como servidor. Atualemente uma única mensagem poderá ser transmitida e o processo retornará ao menu.
Digitando "2" serão listados os processos disponives para comunicação (Não implementado).
Digitando "9" o processo entrara em modo servidor Atualmente o processor aguardará o recebimento de uma única mensagem e retornará ao menu.
Digitanto "0" o processo será encerrado e finalizado.

Para que a mensagem seja transmitida é preciso que um processo enteja em modo servidor ("Receive").
