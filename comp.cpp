#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include "test.h"
#include "maze.h"

int main()
{
  t_robot_state state = CHECKWALLS;
  cout<<"first line"<<endl;
  setup();
  cout<<"camright"<<endl;
  //tell arduino raspberry is ready
  digitalWrite(CAMRIGHT, HIGH);
  delay(1000);
  digitalWrite(CAMRIGHT, LOW);
  delay(1000); 
  listen();
  bool debug=false;
  bool rightWall=false; 
  bool leftWall=false;
  bool frontWall=false;
  bool turnAround=false;
  int roundNum = readNum("/round.txt");
  cout << "ReadNumFUnction" << endl;
  const int FOOT_CM = 15;
  int start;
  int finish;
/*		int goOn=1;
		while(goOn==1){
	 	cout<<"Left "<<getCMLeft()<<endl;
		cout<<"Right "<<getCMRight()<<endl;
                cout<<"Front "<<getCMFront()<<endl;
		cin>>goOn;
		}*/
  int size = 7;
  Maze aMaze(7,7);
  Edge wall;
  wall.status = WALL_EXISTS;
  Edge nowall;
  nowall.status = WALL_DNE;
  int dest=-1;
  int nodesToExplore=0;
  roundNum=1;
  if(roundNum==1)
  {
    nodesToExplore=26;
    start=47;
    finish=8;
    aMaze.setEdge(8,1,wall);
    aMaze.setEdge(9,2,wall);
    aMaze.setEdge(10,3,wall);
    aMaze.setEdge(11,4,wall);
    aMaze.setEdge(12,5,wall);
    aMaze.setEdge(12,13,wall);
    aMaze.setEdge(19,20,wall);
    aMaze.setEdge(26,27,wall);
    aMaze.setEdge(33,34,wall);
    aMaze.setEdge(40,41,wall);
    aMaze.setEdge(39,46,wall);
    aMaze.setEdge(38,45,wall);
    aMaze.setEdge(37,44,wall);
    aMaze.setEdge(36,43,wall);
    aMaze.setEdge(36,35,wall);
    aMaze.setEdge(29,28,wall);
    aMaze.setEdge(22,21,wall);
    aMaze.setEdge(15,14,wall);
    aMaze.setEdge(47,48,wall);
    aMaze.setEdge(46,47,wall);
    aMaze.setEdge(8,7,wall);
  }
  else if(roundNum==2)
  {
    nodesToExplore=36;
    start=47;
    finish=0;
    aMaze.setEdge(8,1,nowall);
    aMaze.setEdge(9,2,nowall);
    aMaze.setEdge(10,3,nowall);
    aMaze.setEdge(11,4,nowall);
    aMaze.setEdge(12,5,nowall);
    aMaze.setEdge(0,1,nowall);
    aMaze.setEdge(1,2,nowall);
    aMaze.setEdge(2,3,nowall);
    aMaze.setEdge(3,4,nowall);
    aMaze.setEdge(4,5,nowall);
    aMaze.setEdge(12,13,wall);
    aMaze.setEdge(19,20,wall);
    aMaze.setEdge(26,27,wall);
    aMaze.setEdge(33,34,wall);
    aMaze.setEdge(40,41,wall);
    aMaze.setEdge(39,46,wall);
    aMaze.setEdge(38,45,wall);
    aMaze.setEdge(37,44,wall);
    aMaze.setEdge(36,43,wall);
    aMaze.setEdge(35,42,wall);
    aMaze.setEdge(5,6,wall);
    aMaze.setEdge(47,48,wall);
    aMaze.setEdge(46,47,wall);

    aMaze.setEdge(36,35,nowall);
    aMaze.setEdge(29,28,nowall);
    aMaze.setEdge(22,21,nowall);
    aMaze.setEdge(15,14,nowall);
    aMaze.setEdge(8,7,nowall);
    aMaze.setEdge(1,0,nowall);

    aMaze.setEdge(0,7,nowall);
    aMaze.setEdge(7,14,nowall);
    aMaze.setEdge(14,21,nowall);
    aMaze.setEdge(21,28,nowall);
    aMaze.setEdge(28,35,nowall);

  }
  else
  {
   nodesToExplore=49;
   start=48;
   finish=0;
   aMaze.setEdge(42,35, nowall);
   aMaze.setEdge(43,36, nowall);
   aMaze.setEdge(44,37, nowall);
   aMaze.setEdge(45,38, nowall);
   aMaze.setEdge(46,39, nowall);
   aMaze.setEdge(47,40, nowall);
   aMaze.setEdge(48,41, nowall);
   aMaze.setEdge(41,40, nowall);
   aMaze.setEdge(34,33, nowall);
   aMaze.setEdge(27,26, nowall);
   aMaze.setEdge(20,19, nowall);
   aMaze.setEdge(13,12, nowall);
   aMaze.setEdge(6,5, nowall);
   aMaze.setEdge(42,43, nowall);
   aMaze.setEdge(43,44, nowall);
   aMaze.setEdge(44,45, nowall);
   aMaze.setEdge(45,46, nowall);
   aMaze.setEdge(46,47, nowall);
   aMaze.setEdge(47,48, nowall);
   aMaze.setEdge(41,34, nowall);
   aMaze.setEdge(34,27, nowall);
   aMaze.setEdge(27,20, nowall);
   aMaze.setEdge(20,13, nowall);
   aMaze.setEdge(13,6, nowall);
   aMaze.setEdge(6,5, nowall);
  }

    aMaze.setEdge(8,9,nowall);
    aMaze.setEdge(9,10,nowall);
    aMaze.setEdge(10,11,nowall);
    aMaze.setEdge(11,12,nowall);
    aMaze.setEdge(15,16,nowall);
    aMaze.setEdge(16,17,nowall);
    aMaze.setEdge(17,18,nowall);
    aMaze.setEdge(18,19,nowall);
    aMaze.setEdge(22,23,nowall);
    aMaze.setEdge(23,24,nowall);
    aMaze.setEdge(24,25,nowall);
    aMaze.setEdge(25,26,nowall);
    aMaze.setEdge(29,30,nowall);
    aMaze.setEdge(30,31,nowall);
    aMaze.setEdge(31,32,nowall);
    aMaze.setEdge(32,33,nowall);
    aMaze.setEdge(36,37,nowall);
    aMaze.setEdge(37,38,nowall);
    aMaze.setEdge(38,39,nowall);
    aMaze.setEdge(39,40,nowall);

    aMaze.setEdge(8,15,nowall);
    aMaze.setEdge(9,16,nowall);
    aMaze.setEdge(10,17,nowall);
    aMaze.setEdge(11,18,nowall);
    aMaze.setEdge(12,19,nowall);
    aMaze.setEdge(15,22,nowall);
    aMaze.setEdge(16,23,nowall);
    aMaze.setEdge(17,24,nowall);
    aMaze.setEdge(18,25,nowall);
    aMaze.setEdge(19,26,nowall);
    aMaze.setEdge(22,29,nowall);
    aMaze.setEdge(23,30,nowall);
    aMaze.setEdge(24,31,nowall);
    aMaze.setEdge(25,32,nowall);
    aMaze.setEdge(26,33,nowall);
    aMaze.setEdge(29,36,nowall);
    aMaze.setEdge(30,37,nowall);
    aMaze.setEdge(31,38,nowall);
    aMaze.setEdge(32,39,nowall);
    aMaze.setEdge(33,40,nowall);
    aMaze.setEdge(40,47,nowall);
  cout << "Maze initialized" << endl;
  int pos=start;
  int north=-size;
  int east=1;
  int south= size;
  int west=-1;
  string direction = "north";
  //create maze with no walls
  //while(digitalRead(START_BUTTON)==LOW);
  cout << "Turn on start led" << endl;
  digitalWrite(START_LED, LOW);
  do
  {
    aMaze.getVertex(pos).visited=true;

    cout << "Enter while loop" << endl;
    int leftEdge[2];
    int rightEdge[2];
    int frontEdge[2];
    cout << "position: " << pos << endl;
    if(state == CHECKWALLS){
      listen();
      rightWall = false;
      leftWall = false;
      frontWall = false;
      turnAround = false;
      cout << "Get sonar values" << endl;
      int CMLeft = getCMLeft();
      cout << "Sonar left" << CMLeft << endl;
      int CMRight = getCMRight();
      cout << "Sonar right" << CMRight << endl;
      int CMFront = getCMFront();
      cout << "Sonar front" << CMFront << endl;
      if(!(CMLeft>FOOT_CM)) //setting edges
      { 
        if(direction=="north" &&((pos+west) >= 0))
        {
          aMaze.setEdge(pos,pos+west,wall);
          aMaze.setEdge(pos+west,pos,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+west+1<<endl;
          leftEdge[0]=pos;
          leftEdge[1]=pos+west;
        }
        else if(direction=="east" && ((pos+north) >= 0) )
        {
          aMaze.setEdge(pos,pos+north,wall);
          aMaze.setEdge(pos+north,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+north+1<<endl;
          leftEdge[0]=pos;
          leftEdge[1]=pos+north; 
        }
        else if(direction=="south" && ((pos+east) < (size*size)) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          aMaze.setEdge(pos+east, pos, wall);
          leftEdge[0]=pos;
          leftEdge[1]=pos+east;
          cout<<"set edge between"<<pos+1<<", "<<pos+east+1<<endl;
        }
        else if(direction=="west" && ((pos+south) < (size*size)) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          aMaze.setEdge(pos+south,pos,wall);
          leftEdge[0]=pos;
          leftEdge[1]=pos+south;
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
        }
        leftWall=true;
      }
      if(!(CMFront>FOOT_CM))
      {
        if(direction=="north" && ((pos+north) >= 0) )
        {
          aMaze.setEdge(pos,pos+north,wall);
          aMaze.setEdge(pos+north,pos,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+north+1<<endl;
          frontEdge[0]=pos;
          frontEdge[1]=pos+north;
        }
        else if(direction=="east" && ((pos+east) < (size*size)) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          aMaze.setEdge(pos+east,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+east+1<<endl;
          frontEdge[0]=pos;
          frontEdge[1]=pos+east;
        }
        else if(direction=="south" && (pos+south) < (size*size) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          aMaze.setEdge(pos+south,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
          frontEdge[0]=pos;
          frontEdge[1]=pos+south;
        }
        else if(direction=="west" && ((pos+west) >= 0) )
        {
          aMaze.setEdge(pos,pos+west,wall);
          aMaze.setEdge(pos+west,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+west+1<<endl;
          frontEdge[0]=pos;
          frontEdge[1]=pos+west;
        }
        frontWall=true;
      }
      if(!(CMRight>FOOT_CM))
      {
        if(direction=="north" && ((pos+east) < (size*size)) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          aMaze.setEdge(pos+east,pos,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+east+1<<endl;
          rightEdge[0]=pos;
          rightEdge[1]=pos+east;
        }
        else if(direction=="east" && ((pos+south) < (size*size)) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          aMaze.setEdge(pos+south,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
          rightEdge[0]=pos;
          rightEdge[1]=pos+south;
        }
        else if(direction=="south" && ((pos+west) >= 0) )
        {
          aMaze.setEdge(pos,pos+west,wall);
          aMaze.setEdge(pos+west,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+west+1<<endl;
          rightEdge[0]=pos;
          rightEdge[1]=pos+west;
        }
        else if(direction=="west" && ((pos+north) >= 0) )
        {
          aMaze.setEdge(pos,pos+north,wall);
          aMaze.setEdge(pos+north,pos,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+north+1<<endl;
          rightEdge[0]=pos;
          rightEdge[1]=pos+north;
        }
        rightWall=true;
      }
    cout << " new state TAKEPICTURES" << endl;
    state = TAKEPICTURES;
    }
    if(state == TAKEPICTURES){
      //Output OCR result
      if( frontWall )
      {
        if(aMaze.getEdge(frontEdge[0],frontEdge[1]).pic
        &&aMaze.getEdge(frontEdge[1],frontEdge[0]).pic)
        {
          cout << "Take Picture" << endl; 
          bool isGreen=false;
          listen();
          bool isChar = get_OCR(debug,isGreen );
          if(isGreen||isChar)
          {
            if(isChar)
            {
              cout<<"In cell  "<<pos<<endl;
            }
            aMaze.getEdge(frontEdge[0],frontEdge[1]).pic=false;
            aMaze.getEdge(frontEdge[1],frontEdge[0]).pic=false;
          }
        }
       }

      if( leftWall )
      {
        if(aMaze.getEdge(leftEdge[0],leftEdge[1]).pic
           &&aMaze.getEdge(leftEdge[1],leftEdge[0]).pic)
        {
          cout << "Turn Left and Take Picture" << endl;
          camLeft();
          bool isGreen=false;
          bool isChar = get_OCR(debug, isGreen);
          digitalWrite(CAMLEFT,LOW);
          cout << "Center" << endl;
          if(isChar||isGreen)
          {
            if(isChar)
            {
              cout<<"In cell  "<<pos<<endl;
            }
            aMaze.getEdge(leftEdge[0],leftEdge[1]).pic=false;
            aMaze.getEdge(leftEdge[1],leftEdge[0]).pic=false;
          }
        }
       }
      if( rightWall) 
      {
        if(aMaze.getEdge(rightEdge[0],rightEdge[1]).pic
          &&aMaze.getEdge(rightEdge[1],rightEdge[0]).pic)
        {
          cout << "Turn Right and Take Picture" << endl;
          camRight();
          bool isGreen = false;
          bool isChar=get_OCR(debug, isGreen);
          digitalWrite(CAMRIGHT,LOW);
          cout << "Center" << endl;
          if(isChar||isGreen)
          {
            if(isChar)
            {
              cout<<"In cell  "<<pos<<endl;
            }
            aMaze.getEdge(rightEdge[0],rightEdge[1]).pic=false;
            aMaze.getEdge(rightEdge[1],rightEdge[0]).pic=false;
          }
        }
       }

     cout << "new state MOVEDIRECTION" << endl;
     state = MOVEDIRECTION;   
     }
     if( state == MOVEDIRECTION){
      cout << "move the robot" << endl;
      if(leftWall&&rightWall&&frontWall)
      {
        turnAround=true;
        cout<<"Turn Around!"<<endl;
        changeDirection(direction,"around",pos);
        turnRight();
        turnRight();
        cout<<"Move Forward"<<endl;
        move();

      }

      else if(!leftWall)
      {
        cout<<"Turn Left"<<endl;
        turnLeft();
        cout<<"Move Forward"<<endl;
        move();
        changeDirection(direction,"left",pos);
      }
      else if(!frontWall)
      {
        changeDirection(direction,"forward",pos);
        cout<<"Move Forward"<<endl;
         move();
      }
      else if(!rightWall)
      {
         cout<<"Turn Right"<<endl;
         turnRight();
         move();
         changeDirection(direction,"right",pos);
      }

      cout << "next move is determined" << endl;
    cout << "new state CHECKWALLS" << endl;
    state = CHECKWALLS;
    cout<<"NODES LEFT TO EXPLORE: "<<nodesToExplore-aMaze.getExploredNodes()<<endl;
    } 
  }while(aMaze.getExploredNodes()<nodesToExplore);

  cout<<"delay"<<endl;
  while(digitalRead(MODE_BUTTON==HIGH)); 
  delay(1000);
  cout<<"starting critical path"<<endl;
  cout<<"start "<<start<<"finish "<<finish<<endl;
  vector<int> path=aMaze.getPath(start, finish);
  cout<<"path calculation done"<<endl;
  pos=start;
  direction="north";
  cout<<"path {"<<endl;
  for(int i=0; i <path.size(); i++)
  {
    cout<<path[i]<<", "<<endl;
  }
  cout<<"}"<<endl;
  for(int j=1; j<path.size(); j++)
  {
    cout<<"entering for loop"<<endl;
    dest=path[j]-pos;
    pos=path[j];
    if(dest==north) //Get Shortest/Critical Path
    {
      if(direction=="north")
      {
        move();
        cout<<"Move Forward"<<endl;
      }
      else if(direction=="east")
      {
        cout<<"Turn Left"<<endl;
        turnLeft();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="south")
      {
        cout<<"Turn Around"<<endl;
        turnRight();
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else
      {
        turnRight();
        cout<<"Turn Right"<<endl;
        move();
        cout<<"Move Forward"<<endl;
      }

      direction="north";
    }
    else if(dest==east)
    {
      if(direction=="north")
      {
        cout<<"Turn Right"<<endl;
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="east")
      {
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="south")
      {
        cout<<"Turn Left"<<endl;
        turnLeft();
        cout<<"Move Forward"<<endl;
        move();
      }
      else 
      {
        cout<<"Turn Around"<<endl;
        turnRight();
        turnRight();
        listen();
        cout<<"Move Forward"<<endl;
        move();
      }

      direction="east";
    }
    else if(dest==south)
    {
      if(direction=="north")
      {
        cout<<"Turn Around"<<endl;
        turnRight();
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="east")
      {
        cout<<"Turn Right"<<endl;
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="south")
      {
        cout<<"Move Forward"<<endl;
        move();
      }
      else 
      {
        cout<<"Turn Left"<<endl;
        turnLeft();
        cout<<"Move Forward"<<endl;
        move();
      }

      direction="south";
    }
    else//west
    {
      if(direction=="north")
      {
        cout<<"Turn Left"<<endl;
        turnLeft();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="east")
      {
        cout<<"Turn Around"<<endl;
        turnRight();
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else if(direction=="south")
      {
        cout<<"Turn Right"<<endl;
        turnRight();
        cout<<"Move Forward"<<endl;
        move();
      }
      else 
      {
        cout<<"Move Forward"<<endl;
        move();
      }

      direction="west";
    }

    if(pos == finish)
    {
      digitalWrite(END_LED, HIGH);
      delay(50);
    }

  }
  return 0;
}
