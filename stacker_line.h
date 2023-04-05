#define L 1
#define R 2

int goFlag = 0;                           //교차로 동작감지 flag
unsigned long beforTurnStraightDelay;   //회전하기 전 직진하는 시간값 광역변수
unsigned int lsensorVal;                           //왼쪽센서 판별값(bool값)
unsigned int rsensorVal;                           //오른쪽센서 판별값(bool값)
bool lsensor;
bool rsensor;
/*************************************************************************************/
void onego(int spd);
void sensorTest(unsigned char dir);
void sensorRead(void);
void readyCar(int pin, int ls, int rs);
void left(int spd);
void right(int spd);
void rsensorValue(unsigned int val);
void lsensorValue(unsigned int val);
void straightDelay(unsigned int t);
void delayLine(int spd, unsigned long t);
void lineTrace(int spd);
/*************************************************************************************/
void straightDelay(unsigned int t)     //회전하기 전 직진시간 정하기 함수
{
  beforTurnStraightDelay = t;
}
void lsensorValue(unsigned int val)     //왼쪽센서의 bool 판단 기준값 설정함수
{
  lsensorVal = val;    
}
void rsensorValue(unsigned int val)     //오른쪽센서의 bool 판단 기준값 설정함수
{
  rsensorVal = val;
}
void readyCar(int pin, int ls, int rs)  //라인트레이서 준비함수(시작버튼핀번호, 왼쪽센서기준값, 오른쪽센서기준값)
{
  tempo(4);
  ledBright(255);
  lsensorValue(ls);
  rsensorValue(rs);
  for(int i=0;i<3;i++) buzzer(100,100);
  pinMode(pin, INPUT);
  while (digitalRead(pin) == 1);
  note(do5, n32);
  note(mi5, n32);
  note(sol5, n32);
  note(do6, n32); 
  delay(1000);
}
void sensorRead(void)                   //센서값 읽기 함수(시리얼로 출력)
{
  Serial.begin(9600);
  while (1) {
    Serial.print("left : ");
    Serial.print(analogRead(A0));
    Serial.print(",\t");
    Serial.print("right : ");
    Serial.println(analogRead(A1));
    delay(1000);
  }
}

void sensorTest(unsigned char dir)      //좌우 센서작동 테스트함수(왼쪽 낮은 "도"음, 오른쪽 높은 "도"음 부저출력)
{
  tempo(1);
  if (dir == 1) {
    while (1) {
      lsensor = boolanalogRead(A0, lsensorVal);   
      if (lsensor == 1) note(do4, 10);
      else note(rest, 10);
    }
  }
  else if (dir == 2) {
    while (1) {
      rsensor = boolanalogRead(A1, rsensorVal);        
      if (rsensor == 1) note(do6, 10);
      else note(rest, 10);
    }
  }
}
void onego(int spd)                //한칸 전진 함수(무조건 센서는 크로스를 넘어서 정지함)
{
  while (1) {
    lsensor = boolanalogRead(A0, lsensorVal);   
    rsensor = boolanalogRead(A1, rsensorVal);  
    if ((lsensor == 1) && (rsensor == 1)) break;
    else if ((lsensor == 1) && (rsensor == 0)) wheel(0, spd);
    else if ((lsensor == 0) && (rsensor == 1)) wheel(spd, 0);
    else wheel(spd, spd);
  }
  while (1) {
    lsensor = boolanalogRead(A0, lsensorVal);   
    rsensor = boolanalogRead(A1, rsensorVal);      
    if ((lsensor == 1) && (rsensor == 1)) wheel(spd, spd);
    else if ((lsensor == 1) && (rsensor == 0))wheel(spd, 0);
    else if ((lsensor == 0) && (rsensor == 1))wheel(0, spd);
    else break;
  }
  delayLine(spd, beforTurnStraightDelay);
  wheel(0, 0);
}

void left(int spd)                        //왼쪽으로 한칸 제자리 회전(onego함수 바로 다음 회전하면 straightDelay시간만큼 직진후 회전함)
{
  motor(-200, 200,150);
  while (1) {
    lsensor = boolanalogRead(A0, lsensorVal);      
    if (lsensor == 1) break;
    else wheel(-spd, spd);
  }
  while (1) {
    lsensor = boolanalogRead(A0, lsensorVal);      
    if (lsensor == 0) break;
    else wheel(-spd / 3, spd / 3);
  }
  wheel(0, 0);
}

void right(int spd)                     //오른쪽으로 한칸 제자리 회전(onego함수 바로 다음 회전하면 straightDelay시간만큼 직진후 회전함)
{
  motor(200, -200,150);
  while (1) {
    rsensor = boolanalogRead(A1, rsensorVal);      
    if (rsensor == 1) break;
    else wheel(spd, -spd);
  }
  while (1) {
    rsensor = boolanalogRead(A1, rsensorVal);      
    if (rsensor == 0) break;
    else wheel(spd / 3, -spd / 3);
  }
  wheel(0, 0);
}
void lineTrace(int spd)                                 //라인트래킹 함수(내부함수용)
{
  lsensor = boolanalogRead(A0, lsensorVal);   
  rsensor = boolanalogRead(A1, rsensorVal);  
  if ((lsensor == 1) && (rsensor == 0)) wheel(0, spd);
  else if ((lsensor == 0) && (rsensor == 1)) wheel(spd, 0);
  else wheel(spd, spd);
}

void delayLine(int spd, unsigned long t)
{
  static unsigned long currentTime = 0;
  currentTime = millis();
  while(1) {
    lineTrace(spd);
    if((millis()-currentTime) > t) break;
  }
  wheel(0,0);
}
