#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  renderer1.setup("renderer.1");
  renderer2.setup("renderer.2");
  // Note that both Groups are named the same, in order to demonstrate
  // ossia's behavior in the case of duplicated nodes (adds .1 to the name)

  parameters.setName("settings");
  parameters.add(vSync.set("vSync",true));
  parameters.add(renderer1.parameters);
  parameters.add(renderer2.parameters);

  gui.setup(parameters);


  //*************************************************//
  //   This is where we setup our OSCQuery Server:   //
  // NB this is the only change from of's gui/ParameterGroupExample
  server.setup(parameters, 1233, 4677, "ofxOscQueryDemo");
  // i.e. this will create an OSCquery server from 'parameters' ParameterGroup
  // scan all sub-ParameterGroups and children Parameters recursively
  // and expose the whole thing to ports 1233 for OSC and 4677 for Websocket
  // All network communication (sending, receving, listening, queries and replies)
  // will then be managed by the internal implementation of libossia
  //*************************************************//

  server.findNode("/renderer.1/color").setUnit("color.rgba");
  server.findNode("/renderer.1/position").setUnit("position.cart2D"); // The cannonnical way to set the unit for a 2D position
  server.findNode(renderer2.position).setUnit("xy")                   // A shorthand way to do the same
                                     .setDescription("A circle renderer position") // More attributes can be added
                                     .setTags({"a little tag", "some other tag", "another tag"});

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
    settings.serialize(parameters);
    settings.save("settings.xml");
  }
  if(key=='l'){
    settings.load("settings.xml");
    settings.deserialize(parameters);
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
