//Ozan Aydogan
//160202039
/*
 *16x02 Baglantilar
 /-------------------------------------------------/
 LCD             Tiva C
 -----           ------
 VSS              GROUND
 VDD              +5V
 V0               GROUND
 RS               PE1
 E                PE2
 D4               PD0
 D5               PD1
 D6               PD2
 D7               PD3
 BUTTON
 ----------
 0.25 		PB0
 0.50           PB1
 1.00           PB2
 Hesapla	PB3
 su	        PB4
 cay            PB5
 kahve	        PC4
 cikolata 	PC5
 biskuvi	PC6
 bitis		PA4
 reset		PA3

 LED
 ----
 LEDKirmizi          PE3
 LEDSari             PE4

 /--------------------------------------------------------/
 */
#include "inc/tm4c123gh6pm.h"
#include <stdlib.h>
#include "driverlib/sysctl.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#define BAUDRATE 600
#define LcdDataBits GPIO_PORTD_DATA_R

#define BIT_SET(PORT, PIN) do { \
        PORT |= 1<<PIN; \
} while (0)

#define BIT_RESET(PORT, PIN) do { \
        PORT &= ~(1<<PIN); \
} while (0)

void EkranaYazdir_Lcd_ikili(int x);
void EN(int x) ;
void resetEN();
void data(unsigned char c);
void cmd(unsigned char c);
void cmd_8bit(unsigned char c);
void EkraniTemizle(void) ;
void lcd_init() ;
void KonumXY(char x, char y);
void EkranaYazdir(char *p);

int button_bir, button_iki, button_uc, button_dort, button_bes, button_alti,
		button_yedi, button_sekiz, button_dokuz, button_on, button_onBir;
int kalanElde4;
char yedek4[100];
char sayac4[100], sayac6[100];
int virgulSolu, virgulSagi, sayac = 0;
int p, r, s;
char b4[100], b5[100], b6[100];

char yirmiBesKurus[50] = "20", elliKurus[50] = "20", birTl[50] = "10";

char suStok[50] = "30", cayStok[50] = "20", kahveStok[50] = "15",
		cikolataStok[50] = "50", biskuviStok[50] = "100";

volatile unsigned long gecici;

void port_yukle() {
	SysCtlClockSet( //Hazir kutuphane kullanilarak zaman hesabi yapilir.
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
					| SYSCTL_OSC_MAIN); //16 M 1 saniye
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

	gecici = SYSCTL_RCGC2_R;

	GPIO_PORTD_DIR_R = 0xff;
	GPIO_PORTD_DEN_R = 0xff;

	GPIO_PORTE_DIR_R = 0xff;
	GPIO_PORTE_DEN_R = 0xff;

//	GPIO_PORTA_DIR_R = 0xff;
//	GPIO_PORTA_DEN_R = 0xff;

	GPIO_PORTF_DIR_R = 0xff;
	GPIO_PORTF_DEN_R = 0xff;

	GPIO_PORTB_DIR_R |= 0x00;
	GPIO_PORTB_AFSEL_R &= ~0x00;
	GPIO_PORTB_DEN_R |= 0xFF;

	GPIO_PORTA_DIR_R |= 0x00;
	GPIO_PORTA_AFSEL_R &= ~0x00;
	GPIO_PORTA_DEN_R |= 0xFF;

	GPIO_PORTC_DIR_R |= 0x00;
	GPIO_PORTC_AFSEL_R &= ~0x00;
	GPIO_PORTC_DEN_R |= 0xFF;

}
void totalParaHesabi(int para) {
	int para4 = 0, para5 = 0, para6 = 0;

	int birlik = 0, yarimlik = 0, ceyreklik = 0;

	if (para == 25) {
		KonumXY(1, 1);
		EkranaYazdir("para : ");

		itoa(birlik, sayac4, 10);
		KonumXY(1, 13);
		EkranaYazdir(sayac4);

		KonumXY(1, 14);
		EkranaYazdir(".");
		itoa(para, sayac6, 10);
		KonumXY(1, 15);
		EkranaYazdir(sayac6);

	} else if (para == 50) {
		KonumXY(1, 1);
		EkranaYazdir("para : ");

		itoa(birlik, sayac4, 10);
		KonumXY(1, 13);
		EkranaYazdir(sayac4);

		KonumXY(1, 14);
		EkranaYazdir(".");
		itoa(para, sayac6, 10);
		KonumXY(1, 15);
		EkranaYazdir(sayac6);
	} else if (para == 75) {
		KonumXY(1, 1);
		EkranaYazdir("para : ");

		itoa(birlik, sayac4, 10);
		KonumXY(1, 13);
		EkranaYazdir(sayac4);

		KonumXY(1, 14);
		EkranaYazdir(".");
		itoa(para, sayac6, 10);
		KonumXY(1, 15);
		EkranaYazdir(sayac6);
	} else {

		para4 = para;
		while (para >= 100) {
			birlik++;
			para = para - 100;
		}
		para5 = para;
		while (para >= 50) {
			yarimlik++;
			para = para - 50;
		}
		para6 = para;
		while (para >= 25) {
			ceyreklik++;
			para = para - 25;
		}

		if (birlik != 0) {
			EkraniTemizle();
			SysCtlDelay(8000000);
			kalanElde4 = para4 - (birlik * 100);

			if (kalanElde4 >= 100) // sayi = 733
					{
				virgulSolu = kalanElde4 / 100; // 733 / 100 = 7 virgulun solu;
				virgulSagi = kalanElde4 - (virgulSolu * 100); //virgulsagi = 33;

				KonumXY(1, 1);
				EkranaYazdir("para : ");

				itoa(virgulSolu, yedek4, 10);
				KonumXY(1, 11);
				EkranaYazdir(yedek4);

				KonumXY(1, 14);
				EkranaYazdir(".");

				itoa(virgulSagi, b4, 10);
				KonumXY(1, 15);
				EkranaYazdir(b4);

			} else if (kalanElde4 >= 10) //sayi=34;
					{
				virgulSagi = kalanElde4;

				KonumXY(1, 1);
				EkranaYazdir("para : ");

				itoa(birlik, sayac4, 10);
				KonumXY(1, 11);
				EkranaYazdir(sayac4);

				KonumXY(1, 14);
				EkranaYazdir(".");

				itoa(virgulSagi, b4, 10);
				KonumXY(1, 15);
				EkranaYazdir(b4);

			} else if (kalanElde4 >= 1) //sayi = 6;
					{
				virgulSagi = kalanElde4;

				KonumXY(1, 1);
				EkranaYazdir("para : ");

				itoa(birlik, sayac4, 10);
				KonumXY(1, 11);
				EkranaYazdir(sayac4);

				KonumXY(1, 14);
				EkranaYazdir(".");

				KonumXY(1, 15);
				EkranaYazdir("0");

				itoa(virgulSagi, b4, 10);
				KonumXY(1, 16);
				EkranaYazdir(b4);

			} else {
				virgulSagi = kalanElde4;

				KonumXY(1, 1);
				EkranaYazdir("para : ");

				itoa(birlik, sayac4, 10);
				KonumXY(1, 11);
				EkranaYazdir(sayac4);

				KonumXY(1, 14);
				EkranaYazdir(".");

				KonumXY(1, 15);
				EkranaYazdir("0");

				itoa(virgulSagi, b4, 10);
				KonumXY(1, 16);
				EkranaYazdir(b4);
			}
		}
	}
	SysCtlDelay(32000000);
}

void init_UARTstdio() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(0x00000001);
	GPIOPinConfigure(0x00000401);
	GPIOPinTypeUART(0x40004000, 0x00000001 | 0x00000002);
	UARTConfigSetExpClk(0x40004000, SysCtlClockGet(), BAUDRATE,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE));
	UARTStdioConfig(0, BAUDRATE, SysCtlClockGet());
}

int main() {

	int suStokSayisi = atoi(suStok), cayStokSayisi = atoi(cayStok),
			kahveStokSayisi = atoi(kahveStok), cikolataStokSayisi = atoi(
					cikolataStok), biskuviStokSayisi = atoi(biskuviStok);
	port_yukle();
	lcd_init();
	init_UARTstdio();

	int suStokSayisi_1 = 0;
	int cayStokSayisi_1 = 0;
	int kahveStokSayisi_1 = 0;
	int cikolataStokSayisi_1 = 0;
	int biskuviStokSayisi_1 = 0;
	int count = 0;
	int paraustu = 0;
        int xyirmiBesKurus = atoi(yirmiBesKurus), xelliKurus = atoi(elliKurus), xbirTl = atoi(birTl) ;

	EkraniTemizle();
	SysCtlDelay(8000000);


	go2: EkraniTemizle();
	SysCtlDelay(8000000);
    count = 0;
	paraustu = 0;
	int paraUstu2 = 0;
	int yyirmiBesKurus = 0, yelliKurus = 0, ybirTl = 0;
	KonumXY(1, 1);
	EkranaYazdir("para");
	KonumXY(2, 1);
	EkranaYazdir("atiniz");
	int atilanPara = 0;
	int odenecekPara = 0;
	int myirmiBesKurus = 0;
        int melliKurus = 0;
	int mbirTl = 0;
	while (1) {
		button_bir = GPIO_PORTB_DATA_R & 0x01;             //PB0 bagli 1. buton.
		button_iki = GPIO_PORTB_DATA_R & 0x02;             //PB1 bagli 2. buton.
		button_uc = GPIO_PORTB_DATA_R & 0x04;              //PB2 bagli 3. buton.
		button_dort = GPIO_PORTB_DATA_R & 0x08;            //PB3 bagli 4. buton.

	        myirmiBesKurus = 0;
                melliKurus = 0;
	        mbirTl = 0;
		suStokSayisi_1 = 0;
		cayStokSayisi_1 = 0;
		kahveStokSayisi_1 = 0;
		cikolataStokSayisi_1 = 0;
		biskuviStokSayisi_1 = 0;


		if (button_bir == 0) {
			EkraniTemizle();
			SysCtlDelay(3200000);
			EkranaYazdir("0.25 attiniz.");

			xyirmiBesKurus++;
			atilanPara = atilanPara + 25;

		} else if (button_iki == 0) {
			EkraniTemizle();
			SysCtlDelay(3200000);

			EkranaYazdir("0.50 attiniz.");
			xelliKurus++;
			atilanPara = atilanPara + 50;

		} else if (button_uc == 0) {

			EkraniTemizle();
			SysCtlDelay(3200000);
			EkranaYazdir("1.00 attiniz.");
			xbirTl++;
			atilanPara = atilanPara + 100;

		} else if (button_dort == 0) {
			EkraniTemizle();
			SysCtlDelay(3200000);

			totalParaHesabi(atilanPara);

			while (1) {
				KonumXY(1, 1);
				EkranaYazdir("urun seciniz....");
				SysCtlDelay(3200000);

				button_bes = GPIO_PORTB_DATA_R & 0x10;     //PB4 bagli 5. buton.
				button_alti = GPIO_PORTB_DATA_R & 0x20;    //PB5 bagli 6. buton.
				button_yedi = GPIO_PORTC_DATA_R & 0x10;    //PC4 bagli 7. buton.
				button_sekiz = GPIO_PORTC_DATA_R & 0x20;   //PC5 bagli 8. buton.
				button_dokuz = GPIO_PORTC_DATA_R & 0x40;   //PC6 bagli 9. buton.
				button_on = GPIO_PORTA_DATA_R & 0x10;     //PA4 bagli 10. buton.
				button_onBir = GPIO_PORTA_DATA_R & 0x08;  //PA3 bagli 11. buton.
				if (button_onBir != 0) {


					if (button_bes == 0) {

						//1-Su
						EkraniTemizle();
						SysCtlDelay(3200000);

						if (suStokSayisi != 0
								&& (atilanPara - odenecekPara >= 50)) {
							suStokSayisi--;
							suStokSayisi_1++;
							odenecekPara = odenecekPara + 50;
							KonumXY(1, 1);
							EkranaYazdir("su sectiniz.");
							SysCtlDelay(3200000);


						} else if ((atilanPara - odenecekPara < 50)) {
							KonumXY(1, 1);
							EkranaYazdir("yetersiz bakiye");
						} else {
							KonumXY(1, 1);
							EkranaYazdir("su kalmadi!!");
						}
						SysCtlDelay(16000000);
					} else if (button_alti == 0) {
						//2-Cay
						EkraniTemizle();
						SysCtlDelay(3200000);

						if (cayStokSayisi != 0
								&& (atilanPara - odenecekPara >= 100)) {
							cayStokSayisi--;
							cayStokSayisi_1++;
							odenecekPara = odenecekPara + 100;
							KonumXY(1, 1);
							EkranaYazdir("cay sectiniz.");
						} else if ((atilanPara - odenecekPara < 100)) {
							KonumXY(1, 1);
							EkranaYazdir("yetersiz bakiye");
						} else {
							KonumXY(1, 1);
							EkranaYazdir("cay kalmadi!!");
						}
						SysCtlDelay(16000000);
					} else if (button_yedi == 0) {
						//3-Kahve
						EkraniTemizle();
						SysCtlDelay(3200000);

						if (kahveStokSayisi != 0
								&& (atilanPara - odenecekPara >= 150)) {
							kahveStokSayisi--;
							kahveStokSayisi_1++;
							odenecekPara = odenecekPara + 150;
							KonumXY(1, 1);
							EkranaYazdir("kahve sectiniz.");
						} else if ((atilanPara - odenecekPara < 150)) {
							KonumXY(1, 1);
							EkranaYazdir("yetersiz bakiye");
						} else {
							KonumXY(1, 1);
							EkranaYazdir("kahve kalmadi!!");
						}
						SysCtlDelay(16000000);
					} else if (button_sekiz == 0) {
						//4-Cikolata
						EkraniTemizle();
						SysCtlDelay(3200000);

						if (cikolataStokSayisi != 0
								&& (atilanPara - odenecekPara >= 175)) {
							cikolataStokSayisi--;
							cikolataStokSayisi_1++;
							odenecekPara = odenecekPara + 175;
							KonumXY(1, 1);
							EkranaYazdir("cikolata sectin");
						} else if ((atilanPara - odenecekPara < 175)) {
							KonumXY(1, 1);
							EkranaYazdir("yetersiz bakiye");
						} else {
							KonumXY(1, 1);
							EkranaYazdir("cikolata kalmadi");
						}
						SysCtlDelay(16000000);
					} else if (button_dokuz == 0) {
						//5-Biskuvi
						EkraniTemizle();
						SysCtlDelay(3200000);
						if (biskuviStokSayisi != 0
								&& (atilanPara - odenecekPara >= 200)) {
							biskuviStokSayisi--;
							biskuviStokSayisi_1++;
							odenecekPara = odenecekPara + 200;
							KonumXY(1, 1);
							EkranaYazdir("biskuvi sectin");
						} else if ((atilanPara - odenecekPara < 200)) {
							KonumXY(1, 1);
							EkranaYazdir("yetersiz bakiye");
						} else {
							KonumXY(1, 1);
							EkranaYazdir("biskuvi kalmadi");
						}
						SysCtlDelay(16000000);
					} else if (button_on == 0) {

						int random = 0;

						random = 0;
						switch (random) {
						case 0: {
							GPIO_PORTE_DATA_R |= 0x10; // sari led yanar PE4.
							EkraniTemizle();
							SysCtlDelay(3200000);
							GPIO_PORTE_DATA_R &= ~(0x10); // sari led soner PE4.
							EkranaYazdir("islem basarili");
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);

							itoa(suStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("su sayisi: %d", suStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cayStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cay sayisi: %d", cayStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(kahveStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("kahve sayisi: %d", kahveStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(biskuviStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("biskuvi sayisi: %d", biskuviStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cikolataStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cikolata sayisi: %d",
									cikolataStokSayisi);
							EkraniTemizle();
							SysCtlDelay(3200000);
							paraustu = atilanPara - odenecekPara;
							paraUstu2 = atilanPara - odenecekPara;
							while (paraUstu2 != 0) {
								while (paraustu != 0) {
									if (paraustu >= 100) {
										paraustu = paraustu - 100;
										ybirTl++;

									} else if (50 <= paraustu) {
										paraustu = paraustu - 50;
										yelliKurus++;

									} else if (paraustu <= 25) {
										paraustu = paraustu - 25;
										yyirmiBesKurus++;

									}

								}

								if (yyirmiBesKurus > xyirmiBesKurus
										|| yelliKurus > xelliKurus
										|| ybirTl > xbirTl) {
									KonumXY(1, 1);
									EkranaYazdir("para kasada yok");
									SysCtlDelay(3200000);
								} else {
									while (paraUstu2 != 0) {
										if (paraUstu2 >= 100 && xbirTl != 0) {
											paraUstu2 = paraUstu2 - 100;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("1 lira odendi");
											SysCtlDelay(3200000);
											xbirTl--;
											mbirTl++;

										} else if (50 <= paraUstu2
												&& xelliKurus != 0) {
											paraUstu2 = paraUstu2 - 50;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,50 lira odendi");
											SysCtlDelay(3200000);
											xelliKurus--;
											melliKurus++;

										} else if (paraUstu2 <= 25
												&& xyirmiBesKurus != 0) {
											paraUstu2 = paraUstu2 - 25;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,25 lira odendi");
											SysCtlDelay(3200000);
											xyirmiBesKurus--;
											myirmiBesKurus++;

										}

									}

								}
								goto go2;}
						}
							break;
						case 1: {
							GPIO_PORTE_DATA_R |= 0x08; // kirmizi led yanar PE3 .
							EkraniTemizle();
							SysCtlDelay(3200000);

							KonumXY(1, 1);
							EkranaYazdir("para takildi!!!!");
							GPIO_PORTE_DATA_R &= ~(0x08); // kirmizi led soner PE3 .
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							EkranaYazdir("para iade edildi.");
							SysCtlDelay(3200000);
							goto go2;

						}
							break;
						case 2: {
							GPIO_PORTE_DATA_R |= 0x10; // sari led yanar PE4.
							EkraniTemizle();
							SysCtlDelay(3200000);
							GPIO_PORTE_DATA_R &= ~(0x10); // sari led soner PE4.
							EkranaYazdir("islem basarili");
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);

							itoa(suStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("su sayisi: %d", suStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cayStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cay sayisi: %d", cayStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(kahveStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("kahve sayisi: %d", kahveStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(biskuviStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("biskuvi sayisi: %d", biskuviStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cikolataStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cikolata sayisi: %d",
									cikolataStokSayisi);
							EkraniTemizle();
							SysCtlDelay(3200000);
							paraustu = atilanPara - odenecekPara;
							paraUstu2 = atilanPara - odenecekPara;
							while (paraUstu2 != 0) {
								while (paraustu != 0) {
									if (paraustu >= 100) {
										paraustu = paraustu - 100;
										ybirTl++;

									} else if (50 <= paraustu) {
										paraustu = paraustu - 50;
										yelliKurus++;

									} else if (paraustu <= 25) {
										paraustu = paraustu - 25;
										yyirmiBesKurus++;

									}

								}

								if (yyirmiBesKurus > xyirmiBesKurus
										|| yelliKurus > xelliKurus
										|| ybirTl > xbirTl) {
									KonumXY(1, 1);
									EkranaYazdir("para kasada yok");
									SysCtlDelay(3200000);
								} else {
									while (paraUstu2 != 0) {
										if (paraUstu2 >= 100 && xbirTl != 0) {
											paraUstu2 = paraUstu2 - 100;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("1 lira odendi");
											SysCtlDelay(3200000);
											xbirTl--;
											mbirTl++;

										} else if (50 <= paraUstu2
												&& xelliKurus != 0) {
											paraUstu2 = paraUstu2 - 50;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,50 lira odendi");
											SysCtlDelay(3200000);
											xelliKurus--;
											melliKurus++;

										} else if (paraUstu2 <= 25
												&& xyirmiBesKurus != 0) {
											paraUstu2 = paraUstu2 - 25;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,25 lira odendi");
											SysCtlDelay(3200000);
											xyirmiBesKurus--;
											myirmiBesKurus++;

										}

									}

								}

								goto go2;}
						}
							break;
						case 3: {
							GPIO_PORTE_DATA_R |= 0x10; // sari led yanar PE4.
							EkraniTemizle();
							SysCtlDelay(3200000);
							GPIO_PORTE_DATA_R &= ~(0x10); // sari led soner PE4.
							EkranaYazdir("islem basarili");
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);

							itoa(suStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("su sayisi: %d", suStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cayStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cay sayisi: %d", cayStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(kahveStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("kahve sayisi: %d", kahveStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(biskuviStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("biskuvi sayisi: %d", biskuviStokSayisi);
							SysCtlDelay(3200000);
							EkraniTemizle();
							SysCtlDelay(3200000);
							itoa(cikolataStokSayisi, sayac6, 10);
							KonumXY(1, 1);
							EkranaYazdir(sayac6);
							UARTprintf("cikolata sayisi: %d",
									cikolataStokSayisi);
							EkraniTemizle();
							SysCtlDelay(3200000);

							paraustu = atilanPara - odenecekPara;
							paraUstu2 = atilanPara - odenecekPara;
							while (paraUstu2 != 0) {
								while (paraustu != 0) {
									if (paraustu >= 100) {
										paraustu = paraustu - 100;
										ybirTl++;

									} else if (50 <= paraustu) {
										paraustu = paraustu - 50;
										yelliKurus++;

									} else if (paraustu <= 25) {
										paraustu = paraustu - 25;
										yyirmiBesKurus++;

									}

								}

								if (yyirmiBesKurus > xyirmiBesKurus
										|| yelliKurus > xelliKurus
										|| ybirTl > xbirTl) {
									KonumXY(1, 1);
									EkranaYazdir("para kasada yok");
									SysCtlDelay(3200000);
								} else {
									while (paraUstu2 != 0) {
										if (paraUstu2 >= 100 && xbirTl != 0) {
											paraUstu2 = paraUstu2 - 100;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("1 lira odendi");
											SysCtlDelay(3200000);
											xbirTl--;
											mbirTl++;

										} else if (50 <= paraUstu2
												&& xelliKurus != 0) {
											paraUstu2 = paraUstu2 - 50;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,50 lira odendi");
											SysCtlDelay(3200000);
											xelliKurus--;
											melliKurus++;

										} else if (paraUstu2 <= 25
												&& xyirmiBesKurus != 0) {
											paraUstu2 = paraUstu2 - 25;
											EkraniTemizle();
											SysCtlDelay(3200000);
											EkranaYazdir("0,25 lira odendi");
											SysCtlDelay(3200000);
											xyirmiBesKurus--;
											myirmiBesKurus++;

										}

									}

								}
							}
							goto go2;
						}

						}
						SysCtlDelay(16000000);

					}
				} else if (button_onBir == 0) {
					EkraniTemizle();
					SysCtlDelay(3200000);

					KonumXY(1, 1);
					EkranaYazdir("resetlendi!!");

					SysCtlDelay(3200000);
					EkraniTemizle();
					SysCtlDelay(3200000);
					xyirmiBesKurus += myirmiBesKurus;
					xelliKurus  += melliKurus;
					xbirTl += mbirTl;
					SysCtlDelay(3200000);
					EkraniTemizle();
					SysCtlDelay(3200000);
					EkranaYazdir("para iade edildi.");
					SysCtlDelay(3200000);
					suStokSayisi += suStokSayisi_1;
					cayStokSayisi += cayStokSayisi_1;
					kahveStokSayisi += kahveStokSayisi_1;
					cikolataStokSayisi += cikolataStokSayisi_1;
					biskuviStokSayisi += biskuviStokSayisi_1;
					EkraniTemizle();
					SysCtlDelay(3200000);
					EkranaYazdir("urun iade edildi");
					SysCtlDelay(3200000);
					goto go2;

				}

			}

		}

	}
}

void EkranaYazdir_Lcd_ikili(int x) {
	GPIO_PORTE_DATA_R = ((GPIO_PORTE_DATA_R & ~(1 << 1)) | (x << 1));
}

void EN(int x) {
	GPIO_PORTE_DATA_R = ((GPIO_PORTE_DATA_R & ~(1 << 2)) | (x << 2));
}

void resetEN() {
	EN(1);
	SysCtlDelay(500);
	EN(0);
}

void data(unsigned char c) {
	EkranaYazdir_Lcd_ikili(1);
	SysCtlDelay(400);
	LcdDataBits = c >> 4;
	resetEN();
	LcdDataBits = c;
	resetEN();
}

void cmd(unsigned char c) {
	EkranaYazdir_Lcd_ikili(0);
	SysCtlDelay(400);
	LcdDataBits = c >> 4;
	resetEN();
	LcdDataBits = c;
	resetEN();
}

void cmd_8bit(unsigned char c) {
	EkranaYazdir_Lcd_ikili(0);
	SysCtlDelay(15000);
	LcdDataBits = (c);
	resetEN();
}

void EkraniTemizle(void) {
	cmd(0x01);
	SysCtlDelay(30000);
}

void lcd_init() {
	cmd_8bit(0x30);
	cmd_8bit(0x30);
	cmd_8bit(0x30);
	cmd_8bit(0x20);
	cmd(0x28);
	cmd(0x0c);
	EkraniTemizle();
	cmd(0x6);
}

void KonumXY(char x, char y) {
	if (x == 1)
		cmd(0x80 + ((y - 1) % 16));
	else
		cmd(0xC0 + ((y - 1) % 16));
}

void EkranaYazdir(char *p) {
	while (*p)
		data(*p++);
}
