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
        //     This is where we declare our OSCQuery Servers     //
        ofxOscQueryServer server1{"Renderer1", 1234, 4677};
        ofxOscQueryServer server2{"Renderer2", 1233, 4676};
        // Notice that we need to declare names and ports,
        // or they would get the same default names and ports
        // and conflict with each other. 
        // We will attach them to ofParameterGRoups in ofApp.cpp
        //*****************************************************//
};
