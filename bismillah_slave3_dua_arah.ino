#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
//SSL Client
#include <WiFiClientSecure.h>

#define pin_motor D2

//#define botToken "576188757:AAESFfKu1T7jvZbASG6wpHf0m1WG0-xlOmQ"  // your Bot Token (Get from Botfather)  //bot coba
#define botToken "465769589:AAFg10QLqp7XKf_2DpGgTfCWPzCtiOe0Yus"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
long checkTelegramDueTime;
int checkTelegramDelay = 100; //  1000 (1 second)

String defaultChatId = "-128380507"; //This can be got by using a bot called "myIdBot"

char ssid[] = "Gratisan";       // your network SSID (name)
char password[] = "gakngerticok";  // your network key

bool flag=false;

int max_user_telegram=5;
String user_telegram[5];

void setup() {
  Serial.begin(115200);
  pinMode(pin_motor, OUTPUT);
  digitalWrite(pin_motor,HIGH);
  WiFi.begin(ssid, password);
  //Serial.print("kk");
}

// --- Telgram functions ---
void handleNewMessages(int numNewMessages) {
  
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

  //  String from_name = bot.messages[i].from_name;
  //  if (from_name == "") from_name = "Guest";
  
  if (text == "/start" || text == "/help") {
      String welcome = "Welcome to E-TrashBot.\n";// + from_name + ".\n";
      welcome += "The following options are available.\n";
      welcome += "/options is to show the shortcut button.\n";
      welcome += "Use shortcut of button to get the robot information\n";
      welcome += "/start_subscribe is to register automatic notifications\n";    
      welcome += "/help : displays this message again\n";
      bot.sendMessage(chat_id, welcome);
    }
    else if (text == "/start_subscribe") {
      for(int i=0;i < max_user_telegram; i++){
        if(user_telegram[i]==""){
          user_telegram[i]=chat_id;
          break;
        }
      }
      bot.sendMessage(chat_id, "Registered Automatic Notifications");
    }
    else if (text == "/stop_subscribe") {
      for(int i=0; i<max_user_telegram;i++){
        if(user_telegram[i]==chat_id){
          user_telegram[i]="";
          break;
        }
      }
      bot.sendMessage(chat_id, "Automatic Notifications Stopped");
    }
    else if (text == "Check Voltage of Motor Battery") {
      Serial.print('1');
      delay(1000);
      String value1=Serial.readStringUntil('.');
      int data1 = value1.toInt();
      if(data1<=0){
        bot.sendMessage(chat_id, "No Data Received, Check Connection");
      }
      else{
        bot.sendMessage(chat_id, "Voltage of battery motor (%) : " + value1);
      }
    }

    else if (text == "Check Voltage of PC Battery") {
      Serial.print('2');
      delay(1000);
      String value2=Serial.readStringUntil('.');
      int data2 = value2.toInt();
      if(data2<=0){
        bot.sendMessage(chat_id, "No Data Received, Check Connection");
      }
      else{
        bot.sendMessage(chat_id, "Voltage of battery PC (%) : " + value2);
      }
      
    }

    else if (text == "Check Estimation of Motor Battery") {
      Serial.print('3');
      delay(1000);
      String value3=Serial.readStringUntil('.');
      int data3 = value3.toInt();
      if(data3<=0){
        bot.sendMessage(chat_id, "No Data Received, Check Connection");
      }
      else{
        bot.sendMessage(chat_id, "Estimation Time of Battery Motor (minutes) : "+ value3);
      }
      
    }

    else if (text == "Check Estimation of PC Battery") {
      Serial.print('4');
      delay(1000);
      String value4=Serial.readStringUntil('.');
      int data4 = value4.toInt();
      if(data4<=0){
        bot.sendMessage(chat_id, "No Data Received, Check Connection");
      }
      else{
        bot.sendMessage(chat_id, "Estimation Time of Battery PC (minutes) :  "+ value4);
      }
      
    }

    else if (text == "Check Trash Level") {
      Serial.print('5');
      delay(1000);
      String value5=Serial.readStringUntil('.');
      int data5 = value5.toInt();
      if(data5<=0){
        bot.sendMessage(chat_id, "No Data Received, Check Connection");
      }
      else{
        bot.sendMessage(chat_id, "The Level of Trash (%) : " + value5);
      }
      
    }
    
    else if (text == "Power Off Robot") {
        Serial.print('6');
        delay(1000);
        bot.sendMessage(chat_id, "Robot is OFF");
        digitalWrite(pin_motor,LOW); 
        delay(10000);
        digitalWrite(pin_motor,HIGH); 
        delay(10000);
    }

    else if (text == "/options") {
      String keyboardJson = "[[\"Check Voltage of Motor Battery\", \"Check Voltage of PC Battery\"],[\"Check Estimation of Motor Battery\", \"Check Estimation of PC Battery\"] ,[\"Check Trash Level\"],[\"Power Off Robot\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }
    else{
      }
  }
}

void loop() {
  long noww = millis();
  String perintah;
   
  if(noww >= checkTelegramDueTime) {
    //Serial.print(" ");
    Serial.print("dd");
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      //Serial.println("Bot recieved a message");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    if(Serial.available()){
      delay(1000);
      perintah = Serial.readString();
    }
    if(perintah == "stop"){
      for(int i=0; i< max_user_telegram; i++){
        if(user_telegram[i]!=""){
          bot.sendMessage(user_telegram[i], "Robot Stopped Working Because Decision Making System");
        }
      }
      digitalWrite(pin_motor,LOW); 
        delay(10000);
        digitalWrite(pin_motor,HIGH); 
        delay(10000);
        perintah="";
    } 
    else if(perintah=="fail"){
      for(int i=0; i< max_user_telegram; i++){
        if(user_telegram[i]!=""){
          bot.sendMessage(user_telegram[i], "Robot Stopped Working Because Failure System");
        }
      }
      digitalWrite(pin_motor,LOW); 
        delay(10000);
        digitalWrite(pin_motor,HIGH); 
        delay(10000);
        perintah="";
    }
    else if(perintah=="motor"){
      for(int i=0; i< max_user_telegram; i++){
        if(user_telegram[i]!=""){
          bot.sendMessage(user_telegram[i], "The Voltage of Motor Battery is Low");
        }
      }
      perintah="";
    }else if(perintah=="pc"){
      for(int i=0; i< max_user_telegram; i++){
        if(user_telegram[i]!=""){
          bot.sendMessage(user_telegram[i], "The Voltage of PC Battery is Low");
        }
      }
      perintah="";
    }else if(perintah=="trash"){
      for(int i=0; i< max_user_telegram; i++){
        if(user_telegram[i]!=""){
          bot.sendMessage(user_telegram[i], "The Level of Trash is Full");
        }
      }
      perintah="";
    }
    checkTelegramDueTime = noww + checkTelegramDelay;
  }
  noww = millis();
  
}
