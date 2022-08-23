/*********
  Alef Júnior &
  André V. de Freitas
*********/

//===============================================
// BIBLIOTECAS
//===============================================

//EXCLUIR DEPOIS
#include <stdio.h>
#include <stdlib.h>

int randomico = 0;

// Conexão WIFI

#include <WiFi.h>;
#include <ESPAsyncWebServer.h>;
#include <Adafruit_Sensor.h>;

// Controle e funcoes

#include <ArduinoOTA.h>;
#include <LiquidCrystal_I2C.h>;
// Sensor BMP280 do sistema
#include <Adafruit_Sensor.h>;
#include <Adafruit_BMP280.h>;
// Barramento I2C, comunicacao via I2C com a minimet
#include <Wire.h>;

//===============================================
// CONEXAO WIFI
//===============================================

// Credenciais para conexão com o Wi-fi
//const char *ssid = "Consultorio";
//const char *password = "terapiaCris";

const char *ssid = "Valter 2.4Ghz";
const char *password = "casa092614";

// const char *ssid = "Quantum Team";
// const char *password = "amotolia_oleosa";

// const char *ssid = "Andre Wifi";
// const char *password = "gostoso1";

// const char *ssid = "PlusEnergy_Control - Host1";
// const char *password = "diaqueohomemfoiemdirecaoalua16";

//===============================================
// PINOS
//===============================================



//===============================================
// INSTANCIACAO E PORTA
//===============================================

// Intanciando o objeto DHT com o tipo de pin que foi definido
// DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


//===============================================
// VARIAVEIS
//===============================================

// LiquidCrystal
LiquidCrystal_I2C lcd(0x25, 20, 4);

Adafruit_BMP280 bmp; // OBJETO DO TIPO Adafruit_BMP280 (I2C)

byte outData = 1;
float inByte = 0;
String inString = "";        //string para armazenar bytes enviados pelo slave

float winddir = 0;
double windspeed = 0;
float winddirMF = 0;

float info1 = 0;
int msgconvert = 0;
int freee = 0;


//variáveis parametrais
int rpmmax1 = 400; //1500 RPM para redução automática do andulo de pitch
int rpmmax2 = 600;  // 1700 rpm para entrar no auto safe
int rpmmax3 = 650; //2200 . //rpm para última tentativa de segurança bloaquear o rotor intependente da velocidade
int rpmBrake = 50; //rpm para travar o rotor em Auto-Safe normal, ou desativação do sistema.
int rpmcarga = 200; //rpm para conexao do gerador na carga
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
#define sinalRPM 25

//Variáveis da mediçnao de RPM
int rpm = 0; //rmp gerador
int rpmRot = 0; //rpm rotor
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int pulsos_por_volta = 3; //furos do disco

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
int armazenaModo;
int operationMode = armazenaModo;


//variáveis sensores
float tempgerador = 0;
float tempsistema = 0;
float tSis = 0;
float tensaoSis = 0;
float tensaoPot = 0;
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
int b1 = 0;


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
int potnacele = 0;
long int tempo14 = 0;//tempo máximo para anular o loop de giro da nacele
long int tempo20 = 0;
int ultimointer = 0;
float Vangular = 0;
long int tempo40 = 0;
int posicao = 0;
String pontoNacele = "";


//para referência do pitch
int freq = 5000;
int channel = 0;
int channel1 = 1;
int resolution = 8;
int dutyCycle = 150;
int dutyCyclePitch1 = 0;
int medpotpitch = 0; //armazena o valor lido do pot
int potpitch = 0;
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


// Led placa da própria placa LED DE TESTE (PODE SER TIRADO DEPOIS)
int LED_BUILTIN = 2;
int contaTudo = 0;



//-------------------------------
// Variaveis para o WebServer
//-------------------------------

// Variaveis de contagem para teste das funcoes
// int cont = 0;
int cont1 = 1;
int controle0 = 0;
int controle1 = 0;
int controle2 = 0;
int controle3 = 0;

// Captura dos parametros
String btnParam1; // Nome do botao
String btnParam2; // Estado do botao
String valueParam3; // Valor do input
int valueParam3Int = 0; // Valor do input (numerico)
String inputParam4; // Nome do input
int modoParam5Int = 0; // Valor do modo (numerico)
String modoParam5; // Qual modo de operacao

// Receber o estado dos botoes
int btnParam2Int = 0;
int btnFreioRotor = 0;
int btnFreioNacele = 0;
int btnAcionaManual = 0;
int btnAcionaOnline = 0;
int btnAcionaOffline = 0;
int btnAcionaIdle = 0;
int btnAcionaParadaEmergencia = 0;
int btnAcionaAutosafe = 0;
int btnArrefecimento = 0;
int btnLiberaBotoes = 0;
int btnAtualizasoftware = 0;

// Controle webpage
const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

const char *PARAM_INPUT_3 = "value";
const char *PARAM_INPUT_4 = "input";

const char *PARAM_INPUT_5 = "modo";



//===============================================
// LENDO OS SENSORES E OS TRANSFORMANDO EM STRING
//===============================================

// Lê temperatura e converte para string
// String readDHTTemperature() {
//  Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//  Read temperature as Celsius (the default)

// Lê a temperatura do objeto declarado anteriomente, em float
// float t = dht.readTemperature();

// Read temperature as Fahrenheit (isFahrenheit = true)
// float t = dht.readTemperature(true);

// Check if any reads failed and exit early (to try again).
// if (isnan(t)) {
//   Serial.println("Failed to read from DHT sensor!");
//   return "--";
// }
// else {
//   Serial.println(t);
//   return String(t);
// }
//}

// Contadores de teste
String readContaTudo() {
  //  contaTudo++;
  //  Serial.println(contaTudo);
  //  return String(contaTudo);
}

// Temperatura Gerador
String readTempGerador()
{
  return String(Tgerador);
}

// Temperatura do sistema
String readTempSistema()
{
  return String(tempsistema);
}

// Tensão do sistema
String readTensaoSistema()
{
  return String(tSis);
}

// Tensão de potencia
String readTensaoPotencia()
{
  return String(tPot);
}

// RPM gerador
String readRPMGerador()
{
  return String(rpm);
}

// RPM Rotor
String readRPMRotor()
{
  return String((rpm / 32));
}

// Angulo de Pitch
String readPosiPitch()
{
  return String(medpotpitch);
}

// Posição da nacele
String readPosiNacele()
{
  return String(medpot);
}

// Velocidade do vento
String readWindspeedAt()
{
  return String(windspeed);
}

// Velocidade media na direção mais frequente
String readWindspeedMF()
{
  return String(velposimf);
}

// Direção atual do vento
String readWinddirAt()
{
  return String(winddir);
}

// Direção mais frequente do vento
String readWinddirMF()
{
  return String(winddirMF);
}

//COLOCAR DIRECAO BIRUTA
String readDirecaoBiruta() {
  return String(ponto);
}

String readDirecaoNacele()
{
  return String(pontoNacele);
}

//Status do freio nacele
String readFreioNacele() {

  if (a10 == 1) {
    controle1 = 1;
  } else if (a10 == 0) {
    controle1 = 0;
  }
  return String(controle1);
}

//Status do freio rotor
String readFreioRotor() {

  if (brake == 1) {
    controle2 = 1;
  } else if (brake == 0) {
    controle2 = 0;
  }
  return String(controle2);
}

//Status do arrefecimento
String readArrefecimento() {

  if (arrefState == "On") {
    controle3 = 1;
  } else if (arrefState == "Off") {
    controle3 = 0 ;
  }
  return String(controle3);
}

//Status do modo de operação
String readModoOperacao() {

  if ((armazenaModo != 0) && (armazenaModo != 1) && (armazenaModo != 2) && (armazenaModo != 3) && (armazenaModo != 4) && (armazenaModo != 5) && (armazenaModo != 6)){
    armazenaModo = 0;
  }

  int resultado = ((operationMode*10)+armazenaModo);
  
  return String(resultado);
}

// OLHAR ESSE SITE A ADAPTAR O CODIGO
// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/

//===============================================
// PAGINA WEB
//===============================================

// Configurando a pagina web
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Icone barra-->
    <!-- <link rel="icon" href="img/plus energy png em baixo.png"> -->
    <title>Sistema de Gerenciamento Torre Eolica - Plus Energy</title>
    
    <style type="text/css">
        html {font-family: Helvetica;font-size: 16px;text-align: left;background-color: #bff1fa;background-repeat: repeat-y;background-size: cover;}
        input {padding: 2.5px 2.5px;font-size: 1.5rem;width: 7rem;height: 2.5rem;text-align: center;border: #666666 solid 2px;border-radius: 15px 50px;background-color: #e4e4e4;}
        input#status_sistema {padding: 2.5px 2.5px;font-size: 1.3rem;margin: 3rem 0.5rem;width: 7rem;height: 2.5rem;text-align: center;border: #666666 dotted 2px;border-radius: 15px 50px;box-shadow: #747474 -1px 1px 7px;background-color: #e4e4e4;}
        input#status_sistema.status_sistema_online {background-color: #4CAF50;color: white;}
        input#status_sistema.status_sistema_offline {background-color: #1f1e1e;color: white;}
        input#status_sistema.status_sistema_idle {background-color: #e4e4e4;color: rgb(88, 88, 88);}
        input#status_sistema.status_sistema_manual {background-color: #471e7c;color: white;}
        input#status_sistema.status_sistema_autosafe {background-color: #ffbb00;color: white;}
        input#status_sistema.status_sistema_parada_emergencia {background-color: #be0000;color: white;}
        input#status_atual_nacele {padding: 2.5px 2.5px;font-size: 1.5rem;text-align: center;border: #666666 dotted 2px;border-radius: 15px 50px;box-shadow: #747474 -1px 1px 7px;background-color: #e4e4e4;}
        input#status_atual_nacele.status_atual_nacele_desbloqueada {background-color: #4CAF50;color: white;}
        input#status_atual_freio_nacele {background-color: #4CAF50;color: white;font-size: 1.2rem;}
        input#status_atual_freio_nacele.status_atual_freio_nacele_freado {background-color: #d47800;color: white;}
        input#status_atual_freio_rotor {background-color: #4CAF50;color: white;font-size: 1.2rem;}
        input#status_atual_freio_rotor.status_atual_freio_rotor_freado {background-color: #d47800;color: white;}
        input#status_atual_pitch {padding: 2.5px 2.5px;font-size: 1.5rem;text-align: center;border: #666666 dotted 2px;border-radius: 15px 50px;box-shadow: #747474 -1px 1px 7px;background-color: #e4e4e4;}
        input#status_atual_pitch.status_atual_pitch_desbloqueado {background-color: #4CAF50;color: white;}
        input.mostra_info {padding: 2.5px 2.5px;font-size: 1.5rem;margin: 1rem 0.5rem;width: 7rem;height: 2.5rem;text-align: center;border: #666666 solid 2px;border-radius: 10px 40px;background-color: #e4e4e4;}
        h2 {color: #00777e;margin: 0.5rem 0rem;font-weight: 500;}
        body::-webkit-scrollbar {width: 12px;}
        body::-webkit-scrollbar-track {background: #00AAB4;}
        body::-webkit-scrollbar-thumb {background-color: #b1ffb1;border-radius: 20px;border: 3px solid #00AAB4;}
        .hide {display: none;}
        .show {display: block;}
        .button {color: white;padding: 0.5rem 1rem;text-decoration: none;font-size: 1.5rem;margin: 0.1rem;cursor: pointer;text-align: center;border: #969595 solid 3px;}
        .button:hover {background-color: #4CAF50;border: #dfdddd solid 3px;}
        .button:disabled:hover {background-color: #afafaf;}
        .button-off {background-color: #afafaf;}
        .button-on {background-color: #348137;border: #dfdddd solid 3px;}
        .title-container {background-color: #2377a2;text-align: center;margin-top: -0.01rem;margin-left: -0.1rem;margin-right: -0.1rem;margin-bottom: 0.6rem;position: sticky;top: 0;border: 2px solid #ffffff;padding-left: 0.5rem;padding-right: 0.5rem;}
        .wrapper {display: grid;}
        .column-gap {column-gap: 0.5rem;}
        .grid-titulo {grid-template-columns: 1fr 9fr;}
        .grid-col-2-8-4 {grid-template-columns: 2fr 8fr 4fr;}
        .grid-col-8-2 {grid-template-columns: 8fr 2fr;}
        .grid-col {grid-template-columns: 1fr;}
        .grid-col-2 {grid-template-columns: 1fr 1fr;}
        .grid-col-3-repeat {grid-template-columns: repeat(3, 3fr);}
        .grid-col-4-repeat {grid-template-columns: repeat(2, 2fr) 3fr 3fr;}
        .bloco-col {border-right: #7c7c7c7c groove 0.1rem;text-align: center;padding-top: 0.5rem;padding-bottom: 0.5rem;}
        .border-row {border: #00AAB4 solid 0.2rem;border-radius: 5px;}
        .border-none {border: none;}
        .img-title {margin-top: 0.5rem;}
        .background-img {background-color: #fff;margin-left: -0.6rem;}
        .mx-1 {margin-left: 1rem;margin-right: 1rem;}
        .mt-05 {margin-top: 0.5rem;}
        .mt-1 {margin-top: 1rem;}
        .mt-105 {margin-top: 1.5rem;}
        .mb-05 {margin-bottom: 0.5rem;}
        .mb-1 {margin-bottom: 1rem;}
        .mb-105 {margin-bottom: 1.5rem;}
        .pt-1 {padding-top: 1rem;}
        .pt-2 {padding-top: 1.5rem;}
        .py-2 {padding-top: 3rem;padding-bottom: 2rem;}
        .align-center {text-align: center;}
        .center {align-self: center;}
        .background-rows {background-color: #f8f8ff;}
        .p-col-title {color: #00AAB4;text-align: center;font-size: 1.2rem;margin-bottom: 0.2rem;margin-top: 0.2rem;}
        .p-dado-sensor {color: #666666;text-align: center;font-size: 1.2rem;}
        #h1-title {font-size: 2.0rem;text-align: center;color: #fff;text-transform: uppercase;padding-right: 1.5rem;}
    </style>
</head>
<body>
     <!-- (CASO NECESSÁRIO) PLACEHOLDER BOTOES COL1_LIN1_1 Freio Nacele (COL = Coluna | LIN = Linha | Número = Parte)-->
     <!--%BOTOES_COL1_LIN_1% -> é um placeholder que é subtituido ao processar o codigo-->
     <!--%BOTOES_COL1_LIN1_1%-->
    <div class="wrapper display-sticky title-container">
        <div>
            <h1 id="h1-title" class="">Sistema de Controle - Torre Eolica</h1>
        </div>
    </div>

    <!-- Linha 1 -->
    <div class="wrapper grid-col-8-2 column-gap ">

        <!-- Coluna 1 -->
        <div class="wrapper grid-col-4-repeat background-rows border-row">
            <div class="bloco-col pt-1">
                <div>
                    <p class="p-col-title">Freio Nacele</p>
                    <input type="text" name=" " id="status_atual_freio_nacele"
                         value="Desacoplado" disabled>
                </div>
                <div class="mt-105 pt-1">
                    <p class="p-col-title">Freio Rotor</p>
                    <input type="text" name="status_atual_freio_rotor" id="status_atual_freio_rotor"
                         value="Desacoplado" disabled>
                </div>
            </div>
            <div class="bloco-col pt-1">       
               <div>
                    <p class="p-col-title">RPM Rotor</p>
                    <p class="p-dado-sensor">
                        <span id="rpmrotor">%RPMROTOR%</span>
                        <sup class="units">RPM</sup>
                    </p>
                </div>
                <div class="mt-105 pt-1">
                    <p class="p-col-title">RPM Gerador</p>
                    <p class="p-dado-sensor">
                        <span id="rpm_gerador">%RPMGERADOR%</span>
                        <sup class="units">RPM</sup>
                    </p>
                </div>
            </div>
            <div class="bloco-col pt-1">
                <div>
                    <p class="p-col-title">Temperatura Sistema</p>
                    <p class="p-dado-sensor">
                        <span id="temp_externa">%TEMPEXTERNA%</span>
                        <sup class="units">&deg;C</sup>
                    </p>
                </div>
                <div class="mt-105 pt-1">
                    <p class="p-col-title">Temperatura Gerador</p>
                    <p class="p-dado-sensor">
                        <span id="temp_gerador">%TEMPGERADOR%</span>
                        <sup class="units">&deg;C</sup>
                    </p>
                </div>
            </div>
            <div class="bloco-col pt-1 border-none">
              <div>
                    <p class="p-col-title">Tensao Sistema</p>
                    <p class="p-dado-sensor">
                        <span id="tensao_sistema">%TENSAOSISTEMA%</span>
                        <sup class="units">V</sup>
                    </p>
                </div>
                <div class="mt-105 pt-1">
                    <p class="p-col-title">Tensao Sis. Potencia</p>
                    <p class="p-dado-sensor">
                        <span id="tensao_sistema_potencia">%TENSAOSISTEMAPOTENCIA%</span>
                        <sup class="units">V</sup>
                    </p>
                </div>
            </div>
        </div>

        <!-- Coluna 2 -->
        <div class="bloco-col pt-1 background-rows border-row">
            <p class="p-col-title">Status Sistema</p>
            <input class="status_sistema_online" type="text" name="status_sistema" id="status_sistema" value="ONLINE" disabled>
        </div>
    </div>

    <!-- Linha 2 -->
    <div class="wrapper grid-col background-rows border-row mt-1">
        <div class="wrapper grid-col-3-repeat">
        
            <!-- Primeira coluna-->
            <div class="bloco-col">

                <!-- Titulo secao -->
                <h2>Estacao Metereologica</h2>
                <hr class="hr">

                <div class="wrapper grid-col-2">
                    <!-- Primeira Parte -->
                    <div>
                        <div>
                            <p class="p-col-title">Velocidade</p>
                            <p class="p-dado-sensor">
                                <span id="velocidade_met">%VELOCIDADE%</span>
                                <sup class="units">m/s</sup>
                            </p>
                        </div>
                        <div class="mt-105 pt-1">
                            <p class="p-col-title">Direcao Atual</p>
                            <p class="p-dado-sensor">
                                <span id="direcao_atual_met">%DIRECAOATUALMET%</span>
                                <sup id="direcao_biruta" class="units">/ %DIRECAOCARDIALBIRUTA%</sup>
                            </p>
                        </div>
                    </div>

                    <!-- Segunda Parte -->
                    <div>
                        <div>
                            <p class="p-col-title">Velocidade Media</p>
                            <p class="p-dado-sensor">
                                <span id="velocidade_media_met">%VELOCIDADEMEDIAMET%</span>
                                <sup class="units">m/s</sup>
                            </p>
                        </div>
                        <div class="mt-105 pt-1">
                            <p class="p-col-title">Direcao MF</p>
                            <p class="p-dado-sensor">
                                <span id="direcao_final_met">%DIRECAOFINALMET%</span>
                                <sup class="units">&deg;</sup>
                            </p>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Segunda Coluna -->
            <div class="bloco-col">

                <!-- Titulo secao -->
                <h2>Posicionamento da Nacele</h2>
                <hr class="hr">

                <div class="wrapper grid-col-2">

                    <!-- Primeira Parte -->
                    <div class="center">
                        <div>
                            <p class="p-col-title">Posicao Atual</p>
                            <p class="p-dado-sensor">
                                <span id="posicao_atual_nacele">%POSICAOATUALNACELE%</span>
                                <sup id="direcao_nacele" class="units">/ %DIRECAOCARDIALNACELE%</sup>
                            </p>
                        </div>
                    </div>

                    <!-- Segunda Parte -->
                    <div>
                        <div>
                            <p class="p-col-title">Modo</p>
                            <input type="text" name="modo_nacele" id="modo_nacele" class="mostra_info" placeholder=""
                                disabled>
                        </div>
                        <div>
                            <p class="p-col-title">Status Atual</p>
                            <input type="text" name="status_atual_nacele" id="status_atual_nacele"
                                class="status_atual_nacele_desbloqueada" placeholder="OK" disabled>
                        </div>
                    </div>
                </div>
            </div>
             <!-- Terceira Coluna -->
            <div class="bloco-col">

                <!-- Titulo secao -->
                <h2>Posicionamento Controle de Pitch</h2>
                <hr class="hr">

                <div class="wrapper grid-col-2">

                    <!-- Primeira Parte -->
                    <div class="center">
                        <p class="p-col-title">Angulo Atual</p>
                        <p class="p-dado-sensor">
                            <span id="angulo_atual_pitch">%ANGULOATUALPITCH%</span>
                            <sup class="units">&deg;</sup>
                        </p>
                    </div>

                    <!-- Segunda Parte -->
                    <div>
                        <div>
                            <p class="p-col-title">Modo</p>
                            <input type="text" name="modo_atual_pitch" id="modo_atual_pitch" class="mostra_info"
                                placeholder="" disabled>
                        </div>
                        <div>
                            <p class="p-col-title">Status Atual</p>

                            <input type="text" name="status_atual_pitch" id="status_atual_pitch"
                                class="status_atual_pitch_desbloqueado" placeholder="OK" disabled>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Linha 3 -->
    <div class="wrapper grid-col-2-8-4 column-gap mt-1">

        <!-- Coluna 1 -->
        <div class="background-rows border-row">

            <div class="bloco-col border-none">

                <!-- Titulo secao -->
                <h2>Ativa menu botao</h2>
                <hr class="hr">

                %BOTAOLIBERABOTOES%
                
             </div>
         </div>
         
         <!-- Coluna 2 -->
         <div class="wrapper grid-col background-rows border-row">
            <div class="bloco-col border-none">

                <!-- Titulo secao -->
                <h2>Modos de Funcionamento</h2>
                <hr class="hr">

                <div class="wrapper grid-col-3-repeat">

                    <!-- Primeira Parte -->
                    <div>
                        <div>
                            %BOTAOACIONAOFFLINE%
                        </div>
                        <div>
                            %BOTAOACIONAONLINE%
                        </div>
                    </div>

                    <!-- Segunda Parte -->
                    <div>
                        <div>
                            %BOTAOACIONAMANUAL%
                        </div>
                        <div>
                            %BOTAOACIONAPARADAEMERGENCIA%
                        </div>
                    </div>

                    <!-- Terceira Parte -->
                    <div>
                        <div>
                            %BOTAOACIONAIDLE%
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Coluna 3 -->
        <div class="background-rows border-row wrapper ">
            <div class="bloco-col border-none">

                <!-- Titulo secao -->
                <h2>Autosafe</h2>
                <hr class="hr">

                <!-- Primeira Parte -->
                <div>
                    <div>
                        %BOTAOACIONAAUTOSAFE%
                    </div>
                </div>
            </div>
        </div>
    </div>

 <!-- Linha 4 -->
    <div id="modosManuais" class="hide">
        <div class="wrapper grid-col-2-8-4 column-gap mt-1">
            <!-- Coluna 1 -->
            <div class="background-rows border-row">
                <div class="bloco-col border-none">
                    <!-- Titulo secao -->
                    <h2>Freios</h2>
                    <hr class="hr">
                    <!-- Primeira Parte -->
                    <div>
                        <div>
                            %BOTAOFREIONACELE%
                        </div>
                        <div>
                            %BOTAOFREIOROTOR%
                        </div>
                    </div>
                </div>
            </div>
            <!-- Coluna 2 -->
            <div class="wrapper grid-col background-rows border-row">
                <div class="bloco-col border-none">
                    <!-- Titulo secao -->
                    <h2>Ajuste angular</h2>
                    <hr class="hr">
                    <!-- Primeira Parte -->
                    <div>
                        <div id="formNacele" class="mt-105">
                            <p class="p-col-title">Posicionamento Manual Nacele</p>
                            <input type="number" min="0" max="360" id="posicaoManualGraus" name="posicaoManualGraus"
                                onkeyup='controlaGrau("posicao")' placeholder="0º">
                            <button id="modoManualPosicaoEnviar" class="button button-off"
                                onclick='enviaGrausManual("posicaoManualGraus")'>Enviar</button>
                            <!-- <button id="modoManual" class="button button-off" onclick='capturaElemento("modoManual");alerta();'>OFF</button> -->
                        </div>
                    </div>
                    <!-- Segunda Parte -->
                    <div>
                        <div id="formPitch" class="mt-105">
                            <p class="p-col-title">Posicionamento Manual Pitch</p>
                            <input type="number" min="0" max="360" id="pitchManualGraus" name="pitchManualGraus"
                                onkeyup='controlaGrau("pitch")' placeholder="0º">
                            <button id="modoManualPitchEnviar" class="button button-off"
                                onclick='enviaGrausManual("pitchManualGraus")'>Enviar</button>
                        </div>
                    </div>
                </div>
            </div>
            <!-- Coluna 3 -->
            <div class="background-rows border-row wrapper ">
                <div class="bloco-col border-none">
                    <!-- Titulo secao -->
                    <h2>Seguranca</h2>
                    <hr class="hr">
                    <!-- Primeira Parte -->
                    <div>
                        <div>
                            %BOTAOATUALIZASOFTWARE%
                        </div>
                        <div>
                            %BOTAOACIONAARREFECIMENTO%
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</body>

<!-- JavaScript
    ================ -->
    
<script>

//Declarando as funcoes criadas (Declarar as funcoes usando - const = *nome funcao* function(){} - pq ele nao reconhce o function)
//Declarando as variaveis
    const locker = "pluseamelhor";
    var elemento = "", armazenaUltimoModo, controladorBuscaOperacao = 1;

//INTERVALOS DE 0.5 SEGUNDO
    //RPMS
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("rpmrotor").innerHTML = this.responseText;}};
        xhttp.open("GET", "/rpmrotor", true);
        xhttp.send();
        
    }, 500);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("rpm_gerador").innerHTML = this.responseText;}};
        xhttp.open("GET", "/rpm_gerador", true);
        xhttp.send();
        
    }, 500);

    //TENSOES
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("tensao_sistema").innerHTML = this.responseText;}};
        xhttp.open("GET", "/tensao_sistema", true);
        xhttp.send();
        
    }, 500);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("tensao_sistema_potencia").innerHTML = this.responseText;}};
        xhttp.open("GET", "/tensao_sistema_potencia", true);
        xhttp.send();
        
    }, 500);

    //ANGULOS
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("angulo_atual_pitch").innerHTML = this.responseText;}};
        xhttp.open("GET", "/angulo_atual_pitch", true);
        xhttp.send();
        
    }, 500);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();

        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("posicao_atual_nacele").innerHTML = this.responseText;}};
        xhttp.open("GET", "/posicao_atual_nacele", true);
        xhttp.send();
        
    }, 500);

    //VELOCIDADE VENTO
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        

        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("velocidade_met").innerHTML = this.responseText;}};
        xhttp.open("GET", "/velocidade_met", true);
        xhttp.send();
        
    }, 500);


    //INTERVALOS DE 1 SEGUNDO
    //TEMPERATURAS
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("temp_gerador").innerHTML = this.responseText;}};
        xhttp.open("GET", "/temp_gerador", true);
        xhttp.send();
        
    }, 1000);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("temp_externa").innerHTML = this.responseText;}};
        xhttp.open("GET", "/temp_externa", true);
        xhttp.send();
        
    }, 1000);

    //ESTACAO METEREOLOGICA
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("direcao_atual_met").innerHTML = this.responseText;}};
        xhttp.open("GET", "/direcao_atual_met", true);
        xhttp.send();
        
    }, 1000);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("direcao_final_met").innerHTML = this.responseText;}};
        xhttp.open("GET", "/direcao_final_met", true);
        xhttp.send();
        
    }, 1000);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("velocidade_media_met").innerHTML = this.responseText;}};
        xhttp.open("GET", "/velocidade_media_met", true);
        xhttp.send();
        
    }, 1000);

    //FREIOS
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {var modo = this.responseText; mudaCorFreioNacele(modo)}};
        xhttp.open("GET", "/freio_nacele", true);
        xhttp.send();
        
    }, 1000);
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        
        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {var modo = this.responseText; mudaCorFreioRotor(modo)}};
        xhttp.open("GET", "/freio_rotor", true);
        xhttp.send();
        
    }, 1000);

    //ADICOINAR ARREFECIMENTO 
    //INTERVALOS DE 3 SEGUNDOS
    
    //MODO DE OPERAÇÃO 3segundos
      //DEFINE FUNCAO DO INTERVALO
      // ATIVA BUSCA SE controladorBuscaOperacao == 0
      const buscaOperacao  = function () {
        var xhttp = new XMLHttpRequest();
         xhttp.onreadystatechange = function () { if (this.readyState == 4 && this.status == 200) { modo = this.responseText; modoInt = parseInt(modo); console.log("MUDOU OPERACAO:" + modoInt); trataDados(modoInt); } }
         xhttp.open("GET", "/status_modo_operacao", true);
         xhttp.send();
        
        if (controladorBuscaOperacao == 1){
            clearInterval(intervaloOperacao)
            desativouUmaVez = 1;
          }
      }
      //ATIVA INTERVALO
      //var intervaloOperacao = setInterval(buscaOperacao, 3000)
      // FUNCAO PARA ATIVAR O INTERVALO
      const ativaIntervalo = function(){
        controladorBuscaOperacao = 0
        intervaloOperacao = setInterval(buscaOperacao,3000);
        console.log("Ativou essa merda")
      }
    // DIRECAO BIRUTA
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        

        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("direcao_biruta").innerHTML = "/" + this.responseText;}};
        xhttp.open("GET", "/direcao_biruta", true);
        xhttp.send(); 
        
    }, 3000);

    //DIRECAO NACELE
    setInterval(function () {
        var xhttp = new XMLHttpRequest();        

        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {document.getElementById("direcao_nacele").innerHTML = "/" + this.responseText;}};
        xhttp.open("GET", "/direcao_nacele", true);
        xhttp.send(); 
        
    }, 3000);    

    //ARREFECIMENTO
   /* setInterval(function () {
        var xhttp = new XMLHttpRequest(),modo, modoInt;

        xhttp.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) { document.getElementById("direcao_nacele").innerHTML = this.responseText;}};
        xhttp.open("GET", "/arrefecimento", true);
        xhttp.send();
        
    }, 3000);
    */
   
    //NOVO
    const mudaStatusSistema = function (status) {
        var input = document.getElementById("status_sistema"),inputContainsOffline = input.classList.contains("offline"),inputContainsOnline = input.classList.contains("online");
        if (status == 4) {input.value = "OFFLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_offline");mudaEstado("btnAcionaOffline");toggleBtn("btnAcionaOffline");armazenaUltimoModo = 4;
        } else if (status == 3) {input.value = "ONLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_online");mudaEstado("btnAcionaOnline");toggleBtn("btnAcionaOnline");armazenaUltimoModo = 3;
        } else if (status == 2) {input.value = "IDLE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_idle");mudaEstado("btnAcionaIdle");toggleBtn("btnAcionaIdle");armazenaUltimoModo = 2;
        } else if (status == 5) {input.value = "MANUAL";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_manual");mudaEstado("btnAcionaModoManual");toggleBtn("btnAcionaModoManual");habilitaManual();armazenaUltimoModo = 5;
        } else if (status == 1) {input.value = "AUTOSAFE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_autosafe");mudaEstado("btnAutosafe");toggleBtn("btnAutosafe");armazenaUltimoModo = 1;
        } else if (status == 6) {input.value = "P.E.";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");document.getElementById("status_sistema").classList.add("status_sistema_parada_emergencia");mudaEstado("btnParadaEmergencia");toggleBtn("btnParadaEmergencia");armazenaUltimoModo = 6;
        } else if (status == armazenaUltimoModo) { /*input.value = "ONLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");// document.getElementById("status_sistema").classList.add("status_sistema_online")*/;}
    }
    const mudaStatusSistemaCameServer = function (status) {
        var input = document.getElementById("status_sistema"),inputContainsOffline = input.classList.contains("offline"),inputContainsOnline = input.classList.contains("online");
        if (status == 4) {input.value = "OFFLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnAcionaOffline");toggleBtn("btnAcionaOffline");armazenaUltimoModo = 4;
        } else if (status == 3) {input.value = "ONLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnAcionaOnline");toggleBtn("btnAcionaOnline");armazenaUltimoModo = 3;
        } else if (status == 2) {input.value = "IDLE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnAcionaIdle");toggleBtn("btnAcionaIdle");armazenaUltimoModo = 2;
        } else if (status == 5) {input.value = "MANUAL";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnAcionaModoManual");toggleBtn("btnAcionaModoManual");habilitaManual();armazenaUltimoModo = 5;
        } else if (status == 1) {input.value = "AUTOSAFE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnAutosafe");toggleBtn("btnAutosafe");armazenaUltimoModo = 1;
        } else if (status == 6) {input.value = "P.E.";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");mudaEstado("btnParadaEmergencia");toggleBtn("btnParadaEmergencia");armazenaUltimoModo = 6;
        } else if (status == armazenaUltimoModo) { /*input.value = "ONLINE";document.getElementById("status_sistema").classList.remove("status_sistema_offline", "status_sistema_online", "status_sistema_idle", "status_sistema_manual", "status_sistema_autosafe", "status_sistema_parada_emergencia");// document.getElementById("status_sistema").classList.add("status_sistema_online")*/;}
    }
    
    const liberaBloqueaBotoes = function () {
        vetorId = ["btnAcionaModoManual", "btnAcionaOnline", "btnAcionaOffline", "btnAcionaIdle", "btnParadaEmergencia", "btnAutosafe"];
        var contAtivo = 0, contDesativado = 0, divId = "";
        
        for (i = 0; i < vetorId.length; i++) {divId = document.getElementById(vetorId[i]).hasAttribute("disabled")
            if (divId == false) {contAtivo++;
            }else{contDesativado++;}
        }
        if (vetorId.length == contAtivo) {
            for (i = 0; i < contAtivo; i++) {document.getElementById(vetorId[i]).setAttribute("disabled", "disabled");}
            controladorBuscaOperacao = 0;
        } else if (vetorId.length == contDesativado) {
            for (i = 0; i < contDesativado; i++) {document.getElementById(vetorId[i]).removeAttribute("disabled");}
            controladorBuscaOperacao = 1;
        }

       
    }
    const pedeSenha = function () {
        var senha = prompt("Digite a senha para continuar")
        if (senha != null) {
            if (senha == "pluseamelhor") {alert("Deu bom!");mudaEstado(elemento);toggleBtn(elemento);liberaBloqueaBotoes();
            }else {alert("Senha Incorreta!");}
        }
    }
    const alerta = function () {
        var verifica = document.getElementById(elemento).classList.contains("button-on");
        if (verifica == false) {
            var escolha = confirm("Essa operacao pode comprometer o sistema, deseja continuar?")
            if (escolha == true) {pedeSenha();}
        } else {
            mudaEstado(elemento);
            liberaBloqueaBotoes();
            toggleBtn(elemento);
            ativaIntervalo();
            console.log("Deifiniu controlador" + controladorBuscaOperacao);
        }
    }
    const capturaElemento = function (e) {elemento = e;}
    const mudaEstado = function (elemento) {
        var verifica = document.getElementById(elemento).classList.contains("button-on")
        if (verifica == false) {
            document.getElementById(elemento).classList.remove("button-off");
            document.getElementById(elemento).classList.add("button-on");
            document.getElementById(elemento).innerHTML = "ON"
        } else {
            document.getElementById(elemento).classList.remove("button-on");
            document.getElementById(elemento).classList.add("button-off");
            document.getElementById(elemento).innerHTML = "OFF"
        }
    }
    const controlaGrau = function (id) {
        var input = "#" + id + "ManualGraus",inputGrau = document.querySelector(input),valor = inputGrau.value,attrMax = inputGrau.getAttribute("max"),attrMin = inputGrau.getAttribute("min");
        // inputGrau.onkeyup = function(){}
        console.log(inputGrau);console.log(valor);console.log(attrMax);console.log(attrMin);
        //Se for maior que o max altera para o min
        if (valor > attrMax) {inputGrau.value = 360;}
        //Se for menor que o min ele muda para o max
        if (valor < attrMin) {inputGrau.value = 360;}
        //Se for vazio atribui vazio
        if (valor == "") {inputGrau.value = "";}
        //Se for valor certo forca ser certo 
        if ((valor >= 0) && (valor <= 359)) {inputGrau.value = valor;}
    }

    const toggleBtn = function (botao) {
        //variaveis da funcao
        var botao = botao
        var classeAtiva = document.getElementById(botao).classList.contains("button-on")
        var classeDesligada = document.getElementById(botao).classList.contains("button-off")
        var xhr = new XMLHttpRequest();

        //Verifica se ta ativo ou nao
        if (classeAtiva == true) { xhr.open("GET", "/atualizaBtn?output=" + botao + "&state=1", true); console.log("Enviou:" + botao + "| Estado: 1"); }
        else if (classeDesligada == true) { xhr.open("GET", "/atualizaBtn?output=" + botao + "&state=0", true); console.log("Enviou:" + botao + "| Estado: 0"); }
        xhr.send();

    }

    const enviaGrausManual = function (id) {
        var valor = document.getElementById(id).value;
        var input = id;
        var xhr = new XMLHttpRequest();

        var escolha = confirm("Deseja enviar esse grau?");

        if (escolha == true) {
            xhr.open("GET", "/atualizaGrau?value=" + valor + "&input=" + input, true); console.log("Enviou grau:" + valor);
            xhr.send();
        }
        else if (escolha == false) {
            if (id = "pitchManualGraus") {
                mudaEstado("modoManualPitch");
                toggleBtn("modoManualPitch");
            } else if (id = "posicaoManualGraus") {
                mudaEstado("modoManualPosicao");
                toggleBtn("modoManualPosicao");
            }
        }
    }

    const trataDados = function(modo){
        var btn ="";
        verificaUltimoModo(modo);
        if(modo == 1){
            btn = "btnAutosafe";
            enviaModoAtual(btn,modo,1);
        }else if(modo == 2){
            btn = "btnAcionaIdle";
            enviaModoAtual(btn,modo,1);
        }else if(modo == 4){
            btn = "btnAcionaOffline";
            enviaModoAtual(btn,modo,1);
        }else if(modo == 3){
            btn = "btnAcionaOnline";
            enviaModoAtual(btn,modo,1);
        }else if(modo == 5){
            btn = "btnAcionaManual";
            enviaModoAtual(btn,modo,1);
        }else if(modo == 6){
            btn = "btnParadaEmergencia";
            enviaModoAtual(btn,modo,1);
        }
    }

    const verificaUltimoModo = function(modo){
//        if(armazenaUltimoModo == 0){
//            armazenaUltimoModo = 3;
//        }
    }

    const enviaModoAtual = function (btn, modo, comeFromServer) {
        var escolha,
            repetido = 0,
            xhr = new XMLHttpRequest(),
            input = document.getElementById(btn);

        console.log("armazenaUltimoModo:" + armazenaUltimoModo)
       // if ((armazenaUltimoModo == 5) && (modo != 5)) {
        //    habilitaManual()
        //}

        if(comeFromServer == 0){
          console.log("Entrou não server")
            if (input.classList.contains("button-off")) {
                escolha = confirm("Deseja ativar esse modo?");
            } else if (input.classList.contains("button-on")) {
                escolha = confirm("Deseja desativar esse modo?")
            }

            if (escolha == true) {

                if ((modo == armazenaUltimoModo)) {
                    if (modo == 3) {

                    } else {
                        mudaStatusSistema(modo)

                        modo = 3    
                        // xhr.open("GET", "/atualizaModo?value=" + modo, true);
                        // xhr.send();
                        mudaStatusSistema(modo)
                    }
                } else {
                    // xhr.open("GET", "/atualizaModo?value=" + modo, true);
                    // xhr.send();
                    //Verifica se deu certo e já altera o atual estado do sistema

                    mudaStatusSistema(armazenaUltimoModo)
                    mudaStatusSistema(modo)
                    //xhr.addEventListener("load", mudaStatusSistema(modo), false)
                }
            }else if (escolha == false) {}
        }else if(comeFromServer == 1){
          //primeira request
          if(modo == 31){
            modo = 3;
            mudaStatusSistema(modo); 
          }else if ((0 == armazenaUltimoModo)) {
                    if (modo == 3) {
                        
                    } else {
                        mudaStatusSistema(modo)

                        //modo = 3    
                        // xhr.open("GET", "/atualizaModo?value=" + modo, true);
                        // xhr.send();
                        //mudaStatusSistema(modo)
                    }
            }else if(modo == armazenaUltimoModo){
                      
            }else {
                    // xhr.open("GET", "/atualizaModo?value=" + modo, true);
                    // xhr.send();
                    //Verifica se deu certo e já altera o atual estado do sistema

                    mudaStatusSistemaCameServer(armazenaUltimoModo)
                    mudaStatusSistema(modo)
                    //xhr.addEventListener("load", mudaStatusSistema(modo), false)
                }
        }  
    }

    const habilitaManual = function () {
        modosManuais = document.getElementById("modosManuais").classList.contains("hide");

        if (modosManuais == true) {
            document.getElementById("modosManuais").classList.toggle("hide");
            document.getElementById("modosManuais").classList.toggle("show");
        } else if (modosManuais == false) {
            document.getElementById("modosManuais").classList.toggle("show");
            document.getElementById("modosManuais").classList.toggle("hide");
        }
    }

  const mudaCorFreioNacele = function (modo) {
        var modoInt = parseInt(modo),
            id = document.getElementById("status_atual_freio_nacele"),
            contain = document.getElementById("status_atual_freio_nacele").classList.contains("status_atual_freio_nacele_freado"),
            adiciona = document.getElementById("status_atual_freio_nacele").classList.add("status_atual_freio_nacele_freado");

        if (modoInt == 0) {
            if (contain == true) {
                id.value = "Desacoplado";
                document.getElementById("status_atual_freio_nacele").classList.remove("status_atual_freio_nacele_freado")
            }
        } else if (modoInt == 1) {
            if (contain == false) {
                adiciona;
                id.value = "Acoplado"
            } 
        }
    }

    const mudaCorFreioRotor = function (modo) {
        var modoInt = parseInt(modo),
            id = document.getElementById("status_atual_freio_rotor"),
            contain = document.getElementById("status_atual_freio_rotor").classList.contains("status_atual_freio_rotor_freado"),
            adiciona = document.getElementById("status_atual_freio_rotor").classList.add("status_atual_freio_rotor_freado");

        if (modoInt == 0) {
            if (contain == true) {
                id.value = "Desacoplado";
                document.getElementById("status_atual_freio_rotor").classList.remove("status_atual_freio_rotor_freado")
            } 
        } else if (modoInt == 1) {
            if (contain == false) {
                adiciona;
                id.value = "Acoplado"
            } 
        }
    }

const verificaPrimeiraRequisicao = function(modo){
        
        ultimoModo = modo%10
        modo = Math.floor(modo/10)

        console.log(modo);
        console.log(ultimoModo);
        
        if(modo == 1){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 2){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 4){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 3){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 5){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 6){
//            armazenaUltimoModo = modo
            trataDados(modo);
        }else if(modo == 0){
//            armazenaUltimoModo = 3;
            modo = 31
            trataDados(modo);
        }
        console.log("OPERACAO APÓS TRATAR: " + modo)
    }
    
    document.addEventListener("DOMContentLoaded", function () {
        if (document.getElementById("btnLiberaBotoes").classList.contains("button-off") == true) {
            
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () { if (this.readyState == 4 && this.status == 200) { modo = this.responseText; modoInt = parseInt(modo); console.log("ENTROU OPERACAO:" + modoInt); verificaPrimeiraRequisicao(modoInt); } }
            xhttp.open("GET", "/status_modo_operacao", true);
            xhttp.send();

            ativaIntervalo();
            liberaBloqueaBotoes();
            
        }else{
            controladorBuscaOperacao = 1;
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () { if (this.readyState == 4 && this.status == 200) { modo = this.responseText; modoInt = parseInt(modo); console.log("MUDOU OPERACAO:" + modoInt); verificaPrimeiraRequisicao(modoInt); } }
            xhttp.open("GET", "/status_modo_operacao", true);
            xhttp.send();
        }
    });

</script>

</html>)rawliteral";

// Captura e subsitui na variavel o valor do sensor em tempo real
//  Now, we need to create the processor() function, that will replace the placeholders in our HTML text with the actual temperature and humidity values.
//  When the web page is requested, we check if the HTML has any placeholders. If it finds the %TEMPERATURE% placeholder, we return the temperature by calling
//  the readDHTTemperature() function created previously.

//===============================================
// PROCESSADOR DOS PLACEHOLDERS
//Percorre o codigo HTML e troca os placeholders entre "%placeholder%" pelo o que for definido aqui
//===============================================
String processor(const String &var)
{
  // Serial.println(var);

  // MODELO QUE CONTROLA SE O BOTAO ESTA ATIVADO OU NAO E MANTÊM MESMO SE A PÁGINA ATUALIZAR
  // ====================================================

  // PERCORRE OS PLACEHOLDERS
  if (var == "BUTTONPLACEHOLDER")
  {
    // Declara a variavel botao e soma os textoa ela
    String buttons = "";
    buttons += "<div><p class=\"dht-labels\">Velocidade</p><input type=\"text\" name=\"velocidade\" id=\"velocidade\" placeholder=\"30 Km/h\" value="
               " disabled></div>";

    //Veriica se o botao esta ativado ou nao
    if (btnParam2Int == 0)
    {
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-off\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>OFF</button></div>";
    }
    else if (btnParam2Int == 1)
    {
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-on\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>ON</button></div>";
    }
    //retorna o botao
    return buttons;

    // Passa para o proximo placeholder

    // IF DADOS
    // -------------------
  } else if (var == "PERCENT") {
    return "%";

  } else if (var == "RPMROTOR") {
    return readContaTudo();
  } else if (var == "RPMGERADOR") {
    return "Rpm gerador";

  } else if (var == "TEMPEXTERNA") {
    return "Temp externa";
  } else if (var == "TEMPGERADOR") {
    return readTempGerador();

  } else if (var == "TENSAOSISTEMA") {
    return "Tensao sistema";
  } else if (var == "TENSAOSISTEMAPOTENCIA") {
    return "Temp sistema potencia";

  } else if (var == "VELOCIDADE") {
    return "Temp velocidade";
  } else if (var == "DIRECAOATUALMET") {
    return "Direcao atual met";
  } else if (var == "VELOCIDADEMEDIAMET") {
    return "Velocidade media met";
  } else if (var == "DIRECAOFINALMET") {
    return "Direcao final met";

  } else if (var == "POSICAOATUALNACELE") {
    return "Posicao atual nacele";
  } else if (var == "ANGULOATUALPITCH") {
    return "Posicao atual pitch";


  } else if (var == "DIRECAOCARDIALNACELE") {
    return readDirecaoNacele();
  } else if (var == "DIRECAOCARDIALBIRUTA") {
    return readDirecaoBiruta();
    // IF BOTOES
    // ---------------------
  } else if (var == "BOTAOFREIONACELE") {
    String btns = "";

    if (btnFreioNacele == 0) {
      btns = + "<div><p class=\"p-col-title\">Freio Nacele</p><button id=\"freioNacele\" class=\"button button-off\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\");' >OFF</button></div>";
    } else if (btnFreioNacele == 1) {
      btns = + "<div><p class=\"p-col-title\">Freio Nacele</p><button id=\"freioNacele\" class=\"button button-on\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\");'>ON</button></div>";
    }
    return btns;

  } else if (var == "BOTAOFREIOROTOR") {
    String btns = "";

    if (btnFreioRotor == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Freio Rotor </p><button id=\"freioRotor\" onclick='mudaEstado(\"freioRotor\");toggleBtn(\"freioRotor\");' class=\"button button-off\">OFF</button></div>";
    } else if (btnFreioRotor == 1) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Freio Rotor </p><button id=\"freioRotor\" onclick='mudaEstado(\"freioRotor\");toggleBtn(\"freioRotor\");' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAMANUAL") {
    String btns = "";

    if (btnAcionaManual == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Manual</p><button id=\"btnAcionaModoManual\" onclick='enviaModoAtual(\"btnAcionaModoManual\",5,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaManual == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Manual </p><button id=\"btnAcionaModoManual\" onclick='enviaModoAtual(\"btnAcionaModoManual\",5,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAONLINE") {
    String btns = "";

//    if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0)) {
//      btnAcionaOnline = 1;
//    } else {
//      btnAcionaOnline = 0;
//    }

    if (btnAcionaOnline == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Online</p><button id=\"btnAcionaOnline\" onclick='enviaModoAtual(\"btnAcionaOnline\",3,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaOnline == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Online </p><button id=\"btnAcionaOnline\" onclick='enviaModoAtual(\"btnAcionaOnline\",3,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAOFFLINE") {
    String btns = "";

    if (btnAcionaOffline == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Offline</p><button id=\"btnAcionaOffline\" onclick='enviaModoAtual(\"btnAcionaOffline\",4,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaOffline == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Offline </p><button id=\"btnAcionaOffline\" onclick='enviaModoAtual(\"btnAcionaOffline\",4,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAIDLE") {
    String btns = "";

    if (btnAcionaIdle == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Idle</p><button id=\"btnAcionaIdle\" onclick='enviaModoAtual(\"btnAcionaIdle\",2,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaIdle == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Idle</p><button id=\"btnAcionaIdle\" onclick='enviaModoAtual(\"btnAcionaIdle\",2,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAAUTOSAFE") {
    String btns = "";

    if (btnAcionaAutosafe == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Autosafe</p><button id=\"btnAutosafe\" onclick='enviaModoAtual(\"btnAutosafe\",1,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaAutosafe == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Autosafe</p><button id=\"btnAutosafe\" onclick='enviaModoAtual(\"btnAutosafe\",1,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAPARADAEMERGENCIA") {
    String btns = "";

    if (btnAcionaParadaEmergencia == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Parada Emergência</p><button id=\"btnParadaEmergencia\" onclick='enviaModoAtual(\"btnParadaEmergencia\",6,0)' class=\"button button-off\">OFF</button></div>";
    } else if (btnAcionaParadaEmergencia == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Modo Parada Emergência</p><button id=\"btnParadaEmergencia\" onclick='enviaModoAtual(\"btnParadaEmergencia\",6,0)' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOLIBERABOTOES") {
    String btns = "";

    if (btnLiberaBotoes == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Liberar Botoes</p><button id=\"btnLiberaBotoes\" onclick='capturaElemento(\"btnLiberaBotoes\");alerta()' class=\"button button-off\">OFF</button></div>";
    } else if (btnLiberaBotoes == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Liberar Botoes</p><button id=\"btnLiberaBotoes\" onclick='capturaElemento(\"btnLiberaBotoes\");alerta()' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOATUALIZASOFTWARE") {
    String btns = "";

    if (btnAtualizasoftware == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Atualiza Soft.</p><button id=\"btnAtualizasoftware\" onclick='mudaEstado(\"btnAtualizasoftware\");toggleBtn(\"btnAtualizasoftware\");' class=\"button button-off\">OFF</button></div>";
    } else if (btnAtualizasoftware == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Atualiza Soft.</p><button id=\"btnAtualizasoftware\" onclick='mudaEstado(\"btnAtualizasoftware\");toggleBtn(\"btnAtualizasoftware\");' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else if (var == "BOTAOACIONAARREFECIMENTO") {
    String btns = "";

    if (btnArrefecimento == 0) {
      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Arrefecimento</p><button id=\"btnArrefecimento\" onclick='mudaEstado(\"btnArrefecimento\");toggleBtn(\"btnArrefecimento\");' class=\"button button-off\">OFF</button></div>";
    } else if (btnArrefecimento == 1) {
//      btns = + "<div class=\"mt-105\"><p class=\"p-col-title\">Arrefecimento</p><button id=\"btnArrefecimento\" onclick='mudaEstado(\"btnArrefecimento\");toggleBtn(\"btnArrefecimento\");' class=\"button button-on\">ON</button></div>";
    }
    return btns;
  } else {
    return String();
  }
}

//===============================================
// SETUP
//===============================================

void setup()
{
  Wire.begin(); // INICIALIZA O I2C BUS
  bmp.begin(0x76);
  // Called this function to update de cod, it run for 3 minuts, automatically after this the ESP reset,
  // reiniting it return on de updateWifiCode(), wait again 3 minuts and then, if does`t have cod for
  // for update the ESP return to setup and init the webserver and others functions.
  // For update the code is ever necessary restart the ESP ou call de function updateWifiCode(),
  // for get successe in this processe, is recommended restart de ESP, instead of call de function


  // INSTANCIANDO AS VARIÁVEIS
  //-----------------------
  // Initialize the output variables as outputs

  /*  //OUTPUTs

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


    // Abilitação do AttachInterrupt
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
    //brakenacele();
    //braking = 1;
    //freerotor();

    // ATUALIZA O CÓDIGO VIA WIFI
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


    //fins de teste para leitura do potenciøemtro de ref da nacele e pitch
    //while(1){
     // lcd.clear();
     // lcd.setCursor(0, 0);
     // lcd.print(analogRead(GPIOpotnacele));
     // delay(500);
    //}

    settozero();
  */

  // CRIANDO O WEBSERVER
  //-----------------------
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  //******************
  // FAZ AS REQUISICOES
  //******************
  // When we make a request on the root URL, we send the HTML text that is stored on the index_html variable.
  // We also need to pass the processorfunction, that will replace all the placeholders with the right values
  //  Route for root / web page

  // PUXA A PAGINA
  //---------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/html", index_html, processor);
  });
  //---------------------------

  // PUXA OS DADOS EM TEMPO REAL
  //---------------------------
  //modelo
  //server.on("/rpmrotor", HTTP_GET, [](AsyncWebServerRequest *request)
  //          { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });

  server.on("/rpmrotor", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readRPMRotor().c_str());
  });
  server.on("/rpm_gerador", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readRPMGerador().c_str());
  });

  server.on("/velocidade_met", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readWindspeedAt().c_str());
  });
  server.on("/velocidade_media_met", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readWindspeedMF().c_str());
  });
  server.on("/direcao_atual_met", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readWinddirAt().c_str());
  });
  server.on("/direcao_final_met", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readWinddirMF().c_str());
  });

  server.on("/posicao_atual_nacele", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readPosiNacele().c_str());
  });
  server.on("/angulo_atual_pitch", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readPosiPitch().c_str());
  });

  server.on("/temp_externa", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readTempSistema().c_str());
  });
  server.on("/temp_gerador", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readTempGerador().c_str());
  });

  server.on("/tensao_sistema", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readTensaoSistema().c_str());
  });
  server.on("/tensao_sistema_potencia", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readTensaoPotencia().c_str());
  });

  server.on("/direcao_biruta", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readDirecaoBiruta().c_str());
  });
  server.on("/direcao_nacele", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readDirecaoNacele().c_str());
  });



  //MODO DE OPERAÇÃO, FREIO E ARREFECIMENTO
  server.on("/freio_nacele", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readFreioNacele().c_str());
  });
  server.on("/freio_rotor", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readFreioRotor().c_str());
  });

  /*server.on("/status_arrefecimento", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readArrefecimento().c_str()); });*/

  server.on("/status_modo_operacao", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", readModoOperacao().c_str());
  });
  //---------------------------

  // VERIFICA E ATUALIZA O BOTAO QUE FOR ATIVADO
  //---------------------------
  //  Send a GET request to <ESP_IP>/update?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>

  server.on("/atualizaBtn", HTTP_GET, [](AsyncWebServerRequest * request) {
    // GET input1 value on <ESP_IP>/atualizaBtn?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {

      //Captura os valores do request
      btnParam1 = request->getParam(PARAM_INPUT_1)->value();
      btnParam2 = request->getParam(PARAM_INPUT_2)->value();

      btnParam2Int = btnParam2.toInt();

      //FREIOS
      if (btnParam1 == "freioNacele") {
        if (btnParam2Int == 0) {
          btnFreioNacele = 0;
          freenacele();
          Serial.println("FREENACELE");
        } else if (btnParam2Int == 1) {
          btnFreioNacele = 1;
          brakenacele();
          Serial.println("BRAKENACELE");
        }
      } else if (btnParam1 == "freioRotor") {
        if (btnParam2Int == 0) {
          btnFreioRotor = 0;
          freerotor();
          Serial.println("FREEROTOR");
        } else if (btnParam2Int == 1) {
          btnFreioRotor = 1;
          braking = 1;
          Serial.println("BRAKEROTOR");
        }

        //ARREFECIMENTO
      } else if (btnParam1 == "btnArrefecimento") {
        if (btnParam2Int == 0) {
          btnArrefecimento = 0;
          offarref();
          Serial.println("OFF ARREF");
        } else if (btnParam2Int == 1) {
          btnArrefecimento = 1;
          onarref();
          Serial.println("ON ARREF");
        }


        //LIBERA BOTOES
      } else if (btnParam1 == "btnLiberaBotoes") {
        if (btnParam2Int == 0) {
          btnLiberaBotoes = 0;
        } else if (btnParam2Int == 1) {
          btnLiberaBotoes = 1;
        }

        //MODOS
      } else if (btnParam1 == "btnAcionaModoManual") {
        if (btnParam2Int == 0) {
          btnAcionaManual = 0;

          if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0) && (btnAcionaOnline == 0)) {
            operationMode = 3;
            armazenaModo = 3;
          }

          Serial.println("MANUL OFF ONLINE ON");
        } else if (btnParam2Int == 1) {
          btnAcionaManual = 1;
          operationMode = 5;
          armazenaModo = 5;
          Serial.println("MANUAL ON");
        }
      } else if (btnParam1 == "btnAcionaOnline") {
        if (btnParam2Int == 0) {
          btnAcionaOnline = 0;
          Serial.println("ONLINE OFF");
        } else if (btnParam2Int == 1) {
          btnAcionaOnline = 1;
          operationMode = 3;
          armazenaModo = 3;
          Serial.println("ONLINE ON");
        }
      } else if (btnParam1 == "btnAcionaOffline") {
        if (btnParam2Int == 0) {
          btnAcionaOffline = 0;
          if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0) && (btnAcionaOnline == 0) && btnLiberaBotoes == 1) {
            operationMode = 3; //AARRUMAR POR SOB CONDIÇAO DE QUE SOMENTE TODOS OS OUTROS BOTOES FOREM IGUAL A 0 ELE ASSUME O OPERATION MODE = 3
            armazenaModo = 3;
          }
          Serial.println("OFFLINE OFF ONLINE ON");
        } else if (btnParam2Int == 1) {
          btnAcionaOffline = 1;
          operationMode = 4;
          armazenaModo = 4;
          Serial.println("OFFLINE ON");
        }
      } else if (btnParam1 == "btnAcionaIdle") {
        if (btnParam2Int == 0) {
          btnAcionaIdle = 0;
          if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0) && (btnAcionaOnline == 0)) {
            operationMode = 3;
            armazenaModo = 3;
          }
          Serial.println("IDLE OFF ONLINE ON");
        } else if (btnParam2Int == 1) {
          btnAcionaIdle = 1;
          operationMode = 2;
          armazenaModo = 2;
          Serial.println("IDLE ON");
        }
      } else if (btnParam1 == "btnAcionaAutosafe") {
        if (btnParam2Int == 0) {
          btnAcionaAutosafe = 0;
          if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0) && (btnAcionaOnline == 0)) {
            operationMode = 3; 
            armazenaModo = 3;
          }
          Serial.println("AUTOSAFE OFF ONLINE ON");

        } else if (btnParam2Int == 1) {
          btnAcionaAutosafe = 1;
          operationMode = 1;
          armazenaModo = 1;
          Serial.println("AUTOSAFE ON");
        }
      } else if (btnParam1 == "btnParadaEmergencia") {
        if (btnParam2Int == 0) {
          btnAcionaParadaEmergencia = 0;
          if ((btnAcionaOffline == 0) && (btnAcionaIdle == 0) && (btnAcionaManual == 0) && (btnAcionaParadaEmergencia == 0) && (btnAcionaAutosafe == 0) && (btnAcionaOnline == 0)) {
            operationMode = 3; 
            armazenaModo = 3;
          }
          Serial.println("EMERGENCIA OFF ONLINE ON");

        } else if (btnParam2Int == 1) {
          btnAcionaParadaEmergencia = 1;
          operationMode = 6;
          armazenaModo = 6;
          Serial.println("EMERGENCIA ON");
        }
      }


    };
    request->send(200, "text/plain", "OK");
  });

  //AVALIA INPUTS GRAUS

  server.on("/atualizaGrau", HTTP_GET, [](AsyncWebServerRequest * request) {
    // GET input1 value on <ESP_IP>/atualizaBtn?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
    if (request->hasParam(PARAM_INPUT_3) && request->hasParam(PARAM_INPUT_4)) {

      //Captura os valores do request
      valueParam3 = request->getParam(PARAM_INPUT_3)->value();
      inputParam4 = request->getParam(PARAM_INPUT_4)->value();

      valueParam3Int = valueParam3.toInt();

      if (inputParam4 == "pitchManualGraus") {
        pitchReq = valueParam3Int;
      } else if (inputParam4 == "posicaoManualGraus") {
        deg = valueParam3Int;
      }
    }
    request->send(200, "text/plain", "OK");
  });

  //---------------------------


  // INICIA O SERVIDOR
  //---------------------------
  server.begin();

  //---------------------------
}

//===============================================
// LOOP
//===============================================

void loop()
{
  /*  // INICIA AS FUNÇÕES
    //------------------------
    readsensors();

    readminimet();

    operation();

    Display();

    if(automatico == 1 & deg >= 0 & deg < 360){

    //ajuste na posição da nacele
    ajustenacele();

    }
  */
    delay(11000);
    randomizaOperacao();
    delay(9000);
}

//===============================================
// FUNCOES
//===============================================

//FUNÇÃO PARA TESTAR A AUTOMOÇÃO SERVER-PAGINA
void randomizaOperacao() {
  randomico = 1 + ( rand() % 5 );

  if (randomico == 3) {
    operationMode = 3;
  } else if (randomico == 4) {
    operationMode = 4;
  } else if (randomico == 2) {
    operationMode = 2;
  } else if (randomico == 1) {
    operationMode = 1;
  } else if (randomico == 5) {
    operationMode = 5;
  }
}

void reiniciaESP() {
  ESP.restart();
}

//aquisição dos dasos dos sensores
void readsensors() {


  //para a medição de temperatura do sistema
  tempsistema = bmp.readTemperature();
  b = 0;


  //para a medição de temperatura do gerador
  tempgerador = 0;
  while (b <= 500) {
    tempgerador = (analogRead(GPIOreftemp1) * 0.000805664 * 100) + tempgerador + 6;
    b++;
  }
  tempgerador = tempgerador / 500;
  Tgerador = tempgerador;
  b = 0;


  //Para a medição da tensão so sistema de controle
  b = 0;
  tensaoSis = 0;
  while (b <= 20) {
    tensaoSis = (((analogRead(GPIOtSis) * 3.3) / 4096.0) / 0.2) + tensaoSis - 0.3;
    b++;
  }
  tSis = tensaoSis / 20;



  //Para a medição da tensão do sistema de potência
  b = 0;
  tensaoPot = 0;
  while (b <= 20) {
    tensaoPot = (((analogRead(GPIOtPot) * 3.3) / 4096.0) / 0.2) + tensaoPot - 0.3;
    b++;
  }
  tPot = tensaoPot / 20;



  //leitura da posição da nacele
  mediapot();


  //leitura do angulo pitch
  mediapotpitch();


  //Leitura da RPM
  if (millis() - timeold >= 250) {
    //Desabilita interrupcao durante o cálculo
    detachInterrupt(digitalPinToInterrupt(35));
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    rpmRot = rpm / 32;
    timeold = millis();
    pulsos = 0;
    //Reabilitação do AttachInterrupt
    attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);
  }



}


//medição da valor do potenciometro de referência do sistema de posicionamento
void mediapot() {
  i1 = 0;
  potnacele = 0;
  while (i1 < 20)  {
    potnacele = potnacele + (map(analogRead(GPIOpotnacele), 3945, 540, 0, 359));
    i1++;
  }
  medpot = (potnacele / 20);
  i1 = 0;

  //para o cálculo da velocidade de rotação da nacele


  if (posicao = ! medpot) {
    Vangular = (medpot - posicao) * 1000 / (millis() - tempo40);
    posicao = medpot;
    tempo40 = millis();
  }

  if (Vangular < 0) {
    Vangular = Vangular * -1;
  }


  //determinação do ponto cardeal da nacele
  if (medpot >= 354 & medpot < 5) {
    pontoNacele = "Norte";
  }
  if (medpot >= 5 & medpot < 16) {
    pontoNacele = "Norte";
  }
  if (medpot >= 16 & medpot < 27) {
    pontoNacele = "Norte";
  }
  if (medpot >= 27 & medpot < 38) {
    pontoNacele = "Nordeste";
  }
  if (medpot >= 38 & medpot < 50) {
    pontoNacele = "Nordeste";
  }
  if (medpot >= 50 & medpot < 61) {
    pontoNacele = "Nordeste";
  }
  if (medpot >= 61 & medpot < 72) {
    pontoNacele = "Nordeste";
  }
  if (medpot >= 72 & medpot < 83) {
    pontoNacele = "Leste";
  }
  if (medpot >= 83 & medpot < 95) {
    pontoNacele = "Leste";
  }
  if (medpot >= 95 & medpot < 106) {
    pontoNacele = "Leste";
  }
  if (medpot >= 106 & medpot < 117) {
    pontoNacele = "Leste";
  }
  if (medpot >= 117 & medpot < 128) {
    pontoNacele = "Sudeste";
  }
  if (medpot >= 128 & medpot < 140) {
    pontoNacele = "Sudeste";
  }
  if (medpot >= 140 & medpot < 151) {
    pontoNacele = "Sudeste";
  }
  if (medpot >= 151 & medpot < 162) {
    pontoNacele = "Sudeste";
  }
  if (medpot >= 162 & medpot < 173) {
    pontoNacele = "Sul";
  }
  if (medpot >= 173 & medpot < 185) {
    pontoNacele = "Sul";
  }
  if (medpot >= 185 & medpot < 196) {
    pontoNacele = "Sul";
  }
  if (medpot >= 196 & medpot < 207) {
    pontoNacele = "Sul";
  }
  if (medpot >= 207 & medpot < 218) {
    pontoNacele = "Sudoeste";
  }
  if (medpot >= 218 & medpot < 230) {
    pontoNacele = "Sudoeste";
  }
  if (medpot >= 230 & medpot < 241) {
    pontoNacele = "Sudoeste";
  }
  if (medpot >= 241 & medpot < 252) {
    pontoNacele = "Sudoeste";
  }
  if (medpot > 252 & medpot < 263) {
    pontoNacele = "Oeste";
  }
  if (medpot >= 263 & medpot < 275) {
    pontoNacele = "Oeste";
  }
  if (medpot >= 275 & medpot < 286) {
    pontoNacele = "Oeste";
  }
  if (medpot >= 286 & medpot < 297) {
    pontoNacele = "Oeste";
  }
  if (medpot >= 297 & medpot < 308) {
    pontoNacele = "Oeste";
  }
  if (medpot >= 308 & medpot < 320) {
    pontoNacele = "Noroeste";
  }
  if (medpot >= 320 & medpot < 331) {
    pontoNacele = "Noroeste";
  }
  if (medpot >= 331 & medpot < 342) {
    pontoNacele = "Noroeste";
  }
  if (medpot >= 342 & medpot < 354) {
    pontoNacele = "Norte";
  }


}


//medição da valor do potenciometro de referência do sistema de pitch
void mediapotpitch() {
  i1 = 0;
  potpitch = 0;
  while (i1 < 20)  {
    potpitch = potpitch + (map(analogRead(GPIOpotpitch), 110, 3950, 45, 0));
    i1++;
  }
  medpotpitch = (potpitch / 20);
  i1 = 0;
}


//Função que Incrementa o contador para a RPM
void addcount() {
  pulsos++;
}





//Função de leitura dos dados via I2C da minimet
void readminimet() {

  if ((millis() - time7) >= 2000) {


    outData = 1;
    while (outData <= 4) {
      //Serial.print("Dados enviados ao slave==> ");

      Wire.beginTransmission(8); //transmitir dados para o slave com endereço #8
      Wire.write("Request");     //envia uma string
      Wire.write(outData);       //enviar uma variável numérica tipo byte
      Wire.endTransmission(8);    //sinaliza o fim da trasnmissão de dados para o slave
      delay(50);
      //Serial.print(outData);

      Wire.requestFrom(8, 6);  //requisitando resposta do slave
      inString = "";
      while (Wire.available()) {   //loop para receber todos os dados do slave #8
        char inChar = Wire.read();    //recebe cada byte como caractere
        //if(isAlphaNumeric(inChar)){   //se caractere recebido do slave for alphanumerico concatena string
        inString += inChar;         //concatenar o caracter receido
        //}
        //Serial.print("noWhilw");
      }

      //Distribuição dos valores recebidos para as suas respectivas variáveis
      if (outData == 1) {
        windspeed = inString.toFloat() / 2;
      }


      if (outData == 3) {
        if (inString.toFloat() >= 0 & inString.toFloat() < 360) {
          //winddirMF = inString.toFloat();
        }
      }


      if (outData == 2) {
        if (inString.toFloat() >= 0 & inString.toFloat() < 360) {
          winddir = inString.toFloat();

          dir = medpot + winddir;

          if (dir > 360) {
            dir = dir - 360;
          }



        }

        if (dir >= 354 & dir < 5) {
          winddir = 0;
          ponto = "Norte";
        }
        if (dir >= 5 & dir < 16) {
          winddir = 11.25;
          ponto = "Norte";
        }
        if (dir >= 16 & dir < 27) {
          winddir = 11.25 * 2;
          ponto = "Norte";
        }
        if (dir >= 27 & dir < 38) {
          winddir = 11.25 * 3;
          ponto = "Nordeste";
        }
        if (dir >= 38 & dir < 50) {
          winddir = 11.25 * 4;
          ponto = "Nordeste";
        }
        if (dir >= 50 & dir < 61) {
          winddir = 11.25 * 5;
          ponto = "Nordeste";
        }
        if (dir >= 61 & dir < 72) {
          winddir = 11.25 * 6;
          ponto = "Nordeste";
        }
        if (dir >= 72 & dir < 83) {
          winddir = 11.25 * 7;
          ponto = "Leste";
        }
        if (dir >= 83 & dir < 95) {
          winddir = 11.25 * 8;
          ponto = "Leste";
        }
        if (dir >= 95 & dir < 106) {
          winddir = 11.25 * 9;
          ponto = "Leste";
        }
        if (dir >= 106 & dir < 117) {
          winddir = 11.25 * 10;
          ponto = "Leste";
        }
        if (dir >= 117 & dir < 128) {
          winddir = 11.25 * 11;
          ponto = "Sudeste";
        }
        if (dir >= 128 & dir < 140) {
          winddir = 11.25 * 12;
          ponto = "Sudeste";
        }
        if (dir >= 140 & dir < 151) {
          winddir = 11.25 * 13;
          ponto = "Sudeste";
        }
        if (dir >= 151 & dir < 162) {
          winddir = 11.25 * 14;
          ponto = "Sudeste";
        }
        if (dir >= 162 & dir < 173) {
          winddir = 11.25 * 15;
          ponto = "Sul";
        }
        if (dir >= 173 & dir < 185) {
          winddir = 11.25 * 16;
          ponto = "Sul";
        }
        if (dir >= 185 & dir < 196) {
          winddir = 11.25 * 17;
          ponto = "Sul";
        }
        if (dir >= 196 & dir < 207) {
          winddir = 11.25 * 18;
          ponto = "Sul";
        }
        if (dir >= 207 & dir < 218) {
          winddir = 11.25 * 19;
          ponto = "Sudoeste";
        }
        if (dir >= 218 & dir < 230) {
          winddir = 11.25 * 20;
          ponto = "Sudoeste";
        }
        if (dir >= 230 & dir < 241) {
          winddir = 11.25 * 21;
          ponto = "Sudoeste";
        }
        if (dir >= 241 & dir < 252) {
          winddir = 11.25 * 22;
          ponto = "Sudoeste";
        }
        if (dir > 252 & dir < 263) {
          winddir = 11.25 * 23;
          ponto = "Oeste";
        }
        if (dir >= 263 & dir < 275) {
          winddir = 11.25 * 24;
          ponto = "Oeste";
        }
        if (dir >= 275 & dir < 286) {
          winddir = 11.25 * 25;
          ponto = "Oeste";
        }
        if (dir >= 286 & dir < 297) {
          winddir = 11.25 * 26;
          ponto = "Oeste";
        }
        if (dir >= 297 & dir < 308) {
          winddir = 11.25 * 27;
          ponto = "Oeste";
        }
        if (dir >= 308 & dir < 320) {
          winddir = 11.25 * 28;
          ponto = "Noroeste";
        }
        if (dir >= 320 & dir < 331) {
          winddir = 11.25 * 29;
          ponto = "Noroeste";
        }
        if (dir >= 331 & dir < 342) {
          winddir = 11.25 * 30;
          ponto = "Noroeste";
        }
        if (dir >= 342 & dir < 354) {
          dir = 11.25 * 31;
          ponto = "Norte";
        }


      }


      outData++;
    }


    //fim da comunicação I2C como a meteo

    time7 = millis();
  }


  //análise da direção de maior ocorrencia
  if ((millis() - tempo32) >= 240000) {

    pass = 0;
    while (pass <= 32) {
      ocorr[pass] = 0;
      classespeeds[pass] = 0.0;
      pass++;
      //Serial.println("travou 7 ");
    }
    tempo32 = millis();
  }


  analizwind();

}



//aqui toman-se as decisøes com base nos sensores e operação
void operation() {

  //Elevação da RPM por angulo de pitch
  if (rpm < rpmmax1 & autopitch == 1 & operationMode == 3) {
    if (pitchReq < 45 & (millis() - tempo11) > 500) {
      pitchReq = pitchReq + 1;
      tempo11 = millis();
    }

    //direcionamento para a melhor velocidade de vento
    if (operationMode != 4 & operationMode != 6 & velposimf >= cutIn) {
      deg = winddirMF;
      //deg = 180;
    }

  }

  if (operationMode != 4) {
    //ativação automática do arrefecimento
    if (tempgerador >= temparref & a == 0) {
      onarref();
      a = 1;
    }
    if (tempgerador <= 40.0 & a == 1) {
      offarref();
      a = 0;
    }
  }


  //liberação do rotor
  if (operationMode != 1 & operationMode != 4 & operationMode != 6 & brake == 1 & a1 == 0 & velposimf >= cutIn & windspeed <= cutOff2) {
    freerotor();
  }

  //aviso de rpm para a conexão do gerador com a carga
  if (rpm >= rpmcarga & operationMode == 3) {
    digitalWrite(sinalRPM, HIGH);
  }
  if (rpm < rpmcarga & operationMode != 5) {
    digitalWrite(sinalRPM, LOW);
  }

  //desativação do sisetema
  if (operationMode == 4) {
    pitchReq = 0;
    ajustepitch();

    if (brake == 0 & rpm < rpmBrake) {
      braking = 1;
    }
  }

  //bloqueio da necele em offline
  if (operationMode == 4 & a10 == 0) {
    brakenacele();
  }




  //retomada de operação após normalização de parâmetros
  if ((millis() - tempo16) >= temporet & rpm < 50 & velposimf < cutOff2 & tSis >= 11.0 & tPot >= 11.5 & tempgerador <= 40.0 & tempsistema <= 60.0) {
    a3 = 0; //permite novamente a entrada no bloqueio do rotor em autosafe e baixa velocidade
    a5 = 0; //impede a entrada rechamada do autosafe
    a7 = 0; //entrada no modo online
    operationMode = 3; //online
  }



  //*************************************************************************************************
  //Rotinas de segurança do sistema


  //Redução do angulo de pitch em altas rotações
  if (rpm > rpmmax1 & millis() - tempo10 > 5000 & operationMode != 6 & operationMode != 5) {

    if (pitchReq >= 5) {
      pitchReq = pitchReq - 5;

    }
    if (pitchReq < 5 & pitchReq >= 1) {
      pitchReq = pitchReq - 1;

    }
    tempo10 = millis();

  }


  //Auto-Safe por alta rpm - rpmmax2
  if (rpm >= rpmmax2) {
    operationMode = 1;
    tempo16 = millis();
  }

  //Auto-Safe por cutOff de vento
  if (windspeed >= cutOff) {
    operationMode = 1;
    tempo16 = millis();
  }

  //Auto-Safe por alta temperaura do gerador
  if (tempgerador >= 55.0) {
    operationMode = 1;
  }

  //Auto-Safe por alta temperaura do sistema
  if (tempsistema >= 65.0) {
    operationMode = 1;
  }

  //Auto-Safe por baixa tensão no sistema de controle
  if (tSis <= 10.0) {
    operationMode = 1;
  }

  //Auto-Safe por baixa tensão no sistema de potência
  if (tPot <= 8.0) {
    operationMode = 1;
  }


  //Freagem do sistema após redução da rpm
  if (rpm <= rpmBrake & a3 == 0 & operationMode == 1) {
    braking = 1;
    a3 = 1;
  }


  //bloqueio automático do rotor com base em alta rpm - condição crítica - para entrar nesta função o pitch deve ter falhado.
  if (rpm >= rpmmax3 & brake == 0) {
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
  if (braking == 1) {
    brakerotor();
  }



  //rotina de definição do status do sistema

  //Modo Auto-safe
  if (operationMode == 1 & a5 == 0) {
    AutoSafeMode();
  }

  //Modo IDLE
  if (operationMode == 2 & a6 == 0) {
    IdleMode();
  }

  //Modo Online
  if (operationMode == 3 & a7 == 0) {
    onlineMode();
  }

  //Modo Offline
  if (operationMode == 4 & a8 == 0) {
    offlineMode();
  }

  //Modo Manual
  if (operationMode == 5 & a9 == 0) {
    manualMode();
  }

  //modo parada de emergencia
  if (operationMode == 6 & a11 == 0) {
    emergenciMode();
  }


  if (autopitch == 1) {

    //Ajuste no sistema de pitch
    ajustepitch();

  }

  //para o ajuste manual
  if (operationMode == 5) {
    ajustepitch();
    //deg = winddirMF; //colocar o valor lido na serial
    ajustenacele();
  }


}//fim do operation()




void ajustepitch() {

  mediapotpitch();

  //ajuste no sentido horário
  if (medpotpitch > pitchReq & tent3 < 2) {

    //aumenta a pwm
    if ((millis() - tempo9) >= (pwmpitch / 50) & pwmpitch < 255) {
      pwmpitch++;
      tempo9 = millis();
    }


    ledcWrite(0, pwmpitch);
  }


  //ajuste no sentido Anti-horário
  if (medpotpitch < pitchReq & tent3 < 2) {

    //aumenta a pwm
    if ((millis() - tempo9) >= (pwmpitch / 50) & pwmpitch < 255) {
      pwmpitch++;
      tempo9 = millis();
    }


    ledcWrite(1, pwmpitch);
  }

  if (medpotpitch == pitchReq) {

    pwmpitch = 0;
    ledcWrite(1, pwmpitch);
    ledcWrite(0, pwmpitch);

    if (tent3 < 11) {
      tent3++;
    }

  }
  if (medpotpitch != pitchReq & tent3 > 0) {
    tent3 = 0;
  }




}



//entrada dos modos de operação do sistema

//Modo Auto-Safe - aqui é feito o rastreamento constante do vento com rotor bloqueado, e pitch em 0, até que as condições de normalidade sejam reestabelecidas
void AutoSafeMode() {
  //1-levar o amgulo de pitch para 0
  pitchReq = 0;
  //2-alinhar a nacele com a origem do vento ou mantê-la nesta posição
  deg = winddirMF;
  //*as ações 1 e 2 devem ser executadas simultaneamente
  //3-bloqueia rotação da nacele - esta função é executada automaticamente no final do posicionamento.

  autoposi = 1;

  Status = "Auto-Safe";

  a5 = 1;
}

//Modo Idle - aqui é feito o constante rastreamento da melhor velocidade do vento, mas o pitch se mantem em 0, o sistema está pronto para operar em qualquer intante
void IdleMode() {
  pitchReq = 0;
  autoposi = 1;
  autopitch = 0;

  Status = "Idle";

  a6 = 1;
}

//Modo Online - aqui a torre eólica opera normalmente, rodando todas as funcões simultanemanete
void onlineMode() {
  autoposi = 1;
  autopitch = 1;

  Status = "Online";

  a7 = 1;
}

//Modo Offline - aqui a torre eólica se mantém completamente desativada, apena as rotinas de segurança pode rodar, é importnate ressaltar que a melhor direcão de vento nao é rastreada aqui,
void offlineMode() {
  automatico = 0;
  autopitch = 0;
  autoposi = 0;

  Status = "Offline";

  a8 = 1;
}

//Modo Manual - aqui a torre eólica so pode ter sua condição física alterada manualmente via terminal apenas as rotinas de segurança se mantem acessiveis automáticamente
void manualMode() {
  automatico = 1;
  autopitch = 0;
  autoposi = 0;

  Status = "Manual";

  a9 = 1;
}

//Modo Parada de emegência - aqui todos o sistemas da torre são desativados, ou seja, todas as portas do ESP32 são desativadas em termos de ativação e ela permanece em um loop constante
void emergenciMode() {
  automatico = 0;
  autopitch = 0;
  autoposi = 0;

  Status = "Emergencia";
  a11 = 1;

  while (a11 == 1) {
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
void onarref() {
  Serial.println("Arrefecimento ativado");
  arrefState = "on";
  digitalWrite(arref, LOW);
}

//Desativação do sistema de Arrefecimento geral
void offarref() {
  Serial.println("Arrefecimento desativado");
  arrefState = "off";
  digitalWrite(arref, HIGH);
}

//function to brake the nacele rotation
void brakenacele() {
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
void freenacele() {
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
void brakerotor() {


  if ((millis() - tempo3) >= (800 / (pwmbrake + 1)) & pwmbrake < 255) {
    pwmbrake++;
    tempo3 = millis();
    rtState = "locking";
  }

  if (pwmbrake == 255 & (millis() - tempo3) >= 200) {
    ledcWrite(2, 0);
    braking = 0;
    brake = 1;
    rtState = "locked";
    pwmbrake = 0;
    a1 = 0;
  }
  ledcWrite(2, pwmbrake);


}

//function to free the rotor rotation
void freerotor() {

  ledcWrite(3, 255);
  delay(500);
  ledcWrite(3, 0);
  delay(100);
  ledcWrite(3, 255);
  delay(200);
  ledcWrite(3, 0);
  a1 = 1;
  brake = 0;
  rtState = "liberate";


}


//botões do terminal


void ajustenacele() {
  //Def de ajuste normal -- ajusta o valor de intervalo para não ficar negativo
  mediapot();
  if (deg > medpot) {
    inter = deg - medpot;
  }
  if (deg < medpot) {
    inter = medpot - deg;
  }

  //Renovação do tent e tent1
  if (deg != olddeg) {
    tent = 0;
    tent1 = 0;
  }

  //quando não é um ajuste de precisão -> inter > 10
  if (deg != medpot & tent <= 1 & inter > 4) {


    mediapot();
    posi = medpot;//guarda o valor para fins de fer. no aumento da pwm
    olddeg = deg;
    tent++; // permite 2 ajustes de posição
    valpwm = 30;
    ultimointer = 0;

    //ajusta o valor de intervalo para não ficar negativo
    if (deg > medpot) {
      inter = deg - medpot;
    }
    if (deg < medpot) {
      inter = medpot - deg;
    }

    if (a10 == 1 & inter < 10) {
      freenacele();
    }

    tempo14 = millis();
    //inicia a rotação o aumento de pwm no sentido Anti-horário
    while (olddeg > medpot) {

      if (a10 == 1 & valpwm > 38) {
        freenacele();
      }
      Serial.println("          entrou no while horario");

      mediapot();//atualiza os valores de posição

      //ajuste do valor da pwm - aceleração
      if ((medpot - posi) < (inter * 0.90)  & (millis() - tempo1) > 30) {
        tempo1 = millis();
        if (valpwm < 210) {
          valpwm++;
        }
      }

      //ajuste do valor da pwm - desacelaração
      if ((medpot - posi) > (inter * 0.10) & (millis() - tempo1) >= 1) {
        tempo1 = millis();
        if (valpwm > 90) {
          valpwm = valpwm - 3;
        }
      }


      ledcWrite(4, valpwm);//envia os dados para a pontencia


      //anula o loop se passar de 4 min
      if ((millis() - tempo14) > 80000) {
        ledcWrite(4, 0);
        ledcWrite(5, 0);
        tent = 10;
        tent1 = 10;
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

      Display();

    }
    ledcWrite(4, 0);
    ledcWrite(5, 0);


    mediapot();//atualiza os valores de posição


    tempo14 = millis();
    //inicia a rotação e aumento de pwm no sentido Horário
    while (olddeg < medpot) {

      if (a10 == 1 & valpwm > 38) {
        freenacele();
      }
      Serial.println("         entrou no while antihorario");


      mediapot();//atualiza os valores de posição

      //ajuste do valor da pwm - aceleração
      if ((posi - medpot) < (inter * 0.90)  & (millis() - tempo1) > 30) {
        tempo1 = millis();
        if (valpwm < 255) {
          valpwm++;
        }
      }

      //ajuste do valor da pwm - desacelaração
      if ((posi - medpot) > (inter * 0.10) & (millis() - tempo1) >= 1) {
        if (valpwm > 90) {
          valpwm = valpwm - 3;
        }
      }


      ledcWrite(5, valpwm);//envia os dados para a pontencia


      //anula o loop se passar de 2.5 min
      if ((millis() - tempo14) > 80000) {
        ledcWrite(4, 0);
        ledcWrite(5, 0);
        tent = 10;
        tent1 = 10;
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

      Display();


    }
    ledcWrite(4, 0);
    ledcWrite(5, 0);

  }//fim do ajuste normal

  if (a10 == 0 & (tent1 > 0 | deg == medpot)) {
    brakenacele();
  }

  if (valpwm == 0 & a10 == 0) {
    brakenacele();
  }


}

//para a análise de vento
void analizwind() {

  if ((millis() - tempo30) >= 5000) {
    //distribuis as variáveis nos vetores
    winddirs[numposi] = winddir;
    windspeeds[numposi] = windspeed;
    //reinicio do ajuste de variáveis
    if (numposi == 49) {
      numposi = 0;
    }
    else {
      numposi++;
    }
    //Serial.println("renonvar................................................................................... ");

    tempo30 = millis();

  }

  pass = 0;
  pass1 = 0;


  //análise dos dados contidos nos vetores
  if ((millis() - tempo31) > 25000) {


    //contagem de ocorrências e tirando as médias de velocidade para cada posição.
    pass = 0;
    pass1 = 0;
    while (pass <= 31) {

      ocorr[pass] = 0; //zera o vetor na posição pass, para não somar com o anterior
      while (pass1 <= 49) { //aqui etava numposi no lugar de 49
        if (classes[pass] == winddirs[pass1]) {
          // Serial.print("vetor windspeeds/;");
          //Serial.println(windspeeds[pass1]);
          if (ocorr[pass] == 0) {
            classespeeds[pass] = 0;
          }
          classespeeds[pass] = windspeeds[pass1] + classespeeds[pass];
          ocorr[pass]++;
        }
        pass1++;
      }
      if (ocorr[pass] != 0) {
        ///Serial.print("vetor classesspeeds/;");
        //Serial.println(classespeeds[pass]);

        classespeeds[pass] = classespeeds[pass] / ocorr[pass];

      }

      //Serial.print("Classe: ");
      //Serial.println(classes[pass]);
      //Serial.print("média de velocidade da classe: ");
      //Serial.println(classespeeds[pass]);
      //Serial.print("ocorrencias: ");
      //Serial.println(ocorr[pass]);


      pass++;
      pass1 = 0;
    }
    pass = 0;
    pass1 = 0;


    //verificação da maior ocorrência e se é cut-in
    maiorocorr = 0;
    while (pass <= 31) { //verifica qual o mais recorrente
      //Serial.print("entrou no ultimo while -> velocidade: ");
      //Serial.println(classespeeds[pass]);
      if (ocorr[pass] > maiorocorr & classespeeds[pass] >= cutin) {
        maiorocorr = ocorr[pass];
        posimf = classes[pass];
        velposimf = classespeeds[pass];
      }
      pass++;
    }
    //deg = posimf;
    winddirMF = posimf;

    pass1 = 0;
    pass = 0;

    tempo31 = millis();
  }

}//fim da função analzwind


void settozero() {
  //zerando os vetores
  while (pass <= 49) {
    winddirs[pass] = 1;
    pass++;
    //Serial.println("travou 6 :  ");
    //Serial.print(pass);
  }
  pass = 0;
  while (pass <= 31) {
    ocorr[pass] = 0;
    classespeeds[pass] = 0.0;
    pass++;
    ///Serial.println("travou 7 ");
  }
  pass = 0;

  while (pass <= 49) {
    windspeeds[pass] = 0;
    pass++;
    //Serial.println("travou 8 ");
  }
  pass = 0;
  //fim do zeramento dos vetores

  pass = 0;
  pass1 = 0;
  numposi = 0;
}



void Display() {
  //print 1 no display
  if ((millis() - time2) >= 1500 & b1 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAZZARUS_Eolica V2.1");

    if (medpotpitch != pitchReq) {
      lcd.setCursor(0, 1);
      lcd.print("Pitch At:");
      lcd.print(medpotpitch);
      lcd.print(" ");

      lcd.setCursor(11, 1);
      lcd.print("Req:");
      lcd.setCursor(11, 1);
      lcd.print("      ");
      lcd.print(pitchReq);

    } else { //se o pitch nao está sendo usado printa os dados de temperatura
      lcd.setCursor(0, 1);
      lcd.print("TGer:");
      lcd.print(tempgerador, 1);
      lcd.print("C");

      lcd.setCursor(11, 1);
      lcd.print("Ts:");
      lcd.print(tempsistema, 2);
      lcd.print("C");
    }

    lcd.setCursor(11, 2);
    lcd.print("ts:");
    lcd.print(tSis, 1);
    lcd.print("V");

    lcd.setCursor(0, 2);
    lcd.print("tPot:");
    lcd.print(tPot, 1);
    lcd.print("V");


    lcd.setCursor(0, 3);
    lcd.print("U");
    lcd.setCursor(1, 3);
    lcd.print(windspeed, 1);
    lcd.setCursor(6, 3);
    lcd.print("D");
    lcd.setCursor(7, 3);
    lcd.print(winddir, 1);
    lcd.setCursor(13, 3);
    lcd.print("MF");
    lcd.setCursor(15, 3);
    lcd.print(winddirMF, 1);

    b1 = 0;
    time2 = millis();
  }

  //print 2 no display
  if ((millis() - time2) >= 1500 & b1 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAZZARUS_Eolica V2.1");

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
    b1 = 1;
  }

}


//aqui serão printadas no dataloguer todas as decisões tomadas pelo sistema
void datalog() {

}


//******************
// ATUALIZACAO VIA WIFI DO CODIGO
//******************
// função que permite atulizar o código via wifi
void updateWifiCode()
{

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
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
  .onStart([]()
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]()
  {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error)
  {
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
  while ((millis() - time2) < 120000)
  {

    ArduinoOTA.handle();
  }
  Serial.println("Tempo de espera exedido");
  Serial.println("Inicializando servidor");
}
