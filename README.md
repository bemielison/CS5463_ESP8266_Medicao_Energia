# CS5463_ESP8266_Medicao_Energia
Descrição do projeto que desenvolvi no programa de mestrado em engenharia elétrica da Universidade Federal Rural do Semi-Árido (UFERSA. Utiliza um chip dedicado para medição de energia elétrica (CS5463 da empresa Cirrus Logic) e o processamento é realizado por um módulo Wemos D1 Mini (composto por um ESP8266). A comunicação se dá sem fio (Wi-Fi) e faz uso do MQTT broker Mosquitto instalado em um computador de placa única Raspberry Pi. O projeto possui aplicação direta na medição das grandezas elétricas de uma central condicionadora de ar e de ambiente (temperatura e umidade) e através dos dados medidos se faz o diagnóstico da saúde do equipamento e condições de uso e mau uso. Um dashboard foi implementado utilizando-se Node-Red e é possível realizar o controle da central de ar em substituição ao controle remoto convencional (modificar temperatura, velocidade do ventilador, ligar e desligar, etc.). 

Este projeto sofreu modificações e atualmente as 3 placas (força, controle, medição) foram concentradas em uma única placa. Cinquenta unidades estão sendo produzidas para testes na UFERSA para posteriormente expandir para todas as centrais de ar dos quatro campus.


Description of the project that I developed in the master's program in electrical engineering of the Federal Rural University of the Semi-Arid (UFERSA). It uses a dedicated chip to measure electrical energy (CS5463 from Cirrus Logic) and the processing is performed by a Wemos D1 Mini module (composed of an ESP8266). The communication goes wireless (Wi-Fi) and makes use of the MQTT broker Mosquitto installed on a single board computer Raspberry Pi. The project has direct application in the measurement of the electrical quantities of a central air conditioner and environment (temperature and humidity) and through the measured data it is made the diagnosis of the health of the equipment and conditions of use and misuse. A dashboard has been implemented using Node-Red and it is possible to perform control of the air central instead of the conventional remote control (modify temperature, fan speed, on and off, etc.)

This project was modified and currently the 3 plates (force, control, measurement) were concentrated in a single plate. Fifty units are being produced for testing at UFERSA and later expanded to all air conditioning units in the four campuses.
