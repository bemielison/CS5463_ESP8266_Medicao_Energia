void medicao() {

//-----------------------------------------------------------------------------------------
leitura(ler_Temp); //leitura de temperatura do chip
Temp_lido = (valor_3byte*M_Temp);
dtostrf(Temp_lido,2,2,Temp_lidoCHAR);
client.publish("chip/temperatura", Temp_lidoCHAR);
//Serial.print("Temperatura: ");  Serial.println(Temp_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_VRMS); //leitura de tensao RMS  
VRMS_lido = (valor_3byte*M_Tensao*100000);
if(VRMS_lido < 500){  VRMS_lido = 0;}
VRMS_lido = (((0.0000000000003*(VRMS_lido*VRMS_lido*VRMS_lido)))-(0.00000002*(VRMS_lido*VRMS_lido))+(0.0041*VRMS_lido)-(0.9039));
if(VRMS_lido > 270){ irsend.sendRaw(DESLIGA, 197, 38);  }                    //desligamento em caso de emergência
dtostrf(VRMS_lido,2,2,VRMS_lidoCHAR);
client.publish("central/VRMS",VRMS_lidoCHAR);
//Serial.print(" Tensao RMS: "); Serial.println(VRMS_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_IRMS); //leitura de corrente RMS   
IRMS_lido = (valor_3byte*M_Corrente*100000);      
if(IRMS_lido < 30){  IRMS_lido = 0;}
IRMS_lido = IRMS_lido/1400;
if(IRMS_lido > 50){ irsend.sendRaw(DESLIGA, 197, 38); }                      //desligamento em caso de emergência
dtostrf(IRMS_lido,2,2,IRMS_lidoCHAR);
client.publish("central/IRMS",IRMS_lidoCHAR);
//Serial.print("Corrente RMS: "); Serial.println(IRMS_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_PActive); //leitura de potencia ativa media 
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){  PActive_lido = (valor_3byte*M_P_Ativa*10000); } 
else {PActive_lido = (((valor_3byte&0b011111111111111111111111)*M_P_Ativa*10000) -1);}
PActive_lido = PActive_lido*2.5;
if(PActive_lido < 5){  PActive_lido = 0;}
dtostrf(PActive_lido,2,2,PActive_lidoCHAR);

client.publish("central/PAtiva",PActive_lidoCHAR);

//Serial.print("Potencia Ativa: "); Serial.println(PActive_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_S); //leitura da potencia aparente - valor entre 0 e 1
S_lido = (valor_3byte*M_P_Aparente*100000); //
S_lido = S_lido/3.7;
if(S_lido < 5){  S_lido = 0;}
dtostrf(S_lido,2,2,S_lidoCHAR);
client.publish("central/PAparente",S_lidoCHAR);
//Serial.print("Potencia Aparente: ");Serial.println(S_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_PF); //leitura do fator de potencia
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){   PF_lido = (valor_3byte*M_FP);   }
else {  PF_lido = (((valor_3byte&0b011111111111111111111111)*M_FP) -1);  }
dtostrf(PF_lido,2,2,PF_lidoCHAR);
client.publish("central/FP",PF_lidoCHAR);
}


/*
//-----------------------------------------------------------------------------------------
leitura(ler_Q); //leitura de potencia reativa
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ Q_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  Q_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Potencia Reativa: "); Serial.println(Q_lido);
//-----------------------------------------------------------------------------------------

leitura(ler_QTrig); //leitura de potencia reativa calculada por triangulo  - valor entre 0 e 1
QTrig_lido = (valor_3byte*M_P_Reativa*100000);
Serial.print("Potencia Reat. calc. triangulo: ");Serial.println(QTrig_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_I_instant); //leitura de corrente instantanea
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ I_instant_lido = (valor_3byte*I_V_P_inst*100000);   }
else {  I_instant_lido = (((valor_3byte&0b011111111111111111111111)*I_V_P_inst*100000) -1);  }
Serial.print("Corrente Instantanea: "); Serial.println(I_instant_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_V_instant); //leitura de tensao instantanea
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ V_instant_lido = (valor_3byte*I_V_P_inst*100000);   }
else {  V_instant_lido = (((valor_3byte&0b011111111111111111111111)*I_V_P_inst*100000) -1);  }
Serial.print("Tensao Instantanea: "); Serial.println(V_instant_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_P_instant); //leitura de potencia ativa instantanea
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ P_instant_lido = (valor_3byte*I_V_P_inst*100000);   }
else {  P_instant_lido = (((valor_3byte&0b011111111111111111111111)*I_V_P_inst*100000) -1);  }
Serial.print("Potencia Ativa Instantanea: "); Serial.println(P_instant_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_QAVG); //leitura de potencia reativa media
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ QAVG_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  QAVG_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Potencia Reativa Media: "); Serial.println(QAVG_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_IPeak); //leitura da corrente de pico
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ IPeak_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  IPeak_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Corrente Pico: "); Serial.println(IPeak_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_VPeak); //leitura de tensao de pico
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ VPeak_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  VPeak_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Tensao Pico: "); Serial.println(VPeak_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_PH); //leitura da potencia ativa harmonica
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ PH_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  PH_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Potencia Ativa Harmonica: "); Serial.println(PH_lido); 
//-----------------------------------------------------------------------------------------
leitura(ler_PFA); //leitura da potencia ativa Fundamental
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ PFA_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  PFA_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Potencia Ativa Fundamental: "); Serial.println(PFA_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_QF); //leitura da potencia reativa fundamental
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){ QF_lido = (valor_3byte*M_Qavg_IVpk*100000);   }
else {  QF_lido = (((valor_3byte&0b011111111111111111111111)*M_Qavg_IVpk*100000) -1);  }
Serial.print("Potencia Reativa Fundamental: "); Serial.println(QF_lido);
//-----------------------------------------------------------------------------------------
*/
