#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define BUTTON_UP 6
#define BUTTON_DOWN 7
#define HEIGHT_LCD 65
#define WIDTH_LCD 128

Adafruit_SSD1306 display(WIDTH_LCD, HEIGHT_LCD, &Wire);

//Posição inicial da bola
int ball_dir_y = 30;
int ball_dir_x = 60;

boolean pos_y = true;
boolean pos_x = true;

//Ponto inicial
int point_player = 0;
int point_cpu = 0;

//Velocidade inicial
int speed = 2;
int speed_ball = 2;

//Posição Inicial: Player e CPU
int pos_cpu = HEIGHT_LCD / 4;
int pos_player = HEIGHT_LCD / 4;
const int paddle_height = 16;

void setup() {

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  unsigned long start = millis();

  display.display();
  delay(300);
  display.clearDisplay();
  
  //Loading
  while (millis() - start < 2000);
}

void loop() {
  display.clearDisplay();

  int paddle_half = paddle_height / 2;

  drawWell();
  
  moveBall();
  
  counterPoint();
  
  hitBallAndPlayer();
  
  hitPlayerAndWell(); 
  
  cpuFollowBall(paddle_half);
  
  //LEITURA DO BOTAO
  if (digitalRead(BUTTON_UP) == HIGH) {
    pos_player += speed;
  }

  if (digitalRead(BUTTON_DOWN) == HIGH) {
    pos_player -= speed;
  }

  display.fillCircle(ball_dir_x, ball_dir_y, 2, WHITE); //Bola
  drawPlayer(10, pos_cpu); //CPU
  drawPlayer(119, pos_player); //PLAYER
  
  //PLACAR
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 57);
  display.print(point_cpu);
  display.setCursor(120, 57);
  display.print(point_player);
  
  display.display();
  delay(1);
}

//Desenho da parede ao redor do jogo
void drawWell() {
  display.drawRect(0, 0, 128, 55, WHITE);
}

//Desenho do Player
void drawPlayer(int pos_x, int pos_y) {
  display.drawFastVLine(pos_x, pos_y, paddle_height, WHITE);
}

//Colisao da bola com o Player
void hitBallAndPlayer() {
  //Cada colisao a bola varia sua velocidade entre 1 a 5
  if (ball_dir_x < 15 && ball_dir_y >= pos_cpu && ball_dir_y <= pos_cpu + paddle_height) {
    pos_x = !pos_x;
    speed_ball = random(1, 5);
  } else if (ball_dir_x > 115 && ball_dir_y >= pos_player && ball_dir_y <= pos_player + paddle_height) {
    pos_x = !pos_x;
    speed_ball = random(1, 5);
  }
}

//Colisao dos Players com a Barreira
void hitPlayerAndWell() {
  if (pos_cpu + paddle_height > 50) pos_cpu = 50 - paddle_height;
  if (pos_cpu < 5) pos_cpu = 5;

  if (pos_player + paddle_height > 50) pos_player = 50 - paddle_height;
  if (pos_player < 5) pos_player = 5;
}

//Movimento da Bola e colisão no eixo y
void moveBall() {
  if (ball_dir_y > 50 || ball_dir_y < 5) pos_y = !pos_y;

  if (pos_y == true) ball_dir_y += speed_ball;
  if (pos_y != true) ball_dir_y -= speed_ball;

  if (pos_x == true) ball_dir_x += speed_ball;
  if (pos_x != true) ball_dir_x -= speed_ball;

}

//Contando os pontos...
void counterPoint() {
  //Ponto Player
  if (ball_dir_x > 120) {
    if (pos_x == true) point_cpu++;
    pos_x = false;
  //Ponto CPU
  } else if (ball_dir_x < 2) {
    if (pos_x == false) point_player++;
    pos_x = true;
  }
}

void cpuFollowBall(int paddle_half) {
  //Se a bola estiver a frente da raquete: ELE SEGUE!
  if (ball_dir_x >= 10) {
    if (pos_cpu + paddle_half < ball_dir_y) {
      pos_cpu += speed;
    } else if (pos_cpu + paddle_half > ball_dir_y) {
      pos_cpu -= speed;
    }
  }
  
  //Se a bola estiver atrás da raquete: ELE DESVIA!
  if (ball_dir_x <= 10) {
    if (pos_cpu + paddle_half < ball_dir_y) {
      pos_cpu -= speed;
    } else if (pos_cpu + paddle_half > ball_dir_y) {
      pos_cpu += speed;
    }
  }
}
