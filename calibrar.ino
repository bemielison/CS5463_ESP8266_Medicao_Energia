
int calibrar(){  
  
Serial.println("Valores iniciais default");
//-----------------------------------------------------------------------------------------
leitura(ler_IDCoff); //
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){  IDCoff_lido = (valor_3byte*calibra*100000); } 
else {IDCoff_lido = (((valor_3byte&0b011111111111111111111111)*calibra*100000) -1);}
Serial.print("IDCoff_lido: "); Serial.println(IDCoff_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_VDCoff); //
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){  VDCoff_lido = (valor_3byte*calibra*100000); } 
else {VDCoff_lido = (((valor_3byte&0b011111111111111111111111)*calibra*100000) -1);}
Serial.print("VDCoff_lido: "); Serial.println(VDCoff_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_IACoff); //leitura de tensao RMS
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){  IACoff_lido = (valor_3byte*calibra*100000); } 
else {IACoff_lido = (((valor_3byte&0b011111111111111111111111)*calibra*100000) -1);}
Serial.print("IACoff_lido: "); Serial.println(IACoff_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_VACoff); //
MSB = valor_3byte&0b100000000000000000000000;
if(MSB==0){  VACoff_lido = (valor_3byte*calibra*100000); } 
else {VACoff_lido = (((valor_3byte&0b011111111111111111111111)*calibra*100000) -1);}
Serial.print("VACoff_lido: "); Serial.println(VACoff_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_Ign); //
Ign_lido = valor_3byte*4194304;
Serial.print("Ign_lido: "); Serial.println(Ign_lido);
//-----------------------------------------------------------------------------------------
leitura(ler_Vgn); //
Vgn_lido = valor_3byte*4194304;
Serial.print("Vgn_lido: "); Serial.println(Vgn_lido);
//-----------------------------------------------------------------------------------------

}
