//código de acionamento sequencial dos leds
//rgb

int i = 0;

//Arco 1
#define led1R 22
#define led1G 23
#define led1B 24

//Arco 2
#define led2R 25
#define led2G 26 
#define led2B 27

//Arco 3
#define led3R 28
#define led3G 29
#define led3B 30

//Arco 4
#define led4R 31
#define led4G 32
#define led4B 33

//Arco 5
#define led5R 34
#define led5G 35
#define led5B 36

//Arco 6
#define led6R 37
#define led6G 38
#define led6B 39

//Arco 7
#define led7R 40
#define led7G 41
#define led7B 42

//Arco 8
#define led8R 43
#define led8G 44
#define led8B 45

//Arco 9
#define led9R 46
#define led9G 47
#define led9B 48

//Arco 10
#define led10R 49
#define led10G 50
#define led10B 51



//vetorização

//float float red[10] = {};

void setup() {
  // put your setup code here, to run once:

pinMode(led1R, OUTPUT);
pinMode(led1G, OUTPUT);
pinMode(led1B, OUTPUT);

pinMode(led2R, OUTPUT);
pinMode(led2G, OUTPUT);
pinMode(led2B, OUTPUT);

pinMode(led3R, OUTPUT);
pinMode(led3G, OUTPUT);
pinMode(led3B, OUTPUT);

pinMode(led4R, OUTPUT);
pinMode(led4G, OUTPUT);
pinMode(led4B, OUTPUT);

pinMode(led5R, OUTPUT);
pinMode(led5G, OUTPUT);
pinMode(led5B, OUTPUT);

pinMode(led6R, OUTPUT);
pinMode(led6G, OUTPUT);
pinMode(led6B, OUTPUT);

pinMode(led7R, OUTPUT);
pinMode(led7G, OUTPUT);
pinMode(led7B, OUTPUT);

pinMode(led8R, OUTPUT);
pinMode(led8G, OUTPUT);
pinMode(led8B, OUTPUT);

pinMode(led9R, OUTPUT);
pinMode(led9G, OUTPUT);
pinMode(led9B, OUTPUT);

pinMode(led10R, OUTPUT);
pinMode(led10G, OUTPUT);
pinMode(led10B, OUTPUT);


//Inicialização dos leds
Off();

/*/ativação sequencial Vermelho
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH);
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
delay(30); 
digitalWrite(led4G, HIGH);
digitalWrite(led4B, HIGH);
digitalWrite(led7G, HIGH);
digitalWrite(led7B, HIGH);
digitalWrite(led4R, HIGH);
digitalWrite(led7R, HIGH);
delay(30); 
digitalWrite(led8G, HIGH);
digitalWrite(led3B, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led8B, HIGH);
delay(30); 
digitalWrite(led2G, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led9B, HIGH);
digitalWrite(led9G, HIGH);
delay(30); 
digitalWrite(led1G, HIGH);
digitalWrite(led1B, HIGH);
digitalWrite(led10G, HIGH);
digitalWrite(led10B, HIGH);
/*/
//Off();
//delay(800);
}

void loop() {

//TESTE
//pulsoB();
//delay(116);
//pulsoB();
//delay(116);

//PRIMEIRA PARTE PARA LUZ BRANCA 2 COMPASSOS
//ascendenteB();
//delay(930);
//ascendenteB();
//delay(930);

//SEGUNDA PARTE PARA LUZ BRANCA 1/4 COMPASSO (SEMIMINIMA)
//ascendenteB();
//delay(116);
//ascendenteB();
//delay(116);

//TERCEIRA PARTE PARA LUZ BRANCA 1/8 COMPASSO (COLCHEIA)
//ascendenteB();
//delay(58);
//ascendenteB();
//delay(58);

if(millis()>= 39000 && millis()<54200){
  ascendenteB();
  delay(915);
  ascendenteB();
  delay(915);
}

if(millis() >= 54300 && millis()<68000){
  ascendenteB();
  delay(116);
  ascendenteB();
  delay(116);
}

if(millis() >= 68000 && millis()<84000){
  ascendenteB();
  delay(58);
  ascendenteB();
  delay(58);
}





/*
 * EM USO DE FATO
 * ============================
delay(500);
Off();
tetrisR();
delay(500);
Off();
tetrisG();
delay(500);
Off();
tetrisB();
delay(500);
Off();
ascendenteR();
delay(500);
Off();
ascendenteY();
delay(500);
Off();
ascendenteG();
delay(500);
Off();
ascendenteC();
delay(500);
Off();
ascendenteB();
delay(500);
Off();
ascendenteP();

delay(1000);
midedgeR();
midedgeG();
midedgeB();

delay(1000);

upR();

upG();

upB();

onR();
delay(3000);
Off();
onG();
delay(3000);
Off();
onB();
delay(3000);
Off();
onY();
delay(3000);
Off();
onC();
delay(3000);
Off();
onP();
delay(3000);
Off();
onW();
delay(3000);


blockdownB();

Off();
lgbt();
delay(3000);
Off();
Brasil();
delay(3000);
Off();
ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();ascendenteB();

tetrisR();tetrisR();tetrisR();tetrisR();tetrisR();
Off();
blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();blockdownG();

upR();
upR();
upR();
upR();upR();upR();upR();upR();upR();upR();upR();upR();

midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();
midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();midedgeB();

  * ============================
*/



/*


if(millis() > 7200 & millis() < 16000){
  if(millis()%488 == 0){
  ascendenteB();
 }
}


if(millis() > 16000 & millis() < 23000){
  if(millis()%488 == 0){
  ascendenteG();
 }
}

if(millis() > 23000 & millis() < 54000){
  if(millis()%488 == 0){
  blockdownR();
  blockdownG();
  blockdownB();
 }
}

if(millis() > 54000 & millis() < 69000){
  if(millis()%488== 0){
  runningup();
 }
}





if(millis() > 69000 & millis() < 69480){
  if(millis()%488== 0){
   Off();
  onR();
 }
}
if(millis() > 69480 & millis() < 69960){
  if(millis()%488 == 0){
  Off();
  onG();
 }
}
if(millis() > 69960 & millis() < 70440){
  if(millis()%488 == 0){
    Off();
  onB();

 }
}
if(millis() > 70440 & millis() < 70920){
  if(millis()%488== 0){
    Off();
  onR();

 }
}
if(millis() > 70920 & millis() < 71880){
  if(millis()%488== 0){
    Off();
  onG();

 }
}
if(millis() > 71880 & millis() < 72360){
  if(millis()%488== 0){
    Off();
  onB();

 }
}
if(millis() > 72360 & millis() < 72840){
  if(millis()%488== 0){
    Off();
  onR();

 }
}



if(millis() > 73000 & millis() < 200400){
  if(millis()%488 == 0){
  preencherB();
 }
}




*/

}


/*  MODELO PARA PEGAR OS DIGITALWRITE DE FORMA PRATICA
 *   
  //LED 10
  //=================
  // VERMELHO
  digitalWrite(led10R,HIGH);
  digitalWrite(led10R,LOW);
  //VERDE
  digitalWrite(led10G,HIGH);
  digitalWrite(led10G,LOW);
  //AZUL
  digitalWrite(led10B,HIGH);
  digitalWrite(led10B,LOW);

  //LED 9
  //=================
  //VERMELHO
  digitalWrite(led9R,HIGH);
  digitalWrite(led9R,LOW);
  //VERDE
  digitalWrite(led9G,HIGH);
  digitalWrite(led9G,LOW);
  //AZUL
  digitalWrite(led9B,HIGH);
  digitalWrite(led9B,LOW);

  //LED 8
  //=================
  //VERMELHO
  digitalWrite(led8R,HIGH);
  digitalWrite(led8R,LOW);
  //VERDE
  digitalWrite(led8G,HIGH);
  digitalWrite(led8G,LOW);
  //AZUL
  digitalWrite(led8B,HIGH);
  digitalWrite(led8B,LOW);

  //LED 7
  //=================
  //VERMELHO
  digitalWrite(led7R,HIGH);
  digitalWrite(led7R,LOW);
  //VERDE
  digitalWrite(led7G,HIGH);
  digitalWrite(led7G,LOW);
  //AZUL
  digitalWrite(led7B,HIGH);
  digitalWrite(led7B,LOW);

  //LED 6
  //=================
  //VERMELHO
  digitalWrite(led6R,HIGH);
  digitalWrite(led6R,LOW);
  //VERDE
  digitalWrite(led6G,HIGH);
  digitalWrite(led6G,LOW);
  //AZUL
  digitalWrite(led6B,HIGH);
  digitalWrite(led6B,LOW);

  //LED 5
  //=================
  //VERMELHO
  digitalWrite(led5R,HIGH);
  digitalWrite(led5R,LOW);
  //VERDE
  digitalWrite(led5G,HIGH);
  digitalWrite(led5G,LOW);
  //AZUL
  digitalWrite(led5B,HIGH);
  digitalWrite(led5B,LOW);

  //LED 4
  //=================
  //VERMELHO
  digitalWrite(led4R,HIGH);
  digitalWrite(led4R,LOW);
  //VERDE
  digitalWrite(led4G,HIGH);
  digitalWrite(led4G,LOW);
  //AZUL
  digitalWrite(led4B,HIGH);
  digitalWrite(led4B,LOW);


  //LED 3
  //=================
  //VERMELHO
  digitalWrite(led3R,HIGH);
  digitalWrite(led3R,LOW);
  //VERDE
  digitalWrite(led3G,HIGH);
  digitalWrite(led3G,LOW);
  //AZUL
  digitalWrite(led3B,HIGH);
  digitalWrite(led3B,LOW);

  //LED 2
  //=================
  //VERMELHO
  digitalWrite(led2R,HIGH);
  digitalWrite(led2R,LOW);
  //VERDE
  digitalWrite(led2G,HIGH);
  digitalWrite(led2G,LOW);
  //AZUL
  digitalWrite(led2B,HIGH);
  digitalWrite(led2B,LOW);

  //LED 1
  //=================
  //VERMELHO
  digitalWrite(led1R,HIGH);
  digitalWrite(led1R,LOW);
  //VERDE
  digitalWrite(led1G,HIGH);
  digitalWrite(led1G,LOW);
  //AZUL
  digitalWrite(led1B,HIGH);
  digitalWrite(led1B,LOW);

*/

void testeAndre(){

  
  digitalWrite(led1B,HIGH);
  digitalWrite(led2R, HIGH);
  delay(200);
  digitalWrite(led3R,HIGH);
  digitalWrite(led4B,HIGH);
  delay(200);
  digitalWrite(led5G,HIGH);
  digitalWrite(led6B,HIGH);



  
}

void pulsoB(){
  digitalWrite(led10B,HIGH);
  digitalWrite(led9B,HIGH);
  digitalWrite(led8B,HIGH);
  digitalWrite(led7B,HIGH);
  digitalWrite(led6B,HIGH);
  digitalWrite(led5B,HIGH);
  digitalWrite(led4B,HIGH);
  digitalWrite(led3B,HIGH);
  digitalWrite(led2B,HIGH);
  digitalWrite(led1B,HIGH);

  delay(465);

  digitalWrite(led10B,LOW);
  digitalWrite(led9B,LOW);
  digitalWrite(led8B,LOW);
  digitalWrite(led7B,LOW);
  digitalWrite(led6B,LOW);
  digitalWrite(led5B,LOW);
  digitalWrite(led4B,LOW);
  digitalWrite(led3B,LOW);
  digitalWrite(led2B,LOW);
  digitalWrite(led1B,LOW);
  
}


void tetrisR(){
digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);
digitalWrite(led6R, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
delay(20); 
digitalWrite(led3R, HIGH);
delay(50); 
digitalWrite(led3R, LOW);
delay(20); 
digitalWrite(led2R, HIGH);
delay(50); 
digitalWrite(led2R, LOW);
delay(20); 
digitalWrite(led1R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);
digitalWrite(led6R, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
delay(20); 
digitalWrite(led3R, HIGH);
delay(50); 
digitalWrite(led3R, LOW);
delay(20); 
digitalWrite(led2R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);
digitalWrite(led6R, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
delay(20); 
digitalWrite(led3R, HIGH);
delay(50);

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);
digitalWrite(led6R, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);
digitalWrite(led6R, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
delay(50);

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
delay(50);

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
delay(50); 

digitalWrite(led10R, HIGH);
delay(50); 
Off();
}

void tetrisG(){
digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led4G, HIGH);
delay(50); 
digitalWrite(led4G, LOW);
delay(20); 
digitalWrite(led3G, HIGH);
delay(50); 
digitalWrite(led3G, LOW);
delay(20); 
digitalWrite(led2G, HIGH);
delay(50); 
digitalWrite(led2G, LOW);
delay(20); 
digitalWrite(led1G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led4G, HIGH);
delay(50); 
digitalWrite(led4G, LOW);
delay(20); 
digitalWrite(led3G, HIGH);
delay(50); 
digitalWrite(led3G, LOW);
delay(20); 
digitalWrite(led2G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led4G, HIGH);
delay(50); 
digitalWrite(led4G, LOW);
delay(20); 
digitalWrite(led3G, HIGH);
delay(50);

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led4G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led5G, HIGH);
delay(50);

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led6G, HIGH);
delay(50);

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led7G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led8G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
delay(20); 
digitalWrite(led9G, HIGH);
delay(50); 

digitalWrite(led10G, HIGH);
delay(50); 
Off();
}

void tetrisB(){
digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
delay(20); 
digitalWrite(led4B, HIGH);
delay(50); 
digitalWrite(led4B, LOW);
delay(20); 
digitalWrite(led3B, HIGH);
delay(50); 
digitalWrite(led3B, LOW);
delay(20); 
digitalWrite(led2B, HIGH);
delay(50); 
digitalWrite(led2B, LOW);
delay(20); 
digitalWrite(led1B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
delay(20); 
digitalWrite(led4B, HIGH);
delay(50); 
digitalWrite(led4B, LOW);
delay(20); 
digitalWrite(led3B, HIGH);
delay(50); 
digitalWrite(led3B, LOW);
delay(20); 
digitalWrite(led2B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
delay(20); 
digitalWrite(led4B, HIGH);
delay(50); 
digitalWrite(led4B, LOW);
delay(20); 
digitalWrite(led3B, HIGH);
delay(50);

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
delay(20); 
digitalWrite(led4B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
delay(50);

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
delay(50);

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
delay(50); 

digitalWrite(led10B, HIGH);
delay(50); 
Off();
}

void Off(){
//desativação Vermelho
digitalWrite(led1R, LOW);
digitalWrite(led2R, LOW); 
digitalWrite(led3R, LOW); 
digitalWrite(led4R, LOW);
digitalWrite(led5R, LOW);
digitalWrite(led6R, LOW); 
digitalWrite(led7R, LOW);
digitalWrite(led8R, LOW); 
digitalWrite(led9R, LOW); 
digitalWrite(led10R, LOW);

//desativação Verde
digitalWrite(led1G, LOW); 
digitalWrite(led2G, LOW);
digitalWrite(led3G, LOW);
digitalWrite(led4G, LOW);
digitalWrite(led5G, LOW);
digitalWrite(led6G, LOW);
digitalWrite(led7G, LOW); 
digitalWrite(led8G, LOW); 
digitalWrite(led9G, LOW);
digitalWrite(led10G, LOW);

//desativação Azul
digitalWrite(led1B, LOW);
digitalWrite(led2B, LOW);
digitalWrite(led3B, LOW); 
digitalWrite(led4B, LOW);
digitalWrite(led5B, LOW);
digitalWrite(led6B, LOW);
digitalWrite(led7B, LOW);
digitalWrite(led8B, LOW);
digitalWrite(led9B, LOW);
digitalWrite(led10B, LOW);

}
void onR(){
//Vermelho
digitalWrite(led1R, HIGH);
digitalWrite(led2R, HIGH); 
digitalWrite(led3R, HIGH); 
digitalWrite(led4R, HIGH);
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH); 
digitalWrite(led7R, HIGH);
digitalWrite(led8R, HIGH); 
digitalWrite(led9R, HIGH); 
digitalWrite(led10R, HIGH);
}

void onG(){
//Verde
digitalWrite(led1G, HIGH); 
digitalWrite(led2G, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led4G, HIGH);
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
digitalWrite(led7G, HIGH); 
digitalWrite(led8G, HIGH); 
digitalWrite(led9G, HIGH);
digitalWrite(led10G, HIGH);
}

void onB(){
// Azul
digitalWrite(led1B, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led3B, HIGH); 
digitalWrite(led4B, HIGH);
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
digitalWrite(led7B, HIGH);
digitalWrite(led8B, HIGH);
digitalWrite(led9B, HIGH);
digitalWrite(led10B, HIGH);

}

void onP(){
//lilás
digitalWrite(led1B, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led3B, HIGH); 
digitalWrite(led4B, HIGH);
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
digitalWrite(led7B, HIGH);
digitalWrite(led8B, HIGH);
digitalWrite(led9B, HIGH);
digitalWrite(led10B, HIGH);
digitalWrite(led1R, HIGH);
digitalWrite(led2R, HIGH); 
digitalWrite(led3R, HIGH); 
digitalWrite(led4R, HIGH);
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH); 
digitalWrite(led7R, HIGH);
digitalWrite(led8R, HIGH); 
digitalWrite(led9R, HIGH); 
digitalWrite(led10R, HIGH);
}

void onY(){
//amarelo
digitalWrite(led1G, HIGH); 
digitalWrite(led2G, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led4G, HIGH);
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
digitalWrite(led7G, HIGH); 
digitalWrite(led8G, HIGH); 
digitalWrite(led9G, HIGH);
digitalWrite(led10G, HIGH);
digitalWrite(led1R, HIGH);
digitalWrite(led2R, HIGH); 
digitalWrite(led3R, HIGH); 
digitalWrite(led4R, HIGH);
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH); 
digitalWrite(led7R, HIGH);
digitalWrite(led8R, HIGH); 
digitalWrite(led9R, HIGH); 
digitalWrite(led10R, HIGH);
}

void onC(){
// Ciano
digitalWrite(led1B, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led3B, HIGH); 
digitalWrite(led4B, HIGH);
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
digitalWrite(led7B, HIGH);
digitalWrite(led8B, HIGH);
digitalWrite(led9B, HIGH);
digitalWrite(led10B, HIGH);
digitalWrite(led1G, HIGH); 
digitalWrite(led2G, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led4G, HIGH);
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
digitalWrite(led7G, HIGH); 
digitalWrite(led8G, HIGH); 
digitalWrite(led9G, HIGH);
digitalWrite(led10G, HIGH);

}

void onW(){
// White
digitalWrite(led1B, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led3B, HIGH); 
digitalWrite(led4B, HIGH);
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
digitalWrite(led7B, HIGH);
digitalWrite(led8B, HIGH);
digitalWrite(led9B, HIGH);
digitalWrite(led10B, HIGH);
digitalWrite(led1G, HIGH); 
digitalWrite(led2G, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led4G, HIGH);
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
digitalWrite(led7G, HIGH); 
digitalWrite(led8G, HIGH); 
digitalWrite(led9G, HIGH);
digitalWrite(led10G, HIGH);
digitalWrite(led1R, HIGH);
digitalWrite(led2R, HIGH); 
digitalWrite(led3R, HIGH); 
digitalWrite(led4R, HIGH);
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH); 
digitalWrite(led7R, HIGH);
digitalWrite(led8R, HIGH); 
digitalWrite(led9R, HIGH); 
digitalWrite(led10R, HIGH);

}

void ascendenteR(){
  //ativação sequencial Vermelho
digitalWrite(led1R, HIGH);
delay(30); 
digitalWrite(led1R, LOW);
delay(15); 
digitalWrite(led2R, HIGH);
delay(30); 
digitalWrite(led2R, LOW);
delay(15); 
digitalWrite(led3R, HIGH);
delay(30); 
digitalWrite(led3R, LOW);
delay(15); 
digitalWrite(led4R, HIGH);
delay(30); 
digitalWrite(led4R, LOW);
delay(15); 
digitalWrite(led5R, HIGH);
delay(30); 
digitalWrite(led5R, LOW);
delay(15); 
digitalWrite(led6R, HIGH);
delay(30); 
digitalWrite(led6R, LOW);
delay(15); 
digitalWrite(led7R, HIGH);
delay(30); 
digitalWrite(led7R, LOW);
delay(15); 
digitalWrite(led8R, HIGH);
delay(30); 
digitalWrite(led8R, LOW);
delay(15); 
digitalWrite(led9R, HIGH);
delay(30); 
digitalWrite(led9R, LOW);
delay(15); 
digitalWrite(led10R, HIGH);
delay(30); 
digitalWrite(led10R, LOW);
delay(15); 
Off();
}

void descendenteR(){
  //ativação sequencial Vermelho
digitalWrite(led10R, HIGH);
delay(30); 
digitalWrite(led10R, LOW);
delay(15); 
digitalWrite(led9R, HIGH);
delay(30); 
digitalWrite(led9R, LOW);
delay(15); 
digitalWrite(led8R, HIGH);
delay(30); 
digitalWrite(led8R, LOW);
delay(15); 
digitalWrite(led7R, HIGH);
delay(30); 
digitalWrite(led7R, LOW);
delay(15); 
digitalWrite(led6R, HIGH);
delay(30); 
digitalWrite(led6R, LOW);
delay(15); 
digitalWrite(led5R, HIGH);
delay(30); 
digitalWrite(led5R, LOW);
delay(15); 
digitalWrite(led4R, HIGH);
delay(30); 
digitalWrite(led4R, LOW);
delay(15); 
digitalWrite(led3R, HIGH);
delay(30); 
digitalWrite(led3R, LOW);
delay(15); 
digitalWrite(led2R, HIGH);
delay(30); 
digitalWrite(led2R, LOW);
delay(15); 
digitalWrite(led1R, HIGH);
delay(30); 
digitalWrite(led1R, LOW);
delay(15); 
Off();
}

void ascendenteG(){
//ativação sequencial Verde
digitalWrite(led1G, HIGH);
delay(30); 
digitalWrite(led1G, LOW);
delay(15); 
digitalWrite(led2G, HIGH);
delay(30); 
digitalWrite(led2G, LOW);
delay(15); 
digitalWrite(led3G, HIGH);
delay(30); 
digitalWrite(led3G, LOW);
delay(15); 
digitalWrite(led4G, HIGH);
delay(30); 
digitalWrite(led4G, LOW);
delay(15); 
digitalWrite(led5G, HIGH);
delay(30); 
digitalWrite(led5G, LOW);
delay(15); 
digitalWrite(led6G, HIGH);
delay(30); 
digitalWrite(led6G, LOW);
delay(15); 
digitalWrite(led7G, HIGH);
delay(30); 
digitalWrite(led7G, LOW);
delay(15); 
digitalWrite(led8G, HIGH);
delay(30); 
digitalWrite(led8G, LOW);
delay(15); 
digitalWrite(led9G, HIGH);
delay(30); 
digitalWrite(led9G, LOW);
delay(15); 
digitalWrite(led10G, HIGH);
delay(30); 
digitalWrite(led10G, LOW);
delay(15); 
Off();
}

void descendenteG(){
  //ativação sequencial Vermelho
digitalWrite(led10G, HIGH);
delay(30); 
digitalWrite(led10G, LOW);
delay(15); 
digitalWrite(led9G, HIGH);
delay(30); 
digitalWrite(led9G, LOW);
delay(15); 
digitalWrite(led8G, HIGH);
delay(30); 
digitalWrite(led8G, LOW);
delay(15); 
digitalWrite(led7G, HIGH);
delay(30); 
digitalWrite(led7G, LOW);
delay(15); 
digitalWrite(led6G, HIGH);
delay(30); 
digitalWrite(led6G, LOW);
delay(15); 
digitalWrite(led5G, HIGH);
delay(30); 
digitalWrite(led5G, LOW);
delay(15); 
digitalWrite(led4G, HIGH);
delay(30); 
digitalWrite(led4G, LOW);
delay(15); 
digitalWrite(led3G, HIGH);
delay(30); 
digitalWrite(led3G, LOW);
delay(15); 
digitalWrite(led2G, HIGH);
delay(30); 
digitalWrite(led2G, LOW);
delay(15); 
digitalWrite(led1G, HIGH);
delay(30); 
digitalWrite(led1G, LOW);
delay(15); 
Off();
}

void ascendenteB(){
//ativação sequencial Azul
digitalWrite(led1B, HIGH);
delay(30); 
digitalWrite(led1B, LOW);
delay(15); 
digitalWrite(led2B, HIGH);
delay(30); 
digitalWrite(led2B, LOW);
delay(15); 
digitalWrite(led3B, HIGH);
delay(30); 
digitalWrite(led3B, LOW);
delay(15); 
digitalWrite(led4B, HIGH);
delay(30); 
digitalWrite(led4B, LOW);
delay(15); 
digitalWrite(led5B, HIGH);
delay(30); 
digitalWrite(led5B, LOW);
delay(15); 
digitalWrite(led6B, HIGH);
delay(30); 
digitalWrite(led6B, LOW);
delay(15); 
digitalWrite(led7B, HIGH);
delay(30); 
digitalWrite(led7B, LOW);
delay(15); 
digitalWrite(led8B, HIGH);
delay(30); 
digitalWrite(led8B, LOW);
delay(15); 
digitalWrite(led9B, HIGH);
delay(30); 
digitalWrite(led9B, LOW);
delay(15); 
digitalWrite(led10B, HIGH);
delay(30); 
digitalWrite(led10B, LOW);
delay(15); 
Off();
}

void descendenteB(){
  //ativação sequencial Vermelho
digitalWrite(led10B, HIGH);
delay(30); 
digitalWrite(led10B, LOW);
delay(15); 
digitalWrite(led9B, HIGH);
delay(30); 
digitalWrite(led9B, LOW);
delay(15); 
digitalWrite(led8B, HIGH);
delay(30); 
digitalWrite(led8B, LOW);
delay(15); 
digitalWrite(led7B, HIGH);
delay(30); 
digitalWrite(led7B, LOW);
delay(15); 
digitalWrite(led6B, HIGH);
delay(30); 
digitalWrite(led6B, LOW);
delay(15); 
digitalWrite(led5B, HIGH);
delay(30); 
digitalWrite(led5B, LOW);
delay(15); 
digitalWrite(led4B, HIGH);
delay(30); 
digitalWrite(led4B, LOW);
delay(15); 
digitalWrite(led3B, HIGH);
delay(30); 
digitalWrite(led3B, LOW);
delay(15); 
digitalWrite(led2B, HIGH);
delay(30); 
digitalWrite(led2B, LOW);
delay(15); 
digitalWrite(led1B, HIGH);
delay(30); 
digitalWrite(led1G, LOW);
delay(15); 
Off();
}


void ascendenteC(){
//ativação sequencial Ciano
digitalWrite(led1B, HIGH);
digitalWrite(led1G, HIGH);
delay(50); 
digitalWrite(led1B, LOW);
digitalWrite(led1G, LOW);
delay(20); 
digitalWrite(led2B, HIGH);
digitalWrite(led2G, HIGH);
delay(50); 
digitalWrite(led2B, LOW);
digitalWrite(led2G, LOW);
delay(20); 
digitalWrite(led3B, HIGH);
digitalWrite(led3G, HIGH);
delay(50); 
digitalWrite(led3B, LOW);
digitalWrite(led3G, LOW);
delay(20); 
digitalWrite(led4B, HIGH);
digitalWrite(led4G, HIGH);
delay(50); 
digitalWrite(led4B, LOW);
digitalWrite(led4G, LOW);
delay(20); 
digitalWrite(led5B, HIGH);
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led6B, HIGH);
digitalWrite(led6G, HIGH);
delay(50); 
digitalWrite(led6B, LOW);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led7B, HIGH);
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7B, LOW);
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led8B, HIGH);
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8B, LOW);
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led9B, HIGH);
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9B, LOW);
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led10B, HIGH);
digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
digitalWrite(led10G, LOW);
delay(20); 
Off();
}

void ascendenteY(){
//ativação sequencial amarelo
digitalWrite(led1R, HIGH);
digitalWrite(led1G, HIGH);
delay(50); 
digitalWrite(led1R, LOW);
digitalWrite(led1G, LOW);
delay(20); 
digitalWrite(led2R, HIGH);
digitalWrite(led2G, HIGH);
delay(50); 
digitalWrite(led2R, LOW);
digitalWrite(led2G, LOW);
delay(20); 
digitalWrite(led3R, HIGH);
digitalWrite(led3G, HIGH);
delay(50); 
digitalWrite(led3R, LOW);
digitalWrite(led3G, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
digitalWrite(led4G, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
digitalWrite(led4G, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
digitalWrite(led5G, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
digitalWrite(led5G, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
digitalWrite(led6G, HIGH);
delay(50); 
digitalWrite(led6R, LOW);
digitalWrite(led6G, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
digitalWrite(led7G, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
digitalWrite(led8G, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
digitalWrite(led9G, LOW);
delay(20); 
digitalWrite(led10R, HIGH);
digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
digitalWrite(led10G, LOW);
delay(20); 
Off();
}

void ascendenteP(){
//ativação sequencial Lilaz
digitalWrite(led1R, HIGH);
digitalWrite(led1B, HIGH);
delay(50); 
digitalWrite(led1R, LOW);
digitalWrite(led1B, LOW);
delay(20); 
digitalWrite(led2R, HIGH);
digitalWrite(led2B, HIGH);
delay(50); 
digitalWrite(led2R, LOW);
digitalWrite(led2B, LOW);
delay(20); 
digitalWrite(led3R, HIGH);
digitalWrite(led3B, HIGH);
delay(50); 
digitalWrite(led3R, LOW);
digitalWrite(led3B, LOW);
delay(20); 
digitalWrite(led4R, HIGH);
digitalWrite(led4B, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
digitalWrite(led4B, LOW);
delay(20); 
digitalWrite(led5R, HIGH);
digitalWrite(led5B, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
digitalWrite(led5B, LOW);
delay(20); 
digitalWrite(led6R, HIGH);
digitalWrite(led6B, HIGH);
delay(50); 
digitalWrite(led6R, LOW);
digitalWrite(led6B, LOW);
delay(20); 
digitalWrite(led7R, HIGH);
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led7R, LOW);
digitalWrite(led7B, LOW);
delay(20); 
digitalWrite(led8R, HIGH);
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led8R, LOW);
digitalWrite(led8B, LOW);
delay(20); 
digitalWrite(led9R, HIGH);
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led9R, LOW);
digitalWrite(led9B, LOW);
delay(20); 
digitalWrite(led10R, HIGH);
digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
digitalWrite(led10B, LOW);
delay(20); 
Off();
}

void midedgeB(){
//ativação sequencial Azul
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
delay(50); 
digitalWrite(led5B, LOW);
digitalWrite(led6B, LOW);
delay(20); 

digitalWrite(led4B, HIGH);
digitalWrite(led7B, HIGH);
delay(50); 
digitalWrite(led4B, LOW);
digitalWrite(led7B, LOW);
delay(20); 

digitalWrite(led3B, HIGH);
digitalWrite(led8B, HIGH);
delay(50); 
digitalWrite(led3B, LOW);
digitalWrite(led8B, LOW);
delay(20); 

digitalWrite(led2B, HIGH);
digitalWrite(led9B, HIGH);
delay(50); 
digitalWrite(led2B, LOW);
digitalWrite(led9B, LOW);
delay(20); 

digitalWrite(led1B, HIGH);
digitalWrite(led10B, HIGH);
delay(50); 
digitalWrite(led10B, LOW);
digitalWrite(led1B, LOW);
delay(20); 
Off();
}

void midedgeG(){
//ativação centro para bordas verde
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
delay(50); 
digitalWrite(led5G, LOW);
digitalWrite(led6G, LOW);
delay(20); 

digitalWrite(led4G, HIGH);
digitalWrite(led7G, HIGH);
delay(50); 
digitalWrite(led4G, LOW);
digitalWrite(led7G, LOW);
delay(20); 

digitalWrite(led3G, HIGH);
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led3G, LOW);
digitalWrite(led8G, LOW);
delay(20); 

digitalWrite(led2G, HIGH);
digitalWrite(led9G, HIGH);
delay(50); 
digitalWrite(led2G, LOW);
digitalWrite(led9G, LOW);
delay(20); 

digitalWrite(led1G, HIGH);
digitalWrite(led10G, HIGH);
delay(50); 
digitalWrite(led10G, LOW);
digitalWrite(led1G, LOW);
delay(20); 
Off();
}
void midedgeR(){
//ativação sequencial Azul
digitalWrite(led5R, HIGH);
digitalWrite(led6R, HIGH);
delay(50); 
digitalWrite(led5R, LOW);
digitalWrite(led6R, LOW);
delay(20); 

digitalWrite(led4R, HIGH);
digitalWrite(led7R, HIGH);
delay(50); 
digitalWrite(led4R, LOW);
digitalWrite(led7R, LOW);
delay(20); 

digitalWrite(led3R, HIGH);
digitalWrite(led8R, HIGH);
delay(50); 
digitalWrite(led3R, LOW);
digitalWrite(led8R, LOW);
delay(20); 

digitalWrite(led2R, HIGH);
digitalWrite(led9R, HIGH);
delay(50); 
digitalWrite(led2R, LOW);
digitalWrite(led9R, LOW);
delay(20); 

digitalWrite(led1R, HIGH);
digitalWrite(led10R, HIGH);
delay(50); 
digitalWrite(led10R, LOW);
digitalWrite(led1R, LOW);
delay(20); 
Off();
}

void upR(){
  ascendenteR();
  descendenteR();
  Off();
}

void upG(){
  ascendenteG();
  descendenteG();
  Off();
}

void upB(){
  ascendenteB();
  descendenteB();
  Off();
}

void blockdownB(){
  
digitalWrite(led10B, HIGH);
delay(100); 
digitalWrite(led9B, HIGH);
delay(100); 
digitalWrite(led10B, LOW);
digitalWrite(led8B, HIGH);
delay(100); 
digitalWrite(led9B, LOW);
digitalWrite(led7B, HIGH);
delay(100); 
digitalWrite(led8B, LOW);
digitalWrite(led6B, HIGH);
delay(100); 
digitalWrite(led7B, LOW);
digitalWrite(led5B, HIGH);
delay(100); 
digitalWrite(led6B, LOW);
digitalWrite(led4B, HIGH);
delay(100); 
digitalWrite(led5B, LOW);
digitalWrite(led3B, HIGH);
delay(100); 
digitalWrite(led4B, LOW);
digitalWrite(led2B, HIGH);
delay(100); 
digitalWrite(led3B, LOW);
digitalWrite(led1B, HIGH);
delay(100); 
digitalWrite(led2B, LOW);
delay(100); 
digitalWrite(led1B, LOW);
Off();

}

void blockdownG(){
  
digitalWrite(led10G,HIGH);
delay(100); 
digitalWrite(led9G,HIGH);
delay(100); 
digitalWrite(led10G,LOW);
digitalWrite(led8G, HIGH);
delay(100); 
digitalWrite(led9G, LOW);
digitalWrite(led7G, HIGH);
delay(100); 
digitalWrite(led8G, LOW);
digitalWrite(led6G, HIGH);
delay(100); 
digitalWrite(led7G, LOW);
digitalWrite(led5G, HIGH);
delay(100); 
digitalWrite(led6G, LOW);
digitalWrite(led4G, HIGH);
delay(100); 
digitalWrite(led5G, LOW);
digitalWrite(led3G, HIGH);
delay(100); 
digitalWrite(led4G, LOW);
digitalWrite(led2G, HIGH);
delay(100); 
digitalWrite(led3G, LOW);
digitalWrite(led1G, HIGH);
delay(100); 
digitalWrite(led2G, LOW);
delay(100); 
digitalWrite(led1G, LOW);
Off();

}

void blockdownR(){
  
digitalWrite(led10R, HIGH);
delay(100); 
digitalWrite(led9R, HIGH);
delay(100); 
digitalWrite(led10R, LOW);
digitalWrite(led8R, HIGH);
delay(100); 
digitalWrite(led9R, LOW);
digitalWrite(led7R, HIGH);
delay(100); 
digitalWrite(led8R, LOW);
digitalWrite(led6R, HIGH);
delay(100); 
digitalWrite(led7R, LOW);
digitalWrite(led5R, HIGH);
delay(100); 
digitalWrite(led6R, LOW);
digitalWrite(led4R, HIGH);
delay(100); 
digitalWrite(led5R, LOW);
digitalWrite(led3R, HIGH);
delay(100); 
digitalWrite(led4R, LOW);
digitalWrite(led2R, HIGH);
delay(100); 
digitalWrite(led3R, LOW);
digitalWrite(led1R, HIGH);
delay(100); 
digitalWrite(led2R, LOW);
delay(100); 
digitalWrite(led1R, LOW);
Off();

}

void lgbt(){
  //ativação sequencial Azul
digitalWrite(led1B, HIGH);
digitalWrite(led2B, HIGH);
digitalWrite(led1R, HIGH);
digitalWrite(led2R, HIGH);
delay(20); 
digitalWrite(led3B, HIGH);
digitalWrite(led4B, HIGH);
delay(20); 
digitalWrite(led5G, HIGH);
digitalWrite(led6G, HIGH);
delay(20); 
analogWrite(led7G, 160);
digitalWrite(led7R, HIGH);
analogWrite(led8G, 60);
digitalWrite(led8R, HIGH);
delay(20); 
digitalWrite(led9R, HIGH);
digitalWrite(led10R, HIGH);
}


void Brasil(){
  //ativação sequencial Azul
digitalWrite(led5B, HIGH);
digitalWrite(led6B, HIGH);
delay(50); 
digitalWrite(led3R, HIGH);
digitalWrite(led4R, HIGH);
digitalWrite(led7R, HIGH);
digitalWrite(led8R, HIGH);
digitalWrite(led3G, HIGH);
digitalWrite(led4G, HIGH);
digitalWrite(led7G, HIGH);
digitalWrite(led8G, HIGH);
delay(50); 
digitalWrite(led1G, HIGH);
digitalWrite(led2G, HIGH);
digitalWrite(led9G, HIGH);
digitalWrite(led10G, HIGH);

}

void runningupB(){

digitalWrite(led1B, HIGH);
//digitalWrite(led2R, HIGH);
digitalWrite(led3B, HIGH);
//digitalWrite(led4R, HIGH);
digitalWrite(led5B, HIGH);
//digitalWrite(led6R, HIGH);
digitalWrite(led7B, HIGH);
//digitalWrite(led8R, HIGH);
digitalWrite(led9B, HIGH);
//digitalWrite(led10R, HIGH);
delay(100);
Off();
//digitalWrite(led1R, HIGH);
digitalWrite(led2B, HIGH);
//digitalWrite(led3R, HIGH);
digitalWrite(led4B, HIGH);
//igitalWrite(led5R, HIGH);
digitalWrite(led6B, HIGH);
//digitalWrite(led7R, HIGH);
digitalWrite(led8B, HIGH);
//digitalWrite(led9R, HIGH);
digitalWrite(led10B, HIGH);
delay(100);
 Off();
}

void runningcolor(){
    
delay(250);
digitalWrite(led1R, LOW);
digitalWrite(led1B, HIGH);
delay(250);
digitalWrite(led1R, HIGH);
digitalWrite(led1B, LOW);
digitalWrite(led2R, LOW);
digitalWrite(led2B, HIGH);
delay(250);
digitalWrite(led2R, HIGH);
digitalWrite(led2B, LOW);
digitalWrite(led3R, LOW);
digitalWrite(led3B, HIGH);
delay(250);
digitalWrite(led3R, HIGH);
digitalWrite(led3B, LOW);
digitalWrite(led4R, LOW);
digitalWrite(led4B, HIGH);
delay(250);
digitalWrite(led4R, HIGH);
digitalWrite(led4B, LOW);
digitalWrite(led5R, LOW);
digitalWrite(led5B, HIGH);
delay(250);
digitalWrite(led5R, HIGH);
digitalWrite(led5B, LOW);
digitalWrite(led6R, LOW);
digitalWrite(led6B, HIGH);
delay(250);
digitalWrite(led6R, HIGH);
digitalWrite(led6B, LOW);
digitalWrite(led7R, LOW);
digitalWrite(led7B, HIGH);
delay(250);
digitalWrite(led7R, HIGH);
digitalWrite(led7B, LOW);
digitalWrite(led8R, LOW);
digitalWrite(led8B, HIGH);
delay(250);
digitalWrite(led8R, HIGH);
digitalWrite(led8B, LOW);
digitalWrite(led9R, LOW);
digitalWrite(led9B, HIGH);
delay(250);
digitalWrite(led9R, HIGH);
digitalWrite(led9B, LOW);
digitalWrite(led10R, LOW);
digitalWrite(led10B, HIGH);
delay(250);
digitalWrite(led10R, HIGH);
digitalWrite(led10B, LOW);
delay(250);
Off();

}

void preencherR(){

digitalWrite(led1R, HIGH);
delay(15);
digitalWrite(led2R, HIGH);
delay(15);
digitalWrite(led3R, HIGH);
delay(15);
digitalWrite(led4R, HIGH);
delay(15);
digitalWrite(led5R, HIGH);
delay(15);
digitalWrite(led6R, HIGH);
delay(15);
digitalWrite(led7R, HIGH);
delay(15);
digitalWrite(led8R, HIGH);
delay(15);
digitalWrite(led9R, HIGH);
delay(15);
digitalWrite(led10R, HIGH);
delay(15);
    Off();
}

void preencherG(){

digitalWrite(led1G, HIGH);
delay(15);
digitalWrite(led2G, HIGH);
delay(15);
digitalWrite(led3G, HIGH);
delay(15);
digitalWrite(led4G, HIGH);
delay(15);
digitalWrite(led5G, HIGH);
delay(15);
digitalWrite(led6G, HIGH);
delay(15);
digitalWrite(led7G, HIGH);
delay(15);
digitalWrite(led8G, HIGH);
delay(15);
digitalWrite(led9G, HIGH);
delay(15);
digitalWrite(led10G, HIGH);
delay(15);
    Off();
}

void preencherB(){

digitalWrite(led1B, HIGH);
delay(15);
digitalWrite(led2B, HIGH);
delay(15);
digitalWrite(led3B, HIGH);
delay(15);
digitalWrite(led4B, HIGH);
delay(15);
digitalWrite(led5B, HIGH);
delay(15);
digitalWrite(led6B, HIGH);
delay(15);
digitalWrite(led7B, HIGH);
delay(15);
digitalWrite(led8B, HIGH);
delay(15);
digitalWrite(led9B, HIGH);
delay(15);
digitalWrite(led10B, HIGH);
delay(15);
    Off();
}
