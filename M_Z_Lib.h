#pragma once

#ifndef M_Z_Lib_h
#define M_Z_Lib_h

#include "..\S_Vetor.h"

typedef enum {
	Z_Lib_Flags_Level_Fastest,
	Z_Lib_Flags_Level_Fast,
	Z_Lib_Flags_Level_Default,
	Z_Lib_Flags_Level_Slowest,
} M_Z_Lib_Flags_Level;

void Z_Lib_Processar(S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor);
void Z_Lib_Processar_2(M_Z_Lib_Flags_Level Flags_Level, S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor);
void Z_Lib_Processar_3(u08 Compression_Info, u08 Compression_Method, u01 Flags_Dictionary, M_Z_Lib_Flags_Level Flags_Level, S_Vetor* Resposta, u16 Tamanho, S_Memoria* Valor);

#endif