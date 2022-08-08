#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>
struct struct_sensor //esquema para envio dos dados dos sensores
{
  uint8_t ID1; //id do sensor
  uint8_t Config1; // flags de configuração
  int64_t dado1; // dados armazenados
  uint8_t ID2;
  uint8_t Config2;
  int64_t dado2;
  uint8_t ID3;
  uint8_t Config3;
  int64_t dado3;
};

struct struct_Painel //dados para serem enviados para o painel
{
  uint16_t RPM;
  uint8_t Stat_RPM;
  uint8_t Marcha;
  int16_t Temp_Motor;
  int16_t Bateria;
  int8_t Temp_Oleo;
  int8_t Temp_ADM;
  uint16_t erros;
};

struct struct_ECU //dados para serem coletados da ecu
{
  //  Tipo     nome             descrição                           resolução  unidade
  //
  uint16_t Segundos;     // Contador_de_Tempo_UpTime                (1)       seg
  uint16_t Inje1;        // Tempo_de_Injecao do Injetor 1           (0,001)   mseg
  uint16_t inje_AUX;     // Tempo_de_Injecao_da Bancada Auxiliar    (0,001)   mseg
  uint16_t RPM;          // Rotação                                 (1)       RPM
  int16_t Avan_igni;     // Avanco_da_Ignicao                       (0,1)     degAPMS
  uint8_t res1;          // Reservado                               (1)       -
  uint8_t res2;          // Reservado                               (1)       -
  uint8_t res3;          // Reservado                               (1)       -
  uint8_t Ar_Comb;       // Relacao_Ar/Comb,_Alvo-Controlador       (0,01)    Lamdba
  uint8_t Stat_RPM;      // Status_do_Sensor_de_Rotacao             (1)       -
  uint8_t Marcha;        // Gear Position                           (1)       Counts
  int16_t Pres_Bar;      // Pressao_Barometrica                     (0,1)     KPa
  int16_t Pres_MAP;      // Pressao_MAP_Admissao                    (0,1)     KPa
  int16_t Temp_ADM;      // Temperatura_do_Ar_da_Admissao_IAT       (0,1)     degC
  int16_t Temp_Motor;    // Temperatura_do_Motor (ET)               (0,1)     degC
  int16_t TPS;           // Posicao_da_Borboleta                    (0,1)     %
  int16_t Bateria;       // Tensao_da_Bateria                       (0,1)     Volts
  int16_t lam_Ar_Comb;   // Lambda:_Relacao_Ar/Combustive           (0,001)   Lamdba
  int16_t fase;          // Fase_da_Injecao                         (0,1)     degAPMS
  int16_t Temp_escap;    // Exhaust Temp                            (0,1)     degC
  int16_t cor_EGO;       // Correcao_EGO                            (0,1)     %
  uint16_t res4;         // Reservado                               (1)       -
  int16_t Velocidade;    // Ground_Speed                            (0,1)     Km/h
  int16_t Temp_Oleo;     // Oil Temp                                (1)       %
  int16_t Acel_Enr;      // Enriquecimento_Aceleracao               (0,1)     mseg
  int16_t Empob_Des;     // Empobrecimento_Desaceleracao            (1)       %
  int16_t lam2_Ar_Comb;  // Lambda2:_Relacao_Ar Combustivel         (0,1)     %
  int16_t Cor_Total;     // Correcao_Total                          (1)       %
  int16_t Cor_EGO2;      // Correcao EGO 2                          (0,001)   Lamdba
  uint8_t duty_1;        // Ciclo_Ativo_Saida_Injetor_1_(duty_1)    (1)       %
  uint8_t duty_2;        // Ciclo_Ativo_Saida_Injetor_2_(duty_2)    (1)       %
  int16_t Comp_Ar_Lent;  // Abertura_da_compensacao_por_Ar_da_Lenta (0,1)     %
  int16_t MAF_Rate1;     // MAF Rate 1                              (0,1)     Km/h
  int16_t TPS_change;    // Variacao_do_Acelerador                  (0,1)     %/s
  int16_t MAP_Change;    // Variacao_da_Pressao                     (0,1)     KPa/s
  int16_t Carga_Bobina;  // Tempo_de_Carga_da_Bobina                (0,1)     mseg
  int16_t Tens_AUX1;     // Tensao_da_Entrada_Auxiliar_1            (0,01)    Volts
  int16_t RPM_Alvo_lent; // Rotacao_Alvo_Marcha_Lenta               (1)       RPM
  int16_t Pos_Pedal2;    // Posicao_do_Pedal_2                      (0,1)     %
  int16_t Tens_Sond1;    // Tensao da Sonda 1 (volts)               (0,01)    Volts
  uint16_t erros;        // Código_dos_Erros                        (1)       -
  int16_t Tens_AUX2;     // Tensao_da_Entrada_Auxiliar_2            (0,01)    Volts
  int16_t Press_Oleo;    // Pressao_Oleo                            (0,1)     kgf/cm²
  int16_t Press_Comb;    // Pressao_Combustivel                     (0,1)     kgf/cm²
  int16_t Pos_Pedal1;    // Posicao_do_Pedal_1                      (0,1)     %
};

struct struct_PDM //esquema para envio dos dados da PDM
{ // ID0 - ID7 correntes dos canais, ID8 temperatura, ID9 Humidade
  uint8_t ID0;   
  uint16_t dado0;
  uint8_t ID1;
  uint16_t dado1;
  uint8_t ID2;
  uint16_t dado2;
  uint8_t ID3;
  uint16_t dado3;
  uint8_t ID4;
  uint16_t dado4;
  uint8_t ID5;
  uint16_t dado5;
  uint8_t ID6;
  uint16_t dado6;
  uint8_t ID7;
  uint16_t dado7;
  uint8_t ID8;
  int16_t dado8;
  uint8_t ID9;
  uint8_t dado9;
};

struct struct_Canais //esquema para envio dos dados da PDM
{
  bool canal;
  uint8_t id; 
};

#endif