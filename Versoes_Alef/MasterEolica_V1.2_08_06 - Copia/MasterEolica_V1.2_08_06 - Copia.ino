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

float winddir = 0;
double windspeed = 0;
float winddirMF = 0;

float info1 = 0;
int msgconvert = 0;
int freee=0;


//variáveis parametrais 
int rpmmax1 = 400; //1500 RPM para redução automática do andulo de pitch
int rpmmax2 = 600;  // 1700 rpm para entrar no auto safe
int rpmmax3 = 650; //2200 . //rpm para última tentativa de segurança bloaquear o rotor intependente da velocidade
int rpmBrake = 50; //rpm para travar o rotor em Auto-Safe normal, ou desativação do sistema.
float cutOff = 15.0; //cut of para entrada em autosafe
float cutOff2 = 12.0; //velocidade para retomada após evento de alta velocidade
int temporet = 90000; //tempo de espera até retomada da operação -padrão 300000
float cutIn = 0.0;  //vento mínimo para iniciar a operação
float temparref = 45.0; //temperatura para ativação atomática do arref do gerador
int tent3 = 0;

                                                           

// Assign output variables to GPIO pins
//const int output26 = 26;
//const int output27 = 27;
#define arref 2 //arrefecimento do sistema
#define ncBrake 23 //bloqueio do movimento da nacele
#define ncFree 4 //liberação do movimento da nacele
#define GPIOreftemp1 34 //leitura do sensor de temperetura interna do gerador
#define GPIOreftemp2 13 //leitura do sensor de temperatura do sistema (ESP32)
#define GPIOtSis 33 //leitura da tensão do sistema de controle
#define GPIOtPot 32 //leitura da tensão do sistema de pontência
#define GPIOpotpitch 36 //leitura da referência do sisetma de posicionamento da nacele
#define GPIOpotnacele 39 //leitura da referência do sisetma de controle de pitch
#define rtFree 26
#define rtBrake 27


//Variáveis da mediçnao de RPM
int rpm = 0; //rmp gerador
int rpmRot = 0; //rpm rotor
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int pulsos_por_volta = 3; //furos do disco


// Replace with your network credentials
const char* ssid = "Master_Eolica - Host2";
const char* password = "diaqueohomemfoiemdirecaoalua16";
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
String noti = "";



String Status = ""; //demonstra o status do sistema
//status possíveis
int autosafe = 0; //parâmetro para chamada do modo autosafe
int Idle = 0; //parâmetro para chamada do modo idle
int online = 0; //parâmetro para chamada do modo online
int offline = 0; //parâmetro para chamada do modo offline
int manual = 0; //parâmetro para chamada do modo manual
int emergencia = 0; //parametro para parada de emergencia
int automatico = 1; //parâmetro para operação automática
int autoposi = 0;


//variáveis sensores
float tempgerador = 0;
float tempsistema = 0;
float tSis = 0;
float tPot = 0;
int dir = 0;
int a = 0; //verificação de entrada
int a1 = 0; //impede a reetrada no freerotor()
int b = 0; //contador
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;
int a6 = 0;
int a7 = 0;
int a8 = 0;
int a9 = 0;
int a10 = 0;
int a11 = 0;
int b1=0;


//váriáveis rotinas de segurança
long int tempo16 = 0; //tempo para retorno a operação normal.
String notifica = ""; //armazena o texto da notificação para posteriormente postar no datalog
int notificar = 0; //condição parea notificar continuamente o terminal


//variáveis par as rotinas de posicionamento na nacele
int valpwm = 0; //variável que armazena o valor da pwm.
int inter = 0; //intervalo entre deg e posi
unsigned long tempo1 = 0; //tempo para o ajuste de potência do motor
unsigned long tempo3 = 0; //tempo para printar a posição na hora do ajuste
int valpot = 0; //armazena o valor lido do pot
int i = 0;
int posi = 0; //posição lida antes de iniciar o movimento
int cont = 0; //valor de deg - posi = quanto andar e para que sentido
int deg = 0; //posição atual do vento - variável winddir do master e met.
int olddeg = 0; //guarda o último valor de deg
int tent = 0; //tentativas de correção de posição
int tent1 = 0; //tentativas de correção de posição com precisão
int i1 = 0;
int medpot = 0; //media do potenciomentro nacele
long int tempo14 = 0;//tempo máximo para anular o loop de giro da nacele
long int tempo20 = 0;
int ultimointer = 0;
float Vangular = 0;
long int tempo40 = 0;
int posicao = 0; 



//para referência do pitch
int freq = 5000;
int channel = 0;
int channel1 = 1;
int resolution = 8;
int dutyCycle = 150;
int dutyCyclePitch1 = 0;
int medpotpitch = 0; //armazena o valor lido do pot
int pitchReq = 45; //quarda o valor Requerido do pitch
int pwmpitch = 0; //valor de aceleração do pitch
int tent2 = 0; // número de tentativas de ajuste do pitch
long int tempo9 = 0;
long int tempo10 = 0;
long int tempo11 = 0;
int autopitch = 1; //permite aumento automátuco do pitch

//Sistema de freios
int pwmbrake = 0; //ajuste da pwm dos freios
int brake = 0; //freiado se == 1, liberado se == 0
int braking = 0; //variável freiando se == 1

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



//codigo para esolha de melhor velocidade de vento para atuar

//descrição: A operação se baseria no registro da velocidade e direção
//do vento a cada 5s, uma vez que se passaram 25s é feita uma análise
//com os dados que já foram armazenados nos vetores, primeiro é verificado 
//o número de ocorrências de cara uma das classes possíveis descritas
//no ventor chamado classes[], após isto é verificado qual a velocidade
//de cada uma das ocorrências, com essas informações se traça uma média 
//de velocidade para cada vez que uma direção ocorreu, desta forma
//se o valor for maior que o cut-in e for o mais recorrente, o posição
//é indicada ao master.PS se o mais recorrente está abaixo do cut-in,
//ele busca automaticamente o segundo mais recorrente e assim susseciva...

//Armazena o valor da velocidade direção de vento e tempo cada dado
//em um vetor mas com a mesma posição

//declaração dos 3 vetores de dados lidos pela estação
float winddirs[50];
float windspeeds[50];

//vetores com os dados refinados  
float classes[32] = {0.00, 11.25, 22.5, 33.75, 45, 56.25, 67.5, 78.75, 90, 101.25, 112.5, 123.75, 135.0, 146.25, 157.5, 168.75, 180.1, 191.25, 202.5, 213.75, 225.0, 236.25, 247.5, 258.75, 270.0, 281.25, 292.5, 303.75, 315.0, 326.25, 337.5, 348.75};
float ocorr[32];
float classespeeds[32];

//variáveis para armazenamento e análise dos vetores
long int tempo30 = 0; //variável que marca o tempo para gravar valor na nova posição do vetor
long int tempo31 = 0; //variável que marca o tempo para reanalizar os valores
long int tempo32 = 0; //tempo para zerar os vetores novamente
//long int tempo5 = 0;
long int pass = 0; //variável que marca a posição do vet que está sendo analizada
long int pass1 = 0; //marcação de posição na análise dos vetores
long int numposi = 0; //variável que grava qual a posição do vetor que está sendo lida na gravação
float posimf = 0; //variável que armazena a posição mais frequente 
float maiorocorr = 0;//armazena quantas vezes ocorreu
float velocidade = 0;
float cutin = 0; //indica se o cut-in é possível
float velposimf = 0; //armazena o valor de velocidade na posicao mais frequente




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

  pinMode(arref, OUTPUT);
  pinMode(ncBrake, OUTPUT);
  pinMode(ncFree, OUTPUT);
   pinMode(rtBrake, OUTPUT);
  pinMode(rtFree, OUTPUT);
    //set dos pinos do motor de rotaão da nacele


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

  digitalWrite(arref, HIGH);
  digitalWrite(ncBrake, HIGH);
  digitalWrite(ncFree, HIGH);
  digitalWrite(rtBrake, LOW);
  digitalWrite(rtFree, LOW);



//Definições gerais para a PWM

  ledcSetup(0, 5000, 8);
  ledcAttachPin(13, 0);

  ledcSetup(1, 5000, 8);
  ledcAttachPin(12, 1);

  ledcSetup(2, 5000, 8);
  ledcAttachPin(26, 2);

  ledcSetup(3, 5000, 8);
  ledcAttachPin(27, 3);
  
  ledcSetup(4, 5000, 8);
  ledcAttachPin(14, 4);

  ledcSetup(5, 5000, 8);
  ledcAttachPin(15, 5);


    //Abilitação do AttachInterrupt
attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);


lcd.init();
lcd.backlight();

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("LAZZARUS_Eolica v2.1");
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
lcd.print("LAZZARUS_Eolica v2.1");
delay(1000);
lcd.setCursor(2, 1);
lcd.print("--Init_Sistem--");
delay(500);
lcd.setCursor(0, 2);
lcd.print("updateWIFIcode()");
lcd.setCursor(0, 3);
lcd.print("Wait..........");

//freenacele();
brakenacele();
braking = 1;
//freerotor();

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




//fins de teste para leitura do potenciøemtro de ref da nacele e pitch
//while(1){
 // lcd.clear();
 // lcd.setCursor(0, 0);
 // lcd.print(analogRead(GPIOpotnacele));
 // delay(500);
//}

settozero();

}

void loop(){

readsensors(); 

readminimet(); 

operation();

webServer();

Display();

if(automatico == 1 & deg >= 0 & deg < 360){

//ajuste na posição da nacele
ajustenacele();

}

}//fim do loop


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



//leitura da posição da nacele
mediapot();


//leitura do angulo pitch
mediapotpitch();


//Leitura da RPM 
 if (millis() - timeold >= 500){
    //Desabilita interrupcao durante o cálculo
    detachInterrupt(digitalPinToInterrupt(35));
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    rpmRot = (rpm/32);
    timeold = millis();
    pulsos = 0;
    //Reabilitação do AttachInterrupt
attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);
  }


  
 
}


//medição da valor do potenciometro de referência do sistema de posicionamento
void mediapot(){
  i1=0;
  while(i1<20)  {
  medpot = medpot + (map(analogRead(GPIOpotnacele), 3945, 540, 0, 359));
  i1++;
  }
  medpot = (medpot/20);
  i1=0;

  //para o cálculo da velocidade de rotação da nacele

  
  if(posicao =! medpot){
    Vangular = (medpot-posicao)*1000/(millis()-tempo40);
    posicao = medpot;
    tempo40 = millis();
  }
  
    if(Vangular < 0){
      Vangular = Vangular*-1;
    }
  
  
}


//medição da valor do potenciometro de referência do sistema de pitch
void mediapotpitch(){
  i1=0;
  while(i1<20)  {
  medpotpitch = medpotpitch + (map(analogRead(GPIOpotpitch), 110, 3950, 45, 0));
  i1++;
  }
  medpotpitch = (medpotpitch/20);
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
 windspeed = inString.toFloat()/2;
}


if(outData==3){
  if(inString.toFloat() >= 0 & inString.toFloat() < 360){  
 //winddirMF = inString.toFloat();
  }
}


if(outData==2){
  if(inString.toFloat() >= 0 & inString.toFloat() < 360){
 winddir = inString.toFloat();

dir = medpot + winddir;

if(dir > 360){
  dir = dir - 360;
}



  }

  if(dir >= 354 & dir < 5){
  winddir = 0;
  ponto = "Norte";
 }
  if(dir >= 5 & dir < 16){
  winddir = 11.25;
 ponto = "Norte";
 }
  if(dir >= 16 & dir < 27){
  winddir = 11.25*2;
  ponto = "Norte";
 }
  if(dir >= 27 & dir < 38){
  winddir = 11.25*3;
  ponto = "Nordeste";
 }
  if(dir >= 38 & dir < 50){
  winddir = 11.25*4;
  ponto = "Nordeste";
 }
  if(dir >= 50 & dir < 61){
  winddir = 11.25*5;
  ponto = "Nordeste";
 }
  if(dir >= 61 & dir < 72){
  winddir = 11.25*6;
  ponto = "Nordeste";
 }
   if(dir >= 72 & dir < 83){
  winddir = 11.25*7;
  ponto = "Leste";
 }
   if(dir >= 83 & dir < 95){
  winddir = 11.25*8;
  ponto = "Leste";
 }
   if(dir >= 95 & dir < 106){
  winddir = 11.25*9;
  ponto = "Leste";
 }
   if(dir >= 106 & dir < 117){
  winddir = 11.25*10;
  ponto = "Leste";
 }
   if(dir >= 117 & dir < 128){
  winddir = 11.25*11;
  ponto = "Sudeste";
 }
   if(dir >= 128 & dir < 140){
  winddir = 11.25*12;
  ponto = "Sudeste";
 }
   if(dir >= 140 & dir < 151){
  winddir = 11.25*13;
  ponto = "Sudeste";
 }
   if(dir >= 151 & dir < 162){
  winddir = 11.25*14;
  ponto = "Sudeste";
 }
   if(dir >= 162 & dir < 173){
  winddir = 11.25*15;
  ponto = "Sul";
 }
   if(dir >= 173 & dir < 185){
  winddir = 11.25*16;
  ponto = "Sul";
 }
   if(dir >= 185 & dir < 196){
  winddir = 11.25*17;
  ponto = "Sul";
 }
   if(dir >= 196 & dir < 207){
  winddir = 11.25*18;
  ponto = "Sul";
 }
   if(dir >= 207 & dir < 218){
  winddir = 11.25*19;
  ponto = "Sudoeste";
 }
   if(dir >= 218 & dir < 230){
  winddir = 11.25*20;
  ponto = "Sudoeste";
 }
   if(dir >= 230 & dir < 241){
  winddir = 11.25*21;
  ponto = "Sudoeste";
 }
   if(dir >= 241 & dir < 252){
  winddir = 11.25*22;
  ponto = "Sudoeste";
 }
   if(dir > 252 & dir < 263){
  winddir = 11.25*23;
  ponto = "Oeste";
 }
   if(dir >= 263 & dir < 275){
  winddir = 11.25*24;
  ponto = "Oeste";
 }
   if(dir >= 275 & dir < 286){
  winddir = 11.25*25;
  ponto = "Oeste";
 }
   if(dir >= 286 & dir < 297){
  winddir = 11.25*26;
  ponto = "Oeste";
 }
   if(dir >= 297 & dir < 308){
  winddir = 11.25*27;
  ponto = "Oeste";
 }
   if(dir >= 308 & dir < 320){
  winddir = 11.25*28;
  ponto = "Noroeste";
 }
   if(dir >= 320 & dir < 331){
  winddir = 11.25*29;
  ponto = "Noroeste";
 }
   if(dir >= 331 & dir < 342){
  winddir = 11.25*30;
  ponto = "Noroeste";
 }
   if(dir >= 342 & dir < 354){
  dir = 11.25*31;
  ponto = "Norte";
 }

 
}


  outData++;
}


//fim da comunicação I2C como a meteo

time7=millis();
  }


//análise da direção de maior ocorrencia
  if((millis()-tempo32) >= 240000){
   
    pass=0;
  while(pass <= 31){
   ocorr[pass]=0;
   classespeeds[pass]=0.0;
  pass++;
  //Serial.println("travou 7 ");
}
    tempo32 = millis();
  }


  analizwind();

}



//aqui toman-se as decisøes com base nos sensores e operação
void operation(){

//Elevação da RPM por angulo de pitch
if(rpm < rpmmax1 & autosafe == 0 & autopitch == 1 & Idle == 0 & offline == 0 & manual == 0 & online == 1){
  if(pitchReq < 45 & (millis()-tempo11) > 500){
pitchReq = pitchReq + 1;
tempo11 = millis();
}

//direcionamento para a melhor velocidade de vento, adicionar: & windspeed >= cutIn
if(offline == 0 & manual == 0 & (online == 1 | Idle == 1 | autosafe == 1)){
  deg = winddirMF;
  //deg = 180;
}

}

if(offline == 0){
//ativação automática do arrefecimento
if(tempgerador >= temparref & a == 0){
  onarref();
  a=1;
}
if(tempgerador <= 40.0 & a == 1){
offarref();
a=0;
}
}


//liberação do rotor
if(autosafe == 0 & brake == 1 & a1 == 0 & windspeed >= cutIn & windspeed <= cutOff2 & offline == 0 & manual == 0){ 
  freerotor();
}



//retomada de operação após normalização de parâmetros
if((millis()-tempo16) >= temporet & rpm < 50 & windspeed < cutOff2 & tSis >= 11.0 & tPot >= 11.5 & tempgerador <= 40.0 & tempsistema <= 60.0){
  autosafe=0;
  a3=0; //permite novamente a entrada no bloqueio do rotor em autosafe e baixa velocidade
  a5 = 0; //impede a entrada rechamada do autosafe
  a7=0; //entrada no modo online
  online=1;
}



//*************************************************************************************************
//Rotinas de segurança do sistema


//Redução do angulo de pitch em altas rotações
if(rpm > rpmmax1 & millis()-tempo10 > 5000 & autosafe == 0){

if(pitchReq >= 5){
pitchReq = pitchReq - 5;

}
if(pitchReq < 5 & pitchReq >= 1){
pitchReq = pitchReq - 1;

}
 tempo10 = millis();
  
}


//Auto-Safe por alta rpm - rpmmax2
if(rpm >= rpmmax2){
autosafe = 1; 
tempo16 = millis();
}

//Auto-Safe por cutOff de vento
if(windspeed >= cutOff){
autosafe = 1; 
tempo16 = millis();
}

//Auto-Safe por alta temperaura do gerador
if(tempgerador >= 55.0){
  autosafe = 1;
}

//Auto-Safe por alta temperaura do sistema
if(tempsistema >= 65.0){
  autosafe = 1;
}

//Auto-Safe por baixa tensão no sistema de controle
if(tSis <= 10.0){
  autosafe = 1;
}

//Auto-Safe por baixa tensão no sistema de potência
if(tPot <= 8.0){
  autosafe = 1;
}


//Freagem do sistema após redução da rpm
if(rpm <= rpmBrake & a3 == 0 & autosafe == 1){
  braking=1;
  a3=1;
}


//bloqueio automático do rotor com base em alta rpm - condição crítica - para entrar nesta função o pitch deve ter falhado.
if(rpm >= rpmmax3 & brake == 0){
  braking = 1;
  tempo16 = millis();
  noti = "Condição Crítica do sistema - Velocidade extrema!";
  datalog();
  noti = "Possível falha no sistema de pitch";
  datalog();
  noti = "Possível falha no sistema de freios";
  datalog();
}

//para entrar na freagem automática do rotor
if(braking == 1){
  brakerotor();
}



//rotina de definição do status do sistema

//Modo Auto-safe
if(autosafe == 1 & a5 == 0){
 AutoSafeMode();
}

//Modo IDLE
if(autosafe == 0 & Idle == 1 & a6 == 0){
  IdleMode();
}

//Modo Online
if(autosafe == 0 & online == 1 & a7 == 0){
  onlineMode();
}

//Modo Offline
if(offline == 1 & a8 == 0){
  offlineMode();
}

//Modo Manual
if(manual == 1 & a9 == 0){
  manualMode();
  autopitch = 0;
  automatico = 0; 
}
if(emergencia == 1 & a11 == 0){
  emergenciMode();
}


if(automatico == 1){

//Ajuste no sistema de pitch
ajustepitch();

}



}//fim do operation()




void ajustepitch(){

  mediapotpitch();

  //ajuste no sentido horário
  if(medpotpitch > pitchReq & tent3 < 2){
   
   //aumenta a pwm
   if((millis()-tempo9) >= (pwmpitch/50) & pwmpitch < 255){
    pwmpitch++;
    tempo9 = millis();
   }

   
    ledcWrite(0, pwmpitch);
  }


    //ajuste no sentido Anti-horário
  if(medpotpitch < pitchReq & tent3 < 2){
   
   //aumenta a pwm
   if((millis()-tempo9) >= (pwmpitch/50) & pwmpitch < 255){
    pwmpitch++;
    tempo9 = millis();
   }

   
    ledcWrite(1, pwmpitch);
  }

  if(medpotpitch == pitchReq){

    pwmpitch = 0;
    ledcWrite(1, pwmpitch);
    ledcWrite(0, pwmpitch);
    
    if(tent3 < 11){
      tent3++;
    }

  }
  if(medpotpitch != pitchReq & tent3 > 0){
    tent3=0;
  }



  
}



//entrada dos modos de operação do sistema

//Modo Auto-Safe - aqui é feito o rastreamento constante do vento com rotor bloqueado, e pitch em 0, até que as condições de normalidade sejam reestabelecidas
void AutoSafeMode(){
//1-levar o amgulo de pitch para 0
pitchReq=0;
//2-alinhar a nacele com a origem do vento ou mantê-la nesta posição
deg = winddirMF;
    //*as ações 1 e 2 devem ser executadas simultaneamente 
//3-bloqueia rotação da nacele - esta função é executada automaticamente no final do posicionamento.

automatico=1;

autosafe=1;
Idle=0;
online=0;
offline=0;
manual=0;
emergencia=0;
Status = "Auto-Safe";

a5=1;
}

//Modo Idle - aqui é feito o constante rastreamento da melhor velocidade do vento, mas o pitch se mantem em 0, o sistema está pronto para operar em qualquer intante 
void IdleMode(){
pitchReq=0;
automatico = 1;

autosafe=0;
Idle = 1;  
online=0;
offline=0;
manual=0;
emergencia=0;
Status = "Idle";

a6=1;
}

//Modo Online - aqui a torre eólica opera normalmente, rodando todas as funcões simultanemanete
void onlineMode(){
automatico = 1;
autopitch = 1; 

autosafe=0;
Idle = 0;  
online=1;
offline=0;
manual=0;
emergencia=0;
Status = "Online";

a7=1;
}

//Modo Offline - aqui a torre eólica se mantém completamente desativada, apena as rotinas de segurança pode rodar, é importnate ressaltar que a melhor direcão de vento nao é rastreada aqui,
void offlineMode(){
automatico = 1;
autopitch = 0;
autoposi = 0; 

autosafe=0;
Idle = 0;  
online=0;
offline=1;
manual=0;
emergencia=0;
Status = "Offline";

a8=1;
}

//Modo Manual - aqui a torre eólica so pode ter sua condição física alterada manualmente via terminal apenas as rotinas de segurança se mantem acessiveis automáticamente
void manualMode(){
automatico = 1;
autopitch = 0; 
autoposi = 0; 

autosafe=0;
Idle = 0;  
online=0;
offline=0;
manual=1;
emergencia=0;
Status = "Manual";

a9=1;
}

//Modo Parada de emegência - aqui todos o sistemas da torre são desativados, ou seja, todas as portas do ESP32 são desativadas em termos de ativação e ela permanece em um loop constante 
void emergenciMode(){
automatico = 0;
autopitch = 0; 
autoposi = 0; 

autosafe=0;
Idle = 0;  
online=0;
offline=0;
manual=0;
emergencia=1;
Status = "Emergencia";
a11=1;

while(a11 == 1){
  digitalWrite(arref, HIGH);
  digitalWrite(ncBrake, HIGH);
  digitalWrite(ncFree, HIGH);
  digitalWrite(rtBrake, LOW);
  digitalWrite(rtFree, LOW);

ledcWrite(0, 0);
ledcWrite(1, 0);
ledcWrite(2, 0);
ledcWrite(3, 0);
ledcWrite(4, 0);
ledcWrite(5, 0);

}
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
delay(200); 
digitalWrite(ncBrake, HIGH);
delay(200);
digitalWrite(ncBrake, LOW); 
delay(700); 
digitalWrite(ncBrake, HIGH);
ncState = "locked";
a10 = 1; 
}

//function to free the nacele rotation
void freenacele(){
digitalWrite(ncFree, LOW); 
delay(200); 
digitalWrite(ncFree, HIGH);
delay(200);
digitalWrite(ncFree, LOW); 
delay(400); 
digitalWrite(ncFree, HIGH);
ncState = "liberate"; 
a10 = 0;
}

//function to brake the rotor rotation
void brakerotor(){


if((millis() - tempo3) >= (800/(pwmbrake+1)) & pwmbrake < 255){
  pwmbrake++; 
  tempo3 = millis();
  rtState = "locking";
}

if(pwmbrake == 255 & (millis()-tempo3) >= 200){
  ledcWrite(2, 0);
  braking=0;
  brake=1;
  rtState = "locked";
  pwmbrake=0;
  a1=0;
}
   ledcWrite(2, pwmbrake);

 
}

//function to free the rotor rotation
void freerotor(){

ledcWrite(3, 255);
delay(500);
ledcWrite(3, 0);
delay(100);
ledcWrite(3, 255);
delay(200);
ledcWrite(3, 0);
a1=1;
brake=0;  
rtState = "liberate"; 


}


//botões do terminal


void ajustenacele(){
//Def de ajuste normal -- ajusta o valor de intervalo para não ficar negativo
mediapot();
if(deg > medpot){ 
  inter = deg - medpot;
}
if(deg < medpot){
  inter = medpot - deg;
}

//Renovação do tent e tent1
if(deg != olddeg){
  tent=0;
  tent1=0; 
}

//quando não é um ajuste de precisão -> inter > 10
if(deg != medpot & tent <= 1 & inter > 4){


mediapot();
posi = medpot;//guarda o valor para fins de fer. no aumento da pwm
olddeg=deg;
tent++; // permite 2 ajustes de posição
valpwm = 30;
ultimointer = 0;

//ajusta o valor de intervalo para não ficar negativo
if(deg > medpot){ 
  inter = deg - medpot;
}
if(deg < medpot){
  inter = medpot - deg;
}


tempo14 = millis();
//inicia a rotação o aumento de pwm no sentido Anti-horário
while(deg > medpot){

if(a10 == 1 & valpwm > 40){
  freenacele();
}
  Serial.println("          entrou no while horario");

mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((medpot-posi) < (inter*0.85)  & (millis()-tempo1) > 30){
   tempo1 = millis();
  if(valpwm < 210){
    valpwm++;
     }
}

//ajuste do valor da pwm - desacelaração
if((medpot-posi) > (inter*0.15) & (millis()-tempo1) >= 1){
  tempo1 = millis();
  if(valpwm > 90){
   valpwm=valpwm-3;
     }
}


ledcWrite(4, valpwm);//envia os dados para a pontencia


//anula o loop se passar de 4 min
 if((millis()-tempo14) > 80000){
 ledcWrite(4, 0);
 ledcWrite(5, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }

 //retorno de funções 
 readsensors(); 

readminimet(); 

operation();

webServer();

Display();
 
}
 ledcWrite(4, 0);
 ledcWrite(5, 0);


mediapot();//atualiza os valores de posição


tempo14 = millis();
//inicia a rotação e aumento de pwm no sentido Horário
while(deg < medpot){

if(a10 == 1 & valpwm > 40){
  freenacele();
} 
Serial.println("         entrou no while antihorario");


mediapot();//atualiza os valores de posição

//ajuste do valor da pwm - aceleração
if((posi-medpot) < (inter*0.85)  & (millis()-tempo1) > 30){
   tempo1 = millis();
  if(valpwm < 255){
    valpwm++;
     }
}

//ajuste do valor da pwm - desacelaração
if((posi-medpot) > (inter*0.15) & (millis()-tempo1) >= 1){
  if(valpwm > 90){
   valpwm=valpwm-3;
     }
}


ledcWrite(5, valpwm);//envia os dados para a pontencia


//anula o loop se passar de 2.5 min
 if((millis()-tempo14) > 80000){
 ledcWrite(4, 0);
 ledcWrite(5, 0);
 tent=10;
 tent1=10;
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Tempo max. Atingido");
 lcd.setCursor(1, 1);
 lcd.print("--Anulando Rotacao--");
 delay(5000);
  break;
 }

 //retorno de funções 
readsensors(); 

readminimet(); 

operation();

webServer();

Display();

 
}
 ledcWrite(4, 0);
 ledcWrite(5, 0);

}//fim do ajuste normal

if(a10 == 0 & (tent1 > 0 | deg == medpot)){
  brakenacele();
}

if(valpwm == 0 & a10 == 0){
  brakenacele();
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
              //digitalWrite(output27, HIGH);
           
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              //digitalWrite(output27, LOW);
          
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



//para a análise de vento
void analizwind(){
  
  if((millis()-tempo30) >= 5000){
    //distribuis as variáveis nos vetores
    winddirs[numposi] = winddir;
    windspeeds[numposi] = windspeed;
        //reinicio do ajuste de variáveis
    if(numposi == 49){
      numposi=0;
      }
    else{
       numposi++;
    }
    //Serial.println("renonvar................................................................................... ");
  
  tempo30 = millis();
  
  }

  pass=0;
  pass1=0;

  
  //análise dos dados contidos nos vetores
if((millis()-tempo31) > 25000){
 

//contagem de ocorrências e tirando as médias de velocidade para cada posição.
  pass=0;
  pass1=0;
while(pass <= 31){

  ocorr[pass]=0; //zera o vetor na posição pass, para não somar com o anterior
  while(pass1 <= 49){ //aqui etava numposi no lugar de 49
    if(classes[pass] == winddirs[pass1]){
              // Serial.print("vetor windspeeds/;");
         //Serial.println(windspeeds[pass1]);
         if(ocorr[pass] == 0){
          classespeeds[pass]=0;
         }
      classespeeds[pass] = windspeeds[pass1] + classespeeds[pass];
      ocorr[pass]++; 
    }
    pass1++;
  }
  if(ocorr[pass] != 0){
         ///Serial.print("vetor classesspeeds/;");
         //Serial.println(classespeeds[pass]);
       
  classespeeds[pass] = classespeeds[pass]/ocorr[pass];
  
  }
 
  //Serial.print("Classe: ");
  //Serial.println(classes[pass]);
  //Serial.print("média de velocidade da classe: ");
  //Serial.println(classespeeds[pass]);
  //Serial.print("ocorrencias: ");
  //Serial.println(ocorr[pass]);
  

  pass++;
  pass1=0;
}
  pass=0;
  pass1=0;

  
//verificação da maior ocorrência e se é cut-in
 maiorocorr=0;
  while(pass <= 31){//verifica qual o mais recorrente
    //Serial.print("entrou no ultimo while -> velocidade: ");
    //Serial.println(classespeeds[pass]);
    if(ocorr[pass] > maiorocorr & classespeeds[pass] >= cutin){
      maiorocorr = ocorr[pass];
      posimf = classes[pass];
      velposimf = classespeeds[pass];
    }
    pass++;
   }
   //deg = posimf;
   winddirMF = posimf;
   
pass1=0;
pass=0;

 tempo31 = millis();
}

}//fim da função analzwind


void settozero(){
  //zerando os vetores
  while(pass <= 49){
   winddirs[pass]=1;
  pass++;
  //Serial.println("travou 6 :  ");
  //Serial.print(pass);
}
pass=0;
  while(pass <= 31){
   ocorr[pass]=0;
   classespeeds[pass]=0.0;
  pass++;
  ///Serial.println("travou 7 ");
}
pass=0;

  while(pass <= 49){
   windspeeds[pass]=0;
  pass++;
  //Serial.println("travou 8 ");
}
pass=0;
//fim do zeramento dos vetores

  pass=0;
  pass1=0;
  numposi=0;
}



  void Display(){
    //print 1 no display
if((millis()-time2) >= 1500 & b1 == 1){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("LAZZARUS_Eolica V2.1");

  if(medpotpitch != pitchReq){
lcd.setCursor(0, 1);
lcd.print("Pitch At:");
lcd.print(medpotpitch);
lcd.print(" ");

lcd.setCursor(11, 1);
lcd.print("Req:");
lcd.setCursor(11, 1);
lcd.print("      ");
lcd.print(pitchReq);
    
  }else{//se o pitch nao está sendo usado printa os dados de temperatura
lcd.setCursor(0, 1);
lcd.print("TGer:");
lcd.print(tempgerador,1);
lcd.print("C");

lcd.setCursor(11, 1);
lcd.print("Ts:");
lcd.print(tempsistema,2);
lcd.print("C");
}

lcd.setCursor(11, 2);
lcd.print("ts:");
lcd.print(tSis,1);
lcd.print("V");

lcd.setCursor(0, 2);
lcd.print("tPot:");
lcd.print(tPot,1);
lcd.print("V");  


lcd.setCursor(0, 3);
lcd.print("U");
lcd.setCursor(1, 3);
lcd.print(windspeed,1);
lcd.setCursor(6, 3);
lcd.print("D");
lcd.setCursor(7, 3);
lcd.print(winddir,1);
lcd.setCursor(13, 3);
lcd.print("MF");
lcd.setCursor(15, 3);
lcd.print(winddirMF,1);

b1 = 0; 
time2 = millis(); 
}

    //print 2 no display
if((millis()-time2) >= 1500 & b1 == 0){
lcd.clear();
lcd.setCursor(0, 0);
//lcd.print("LAZZARUS_Eolica V2.1");
lcd.print(Vangular);

lcd.setCursor(0, 1);
lcd.print("Status: ");
lcd.print(Status); 

lcd.setCursor(0, 2);
lcd.print("Pitch At:");
lcd.print(medpotpitch);
lcd.print(" ");

lcd.setCursor(12, 2);
lcd.print("Req:");
lcd.setCursor(16, 2);
lcd.print(pitchReq);

lcd.setCursor(0, 3);
lcd.print("RPM");
lcd.setCursor(3, 3);
lcd.print(rpm);

lcd.setCursor(7, 3);
lcd.print("At");
lcd.setCursor(9, 3);
lcd.print(medpot);

lcd.setCursor(14, 3);
lcd.print("Dr");
lcd.setCursor(16, 3);
lcd.print(deg);

  time2 = millis();
  b1=1;  
}
    
  }


//aqui serão printadas no dataloguer todas as decisões tomadas pelo sistema
  void datalog(){
    
  }
