#include "M_Z_Lib.h"

#include "M_Deflate.h"

/*
Z_Lib_Compression_Info   = 0 ==>   256 bytes
Z_Lib_Compression_Info   = 7 ==> 32768 bytes
Z_Lib_Compression_Method = 8 ==> Deflate
*/

#define Z_Lib_Compression_Info				    7
#define Z_Lib_Compression_Flags_Check_Max	   31
#define Z_Lib_Compression_Flags_Dictionary	    0
#define Z_Lib_Compression_Method			    8

void Z_Lib_Processar(S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor) {
	Z_Lib_Processar_3(Z_Lib_Compression_Info, Z_Lib_Compression_Method, Z_Lib_Compression_Flags_Dictionary, Z_Lib_Flags_Level_Slowest, Resposta, Tamanho, Valor);
}

void Z_Lib_Processar_2(M_Z_Lib_Flags_Level Flags_Level, S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor) {
	Z_Lib_Processar_3(Z_Lib_Compression_Info, Z_Lib_Compression_Method, Z_Lib_Compression_Flags_Dictionary, Flags_Level, Resposta, Tamanho, Valor);
}

void Z_Lib_Processar_3(u08 Compression_Info, u08 Compression_Method, u01 Flags_Dictionary, M_Z_Lib_Flags_Level Flags_Level, S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor) {
	u08 Compression = Compression_Info << 4 | Compression_Method;
	u01 Ultimo = nao;

	S_Adler Adler;
	Adler_Novo(&Adler);

	u08 Flags = Flags_Dictionary << 5 | Flags_Level << 6;
	Flags |= Z_Lib_Compression_Flags_Check_Max - (Compression * 256 + Flags) % Z_Lib_Compression_Flags_Check_Max;

	u16 Tamanho_2 = Valor->Tamanho / Tamanho;
	if (Valor->Tamanho % Tamanho) {
		Tamanho_2 += 1;
	}

	M_Deflate_Tipo Tipo = Deflate_Tipo_Uncompressed;

	S_Memoria Valor_2;
	Valor_2.Ponteiro = Valor->Ponteiro;
	Valor_2.Tamanho = Tamanho;

	Vetor_Adicionar(Resposta, 0, Compression);
	Vetor_Adicionar(Resposta, 0, Flags);

	for (u16 Contador = 0; Contador != Tamanho_2; Contador++) {
		if (Contador + 1 == Tamanho_2) {
			Ultimo = sim;
			Valor_2.Tamanho = Valor->Tamanho - (usz)Contador * Tamanho;
		}

		Deflate_Processar(&Adler, Resposta, Tipo, Ultimo, &Valor_2);

		Valor_2.Ponteiro += Tamanho;
	}

	Adler_Processar(&Adler);
	for (u08 Contador = 3; Contador != u08_max; Contador--) {
		Vetor_Adicionar(Resposta, 0, Adler.Resposta_2[Contador]);
	}
}