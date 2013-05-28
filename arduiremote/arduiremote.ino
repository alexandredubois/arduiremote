/*
Remote (RF433) spoofer
2013 - Alexandre Dubois
*/

/* Transmitter pinmap */
const byte RF_TX_VCC = 5;
const byte RF_TX_SIG = 4;
const byte RF_TX_GND = 3;

/* Time constants */
const unsigned long H_TIME = 2080; // Header delay (microseconds)
const unsigned long T_TIME = 405; // base signal duration (microseconds)

/*Commands to send*/
int off[] = {1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,0};
int on[] = {1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1};


/* RF signal usage macro */
#define SIG_HIGH() digitalWrite(RF_TX_SIG, HIGH)
#define SIG_LOW() digitalWrite(RF_TX_SIG, LOW)

/**
 * Send header over RF
 */
inline void send_header(void) {
  SIG_LOW();
  delayMicroseconds(H_TIME);
  SIG_HIGH();
  delayMicroseconds(H_TIME);
}

/**
 * Send footer over RF
 */
inline void send_footer(void) {
  SIG_LOW();
  delayMicroseconds(H_TIME * 10);
}

/**
 * Send logical "1" over RF
 */
inline void send_one(void) {
  SIG_LOW();
  delayMicroseconds(T_TIME);
  SIG_HIGH();
  delayMicroseconds(T_TIME * 2);
}

/**
 * Send logical "0" over RF
 */
inline void send_zero(void) {
  SIG_LOW();
  delayMicroseconds(T_TIME * 2);
  SIG_HIGH();
  delayMicroseconds(T_TIME);
}

/** setup() */
void setup() {

  /* Transmitter pins as output */
  pinMode(RF_TX_VCC, OUTPUT);
  pinMode(RF_TX_SIG, OUTPUT);
  pinMode(RF_TX_GND, OUTPUT);

  /* Fast powerring tips */
  digitalWrite(RF_TX_VCC, HIGH);
  digitalWrite(RF_TX_GND, LOW);

  /* Serial port initialization (for debug) */
  Serial.begin(115200);
  Serial.println("---- Remote spoofer ----");
  Serial.println("Setup completed");
}


void loop(){
  Serial.println("Send ON command");
  send_header();
  for(byte i = 0; i < sizeof(on); ++i) {
	  // Si le bit est à "0" logique
	  if(on[i] == 0)
            send_zero();
          else
            send_one();
    }
   send_footer();
   
   Serial.println("Send OFF command");
   send_header();
  for(byte i = 0; i < sizeof(off); ++i) {
	  // Si le bit est à "0" logique
	  if(off[i] == 0)
            send_zero();
          else
            send_one();
    }
   send_footer();
   delay(2000);
}

