#pragma once

#ifndef M_Deflate_h
#define M_Deflate_h

#include "S_Adler.h"
#include "..\S_Vetor.h"

typedef enum {
	Deflate_Tipo_Uncompressed,
	Deflate_Tipo_Huffman_Fixed,
	Deflate_Tipo_Huffman_Dynamic,
} M_Deflate_Tipo;

void Deflate_Processar(S_Adler* Adler, S_Vetor* Resposta, M_Deflate_Tipo Tipo, u01 Ultimo, S_Memoria* Valor);

#endif