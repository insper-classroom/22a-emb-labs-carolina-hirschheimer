/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// PLACA SAME70-XPLD
// Led 
#define LED_PIO           PIOC                 // periferico que controla o LED
#define LED_PIO_ID        ID_PIOC              // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED
// Botão
#define BUT_PIO			PIOA
#define BUT_PIO_ID		ID_PIOA
#define BUT_PIO_IDX		11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX) // esse já está pronto.

// OLED 1
// Led
#define OLED_1_PIO		PIOA
#define OLED_1_PIO_ID	ID_PIOA
#define OLED_1_PIO_IDX	0
#define OLED_1_PIO_IDX_MASK (1<<OLED_1_PIO_IDX)
// Botão
#define BUT_PIO_1			PIOD
#define BUT_PIO_1_ID		ID_PIOD
#define BUT_PIO_1_IDX		28
#define BUT_PIO_1_IDX_MASK (1u << BUT_PIO_1_IDX)

// OLED 2
// Led
#define OLED_2_PIO		PIOC
#define OLED_2_PIO_ID	ID_PIOC
#define OLED_2_PIO_IDX	30
#define OLED_2_PIO_IDX_MASK (1<<OLED_2_PIO_IDX)
// Botão
#define BUT_PIO_2			PIOC
#define BUT_PIO_2_ID		ID_PIOC
#define BUT_PIO_2_IDX		31
#define BUT_PIO_2_IDX_MASK (1u << BUT_PIO_2_IDX)


// OLED 3
// Led
#define OLED_3_PIO		PIOB
#define OLED_3_PIO_ID	ID_PIOB
#define OLED_3_PIO_IDX	2
#define OLED_3_PIO_IDX_MASK (1<<OLED_3_PIO_IDX)
// Botão
#define BUT_PIO_3			PIOA
#define BUT_PIO_3_ID		ID_PIOA
#define BUT_PIO_3_IDX		19
#define BUT_PIO_3_IDX_MASK (1u << BUT_PIO_3_IDX)

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

// Função de inicialização do uC
void init(void){
	// Initialize the board clock
	sysclk_init();
	
	//Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(OLED_1_PIO_ID);
	pmc_enable_periph_clk(OLED_2_PIO_ID);
	pmc_enable_periph_clk(OLED_3_PIO_ID);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_1_ID);
	pmc_enable_periph_clk(BUT_PIO_2_ID);
	pmc_enable_periph_clk(BUT_PIO_3_ID);
	
	//Inicializa PC8 como saída
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0);
	
	pio_set_output(OLED_1_PIO, OLED_1_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(OLED_2_PIO, OLED_2_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(OLED_3_PIO, OLED_3_PIO_IDX_MASK, 1, 0, 0);
	
	// configura pino ligado ao botão como entrada com um pull-up.
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	// Faz com que o valor padrão do pino seja energizado
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	
	// config botao oled 1
	pio_set_input(BUT_PIO_1, BUT_PIO_1_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO_1, BUT_PIO_1_IDX_MASK, 1);
	
	
	// config botao oled 2
	pio_set_input(BUT_PIO_2, BUT_PIO_2_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO_2, BUT_PIO_2_IDX_MASK, 1);
	
	
	// config botao oled 3
	pio_set_input(BUT_PIO_3, BUT_PIO_3_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO_3, BUT_PIO_3_IDX_MASK, 1);
	
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void) {
	// inicializa sistema e IOs
	init();
	
	
	// super loop
	// aplicacoes embarcadas não devem sair do while(1).
	while (1) {
	
	// PLACA SAME70-XPLD
	if(pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			pio_clear(PIOC, LED_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
			pio_set(PIOC, LED_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
		}
	}
	
	// OLED 1
	if(pio_get(BUT_PIO_1, PIO_INPUT, BUT_PIO_1_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			pio_clear(PIOA, OLED_1_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
			pio_set(PIOA, OLED_1_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
		}
	}
	
	// OLED 2
	if(pio_get(BUT_PIO_2, PIO_INPUT, BUT_PIO_2_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			pio_clear(PIOC, OLED_2_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
			pio_set(PIOC, OLED_2_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
		}
	}
	
	// OLED 3
	if(pio_get(BUT_PIO_3, PIO_INPUT, BUT_PIO_3_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			pio_clear(PIOB, OLED_3_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
			pio_set(PIOB, OLED_3_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			delay_ms(1000);                         // delay
		}
	}
	
	

	}
	// Nunca devemos chegar aqui !
	return 0;
}

