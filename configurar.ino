
void configurar(){
  Serial.begin(115200);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(8);   // Descomentar caso utilize este codigo em arduino e não no ESP-8266
  pinMode(CS, OUTPUT);    //initalize the chip select pin;
  pinMode(RESET, OUTPUT); //initalize the RESET pin;
  digitalWrite(CS, HIGH);
  delay (100);
  digitalWrite(RESET, LOW);
  digitalWrite(RESET, HIGH);
  delay (100);
  //Set Config register
  digitalWrite(CS, LOW);    //Chip select to low to initialise comms with CS5463 
  SPI.transfer(0x40);  //01000000 endereco para config
  SPI.transfer(0xC0);  //3 bytes of data to set 24bits of config register   C0 10 01       
  SPI.transfer(0x10);     
  SPI.transfer(0x01); 
  digitalWrite(CS, HIGH);  //Chip select to HIGH to disable comms with CS5463  
  delay (100);
  //Set modo de operacao 
  digitalWrite(CS, LOW);    //Chip select to low to initialise comms with CS5463 
  SPI.transfer(0x64);  //01000000 endereco para config
  SPI.transfer(0x00);  //3 bytes of data to set 24bits of config register        
  SPI.transfer(0x00);     
  SPI.transfer(0xE1); 
  digitalWrite(CS, HIGH);  //Chip select to HIGH to disable comms with CS5463  
  delay (100);
  //Set Control
  digitalWrite(CS, LOW);  //Chip select to low to disable comms with CS5463  
  SPI.transfer(0x78);    //01111000
  SPI.transfer(0x00);    //
  SPI.transfer(0x00);    //
  SPI.transfer(0x04);    //
  digitalWrite(CS, HIGH);    //Chip select to low to initialise comms with CS5463 
  delay (100);
//Set Start Conversions      OK
  digitalWrite(CS, LOW);  //Chip select to low to disable comms with CS5463  
  SPI.transfer(0xE8);    //11101000 perform continuous computation cycles
  digitalWrite(CS, HIGH);    //Chip select to low to initialise comms with CS5463 
  delay (100);
}
