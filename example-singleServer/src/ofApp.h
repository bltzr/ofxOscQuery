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
        void exit();

		void keyPressed  (int key);
		
		void vSyncChanged(bool & vSync);

		ofxPanel gui;
		ofParameter<bool> vSync;
		ofParameterGroup parameters;
		ofXml settings;
		CirclesRenderer renderer1,renderer2;
		ofTrueTypeFont font;

    //***********************************************************//
    //     This is where we declare our OSCQuery Server:         //
                                                                 //
            ofxOscQueryServer oscQuery;                            //
                                                                 //
    // Here, we use the default name and ports, but we could     //
    // specify them (see example-twoservers for that)            //
    //***********************************************************//
};
