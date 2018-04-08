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

        //***************************************************//
        //     This is where we setup our OSCQuery Server:   //
        ofxOscQueryServer server; //We will set it up in ofApp.cpp
        //***************************************************//
};
