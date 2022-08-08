//libs oficiais
#include <Arduino.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <WiFi.h>

//definições locais
#include "structures.h"
#include "sensorIDs.h"
#include "VariaveisCompilacao.h"


struct_sensor mySensor; // Cria uma struct para os dados espnow dos sensores
struct_PDM myPDM;
struct_Canais myCanais;
struct_Painel myPainel;
#define QuantDados 255 // quantidade de sensores

int64_t *DadosSens;
uint8_t *ConfigSens;
bool *canaisPDM;
void fastWifi() // deprecated
{
  // Disabling AMPDU is necessary to set a fix rate
  wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT(); //We use the default config ...
  my_config.ampdu_tx_enable = 0;                             //... and modify only what we want.
  esp_wifi_init(&my_config);                                 //set the new config
  esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_11M_L); //1M = 800pps; 54M = 5kpps
  esp_wifi_start();
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{ // Callback when data is sent
#ifdef debug
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
#else
  if (status != 0) // erro no envio
  {
    // manda novamente //TODO
  }
#endif
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) // le os dados que vem das placas de sensores e painel
{
  #ifdef debug
  Serial.println("recebido");
  Serial.println(data_len);
  Serial.println(*incomingData,HEX);
  #endif
  switch (data_len)
  {
  case sizeof(struct_sensor):
    memcpy(&mySensor, incomingData, sizeof(mySensor)); // coloca os dados no formato de estrutura
    ConfigSens[mySensor.ID1] = mySensor.Config1;    // carrega os dados no vetor de configuração de acordo com o ID
    DadosSens[mySensor.ID1] = mySensor.dado1;      // carrega os dados no vetor de dados de acordo com o ID
    ConfigSens[mySensor.ID2] = mySensor.Config2;
    DadosSens[mySensor.ID2] = mySensor.dado2;
    ConfigSens[mySensor.ID3] = mySensor.Config3;
    DadosSens[mySensor.ID3] = mySensor.dado3;
    break;
  case sizeof(struct_PDM):
    memcpy(&myPDM, incomingData, sizeof(myPDM));
    DadosSens[myPDM.ID0] = myPDM.dado0;
    DadosSens[myPDM.ID1] = myPDM.dado1;
    DadosSens[myPDM.ID2] = myPDM.dado2;
    DadosSens[myPDM.ID3] = myPDM.dado3;
    DadosSens[myPDM.ID4] = myPDM.dado4;
    DadosSens[myPDM.ID5] = myPDM.dado5;
    DadosSens[myPDM.ID6] = myPDM.dado6;
    DadosSens[myPDM.ID7] = myPDM.dado7;
    DadosSens[myPDM.ID8] = myPDM.dado8;
    DadosSens[myPDM.ID9] = myPDM.dado9;
    break;
  case sizeof(struct_Canais):
    memcpy(&myCanais, incomingData, sizeof(myCanais));
    Serial.println(myCanais.id-1);
    Serial.println(myCanais.canal);
    
    canaisPDM[myCanais.id-1] = myCanais.canal;

    break;
  case sizeof(struct_Painel):
    memcpy(&myPainel, incomingData, sizeof(myPainel));
    DadosSens[ECU_RPM] = myPainel.RPM;
    DadosSens[ECU_Stat_RPM] = myPainel.Stat_RPM;
    DadosSens[ECU_Marcha] = myPainel.Marcha;
    DadosSens[ECU_Temp_Motor] = myPainel.Temp_Motor;
    DadosSens[ECU_Bateria] = myPainel.Bateria;
    DadosSens[ECU_Temp_Oleo] = myPainel.Temp_Oleo;
    DadosSens[ECU_Temp_ADM] = myPainel.Temp_ADM;
    DadosSens[ECU_erros] = myPainel.erros;
    break;

  default:
    Serial.println("ERROR Data len");
    break;
    
  }
}

void AddPear(uint8_t Address[6]){ // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, Address, 6); // coloca os 6 "PainelAddress" em "peerInfo.peer_addr"
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
#ifdef debug
    Serial.println("Failed to add peer");
#endif
    return;
  }
}

void setupESPNOW(uint8_t newAddress[6],int64_t *Dados,uint8_t *Config, bool *Canais)
{
  DadosSens=Dados;
  ConfigSens=Config;
  canaisPDM=Canais;

  WiFi.mode(WIFI_STA);// starta o wifi
  esp_wifi_set_mac(WIFI_IF_STA, &newAddress[0]);
  //fastWifi();                
  if (esp_now_init() != ESP_OK) // verifica se o protocolo ESP NOW esta ativo
  {                             // Init ESP-NOW
#ifdef debug
    Serial.println("Error initializing ESP-NOW");
#endif
    ESP.restart(); // restarta o ESP para tentar iniciar o ESP NOW
  }
  esp_now_register_send_cb(OnDataSent); // Registra a função de callback no envio
  esp_now_register_recv_cb(OnDataRecv);
}

esp_err_t ESPNOWSend(uint8_t Address[6],struct_Canais *myData){
  return esp_now_send(Address,(uint8_t *) myData,sizeof(*myData));
}
esp_err_t ESPNOWSend(uint8_t Address[6],struct_ECU *myData){
  return esp_now_send(Address,(uint8_t *) myData,sizeof(*myData));
}
esp_err_t ESPNOWSend(uint8_t Address[6],struct_Painel *myData){
  return esp_now_send(Address,(uint8_t *) myData,sizeof(*myData));
}
esp_err_t ESPNOWSend(uint8_t Address[6],struct_PDM *myData){
  return esp_now_send(Address,(uint8_t *) myData,sizeof(*myData));
}
esp_err_t ESPNOWSend(uint8_t Address[6],struct_sensor *myData){
  return esp_now_send(Address,(uint8_t *) myData,sizeof(*myData));
}
