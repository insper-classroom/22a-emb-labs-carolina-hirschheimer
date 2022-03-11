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


/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)


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

/**
 * \brief Set a high output level on all the PIOs defined in ul_mask.
 * This has no immediate effects on PIOs that are not output, but the PIO
 * controller will save the value if they are changed to outputs.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */

void _pio_set (Pio *p_pio, const uint32_t ul_mask) {
	p_pio->PIO_SODR = ul_mask;
}

void _pio_clear (Pio *p_pio, const uint32_t ul_mask) {
	p_pio->PIO_CODR = ul_mask;
}

/**
 * \brief Configure PIO internal pull-up.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_pull_up_enable Indicates if the pin(s) internal pull-up shall be
 * configured.
 */

void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask,
        const uint32_t ul_pull_up_enable){
			if (ul_pull_up_enable) {
				p_pio->PIO_PUER = ul_mask;
				} else {
				p_pio->PIO_PUDR = ul_mask;
			}
 }

/**
 * \brief Configure one or more pin(s) or a PIO controller as inputs.
 * Optionally, the corresponding internal pull-up(s) and glitch filter(s) can
 * be enabled.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure as input(s).
 * \param ul_attribute PIO attribute(s).
 */

void _pio_set_input(Pio *p_pio, const uint32_t ul_mask,
        const uint32_t ul_attribute){
			_pio_pull_up(p_pio, ul_mask, ul_attribute & PIO_PULLUP);

			if (ul_attribute & (PIO_DEGLITCH | PIO_DEBOUNCE)) {
				p_pio->PIO_IFER = ul_mask;
				} else {
				p_pio->PIO_IFDR = ul_mask;
			}
			}

/**
 * \brief Configure one or more pin(s) of a PIO controller as outputs, with
 * the given default value. Optionally, the multi-drive feature can be enabled
 * on the pin(s).
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure.
 * \param ul_default_level Default level on the pin(s).
 * \param ul_multidrive_enable Indicates if the pin(s) shall be configured as
 * open-drain.
 * \param ul_pull_up_enable Indicates if the pin shall have its pull-up
 * activated.
 */
void _pio_set_output(Pio *p_pio, const uint32_t ul_mask,
        const uint32_t ul_default_level,
        const uint32_t ul_multidrive_enable,
        const uint32_t ul_pull_up_enable) {
			
			p_pio->PIO_OER = ul_mask;
			p_pio->PIO_PER = ul_mask;

			if (ul_multidrive_enable) {
				p_pio->PIO_MDER = ul_mask;
				} else {
				p_pio->PIO_MDDR = ul_mask;
			}

			if (ul_default_level) {
				_pio_set (p_pio, ul_mask);
				} else {
				_pio_clear (p_pio, ul_mask);
			}
			
			_pio_pull_up(p_pio, ul_mask, ul_pull_up_enable);
			
}

/**
 * \brief Return 1 if one or more PIOs of the given Pin instance currently have
 * a high level; otherwise returns 0. This method returns the actual value that
 * is being read on the pin. To return the supposed output value of a pin, use
 * pio_get_output_data_status() instead.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_type PIO type.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 *
 * \retval 1 at least one PIO currently has a high level.
 * \retval 0 all PIOs have a low level.
 */

uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type,
        const uint32_t ul_mask){
			
			uint32_t ul_reg;

			if ((ul_type == PIO_OUTPUT_0) || (ul_type == PIO_OUTPUT_1)) {
				ul_reg = p_pio->PIO_ODSR;
				} else {
				ul_reg = p_pio->PIO_PDSR;
			}

			if ((ul_reg & ul_mask) == 0) {
				return 0;
				} else {
				return 1;
			}
			
		}
		
		
void _delay_ms(int delay){

	for (int i = 0; i < delay*(300000/2) ; i++){
		asm("nop");
	}
}


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
	_pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0);
	
	_pio_set_output(OLED_1_PIO, OLED_1_PIO_IDX_MASK, 1, 0, 0);
	_pio_set_output(OLED_2_PIO, OLED_2_PIO_IDX_MASK, 1, 0, 0);
	_pio_set_output(OLED_3_PIO, OLED_3_PIO_IDX_MASK, 1, 0, 0);
	
	_pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
		
	// config botao oled 
	_pio_set_input(BUT_PIO_1, BUT_PIO_1_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	
	// config botao oled 2
	_pio_set_input(BUT_PIO_2, BUT_PIO_2_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	
	// config botao oled 3
	_pio_set_input(BUT_PIO_3, BUT_PIO_3_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);

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
	if(_pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			_pio_clear(PIOC, LED_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
			_pio_set(PIOC, LED_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
		}
	}
	
	// OLED 1
	if(_pio_get(BUT_PIO_1, PIO_INPUT, BUT_PIO_1_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			_pio_clear(PIOA, OLED_1_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
			_pio_set(PIOA, OLED_1_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
		}
	}
	
	// OLED 2
	if(_pio_get(BUT_PIO_2, PIO_INPUT, BUT_PIO_2_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			_pio_clear(PIOC, OLED_2_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
			_pio_set(PIOC, OLED_2_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
		}
	}
	
	// OLED 3
	if(_pio_get(BUT_PIO_3, PIO_INPUT, BUT_PIO_3_IDX_MASK)!=1) {
		// Pisca LED
		for (int i=0; i<5; i++) {
			_pio_clear(PIOB, OLED_3_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
			_pio_set(PIOB, OLED_3_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(1000);                         // delay
		}
	}
	
	

	}
	// Nunca devemos chegar aqui !
	return 0;
}

