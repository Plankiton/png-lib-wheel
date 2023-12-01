#pragma once

#ifndef M_CRC_h
#define M_CRC_h

#include "..\T_Tipo.h"

u32 CRC_Tabela[256];

void CRC_Inicializar();
void CRC_Processar(u32* Resposta, u32 Tamanho, u08* Valor);

#endif