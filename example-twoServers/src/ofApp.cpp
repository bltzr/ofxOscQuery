#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  renderer1.setup("renderer1");
  renderer2.setup("renderer2");

  parameters.setName("settings");
  parameters.add(vSync.set("vSync",true));
  parameters.add(renderer1.parameters);
  parameters.add(renderer2.parameters);

  gui.setup(parameters);
    
    std::cout << "group names: " << renderer1.parameters.getName() << " / " << renderer2.parameters.getName() << std::endl;


  //*************************************************//
  //   This is where we setup our OSCQuery Servers:   //
  server1.setup(renderer1.parameters);
  server2.setup(renderer2.parameters);
  // by attaching them to ofParaemeterGroups
  //*************************************************//
  // NB this is the only change with of's ParameterGroupExample
  //

  cout << server1["/position/"].getName();
  cout << server1["/position"].getName();
  cout << server2["/color"].getName();



  gui.loadFromFile("settings.xml");

  font.load( OF_TTF_SANS,9,true,true);
  ofEnableAlphaBlending();
}

void ofApp::vSyncChanged(bool & vSync){
  ofSetVerticalSync(vSync);
}


//--------------------------------------------------------------
void ofApp::update(){
  // frameNum is a readonly parameter so this will fail to compile
  // unless we are inside the CirclesRenderer class
  // renderer.frameNum = 5;
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackgroundGradient(ofColor::white, ofColor::gray);
  renderer1.draw();
  renderer2.draw();
  ofSetColor(255);
  gui.draw();
  font.drawString("frame: " + ofToString(renderer1.frameNum),ofGetWidth()-150,20);
  font.drawString("fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-150,40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key=='s'){
    ofSerialize(settings, parameters);
    settings.save("settings.xml");
  }
  if(key=='l'){
    settings.load("settings.xml");
    ofSerialize(settings, parameters);
  }
  if(key=='o'){
    cout << renderer1.parameters;
    cout << renderer2.parameters;
  }
  if(key=='r'){
    renderer1.color = ofColor(127);
    renderer2.color = ofColor(127);
  }
}
