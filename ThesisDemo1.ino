#include <FlexiTimer2.h>
#include <Ultrasonic.h>
#include <Servo.h>
Ultrasonic sc(45, 12);

class Driver {
    int DIR1;
    int DIR2;
    int DIR3;
    int DIR4;
    int PWM1;
    int PWM2;
  public:
    Driver(int DIR1,
           int DIR2,
           int DIR3,
           int DIR4,
           int PWM1,
           int PWM2)
    {
      this-> DIR1 = DIR1;
      this-> DIR2 = DIR2;
      this-> DIR3 = DIR3;
      this-> DIR4 = DIR4;
      this-> PWM1 = PWM1;
      this-> PWM2 = PWM2;
      pinMode(DIR1, OUTPUT);
      pinMode(DIR2, OUTPUT);
      pinMode(DIR3, OUTPUT);
      pinMode(DIR4, OUTPUT);
    }
    void Forward(int spd) {
      digitalWrite(DIR1, HIGH);
      digitalWrite(DIR2, LOW);
      digitalWrite(DIR3, HIGH);
      digitalWrite(DIR4, LOW);
      analogWrite(PWM1, spd);
      analogWrite(PWM2, spd);
    }
    void Back(int spd) {
      digitalWrite(DIR1, LOW);
      digitalWrite(DIR2, HIGH);
      digitalWrite(DIR3, LOW);
      digitalWrite(DIR4, HIGH);
      analogWrite(PWM1, spd);
      analogWrite(PWM2, spd);
    }
    void Stop() {
      digitalWrite(DIR1, LOW);
      digitalWrite(DIR2, LOW);
      digitalWrite(DIR3, LOW);
      digitalWrite(DIR4, LOW);
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 0);
    }
};

class Platform {
  private:
    Driver *Dr_L;
    Driver *Dr_R;
  public:
    Platform(Driver *Dr_L, Driver *Dr_R) {
      this->Dr_L = Dr_L;
      this->Dr_R = Dr_R;
    }
    void Forward(int, int);
    void Back(int, int);
    void turnLeft(int);
    void turnRight(int);
    void Stop();
};

void Platform::Stop() {
  Dr_L->Stop();
  Dr_R->Stop();
}

void Platform::Forward(int spd1, int spd2) {
  Dr_L->Forward(spd1);
  Dr_R->Forward(spd2);
}

void Platform::Back(int spd1, int spd2) {
  Dr_L->Back(spd1);
  Dr_R->Back(spd2);
}

void Platform::turnLeft(int spd) {
  Dr_L->Back(spd);
  Dr_R->Forward(spd);
}

void Platform::turnRight(int spd) {
  Dr_L->Forward(spd);
  Dr_R->Back(spd);
}

class Datchik {
    int pin;
    bool mode;
    unsigned int timer;
  public:
    bool flag;
    Datchik(int pin, bool mode) {
      flag = false;
      this->pin = pin;
      this->mode = mode;
      pinMode(pin, INPUT);
      timer = 0;
    }
    void Update(unsigned int count = 1) {
      if (digitalRead(pin) != mode) {
        flag = false;
        timer = 0;
      }
      else {
        timer++;
      }
      if (timer > count) {
        flag = true;
      }
    }
};

enum LinesState {
  CLEAR,
  L_LINE,
  R_LINE,
  CROSS
};

class Robot {
    Platform *pl;
    Datchik *L1;
    Datchik *R1;
    Datchik *L2;
    Datchik *R2;
  public:
    LinesState linesState1;
    LinesState linesState2;
    Robot(Platform *pl, Datchik *L1, Datchik *R1, Datchik *L2, Datchik *R2) {
      this->pl = pl;
      this->L1 = L1;
      this->R1 = R1;
      this->L2 = L2;
      this->R2 = R2;
    }
    void Update() {
      L1->Update();
      R1->Update();
      L2->Update();
      R2->Update();
      if (R1->flag == false && L1->flag == false) {
        linesState1 = LinesState::CLEAR;
      }
      else if (R1->flag == true && L1->flag == false) {
        linesState1 = LinesState::R_LINE;
      }
      else if (R1->flag == false && L1->flag == true) {
        linesState1 = LinesState::L_LINE;
      }
      else {
        linesState1 = LinesState::CROSS;
      }

      if (R2->flag == false && L2->flag == false) {
        linesState2 = LinesState::CLEAR;
      }
      else if (R2->flag == true && L2->flag == false) {
        linesState2 = LinesState::R_LINE;
      }
      else if (R2->flag == false && L2->flag == true) {
        linesState2 = LinesState::L_LINE;
      }
      else {
        linesState2 = LinesState::CROSS;
      }
    }
    void Forward(int spd1, int spd2) {
      pl->Forward(spd1, spd2);
    }
    void Back(int spd1, int spd2) {
      pl->Back(spd1, spd2);
    }
    void turnLeft(int spd) {
      pl->turnLeft(spd);
    }
    void turnRight(int spd) {
      pl->turnRight(spd);
    }
    void Balancing(double spd1, double pr = 0.7) {
      int spd2 = spd1 * pr;
      if (linesState1 == LinesState::CLEAR) {
        pl->Forward(spd1, spd1);
      }
      else if (linesState1 == LinesState::L_LINE) {
        pl->turnLeft(spd1);
      }
      else if (linesState1 == LinesState::R_LINE) {
        pl->turnRight(spd1);
      }
      else {
        pl->Forward(spd1, spd1);
      }
    }
   
    void Back_Slide() {
      pl->Back(130, 130);
      if (linesState2 != CLEAR) {
        return;
      }
      Back_Slide();
    }

    void Left(int spd) {
      pl->Stop();
      delay(200);
      Back_Slide();
      pl->turnLeft(spd);
      delay(800);
      while (linesState1 != R_LINE) {
        pl->turnLeft(spd);
      }
      while (linesState1 != L_LINE) {
        pl->turnRight(spd);
      }
      while (linesState1 != R_LINE) {
        pl->turnLeft(spd);
      }
      while (linesState1 != L_LINE) {
        pl->turnRight(spd);
      }

      pl->Stop();
    }

    void Right(int spd) {
      pl->Stop();
      delay(200);
      Back_Slide();
      pl->turnRight(spd);
      delay(800);
      while (linesState1 != L_LINE) {
        pl->turnRight(spd);
      }
      while (linesState1 != R_LINE) {
        pl->turnLeft(spd);
      }
      while (linesState1 != L_LINE) {
        pl->turnRight(spd);
      }
      while (linesState1 != R_LINE) {
        pl->turnLeft(spd);
      }
      pl->Stop();
    }
    void Stop() {
      pl->Stop();
    }

    void Around(int spd) {
      pl->Back(spd, spd);
      delay(350);
      pl->turnLeft(spd);
      delay(800);
      while (linesState1 != R_LINE) {
        pl->turnLeft(spd);
      }
      while (linesState1 != L_LINE) {
        pl->turnRight(50);
      }
      while (linesState1 != R_LINE) {
        pl->turnLeft(40);
      }
      while (linesState1 != L_LINE) {
        pl->turnRight(30);
      }
    }
};

void Go(char arr[], Robot *robot, double spd = 130, int spdTurn = 130, int timer = 60, double pr_balance = 0.7) {
  int now = 0;

  for (int now = 0; arr[now] != '\0';) {
    robot->Balancing(spd, pr_balance);

    if (arr[now] == 'L') {
      if (robot->linesState2 == LinesState::L_LINE || robot->linesState2 == LinesState::CROSS) {
        robot->Left(spdTurn);
        now++;
      }
    }

    else if (arr[now] == 'l') {
      if (robot->linesState2 == LinesState::L_LINE) {
        now++;
      }

    }

    else if (arr[now] == 'R') {
      if (robot->linesState2 == LinesState::R_LINE || robot->linesState2 == LinesState::CROSS) {
        robot->Right(spdTurn);
        now++;
      }
    }
    else if (arr[now] == 'r') {
      if (robot->linesState2 == LinesState::R_LINE) {
        now++;
      }
    }

    else if (arr[now] == 'C') {
      if (robot->linesState2 == LinesState::CROSS) {
        robot->Right(spdTurn);
        robot->Right(spdTurn);
        now++;
      }
    }
    else if (arr[now] == 'c') {
      if (robot->linesState2 != LinesState::CLEAR) {
        now++;
      }
    }
    else {
      robot->Stop();
    }
  }
  robot->Stop();
}

#define DIR1_L 6
#define DIR2_L 7
#define DIR3_L 8
#define DIR4_L 15
#define PWM1_L 5
#define PWM2_L A1

#define DIR1_R 17
#define DIR2_R 16
#define DIR3_R 19
#define DIR4_R 18
#define PWM1_R 11
#define PWM2_R A0

#define L1_pin 4
#define R1_pin 46
#define L2_pin 2
#define R2_pin 44

#define SRV A2

Driver *Dr_L = new Driver(DIR1_L, DIR2_L, DIR3_L, DIR4_L, PWM1_L, PWM2_L);
Driver *Dr_R = new Driver(DIR1_R, DIR2_R, DIR3_R, DIR4_R, PWM1_R, PWM2_R);

Platform *pl = new Platform(Dr_L, Dr_R);

Datchik *L1 = new Datchik(L1_pin, 1);
Datchik *R1 = new Datchik(R1_pin, 1);
Datchik *L2 = new Datchik(L2_pin, 1);
Datchik *R2 = new Datchik(R2_pin, 1);

Robot *robot = new Robot(pl, L1, R1, L2, R2);

Servo servo;

void test(int DEBUG) {
  if (DEBUG == 0) {
    
    Serial.print("L1: ");
    Serial.print(L1->flag);
    Serial.print("\t");
    Serial.print("R1: ");
    Serial.print(R1->flag);
    Serial.print("\t");
    Serial.print("L2: ");
    Serial.print(L2->flag);
    Serial.print("\t");
    Serial.print("R2: ");
    Serial.println(R2->flag);
  }
  if (DEBUG == 1) {
     /*
        digitalWrite(DIR1_L2,HIGH);
        digitalWrite(DIR2_L2,LOW);
        digitalWrite(DIR3_L2,HIGH);
        digitalWrite(DIR4_L2,LOW);
        analogWrite(PWM1_L2,250);
        analogWrite(PWM2_L2,250);
        */
    //pl->turnRight(130);
    /*
      digitalWrite(DIR1, HIGH);
      digitalWrite(DIR2, LOW);
      digitalWrite(DIR3, HIGH);
      digitalWrite(DIR4, LOW);
      analogWrite(PWM1, spd1);
      analogWrite(PWM2, spd2);
    */
    //Dr_L2->Forward(250, 250);
    /*
    Dr_L1->Forward(130, 130);
    Dr_L2->Forward(130, 130);
    Dr_R1->Forward(130, 130);
    Dr_R2->Forward(130, 130);
    */
  }
}
//R - правая линия
//L - левая линия
//С - CROSS
//r - игнорирования
//l
//c
void setup() {
  Serial.begin(9600);
  servo.attach(SRV);
  FlexiTimer2::set(10, timerInterrupt);
  FlexiTimer2::start();
}

void loop() {
  //test(0);
  Go("R",robot,130,200);
  while(sc.Ranging(CM) >= 5){
    robot->Balancing(130);
  }
  dropBall();
  
  robot->Around(200);
  
  Go("l",robot,130,200);
  while(sc.Ranging(CM) >= 5) {
    robot->Balancing(130);
  }
  dropBall();
  
  while(1);
}

void timerInterrupt() {
  robot->Update();
}

void dropBall() {
  servo.write(10);
  delay(500);
  servo.write(90);
  delay(2000);
}
