#include "M_Deflate.h"

void Deflate_Processar_Adicionar(S_Adler* Adler, S_Vetor* Resposta, u16 Tamanho, u08* Valor) {
	for (u16 Contador = 0; Contador != Tamanho; Contador++) {
		if (Adler) {
			Adler_Adicionar(Adler, Valor[Contador]);
		}
			
		Vetor_Adicionar(Resposta, 0, Valor[Contador]);
	}
}

void Deflate_Processar(S_Adler* Adler, S_Vetor* Resposta, M_Deflate_Tipo Tipo, u01 Ultimo, S_Memoria* Valor) {
	u16 Contador;
	if (Tipo == Deflate_Tipo_Uncompressed) {
		u16 Valor_2 = ~Valor->Tamanho;

		Deflate_Processar_Adicionar(0, Resposta, 1, &Ultimo);
		Deflate_Processar_Adicionar(0, Resposta, 2, &Valor->Tamanho);
		Deflate_Processar_Adicionar(0, Resposta, 2, &Valor_2);

		for (Contador = 0; Contador != Valor->Tamanho; Contador++) {
			Deflate_Processar_Adicionar(Adler, Resposta, 1, &Valor->Ponteiro[Contador]);
		}
	}
}