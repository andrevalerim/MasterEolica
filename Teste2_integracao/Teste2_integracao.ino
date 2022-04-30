/*********
  André Valerim de Freitas based in Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

//===============================================
// BIBLIOTECAS
//===============================================

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
const char *ssid = "Consultorio";
const char *password = "terapiaCris";

// const char *ssid = "Quantum Team";
// const char *password = "amotolia_oleosa";

// const char *ssid = "Andre Wifi";
// const char *password = "gostoso1";

//===============================================
// PINOS
//===============================================

// Definindo os pinos conectados
#define arref 2          // arrefecimento do sistem
#define ncBrake 4        // bloqueio do movimento da nacele
#define ncFree 15        // liberação do movimento da nacele
#define GPIOreftemp1 34  // leitura do sensor de temperetura interna do gerador
#define GPIOreftemp2 13  // leitura do sensor de temperatura do sistema (ESP32)
#define GPIOtSis 33      // leitura da tensão do sistema de controle
#define GPIOtPot 32      // leitura da tensão do sistema de pontência
#define GPIOpotpitch 36  // leitura da tensão do sistema de controle
#define GPIOpotnacele 39 // leitura da tensão do sistema de pontência
#define sinlight 15      // luz de sinalização
#define rtFree 8         // Solta freio rotor
#define rtBrake 12       // Aciona freio rotor

#define pwmH 5 // pino que manda o sinal da pwm para o sentido Horário
#define pwmA 6 // pino que manda o sinal da pwm no sentido Anti-horário

#define pot 7 // pino que o potenciômetro de referência está conectado

// Variaveis de saida dos pinos do jeito antigo de mostrar o HTML
const int output26 = 26;
const int output27 = 27;

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

// Variaveis para comunicacao I2C
byte outData = 1;
float inByte = 0;
String inString = ""; // string para armazenar bytes enviados pelo slave

// Variáveis da mediçnao de RPM
int rpm = 0;
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int pulsos_por_volta = 3; // furos do disco

// variáveis sensores
float tempgerador = 0;
float tempsistema = 0;
float tSis = 0;
float tPot = 0;
int a = 0; // verificação de entrada
int b = 0; // contador
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int pwmbrake = 0;
int pitchAt = 0; // quarda o valor atual do pitch
int posiAt = 0;  // guarda o valor atual da posição da nacele

// variáveis par as rotinas de posicionamento na nacele
// para o motor de rotação na nacele
int valpwm = 0;           // variável que armazena o valor da pwm.
int inter = 0;            // intervalo entre deg e posi
unsigned long tempo1 = 0; // tempo para o ajuste de potência do motor
unsigned long tempo3 = 0; // tempo para printar a posição na hora do ajuste

// para o referência da nacele
int valpot = 0; // armazena o valor lido do pot
int potmap = 0; // armazena o valor do potênciômetro mapeado em graus
int i = 0;

// variáveis gerais. vide referência
int posi = 0;   // posição lida antes de iniciar o movimento
int cont = 0;   // valor de deg - posi = quanto andar e para que sentido
int ref = 0;    // valor (alto ou baixo) do switch refposi
int deg = 0;    // posição atual do vento - variável winddir do master e met.
int olddeg = 0; // guarda o último valor de deg
int tent = 0;   // tentativas de correção de posição
int tent1 = 0;  // tentativas de correção de posição com precisão
int i1 = 0;
int medpot = 0;  // media do potenciomentro 200 medições
int tempo14 = 0; // tempo máximo para anular o loop de giro da nacele
int brake = 0;

// variaveis para leitura do vento
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
long int time2 = 0; // time count of wait in update code
long int teme3 = 0; // tempo para brake rotor
long int time5 = 0; // tempo para atualização da interface
long int time6 = 0; // luz de sinalizaçnao
long int time7 = 0; // leitura minimet

// provaveis variaveis antigas da outra forma de mostrar o html
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String arrefState = "off";
String ncState = "liberate";
String rtState = "liberate";
String Tgerador = "";
String ponto = "";

// Led placa da própria placa LED DE TESTE (PODE SER TIRADO DEPOIS)
int LED_BUILTIN = 2;

//-------------------------------
// Variaveis para o WebServer
//-------------------------------

// Variaveis de contagem para teste das funcoes
// int cont = 0;
int cont1 = 1;

// Captura dos parametros
String btnParam1; // Nome do botao
String btnParam2; // Estado do botao

// Receber o estado dos botoes
int btnParam2Int = 0;

// Controle webpage
const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

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
String readDHTTemperature()
{
  cont++;
  Serial.println(cont);
  return String(cont);
}

// Contadores de Teste
String readDHTVelocidade()
{
  cont1++;
  Serial.println(cont1);
  return String(cont1);
}

// OLHAR ESSE SITE A ADAPTAR O CODIGO
// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/

//===============================================
// PAGINA WEB
//===============================================

// Configurando a pagina web
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .button {
            color: white;
            padding: 0.5rem 1rem;
            text-decoration: none;
            font-size: 1.5rem;
            margin: 0.1rem;
            cursor: pointer;
            text-align: center;
            border: #969595 solid 3px;
        }

        .button:hover {
            background-color: #4CAF50;
            border: #dfdddd solid 3px;
        }

        .button-off {
            background-color: #afafaf;
        }

        .button-on {
            background-color: #348137;
            border: #dfdddd solid 3px;
        }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <span class="dht-labels">Temperature</span>
    <!--%TEMPEREATURE% -> é um placeholder que é subtituido ao processar o codigo-->
    <span id="temperature">%TEMPERATURE%</span>
    <!--Acompanha o <p> principal-->
    <sup class="units">&deg;C</sup>
  </p>
  <div>
    %BUTTONPLACEHOLDER%
  </div>
</body>
<script>

//Declarando as variaveis
var elemento = ""

//Faz as requisiçoes
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 500 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("velocidade").value = this.responseText + " Km/h";
    }
  };
  xhttp.open("GET", "/velocidade", true);
  xhttp.send();
}, 500 ) ;

//Declarando as funcoes criadas (Declarar as funcoes usando - const = *nome funcao* function(){} - pq ele nao reconhce o function)

 //funcao nova
 const toggleBtn = function(botao){
  //variaveis da funcao
  var botao = botao
  var classeAtiva = document.getElementById(botao).classList.contains("button-on")
  var classeDesligada = document.getElementById(botao).classList.contains("button-off")
  var xhr = new XMLHttpRequest();

  //Verifica se ta ativo ou nao
  if(classeAtiva == true)
  { xhr.open("GET", "/atualizaBtn?output="+botao+"&state=1", true); }
  else if (classeDesligada == true){ xhr.open("GET", "/atualizaBtn?output="+botao+"&state=0", true); }
  xhr.send();
 }


  const mudaEstado = function(elemento) {
        var verifica = document.getElementById(elemento).classList.contains("button-on")

        if (verifica == false) {
            document.getElementById(elemento).classList.toggle("button-off");
            document.getElementById(elemento).classList.toggle("button-on");
            document.getElementById(elemento).innerHTML = "ON"
        } else {
            document.getElementById(elemento).classList.toggle("button-on");
            document.getElementById(elemento).classList.toggle("button-off");
            document.getElementById(elemento).innerHTML = "OFF"
        }
    }
  
</script>
</html>)rawliteral";

// Captura e subsitui na variavel o valor do sensor em tempo real
//  Now, we need to create the processor() function, that will replace the placeholders in our HTML text with the actual temperature and humidity values.
//  When the web page is requested, we check if the HTML has any placeholders. If it finds the %TEMPERATURE% placeholder, we return the temperature by calling
//  the readDHTTemperature() function created previously.

//===============================================
// PROCESSADOR DOS PLACEHOLDERS
//===============================================
String processor(const String &var)
{
  // Serial.println(var);

  if (var == "BUTTONPLACEHOLDER")
  {
    String buttons = "";
    buttons += "<div><p class=\"dht-labels\">Velocidade</p><input type=\"text\" name=\"velocidade\" id=\"velocidade\" placeholder=\"30 Km/h\" value="
               " disabled></div>";
    if (btnParam2Int == 0)
    {
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-off\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>OFF</button></div>";
    }
    else if (btnParam2Int == 1)
    {
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-on\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>ON</button></div>";
    }
    return buttons;
  }
  else if (var == "TEMPERATURE")
  {
    return readDHTTemperature();
  }
  else
  {
    return String();
  }
}

//===============================================
// SETUP
//===============================================

void setup()
{
  Wire.begin(); //INICIALIZA O I2C BUS
  bmp.begin(0x76);
  //Called this function to update de cod, it run for 3 minuts, automatically after this the ESP reset,
  //reiniting it return on de updateWifiCode(), wait again 3 minuts and then, if does`t have cod for 
  //for update the ESP return to setup and init the webserver and others functions.
  //For update the code is ever necessary restart the ESP ou call de function updateWifiCode(),
  //for get successe in this processe, is recommended restart de ESP, instead of call de function

  
  // Intanciando o LED onboard [PODE SER TIRADO DEPOIS]
  pinMode(LED_BUILTIN, OUTPUT); // Habilita o LED onboard como saída.

  // INSTANCIANDO AS VARIÁVEIS
  //-----------------------
  //OUTPUTs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(arref, OUTPUT);
  pinMode(ncBrake, OUTPUT);
  pinMode(ncFree, OUTPUT);
  pinMode(rtBrake, OUTPUT);
  pinMode(rtFree, OUTPUT);
  pinMode(sinlight, OUTPUT);
  //set dos pinos do motor de rotaão da nacele
  pinMode(pwmH, OUTPUT);
  pinMode(pwmA, OUTPUT);

  //INPUTs
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
  digitalWrite(rtBrake, LOW);
  digitalWrite(rtFree, LOW);
  digitalWrite(sinlight, HIGH);
  digitalWrite(pwmH, LOW);
  digitalWrite(pwmA, LOW);

  //Abilitação do AttachInterrupt
  attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);

  // LCD ARDUINO
  //-----------------------
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

  //ATUALIZA O CÓDIGO VIA WIFI
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  //---------------------------

  // PUXA OS DADOS EM TEMPO REAL
  //---------------------------
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });

  server.on("/velocidade", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTVelocidade().c_str()); });
  //---------------------------

  // VERIFICA E ATUALIZA O BOTAO QUE FOR ATIVADO
  //---------------------------
  //  Send a GET request to <ESP_IP>/update?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
  server.on("/atualizaBtn", HTTP_GET, [](AsyncWebServerRequest *request)
            {

            // GET input1 value on <ESP_IP>/atualizaBtn?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
            if(request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)){

                //Captura os valores do request
                btnParam1 = request->getParam(PARAM_INPUT_1)->value();
                btnParam2 = request->getParam(PARAM_INPUT_2)->value();

                btnParam2Int = btnParam2.toInt();

                //(TEORICAMENTE MUDARIA O ESTADO DO OBJETO LIGADO AO GPIO)
                //digitalWrite(btnNaceleMsg, btnNaceleMsg2.toInt());

                //MAS VAMOS FAZER DE OUTRA FORMA
                if(btnParam2Int == 0){
                  digitalWrite(LED_BUILTIN,LOW);
                }else if(btnParam2Int == 1){
                  digitalWrite(LED_BUILTIN, HIGH);
                }
            }else{
              btnParam1 = "--";
              btnParam2 = "--";
            }
              Serial.print("GPIO: ");
              Serial.print(btnParam1);
              Serial.print(" - Set to: ");
              Serial.println(btnParam2);
              request->send(200, "text/plain", "OK"); });
  //---------------------------

  // INICIA O SERVIDOR
  //---------------------------
  server.begin();
  //---------------------------


  // PARTE FINAL LED
  //--------------------------
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

//===============================================
// LOOP
//===============================================

void loop()
{

  // INICIA AS FUNÇÕES
  ------------------------
  readsensors(); 
  
  readminimet(); 
  
  operation();
  
  sinalization();

 //PPRINTA NO LCD
 -------------------------
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

//===============================================
// FUNCOES
//===============================================

// LEITURA DE SENSORES E CONTROLADORES/CONTADORES
//-----------------------

// Função que Incrementa o contador para a RPM
void addcount()
{
  pulsos++;
}

// Media potencia
void mediapot()
{
  while (i1 < 20)
  {
    medpot = medpot + (map(analogRead(pot), 68, 945, 0, 359));
    i1++;
  }
  medpot = medpot / 20;
  i1 = 0;
}

// aquisição dos dasos dos sensores
void readsensors()
{

  // para a medição de temperatura do sistema
  tempsistema = bmp.readTemperature();
  b = 0;

  // para a medição de temperatura do gerador
  tempgerador = 0;
  while (b <= 500)
  {
    tempgerador = (analogRead(GPIOreftemp1) * 0.000805664 * 100) + tempgerador + 6;
    b++;
  }
  tempgerador = tempgerador / 500;
  Tgerador = tempgerador;
  b = 0;

  // Para a medição da tensão so sistema de controle
  tSis = 0;
  while (b <= 20)
  {
    tSis = (((analogRead(GPIOtSis) * 3.3) / 4096.0) / 0.2) + tSis - 0.3;
    b++;
  }
  tSis = tSis / 20;
  b = 0;

  // Para a medição da tensão do sistema de potência
  tPot = 0;
  while (b <= 20)
  {
    tPot = (((analogRead(GPIOtPot) * 3.3) / 4096.0) / 0.2) + tPot - 0.3;
    b++;
  }
  tPot = tPot / 20;
  b = 0;

  // para a leitura da posição de angulo de pitch
  pitchAt = 0;
  while (b <= 30)
  {
    pitchAt = analogRead(GPIOpotpitch) + pitchAt;
    b++;
  }
  pitchAt = pitchAt / 30;
  pitchAt = map(pitchAt, 0, 3000, 0, 90);
  b = 0;

  // Leitura da RPM
  if (millis() - timeold >= 500)
  {
    // Desabilita interrupcao durante o cálculo
    detachInterrupt(digitalPinToInterrupt(35));
    rpm = (60 * 1000 / pulsos_por_volta) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;
    // Reabilitação do AttachInterrupt
    attachInterrupt(digitalPinToInterrupt(35), addcount, FALLING);
  }

  mediapot();
}

// Função de leitura dos dados via I2C da minimet
void readminimet()
{

  if ((millis() - time7) >= 2000)
  {

    outData = 1;
    while (outData <= 4)
    {
      // Serial.print("Dados enviados ao slave==> ");

      Wire.beginTransmission(8); // transmitir dados para o slave com endereço #8
      Wire.write("Request");     // envia uma string
      Wire.write(outData);       // enviar uma variável numérica tipo byte
      Wire.endTransmission(8);   // sinaliza o fim da trasnmissão de dados para o slave
      delay(50);
      // Serial.print(outData);

      Wire.requestFrom(8, 6); // requisitando resposta do slave
      inString = "";
      while (Wire.available())
      {                            // loop para receber todos os dados do slave #8
        char inChar = Wire.read(); // recebe cada byte como caractere
        // if(isAlphaNumeric(inChar)){   //se caractere recebido do slave for alphanumerico concatena string
        inString += inChar; // concatenar o caracter receido
        //}
        // Serial.print("noWhilw");
      }

      // Distribuição dos valores recebidos para as suas respectivas variáveis
      if (outData == 1)
      {
        windspeed = inString.toFloat();
      }

      if (outData == 2)
      {
        winddir = inString.toFloat();
        lcd.setCursor(14, 3);
        lcd.print(winddir, 1);
      }

      if (outData == 3)
      {
        winddirMF = inString.toFloat();
      }

      if (outData == 4)
      {
        ponto = inString;
      }

      outData++;
    }

    // fim da comunicação I2C como a meteo
    time7 = millis();
  }
}

// OPERACOES, CONTORLE E GERENCIAMENTO
//-----------------------

//******************
// AJUSTE AUTOMATICO NACELE
//******************
void ajustenacele()
{

  // Def de ajuste normal -- ajusta o valor de intervalo para não ficar negativo
  mediapot();
  if (deg > medpot)
  {
    inter = deg - medpot;
  }
  if (deg < medpot)
  {
    inter = medpot - deg;
  }

  // quando não é um ajuste de precisão -> inter > 10
  if (deg != medpot & tent <= 8 & inter > 10)
  {
    Serial.print("            Entrou no iff");
    lcd.clear();

    mediapot();
    posi = medpot; // guarda o valor para fins de fer. no aumento da pwm
    olddeg = deg;
    tent++; // permite 5 ajustes de posição
    valpwm = 50;
    if (tent == 5)
    {
      delay(5000);
      mediapot();
    }

    // ajusta o valor de intervalo para não ficar negativo
    if (deg > medpot)
    {
      inter = deg - medpot;
    }
    if (deg < medpot)
    {
      inter = medpot - deg;
    }

    // inicia a rotação o aumento de pwm no sentido Anti-horário
    if (deg > medpot)
    {

      Serial.println("          entrou no while horario");

      mediapot(); // atualiza os valores de posição

      // ajuste do valor da pwm - aceleração
      if ((medpot - posi) < (inter * 0.75) & (millis() - tempo1) > 80)
      {
        tempo1 = millis();
        if (valpwm < 255)
        {
          valpwm++;
        }
      }

      // ajuste do valor da pwm - desacelaração
      if ((medpot - posi) > (inter * 0.25) & (millis() - tempo1) > 150)
      {
        tempo1 = millis();
        if (valpwm > 175)
        {
          valpwm--;
        }
      }

      ledcWrite(pwmH, valpwm); // envia os dados para a pontencia

      // printa os novos valores na serial e lcd
      if (millis() - tempo3 > 500)
      {
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
        tempo3 = millis();
        Serial.print("valor do potênciometro: ");
        Serial.println(analogRead(pot));
        Serial.print("valor da posição:  ");
        Serial.println(medpot);
        Serial.print("valor do deg ");
        Serial.println(deg);
        Serial.print("valpwm");
        Serial.println(valpwm);
      }
    }
    ledcWrite(pwmH, 0);
    ledcWrite(pwmA, 0);
    lcd.clear();

    mediapot(); // atualiza os valores de posição

    // inicia a rotação e aumento de pwm no sentido Horário
    if (deg < medpot)
    {
      Serial.println("         entrou no while antihorario");

      mediapot(); // atualiza os valores de posição

      // ajuste do valor da pwm - aceleração
      if ((posi - medpot) < (inter * 0.75) & (millis() - tempo1) > 80)
      {
        tempo1 = millis();
        if (valpwm < 255)
        {
          valpwm++;
        }
      }

      // ajuste do valor da pwm - desacelaração
      if ((posi - medpot) > (inter * 0.25) & (millis() - tempo1) > 150)
      {
        tempo1 = millis();
        if (valpwm > 175)
        {
          valpwm--;
        }
      }

      ledcWrite(pwmA, valpwm); // envia os dados para a pontencia

      // printa os novos valores na serial e lcd
      if (millis() - tempo3 > 500)
      {
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
        tempo3 = millis();
        Serial.print("valor do potênciometro: ");
        Serial.println(analogRead(pot));
        Serial.print("valor da posição:  ");
        Serial.println(medpot);
        Serial.print("valor do deg ");
        Serial.println(deg);
        Serial.print("valpwm");
        Serial.println(valpwm);
      }
    }
    ledcWrite(pwmH, 0);
    ledcWrite(pwmA, 0);
    lcd.clear();
  } // fim do ajuste normal

  // Def de ajuste ou ajuste de precisão -- ajusta o valor de intervalo para não ficar negativo
  mediapot();
  if (deg > medpot)
  {
    inter = deg - medpot;
  }
  if (deg < medpot)
  {
    inter = medpot - deg;
  }

  // para um ajuste de precisão -> inter <= 10
  if (deg != medpot & tent1 <= 2 & inter <= 10)
  {
    Serial.print("Ajuste de precisão");
    lcd.setCursor(5, 1);
    lcd.print(deg);

    mediapot();
    posi = medpot; // guarda o valor para fins de fer. no aumento da pwm
    olddeg = deg;
    tent1++; // permite 5 ajustes de posição
    valpwm = 50;

    // inicia a rotação o aumento de pwm no sentido Anti-horário
    if (deg > medpot)
    {

      Serial.println("          entrou no while horario ajuste de precisão");

      mediapot(); // atualiza os valores de posição

      // ajuste do valor da pwm - aceleração
      if ((millis() - tempo1) > 50)
      {
        tempo1 = millis();
        if (valpwm < 190)
        {
          valpwm++;
        }
      }

      ledcWrite(pwmH, valpwm); // envia os dados para a pontencia

      // printa os novos valores na serial e lcd
      if (millis() - tempo3 > 500)
      {
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
        tempo3 = millis();
        Serial.print("valor do potênciometro: ");
        Serial.println(analogRead(pot));
        Serial.print("valor da posição:  ");
        Serial.println(medpot);
        Serial.print("valor do deg ");
        Serial.println(deg);
        Serial.print("valpwm");
        Serial.println(valpwm);
      }
    }
    ledcWrite(pwmH, 0);
    ledcWrite(pwmA, 0);
    lcd.clear();
    mediapot(); // atualiza os valores de posição

    // inicia a rotação e aumento de pwm no sentido Horário
    if (deg < medpot)
    {

      Serial.println("         entrou no while antihorario - ajuste de precisão");

      mediapot(); // atualiza os valores de posição

      // ajuste do valor da pwm - aceleração
      if ((millis() - tempo1) > 10)
      {
        tempo1 = millis();
        if (valpwm < 190)
        {
          valpwm++;
        }
      }

      ledcWrite(pwmA, valpwm); // envia os dados para a pontencia

      // printa os novos valores na serial e lcd
      if (millis() - tempo3 > 500)
      {
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
        tempo3 = millis();
        Serial.print("valor do potênciometro: ");
        Serial.println(analogRead(pot));
        Serial.print("valor da posição:  ");
        Serial.println(medpot);
        Serial.print("valor do deg ");
        Serial.println(deg);
        Serial.print("valpwm");
        Serial.println(valpwm);
      }
    }
    ledcWrite(pwmH, 0);
    ledcWrite(pwmA, 0);
    lcd.clear();
  } // fim do ajuste de presisão
}

//******************
// MODO DE AUTO SEGURANCA
//******************
// Modo auto-safe [ON BUILD]
void autoSafe()
{
  // 1-levar o amgulo de pitch para 0
  // 2-alinhar a nacele com a origem do vento ou mantê-la nesta posição
  //*as ações 1 e 2 devem ser executadas simultaneamente
  // 3-bloqueia rotação da nacele - esta função é executada automaticamente no final do posicionamento.
}

//******************
// ARREFECIMENTO
//******************
// Ativação do sistema de arrefecimento geral
void onarref()
{
  Serial.println("Arrefecimento ativado");
  arrefState = "on";
  digitalWrite(arref, LOW);
}

// Desativação do sistema de Arrefecimento geral
void offarref()
{
  Serial.println("Arrefecimento desativado");
  arrefState = "off";
  digitalWrite(arref, HIGH);
}

//******************
// FREIOS NACELE
//******************

// function to brake the nacele rotation
void brakenacele()
{
  digitalWrite(ncBrake, LOW);
  delay(100);
  digitalWrite(ncBrake, HIGH);
  delay(200);
  digitalWrite(ncBrake, LOW);
  delay(600);
  digitalWrite(ncBrake, HIGH);
  ncState = "locked";
}

// function to free the nacele rotation
void freenacele()
{
  digitalWrite(ncFree, LOW);
  delay(100);
  digitalWrite(ncFree, HIGH);
  delay(200);
  digitalWrite(ncFree, LOW);
  delay(400);
  digitalWrite(ncFree, HIGH);
  ncState = "liberate";
}

//******************
// FREIOS ROTOR
//******************

// function to brake the rotor rotation
void brakerotor()
{

  if ((millis() - tempo3) >= 80 & pwmbrake < 255 & brake == 1)
  {
    pwmbrake++;
    ledcWrite(rtBrake, pwmbrake);
    tempo3 = millis();
    rtState = "locking";
  }

  if (pwmbrake == 255 & (millis() - tempo3) >= 200 & brake == 1)
  {
    digitalWrite(rtBrake, LOW);
    brake = 0;
    tempo3 = 0;
    rtState = "locked";
    pwmbrake = 0;
  }

  Serial.println("Ativo Freia");
};

// function to free the rotor rotation
void freerotor()
{

  digitalWrite(rtFree, HIGH);
  delay(500);
  digitalWrite(rtFree, LOW);
  delay(100);
  digitalWrite(rtFree, HIGH);
  delay(200);
  digitalWrite(rtFree, LOW);

  rtState = "liberate";

  Serial.println("Ativo Libera");
};

//******************
// OPERACAO E SINALIZACAO
//******************
// aqui toman-se as decisøes com base nos sensores e operação
void operation()
{

  // ativação automática do arrefecimento
  if (tempgerador >= 23.0 & a == 0)
  {
    onarref();
    a = 1;
  }
  if (tempgerador <= 22.0 & a == 1)
  {
    offarref();
    a = 0;
  }

  // bloaqueio automático do rotor com base em alta rpm - para entrar nesta função o pitch deve ter falhado.
  if (rpm >= 3500 & brake == 0)
  {
    brake = 1;
  }
  if (rpm < 500 & brake == 1)
  { // liberação do rotor
    freerotor();
    brake = 0;
  }
  if (brake == 1)
  { // para entrar na freagem automática do rotor
    brakerotor();
  }
  ajustenacele();

} // fim do operation()

// luz de sinalização
void sinalization()
{

  if ((millis() - time6) >= 1500 & d == 0)
  {
    time6 = millis();
    digitalWrite(sinlight, HIGH);
    d = 1;
    e = 0;
    f = 0;
    g = 0;
  }

  if ((millis() - time6) >= 250 & e == 0)
  {
    digitalWrite(sinlight, LOW);
    e = 1;
  }
  if ((millis() - time6) >= 500 & f == 0)
  {
    digitalWrite(sinlight, HIGH);
    f = 1;
  }
  if ((millis() - time6) >= 750 & g == 0)
  {
    d = 0;
    digitalWrite(sinlight, LOW);
    g = 1;
  }
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
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

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
