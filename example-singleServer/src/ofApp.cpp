#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  renderer1.setup("renderer");
  renderer2.setup("renderer");
  // Note that both Groups are named the same, in order to demonstrate
  // ossia's behavior in the case of duplicated nodes (adds .1 to the name)

  //parameters.setName("myOfxOscQueryApp"); // we remove this one so we get the default name
  // uncommenting it will name both the ofParameterGroup and the device with the provided string
  parameters.add(vSync.set("vSync",true));
  vSync.addListener(this, &ofApp::vSyncChanged);
  parameters.add(renderer1.parameters);
  parameters.add(renderer2.parameters);

  gui.setup(parameters);

  // ^ All this above is just the same that we have in of's gui/ParameterGroupExample
  // i.e. one does not have to change their ofApp structure to use ofxOscQuery,
  // as long as it uses ofParameters and ofParameterGroups
  //*********************************************************************************//



  //******************************************************************************************//
  //   This is where we setup our OSCQuery Server:                                            //
  // NB this is the only change from of's gui/ParameterGroupExample                           //
                                                                                              //
        server.setup(parameters);                                                             //
                                                                                              //
  // i.e. this will attach our OSCquery server to the 'parameters' ParameterGroup             //
  // scan all sub-ParameterGroups and children Parameters recursively                         //
  // and expose the whole thing to default ports 1234 for OSC and 4567 for Websocket          //
  //                                                                                          //
  // Should we want to set specific ports and name, we could add them as arguments, e.g.:     //
                                                                                              //
       // server.setup(parameters, 4321, 8765, "myFirstOscQueryServer");                      //
                                                                                              //
  // All network communication (sending, receving, listening, queries and replies)            //
  // will then be managed by the internal implementation of libossia                          //
  //******************************************************************************************//



  //*********************************************************************************//
  // Some extra OSCQuery/ossia-specific attributes can be set later on
  // They requires that we find the node for which we want to set those attributes
  // This can be achieved with the findNode method, which accepts as argument:
  // - the object name ofParameter(Group) to which the ossia node is attached
  // - the OSC path of the node we want to access
  // Then we have just to use the appropriate set(Attribute) methods
  //
  // Add a description of this node
  server.findNode("/renderer/color/").setDescription("El Color maravilloso!!");
    
  // The cannonnical way to set the unit for a 2D position
  server.findNode("/renderer/position").setUnit("position.cart2D");
  // A shorthand way to do the same:
  server.findNode(renderer2.position).setUnit("point2d");

  // As they return a ofxOssiaNode&, the set(Attribute) methods
  // can be "cascaded" as demonstrateed below:
  server.findNode(renderer2.position)
        .setDescription("A circle renderer position")
        .setTags({"a little tag", "some other tag", "another tag"})
        .setMin(ofVec2f{2,10});
    
  // Bound values (aka Min or Max can be set, and we can decide what happens at the bounds:
  server.findNode(renderer2.number).setMin(5).setMax(15)
                                   .setBound("FOLD");
  // possible values are: FREE, CLIP, LOW, HIGH, WRAP and FOLD,
  // see: https://ossia.github.io/?cpp--98#bounding-mode
    
  // Retrieve the value of the minimum bound for a node
  ofLogNotice() << "Renderer2's number's min: "  << server.findNode(renderer2.number).getMin<int>();
  
  // We can also provide a set of accepted values:
  server.findNode(renderer1.number).setValues(std::vector<int>{1,3,5,7,9,11,13,15,17,19})
                                   .setBound("CLIP");
  // and restrict the parameter's value to those with the CLIP boundmode
    
  //*********************************************************************************//



  //*********************************************************************************//
  // v The rest below is the usual stuff from the example


  gui.loadFromFile("settings.xml");

  font.load( OF_TTF_SANS,9,true,true);
  ofEnableAlphaBlending();
}

void ofApp::vSyncChanged(bool & vSync){
  ofSetVerticalSync(vSync);
  ofLogNotice() << "vSync changed: " << vSync;
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

void ofApp::exit(){
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key=='s'){
    ofSerialize(settings,parameters);
    settings.save("settings.xml");
  }
  if(key=='l'){
    settings.load("settings.xml");
    ofDeserialize(settings, parameters);
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
