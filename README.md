# Leitura do MBR (Master Boot Record)
Este projeto tem como objetivo a leitura e interpretação do Master Boot Record (MBR) de um disco rígido. O MBR contém informações essenciais sobre as partições do disco e é um componente crítico na inicialização de sistemas operacionais.

## Funcionalidades
- **Leitura do arquivo binário do MBR**: O código lê o conteúdo de um arquivo binário chamado mbr.bin, que contém o MBR do disco.
- **Identificação das partições**: O código identifica até 4 partições, exibindo o status, tipo de partição e os setores que delimitam o início e o fim de cada partição.
- **Verificação da assinatura de boot**: O MBR deve ter uma assinatura de boot (0x55AA) válida. O código verifica essa assinatura.
- **Cálculo do tamanho do disco**: Com base no número de setores, o código calcula o tamanho do disco em GiB e bytes.
- **Exibição detalhada**: Exibe informações sobre o disco, partições, setor lógico/físico, e identificador de disco.
## Estrutura do MBR
- **Tamanho Total**: 512 bytes.
- **Número de Partições**: O MBR permite até 4 partições, cada uma ocupando 16 bytes.
- **Partições**: Cada partição contém os seguintes campos:
  - `status`: Indica se a partição é inicializável.
  - `chs_start`: Endereço CHS (Cylinder-Head-Sector) de início.
  - `partition_type`: Tipo de partição.
  - `chs_end`: Endereço CHS de término.
  - `lba_sector`: Endereço LBA (Logical Block Addressing) de início da partição.
  - `number_sectors`: Quantidade de setores ocupados pela partição.
## Funções Principais
- `get_real_address`: Função que inverte a ordem dos bytes de um valor para correção de endianness (big-endian para little-endian).
- `get_partition_type`: Retorna uma string representando o tipo de partição com base no valor hexadecimal.
- `main`: Função principal que faz a leitura do MBR, extrai as informações das partições e exibe o resultado em um formato similar ao do comando `fdisk`.
## Como Compilar e Executar
### Requisitos
- Compilador C (GCC, por exemplo).
- Arquivo binário do MBR (`mbr.bin`).
### Compilação
Para compilar o código, use o seguinte comando:
```bash
gcc main -o main.c
```
### Execução
Após compilar, execute o programa:
```bash
./main
```
O programa irá ler o arquivo `mbr.bin` e exibir informações detalhadas sobre o disco e as partições.
## Saída Esperada
O programa irá exibir informações como:
- Assinatura de Boot: Verifica se o MBR contém a assinatura válida.
- Identificador do Disco: ID único do disco.
- Detalhes das Partições: Para cada partição, será mostrado o status, LBA de início, número de setores, tipo de partição, e se é inicializável.
Exemplo de saída:
```
BOOT SIGNATURE: 0x55aa
DISK ID: 0x12345678

Disco /dev/sda: 100.00 GiB, 107374182400 bytes, 209715200 setores
Modelo de disco: VBO HARDDISK
Unidades: setor de 1 * 512 = 512 bytes
Tamanho do setor (lógico/físico): 512 bytes / 512 bytes
Tamanho E/S (mínimo/ótimo): 512 bytes / 512 bytes
Tipo de rótulo do disco: dos
Identificador do disco: 0x12345678

Dispositivo  Inicializar  Início   Fim   Setores   Tamanho     Id   Tipo
/dev/sda1    *              2048  4096      2048      1.0G     83   Linux
```
## Observações
- Este código é uma simulação de leitura do MBR e requer um arquivo `mbr.bin` para funcionar.
- Ele lê diretamente os bytes do MBR e exibe as informações de maneira simples, sem realizar alterações no disco.
