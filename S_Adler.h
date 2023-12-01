#pragma once

#ifndef S_Adler_h
#define S_Adler_h

#include "..\T_Tipo.h"

typedef struct {
	u16 S_1;
	u16 S_2;
} S_Adler_Privado;

typedef struct {
	u32 Resposta;
	u08* Resposta_2;
	S_Adler_Privado _;
} S_Adler;

void Adler_Novo(S_Adler* Adler);
void Adler_Adicionar(S_Adler* Adler, u08 Valor);
void Adler_Processar(S_Adler* Adler);

#endif