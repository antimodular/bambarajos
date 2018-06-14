#pragma once

#include "ofMain.h"
//#include "goThreadedVideo.h"


#include "ofxOsc.h"
#include "ofxGui.h"

#include "ofEvents.h"

//OSC sender
#define masterIP "localhost" //"192.168.100.100"
#define remotePORT 44999
//OSC receiver 
#define NUM_MSG_STRINGS 20
#define localPORT 11999


#define maxVideos 2400
#define maxVideoType 800
#define numberOfVideos 10

#define soundVolumeMax 1.25

class ofApp : public ofBaseApp{

    string version;
    
	public:
		void setup();
		void update();
		void draw();
	void exit();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	void OSCupdate();
	
	void startup();
	int convertStringToSeconds(string s);
	
	void triggerNewVideo();
	
    void loaded(); //string & path);
//        void error(goVideoError & err);

	ofTrueTypeFont  franklinBook;
	ofTrueTypeFont franklinBookSmall;
	float textX2;
	
		//goThreadedVideo * videos;
	ofVideoPlayer videos[numberOfVideos];
	int indexToUsedVideos[numberOfVideos];
	
	string videoNames[2400]; // names of all video files
	string usedVideoNames[2400]; //names of the files that we will be working with
	int	kissFrameNum[2400];
//	int loopCount[2400];
	int videoPlayOrder[2400];
	
		
		int numberOfVideosLoaded;
		int currentlyLoadingVideo; //cicles through numOfMovies 0 - 10

	int playOrderCnt; //cicles through maxVideos 0 -2400
	
	bool initDone;
	int initStage;
	
	int gayCount, lesbianCount,straightCount;
	int gayLimit,lesbianLimit,straightLimit;
	ofParameter<float> gayAmount,lesbianAmount,straightAmount;
	float old_gayAmount,old_lesbianAmount,old_straightAmount;
	
	int amtOfVideoFiles;
	int amtOfTextFiles;
	ofDirectory videoDir;
	ofDirectory textDir;
	//int newLoadingVideo;
	
	int usedVideoAmount;
	
	int movieAmt;
	int currentVideo;
	bool bLoadNewVideo, bLoadNewVideoDone;
	
	bool jumpToNext;
	int jumpWaitTimer;
	ofParameter<int> jumWaitTime;
	
	ofParameter<bool> wantHat, wantKick, wantSnare, wantOnset;
	bool oldWantOnset, oldWantKick, oldWantSnare, oldWantHat;
	
	ofParameter<bool> isOnset, isKick, isSnare, isHat;
	long lastOnsetTime, lastKickTime, lastSnareTime, lastHatTime;
    
	ofxPanel gui_main;
    ofxPanel gui_kisses;
    ofxPanel gui_detection;
    ofxPanel gui_distribution;
    
	ofParameter<bool> bShowGui;
	ofParameter<bool> showFullscreen;
	
    ofParameter<float> soundVolume;
    float old_soundVolume;
	
	ofParameter<bool> allSame;
	
	bool mouseDoublePressed;
	unsigned long lastTap;
	
	bool keyIsDown[255];
	bool showSensitivity;
	float showSensitivityTimer;

	ofParameter<bool> exitApp;
	
	int actionMode;
	float showActionModeTimer;
	bool showActionMode;
	bool actionMode2JumpTriggered;
	
    ofParameter<bool> bUseSound;
    bool old_bUseSound;
    ofParameter<bool> bUseKeyTrigger;
    
    float autoTriggerTimer;
    ofParameter<float> autoTriggerDuration;
    ofParameter<bool>     x_keyTriggered;
    
    bool bCheckLoading;
    
private:
	ofxOscReceiver	receiver;
	ofxOscSender sender;
	
	int				current_msg_string;
	string		msg_strings[NUM_MSG_STRINGS];
	float			timers[NUM_MSG_STRINGS];
    
    void checkGui();
};
