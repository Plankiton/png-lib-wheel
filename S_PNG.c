#include "S_PNG.h"

#include <corecrt_malloc.h>

#include "..\..\M_Util.h"

u32 PNG_Tabela_CRC[256];

void PNG_Inicializar() {
	u08 Contador_2;

	for (u16 Contador = 0; Contador != 256; Contador++) {
		PNG_Tabela_CRC[Contador] = Contador;

		for (Contador_2 = 0; Contador_2 != 8; Contador_2++) {
			if (PNG_Tabela_CRC[Contador] & 1) {
				PNG_Tabela_CRC[Contador] = 0xEDB88320 ^ PNG_Tabela_CRC[Contador] >> 1;
			}
			else {
				PNG_Tabela_CRC[Contador] >>= 1;
			}
		}
	}
}

void PNG_Novo(S_PNG* PNG, u32 Width, u32 Height) {
	PNG_Novo_Tipo(PNG, PNG_Tipo_R_G_B_A, Width, Height);
}

void PNG_Novo_Tipo(S_PNG* PNG, S_PNG_Tipo Valor, u32 Width, u32 Height) {
	PNG->Height = Height;
	PNG->PNG_Tipo = Valor;
	PNG->Width = Width;

	if (PNG->PNG_Tipo == PNG_Tipo_Grayscale || PNG->PNG_Tipo == PNG_Tipo_Palette) {
		PNG->_.Bytes_Por_Pixel = 1;
	}
	else if (PNG->PNG_Tipo == PNG_Tipo_Grayscale_A) {
		PNG->_.Bytes_Por_Pixel = 2;
	}
	else if (PNG->PNG_Tipo == PNG_Tipo_R_G_B) {
		PNG->_.Bytes_Por_Pixel = 3;
	}
	else if (PNG->PNG_Tipo == PNG_Tipo_R_G_B_A) {
		PNG->_.Bytes_Por_Pixel = 4;
	}

	if (PNG->PNG_Tipo == PNG_Tipo_R_G_B) {
		PNG->_.Data.Tamanho = (usz)PNG->Height * PNG->Width * 4;
	}
	else {
		PNG->_.Data.Tamanho = (usz)PNG->Height * PNG->Width * PNG->_.Bytes_Por_Pixel;
	}

	PNG->_.Data.Ponteiro = malloc(PNG->_.Data.Tamanho);
	if (!PNG->_.Data.Ponteiro) {
		exit(0);
	}

	u32 Tamanho = 8;
	Tamanho += 4 + 4 + 13 + 4;
	Tamanho += 4 + 4 + PNG->Height + PNG->Height * PNG->Width * PNG->_.Bytes_Por_Pixel + 4;
	Tamanho += 4 + 4 + 0 + 4;
	PNG->_.Data_2.Ponteiro = malloc(Tamanho);
	if (!PNG->_.Data_2.Ponteiro) {
		exit(0);
	}
}

void PNG_Fechar(S_PNG* PNG) {
	free(PNG->_.Data.Ponteiro);
	free(PNG->_.Data_2.Ponteiro);
}

void PNG_Atualizar(S_PNG* PNG, u32 Valor, u32 X, u32 Y) {
	if (PNG->PNG_Tipo == PNG_Tipo_Grayscale || PNG->PNG_Tipo == PNG_Tipo_Palette) {
		PNG->_.Data.Ponteiro[X + Y * PNG->Width] = Valor & 255;
	}
	else if (PNG->PNG_Tipo == PNG_Tipo_Grayscale_A) {
		((u16*)PNG->_.Data.Ponteiro)[X + Y * PNG->Width] = Valor & 65535;
	}
	else {
		((u32*)PNG->_.Data.Ponteiro)[X + Y * PNG->Width] = Valor;
	}
}

void PNG_Processar_CRC(S_PNG* PNG, u32 Tamanho, u08* Valor) {
	for (u32 Contador = 0; Contador != Tamanho; Contador++) {
		PNG->_.CRC = PNG_Tabela_CRC[(PNG->_.CRC ^ Valor[Contador]) & 255] ^ PNG->_.CRC >> 8;
	}
}

void PNG_Processar_Adicionar(S_PNG* PNG, u01 CRC, u32 Tamanho, u08 Tipo, usz Valor) {
	u08* Valor_2;
	if (Tipo == 0) {
		Valor_2 = &Valor;
	}
	else if (Tipo == 1) {
		Valor_2 = Valor;
	}

	if (CRC) {
		PNG_Processar_CRC(PNG, Tamanho, Valor_2);
	}
	
	for (u32 Contador = 0; Contador != Tamanho; Contador++) {
		PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = Valor_2[Contador];
	}
}

void PNG_Processar_Adicionar_Signature(S_PNG* PNG) {
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = 137;
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = 'P';
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = 'N';
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = 'G';
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = '\r';
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = '\n';
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = 26;
	PNG->_.Data_2.Ponteiro[PNG->_.Data_2.Tamanho++] = '\n';
}

u32 PNG_Processar_Inverter(u32 Valor) {
	u32 Resposta;
	Util_Inverter(&Resposta, 4, &Valor);

	return Resposta;
}

void PNG_Processar_Chunk_Novo(S_PNG* PNG, u08* Nome, u32 Tamanho) {
	PNG->_.CRC = u32_max;
	PNG_Processar_Adicionar(PNG, nao, 4, 0, PNG_Processar_Inverter(Tamanho));
	PNG_Processar_CRC(PNG, 4, Nome);
	PNG_Processar_Adicionar(PNG, nao, 4, 1, Nome);
}

void PNG_Processar(S_PNG* PNG) {
	u08 Bits = 8;
	u08 Compressao = 0;
	u01 Entrelaco = nao;
	u08 Filtro = 0;
	PNG->_.Data_2.Tamanho = 0;

	PNG_Processar_Adicionar_Signature(PNG);

	u32 Tamanho = 13;
	PNG_Processar_Chunk_Novo(PNG, "IHDR", Tamanho);
	PNG_Processar_Adicionar(PNG, sim, 4, 0, PNG_Processar_Inverter(PNG->Width));
	PNG_Processar_Adicionar(PNG, sim, 4, 0, PNG_Processar_Inverter(PNG->Height));
	PNG_Processar_Adicionar(PNG, sim, 1, 0, Bits);
	PNG_Processar_Adicionar(PNG, sim, 1, 0, PNG->PNG_Tipo);
	PNG_Processar_Adicionar(PNG, sim, 1, 0, Compressao);
	PNG_Processar_Adicionar(PNG, sim, 1, 0, Filtro);
	PNG_Processar_Adicionar(PNG, sim, 1, 0, Entrelaco);
	PNG_Processar_Adicionar(PNG, nao, 4, 0, PNG_Processar_Inverter(~PNG->_.CRC));

	u08 Contador_u08;
	u32 Contador_u32_2;

	Tamanho = PNG->Height + PNG->Height * PNG->Width * PNG->_.Bytes_Por_Pixel;
	PNG_Processar_Chunk_Novo(PNG, "IDAT", Tamanho);

	for (u32 Contador_u32 = 0; Contador_u32 != PNG->Height; Contador_u32++) {
		for (Contador_u32_2 = 0; Contador_u32_2 != PNG->Width; Contador_u32_2++) {
			if (!Contador_u32_2) {
				PNG_Processar_Adicionar(PNG, sim, 1, 0, 1);
			}

			for (Contador_u08 = 0; Contador_u08 != PNG->_.Bytes_Por_Pixel; Contador_u08++) {				
				PNG_Processar_Adicionar(PNG, sim, 1, 0, PNG->_.Data.Ponteiro[Contador_u32 * Contador_u32_2 + Contador_u08]);
			}
		}
	}

	PNG_Processar_Adicionar(PNG, nao, 4, 0, PNG_Processar_Inverter(~PNG->_.CRC));

	Tamanho = 0;
	PNG_Processar_Chunk_Novo(PNG, "IEND", Tamanho);
	PNG_Processar_Adicionar(PNG, nao, 4, 0, PNG_Processar_Inverter(~PNG->_.CRC));
}

u32 PNG_Valor(S_PNG* PNG, u16 X, u16 Y) {
	u32 Resposta;
	
	if (PNG->PNG_Tipo == PNG_Tipo_Grayscale || PNG->PNG_Tipo == PNG_Tipo_Palette) {
		 Resposta = PNG->_.Data.Ponteiro[X + Y * PNG->Width] & 255;
	}
	else if (PNG->PNG_Tipo == PNG_Tipo_Grayscale_A) {
		Resposta = (u16)PNG->_.Data.Ponteiro[X + Y * PNG->Width] & 65535;
	}
	else {
		Resposta = (u32)PNG->_.Data.Ponteiro[X + Y * PNG->Width];
	}

	return Resposta;
}