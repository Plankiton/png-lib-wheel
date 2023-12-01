#include "S_Adler.h"

#define Adler_Base 65521

void Adler_Novo(S_Adler* Adler) {
	Adler->Resposta_2 = &Adler->Resposta;
	Adler->_.S_1 = 1;
	Adler->_.S_2 = 0;
}

void Adler_Adicionar(S_Adler* Adler, u08 Valor) {
	Adler->_.S_1 = (Adler->_.S_1 + Valor) % Adler_Base;
	Adler->_.S_2 = (Adler->_.S_1 + Adler->_.S_2) % Adler_Base;
}

void Adler_Processar(S_Adler* Adler) {
	Adler->Resposta = Adler->_.S_1 | Adler->_.S_2 << 16;
}