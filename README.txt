Universidade Federal da Fronteira Sul
Campus Chapecó, SC
Curso de Ciência da Computação
Redes de Computadores – 2022.1
Profº Marco Aurélio Spohn

Programação Socket e Roteamento

Simulação de protocolo de roteamento entre processos roteadores com sockets UDP.

Instruções
Compilação:
    gcc router.c router.h -lpthread -o router

Execução:
Ao executar o arquivo 'router' será requisitado um ID para o roteador
    
Após definido o ID, será apresentado um menu de opções:
    [1] - Enviar mensagem
    [2] - Listar destinos
    [0] - Encerrar
    
Digitando "1" deverá ser digitado o ID do roteador de destino e a mensagem que será enviada. Inseridos esses dados, será retornado ao menu.
Digitando "2" serão listados os roteadores disponíves para comunicação.
Digitando "0" os processos serão encerrados e a execução finalizada.

Para realizar a comunicação entre dois roteadores é necessário ter duas instâncias do programa executadas e com IDs diferentes.

Observações:
- IDs atribuídos aos roteadores devem existir nos arquivos de configuração (ID presentes: 1, 2, 3 e 4).
- Na implementação atual não é possível usar a rede para comunicação de mais de um salto ou identificar roteadores ativados ou desativados.
