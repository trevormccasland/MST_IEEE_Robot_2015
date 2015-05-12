//Testing capture and retrieval
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include "test.h"
#include "maze.h"
using namespace std;
using namespace cv;

int main ( int argc,char **argv )
{
  bool debug = false;
  int input;
  bool dummy = false;
  cout<<"Debug mode?"<<endl;
  char as;
  cin>>as;
  if(as != 'n')
  {
    debug = true;
  }
  cout<<"Select an option:"<<endl;
  cout<<"1 for RGB Filter"<<endl;
  cout<<"2 for edge detection"<<endl;
  cout<<"3 OCR"<<endl;
  cout<<"4 sonar reading"<<endl;
  cout<<"5 Maze Simulation"<<endl;
  cin>>input;
  if(input==1)
  {
    Mat mat;
    //while(true)
    
    GetCameraImage(mat);   
    imshow("asdf",mat);
    waitKey();
    
    RGBFilter(mat);
  }
  else if(input==2)
  {
    Mat mat;
    GetCameraImage(mat);   
    Mat edges = CannyThreshold(mat, 20, 50);
    imshow("edges", edges);
    waitKey();
  }
  else if(input==3)
  {
    
    get_OCR(debug, dummy);
  }
  else if(input==4)
  {
    cout<<"entering setup"<<endl;
    setup();

    cout<<"entering getCM"<<endl;
    int distance1 = getCMLeft();
    cout<<"Distance Left is: "<<distance1<<endl;
    int distance2 = getCMRight();
    cout<<"Distance Right is: "<<distance2<<endl;
    int distance3 = getCMFront();
    cout<<"Distance Front is: "<<distance3<<endl;
  }
  else 
  {
    cout<<"entering setup"<<endl;
    setup();
    cout<<readNum("round.txt")<<endl;
    const int FOOT_CM = 20;
    int size=3;
    int pos=(size*size)-1;//start
    int north=-size;
    int east=1;
    int south= size;
    int west=-1;
    string direction = "north";
    Maze aMaze(size,size);
    Edge wall;
    wall.status=WALL_EXISTS;
    Edge nowall;
    nowall.status = WALL_DNE;

    aMaze.setEdge(0,1,nowall);
    aMaze.setEdge(1,2,nowall);
    aMaze.setEdge(3,4,nowall);
    aMaze.setEdge(4,5,nowall);
    aMaze.setEdge(6,7,nowall);
    aMaze.setEdge(7,8,nowall);
    aMaze.setEdge(0,3,nowall);
    aMaze.setEdge(3,6,nowall);
    aMaze.setEdge(1,4,nowall);
    aMaze.setEdge(4,7,nowall);
    aMaze.setEdge(2,5,nowall);
    aMaze.setEdge(5,8,nowall); 
    int dest=-1;
    int finish=0;
    int i=0;
    int start;
      bool rightWall=false;
      bool leftWall=false;
      bool frontWall=false;
      bool turnAround=false;

    while(aMaze.getPathExplore(pos).size() != 0)
    {
cout<<"Next Move: "<<endl;
      if(i==0)
      {
        start = pos;
        aMaze.setStartVertex(pos);
      }
      if(pos == finish)
      {
        pullUpDnControl(END_LED, PUD_UP);
      }
      rightWall = false;
      leftWall = false;
      frontWall = false;
      turnAround = false;

      if(aMaze.getVertex(pos).visited != true)
{
      aMaze.getVertex(pos).visited=true;

      int CMLeft=getCMLeft();
      int CMFront = getCMFront();
      int CMRight = getCMRight();

      if(CMLeft<FOOT_CM&&CMLeft!=0) //setting edges
      { 

        if(direction=="north" &&(pos+west) >= 0 )
        { 
          aMaze.setEdge(pos,pos+west,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+west+1<<endl;
        }
        else if(direction=="east" && (pos+north) >= (0) )
        {
          aMaze.setEdge(pos,pos+north,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+north+1<<endl;
        }
        else if(direction=="south" && (pos+east) < (size*size) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+east+1<<endl;
        }
        else if(direction=="west" && (pos+south) < (size*size) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
        }

        leftWall=true;
      }
      if(CMFront<FOOT_CM&&CMFront!=0)
      {
      

        if(direction=="north" && (pos+north) >= (0) )
        {
          aMaze.setEdge(pos,pos+north,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+north+1<<endl;
        }
        else if(direction=="east" && (pos+east) < (size*size) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+east+1<<endl;
        }
        else if(direction=="south" && (pos+south) < (size*size) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
        }
        else if(direction=="west" && (pos+west) >= 0 )
        {
          aMaze.setEdge(pos,pos+west,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+west+1<<endl;
        }

        frontWall=true;
      }
      if(CMRight<FOOT_CM&&CMRight!=0)
      {
        if(direction=="north" && (pos+east) < (size*size) )
        {
          aMaze.setEdge(pos,pos+east,wall);
          cout<<"set edge between "<<pos+1<<", "<<pos+east+1<<endl;
        }
        else if(direction=="east" && (pos+south) < (size*size) )
        {
          aMaze.setEdge(pos,pos+south,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+south+1<<endl;
        }
        else if(direction=="south" && (pos+west) >= 0 )
        {
          aMaze.setEdge(pos,pos+west,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+west+1<<endl;
        }
        else if(direction=="west" && (pos+north) > 0 )
        {
          aMaze.setEdge(pos,pos+north,wall);
          cout<<"set edge between"<<pos+1<<", "<<pos+north+1<<endl;
        }


        rightWall=true;
      }
      if(leftWall&&rightWall&&frontWall)
      {
        turnAround=true;
        cout<<"Turn Around!"<<endl;
      }
}
     cout<<"Position: "<<pos+1<<endl;
    int c;
    //Output OCR result
     if( frontWall )
     {
      cout << "Take Picture" << endl; 
      cin >> c;
      get_OCR(debug, dummy);
     }

    if( leftWall )
    {
     cout << "Turn Left and Take Picture" << endl;
     cin >> c;
     get_OCR(debug, dummy);
     cout << "Center" << endl;
    }

    if( rightWall )
    {
     cout << "Turn Right and Take Picture" << endl;
     cin >> c;
     get_OCR(debug, dummy);
     cout << "Center" << endl;
    }
              
      vector<int> path = aMaze.getPathExplore(pos);
      
      cout<<"{";
      for(vector<int>::iterator it= path.begin(); it != path.end(); it++)
      {
        cout<<*it<<",";
      }
      cout<<"}";
      dest=path[1]-pos;
      pos=path[1];
      if(dest==north) //determining next move
      { 
        if(direction=="north")
        {
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="north";
      }
      else if(dest==east)
      {
        if(direction=="north")
        {
          cout<<"Turn Right"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="east";
      }
      else if(dest==south)
      {
        if(direction=="north")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="south";
      }
      else//west
      {
        if(direction=="north")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Move Forward"<<endl;
        }

        direction="west";
      }
cout<<"Enter Number Here: ";
cin>>c;
      i++;
      cout<<i<<endl;
    }
vector<int> path=aMaze.getPath(start, finish);
      cout<<"{";
      for(vector<int>::iterator it= path.begin(); it != path.end(); it++)
      {
        cout<<*it<<",";
      }
      cout<<"}";
direction = "north";
pos=start;
for(int j=1; j<path.size(); j++)
{

dest=path[j]-pos;
pos=path[j];
      if(dest==north) //Get Shortest/Critical Path
      {
        if(direction=="north")
        {
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="north";
      }
      else if(dest==east)
      {
        if(direction=="north")
        {
          cout<<"Turn Right"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="east";
      }
      else if(dest==south)
      {
        if(direction=="north")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }

        direction="south";
      }
      else//west
      {
        if(direction=="north")
        {
          cout<<"Turn Left"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="east")
        {
          cout<<"Turn Around"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else if(direction=="south")
        {
          cout<<"Turn Right"<<endl;
          cout<<"Move Forward"<<endl;
        }
        else 
        {
          cout<<"Move Forward"<<endl;
        }

        direction="west";
      }

}
  }

  return 0;
}
