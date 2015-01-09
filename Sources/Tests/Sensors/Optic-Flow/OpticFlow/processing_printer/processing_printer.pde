import processing.serial.*;

final int rate = 38400;
final int off_x = 75;
final int off_y = 70;
final int sz = 22;
final int frameX = 18;
final int frameY = 18;
final int frameLen = frameX * frameY;

Serial port;
int[] frame;
int serialCounter;

int nextFrameTime;
int framePeriod = 300;


void setup()
{
  size( 550, 550 ); 

  frameRate(12);

  frame = new int[frameLen];

  initSerial();

  noStroke();
  noSmooth();
  nextFrameTime = millis();
}

void draw()
{
  serialHandler();

  if( millis() >= nextFrameTime )
  {
    requestFrame();

    background(245);

    for( int i = 0; i < frameLen; i++ )
    {
      fill( map(frame[i], 0, 63, 0, 255) );
      rect(off_x + (i % frameX * sz),
      off_y  + (i / frameY * sz),
      sz, sz);
    }
    
    nextFrameTime = millis() + framePeriod;
  }
}

void keyPressed()
{
  if( key == 'f' )
    port.write('f');

  if( key  == ' ' )
    requestFrame(); 
}

void initSerial()
{
  String portName = Serial.list()[0];
  port = new Serial(this, portName, rate);
  println("Using " + portName + " as serial device.");
}

void requestFrame()
{
  port.write('d');

  serialCounter = frameLen;
}

void serialHandler()
{
  int incoming;
  while( port.available() != 0 )
  {
    incoming = port.read();
    //print(incoming + " ");
    if( serialCounter > 0 )
    {
      if( incoming == 127 )
        serialCounter = 0;
      else
      {
        frame[serialCounter - 1] = incoming;
        serialCounter--;
      }
    }
  }
}
