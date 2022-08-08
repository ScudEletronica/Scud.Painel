// libs oficiais
#include <Arduino.h>

// libs comunidade
#include "NextionControl.h"
#define _TIMERINTERRUPT_LOGLEVEL_ 4
#include "ESP32_New_TimerInterrupt.h"

// definições locais
#include "cons.h"
#include "structures.h"
#include "VariaveisCompilacao.h"
#include "sensorIDs.h"

#define numFunLoop 8   // quantidade de macrofunções (funções no loop de 1 segundo que controlam a ordem de execução das outras funções)
#define QuantDados 255 // quantidade de sensores

int64_t DadosSensores[QuantDados] = {}; // armazena os dados dos sensores
uint8_t ConfigSenores[QuantDados] = {}; // armazena as configurações de envio dos sensores
bool canais[8];
int32_t tempo[numFunLoop] = {};
uint8_t CTAddress[] = {0x0A, 0x1B, 0x2C, 0x3D, 0x4E, 0x0F};     // MAC da central de telemetria
uint8_t PDMAddress[] = {0x0A, 0x1B, 0x2C, 0x3D, 0x4E, 0x1F};    // MAC PDM
uint8_t PainelAddress[] = {0x0A, 0x1B, 0x2C, 0x3D, 0x4E, 0x2F}; // MAC do Painel

#define BTstop 25
#define SensFreio 36
#define ledERRO 32
#define ledSinal 33

#define THSensFreio 350

uint8_t EstadoLuz = 0;
struct_Canais MyPDMCanais;

ESP32Timer RearmaBicosBobinas(0);

void setPins()
{
  pinMode(ledERRO, OUTPUT);
  pinMode(ledSinal, OUTPUT);
  pinMode(BTstop, INPUT_PULLUP);
  ledcSetup(0, 1000, 8);
  ledcSetup(1, 1000, 8);
  ledcAttachPin(ledERRO, 0);
  ledcAttachPin(ledSinal, 1);
}

void SetledERRO(bool a)
{
  ledcWrite(0, a * 180);
}

void SetledSinal(bool a)
{
  ledcWrite(1, a * 180);
}

bool IRAM_ATTR rearma(void *timerNo)
{
  MyPDMCanais.canal = false;
  MyPDMCanais.id = 7;
  ESPNOWSend(PDMAddress, &MyPDMCanais);
  MyPDMCanais.id = 5;
  ESPNOWSend(PDMAddress, &MyPDMCanais);
  RearmaBicosBobinas.stopTimer();
  SetledERRO(0);
  SetledSinal(0);
  return true;
}

void setup()
{
#ifdef debug
  Serial.begin(115200);
#endif
  setPins();

  RearmaBicosBobinas.attachInterruptInterval(1E6, rearma);
  RearmaBicosBobinas.stopTimer();

  setupESPNOW(PainelAddress, DadosSensores, ConfigSenores, canais);
  AddPear(PDMAddress);
  for (int16_t i = 0; i < numFunLoop; i++)
  {
    tempo[i] = millis();
  }
}

void loop()
{
  long myMillis = millis();
  if (tempo[0] + 1000 <= myMillis) // macrofunção executada 1 vezes por segundo
  {
    tempo[0] += 1000;
  }
  if (tempo[7] + 500 <= myMillis) // macrofunção executada  2 vezes por segundo
  {
    tempo[7] += 500;
  }
  if (tempo[1] + 200 <= myMillis) // macrofunção executada  5 vezes por segundo
  {
    /* code */
    tempo[1] += 200;
  }
  if (tempo[2] + 100 <= myMillis) // macrofunção executada  10 vezes por segundo
  {
    /* code */
    tempo[2] += 100;
  }
  if (tempo[3] + 40 <= myMillis) // macrofunção executada  25 vezes por segundo
  {
    tempo[3] += 40;
  }
  if (tempo[4] + 10 <= myMillis) // macrofunção executada  100 vezes por segundo
  {
    bool Estado = false;
    if (analogRead(SensFreio) > THSensFreio)
    {
      Estado = true;
    }
    if (EstadoLuz != Estado)
    {
      MyPDMCanais.canal = Estado;
      MyPDMCanais.id = 8;
      ESPNOWSend(PDMAddress, &MyPDMCanais);
      EstadoLuz = Estado;
    }
    if (!digitalRead(BTstop))
    {
      Serial.println("BTStop");
      MyPDMCanais.canal = false;
      MyPDMCanais.id = 7;
      ESPNOWSend(PDMAddress, &MyPDMCanais);
      MyPDMCanais.id = 5;
      ESPNOWSend(PDMAddress, &MyPDMCanais);
      SetledERRO(1);
      SetledSinal(1);
      RearmaBicosBobinas.restartTimer();
    }
    tempo[4] += 10;
  }
  if (tempo[5] + 4 <= myMillis) // macrofunção executada  250 vezes por segundo
  {
    /* code */
    tempo[5] += 4;
  }
  if (tempo[6] + 2 <= myMillis) // macrofunção executada 500 vezes por segundo
  {
    tempo[6] += 2;
  }
}