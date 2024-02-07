const int vehlight_r = 10;
const int vehlight_y = 12;
const int vehlight_g = 13;

const int pedlight_r = 7;
const int pedlight_g = 26;

const int vehloop_1 = 20;
const int vehloop_2 = 19;
const int vehloop_3 = 18;
const int vehloop_4 = 17;

const int pedbutton = 22;

int t = 0; //obtain level of traffic (1-4)
bool peds = 0; //pedestrians having absolute right of way
bool vehs = 0; //vehicles having absolute right of way
int t_memory = 0; //store information of traffic on last transition to vehs==true

bool currentOpen = 1; //what light was open most currently: 0 = peds, 1 = vehs

int hourOfDay = -1; //time of day, hours 0 ~ 24 h.

void setup() {
  pinMode(vehlight_r, OUTPUT);
  pinMode(vehlight_y, OUTPUT);
  pinMode(vehlight_g, OUTPUT);

  pinMode(pedlight_r, OUTPUT);
  pinMode(pedlight_g, OUTPUT);

  pinMode(pedbutton, INPUT);
  pinMode(vehloop_1, INPUT);
  pinMode(vehloop_2, INPUT);
  pinMode(vehloop_3, INPUT);
  pinMode(vehloop_4, INPUT);

  digitalWrite(vehlight_r, 1);
  digitalWrite(pedlight_r, 1);

  while (hourOfDay == -1) {
    if (digitalRead(vehloop_2) == HIGH) { //pressing 3rd button from bottom
      hourOfDay = 9; // 9am rush hour scenario 
    } else if (digitalRead(vehloop_3) == HIGH) { //pressing 2nd button from bottom
      hourOfDay = 12; // 12pm regular daytime scenario 
    } else if (digitalRead(vehloop_4) == HIGH) { //pressing 1st button from bottom
      hourOfDay = 3; // 3am nighttime scenario 
    }    
  }

  digitalWrite(vehlight_r, 0);
  digitalWrite(pedlight_r, 0);
  delay (500);

  digitalWrite(vehlight_g, 1);
  digitalWrite(pedlight_r, 1);
}

void loop() {
  t = traffic();

  Serial.print("Current traffic: ");
  Serial.println(t);
  delay (1000);

  if (peds) {
    Serial.print("Guaranteed time for pedestrians in seconds: ");
    Serial.println(10);
    delay(10000); //guaranteed minimum time for pedestrian crossing in ms
    peds = !peds; //peds are no longer in absolute right-of-way
  }

  if (vehs) {
    //guaranteed minimum time to open for vehicles is at least 3000 ms
    // exponential multiplier, appending extra 3000ms, 6000ms, 12000ms or 24000ms depending on t_memory level
    //then multiplying with a factor based on hour-of-day
    int timeToDelay = 3000 + (pow(2, t_memory) * 1500 * hourFactor());
    Serial.print("Guaranteed time for vehicles in seconds: ");
    Serial.println(timeToDelay/1000);
    delay (timeToDelay); 
    vehs = !vehs; //vehicles are no longer in absolute right-of-way
  }

  // Check if the pedestrian button is pressed AND there is no absolute right-of-way AND currently opened are vehs lights
  if (digitalRead(pedbutton) == HIGH && (!vehs && !peds) && currentOpen == 1) {
    openPeds();
    peds = 1; //peds are in absolute right-of-way
    currentOpen = 0;
    Serial.println("Opening pedestrian traffic light");
  }

  // Check if the road induction coils are triggered AND there is no absolute right-of-way AND currently opened are peds lights
  if (t > 0 && (!vehs && !peds) && currentOpen == 0) {
    t_memory = t; //store current traffic state
    openVehs(); 
    vehs = 1; //vehicles are in absolute right-of-way
    currentOpen = 1;
    Serial.println("Opening vehicle traffic light");
  } 
  
}

double hourFactor() { //how much the hour-of-day contributes into vehlight delay
  if ((hourOfDay >= 8 && hourOfDay <= 9) || (hourOfDay >= 16 && hourOfDay <= 17)) //8:00~9:59 or 16:00~17:59
   return 1.5; //rush-hour traffic
  else if (hourOfDay >= 7 && hourOfDay <= 21) //7:00~21:59 but outside of hours mentioned above
   return 1; //regular daytime traffic
  else return 0.8; //nighttime traffic
}

int traffic() {
  int t=0;
  if (digitalRead(vehloop_4) == HIGH) t++;
  if (digitalRead(vehloop_3) == HIGH) t++;
  if (digitalRead(vehloop_2) == HIGH) t++;
  if (digitalRead(vehloop_1) == HIGH) t++;
  return t;
}

void openPeds() {     
    //open pedestrian crossing, close vehicle crossing

    //blink vehicle light green at f = 1 Hz
    digitalWrite(vehlight_g, 0);
    delay(500);
    digitalWrite(vehlight_g, 1);
    delay(500);
    digitalWrite(vehlight_g, 0);
    delay(500);
    digitalWrite(vehlight_g, 1);
    delay(500);
    digitalWrite(vehlight_g, 0);
    delay(500);
    digitalWrite(vehlight_g, 1);
    delay(500);
    //transition to yellow. holding for T = 3 sec
    digitalWrite(vehlight_g, 0);
    digitalWrite(vehlight_y, 1);
    delay(3000);
    //transition to red, holding for T = 3 sec, then opening pedestrian crossing light
    digitalWrite(vehlight_y, 0);
    digitalWrite(vehlight_r, 1);
    delay (3000);
    digitalWrite(pedlight_r, 0);
    digitalWrite(pedlight_g, 1);
  }

  void openVehs() {
     //open vehicle crossing, close pedestrian crossing
    digitalWrite(pedlight_r, 1);
    digitalWrite(pedlight_g, 0);
    delay (3000);
    digitalWrite(vehlight_y, 1);
    delay (1000);
    digitalWrite(vehlight_y, 0);
    digitalWrite(vehlight_r, 0);
    digitalWrite(vehlight_g, 1);
  }
