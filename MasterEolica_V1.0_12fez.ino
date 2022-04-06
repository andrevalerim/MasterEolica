/*********
Sistema de Controle Torre Eolica - Plus Energy
Alef J.
André V de Freitas
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//for liquidcristal_I2C
#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x25,20,4);

//para o sensor BMP280 do sistema
#include <Adafruit_Sensor.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_BMP280.h> //INCLUSÃO DE BIBLIOTECA
 
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)


//variáveis para a cumunicação via I2C com a minimet
#include <Wire.h>; //biblioteca barramento I2C
byte outData = 1;
float inByte = 0;
String inString = "";        //string para armazenar bytes enviados pelo slave
                                                           // D11 será Rx e o D10 será TX no Arduino(2)


// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
#define arref 2 //arrefecimento do sistem
#define ncBrake 4 //bloqueio do movimento da nacele
#define ncFree 15 //liberação do movimento da nacele
#define GPIOreftemp1 34 //leitura do sensor de temperetura interna do gerador
#define GPIOreftemp2 13 //leitura do sensor de temperatura do sistema (ESP32)
#define GPIOtSis 33 //leitura da tensão do sistema de controle
#define GPIOtPot 32 //leitura da tensão do sistema de pontência
#define GPIOpotpitch 36 //leitura da tensão do sistema de controle
#define GPIOpotnacele 39 //leitura da tensão do sistema de pontência
#define sinlight 15 //luz de sinalização 


//Variáveis da mediçnao de RPM
int rpm = 0;
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int pulsos_por_volta = 3; //furos do disco


// Replace with your network credentials
const char* ssid = "Quantum Team";
const char* password = "amotolia_oleosa";
//const char* ssid = "Quantum Team";
//const char* password = "amotolia_oleosa";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String arrefState = "off";
String ncState = "liberate";
String rtState = "liberate";
String Tgerador = "";
String ponto = "";

//variáveis sensores
float tempgerador = 0;
float tempsistema = 0;
float tSis = 0;
float tPot = 0;
int a = 0; //verificação de entrada
int b = 0; //contador
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int pwmbrake = 0;
int pitchAt = 0; //quarda o valor atual do pitch
int posiAt = 0; //guarda o valor atual da posição da nacele 



//variáveis par as rotinas de posicionamento na nacele
//para o motor de rotação na nacele
#define pwmH 5 //pino que manda o sinal da pwm para o sentido Horário
#define pwmA 6 //pino que manda o sinal da pwm no sentido Anti-horário
int valpwm = 0; //variável que armazena o valor da pwm.
int inter = 0; //intervalo entre deg e posi
unsigned long tempo1 = 0; //tempo para o ajuste de potência do motor
unsigned long tempo3 = 0; //tempo para printar a posição na hora do ajuste

//para o referência da nacele
#define pot 7 //pino que o potenciômetro de referência está conectado
int valpot = 0; //armazena o valor lido do pot
int potmap = 0; //armazena o valor do potênciômetro mapeado em graus
int i = 0;


//variáveis gerais. vide referência
int posi = 0; //posição lida antes de iniciar o movimento
int cont = 0; //valor de deg - posi = quanto andar e para que sentido
int ref = 0; //valor (alto ou baixo) do switch refposi
int deg = 0; //posição atual do vento - variável winddir do master e met.
int olddeg = 0; //guarda o último valor de deg
int tent = 0; //tentativas de correção de posição
int tent1 = 0; //tentativas de correção de posição com precisão
int i1 = 0;
int mediapot = 0; //media do potenciomentro 200 medições
int tempo14 = 0;//tempo máximo para anular o loop de giro da nacele
int brake = 0;

//SET da mini met
//biblioteca ro recepção via RF
//#include <PinChangeInterruptHandler.h>
//#include <RFReceiver.h>

// Leitura na porta 2
//RFReceiver receiver(5);

float winddir = 0;
double windspeed = 0;
float winddirMF = 0;

float info1 = 0;
int msgconvert = 0;



// Current time
unsigned long currentTime = millis();
long int time1 = 0;
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
long int time2 = 0; //time count of wait in update code
long int teme3 = 0; //tempo para brake rotor
long int time5 = 0; //tempo para atualização da interface
long int time6 = 0;//luz de sinalizaçnao
long int time7 = 0;//leitura minimet



void setup() {
  Serial.begin(115200);
  Wire.begin(); //INICIALIZA O I2C BUS

bmp.begin(0x76);
//Called this function to update de cod, it run for 3 minuts, automatically after this the ESP reset,
//reiniting it return on de updateWifiCode(), wait again 3 minuts and then, if does`t have cod for 
//for update the ESP return to setup and init the webserver and others functions.
//For update the code is ever necessary restart the ESP ou call de function updateWifiCode(),
//for get successe in this processe, is recommended restart de ESP, instead of call de function

  
  // Initialize the output variables as outputs

  //OUTPUTs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(arref, OUTPUT);
  pinMode(ncBrake, OUTPUT);
  pinMode(ncFree, OUTPUT);
  pinMode(sinlight, OUTPUT);
    //set dos pinos do motor de rotaão da nacele
  pinMode(pwmH, OUTPUT);
  pinMode(pwmA, OUTPUT);


  //IMPUTs
  pinMode(GPIOreftemp1, INPUT);
  pinMode(GPIOreftemp2, INPUT);
  pinMode(GPIOtSis, INPUT);
  pinMode(GPIOtPot, INPUT);
  pinMode(35, INPUT);
  pinMode(5, INPUT);
  pinMode(GPIOpotpitch, INPUT);
  pinMode(GPIOpotnacele, INPUT);


  // Set outputs to off
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(arref, HIGH);
  digitalWrite(ncBrake, HIGH);
  digitalWrite(ncFree, HIGH);
  digitalWrite(sinlight, HIGH);
    digitalWrite(pwmH, LOW);
  digitalWrite(pwmA, LOW);


    //Abilitação do AttachInterrupt
attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);


lcd.init();
lcd.backlight();

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("LAZZARUS_Eolica v2.0");
delay(1000);
lcd.setCursor(2, 1);
lcd.print("Developed By_");
delay(500);
lcd.setCursor(0, 2);
lcd.print("Alef Julio S. C.");
lcd.setCursor(0, 3);
lcd.print("Andre V. de Freitas");
delay(3000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("LAZZARUS_Eolica v2.0");
delay(1000);
lcd.setCursor(2, 1);
lcd.print("--Init_Sistem--");
delay(500);
lcd.setCursor(0, 2);
lcd.print("updateWIFIcode()");
lcd.setCursor(0, 3);
lcd.print("Wait..........");


  updateWifiCode();

//Atualização de software encerrada, inicializando servidor
lcd.clear();  
lcd.setCursor(0, 0);
lcd.print("Atualizacao de");
lcd.setCursor(0, 1);
lcd.print("Software encerrada!");

lcd.setCursor(0, 2);
lcd.print("Init_WebServer");
delay(1000);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  webServer();

lcd.clear();  
lcd.setCursor(0, 0);
lcd.print("WiFi connected:");
lcd.setCursor(0, 1);
lcd.print(ssid);
delay(500);
lcd.setCursor(0, 2);
lcd.print("Server Running!!");
lcd.setCursor(0, 3);
lcd.print("IP: ");
delay(500);
lcd.setCursor(4, 3);
lcd.print(WiFi.localIP().toString());
lcd.clear(); 
delay(3000);


 
digitalWrite(sinlight, LOW);

}

void loop(){

readsensors(); 

readminimet(); 

operation();

webServer();

sinalization();

//print no display
while((millis()-time2) >= 500){
  time2 = millis();
lcd.setCursor(0, 0);
lcd.print("LAZZARUS_Eolica V2.0");
  
lcd.setCursor(0, 1);
lcd.print("TGer:");
lcd.print(tempgerador,1);
lcd.print("C");

lcd.setCursor(11, 1);
lcd.print("Ts:");
lcd.print(tempsistema,2);
lcd.print("C");

lcd.setCursor(11, 2);
lcd.print("ts:");
lcd.print(tSis,1);
lcd.print("V");

lcd.setCursor(0, 2);
lcd.print("tPot:");
lcd.print(tPot,1);
lcd.print("V");

lcd.setCursor(0, 3);
lcd.print("RPM:");
lcd.setCursor(4, 3);
lcd.print("     ");
lcd.setCursor(4, 3);
lcd.print(rpm);

lcd.setCursor(8, 3);
lcd.print("U");
lcd.setCursor(9, 3);
lcd.print("     ");
lcd.setCursor(9, 3);
lcd.print(windspeed,1);
lcd.setCursor(13, 3);
lcd.print("D");
lcd.setCursor(14, 3);
lcd.print("     ");
lcd.setCursor(14, 3);
lcd.print(winddir,1);

  
}

}


//aquisição dos dasos dos sensores
void readsensors(){


 //para a medição de temperatura do sistema
tempsistema = bmp.readTemperature();
b=0;

  
 //para a medição de temperatura do gerador
tempgerador=0;
while(b <= 500){
tempgerador = (analogRead(GPIOreftemp1)*0.000805664*100)+tempgerador+6;
b++;
} 
tempgerador = tempgerador/500;
Tgerador = tempgerador;
b=0;


//Para a medição da tensão so sistema de controle
tSis=0;
while(b <= 20){
tSis = (((analogRead(GPIOtSis)* 3.3) / 4096.0)/ 0.2)+tSis-0.3;
b++;
} 
tSis = tSis/20;
b=0;



//Para a medição da tensão do sistema de potência
tPot=0;
while(b <= 20){
tPot = (((analogRead(GPIOtPot)* 3.3) / 4096.0)/ 0.2)+tPot-0.3;
b++;
} 
tPot = tPot/20;
b=0;


//para a leitura da posição da nacele
posiAt=0;
while(b <= 30){
posiAt = analogRead(GPIOpotnacele)+posiAt;
b++;
} 
posiAt = posiAt/30;
posiAt = map(posiAt, 0, 3000, 0, 359);
b=0;


//para a leitura da posição de angulo de pitch
pitchAt=0;
while(b <= 30){
pitchAt = analogRead(GPIOpotpitch)+pitchAt;
b++;
} 
pitchAt = pitchAt/30;
pitchAt = map(pitchAt, 0, 3000, 0, 90);
b=0;


//Leitura da RPM 
 if (millis() - timeold >= 500){
    //Desabilita interrupcao durante o cálculo
    detachInterrupt(digitalPinToInterrupt(35));
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;
    //Reabilitação do AttachInterrupt
attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);
  }


mediapotnacele();
  
 
}

void mediapotnacele(){
  while(i1<20)  {
  medpot = medpot + (map(analogRead(pot), 68, 945, 0, 359));
  i1++;
  }
  medpot=medpot/20;
  i1=0;
}


//Função que Incrementa o contador para a RPM
void addcount(){
  pulsos++;
} 





//Função de leitura dos dados via I2C da minimet
void readminimet(){

  if((millis()-time7) >= 2000){

    
   outData=1;
   while(outData <= 4){
//Serial.print("Dados enviados ao slave==> ");

Wire.beginTransmission(8); //transmitir dados para o slave com endereço #8
Wire.write("Request");     //envia uma string
Wire.write(outData);       //enviar uma variável numérica tipo byte
Wire.endTransmission(8);    //sinaliza o fim da trasnmissão de dados para o slave
delay(50);
//Serial.print(outData);

Wire.requestFrom(8, 6);  //requisitando resposta do slave
inString = "";
while(Wire.available()){     //loop para receber todos os dados do slave #8
  char inChar = Wire.read();    //recebe cada byte como caractere
 //if(isAlphaNumeric(inChar)){   //se caractere recebido do slave for alphanumerico concatena string
    inString += inChar;         //concatenar o caracter receido
  //}
  //Serial.print("noWhilw");
}

//Distribuição dos valores recebidos para as suas respectivas variáveis
if(outData==1){
 windspeed = inString.toFloat();
}


if(outData==2){
 winddir = inString.toFloat();
 lcd.setCursor(14, 3);
lcd.print(winddir,1);
}


if(outData==3){
 winddirMF = inString.toFloat();
}

if(outData==4){
 ponto = inString;
}


  outData++;
}


//fim da comunicação I2C como a meteo

time7=millis();
  }
  
}



//aqui toman-se as decisøes com base nos sensores e operação
void operation(){


//ativação automática do arrefecimento
if(tempgerador >= 23.0 & a == 0){
  onarref();
  a=1;
}
if(tempgerador <= 22.0 & a == 1){
offarref();
a=0;
}


//bloaqueio automático do rotor com base em alta rpm - para entrar nesta função o pitch deve ter falhado.
if(rpm >= 3500 & brake == 0){
  brake = 1;
}
if(rpm < 500 & brake == 1){ //liberação do rotor
  freerotor();
  brake = 0;
}

//para entrar na freagem automática do rotor
if(brake == 1){
  brakerotor();
}




}//fim do operation()


void ajustenacele(){

//Def de ajuste normal -- ajusta o valor de intervalo para não ficar negativo
mediapot();
if(deg > medpot){ 
  inter = deg - medpot;
}
if(deg < medpot){
  inter = medpot - deg;
}

//quando não é um ajuste de precisão -> inter > 10
if(deg != medpot & tent <= 8 & inter > 10){
  Serial.print("            Entrou no iff");
lcd.clear();

mediapot();
posi = medpot;//guarda o valor para fins de fer. no aumento da pwm
olddeg=deg;
tent++; // permite 5 ajustes de posição
valpwm = 50;
if(tent == 5){
  delay(5000);
  mediapot();
}

//ajusta o valor de intervalo para não ficar negativo
if(deg > medpot){ 
  inter = deg - medpot;
}
if(deg < medpot){
  inter = medpot - deg;
}

//inicia a rotação o aumento de pwm no sentido Anti-horário
if(deg > medpot){

  Serial.println("          entrou no while horario");

mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((medpot-posi) < (inter*0.75)  & (millis()-tempo1) > 80){
   tempo1 = millis();
  if(valpwm < 255){
    valpwm++;
     }
}

//ajuste do valor da pwm - desacelaração
if((medpot-posi) > (inter*0.25)  & (millis()-tempo1) > 150){
   tempo1 = millis();
  if(valpwm > 175){
    valpwm--;
     }
}

analogWrite(pwmH, valpwm);//envia os dados para a pontencia

//printa os novos valores na serial e lcd
if(millis()-tempo3 > 500){
lcd.setCursor(0, 0);
lcd.print("SlaveNacele");
lcd.setCursor(12, 0);
lcd.print("Posi:");
lcd.setCursor(17, 0);
lcd.print("   ");
lcd.setCursor(17, 0);
lcd.print(medpot);
lcd.setCursor(5, 1); // apaga o deg
lcd.print("   ");
lcd.setCursor(0, 1);
lcd.print("Deg:");
lcd.setCursor(5, 1);
lcd.print(deg);
lcd.setCursor(9, 1);
lcd.print("LastDeg:");
lcd.setCursor(17, 1);
lcd.print(posi);
lcd.setCursor(0, 2);
lcd.print("Rot. Anti-horario");
lcd.setCursor(17, 3);
lcd.print("   ");
lcd.setCursor(0, 3);
lcd.print("Inter:");
lcd.setCursor(6, 3);
lcd.print(inter);
lcd.setCursor(10, 3);
lcd.print("ValPWM:");
lcd.setCursor(17, 3);
lcd.print(valpwm);
tempo3=millis();
Serial.print("valor do potênciometro: ");
Serial.println(analogRead(pot));
Serial.print("valor da posição:  ");
Serial.println(medpot);
Serial.print("valor do deg ");
Serial.println(deg);
Serial.print("valpwm");
Serial.println(valpwm);
 }

/*/anula o loop se passar de 4 min
 if((millis()-tempo14) > (300000)){
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }*/
 
}
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 lcd.clear();

mediapot();//atualiza os valores de posição

//inicia a rotação e aumento de pwm no sentido Horário
if(deg < medpot){

  
Serial.println("         entrou no while antihorario");


mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((posi-medpot) < (inter*0.75)  & (millis()-tempo1) > 80){
   tempo1 = millis();
  if(valpwm < 255){
    valpwm++;
     }
}

//ajuste do valor da pwm - desacelaração
if((posi-medpot) > (inter*0.25)  & (millis()-tempo1) > 150){
   tempo1 = millis();
  if(valpwm > 175){
    valpwm--;
     }
}

analogWrite(pwmA, valpwm);//envia os dados para a pontencia

//printa os novos valores na serial e lcd
if(millis()-tempo3 > 500){
lcd.setCursor(0, 0);
lcd.print("SlaveNacele");
lcd.setCursor(12, 0);
lcd.print("Posi:");
lcd.setCursor(17, 0);
lcd.print("   ");
lcd.setCursor(17, 0);
lcd.print(medpot);
lcd.setCursor(5, 1); // apaga o deg
lcd.print("   ");
lcd.setCursor(0, 1);
lcd.print("Deg:");
lcd.setCursor(5, 1);
lcd.print(deg);
lcd.setCursor(9, 1);
lcd.print("LastDeg:");
lcd.setCursor(17, 1);
lcd.print(posi);
lcd.setCursor(0, 2);
lcd.print("Rot. Horario");
lcd.setCursor(17, 3);
lcd.print("   ");
lcd.setCursor(0, 3);
lcd.print("Inter:");
lcd.setCursor(6, 3);
lcd.print(inter);
lcd.setCursor(10, 3);
lcd.print("ValPWM:");
lcd.setCursor(17, 3);
lcd.print(valpwm);
tempo3=millis();
Serial.print("valor do potênciometro: ");
Serial.println(analogRead(pot));
Serial.print("valor da posição:  ");
Serial.println(medpot);
Serial.print("valor do deg ");
Serial.println(deg);
Serial.print("valpwm");
Serial.println(valpwm);
 }

/*/anula o loop se passar de 2.5 min
 if((millis()-tempo14) > (300000)){
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }*/
 
}
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
  lcd.clear();
}//fim do ajuste normal

//Def de ajuste ou ajuste de precisão -- ajusta o valor de intervalo para não ficar negativo
mediapot();
if(deg > medpot){ 
  inter = deg - medpot;
}
if(deg < medpot){
  inter = medpot - deg;
}

//para um ajuste de precisão -> inter <= 10
if(deg != mediapot & tent1 <= 2 & inter <= 10){
Serial.print("Ajuste de precisão");
lcd.setCursor(5, 1);
lcd.print(deg);

mediapot();
posi = medpot;//guarda o valor para fins de fer. no aumento da pwm
olddeg=deg;
tent1++; // permite 5 ajustes de posição
valpwm = 50;


//inicia a rotação o aumento de pwm no sentido Anti-horário
if(deg > medpot){

  Serial.println("          entrou no while horario ajuste de precisão");

mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((millis()-tempo1) > 50){
   tempo1 = millis();
  if(valpwm < 190){
    valpwm++;
     }
}

analogWrite(pwmH, valpwm);//envia os dados para a pontencia

//printa os novos valores na serial e lcd
if(millis()-tempo3 > 500){
lcd.setCursor(17, 0);
lcd.print("   ");
lcd.setCursor(0, 0);
lcd.print("SlaveNacele");
lcd.setCursor(12, 0);
lcd.print("Posi:");
lcd.setCursor(17, 0);
lcd.print(medpot);
lcd.setCursor(5, 1); // apaga o deg
lcd.print("   ");
lcd.setCursor(0, 1);
lcd.print("Deg:");
lcd.setCursor(5, 1);
lcd.print(deg);
lcd.setCursor(9, 1);
lcd.print("LastDeg:");
lcd.setCursor(17, 1);
lcd.print(posi);
lcd.setCursor(0, 2);
lcd.print("RotPre. Anti-horario");
lcd.setCursor(17, 3);
lcd.print("   ");
lcd.setCursor(0, 3);
lcd.print("Inter:");
lcd.setCursor(6, 3);
lcd.print(inter);
lcd.setCursor(10, 3);
lcd.print("ValPWM:");
lcd.setCursor(17, 3);
lcd.print(valpwm);
tempo3=millis();
Serial.print("valor do potênciometro: ");
Serial.println(analogRead(pot));
Serial.print("valor da posição:  ");
Serial.println(medpot);
Serial.print("valor do deg ");
Serial.println(deg);
Serial.print("valpwm");
Serial.println(valpwm);
 }
 /*/anula o loop se passar de 30 seg
 if((millis()-tempo14) > (120000)){
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }*/
}
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
lcd.clear();
mediapot();//atualiza os valores de posição

//inicia a rotação e aumento de pwm no sentido Horário
if(deg < medpot){
 
Serial.println("         entrou no while antihorario - ajuste de precisão");


mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((millis()-tempo1) > 10){
   tempo1 = millis();
  if(valpwm < 190){
    valpwm++;
     }
}

analogWrite(pwmA, valpwm);//envia os dados para a pontencia

//printa os novos valores na serial e lcd
if(millis()-tempo3 > 500){
lcd.setCursor(17, 0);
lcd.print("   ");
lcd.setCursor(0, 0);
lcd.print("SlaveNacele");
lcd.setCursor(12, 0);
lcd.print("Posi:");
lcd.setCursor(17, 0);
lcd.print(medpot);
lcd.setCursor(5, 1); // apaga o deg
lcd.print("   ");
lcd.setCursor(0, 1);
lcd.print("Deg:");
lcd.setCursor(5, 1);
lcd.print(deg);
lcd.setCursor(9, 1);
lcd.print("LastDeg:");
lcd.setCursor(17, 1);
lcd.print(posi);
lcd.setCursor(0, 2);
lcd.print("RotPre. Horario");
lcd.setCursor(17, 3);
lcd.print("   ");
lcd.setCursor(0, 3);
lcd.print("Inter:");
lcd.setCursor(6, 3);
lcd.print(inter);
lcd.setCursor(10, 3);
lcd.print("ValPWM:");
lcd.setCursor(17, 3);
lcd.print(valpwm);
tempo3=millis();
Serial.print("valor do potênciometro: ");
Serial.println(analogRead(pot));
Serial.print("valor da posição:  ");
Serial.println(medpot);
Serial.print("valor do deg ");
Serial.println(deg);
Serial.print("valpwm");
Serial.println(valpwm);
 }
 /*/anula o loop se passar de 30 seg
 if((millis()-tempo14) > (120000)){
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }*/
}
 analogWrite(pwmH, 0);
 analogWrite(pwmA, 0);
 lcd.clear();
}//fim do ajuste de presisão

  
}

//Modo auto-safe
void autoSafe(){
//1-levar o amgulo de pitch para 0
//2-alinhar a nacele com a origem do vento ou mantê-la nesta posição
    //*as ações 1 e 2 devem ser executadas simultaneamente 
//3-bloqueia rotação da nacele - esta função é executada automaticamente no final do posicionamento.
   

  
}


//Ativação do sistema de arrefecimento geral
void onarref(){  
Serial.println("Arrefecimento ativado");
arrefState = "on";
digitalWrite(arref, LOW);
}

//Desativação do sistema de Arrefecimento geral
void offarref(){  
Serial.println("Arrefecimento desativado");
arrefState = "off";
digitalWrite(arref, HIGH);
}

//function to brake the nacele rotation
void brakenacele(){
digitalWrite(ncBrake, LOW); 
delay(100); 
digitalWrite(ncBrake, HIGH);
delay(200);
digitalWrite(ncBrake, LOW); 
delay(600); 
digitalWrite(ncBrake, HIGH);
ncState = "locked"; 
}

//function to free the nacele rotation
void freenacele(){
digitalWrite(ncFree, LOW); 
delay(100); 
digitalWrite(ncFree, HIGH);
delay(200);
digitalWrite(ncFree, LOW); 
delay(400); 
digitalWrite(ncFree, HIGH);
ncState = "liberate"; 
}

//function to brake the rotor rotation
void brakerotor(){

if((millis() - tempo3) >= 80 & pwmbrake < 255 & brake == 1){
  pwmbrake++; 
  analogWrite(rtBreak, pwmbreak);
  tempo3 = millis();
  rtState = "locking";
}

if(pwmbreak == 255 & (millis()-tempo3) >= 200 & brake == 1){
  analogWrite(rtBreak, LOW);
  brake=0;
  tempo3=0;
  rtState = "locked";
  pwmBreak=0;
}
  

 
}

//function to free the rotor rotation
void freerotor(){

analogWrite(rtFree, HIGH);
delay(500);
analogWrite(rtFree, LOW);
delay(100):
analogWrite(rtFree, HIGH);
delay(200);
analogWrite(rtFree, LOW);
  
rtState = "liberate"; 


}



//luz de sinalização
void sinalization(){

  
  if((millis() - time6) >= 1500 & d==0){
    time6=millis();
    digitalWrite(sinlight, HIGH);
   d=1;
   e=0;
   f=0;
   g=0;
  }

  if((millis()-time6) >= 250 & e==0){
    digitalWrite(sinlight, LOW);
    e=1;
  }
  if((millis() - time6) >= 500 & f==0){
    digitalWrite(sinlight, HIGH);
    f=1;
  }
    if((millis() - time6) >= 750 & g==0){
    d=0;
     digitalWrite(sinlight, LOW);
    g=1;
  }

  
}




//Definições do servidor web
  void webServer(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();



            
            //Chamada das funções - sever's buttons 

          //Para o arrefeciemento
            if (header.indexOf("GET /arref/on") >= 0) {
                  onarref();
            } else if (header.indexOf("GET /arref/off") >= 0) {
                  offarref();
            } 
            
            
            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
           
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
          
            }else if (header.indexOf("GET /pitch/on") >= 0) {
              Serial.println("pitch on");
              arrefState = "on";
              //digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /pitch/off") >= 0) {
              Serial.println("pitch off");
              arrefState = "off";
              //digitalWrite(output27, LOW);
            }


             

// HTML INTEIRO 

            //Abrindo HTML, CSS e titulo
            client.println("<!doctype html><html lang=en><head><meta charset=UTF-8><meta http-equiv=X-UA-Compatible content=\"IE=edge\"><meta name=viewport content=\"width=device-width,initial-scale=1\"><title>Sistema de Gerenciamento Torre Eolica - Plus Energy</title><style>html{font-family:Helvetica;font-size:16px;text-align:left;background-color: #bff1fa;}input{padding:2.5px 2.5px;font-size:1.5rem;width:7rem;height:2.5rem;text-align:center;border:#666 solid 2px;border-radius:7%;background-color:#e4e4e4}h2{color:#00777e;margin:.5rem 0;font-weight:500}body::-webkit-scrollbar{width:12px}body::-webkit-scrollbar-track{background:#00aab4}body::-webkit-scrollbar-thumb{background-color:#b1ffb1;border-radius:20px;border:3px solid #00aab4}.button{color:#fff;padding:.5rem 1rem;text-decoration:none;font-size:1.5rem;margin:.1rem;cursor:pointer;text-align:center;border:#969595 solid 3px}.button:hover{background-color:#4caf50;border:#dfdddd solid 3px}.button-off{background-color:#afafaf}.button-on{background-color:#4caf50}input#status_sistema{padding:2.5px 2.5px;font-size:1.5rem;margin:3rem .5rem;width:7rem;height:2.5rem;text-align:center;border:#666 dotted 2px;border-radius:7%;box-shadow:#747474 -1px 1px 7px;background-color:#e4e4e4}input.mostra_info{padding:2.5px 2.5px;font-size:1.5rem;margin:1rem .5rem;width:7rem;height:2.5rem;text-align:center;border:#666 solid 2px;border-radius:5%;background-color:#e4e4e4}.title-container{background-color:#2377a2;text-align:center;margin-top:-.01rem;margin-left:-.1;margin-right:-.1rem;margin-bottom:.6rem;position:sticky;top:0;border:2px solid #fff;padding-left:.5rem;padding-right:.5rem}.wrapper{display:grid}.column-gap{column-gap:.5rem}.grid-titulo{grid-template-columns:1fr 9fr}.grid-col-2-8-4{grid-template-columns:2fr 8fr 4fr}.grid-col-8-2{grid-template-columns:8fr 2fr}.grid-col{grid-template-columns:1fr}.grid-col-2{grid-template-columns:1fr 1fr}.grid-col-3-repeat{grid-template-columns:repeat(3,3fr)}.grid-col-4-repeat{grid-template-columns:repeat(2,2fr) 3fr 3fr}.bloco-col{border-right:#7c7c7c7c groove .1rem;text-align:center;padding-top:.5rem;padding-bottom:.5rem}.border-row{border:#00aab4 solid .2rem;border-radius:5px}.border-none{border:none}.img-title{margin-top:.5rem}.background-img{background-color:#fff;margin-left:-.6rem}.hr{width:45%;border:#999999b2 solid .5px;background:#747474;margin-top:-.1rem;margin-bottom:.7rem}.mt-05{margin-top:.5rem}.mt-1{margin-top:1rem}.pt-1{padding-top:1rem}.pt-2{padding-top:1.5rem}.py-2{padding-top:3rem;padding-bottom:2rem}.align-center{text-align:center}.center{align-self:center}.background-rows{background-color:#f8f8ff}.p-col-title{color:#00aab4;text-align:center;font-size:1.2rem;margin-bottom:.2rem;margin-top:.2rem}#h1-title{font-size:2rem;text-align:center;color:#fff;text-transform:uppercase;padding-right:1.5rem}</style></head><body><div class=\"wrapper display-sticky title-container\"><div><h1 id=h1-title>Sistema de Controle - Torre Eolica</h1></div></div>");

            //Abrindo o menu de controle e divs FREIO e RPM
            client.println("<div class=\"wrapper grid-col-8-2 column-gap\"><div class=\"wrapper grid-col-4-repeat background-rows border-row\"><div class=\"bloco-col pt-1\"><div><p class=p-col-title>Freio Nacele</p><p><button class=\"button button-off\">OFF</button></p></div><div><p class=p-col-title>Freio Rotor </p><p><button class=\"button button-off\">OFF</button></p></div></div><div class=\"bloco-col pt-1\"><div><p class=p-col-title>RPM Rotor</p><input name=rpm_rotor id=rpm_rotor class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>RPM Gerador</p><input name=rpm_gerador id=rpm_gerador class=mostra_info placeholder=30 disabled></div></div>");

            //div TEMPERATURA
            client.println("<div class=\"bloco-col pt-1\"><div><p class=p-col-title>Temperatura Externa</p><input name=temp_externa id=temp_externa class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Temperatura Gerador</p><input name=temp_gerador id=temp_gerador class=mostra_info value=" + Tgerador + "ºC"+ " disabled></div></div>");

            //continuação CÓDIGO
            client.println("<div class=\"bloco-col pt-1 border-none\"><div><p class=p-col-title>Tensao Sistema</p><input name=tensao_sistema id=tensao_sistema class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Tensao Sis. Polo</p><input name=tensao_sis_polo id=tensao_sis_polo class=mostra_info placeholder=30 disabled></div></div></div><div class=\"bloco-col pt-1 background-rows border-row\"><p class=p-col-title>Status Sistema</p><input name=status_sistema id=status_sistema placeholder=OFF disabled></div></div><div class=\"wrapper grid-col background-rows border-row mt-1\"><div class=\"wrapper grid-col-3-repeat\"><div class=bloco-col><h2>Estacao Metereologica</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Velocidade</p><input name=velocidade id=velocidade class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Direcao Atual</p><input name=direcao_atual id=direcao_atual class=mostra_info placeholder=30 disabled></div></div><div><div><p class=p-col-title>Velocidade Media</p><input name=velocidade_media id=velocidade_media class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Direcao Final</p><input name=direcao_final id=direcao_final class=mostra_info placeholder=30 disabled></div></div></div></div><div class=bloco-col><h2>Posicionamento da Nacele</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Posicao Atual</p><input name=posicao_atual_nacele id=posicao_atual_nacele class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>......</p><p><button class=\"button button-off\">OFF</button></p></div></div><div><div><p class=p-col-title>Modo</p><input name=modeo_nacele id=modo_nacele class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Status Atual</p><input name=status_atual_nacele id=status_atual_nacele class=mostra_info placeholder=30 disabled></div></div></div></div><div class=bloco-col><h2>Posicionamento Controle de Pitch</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div class=center><p class=p-col-title>Angulo Atual</p><input name=angulo_atual_pitch id=angulo_atual_pitch class=mostra_info placeholder=30 disabled></div><div><div><p class=p-col-title>Modo</p><input name=modo_atual_pitch id=modo_atual_pitch class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Status Atual</p><input name=status_atual_pitch id=status_atual_pitch class=mostra_info placeholder=30 disabled></div></div></div></div></div></div><div class=\"wrapper grid-col-2-8-4 column-gap mt-1\"><div class=\"background-rows border-row\"><div class=\"bloco-col border-none\"><h2>Manual</h2><hr class=hr><div class=pt-1><p class=p-col-title>Freio Rotor</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Freio Nacele</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Arrefecimento</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div></div></div><div class=\"wrapper grid-col-2 background-rows border-row\"><div class=\"bloco-col py-2\"><h2>Ajuste de posicao</h2><hr class=hr><div><div><p class=p-col-title>Ativar Modo Manual</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Posicionamento Manual em Graus</p><p><button class=\"button button-off\">OFF</button></p></div></div></div><div class=\"bloco-col py-2\"><h2>Ajuste de Pitch</h2><hr class=hr><div><div><p class=p-col-title>Ativar Modo Manual</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Posicionamento Manual em Graus</p><p><button class=\"button button-off\">OFF</button></p></div></div></div></div><div class=\"background-rows border-row wrapper\"><div class=\"bloco-col py-2 border-none\"><h2>Seguranca</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Parada de <br>Emergencia</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Auto</p><p><button class=\"button button-off\">OFF</button></p></div></div><div><div><p class=p-col-title>Reiniciar <br>Sistema</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Atuali...</p><p><button class=\"button button-off\">OFF</button></p></div></div></div></div></div></div></body><script>const locker = \"pluseamelhor\"");
            
            //JS
            client.println("function pedeSenha(){var senha = prompt(\"Digite a senha para continuar\")if (senha != null) {if (senha == \"pluseamelhor\") {alert(\"Deu bom!\")} else {alert(\"Senha Incorreta!\")}}}function alerta() {var escolha = confirm(\"Essa operacao pode comprometer o sistema, deseja continuar?\")if (escolha == true) {pedeSenha();}}</script></html>");




            
            // Display the HTML web page
            //client.println("<!DOCTYPE html><html>");
            //client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            //client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            //client.println("<style>html { font-family: Helvetica; display: inline-block; font-size: 16px; margin: 20 auto; text-align: left;}");
            //client.println(".button { background-color: #555555; border: none; color: white; padding: 16px 40px;");
            //client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            //client.println(".button2 {background-color: #4CAF50;} .formatar{display: inline-block; border: 1px solid red}</style></head>");
            
            
            // Web Page Heading - título da página
            //client.print("<body><h1>Sistema de Controle - Torre Eolica</h1>");
          
            
            // Display current state, and ON/OFF buttons for Arrefecimento geral
            //client.println("<div class=\"formatar\"><p>Arrefeciemnto geral - State " + output26State + "</p>");
        
            // If the output26State is off, it displays the ON button       
            //if (output26State=="off") {
            //  client.println("<p><a href=\"/arref/on\"><button class=\"button\">OFF</button></a></p>");
            //} else {
            //  client.println("<p><a href=\"/arref/off\"><button class=\"button button2\">ON</button></a></p>");
            //} 
            //client.println("</div>");
            //client.println();
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            //client.println("<div class=\"formatar\"><p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            //if (output27State=="off") {
            //  client.println("<p><a href=\"/27/on\"><button class=\"button\">OFF</button></a></p>");
            //} else {
            //  client.println("<p><a href=\"/27/off\"><button class=\"button button2\">ON</button></a></p>");
            //}
            //client.println("</div>");
            //client.println("</body></html>");
      
           // client.println();

            //para o botao do sistema de pitch
            //client.println("<<div class=\"formatar\">p>Pitch - State " + arrefState + "</p>");
            // If the output27State is off, it displays the ON button       
            //if (arrefState=="off") {
            //  client.println("<p><a href=\"/pitch/on\"><button class=\"button\">OFF</button></a></p>");
            //} else {
            //  client.println("<p><a href=\"/pitch/off\"><button class=\"button button2\">ON</button></a></p>");
            //}
            //client.println("</div>");
            //client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }



  
// HTML INTEIRO 

            //Abrindo HTML, CSS e titulo
            client.println("<!doctype html><html lang=en><head><meta charset=UTF-8><meta http-equiv=X-UA-Compatible content=\"IE=edge\"><meta name=viewport content=\"width=device-width,initial-scale=1\"><title>Sistema de Gerenciamento Torre Eolica - Plus Energy</title><style>html{font-family:Helvetica;font-size:16px;text-align:left;background-color: #bff1fa;}input{padding:2.5px 2.5px;font-size:1.5rem;width:7rem;height:2.5rem;text-align:center;border:#666 solid 2px;border-radius:7%;background-color:#e4e4e4}h2{color:#00777e;margin:.5rem 0;font-weight:500}body::-webkit-scrollbar{width:12px}body::-webkit-scrollbar-track{background:#00aab4}body::-webkit-scrollbar-thumb{background-color:#b1ffb1;border-radius:20px;border:3px solid #00aab4}.button{color:#fff;padding:.5rem 1rem;text-decoration:none;font-size:1.5rem;margin:.1rem;cursor:pointer;text-align:center;border:#969595 solid 3px}.button:hover{background-color:#4caf50;border:#dfdddd solid 3px}.button-off{background-color:#afafaf}.button-on{background-color:#4caf50}input#status_sistema{padding:2.5px 2.5px;font-size:1.5rem;margin:3rem .5rem;width:7rem;height:2.5rem;text-align:center;border:#666 dotted 2px;border-radius:7%;box-shadow:#747474 -1px 1px 7px;background-color:#e4e4e4}input.mostra_info{padding:2.5px 2.5px;font-size:1.5rem;margin:1rem .5rem;width:7rem;height:2.5rem;text-align:center;border:#666 solid 2px;border-radius:5%;background-color:#e4e4e4}.title-container{background-color:#2377a2;text-align:center;margin-top:-.01rem;margin-left:-.1;margin-right:-.1rem;margin-bottom:.6rem;position:sticky;top:0;border:2px solid #fff;padding-left:.5rem;padding-right:.5rem}.wrapper{display:grid}.column-gap{column-gap:.5rem}.grid-titulo{grid-template-columns:1fr 9fr}.grid-col-2-8-4{grid-template-columns:2fr 8fr 4fr}.grid-col-8-2{grid-template-columns:8fr 2fr}.grid-col{grid-template-columns:1fr}.grid-col-2{grid-template-columns:1fr 1fr}.grid-col-3-repeat{grid-template-columns:repeat(3,3fr)}.grid-col-4-repeat{grid-template-columns:repeat(2,2fr) 3fr 3fr}.bloco-col{border-right:#7c7c7c7c groove .1rem;text-align:center;padding-top:.5rem;padding-bottom:.5rem}.border-row{border:#00aab4 solid .2rem;border-radius:5px}.border-none{border:none}.img-title{margin-top:.5rem}.background-img{background-color:#fff;margin-left:-.6rem}.hr{width:45%;border:#999999b2 solid .5px;background:#747474;margin-top:-.1rem;margin-bottom:.7rem}.mt-05{margin-top:.5rem}.mt-1{margin-top:1rem}.pt-1{padding-top:1rem}.pt-2{padding-top:1.5rem}.py-2{padding-top:3rem;padding-bottom:2rem}.align-center{text-align:center}.center{align-self:center}.background-rows{background-color:#f8f8ff}.p-col-title{color:#00aab4;text-align:center;font-size:1.2rem;margin-bottom:.2rem;margin-top:.2rem}#h1-title{font-size:2rem;text-align:center;color:#fff;text-transform:uppercase;padding-right:1.5rem}</style></head><body><div class=\"wrapper display-sticky title-container\"><div><h1 id=h1-title>Sistema de Controle - Torre Eolica</h1></div></div>");

            //Abrindo o menu de controle e divs FREIO e RPM
            client.println("<div class=\"wrapper grid-col-8-2 column-gap\"><div class=\"wrapper grid-col-4-repeat background-rows border-row\"><div class=\"bloco-col pt-1\"><div><p class=p-col-title>Freio Nacele</p><p><button class=\"button button-off\">OFF</button></p></div><div><p class=p-col-title>Freio Rotor </p><p><button class=\"button button-off\">OFF</button></p></div></div><div class=\"bloco-col pt-1\"><div><p class=p-col-title>RPM Rotor</p><input name=rpm_rotor id=rpm_rotor class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>RPM Gerador</p><input name=rpm_gerador id=rpm_gerador class=mostra_info placeholder=30 disabled></div></div>");

            //div TEMPERATURA
            client.println("<div class=\"bloco-col pt-1\"><div><p class=p-col-title>Temperatura Externa</p><input name=temp_externa id=temp_externa class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Temperatura Gerador</p><input name=temp_gerador id=temp_gerador class=mostra_info value=" + Tgerador + "ºC"+ " disabled></div></div>");

            //continuação CÓDIGO
            client.println("<div class=\"bloco-col pt-1 border-none\"><div><p class=p-col-title>Tensao Sistema</p><input name=tensao_sistema id=tensao_sistema class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Tensao Sis. Polo</p><input name=tensao_sis_polo id=tensao_sis_polo class=mostra_info placeholder=30 disabled></div></div></div><div class=\"bloco-col pt-1 background-rows border-row\"><p class=p-col-title>Status Sistema</p><input name=status_sistema id=status_sistema placeholder=OFF disabled></div></div><div class=\"wrapper grid-col background-rows border-row mt-1\"><div class=\"wrapper grid-col-3-repeat\"><div class=bloco-col><h2>Estacao Metereologica</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Velocidade</p><input name=velocidade id=velocidade class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Direcao Atual</p><input name=direcao_atual id=direcao_atual class=mostra_info placeholder=30 disabled></div></div><div><div><p class=p-col-title>Velocidade Media</p><input name=velocidade_media id=velocidade_media class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Direcao Final</p><input name=direcao_final id=direcao_final class=mostra_info placeholder=30 disabled></div></div></div></div><div class=bloco-col><h2>Posicionamento da Nacele</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Posicao Atual</p><input name=posicao_atual_nacele id=posicao_atual_nacele class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>......</p><p><button class=\"button button-off\">OFF</button></p></div></div><div><div><p class=p-col-title>Modo</p><input name=modeo_nacele id=modo_nacele class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Status Atual</p><input name=status_atual_nacele id=status_atual_nacele class=mostra_info placeholder=30 disabled></div></div></div></div><div class=bloco-col><h2>Posicionamento Controle de Pitch</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div class=center><p class=p-col-title>Angulo Atual</p><input name=angulo_atual_pitch id=angulo_atual_pitch class=mostra_info placeholder=30 disabled></div><div><div><p class=p-col-title>Modo</p><input name=modo_atual_pitch id=modo_atual_pitch class=mostra_info placeholder=30 disabled></div><div><p class=p-col-title>Status Atual</p><input name=status_atual_pitch id=status_atual_pitch class=mostra_info placeholder=30 disabled></div></div></div></div></div></div><div class=\"wrapper grid-col-2-8-4 column-gap mt-1\"><div class=\"background-rows border-row\"><div class=\"bloco-col border-none\"><h2>Manual</h2><hr class=hr><div class=pt-1><p class=p-col-title>Freio Rotor</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Freio Nacele</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Arrefecimento</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div></div></div><div class=\"wrapper grid-col-2 background-rows border-row\"><div class=\"bloco-col py-2\"><h2>Ajuste de posicao</h2><hr class=hr><div><div><p class=p-col-title>Ativar Modo Manual</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Posicionamento Manual em Graus</p><p><button class=\"button button-off\">OFF</button></p></div></div></div><div class=\"bloco-col py-2\"><h2>Ajuste de Pitch</h2><hr class=hr><div><div><p class=p-col-title>Ativar Modo Manual</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Posicionamento Manual em Graus</p><p><button class=\"button button-off\">OFF</button></p></div></div></div></div><div class=\"background-rows border-row wrapper\"><div class=\"bloco-col py-2 border-none\"><h2>Seguranca</h2><hr class=hr><div class=\"wrapper grid-col-2\"><div><div><p class=p-col-title>Parada de <br>Emergencia</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Auto</p><p><button class=\"button button-off\">OFF</button></p></div></div><div><div><p class=p-col-title>Reiniciar <br>Sistema</p><p><button class=\"button button-off\" onclick=alerta()>OFF</button></p></div><div><p class=p-col-title>Atuali...</p><p><button class=\"button button-off\">OFF</button></p></div></div></div></div></div></div></body><script>const locker = \"pluseamelhor\"");
            
            //JS
            client.println("function pedeSenha(){var senha = prompt(\"Digite a senha para continuar\")if (senha != null) {if (senha == \"pluseamelhor\") {alert(\"Deu bom!\")} else {alert(\"Senha Incorreta!\")}}}function alerta() {var escolha = confirm(\"Essa operacao pode comprometer o sistema, deseja continuar?\")if (escolha == true) {pedeSenha();}}</script></html>");


  
  }



//função que permite atulizar o código via wifi 
  void updateWifiCode(){


      Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("Torre_Eolica");

  // No authentication by default
   ArduinoOTA.setPassword("3438");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  time2 = millis();
  while((millis()-time2) < 120000){
    
      ArduinoOTA.handle();

  }
  Serial.println("Tempo de espera exedido");
  Serial.println("Inicializando servidor");
  }
