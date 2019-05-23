
char nibble2c(char c){ //leitura da string
   if ((c>='0') && (c<='9')) return c-'0' ;
   if ((c>='A') && (c<='F')) return c+10-'A' ;
   if ((c>='a') && (c<='a')) return c+10-'a' ;}
char hex2c(char c1, char c2){ //conversao para hexadecimal
   if(nibble2c(c2) >= 0){ aux00 = nibble2c(c1)*16+nibble2c(c2) ;   }}

int leitura(int ler){
   x= 0;
   while (x<2){            //rotina de leitura duplicada para limpar o buffer
     digitalWrite(CS, LOW);
     SPI.transfer(ler);   // leitura do valor 
     H_byte = SPI.transfer(0xFF);
     M_byte = SPI.transfer(0xFF);
     L_byte = SPI.transfer(0xFF);
     delay(100);
     digitalWrite(CS, HIGH);
     x+=1;   }
  H_byte_string = String(H_byte,HEX);   //
  M_byte_string = String(M_byte,HEX);   //
  L_byte_string = String(L_byte,HEX);   // 
  valor_3byte = 0;
 // verifica o tamanho da variavel e agrupa em uma unica string   0x00 00 00
 if((H_byte_string.length())<2){aux=H_byte_string[0];H_byte0='0';H_byte1=aux; }else{H_byte0=H_byte_string[0];H_byte1=H_byte_string[1];}
 if((M_byte_string.length())<2){aux=M_byte_string[0];M_byte0='0';M_byte1=aux; }else {M_byte0 = M_byte_string[0];M_byte1 = M_byte_string[1];}
 if((L_byte_string.length())<2){aux=L_byte_string[0];L_byte0='0';L_byte1=aux; }else{L_byte0=L_byte_string[0];L_byte1=L_byte_string[1];}
 valor_3byte_string = H_byte0 + H_byte1 + M_byte0 + M_byte1 + L_byte0 + L_byte1;
 //convertendo a string para char
 H_byte0_char = H_byte0[0]; M_byte0_char = M_byte0[0]; L_byte0_char = L_byte0[0];
 H_byte1_char = H_byte1[0]; M_byte1_char = M_byte1[0]; L_byte1_char = L_byte1[0];
 // convertendo minusculo em maiusculo
 for (x=0;x<6;x++){
      if(H_byte0_char == letra_m[x]){H_byte0_char = letra_M[x];} if(H_byte1_char == letra_m[x]){H_byte1_char = letra_M[x];}
      if(M_byte0_char == letra_m[x]){M_byte0_char = letra_M[x];} if(M_byte1_char == letra_m[x]){M_byte1_char = letra_M[x];}
      if(L_byte0_char == letra_m[x]){L_byte0_char = letra_M[x];} if(L_byte1_char == letra_m[x]){L_byte1_char = letra_M[x];} }
  //lendo valor hexa e convertendo valor de 3 bytes para decimal
 hex2c(H_byte0_char,H_byte1_char); valor_3byte += (aux00<<16);
 hex2c(M_byte0_char,M_byte1_char); valor_3byte += (aux00<<8);
 hex2c(L_byte0_char,L_byte1_char); valor_3byte += (aux00<<0);}
