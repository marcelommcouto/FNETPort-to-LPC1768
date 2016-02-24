#ifndef FNET_STACK_CPU_LPC_FNET_LPC1768_ETHERNET_H_
#define FNET_STACK_CPU_LPC_FNET_LPC1768_ETHERNET_H_

/** ************************************************************************
 * Modulo: fnet_lpc1768_ethernet 
 * @file fnet_lpc1768_ethernet.h
 * @headerfile fnet_lpc1768_ethernet.h
 * @author Marcelo Martins Maia do Couto - Email: marcelo.m.maia@gmail.com
 * @date Feb 22, 2016
 *
 * @brief Substitua este texto por uma descrição breve deste módulo.
 *
 * Substitua este texto pela descrição completa deste módulo.
 * Este módulo é um modelo para a criação de novos módulos. Ele contém
 * toda a estrutura que um módulo deve conter, sendo composto pelos arquivos:
 *   - fnet_lpc1768_ethernet.c;
 *   - fnet_lpc1768_ethernet.h.
 *
 * @copyright Copyright 2015 M3C Tecnologia
 * @copyright Todos os direitos reservados.
 *
 * @note
 *  - Não sobrescreva os arquivos de template do módulo. Implemente um novo
 *    módulo sobre uma cópia do template.
 *  - Os padrões de comentário que começam com "/** ", como este, devem ser
 *    compilados com a ferramenta Doxygen (comando:
 *    "doxygen.exe doxygen.cfg").
 *  - Leia a documentação do @b Doxygen para maiores informações sobre o
 *    funcionamento dos recursos de documentação de código.
 *
 * @warning
 *  - É altamente recomendado manter todos os arquivos de template como
 *    somente-leitura, evitando assim que eles sejam sobrescritos ao serem
 *    utilizados.
 *
 * @attention
 *  - A descrição de cada módulo como um todo deve ser realizada no arquivo
 *    ".h" do mesmo.
 *  - Cada módulo de um projeto de software deve conter, pelo menos, um
 *    arquivo ".h" e um ".c".
 * @pre 
 *   Coloque algum pré-requisito para utilizar este módulo aqui.
 *
 ******************************************************************************/

/*
 * Inclusão de arquivos de cabeçalho da ferramenta de desenvolvimento.
 * Por exemplo: '#include <stdlib.h>'.
 */

/*
 * Inclusão de arquivos de cabeçalho sem um arquivo ".c" correspondente.
 * Por exemplo: '#include "stddefs.h"'.
 */

/*
 * Inclusão de arquivos de cabeçalho de outros módulos utilizados por este.
 * Por exemplo: '#include "serial.h"'.
 */
#include "fnet.h"

#if FNET_LPC && FNET_CFG_CPU_ETH0
/*******************************************************************************
 *                           DEFINICOES E MACROS							   *
 ******************************************************************************/

/*******************************************************************************
 *                     ESTRUTURAS E DEFINICOES DE TIPOS						   *	
 ******************************************************************************/

/*******************************************************************************
 *                       VARIAVEIS PUBLICAS (Globais)						   *
 ******************************************************************************/

/*******************************************************************************
 *                      PROTOTIPOS DAS FUNCOES PUBLICAS						   *
 ******************************************************************************/
int fnet_lpceth_init(fnet_netif_t *netif);
void fnet_lpceth_release(fnet_netif_t *netif);
void fnet_lpceth_input(fnet_netif_t *netif);
void fnet_lpceth_output(fnet_netif_t *netif, unsigned short type, const fnet_mac_addr_t dest_addr, fnet_netbuf_t* nb);
int fnet_lpceth_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
int fnet_lpceth_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
int fnet_lpceth_get_statistics(struct fnet_netif *netif, struct fnet_netif_statistics * statistics);
int fnet_lpceth_is_connected(fnet_netif_t *netif);
void fnet_lpceth_init_dma(void);
uint8_t fnet_lpceth_transmit_packet(uint8_t *packet, uint16_t size);
uint32_t fnet_lpceth_read_packet(void *buffer);
void fnet_lpceth_multicast_join(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
void fnet_lpceth_multicast_leave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
/*******************************************************************************
 *                                   EOF									   *	
 ******************************************************************************/
#endif

#endif
