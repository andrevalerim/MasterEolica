/*********
  André Valerim de Freitas based in Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Importa as bibliotecas
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//===============================================
// CONEXAO WIFI
//===============================================

// Credenciais para conexão com o Wi-fi
const char *ssid = "Consultorio";
const char *password = "terapiaCris";

//const char *ssid = "Quantum Team";
//const char *password = "amotolia_oleosa";

//const char *ssid = "Andre Wifi";
//const char *password = "gostoso1";

//===============================================
// PINOS
//===============================================

//Definindo os pinos digitais conectados aos sensores
#define DHTPIN 27 // Digital pin connected to the DHT sensor

// Tipo de sensor em uso
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

//===============================================
// INSTANCIACAO E PORTA
//===============================================

//Intanciando o objeto DHT com o tipo de pin que foi definido
//DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//===============================================
//VARIAVEIS
//===============================================

// Lendo sensores e criando as Strings para exibicao
int cont = 0;
int cont1 = 0;

//Declarando as variaveis de captura dos paramentros
String btnNaceleMsg1;
String btnNaceleMsg2;
int btnNaceleMsg2int = 0;

//Controle webpage
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Led placa
int LED_BUILTIN = 2;

//===============================================
// LENDO OS SENSORES E OS TRANSFORMANDO EM STRING
//===============================================

//Lê temperatura e converte para string
 //String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)

  // Lê a temperatura do objeto declarado anteriomente, em float
 // float t = dht.readTemperature();
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
 //float t = dht.readTemperature(true);
  
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

//Contadores de teste 
String readDHTTemperature()
{
  cont++;
  Serial.println(cont);
  return String(cont);
}

//Contadores de Teste
String readDHTVelocidade()
{
  cont1++;
  Serial.println(cont1);
  return String(cont1);
}


//OLHAR ESSE SITE A ADAPTAR O CODIGO
//https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/

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

String processor(const String &var)
{
  // Serial.println(var);
  
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<div><p class=\"dht-labels\">Velocidade</p><input type=\"text\" name=\"velocidade\" id=\"velocidade\" placeholder=\"30 Km/h\" value="" disabled></div>";
    if(btnNaceleMsg2int == 0){
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-off\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>OFF</button></div>";
    }else if(btnNaceleMsg2int == 1){
      buttons += "<div><p class=\"dht-labels\">Led Placa</p><button id=\"freioNacele\" class=\"button button-on\" onclick='mudaEstado(\"freioNacele\");toggleBtn(\"freioNacele\")'>ON</button></div>";
    }
    return buttons;
    }else if (var == "TEMPERATURE")
  {
    return readDHTTemperature();
  }else
  {
    return String();}
}

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);//Habilita o LED onboard como saída.
  
  // Serial port for debugging purposes
  Serial.begin(115200);

  //dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // When we make a request on the root URL, we send the HTML text that is stored on the index_html variable.
  // We also need to pass the processorfunction, that will replace all the placeholders with the right values
  //  Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });

  server.on("/velocidade", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTVelocidade().c_str()); });
 
  // Send a GET request to <ESP_IP>/update?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
  server.on("/atualizaBtn", HTTP_GET, [](AsyncWebServerRequest *request){

            // GET input1 value on <ESP_IP>/atualizaBtn?output=<btnNaceleMsg1>&state=<btnNaceleMsg2>
            if(request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)){

                //Captura os valores do request
                btnNaceleMsg1 = request->getParam(PARAM_INPUT_1)->value();
                btnNaceleMsg2 = request->getParam(PARAM_INPUT_2)->value();

                btnNaceleMsg2int = btnNaceleMsg2.toInt();

                //(TEORICAMENTE MUDARIA O ESTADO DO OBJETO LIGADO AO GPIO)
                //digitalWrite(btnNaceleMsg, btnNaceleMsg2.toInt());

                //MAS VAMOS FAZER DE OUTRA FORMA
                if(btnNaceleMsg2int == 0){
                  digitalWrite(LED_BUILTIN,LOW);
                }else if(btnNaceleMsg2int == 1){
                  digitalWrite(LED_BUILTIN, HIGH);
                }
            }else{
              btnNaceleMsg1 = "No message sent";
              btnNaceleMsg2 = "No message sent";
            }
              Serial.print("GPIO: ");
              Serial.print(btnNaceleMsg1);
              Serial.print(" - Set to: ");
              Serial.println(btnNaceleMsg2);
              request->send(200, "text/plain", "OK");
           });

  // Start server
  server.begin();
}

void loop()
{
 //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));//Faz o LED piscar (inverte o estado).
 //delay(250);//espera 250mS para inverter o estado do LED. 
}
