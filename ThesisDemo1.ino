// подключите пины контроллера к цифровым пинам Arduino

// левый передний двигатель
int enAL = 5;
int in1L = 6;
int in2L = 7;

// левый задний двигатель
int enBL = 10;
int in3L = 8;
int in4L = 9;

//правый передний двигатель
int enAR = 15;
int in1R = 16;
int in2R = 17;

//правый задний двигатель
int enBR = 20;
int in3R = 18;
int in4R = 19;

void setup()
{
  // инициализируем все пины для управления двигателями как outputs
  pinMode(enAL, OUTPUT);
  pinMode(enBL, OUTPUT);
  pinMode(enAR, OUTPUT);
  pinMode(enBR, OUTPUT);
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);
}

void demoOne()
{
  // эта функция обеспечит вращение двигателей в двух направлениях на установленной скорости
  
  // запуск двигателя левый A
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  
  // устанавливаем скорость 200 из доступного диапазона 0~255
  analogWrite(enAL, 200);
  
  // запуск двигателя левый B
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
  
  // устанавливаем скорость 200 из доступного диапазона 0~255
  analogWrite(enBL, 200);

  // запуск двигателя правый A
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  
  // устанавливаем скорость 200 из доступного диапазона 0~255
  analogWrite(enAR, 200);
  
  // запуск двигателя правый B
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
  
  // устанавливаем скорость 200 из доступного диапазона 0~255
  analogWrite(enBR, 200);
  
  delay(2000);
  
  // меняем направление вращения двигателей
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
  
  delay(2000);
  
  // выключаем двигатели
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
  
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);
}

void demoTwo()
{
  // эта функция обеспечивает работу двигателей во всем диапазоне возможных скоростей
  // обратите внимание, что максимальная скорость определяется самим двигателем и напряжением питания
  // ШИМ-значения генерируются функцией analogWrite()
  // и зависят от вашей платы управления
  
  // запускают двигатели
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
  
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
  
  // ускорение от нуля до максимального значения
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enAL, i);
    analogWrite(enBL, i);
    analogWrite(enAR, i);
    analogWrite(enBR, i);
    delay(20);
  }
  
  // торможение от максимального значения к минимальному
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enAL, i);
    analogWrite(enBL, i);
    analogWrite(enAR, i);
    analogWrite(enBR, i);
    delay(20);
  }
  
  // теперь отключаем моторы
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
  
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);
}

void loop()
{
  demoOne();
  delay(1000);
  
  demoTwo();
  delay(1000);
}
