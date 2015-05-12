#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <raspicam/raspicam_cv.h>
#include <raspicam/raspicam.h>
#include <raspicam/raspicamtypes.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <stdlib.h>
#include "sonar.h"
using namespace std;
using namespace cv;
using namespace raspicam;

void changeDirection(string &direction, string turn, int &pos)
{
  int west=-1;
  int north=-7;
  int south=7;
  int east=1;
  if(turn=="forward")
  {
    if(direction=="west")
    {
      pos+=west;
    }
    else if(direction=="east")
    {
      pos+=east;
    }
    else if(direction=="north")
    {
      pos+=north;
    }
    else if(direction=="south")
    {
      pos+=south;
    }
  }
  else if(direction=="north"&&turn=="left")
  {
    direction="west";
    pos+=west;
  }
  else if(direction=="north"&&turn=="right")
  {
    direction="east";
    pos+=east;
  }
  else if(direction=="north"&&turn=="around")
  {
    direction="south";
    pos+=south;
  }
  else if(direction=="east"&&turn=="left")
  {
    direction="north";
    pos+=north;
  }
  else if(direction=="east"&&turn=="right")
  {
    direction="south";
    pos+=south;
  }
  else if(direction=="east"&&turn=="around")
  {
    direction="west";
    pos+=west;
  }
  else if(direction=="south"&&turn=="left")
  {
    direction="east";
    pos+=east;
  }
  else if(direction=="south"&&turn=="right")
  {
    direction="west";
    pos+=west;
  }
  else if(direction=="south"&&turn=="around")
  {
    direction="north";
    pos+=north;
  }
  else if(direction=="west"&&turn=="left")
  {
    direction="south";
    pos+=south;
  }
  else if(direction=="west"&&turn=="right")
  {
    direction="north";
    pos+=north;
  }
  else if(direction=="west"&&turn=="around")
  {
    direction="east";
    pos+=east;
  }

  return;
}
int readNum(string file)
{
  int num;
  int i;
  ifstream f;
  f.open(file.c_str());
  f>>num;
  f.close();
  ofstream of;
  i=num;
  num++;
  if(num==3)
  {
    num=1;
  }
  of.open(file.c_str()); 
  of<<num;
  of.close();
  return i;
}
void GetCameraImage(Mat & mat)
{
/*  RaspiCam cam;
  cam.setAWB(RASPICAM_AWB_FLUORESCENT); 
  //cam.setImageEffect(RASPICAM_IMAGE_EFFECT_NEGATIVE);
  cam.setFormat(RASPICAM_FORMAT_BGR);
//  cam.setAWB(RASPICAM_AWB_OFF);
  cam.setImageEffect(RASPICAM_IMAGE_EFFECT_COLORBALANCE);
  cam.setExposure(RASPICAM_EXPOSURE_SPORTS);
  cam.setContrast(800);
  cam.setSaturation(100);
  if(!cam.open())
  {
    cout<<"error opening camera"<<endl;
  }
  else
  {
    sleep(1.5);
    cam.grab();
    unsigned char *data=new unsigned char[cam.getImageTypeSize(RASPICAM_FORMAT_BGR)];
    cam.retrieve(data, RASPICAM_FORMAT_BGR);
    //data = cam.getImageBufferData();
    mat=Mat(cam.getHeight(),cam.getWidth(),CV_8UC3,data);
    transpose(mat,mat);
    cout << "Matrix transposed" <<endl;

    flip(mat,mat,1);
    cout << "Matrix transposed" <<endl;
    
    cam.release();
  }*/
  RaspiCam_Cv camera;
  camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );

  cout << "Opening Camera..." << endl;

  if (!camera.open())
  {
    cerr<<"Error opening camera"<<endl;
  }
  else
  {

    //wait a while until camera stabilizes
    cout << "Sleeping for 0.1 sec" << endl;

    sleep(1);

    cout << "Capture" << endl;

    //capture
    camera.grab();
    cout << "Camera Grab Complete" <<endl;

    camera.retrieve(mat);
    cout << "Matrix Retrieved" <<endl;

    transpose(mat,mat);
    cout << "Matrix transposed" <<endl;

    flip(mat,mat,1);
    cout << "Matrix transposed" <<endl;

  }

  return;
}

// 0 for red channel, 1 for green channel, 2 for blue channel
Mat ColorFilter(Mat mat, const int& color)
{
  Mat channel[3];
  split(mat, channel);
  //Filters out blue and green
  if(color==0)
  {
    //set blue channel to zero
    channel[0]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    //set green filter to zero
    channel[1]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    merge(channel, 3, mat);
  }
  //Filters out red and blue
  else if(color==1)
  { 
    //set blue channel to zero
    channel[0]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    //set red filter to zero
    channel[2]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    merge(channel, 3, mat);

  }
  //Filters out red and green
  else if(color==2)
  {
    //set green channel to zero
    channel[1]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    //set red filter to zero
    channel[2]=Mat::zeros(mat.rows, mat.cols, CV_8UC1);
    merge(channel, 3, mat);
  }
  else
  {
    cout<<"color must be 0, 1, or 2"<<endl;
  }

  return mat;
}


Mat CannyThreshold(Mat &src, int lowThresh, int highThresh)
{
  Mat src_gray;
  Mat detected_edges, blurred;
  int ksize =3;
  int iKey=0;
  int min=8000;
  int max=100000;
  float min_ratio =0.1667;
  float max_ratio=0.5;
  cout<<"width "<<src.rows<<endl;
  cout<<"height "<<src.cols<<endl;
  int hRatio = 1;
  int wRatio = 1;
  int Ratio = ((src.rows/src.cols)*1000);
  int poly=0;
  Mat dst;
  namedWindow("source",1);
  imshow("source",src);
  createTrackbar("lowThresh:","source", &lowThresh, 255);
  createTrackbar("highThresh:","source", &highThresh, 255);
  createTrackbar("HeightSize:","source", &hRatio, Ratio);
  createTrackbar("WidthSize:","source", &wRatio, Ratio);

  while(iKey!=27)
  {
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur(src_gray, blurred, Size(ksize,ksize));
  Canny(blurred, detected_edges, lowThresh, highThresh,ksize);
  vector<vector<Point> > contours ;
  vector<Vec4i> hierarchy;
  findContours(detected_edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
  vector<Point> approx;
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect;
  float fwRatio = static_cast<float>(wRatio);
  float fhRatio = static_cast<float>(hRatio);
  dst = Mat::zeros( detected_edges.size(), CV_8UC3 );

 // for(int i=0; i< contours.size(); i++)
  //{
 //   drawContours( dst, contours, i, Scalar(57, 83, 158), 2, 8, hierarchy, 0, Point() );
  //}
  for( int i = 0; i < contours.size(); i++ )
     {
      Mat tmp = detected_edges.clone();

       approxPolyDP( Mat(contours[i]), contours_poly[i], poly, true );
       //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    Rect boundingR=boundingRect( Mat(contours_poly[i]) );
    int height = hRatio*tmp.cols*.001;
    int width = wRatio*tmp.rows*.001;
    
    Point tl=Point(tmp.rows*0.5-width,tmp.rows*0.5+width);
    Point br=Point(tmp.cols*0.5-height,tmp.cols*0.5+height);
    Rect r = Rect(tl,br);
   // cout<<"size "<<r.area()<<endl;
    tmp=tmp(r);
        if(boundingR.area()< max 
    && boundingR.area()>min)
    {
      boundRect.push_back(boundingR );
    }

     }
  for( int i = 0; i< contours.size(); i++ )
     {
      // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( dst, contours_poly, i, Scalar(58, 28, 167), 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( dst, boundRect[i].tl(), boundRect[i].br(), Scalar(178, 83, 28), 2, 8, 0 );
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }
    if(boundRect.size()>0)
    {
    imshow("source",dst);
    iKey=waitKey(50);
   }
  }
  return dst;
}

void RGBFilter(Mat mat)
{
  string window = "Image";
  namedWindow(window, 1);
  imshow(window, mat);
  int lb=0;
  int hb=255;
  int lg=0;
  int hg=255;
  int lr=0;
  int hr=255;
  int iKey=0;
  createTrackbar("lb",window,&lb,255);
  createTrackbar("lg",window,&lg,255);
  createTrackbar("lr",window,&lr,255);
  createTrackbar("hb",window,&hb,255);
  createTrackbar("hg",window,&hg,255);
  createTrackbar("hr",window,&hr,255);

  while(iKey!=27)
  {
    Mat filterMat;
    iKey=waitKey(50);
    inRange(mat, Scalar(lb,lg,lr), Scalar(hb,hg,hr), filterMat);
    imshow(window, filterMat);
  }

  return;
}


bool FindAndCrop(Mat & filterMat, const bool debug)
{
  int ksize = 3;
  int lowThresh = 20;
  int highThresh = 50;
  const int EXPAND=0.5;
  const int MIN_AREA=8000;
  const int MAX_AREA=20000;
  const int POLYDP=0;
  const float MINWIDTH_RATIO =0.1667;
  const float MAXWIDTH_RATIO=1.1;
  const float MINHEIGHT_RATIO=0.1667;
  const float MAXHEIGHT_RATIO=1.1;
  bool isAsterisk=false;
  Mat detected_edges;
  cout<<"blurring image"<<endl;
  blur(filterMat, filterMat, Size(ksize,ksize));
  cout<<"Canny edge detection"<<endl;
  Canny(filterMat, detected_edges, lowThresh, highThresh,ksize);
  vector<vector<Point> > contours ;
  vector<Vec4i> hierarchy;
  cout<<"Finding contours"<<endl;
  findContours(detected_edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
  hierarchy.clear();
  vector<Point> approx;
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect;
  Mat dst = Mat::zeros( detected_edges.size(), CV_8UC3 );
  detected_edges.release();
  cout<<"Approximating polygons and bounding rectangles"<<endl;
  for( int i = 0; i < contours.size(); i++ )
  {
    approxPolyDP( Mat(contours[i]), contours_poly[i], POLYDP, true );
    Rect boundingR=boundingRect( Mat(contours_poly[i]) );
    if(boundingR.area()< MAX_AREA
    && boundingR.area()>MIN_AREA
    && boundingR.height>boundingR.width*MINWIDTH_RATIO
    && boundingR.width<boundingR.height*MAXWIDTH_RATIO)
    {
      boundRect.push_back(boundingR );
    }
  }
  if(debug)
  {
  for( int i = 0; i< contours.size(); i++ )
  {
    drawContours( dst, contours_poly, i, Scalar(58, 28, 167), 1, 8, vector<Vec4i>(), 0, Point() );
  }
  }
  contours_poly.clear();
  contours.clear();
  int min=filterMat.rows+filterMat.cols;
  int index=0;
  Rect rec;
  //Finding max area rectangle
  int cenY=filterMat.rows*0.5;
  int cenX=filterMat.cols*0.5;
  int centroid = cenY + cenX;
  cout<<"Finding max area for bounding rectangles"<<endl;
  for(int i=0; i<boundRect.size(); i++)
  {
    int tcentroid = boundRect[i].tl().x*0.5+boundRect[i].tl().y*0.5;
    int result = centroid - tcentroid;
    //absolute value
    result < 0 ? result*=-1 : result*=1;
    if(result<min)
    {
      min=result;
      index=i;
    }
  }
  if(boundRect.size()>0)
  {
  //  cout<<"Size of rec "<<boundRect[index].area()<<endl;
  //  cout<<"Width "<<boundRect[index].width<<" height "<<boundRect[index].height<<endl;
    rec=boundRect[index];
    //Checking for asterisk
    float width = static_cast<float>(rec.width);
    float height = static_cast<float>(rec.height);
    float hwratio = height/width;
    float rarea = rec.area()/11130;
    if((hwratio<=1.11)&&(hwratio>=0.91)
        &&(rarea<=1.11&&rarea>=0.91))
    {
      isAsterisk=true;
    }
  
    int ntlx = rec.tl().x-(rec.tl().x*EXPAND)*0.5;
    int ntly = rec.tl().y - (rec.tl().y*EXPAND)*0.5;
    int nbrx = rec.br().x + (rec.br().x*EXPAND)*0.5;
    int nbry = rec.br().y + (rec.br().x*EXPAND)*0.5;
    if((ntlx>=0)&&(ntly>=0)&&(nbrx<filterMat.cols)&&(nbry<filterMat.rows))
    {
      Point ntl = Point(ntlx, ntly);
      Point nbr = Point(nbrx, nbry);
      rec=Rect(ntl,nbr);
    }
    
  boundRect.clear();

  if(debug)
  {
    char i;
    string window="asdf";
    rectangle(dst, rec.tl(), rec.br(), Scalar(178,183,228), 2, 8 ,0);
    namedWindow(window, 1);
    namedWindow("cropped",1);
    if(dst.empty())
    {
      cout<<"Empty Image"<<endl;
    }
    else
    {
      imshow(window,dst);
      cout<<"IM NOT EMPTY"<<endl;
    }
    cout<<"input2"<<endl;
    cin>>i;
  }
  cout<<"ITS ABOUT TO DIE"<<endl;
  if(rec.area()>0)
  {
    filterMat = filterMat(rec);
    if(debug)
    {
      imshow("cropped",filterMat);
      waitKey();
    }
    dst.release();

  }
  }
  return isAsterisk;
}


bool get_OCR( const bool debug, bool & isGreen)
{
  Mat mat;
  GetCameraImage(mat);
  Point tl = Point(0.2*mat.cols,0.2*mat.rows);
  Point br = Point(0.75*mat.cols,0.6*mat.rows);
  Rect r = Rect(tl,br);
  mat = mat(r);
  Mat m_green, m_red, m_blue, CroppedImage;  
  bool isChar=false;
  bool isAsterisk=false;
    //Green
    int lb=0;   //Low Blue
    int lg=110;   //Low Green
    int lr =0;   //Low Red
    int hb=45;  //High Blue
    int hg=155;  //High Green
    int hr =54;  //High Red
    inRange(mat, Scalar(lb,lg,lr), Scalar(hb,hg,hr), m_green);
    //Red
    lb = 70;
    lg = 50;
    lr = 180;
    hb = 135;
    hg = 135;
    hr = 255;
    inRange(mat, Scalar(lb,lg,lr), Scalar(hb,hg,hr), m_red);
    //Blue
    lb = 60;
    lg = 60;
    lr = 60;
    hb = 125;
    hg = 125;
    hr = 165;
    inRange(mat, Scalar(lb,lg,lr), Scalar(hb,hg,hr), m_blue);
if(debug)
{
    imshow("green", m_green);
    waitKey();
    imshow("blue", m_blue);
    waitKey();
    imshow("red", m_red);
    waitKey();
}
    int nonZeroGreen = countNonZero(m_green);
    int nonZeroRed = countNonZero(m_red);
    int nonZeroBlue = countNonZero(m_blue);
    string color="";
    cout << "Checking Color" << endl;
    if((nonZeroGreen>nonZeroRed)&&(nonZeroGreen>nonZeroBlue))
    {//Green
     //FindAndCrop(m_green);
      color="Green";
     cout << color << endl; 
      isChar=false;
      isGreen=true;
    }
    else if(nonZeroRed>nonZeroBlue)
    {//Red
      color="Red";
      cout << color << endl;
      isAsterisk=FindAndCrop(m_red, debug);
      cout<<"FIND AN CROP COMPLETE "<<endl;
    }
    else
    {//Blue
      color="Blue";
      cout << color << endl;
      isAsterisk=FindAndCrop(m_blue, debug);
      cout<<"FIND AN CROP COMPLETE "<<endl;
    }

   // imshow("asdf2", CroppedImage);
//    waitKey();
    if(color=="Red")
    {
      CroppedImage=m_red;
    }
    else if(color=="Blue")
    {
      CroppedImage=m_blue;
    }
    if(isAsterisk)
    {
      cout<<"OCR Output : "<<"*"<<endl;
    }
    if(!(CroppedImage.empty())&&(color!="Green")&&(!isAsterisk))
    { 

      char *outText;

      tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
      // Initialize tesseract-ocr with English, without specifying tessdata path
      if (api->Init("/usr/local/share/", "eng")) {
          fprintf(stderr, "Could not initialize tesseract.\n");
          exit(1);
      }

      if(color=="Red")
        api->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()");
      else if(color=="Blue")
        api->SetVariable("tessedit_char_whitelist", "0123456789!@#$%^&*()");
      api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
      api->SetVariable("tessedit_pageseg_mode", "char");
      api->SetImage((uchar*)CroppedImage.data, CroppedImage.size().width, 
      CroppedImage.size().height, CroppedImage.channels(), CroppedImage.step1());
      api->Recognize(0);
      // Get OCR result
      outText = api->GetUTF8Text();
      if(outText!="")
      {
        cout<<"OCR output:\n"<< outText<<endl;
        isChar=true;
      }
      else
      {
        isChar=false;
      }
      // Destroy used object and release memory
      api->End();
      delete [] outText;
  }
  return isChar;
}
