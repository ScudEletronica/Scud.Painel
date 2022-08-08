#ifndef CONS_H
#define CONS_H
#include <Arduino.h>
#include <esp_now.h>
#include "structures.h"

// private:
void fastWifi();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len);

// public:
void AddPear(uint8_t Address[6]);
void setupESPNOW(uint8_t newAddress[6], int64_t *Dados, uint8_t *Config, bool *Canais);
void sendPainel();
void setUpLora(uint8_t SS, uint8_t reset, uint8_t dio0);
void sendLora();
void sendGSM();
esp_err_t ESPNOWSend(uint8_t Address[6],struct_Canais *myData);
esp_err_t ESPNOWSend(uint8_t Address[6],struct_ECU *myData);
esp_err_t ESPNOWSend(uint8_t Address[6],struct_Painel *myData);
esp_err_t ESPNOWSend(uint8_t Address[6],struct_PDM *myData);
esp_err_t ESPNOWSend(uint8_t Address[6],struct_sensor *myData);

#endif