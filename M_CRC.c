#include "M_CRC.h"

u32 CRC_Tabela[256];

void CRC_Inicializar() {
	u08 Contador_2;

	for (u16 Contador = 0; Contador != 256; Contador++) {
		CRC_Tabela[Contador] = Contador;

		for (Contador_2 = 0; Contador_2 != 8; Contador_2++) {
			if (CRC_Tabela[Contador] & 1) {
				CRC_Tabela[Contador] = 0xEDB88320 ^ CRC_Tabela[Contador] >> 1;
			}
			else {
				CRC_Tabela[Contador] >>= 1;
			}
		}
	}
}

void CRC_Processar(u32* Resposta, u32 Tamanho, u08* Valor) {
	for (u32 Contador = 0; Contador != Tamanho; Contador++) {
		*Resposta = CRC_Tabela[(*Resposta ^ Valor[Contador]) & 255] ^ *Resposta >> 8;
	}
}