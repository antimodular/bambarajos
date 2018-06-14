/*
http://www.tree-axis.com/Ess/
 
 http://code.compartmental.net/beat/demo.htm
 http://code.compartmental.net/beat/
 
 Controls: '1' switches to "SOUND_ENERGY" mode, '2' switches to "FREQ_ENERGY" mode. 
 You'll need to click in the applet before it responds to these keystrokes.
 
 In "SOUND_ENERGY" mode the graphs are the result of calling drawGraph() in each frame 
 and the circle appears every time isOnset() is true. 
 In "FREQ_ENERGY" mode the bars along the bottom of the app are the result of 
 calling drawGraph() in each frame (a bar appears if the corresponding frequency band 
 registers an onset, red bars are every eighth band) and the circles from left to right 
 correspond to isKick(), isSnare(), isHat() and isOnset().
 
 If you don't see any activity it's because you aren't giving it input. 
 You can give it input by plugging a CD player or iPod into the line-in jack of your 
 computer and pressing play on the plugged device, turn up the volume if it seems unresponsive. 
 Alternatively, you can set the record source of your soundcard to "stereo mix" or similar and then 
 play an mp3 in your player of choice.
 */


//with logitech camera in.sampleRate(16000);

import com.apple.cocoa.foundation.*;

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

import krister.Ess.*;

AudioInput in;
int bufferSize;
FFT analysis;
BeatDetect beatFrame;

boolean equalize = false;
boolean limit = true;
boolean useSmooth = true;
float dampAmt = 0.9; //1.0;
boolean useAvg = true; //false;
int numAvg = 64;
float upMult = 0.95;
int kp = 0;
int sp = 0;
int hp = 0;
int op = 0;

float myVolume = 1.0;

PFont fontA;

//int snareVal = 0;

color snareColor = color(0, 0, 0);
void setup()
{
  size(300, 300);
  frameRate(40);
  // smooth();

  fontA = loadFont("SansSerif-12.vlw");
  textFont(fontA, 12);

  Ess.start(this);
  Ess.masterVolume(1);
  bufferSize = 1024;
  // in = new AudioInput();
  println(Ess.activeAudioInput);


  in = new AudioInput(bufferSize);
  in.sampleRate(32000);

  println("buffersize "+bufferSize);
  println("samplerate "+in.sampleRate);


  analysis = new FFT(bufferSize);
  analysis.smooth = useSmooth;
  analysis.equalizer(equalize);
  analysis.limits();
  analysis.damp(dampAmt);
  analysis.averages(numAvg);
  analysis.updateMultiplier = upMult;
  beatFrame = new BeatDetect(in.size, in.sampleRate);
  beatFrame.detectMode("FREQ_ENERGY");



  in.start();
  println(Ess.activeAudioInput);

  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 44999);
  myRemoteLocation = new NetAddress("127.0.0.1", 11999);
  
  frame.setExtendedState(1);
}

void draw()
{
  background(0);

  //----------------------------------------------------------
  strokeWeight(1);
  if ( beatFrame.isHat() )
  {
    OscMessage myMessage = new OscMessage("/isHat");
    oscP5.send(myMessage, myRemoteLocation); 
    fill(182, 115, 55);
    noStroke();
    rect(2, height - (myVolume * height/2), width/4-2, height-1);
    hp = 3;
  }
  else if ( hp > 0 )
  {
    fill(182, 115, 55);
    noStroke();
    rect(2, height - (myVolume * height/2), width/4-2, height-1);
    hp--;
  }
  noFill();
  stroke(255, 255, 255);
  rect(2, height - (myVolume * height/2), width/4-2, height-1);
  fill(255);
  text("HiHat", 10, height-10);
  //----------------------------------------------------------

  if ( beatFrame.isSnare() )
  {
    OscMessage myMessage = new OscMessage("/isSnare");
    oscP5.send(myMessage, myRemoteLocation); 
    fill(182, 115, 55);
    noStroke();
    rect(width/4*1+2, height - (myVolume * height/2), width/4-2, height-1);
    sp = 3;
  }
  else if ( sp > 0 )
  {
    //let's value sink over a few frames
    fill(182, 115, 55);
    noStroke();
    rect(width/4*1+2, height - (myVolume * height/2), width/4-2, height-1);
    sp--;
  }
  noFill();
  stroke(255, 255, 255);
  rect(width/4*1+2, height - (myVolume * height/2), width/4-2, height-1);
  fill(255);
  text("Snare", width/4*1+10, height-10);

  //----------------------------------------------------------

  if ( beatFrame.isKick() )
  {
    OscMessage myMessage = new OscMessage("/isKick");
    oscP5.send(myMessage, myRemoteLocation); 
    fill(182, 115, 55);
    noStroke();
    rect(width/4*2+2, height - (myVolume * height/2), width/4-2, height-1);
    kp = 3;
  }
  else if ( kp > 0 )
  {
    fill(182, 115, 55);
    noStroke();
    rect(width/4*2+2, height - (myVolume * height/2), width/4-2, height-1);
    kp--;
  }    
  noFill();
  stroke(255, 255, 255);
  rect(width/4*2+2, height - (myVolume * height/2), width/4-2, height-1);
  fill(255);
  text("Kick", width/4*2+10, height-10);
  //----------------------------------------------------------



  if ( beatFrame.isOnset() ) 
  {
    OscMessage myMessage = new OscMessage("/isOnset");
    oscP5.send(myMessage, myRemoteLocation); 
    fill(182, 115, 55);
    noStroke();
    rect(width/4*3+2, height - (myVolume * height/2), width/4-4, height-1);
    op = 3;
  }
  else if ( op > 0 )
  {
    fill(182, 115, 55);
    noStroke();
     rect(width/4*3+2, height - (myVolume * height/2), width/4-4, height-1);
    op--;
  }
  noFill();
  stroke(255, 255, 255);
  rect(width/4*3+2, height - (myVolume * height/2), width/4-4, height-1);

  fill(255);
  text("Onset", width/4*3+10, height-10);

  //----------------------------------------------------------

  // draw spectrum
  translate(5, 0);
  stroke(255);
  if ( useAvg )
  {
    strokeWeight(3);
    for (int i = 0; i < analysis.averages.length; i++)
    {
      line(i*6, 0, i*6, analysis.averages[i]*height/2);
    }
  }
  else
  {
    strokeWeight(1);      
    for (int i = 0; i < analysis.spectrum.length; i++)
    {
      line(i*2, 0, i*2, analysis.spectrum[i]*height/2);
    }
  }
  translate(-5, 0);

  //draw text
 // fill(255);
 // text("sensitivity: "+myVolume, 20, height/2);
}

void drawRect() {

  noStroke();
  fill(182, 115, 55);
  rect(0, 0, width/myVolume, height/myVolume);
}
void drawBall(int x, int y)
{
  noStroke();

  ellipse(x, y, 20, 20);
}

void keyPressed()
{
  if ( key == '>' ) {

    myVolume = myVolume + 0.02;
    println("new master valume = "+myVolume);
    in.volume(myVolume);
  }
  if ( key == '<' ) {
    myVolume = myVolume - 0.02;
    println("new master valume = "+myVolume);
    in.volume(myVolume);
  }

  if ( key == 'a' )
  {
    useAvg = (useAvg ? false : true);
  }
  if ( key == 'e' ) 
  { 
    equalize = (equalize ? false : true);
    println("equalize "+equalize);
    analysis.equalizer(equalize);
  }
  if ( key == 'l' )
  {
    if ( limit ) 
    {
      analysis.noLimits();
      limit = false;
    }
    else
    {
      analysis.limits();
      limit = true;
    }
  }
  if ( key == 'f' )
  {
    dampAmt += 0.1;
    if ( dampAmt > 1 ) dampAmt = 1;
    println("Damp amount changed to " + dampAmt + ".");
    analysis.damp(dampAmt);
    //frame.spect.damp(dampAmt);
  }
  if ( key == 'd' )
  {
    dampAmt -= 0.1;
    if ( dampAmt < 0 ) dampAmt = 0;
    println("Damp amount changed to " + dampAmt + ".");
    analysis.damp(dampAmt);
    //frame.spect.damp(dampAmt);
  }
  if ( key == '1' )
  {
    beatFrame.detectMode("SOUND_ENERGY");
  }
  if ( key == '2' )
  {
    beatFrame.detectMode("FREQ_ENERGY");
  }
}

public void audioInputData(AudioInput theInput) {
  analysis.getSpectrum(in);
  beatFrame.detect(in);
}

public void stop()
{
  Ess.stop();
  super.stop();
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */

  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());

  if (theOscMessage.checkAddrPattern("/exit")==true) {
    exit();
  }

  if (theOscMessage.checkAddrPattern("/hideWindow")==true) {
    // frame.setPosition(0,0);
    // frame.setUndecorated(true);
    if (theOscMessage.checkTypetag("i")) {
      int temp_value = theOscMessage.get(0).intValue();
      if (temp_value == 1) frame.setLocation(-width*2, 50);
      if (temp_value == 0) frame.setLocation(0, 50);
    }
  }


  if (theOscMessage.checkAddrPattern("/volume")==true) {

    /* check if the typetag is the right one. */
    if (theOscMessage.checkTypetag("f")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      // int firstValue = theOscMessage.get(0).intValue();  
      float temp_value = theOscMessage.get(0).floatValue();
      //  String temp_value = theOscMessage.get(2).stringValue();
      print("### received an osc message /volume with typetag f.");
      println(" values: "+temp_value);
      myVolume = temp_value;
      in.volume(myVolume);
      // println(" values: "+firstValue+", "+secondValue+", "+thirdValue);
      return;
    }
  }
}

