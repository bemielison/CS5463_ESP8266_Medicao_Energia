/*
 * PPGSCA UFERSA DEZ/2018
 * Discente: Bemielison Gletson da Silva Bezerra
 * Orientador: Prof. Dr. Marcelo Roberto Bastos Guerra Vale
 * Co-Orientadora: Prof.ª Dr.ª Fabiana Karla de Oliveira Martins Varella Guerra 
 * Tema: IoT BASEADA EM PHM APLICADA A CENTRAIS CONDICIONADORAS DE AR
 * 
 * Contribuiçao com trechos de codigos:  
 * http://randomnerdtutorials.com/
 * Infravermelho: Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 * http://www.felipeflop.com
 * Projeto PiPowerMeter: https://github.com/crjens/PiPowerMeter
 * Outros que entre centenas de exemplos de codigos e retalhos infelizmente foram esquecidos. peço perdao e desejo que este lhes possa ser util de algum modo. 
 * 
 * Observacoes: 
 * Aos que tiverem acesso a este codigo, e precisarem utilizar mesmo que em partes, favor citar a fonte. 
 */

#include <SPI.h> 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <DS1307.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#define DHTTYPE DHT11   // DHT 11
const char* ssid = "SUA REDE";
const char* password = "SUA SENHA";
const char* mqtt_server = "IP DO COMPUTADOR UTILIZADO COMO SERVIDOR"; // ip da rasp
WiFiClient espClient;
PubSubClient client(espClient);

DS1307 rtc(4, 5); // RELOGIO RTC
String hora;
String data; 
String dia;

const uint16_t kIrLed = 2;  //  Utilizado: GPIO2 D4 PARA O INFRAVERMELHO.
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


//CODIGOS DE UM CONDICIONADOR DE AR MARCA ELGIN
uint16_t LIGA[197]    = {6116, 7348, 604, 1608, 588, 1596, 556, 1652, 584, 1604, 600, 1608, 584, 1604, 552, 1656, 588, 1596, 556, 588, 588, 528, 552, 588, 588, 528, 552, 588, 588, 528, 552, 592, 584, 532, 580, 1632, 584, 1604, 588, 1600, 604, 1608, 588, 1600, 584, 1604, 608, 1608, 588, 1596, 608, 536, 584, 532, 552, 592, 580, 536, 588, 556, 588, 532, 580, 560, 584, 536, 576, 1612, 552, 1664, 580, 1608, 588, 1600, 552, 1664, 580, 1608, 588, 1604, 580, 1608, 628, 516, 616, 500, 560, 584, 612, 508, 616, 528, 604, 516, 584, 532, 580, 568, 576, 1612, 584, 536, 556, 1660, 584, 536, 588, 1604, 560, 584, 588, 1604, 580, 1612, 584, 560, 532, 1660, 584, 536, 588, 1604, 556, 592, 576, 1616, 588, 532, 580, 564, 548, 572, 580, 540, 584, 560, 584, 1612, 580, 1640, 556, 564, 556, 1636, 528, 1664, 584, 1612, 580, 1612, 584, 1636, 556, 564, 528, 592, 584, 1612, 580, 568, 556, 564, 528, 592, 552, 1668, 556, 564, 560, 1636, 556, 564, 528, 1668, 556, 592, 552, 572, 552, 1644, 548, 572, 552, 1644, 552, 596, 556, 1640, 556, 564, 556, 1640, 556, 1644, 560, 7376, 584,}; //liga no setpoint 23
uint16_t DESLIGA[197] = {6120, 7376, 584, 1600, 596, 1612, 592, 1592, 592, 1620, 584, 1600, 592, 1620, 536, 1648, 596, 1616, 536, 580, 596, 544, 536, 580, 596, 548, 532, 584, 592, 548, 532, 588, 588, 552, 568, 1620, 584, 1600, 596, 1620, 584, 1604, 592, 1620, 564, 1624, 588, 1600, 596, 1616, 536, 580, 592, 552, 532, 584, 588, 556, 568, 548, 596, 548, 564, 556, 588, 528, 596, 1620, 532, 1656, 588, 1600, 596, 1620, 564, 1624, 588, 1604, 592, 1596, 616, 1600, 532, 588, 588, 560, 564, 552, 592, 552, 560, 556, 536, 584, 588, 556, 568, 552, 580, 1612, 596, 1620, 560, 1628, 536, 584, 592, 1600, 592, 552, 540, 1652, 584, 1608, 584, 560, 564, 556, 588, 528, 596, 1624, 568, 552, 584, 1608, 596, 548, 564, 556, 556, 564, 588, 560, 564, 552, 560, 1664, 560, 1632, 564, 556, 564, 1656, 508, 1684, 532, 1664, 560, 1632, 564, 1628, 564, 556, 568, 580, 532, 1664, 528, 592, 564, 584, 508, 612, 532, 1660, 564, 556, 564, 1632, 564, 584, 528, 1668, 536, 584, 560, 560, 564, 1632, 560, 588, 536, 1660, 532, 588, 568, 1632, 560, 560, 564, 1660, 536, 1656, 536, 7360, 592,}; 
uint16_t TEMP_18[197] = {6120, 7376, 584, 1600, 596, 1612, 592, 1592, 592, 1620, 584, 1600, 592, 1620, 536, 1648, 596, 1616, 536, 580, 596, 544, 536, 580, 596, 548, 532, 584, 592, 548, 532, 588, 588, 552, 568, 1620, 584, 1600, 596, 1620, 584, 1604, 592, 1620, 564, 1624, 588, 1600, 596, 1616, 536, 580, 592, 552, 532, 584, 588, 556, 568, 548, 596, 548, 564, 556, 588, 528, 596, 1620, 532, 1656, 588, 1600, 596, 1620, 564, 1624, 588, 1604, 592, 1596, 616, 1600, 532, 588, 588, 560, 564, 552, 592, 552, 560, 556, 536, 584, 588, 556, 568, 552, 580, 1612, 596, 1620, 560, 1628, 536, 584, 592, 1600, 592, 552, 540, 1652, 584, 1608, 584, 560, 564, 556, 588, 528, 596, 1624, 568, 552, 584, 1608, 596, 548, 564, 556, 556, 564, 588, 560, 564, 552, 560, 1664, 560, 1632, 564, 556, 564, 1656, 508, 1684, 532, 1664, 560, 1632, 564, 1628, 564, 556, 568, 580, 532, 1664, 528, 592, 564, 584, 508, 612, 532, 1660, 564, 556, 564, 1632, 564, 584, 528, 1668, 536, 584, 560, 560, 564, 1632, 560, 588, 536, 1660, 532, 588, 568, 1632, 560, 560, 564, 1660, 536, 1656, 536, 7360, 592,};
uint16_t TEMP_19[197] = {6164, 7324, 616, 1600, 592, 1600, 584, 1612, 560, 1632, 552, 1668, 576, 1616, 588, 1604, 592, 1604, 588, 540, 584, 544, 588, 544, 588, 540, 584, 548, 584, 548, 584, 544, 588, 544, 580, 1612, 580, 1616, 588, 1604, 592, 1604, 588, 1608, 588, 1608, 552, 1640, 556, 1640, 604, 528, 552, 580, 584, 548, 584, 544, 592, 544, 588, 540, 580, 552, 584, 548, 584, 1612, 592, 540, 580, 1616, 580, 1616, 588, 1608, 584, 1616, 588, 1608, 616, 1580, 584, 548, 584, 1616, 576, 556, 580, 524, 556, 576, 588, 544, 588, 548, 584, 548, 588, 1608, 584, 548, 584, 1616, 580, 552, 580, 552, 580, 552, 560, 548, 584, 1612, 584, 552, 580, 1616, 588, 548, 584, 1616, 576, 1620, 584, 1616, 528, 1672, 532, 600, 564, 572, 560, 572, 560, 1640, 556, 1644, 560, 1612, 580, 552, 528, 1672, 532, 1672, 552, 1648, 556, 1644, 560, 576, 556, 548, 556, 580, 532, 1668, 556, 580, 556, 576, 556, 580, 532, 1644, 548, 584, 528, 1676, 560, 576, 556, 1644, 560, 548, 532, 604, 560, 1644, 552, 580, 560, 1644, 532, 576, 524, 1680, 556, 580, 552, 1652, 552, 1624, 528, 7416, 588,};
uint16_t TEMP_20[197] = {6128, 7340, 612, 1604, 588, 1600, 552, 1664, 580, 1612, 584, 1608, 584, 1604, 560, 1656, 588, 1604, 588, 540, 584, 544, 588, 540, 584, 544, 588, 540, 584, 548, 584, 544, 588, 540, 584, 1608, 584, 1636, 580, 1608, 584, 1612, 580, 1612, 584, 1608, 584, 1608, 556, 1664, 580, 548, 584, 548, 576, 552, 580, 548, 532, 600, 524, 604, 528, 604, 528, 576, 580, 1640, 572, 556, 528, 1668, 576, 1616, 588, 1608, 584, 1608, 588, 1608, 584, 1608, 616, 516, 616, 1580, 584, 548, 584, 544, 588, 544, 580, 552, 580, 548, 584, 548, 588, 1608, 584, 548, 584, 1612, 584, 544, 588, 544, 576, 556, 548, 584, 528, 1668, 536, 596, 556, 1612, 592, 544, 580, 1616, 588, 1608, 584, 1612, 580, 1620, 584, 548, 584, 1612, 584, 1612, 612, 548, 584, 1612, 580, 1620, 584, 576, 560, 1608, 584, 1616, 588, 572, 528, 604, 528, 1640, 556, 608, 532, 572, 532, 1664, 560, 576, 528, 604, 528, 604, 528, 1644, 560, 604, 528, 1640, 552, 612, 524, 1648, 536, 596, 536, 600, 532, 1640, 552, 608, 524, 1648, 556, 604, 532, 1640, 532, 604, 528, 1644, 560, 1644, 560, 7376, 552,};
uint16_t TEMP_21[197] = {6124, 7348, 552, 1636, 608, 1608, 564, 1628, 552, 1636, 560, 1660, 584, 1604, 588, 1604, 592, 1600, 560, 568, 556, 572, 592, 536, 584, 548, 588, 540, 580, 548, 584, 548, 588, 540, 580, 1640, 576, 1616, 588, 1604, 588, 1604, 588, 1604, 592, 1604, 556, 1636, 600, 1620, 584, 544, 576, 556, 580, 548, 584, 548, 532, 596, 528, 604, 528, 600, 532, 572, 592, 1604, 560, 568, 552, 1672, 572, 1620, 584, 1612, 584, 1612, 592, 1604, 588, 1604, 588, 544, 588, 1608, 616, 512, 612, 520, 612, 520, 612, 520, 604, 524, 580, 552, 528, 1668, 576, 556, 576, 1620, 576, 556, 524, 608, 556, 548, 584, 548, 584, 1612, 580, 552, 584, 1612, 592, 540, 592, 1604, 588, 1612, 580, 1616, 588, 1608, 588, 544, 588, 544, 588, 1612, 580, 552, 584, 1612, 580, 1620, 584, 544, 524, 1676, 528, 1672, 524, 1672, 532, 604, 560, 1636, 556, 576, 556, 580, 552, 1648, 556, 576, 528, 576, 536, 596, 536, 1664, 560, 576, 556, 1644, 560, 572, 560, 1640, 556, 552, 548, 584, 528, 1672, 564, 572, 560, 1640, 552, 584, 560, 1612, 580, 552, 552, 1652, 532, 1668, 556, 7352, 584,};
uint16_t TEMP_22[197] = {6124, 7352, 620, 1600, 580, 1612, 584, 1604, 608, 1584, 608, 1612, 584, 1604, 588, 1604, 588, 1604, 612, 544, 576, 556, 576, 552, 584, 544, 576, 556, 576, 552, 584, 544, 524, 608, 576, 1616, 580, 1612, 592, 1600, 592, 1604, 588, 1604, 588, 1604, 560, 1632, 612, 1584, 608, 548, 588, 544, 576, 552, 528, 604, 580, 552, 532, 596, 556, 548, 584, 576, 556, 1608, 556, 576, 588, 1608, 584, 1612, 560, 1636, 600, 1592, 612, 1584, 608, 1588, 616, 544, 580, 1616, 588, 544, 576, 556, 528, 604, 528, 604, 560, 544, 588, 544, 588, 1608, 584, 548, 616, 1580, 612, 520, 616, 516, 584, 548, 584, 548, 536, 1660, 584, 552, 528, 1668, 524, 584, 580, 1616, 588, 1608, 584, 1616, 588, 1608, 588, 548, 584, 1612, 592, 544, 576, 556, 576, 1620, 584, 1588, 576, 584, 528, 1644, 560, 1640, 584, 548, 584, 1644, 560, 1612, 580, 580, 552, 584, 528, 1640, 556, 580, 560, 576, 560, 572, 560, 1640, 552, 580, 564, 1636, 524, 584, 528, 1648, 588, 572, 560, 576, 528, 1672, 528, 608, 528, 1616, 584, 580, 556, 1620, 580, 580, 536, 1636, 556, 1648, 532, 7412, 580,};
uint16_t TEMP_23[197] = {6156, 7324, 608, 1612, 588, 1604, 592, 1600, 592, 1604, 580, 1636, 576, 1616, 588, 1604, 592, 1600, 592, 540, 580, 548, 588, 544, 588, 540, 580, 552, 580, 548, 588, 544, 588, 540, 580, 1616, 588, 1604, 592, 1604, 588, 1604, 588, 1608, 588, 1604, 608, 1588, 604, 1616, 588, 544, 588, 544, 580, 548, 584, 548, 556, 576, 556, 572, 560, 572, 560, 572, 560, 1608, 588, 544, 588, 1608, 584, 1612, 580, 1616, 588, 1608, 588, 1608, 584, 1612, 592, 568, 564, 1604, 588, 548, 584, 548, 588, 544, 588, 544, 588, 540, 584, 548, 584, 1616, 588, 544, 576, 1592, 580, 528, 584, 548, 588, 544, 588, 544, 588, 1612, 580, 552, 580, 1616, 588, 548, 576, 1592, 612, 1588, 564, 1636, 588, 1612, 592, 540, 592, 544, 588, 544, 588, 544, 592, 1608, 584, 1588, 584, 576, 556, 1616, 588, 1612, 592, 1608, 584, 1616, 588, 1612, 592, 544, 588, 544, 580, 1592, 592, 544, 588, 548, 584, 548, 584, 1616, 588, 548, 532, 1640, 584, 552, 592, 1612, 580, 552, 592, 544, 588, 1584, 588, 548, 588, 1616, 588, 548, 584, 1616, 588, 548, 564, 1612, 588, 1616, 588, 7380, 592,};
uint16_t TEMP_24[197] = {6152, 7340, 588, 1600, 612, 1604, 588, 1600, 584, 1604, 612, 1604, 588, 1604, 588, 1600, 584, 1604, 608, 548, 584, 544, 580, 548, 584, 544, 580, 548, 584, 544, 588, 540, 584, 544, 592, 1600, 588, 1604, 588, 1600, 616, 1604, 588, 1604, 592, 1600, 580, 1608, 608, 1612, 592, 540, 580, 548, 584, 544, 580, 548, 584, 544, 588, 544, 580, 548, 584, 544, 588, 1604, 588, 544, 580, 1612, 592, 1604, 588, 1604, 588, 1604, 560, 1636, 608, 1584, 612, 548, 584, 1608, 584, 548, 584, 548, 584, 544, 580, 548, 584, 548, 584, 544, 580, 1588, 616, 544, 544, 1620, 616, 544, 580, 552, 528, 600, 552, 580, 552, 1616, 560, 600, 560, 1608, 588, 572, 560, 1604, 560, 1640, 560, 1636, 560, 1636, 556, 600, 564, 1604, 556, 1644, 632, 1592, 612, 544, 580, 1592, 612, 548, 552, 1616, 608, 1588, 556, 604, 560, 572, 560, 544, 588, 1608, 584, 552, 580, 1616, 588, 544, 588, 548, 608, 524, 576, 1592, 612, 548, 532, 1640, 584, 576, 588, 1580, 584, 552, 612, 520, 612, 1588, 616, 516, 604, 1596, 608, 524, 580, 1592, 580, 584, 580, 1592, 612, 1588, 584, 7348, 608,};
uint16_t TEMP_25[197] = {6132, 7348, 556, 1628, 612, 1596, 588, 1596, 608, 1604, 588, 1596, 608, 1600, 616, 1568, 612, 1600, 584, 540, 604, 520, 612, 512, 632, 520, 592, 536, 636, 488, 584, 540, 604, 524, 596, 1616, 608, 1576, 556, 1628, 620, 1596, 584, 1600, 636, 1576, 584, 1604, 600, 1612, 584, 540, 592, 536, 588, 536, 552, 572, 636, 492, 596, 528, 608, 548, 584, 540, 580, 548, 588, 536, 584, 1604, 600, 1616, 588, 1600, 636, 1552, 600, 1612, 592, 1596, 584, 1604, 632, 1580, 612, 516, 588, 540, 580, 544, 560, 568, 552, 576, 600, 524, 628, 1588, 584, 544, 592, 1596, 584, 544, 560, 568, 604, 524, 600, 528, 604, 1584, 608, 548, 588, 1600, 592, 536, 588, 1600, 600, 1616, 588, 1604, 592, 1596, 556, 572, 624, 504, 608, 1584, 608, 1608, 584, 544, 588, 1604, 592, 536, 584, 1608, 596, 1596, 660, 1556, 584, 544, 592, 536, 584, 1608, 584, 544, 592, 1600, 560, 572, 552, 576, 596, 532, 552, 1640, 604, 528, 624, 1568, 604, 548, 636, 1560, 584, 544, 588, 540, 584, 1608, 616, 516, 584, 1608, 588, 544, 588, 1604, 588, 540, 612, 1584, 600, 1592, 604, 7336, 604,};
uint16_t TEMP_26[197] = {6128, 7348, 588, 1600, 552, 1640, 608, 1608, 584, 1608, 584, 1608, 588, 1600, 612, 1580, 612, 1608, 584, 548, 588, 540, 580, 548, 584, 548, 584, 544, 580, 548, 584, 544, 588, 544, 580, 1608, 584, 1612, 584, 1608, 552, 1640, 604, 1592, 612, 1608, 584, 1608, 588, 1608, 584, 544, 588, 544, 580, 548, 584, 548, 584, 544, 588, 544, 580, 552, 580, 548, 584, 1608, 584, 548, 584, 1612, 584, 1612, 580, 1612, 592, 1604, 588, 1608, 584, 1612, 584, 548, 584, 1612, 580, 552, 580, 552, 584, 544, 588, 544, 588, 544, 576, 556, 580, 1612, 592, 544, 588, 1608, 584, 548, 604, 528, 532, 600, 584, 548, 584, 1584, 612, 520, 612, 1588, 616, 516, 584, 1612, 592, 1608, 584, 1612, 584, 1616, 584, 548, 588, 1608, 584, 552, 548, 1620, 584, 580, 532, 1636, 588, 548, 584, 1616, 580, 1616, 588, 548, 584, 1616, 588, 544, 556, 1616, 576, 584, 560, 1612, 580, 556, 580, 552, 580, 552, 580, 1620, 584, 552, 548, 1624, 560, 604, 560, 1612, 580, 580, 552, 584, 528, 1644, 560, 572, 532, 1644, 588, 572, 532, 1644, 580, 580, 532, 1640, 560, 1644, 532, 7380, 588,};
uint16_t TEMP_27[197] = {6152, 7324, 608, 1592, 560, 1648, 588, 1616, 588, 1612, 592, 1588, 584, 1620, 584, 1620, 588, 1616, 588, 524, 588, 552, 592, 544, 536, 576, 588, 552, 580, 556, 536, 576, 592, 548, 584, 1620, 532, 1648, 588, 1620, 584, 1620, 532, 1648, 588, 1620, 584, 1592, 592, 1616, 588, 552, 584, 556, 564, 548, 588, 552, 540, 572, 592, 548, 584, 556, 568, 548, 584, 1620, 532, 584, 592, 1616, 588, 1620, 564, 1616, 588, 1620, 536, 1644, 620, 1588, 584, 532, 592, 1616, 588, 552, 560, 556, 588, 552, 528, 584, 592, 548, 584, 560, 564, 1616, 588, 556, 556, 1628, 588, 552, 560, 1624, 592, 548, 564, 1620, 584, 1628, 568, 544, 588, 1624, 540, 576, 588, 1624, 560, 552, 592, 1620, 532, 612, 560, 580, 504, 612, 564, 576, 504, 1684, 560, 580, 504, 1680, 564, 580, 532, 1652, 564, 1624, 560, 1652, 532, 1656, 560, 580, 508, 1680, 556, 588, 504, 1680, 568, 548, 564, 580, 564, 552, 560, 1656, 528, 588, 556, 1656, 508, 608, 564, 1624, 560, 584, 560, 556, 568, 1648, 536, 580, 560, 1656, 528, 588, 556, 1632, 564, 580, 532, 1656, 560, 1656, 536, 7384, 592,};
uint16_t TEMP_28[197] = {6148, 7344, 556, 1644, 592, 1612, 592, 1608, 584, 1620, 536, 1640, 564, 1636, 588, 1616, 588, 1616, 580, 532, 592, 544, 588, 548, 584, 556, 556, 556, 588, 548, 584, 552, 564, 548, 592, 1612, 596, 1612, 580, 1596, 588, 1616, 588, 1620, 584, 1592, 592, 1616, 588, 1616, 592, 544, 568, 544, 588, 552, 580, 560, 564, 548, 584, 552, 532, 608, 564, 548, 584, 1624, 532, 580, 592, 1612, 596, 1612, 560, 1620, 596, 1612, 580, 1600, 596, 1612, 612, 524, 608, 1572, 592, 552, 592, 544, 568, 548, 584, 552, 584, 560, 560, 552, 584, 1624, 536, 576, 592, 1616, 588, 552, 560, 1624, 588, 552, 560, 1620, 588, 1620, 532, 584, 592, 1616, 556, 556, 588, 1624, 580, 560, 564, 1620, 584, 556, 556, 556, 588, 1624, 528, 1680, 584, 588, 528, 584, 556, 1656, 560, 552, 560, 1652, 564, 1620, 564, 580, 532, 580, 564, 1652, 552, 1628, 564, 580, 556, 1628, 556, 584, 536, 580, 556, 584, 560, 1624, 560, 584, 536, 1648, 568, 576, 536, 1648, 568, 576, 536, 580, 564, 1648, 504, 612, 564, 1648, 504, 612, 560, 1628, 556, 588, 556, 1628, 556, 1656, 540, 7400, 560,};

const int DHTPin = 0;// DHT Sensor - GPIO 0 = D3 on ESP-12E 

DHT dht(DHTPin, DHTTYPE);// Initialize DHT sensor.

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
//escreve
 const byte escreve_IACoff      =    0x60; // Current AC (RMS) Offset 
 const byte escreve_VACoff      =    0x62; // Voltage AC (RMS) Offset
 const byte escreve_IDCoff      =    0x42;// Current DC Offset
 const byte escreve_VDCoff      =    0x46; // Voltage DC Offset
// constantes para requisicao de leitura dos dados no chip.
 const byte ler_Config      =    0x00; // Configuration  
 const byte ler_IDCoff      =    0x02;// Current DC Offset
 const byte ler_Ign         =    0x04; // Current Gain
 const byte ler_VDCoff      =    0x06; // Voltage DC Offset
 const byte ler_Vgn         =    0x08; // Voltage Gain
 const byte ler_CycleCount  =    0x0A; // Number of A/D conversions used in one computation cycle (N)).
 const byte ler_PulseRateE  =    0x0C; // Sets the E1, E2 and E3 energy-to-frequency output pulse rate.
 const byte ler_I_instant   =    0x0E; // Instantaneous Current
 const byte ler_V_instant   =    0x10; // Instantaneous Voltage
 const byte ler_P_instant   =    0x12; // Instantaneous Power
 const byte ler_PActive     =    0x14; // Active (Real) Power
 const byte ler_IRMS        =    0x16; // RMS Current
 const byte ler_VRMS        =    0x18; // RMS Voltage
 const byte ler_Epsilon     =    0x1A; // Ratio of line frequency to output word rate (OWR)
 const byte ler_Poff        =    0x1C; // Power Offset
 const byte ler_Status      =    0x1E; // Status
 const byte ler_IACoff      =    0x20; // Current AC (RMS) Offset 
 const byte ler_VACoff      =    0x22; // Voltage AC (RMS) Offset
 const byte ler_Mode        =    0x24; // Operation Mode
 const byte ler_Temp        =    0x26; // Temperature
 const byte ler_QAVG        =    0x28; // Average Reactive Power
 const byte ler_Q           =    0x2A; // Instantaneous Reactive Power
 const byte ler_IPeak       =    0x2C; // Peak Current
 const byte ler_VPeak       =    0x2E; // Peak Voltage
 const byte ler_QTrig       =    0x30; // Reactive Power calculated from Power Triangle
 const byte ler_PF          =    0x32; // Power Factor  
 const byte ler_Mask        =    0x34; // Interrupt Mask
 const byte ler_S           =    0x36; // Apparent Power
 const byte ler_Ctrl        =    0x38; // Control
 const byte ler_PH          =    0x3A; // Harmonic Active Power
 const byte ler_PFA         =    0x3C; // Fundamental Active Power
 const byte ler_QF          =    0x3E; // Fundamental Reactive Power / Page
 //calibracao
 const byte I_DC_offset     =    0xC9; //11001001 Current channel DC offset
 const byte I_DC_gain       =    0xCA; //11001010 Current channel DC gain
 const byte I_AC_offset     =    0xCD; //11001101 Current channel AC offset
 const byte I_AC_gain       =    0xCE; //11001110 Current channel AC gain
 const byte V_DC_offset     =    0xD1; //11010001 Voltage channel DC offset
 const byte V_DC_gain       =    0xD2; //11010010 Voltage channel DC gain
 const byte V_AC_offset     =    0xD5; //11010101 Voltage channel AC offset
 const byte V_AC_gain       =    0xD6; //11010110 Voltage channel AC gain
 const byte I_V_DC_offset   =    0xD9; //11011001 Current and Voltage channel DC offset
 const byte I_V_DC_gain     =    0xDA; //11011010 Current and Voltage channel DC gain
 const byte I_V_AC_offset   =    0xDD; //11011101 Current and Voltage channel AC offset
 const byte I_V_AC_gain     =    0xDE; //11011110 Current and Voltage channel AC gain
//variaveis atualizadas.
unsigned long Config_lido      =    0x00; // Configuration
float IDCoff_lido      =    0x00; // Current DC Offset
float Ign_lido         =    0x00; // Current Gain
float VDCoff_lido      =    0x00; // Voltage DC Offset
float Vgn_lido         =    0x00; // Voltage Gain
unsigned long CycleCount_lido  =    0x00; // Number of A/D conversions used in one computation cycle (N)).
unsigned long PulseRateE_lido  =    0x00; // Sets the E1, E2 and E3 energy-to-frequency output pulse rate.
float I_instant_lido   =    0x00; // Instantaneous Current
float V_instant_lido   =    0x00; // Instantaneous Voltage
float P_instant_lido   =    0x00; // Instantaneous Power
float PActive_lido     =    0x00; // Active (Real) Power
float IRMS_lido        =    0x00; // RMS Current
float VRMS_lido        =    0x00; // RMS Voltage
unsigned long Epsilon_lido     =    0x00; // Ratio of line frequency to output word rate (OWR)
unsigned long Poff_lido        =    0x00; // Power Offset
unsigned long Status_lido      =    0x00; // Status
float IACoff_lido      =    0x00; // Current AC (RMS) Offset
float VACoff_lido      =    0x00; // Voltage AC (RMS) Offset
unsigned long Mode_lido        =    0x00; // Operation Mode
float Temp_lido        =    0x00; // Temperature
float QAVG_lido        =    0x00; // Average Reactive Power
float Q_lido           =    0x00; // Instantaneous Reactive Power
float IPeak_lido       =    0x00; // Peak Current
float VPeak_lido       =    0x00; // Peak Voltage
float QTrig_lido       =    0x00; // Reactive Power calculated from Power Triangle 
float PF_lido          =    0x00; // Power Factor
unsigned long Mask_lido        =    0x00; // Interrupt Mask
float S_lido           =    0x00; // Apparent Power
unsigned long Ctrl_lido        =    0x00;  // Control
float PH_lido          =    0x00; // Harmonic Active Power
float PFA_lido         =    0x00; // Fundamental Active Power
float QF_lido          =    0x00; // Fundamental Reactive Power / Page
//----------------------------------
//pinos do ESP
const int CS = 15;     // Assign the Chip Select signal to pin D8 = GPIO15 
const int RESET = 16; // Assign Reset to pin D0 GPIO 16
byte ler;
unsigned long MSB;
float PF;
float aux01;
float aux02;
String H_byte_string;
String M_byte_string; 
String L_byte_string;
String H_byte0;
String M_byte0; 
String L_byte0;
String H_byte1;
String M_byte1; 
String L_byte1;
String aux;
String valor_3byte_string;
char H_byte0_char;
char M_byte0_char; 
char L_byte0_char;
char H_byte1_char;
char M_byte1_char; 
char L_byte1_char;
unsigned long aux00;
byte H_byte; 
byte M_byte; 
byte L_byte; 
unsigned long valor_3byte;  // valor concatenado dos 3 bytes (H,M,L_byte)
int x = 0;
char letra_m []= {'a','b','c','d','e','f'};
char letra_M []= {'A','B','C','D','E','F'};
float M_P_Ativa = 0.0000001192093038;
float M_Qavg_IVpk = 0.0000001192093038;
float I_V_P_inst = 0.0000001192093038;
float calibra = 0.0000001192093038;
float M_FP = 0.0000001192093038;
float M_Temp = 0.0000152590219;
float M_Tensao = 0.00000005960464833;  
float M_Corrente = 0.00000005960464833;
float M_P_Aparente = 0.0000001192093038;
float M_P_Reativa = 0.0000001192093038;
static char Temp_lidoCHAR[10];
static char VRMS_lidoCHAR[10];
static char IRMS_lidoCHAR[10];
static char PActive_lidoCHAR[10];
static char S_lidoCHAR[10];
static char horaCHAR[10];
static char PF_lidoCHAR[10];
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageAtual;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageAtual += (char)message[i];
  }
  Serial.println();
  if(topic=="central/reset"){ if(messageAtual == "reset"){ ESP.restart();   } }   //usado para reiniciar o ESP em caso de falha.  
  if(topic=="central/AjusteTemp"){ if(messageAtual == "18"){ irsend.sendRaw(TEMP_18, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "19"){ irsend.sendRaw(TEMP_19, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "20"){ irsend.sendRaw(TEMP_20, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "21"){ irsend.sendRaw(TEMP_21, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "22"){ irsend.sendRaw(TEMP_22, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "23"){ irsend.sendRaw(TEMP_23, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "24"){ irsend.sendRaw(TEMP_24, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "25"){ irsend.sendRaw(TEMP_25, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "26"){ irsend.sendRaw(TEMP_26, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "27"){ irsend.sendRaw(TEMP_27, 197, 38);  }}
  if(topic=="central/AjusteTemp"){ if(messageAtual == "28"){ irsend.sendRaw(TEMP_28, 197, 38);  }}
  if(topic=="central/ligar"){ if(messageAtual == "ligar"){ irsend.sendRaw(LIGA, 197, 38);  }}
  if(topic=="central/desligar"){ if(messageAtual == "desligar"){ irsend.sendRaw(DESLIGA, 197, 38);  }}}

void reconnect() {
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      client.subscribe("central/AjusteTemp"); 
      client.subscribe("central/ligar");
      client.subscribe("central/desligar");
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000); }}}
void setup() {  
  configurar();
  irsend.begin();    
  rtc.halt(false);//Aciona o relogio
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883); //porta do servidor broker
  client.setCallback(callback);  }

void loop(){
medicao();
  if (!client.connected()) { reconnect();  }
  if(!client.loop())  client.connect("ESP8266Client");  now = millis();
  if (now - lastMeasure > 5000) {    lastMeasure = now; // Publishes new temperature and humidity
    float h = dht.readHumidity(); // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float t = dht.readTemperature(); // Read temperature as Celsius (the default)
    float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
    if (isnan(h) || isnan(t) || isnan(f)) { // Check if any reads failed and exit early (to try again).
      Serial.println("Failed to read from DHT sensor!");
      return;   }
    float hic = dht.computeHeatIndex(t, h, false); // Computes temperature values in Celsius
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    if(hic >= 60){    //desligamento em caso de emergência
      irsend.sendRaw(DESLIGA, 197, 38);   }
    client.publish("central/temperature", temperatureTemp);  // Publishe topic Temperature and Humidity values
    client.publish("central/humidity", humidityTemp);  } }





















 //hora = rtc.getTimeStr();
   //data = rtc.getDateStr();
   //dia  = rtc.getDOWStr();

 
 //  client.publish("central/data",horaCHAR );
  // client.publish("central/hora",dataCHAR );
  // client.publish("central/dia", diaCHAR );
