#include <IRremote.h>
//hi from mac

const int buttonPin = 8;
const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);

struct nec_signal{
  String signal_name;
  uint16_t sAddress;
  uint8_t sCommand;
  uint8_t sRepeats;
};

//signals
nec_signal rx840_mute;
nec_signal rx840_power;
nec_signal lgtv_power;

void setup(){
  //Prepare to send Serial
  Serial.begin(9600);

  //Recieve button input on correct pin
  pinMode(buttonPin, INPUT);

  //Enable IR Recieving
  irrecv.enableIRIn();
  setLEDFeedback(13, true);

  //Enable IR Sending
  IrSender.begin(3);
  pinMode(LED_BUILTIN, OUTPUT);

  //setup signals
  rx840_mute.signal_name = "RX-840 Mute";
  rx840_mute.sAddress = 0x7A;
  rx840_mute.sCommand = 0x1C;
  rx840_mute.sRepeats = 0;

  rx840_power.signal_name = "RX-840 Power";
  rx840_power.sAddress = 0x7E;
  rx840_power.sCommand = 0x2A;
  rx840_power.sRepeats = 0;

  lgtv_power.signal_name = "LG TV Power";
  lgtv_power .sAddress = 0x4;
  lgtv_power .sCommand = 0x8;
  lgtv_power .sRepeats = 0;

  Serial.println("Setup complete");
}


void loop(){
  //Print recieved ir signal to serial monitor
  if(irrecv.decode()) {
    Serial.print("Recieved: ");
    irrecv.printIRResultShort(&Serial);
    irrecv.resume();
 
  }
  
  if(digitalRead(buttonPin) == HIGH){
    
    // Print current send values
    nec_signal selected_signal = rx840_mute;
    
        
    Serial.println();
    Serial.println("Selected signal: " + selected_signal.signal_name);
    Serial.print(F("Send now: address=0x"));
    Serial.print(selected_signal.sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(selected_signal.sCommand, HEX);
    Serial.print(F(" repeats="));
    Serial.print(selected_signal.sRepeats);
    Serial.println();

    Serial.println(F("Send NEC with 16 bit address"));
    Serial.flush();

    // Results for the first loop to: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    IrSender.sendNEC(selected_signal.sAddress, selected_signal.sCommand, selected_signal.sRepeats);

    delay(95);
  }
  
}
