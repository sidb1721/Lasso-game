#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<Windows.h>
#include<MMSystem.h>


using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  public:
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);
  void setsome(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()


void MovingObject::setsome(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {

  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::setting()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__

#define WINDOW_X 1000
#define WINDOW_Y 750
#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)


class Coin : public MovingObject {
 public:
  double coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  double coin_start_y = PLAY_Y_HEIGHT;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();
  void setCoin(double startx,double starty);//self made function
  void colorit(int r,int g,int b);//
  void freeze(double theta);//
  void setxy(double x,double y){}
  void setrelease(double x){release_angle_deg=x;}//
  void showrelease(){cout << release_angle_deg;}//


}; // End class Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960


#define STEP_TIME 0.05


#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

#define BOMB_RADIUS 20// how close to bomb to detonate

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  int num_coins;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);


  void nextStep(double t);
  void check_for_coin(Coin *coin);
  bool check_for_bomb(Coin *coin);
  int getNumCoins() { return num_coins; }
  double theta(Coin*coin);




}; // End class Lasso





#endif

//coin.h

void Coin::initCoin() {

  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::colorit(int r,int g,int b) {

  coin_circle.setColor(COLOR(r,g,b));
  coin_circle.setFill(true);

}

void Coin::resetCoin() {
  double coin_angle_deg= release_angle_deg;
  double coin_speed =release_speed;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::setCoin(double startx,double starty) {
  double coin_angle_deg= release_angle_deg;
  double coin_speed =release_speed;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(startx, starty, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::freeze(double theta) {
  double coin_speed =release_speed;
  coin_ax = 0;
  coin_ay = 0;
  bool paused = true, rtheta = true;
  setsome(coin_speed, theta, coin_ax, coin_ay, paused,rtheta);
}


//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("brown"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("brown"));

  lasso_band.setColor(COLOR("BlueViolet"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
    the_coin=NULL;
  }
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
  }

bool Lasso::check_for_bomb(Coin *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= BOMB_RADIUS) {
    return true;
  }
  else return false;

}

double Lasso::theta(Coin *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double theta = (180.0/PI)*atan2(ydiff,xdiff);
  return theta;
}

main_program {

  wait(5);
  cout <<"Instructions" <<endl;
  cout <<"The objective of this game is to get a set no of coins in 30 seconds to advance to the next round "<<endl;
  cout <<"Along with usual yellow colored coins there are:"<<endl;
  cout <<"Black bombs:Decrease score by 1 \n"<<"Green bombs:Decrease time limit by 10s  \n";
  cout<<"Red Coins:Increase time limit by 10s\n"<<"Blue coins(also called a freeze coin):Freezes all coins and boms for that level\n";

  cout<<"Controls:"<<endl;
  cout<<"t:throw y:yank l:loop q:quit [:rotate clockwise ]:rotate anticlockwise =:increase speed -:decrease speed";


  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  int lives=3;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  Text livestext(PLAY_X_START+50, PLAY_Y_HEIGHT+80, "Lives:3");
  char livesStr[256];

  string mes("Timer:_");
  char timer[256];
  Text timer_(PLAY_X_START+400, PLAY_Y_HEIGHT-400,mes);
  //timer for level

  paused = true; rtheta = true;

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(int i=1;i<=6;i++){
  char ch;
  int magc=0;

  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;

  switch(i){
  case 1:{
    Text t1(WINDOW_X/2,WINDOW_Y/2,"LEVEL 1");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=1.1;
    coin_speed*=1.2;
    wait(3);
    break;
  }
  case 2:{
    Text t1(WINDOW_X/2,WINDOW_Y/2,"LEVEL 2");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=0.9;
    coin_speed*=1.2;
    break;
    wait(3);
  }
  case 3:{
    Text t1(WINDOW_X/2,WINDOW_Y/2,"LEVEL 3");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=0.9;
    coin_speed*=1.3;
    wait(3);
    break;
  }
  case 4:{
    Text t1(WINDOW_X/2,WINDOW_Y/2,"LEVEL 4");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=0.9;
    coin_speed*=1.3;
    wait(3);
    break;
  }
  case 5:{
    Text t1(WINDOW_X/2,WINDOW_Y/2,"LEVEL 5");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=0.9; //orbiting bomb
    coin_speed*=1.2;
    wait(3);
    break;
  }
  case 6:{
    cout << "Congratulations!You have finished the main game.Would like to play the bonus level?"<<endl;
    cin>>ch;
    if(ch!='y') break;
    else if (ch!='y') {cout <<"Invalid."; break;}
    Text t1(WINDOW_X/2,WINDOW_Y/2,"BONUS LEVEL");
    t1.setColor(COLOR("green"));
    coin_angle_deg*=0.9;
    coin_speed*=1.2;
    wait(3);
    break;

  }
  //bonus level
  }
  if(ch=='n') break;
  if(currTime>29){ break;}
  if(lives==0){break;}
  currTime=0;


 Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
 Coin bomb1(0.9*coin_speed, 1.1*coin_angle_deg, coin_ax, coin_ay, paused, rtheta); bomb1.colorit(0,0,0);
 Coin bomb2(coin_speed, 1.2*coin_angle_deg, coin_ax, coin_ay, paused, rtheta); bomb2.colorit(0,200,0);
 Coin redcoin(coin_speed, 1.15*coin_angle_deg, coin_ax, coin_ay, paused, rtheta); redcoin.colorit(200,0,0);
 Coin freeze(coin_speed, 1.3*coin_angle_deg, coin_ax, coin_ay, paused, rtheta); freeze.colorit(0,0,200);
 Coin satellite(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
   satellite.coin_circle.moveTo(coin.getXPos()+70,coin.getYPos());satellite.coin_circle.rotate(-PI/2);
   satellite.colorit(300,300,300);
//all coins and bombs creation

  if(i==6){
  bomb1.colorit(255,255,0);
  bomb2.colorit(255,255,0);
  redcoin.colorit(255,255,0);
  freeze.colorit(255,255,0);
  satellite.colorit(255,255,0);
  }
// creation of all coins and bombs
  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	if(i==6){
	lasso.check_for_coin(&bomb1);
	lasso.check_for_coin(&bomb2);
	lasso.check_for_coin(&redcoin);
	lasso.check_for_coin(&freeze);
	lasso.check_for_coin(&satellite);
	}
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);
   //motion of lasso and controls

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT ||coin.getXPos()>WINDOW_X||coin.getYPos()<0) {
      coin.resetCoin();
      satellite.resetCoin();
      last_coin_jump_end = currTime;

    }
    //for coin

    if(i==2||i==4||i==6){
        bomb1.nextStep(stepTime);
    if(bomb1.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	bomb1.unpause();
      }
    }

    if(bomb1.getYPos() > PLAY_Y_HEIGHT ||bomb1.getXPos()>WINDOW_X||bomb1.getYPos()<0) {
      bomb1.resetCoin();
      last_coin_jump_end = currTime;

    }


    }
    if(i>2){
        bomb2.nextStep(stepTime);
    if(bomb2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	bomb2.unpause();
      }
    }

    if(bomb2.getYPos() > PLAY_Y_HEIGHT ||bomb2.getXPos()>WINDOW_X||bomb2.getYPos()<0) {
      bomb2.resetCoin();
      last_coin_jump_end = currTime;

    }
    //bomb2

    redcoin.nextStep(stepTime);
    if(redcoin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	redcoin.unpause();
      }
    }

    if(redcoin.getYPos() > PLAY_Y_HEIGHT ||redcoin.getXPos()>WINDOW_X||redcoin.getYPos()<0) {
      redcoin.resetCoin();
      last_coin_jump_end = currTime;

    }
    //redcoin
    }

    if(i==2||i==6){
        freeze.nextStep(stepTime);
    if(freeze.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	freeze.unpause();
      }
    }

    if(freeze.getYPos() > PLAY_Y_HEIGHT ||freeze.getXPos()>WINDOW_X||freeze.getYPos()<0) {
      freeze.resetCoin();
      last_coin_jump_end = currTime;

    }
    }
    //for freeze
   if(i==5||i==6){
   satellite.nextStep(stepTime);
    if(satellite.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	satellite.unpause();
      }
    }

    satellite.coin_circle.forward(14*PI/3);
    satellite.coin_circle.rotate(-PI/15);


   }
   if(i!=6){
      if(lasso.check_for_bomb(&bomb1)){
    lives--;
    sprintf(livesStr,"Lives: %d",lives);
    livestext.setMessage(livesStr);
    bomb1.resetCoin();
    lasso.yank();

    }

    if(i>2&&lasso.check_for_bomb(&bomb2)){
    currTime+=10;
    bomb2.resetCoin();
    lasso.yank();

    }

    if(i>2&&lasso.check_for_bomb(&redcoin)){
    currTime-=10;
    redcoin.resetCoin();

    }
    if(lasso.check_for_bomb(&satellite)){
    lives--;
    satellite.resetCoin();

    }

    if(i==2&&lasso.check_for_bomb(&freeze)){
      magc++;
      freeze.resetCoin();
    }
    if(magc!=0){
      coin.freeze(lasso.theta(&coin));
      bomb1.freeze(lasso.theta(&bomb1));
      bomb2.freeze(lasso.theta(&bomb2));
      redcoin.freeze(lasso.theta(&redcoin));


    }
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    if(i!=6){
    sprintf(timer,"Timer %d",int(30-currTime));
    timer_.setMessage(timer);
    }
    if((currTime>29||lives==0)&&i!=6) break;
    if(lasso.getNumCoins()==i&&i!=6) break;


    stepCount++;
    currTime += stepTime;
    wait(stepTime);

  } // End for(;;)
  }

  if(currTime>29||lives==0){
  Rectangle r(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);
  r.setColor(COLOR("black"));
  r.setFill(true);
  Text game_over(WINDOW_X/2,WINDOW_Y/2,"Game Over");
  game_over.setColor(COLOR("red"));
  r.imprint();
  wait(5);
  }
  //game over screen

  wait(3);
} // End main_program
