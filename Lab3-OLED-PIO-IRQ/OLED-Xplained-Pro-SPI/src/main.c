#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

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
/* prototype                                                            */
/************************************************************************/
void io_init(void);
void pisca_led(int n, int t);

volatile char but_flag_1;
volatile char but_flag_2;
volatile char but_flag_3;
volatile char esta_piscando;
volatile char break_piscando;

void but_callback_1(void){
	if (pio_get(BUT_PIO_1, PIO_INPUT, BUT_PIO_1_IDX_MASK)) {
		but_flag_1 = 0;
		
		// PINO == 1 --> Borda de subida
		} else {
		// PINO == 0 --> Borda de descida
		but_flag_1 =1;
		}}

void but_callback_2(void) {
	if (but_flag_2) {
		but_flag_2 = 0;
		} else {
		but_flag_2 =1;
	}}


void but_callback_3(void) {	
	if (pio_get(BUT_PIO_3, PIO_INPUT, BUT_PIO_3_IDX_MASK)) {
		but_flag_3 = 0;
		
		// PINO == 1 --> Borda de subida
		} else {
		// PINO == 0 --> Borda de descida
		but_flag_3 =1;
	}
}

/************************************************************************/
/* funções                                                              */
/************************************************************************/
// pisca led N vez no periodo T
void pisca_led(int n, int t){
	for (int i=0;i<n;i++){
		esta_piscando = 1;
		pio_clear(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(t);
		pio_set(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(t);
		
		
		if (but_flag_2) {
			pio_set(LED_PIO, LED_PIO_IDX_MASK);
			delay_ms(30);
			break;
		}
	}
	esta_piscando = 0;
}

// Inicializa botao SW0 do kit com interrupcao
void io_init(void)
{

	// Configura led
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_PIO_IDX_MASK, PIO_DEFAULT);

	// Inicializa clock do periférico PIO responsavel pelo botao
	pmc_enable_periph_clk(BUT_PIO_1_ID);
	pmc_enable_periph_clk(BUT_PIO_2_ID);
	pmc_enable_periph_clk(BUT_PIO_3_ID);

	// Configura PIO para lidar com o pino do botão como entrada
	// com pull-up
	pio_configure(BUT_PIO_1, PIO_INPUT, BUT_PIO_1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO_1, BUT_PIO_1_IDX_MASK, 60);
	
	pio_configure(BUT_PIO_2, PIO_INPUT, BUT_PIO_2_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO_2, BUT_PIO_2_IDX_MASK, 60);

	pio_configure(BUT_PIO_3, PIO_INPUT, BUT_PIO_3_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO_3, BUT_PIO_3_IDX_MASK, 60);

	// Configura interrupção no pino referente ao botao e associa
	// função de callback caso uma interrupção for gerada
	// a função de callback é a: but_callback()
	pio_handler_set(BUT_PIO_1,
	BUT_PIO_1_ID,
	BUT_PIO_1_IDX_MASK,
	PIO_IT_EDGE,
	but_callback_1);
	
	pio_handler_set(BUT_PIO_2,
	BUT_PIO_2_ID,
	BUT_PIO_2_IDX_MASK,
	PIO_IT_RISE_EDGE,
	but_callback_2);
	
	pio_handler_set(BUT_PIO_3,
	BUT_PIO_3_ID,
	BUT_PIO_3_IDX_MASK,
	PIO_IT_EDGE,
	but_callback_3);
	
	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT_PIO_1, BUT_PIO_1_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_1);
	
	pio_enable_interrupt(BUT_PIO_2, BUT_PIO_2_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_2);
	
	pio_enable_interrupt(BUT_PIO_3, BUT_PIO_3_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_3);
		
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO_1_ID);
	NVIC_SetPriority(BUT_PIO_1_ID, 4); // Prioridade 4
	
	NVIC_EnableIRQ(BUT_PIO_2_ID);
	NVIC_SetPriority(BUT_PIO_2_ID, 4);
	
	NVIC_EnableIRQ(BUT_PIO_3_ID);
	NVIC_SetPriority(BUT_PIO_3_ID, 4);
}

int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	
	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

	// configura botao com interrupcao
	io_init();

	

  // Init OLED
	gfx_mono_ssd1306_init();
  

  /* Insert application code here, after the board has been initialized. */
	int delay = 300;
	char str[128];
	sprintf(str, "%d", delay);
	gfx_mono_draw_string(str , 100, 16, &sysfont);
	/* Insert application code here, after the board has been initialized. */
	gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
	gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
	
	while(1) {
		
		if (!but_flag_2) {
			pisca_led(1,delay);
			
			//gfx_mono_generic_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
			//gfx_mono_generic_draw_filled_rect(10, 5, 110*(delay/1000), 10, GFX_PIXEL_SET);
			
			
			if(but_flag_3){
				delay -= 100;
				sprintf(str, "%d", delay);
				gfx_mono_draw_string(str , 100, 16, &sysfont);
				gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
				gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
				gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
			}
			if(but_flag_1){
				int i=0;
				while(i<=200 && but_flag_1 == 1){
					delay_ms(1);
					i++;
					}
				
				if (i>200){
					delay += 100;
					sprintf(str, "%d", delay);
					gfx_mono_draw_string(str , 100, 16, &sysfont);
					
					gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
					gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
					gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
				}
				else if (i<200){
					delay -= 100;
					sprintf(str, "%d", delay);
					gfx_mono_draw_string(str , 100, 16, &sysfont);
					
					gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
					gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
					gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
				}
			
			
				but_flag_1 = 0;
			
			}
		}
	}
}