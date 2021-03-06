#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscQueryServer.h"
#include "CirclesRenderer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		
		void vSyncChanged(bool & vSync);

		ofxPanel gui;
		ofParameter<bool> vSync;
		ofParameterGroup parameters;
		ofXml settings;
		CirclesRenderer renderer1,renderer2;
		ofTrueTypeFont font;

        //*****************************************************//
        //     This is where we declare our OSCQuery Servers   //
    
        ofxOscQueryServer server1{1221, 4617};
        ofxOscQueryServer server2{1222, 4616};
    
        // Notice that we need to declare ports to our servers //
        // or they would get the same default ports            //
        // and would conflict with each other.                 //
        // We will attach them to ofParameterGroups in ofApp.cpp
        //*****************************************************//
};
