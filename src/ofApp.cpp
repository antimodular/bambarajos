#include "ofApp.h"

//set development target to OSX 10.6 for it to compile well

//get all text file names
//see if mov counter part exists

//shuffel text file name list

//make list with correct distribution of g l s, movie name
//make list of kiss startFrame
//make list with index to each video


//if the maxMovie number is smaller then the amount of files in folder
//then i need to check if the accumulated amount is not exceeding the maxMovie number

//if i want a proper startup screen i might have to change all the for loops to counters


//load 10 movies

/*
 cd + drag folder to terminal
 make data folder invisible via terminal  mv "Existing Folder" ".ExistingFolder"
 or
 keep data folder inside app content folder
 
 */


//--------------------------------------------------------------
void ofApp::setup(){
	
//    version = "Bambarajos17";
    
	// ofSetLogLevel(OF_LOG_VERBOSE);
	
	//osx
	//ofSetDataPathRoot("../../../.data/"); //hidden data folder in regular place
//    ofSetDataPathRoot("../../data/"); //data folder placed inside app
	//ofSetDataPathRoot("../../rlh/j/t/o/data/"); //data folder placed inside app
	//windows
	//ofSetDataPathRoot(".data/");
	
    ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
    //-----------GUI --------
    int gui_x = 10;
    int gui_y = 150;
    int gui_w = 210;
    int gui_h = 350;
    
    gui_main.setup();
    gui_main.setHeaderBackgroundColor(ofColor(255,0,0));
    gui_main.setName("Bambarajos");
    gui_main.setPosition(gui_x,gui_y);
    gui_main.add(bShowGui.set("bShowGui",false));
    gui_main.add(showFullscreen.set("showFullscreen",true));
    gui_main.add(exitApp.set("exitApp",false));
    
    gui_main.loadFromFile("gui_main.xml");
    bShowGui = false;
    showFullscreen = true;
    exitApp = false;
    
    gui_kisses.setup();
    gui_kisses.setHeaderBackgroundColor(ofColor(255,0,0));
    gui_kisses.setName("kisses");
    gui_kisses.setPosition(gui_x,gui_main.getPosition().y+gui_main.getHeight());
    gui_kisses.add(bUseSound.set("useSound",true));
    gui_kisses.add(soundVolume.set("sensitivity",1,0,soundVolumeMax));
    gui_kisses.add(autoTriggerDuration.set("autoTriggerDuration",200,0,600));
    gui_kisses.loadFromFile("gui_kisses.xml");
    
    
    gui_detection.setup();
    gui_detection.setHeaderBackgroundColor(ofColor(255,0,0));
    gui_detection.setName("detection");
    gui_detection.setPosition(gui_x,gui_kisses.getPosition().y+gui_kisses.getHeight());
    
    gui_detection.add(wantOnset.set("wantOnset",true));
    gui_detection.add(wantKick.set("wantKick",true));
    gui_detection.add(wantSnare.set("wantSnare",true));
    gui_detection.add(wantHat.set("wantHat",true));
    gui_detection.add(isOnset.set("isOnset",false));
    gui_detection.add(isKick.set("isKick",false));
    gui_detection.add(isSnare.set("isSnare",false));
    gui_detection.add(isHat.set("isHat",false));
    
    gui_detection.add(jumWaitTime.set("jumWaitTime",1000,0,5000));
    gui_detection.add(x_keyTriggered.set("xKeyTriggered",false));
    
    gui_detection.loadFromFile("gui_detection.xml");
    x_keyTriggered = false;
    
    gui_distribution.setup();
    gui_distribution.setHeaderBackgroundColor(ofColor(255,0,0));
    gui_distribution.setName("distribution");
    gui_distribution.setPosition(gui_x,gui_detection.getPosition().y+gui_detection.getHeight());
    gui_distribution.add(allSame.set("even",false));
    gui_distribution.add(gayAmount.set("gay",33,0,100));
    gui_distribution.add(lesbianAmount.set("lesbian",33,0,100));
    gui_distribution.add(straightAmount.set("straight",33,0,100));
    gui_distribution.loadFromFile("gui_distribution.xml");


	franklinBook.load("frabk.ttf", 32);
	franklinBookSmall.load("frabk.ttf", 14);
	
	showFullscreen = true;
	exitApp = false;
	
    if(gayAmount <= 1) gayAmount = 25;
    if(lesbianAmount <= 1) lesbianAmount = 50;
    if(straightAmount <= 1) straightAmount = 25;
   
    if(jumWaitTime <= 1) jumWaitTime = 1234;

    
	//gayAmount = 25;
	//lesbianAmount = 50;
	//straightAmount = 25;
	//jumWaitTime = 225;
	
	wantOnset = true;
	wantKick = true;
	wantSnare = true;
	wantHat = true;
	
	old_gayAmount = gayAmount;
	old_lesbianAmount = lesbianAmount;
	old_straightAmount = straightAmount;
	
	old_soundVolume = -1;
	lastOnsetTime = 0;
	lastKickTime = 0;
	lastSnareTime = 0;
	lastHatTime = 0;
	
	old_bUseSound = false;
	
	jumpToNext = true;
	
   // if(bUseSound){
        //osc
        cout << "listening for osc messages on port " << remotePORT << "\n";
        // open an outgoing connection to HOST:PORT
        sender.setup( masterIP, remotePORT );
        
        //receiver
        receiver.setup( localPORT );
        current_msg_string = 0;
        
        //FFT
    if(bUseSound){
        string command = "open " + ofToDataPath("spectrum_live_osc_OF.app");// + "spectrum_live_osc.app";
        system(command.c_str());
        cout<<"\ncommand "<<command<<endl;
    }
	//}
	
	
	//ofLogVerbose() << "Attempt to load" << numberOfVideos;
	
	// create as many instances of goVideoPlayer as there are movies
	//videos = new goThreadedVideo[numberOfVideos];
	
	// use some variables to track where we're upto loading the movies
	numberOfVideosLoaded = 0;
	currentlyLoadingVideo = -1;
	
	//	currentVideo= 0;
	//	bLoadNewVideo = false;
	//	bLoadNewVideoDone = false;
	
	textX2 = 0;
	
	ofSetFullscreen(true);
	ofHideCursor();
	initDone = false;
	initStage = 0;
    bCheckLoading = false;
    
	mouseDoublePressed = false;
	
	actionMode = 1;
	showActionMode = false;
	showActionModeTimer = ofGetElapsedTimef();
	actionMode2JumpTriggered = false;
	
	/*
	 for(int i=0; i<255; i++){
	 keyIsDown[i] = false;
	 }
	 */
    
    ofHideCursor();
   
}

void ofApp::startup(){
	
	
	if(initStage == 0){
		
		//get all text file names
		//check if video file counter part exists
		//put the existing file names in list
		
		amtOfVideoFiles = videoDir.listDir("movs/");
		amtOfTextFiles = textDir.listDir("texts/");
		cout<<"videos available = "<<amtOfVideoFiles<<endl;
		cout<<"texts available = "<<amtOfTextFiles<<endl;
		
		for(int i=0; i<amtOfTextFiles; i++){
			
			//get file name without extension
			vector<string> fileTxt;
			fileTxt = ofSplitString(textDir.getName(i), ".");
			string fileName = fileTxt[0];
			
			//check if mov file exists
			//http://www.openframeworks.cc/forum/viewtopic.php?f=8&t=792&hilit=check+if+file+exists
			fstream fin;
			string fileNameMov = ofToDataPath("movs/"+fileName+".mov"); // since OF files are in the data directory, we need to do this
			//	string fileNameMov = videoDir.path()+fileName+".mov";
			
			fin.open(fileNameMov.c_str(),ios::in);
			if ( fin.is_open() ) {
                //ss	cout<<"file "<<fileName<<".mov exists"<<endl;
				videoNames[i] = fileName;
			}else{
                //ss	cout<<"file "<<fileName<<".mov does NOT exists"<<endl;
			}
			fin.close();
		}
		
		gayCount = 0;
		lesbianCount = 0;
		straightCount = 0;
		
		if(maxVideos > maxVideoType){
			
			
			float m0 = MAX(gayAmount,lesbianAmount);
			float m = MAX(m0,straightAmount.get());
			
			gayLimit = gayAmount / m * maxVideoType;
			lesbianLimit = lesbianAmount / m * maxVideoType;
			straightLimit = straightAmount / m * maxVideoType;
			
		}else{
			gayLimit = maxVideos / 100.0 * gayAmount;
			lesbianLimit = maxVideos / 100.0 * lesbianAmount;
			straightLimit = maxVideos / 100.0 * straightAmount;
		}
		cout<<"gayLimit "<<gayLimit<<", lesbianLimit "<<lesbianLimit<<", straightLimit "<<straightLimit<<endl;
		cout<<endl;
		
		usedVideoAmount = 0;
		
		initStage++;
	}
	if(initStage == 1){
		//check type of movie
		//make list with correct distribution of g l s, movie name
		//make list of kiss startFrame
		//make list with index to each video
		
		
		//get type of movie, i.e. gay, lesbian, straight
		//check if we already have enough of this type of movie
		for(int i=0; i<amtOfTextFiles; i++){
			
			string fileName = videoNames[i];
			vector<string> fileTxt2;
			
			fileTxt2 = ofSplitString(fileName, "-");
			string movieType = fileTxt2[0];
			//cout<<"movie gender type = "<<movieType<<endl;
			
			bool movieTypeWanted = false;
			
			if((movieType == "g" || movieType == "G") &&  gayCount<gayLimit){
				gayCount++;
				movieTypeWanted = true;
			}
			if((movieType == "l" || movieType == "L") &&  lesbianCount<lesbianLimit){
				lesbianCount++;
				movieTypeWanted = true;
			}
			if((movieType == "s" || movieType == "S") &&  straightCount<straightLimit){
				straightCount++;
				movieTypeWanted = true;
			}
			
			//cout<<"movieTypeWanted = "<<movieTypeWanted<<endl;
			
			if(movieTypeWanted){
				usedVideoNames[usedVideoAmount] = fileName;
				cout<<"usedVideoNames["<<usedVideoAmount<<"] = "<<fileName<<endl;
				usedVideoAmount++;
			}
		}
		
		cout<<"gays perc "<<gayAmount<<", lesbians perc "<<lesbianAmount<<", straights perc "<<straightAmount<<endl;
		cout<<"gays loaded "<<gayCount<<", lesbians loaded "<<lesbianCount<<", straights loaded "<<straightCount<<endl;
		cout<<"total amount of used videos "<<usedVideoAmount<<endl;
		
		//open each of the wanted text files, get kiss frame number
		for(int i=0; i<usedVideoAmount;i++){
			//http://www.cplusplus.com/doc/tutorial/files/
			string myLineKissPos;
			string line;
			
			//cout<<"open text file "<<textDIR.getPath(textID).c_str()<<endl;
			
			//ifstream myfile (textDIR.getPath(textID).c_str());
			ifstream myfile;
			string fileName = usedVideoNames[i];
			string textFilePath = ofToDataPath("texts/"+fileName+".txt");
			//string temp_path = ofToDataPath(textDIR.getPath(textID).c_str());
			
			myfile.open(textFilePath.c_str() );
			if (myfile.is_open())
			{
				int lineNum = 0;
				while ( myfile.good() )
				{
					getline (myfile,line);
					//cout <<lineNum<<" "<< line << endl;
					//	cout<<"myfile "<<myfile<<" "<<line<<endl;
					//if(lineNum == 1) myLineDuration = line;
					if(lineNum == 4){
						myLineKissPos = line;
						break;
					}
					lineNum++;
				}
				myfile.close();
				
				//extract kiss frame number from time code
				//	cout<<"try convertStringToSeconds "<<endl;
				int temp_frameNum = convertStringToSeconds(myLineKissPos);
				kissFrameNum[i] = temp_frameNum;
				videoPlayOrder[i] = i; //keeps index to each wanted file name and kissFrame
				
				cout<<"video "<<fileName<<" kiss starts at "<<temp_frameNum<<endl;
			}else{
				cout<< "Unable to open text file "<<fileName<<endl;
				
			} //end if (myfile.is_open())
		}
		
		
		//shuffel the index list to get random file index, i.e. different play order
		//http://en.wikipedia.org/wiki/Fisher-Yates_shuffle
		for(int i = usedVideoAmount - 1; i > 0; i--){
			int j = floor(ofRandomuf() * (i + 1));
			int tmp = videoPlayOrder[i];
			videoPlayOrder[i] = videoPlayOrder[j];
			videoPlayOrder[j] = tmp;
		}
		
		playOrderCnt = 0;
		initStage++;
		
	}
	if(initStage == 2){
		
		// check if there are more movies to load
		if (currentlyLoadingVideo < numberOfVideosLoaded &&  numberOfVideosLoaded < numberOfVideos) {
			// increment the current video counter and start loading the movie
			currentlyLoadingVideo++;
			//ofLogVerbose() << "Start loading:" << dir.getPath(currentlyLoadingVideo);
			
			string fileName = ofToDataPath("movs/"+usedVideoNames[videoPlayOrder[playOrderCnt]]+".mov");
			
			cout<<currentlyLoadingVideo<<" Start loading: "<<fileName<<endl;
			cout<<"playOrderCnt "<<playOrderCnt<<", videoPlayOrder[playOrderCnt] "<<videoPlayOrder[playOrderCnt]<<endl;
			
			indexToUsedVideos[currentlyLoadingVideo] = videoPlayOrder[playOrderCnt];
			videos[currentlyLoadingVideo].loadAsync(fileName);
			playOrderCnt++;
			// attach event listeners so we know when the movie is loaded
//            ofAddListener(videos[currentlyLoadingVideo].isLoaded(), this, &ofApp::loaded);
            bCheckLoading = true;
//            ofAddListener(videos[currentlyLoadingVideo].error, this, &ofApp::error);
		}
		
		// check if we're currently loading a movie
		if (currentlyLoadingVideo == numberOfVideosLoaded && numberOfVideosLoaded < numberOfVideos) {
			// use psuedo draw and update to force the loading of movies without actually having to be showing them onscreen
			// this is an ugly hack I use to load multiple movies without having to actually be drawing them to screen
//            videos[currentlyLoadingVideo].psuedoUpdate();
//            videos[currentlyLoadingVideo].psuedoDraw();
			
			cout<<"pseudo"<<endl;
		}
		if(numberOfVideosLoaded == numberOfVideos){
			initStage++;
			cout<<"initStage "<<initStage<<" done"<<endl;
		}
	}
	if(initStage == 3){
		
		
		string myAppName = "Bambarajos";
		char myScript[255];
		sprintf(myScript, "osascript -e 'tell app \"System Events\"' -e 'set theApps to every process whose name contains \"%s\"' -e 'set theApp to item 1 of theApps' -e 'set frontmost of theApp to true' -e 'end tell'",myAppName.c_str());
		std::system(myScript);
		
		currentVideo = 0;
		//bLoadNewVideo = false;
		//		bLoadNewVideoDone = false;
		//
		//		videos[currentVideo].play();
		//		videos[currentVideo].setSpeed(1);
		
		bLoadNewVideo = true;
		bLoadNewVideoDone = false;
		
		currentlyLoadingVideo = currentVideo;
		currentVideo++;
		if(currentVideo >= numberOfVideosLoaded) currentVideo = 0;
		
		cout<<"showing now video "<<usedVideoNames[indexToUsedVideos[currentVideo]]<<endl;
		cout<<"kiss frame = "<<kissFrameNum[indexToUsedVideos[currentVideo]]<<endl;
		
		
		showSensitivity = false;
		showSensitivityTimer = ofGetElapsedTimef();
		jumpWaitTimer = ofGetElapsedTimeMillis();
		
		initDone = true;
		
		ofSetFullscreen(true);
	}
    
    
}

void ofApp::exit(){
	
    
    ofxOscMessage temp_sendMessage;
    temp_sendMessage.setAddress( "/exit" );
    sender.sendMessage( temp_sendMessage );
    
	
	cout<<"exit"<<endl;
}

int ofApp::convertStringToSeconds(string s){
	
	
	vector<string> fileTxt;
	fileTxt = ofSplitString(s, ":");
	
	string lineHeader = fileTxt[0];
	//cout<<"lineHeader "<<lineHeader<<endl;
	
	int hours = ofToInt(fileTxt[2]);
	int minutes = ofToInt(fileTxt[3]);
	string rest = fileTxt[4];
	//cout<<"rest "<<rest<<endl;
	
	vector<string> fileTxt1;
	fileTxt1 = ofSplitString(fileTxt[4], "/");
	
	string rest1 = fileTxt1[0];
	//cout<<"rest1 "<<rest1<<endl;
	
	vector<string> fileTxt2;
	fileTxt2 = ofSplitString(fileTxt1[0], ".");
	
	
	
	int seconds = ofToInt(fileTxt2[0]);
	
	int frames = ofToInt(fileTxt2[1]);
	float secFraction = frames / 2997.0;
	
	//cout<<"hours "<<hours<<", min "<<minutes<<", secs "<<seconds<<", frames "<<frames<<" secFraction "<<secFraction<<endl;
	
	//kissFrameNum[i] = ofToInt(fileTxt1[0]);
	//	cout<<"kiss frame num "<<kissFrameNum[i]<<endl;
	//textNames[i] = textDIR.getName(i);
	
	float timeInSeconds = hours*3600 + minutes*60 + seconds + secFraction;
	int timeInFrames = timeInSeconds * 30;
	
	//cout<<"timeInFrames "<<timeInFrames<<endl;
	
	return timeInFrames; //frameNum;
	
}
//--------------------------------------------------------------
void ofApp::update(){
	
    ofBackground(0,0,0);
	
    checkGui();
    
    
	if(exitApp == true){
		std::exit(1);
		//exit();
	}
	
	if(initDone == false){
		
		startup();
		
	}else{
        
        if(ofGetElapsedTimef() - autoTriggerTimer > autoTriggerDuration && autoTriggerDuration != 0){
            autoTriggerTimer = ofGetElapsedTimef();
            jumpToNext = true;
        }
        
		if(jumpToNext && (ofGetElapsedTimeMillis() - jumpWaitTimer) > jumWaitTime){
			jumpWaitTimer = ofGetElapsedTimeMillis();
			jumpToNext = false;
			
			if(actionMode == 1) triggerNewVideo();
			if(actionMode == 2 && actionMode2JumpTriggered == false) {
				videos[currentVideo].setFrame(kissFrameNum[indexToUsedVideos[currentVideo]]);
				videos[currentVideo].setSpeed(1);
				actionMode2JumpTriggered = true;
			}
		}
		
		if(actionMode == 2){
			if(ABS(videos[currentVideo].getTotalNumFrames() - videos[currentVideo].getCurrentFrame()) < 3  || videos[currentVideo].getIsMovieDone() == true){
				actionMode2JumpTriggered = false;
				triggerNewVideo();
			}
			
			int diffFrame = 10;
			if(kissFrameNum[indexToUsedVideos[currentVideo]] < 10) diffFrame = kissFrameNum[indexToUsedVideos[currentVideo]] / 2;
            
			if( (kissFrameNum[indexToUsedVideos[currentVideo]] - videos[currentVideo].getCurrentFrame()) < diffFrame && videos[currentVideo].getSpeed() > 0 && actionMode2JumpTriggered == false){
				
				float newSpeed = 1.0;
				float firstRandom = ofRandom(0,1);
				if(firstRandom >= 0.5){
					float secondRandom = ofRandom(0.2, 1);
					newSpeed = -secondRandom;
				}
				
				cout<<"-newSpeed "<<newSpeed<<endl;
				
				videos[currentVideo].setSpeed(newSpeed);
			}
			
			if(videos[currentVideo].getCurrentFrame() == 0){
				float newSpeed = 1.0;
				float firstRandom = ofRandom(0,1);
				
				if(firstRandom >= 0.5){
					float secondRandom = ofRandom(0.2, 1);
					newSpeed = secondRandom;
				}
				
				cout<<"newSpeed "<<newSpeed<<endl;
				
				videos[currentVideo].setSpeed(newSpeed);
			}
		}
		
		if(bLoadNewVideo == true){
			
			bLoadNewVideo  = false;
			bLoadNewVideoDone = true;
			
			videos[currentlyLoadingVideo].close();
			
			string fileName = ofToDataPath("movs/"+usedVideoNames[videoPlayOrder[playOrderCnt]]+".mov");
			cout<<currentlyLoadingVideo<<" Start loading: "<<fileName<<endl;
			cout<<"playOrderCnt "<<playOrderCnt<<", videoPlayOrder[playOrderCnt] "<<videoPlayOrder[playOrderCnt]<<endl;
			
			indexToUsedVideos[currentlyLoadingVideo] = videoPlayOrder[playOrderCnt];
			videos[currentlyLoadingVideo].loadAsync(fileName);
			
			
			// attach event listeners so we know when the movie is loaded
//            ofAddListener(videos[currentlyLoadingVideo].isLoaded(), this, &ofApp::loaded);
            bCheckLoading = true;
//            ofAddListener(videos[currentlyLoadingVideo].error, this, &ofApp::error);
			
			
			
			playOrderCnt++;
			if(playOrderCnt >= usedVideoAmount){
				//if we reached end of index array, shuffel the array and start with 0
				cout<<"shuffel movie index array"<<endl;
				for(int i = usedVideoAmount - 1; i > 0; i--){
					int j = floor(ofRandomuf() * (i + 1));
					int tmp = videoPlayOrder[i];
					videoPlayOrder[i] = videoPlayOrder[j];
					videoPlayOrder[j] = tmp;
				}
				playOrderCnt = 0;
			}
		}//end if(bLoadNewVideo == true)
		
		if(bLoadNewVideoDone == true){
//            videos[currentlyLoadingVideo].psuedoUpdate();
//            videos[currentlyLoadingVideo].psuedoDraw();
			//cout<<"pseudo"<<endl;
		}
		
		videos[currentVideo].update();
		
		if(bUseSound) OSCupdate();
		
		if(bShowGui){
			if(allSame){
				gayAmount = lesbianAmount = straightAmount = 100/3;
			}
            
            
            if(bUseSound){
                if(isOnset && ofGetElapsedTimeMillis()-lastOnsetTime > 250){
                    isOnset = false;
                }
                if(isKick && ofGetElapsedTimeMillis()-lastKickTime > 250){
                    isKick = false;
                }
                if(isSnare && ofGetElapsedTimeMillis()-lastSnareTime > 250){
                    isSnare = false;
                }
                if(isHat && ofGetElapsedTimeMillis()-lastHatTime > 250){
                    isHat = false;
                }
                
                
                if(wantOnset != oldWantOnset){
                    ofxOscMessage temp_sendMessage;
                    temp_sendMessage.setAddress( "/wantOnset" );
                    temp_sendMessage.addIntArg(wantOnset);
                    sender.sendMessage( temp_sendMessage );
                    oldWantOnset = wantOnset;
                }
                if(wantKick != oldWantKick){
                    ofxOscMessage temp_sendMessage;
                    temp_sendMessage.setAddress( "/wantKick" );
                    temp_sendMessage.addIntArg(wantKick);
                    sender.sendMessage( temp_sendMessage );
                    oldWantKick = wantKick;
                }
                if(wantSnare != oldWantSnare){
                    ofxOscMessage temp_sendMessage;
                    temp_sendMessage.setAddress( "/wantSnare" );
                    temp_sendMessage.addIntArg(wantSnare);
                    sender.sendMessage( temp_sendMessage );
                    oldWantSnare = wantSnare;
                }
                if(wantHat != oldWantHat){
                    ofxOscMessage temp_sendMessage;
                    temp_sendMessage.setAddress( "/wantHat" );
                    temp_sendMessage.addIntArg(wantHat);
                    sender.sendMessage( temp_sendMessage );
                    oldWantHat = wantHat;
                }
			}
			
		} //end if(gui.isOn())
		
		
		/*
		 if( keyIsDown['1'] && keyIsDown['g']) {
		 
		 if(gui.isOn()){
		 gui.hide();
		 ofHideCursor();
		 }else{
		 gui.show();
		 ofShowCursor();
		 }
		 
		 }
		 if( keyIsDown['1'] && keyIsDown['f']) {
		 showFullscreen != showFullscreen;
		 ofSetFullscreen(showFullscreen);
		 }
		 */
		
		
		if(ofGetElapsedTimef() - showActionModeTimer >= 3.0){
			showActionMode = false;
		}
		if(ofGetElapsedTimef() - showSensitivityTimer >= 3.0){
			
			showSensitivity = false;
            gui_detection.saveToFile("gui_detection.xml");
		}
		
        if(soundVolume != old_soundVolume){
            
            if(bUseSound){
                
                ofxOscMessage temp_sendMessage;
                temp_sendMessage.setAddress( "/volume" );
                float temp_soundVolume = pow(soundVolume,3);
                
                cout<<"soundVolume "<<soundVolume<<", pow3 "<<temp_soundVolume<<endl;
                temp_sendMessage.addFloatArg( temp_soundVolume * 1.0f);
                sender.sendMessage( temp_sendMessage );
            }
            old_soundVolume = soundVolume;
            
        }
		
	}//else if(initDone == false)

    if(bCheckLoading == true){
        loaded();
    }
}

void ofApp::OSCupdate(){
	
	jumpToNext = false;
	
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimeMillis() )
			msg_strings[i] = "";
	}
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// check for mouse moved message
		if ( m.getAddress() == "/isKick" )
		{
			isKick = true;
			lastKickTime = ofGetElapsedTimeMillis();
			
			//cout<<"osc isKick"<<endl;
			if(wantKick) jumpToNext = true;
		}
		else if ( m.getAddress() == "/isSnare" )
		{
			isSnare = true;
			lastSnareTime = ofGetElapsedTimeMillis();
			//cout<<"osc isSnare"<<endl;
			if(wantSnare) jumpToNext = true;
		}
		else if ( m.getAddress() == "/isHat" )
		{
			isHat = true;
			lastHatTime = ofGetElapsedTimeMillis();
			//cout<<"osc isHat"<<endl;
			if(wantHat) jumpToNext = true;
		}
		else if ( m.getAddress() == "/isOnset" )
		{
			isOnset = true;
			lastOnsetTime = ofGetElapsedTimeMillis();
			//cout<<"osc isOnset"<<endl;
			if(wantOnset) jumpToNext = true;
		}
		else
		{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for ( int i=0; i<m.getNumArgs(); i++ )
			{
				// get the argument type
				msg_string += m.getArgTypeName( i );
				msg_string += ":";
				// display the argument - make sure we get the right type
				if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
					msg_string += ofToString( m.getArgAsInt32( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
					msg_string += ofToString( m.getArgAsFloat( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
					msg_string += m.getArgAsString( i );
				else
					msg_string += "unknown";
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimeMillis() + 5.0f;
			current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
		
	}
}

void ofApp::triggerNewVideo(){
	
	
	currentlyLoadingVideo = currentVideo;
	currentVideo++;
	if(currentVideo >= numberOfVideos) currentVideo = 0;
	
	cout<<"currentVideo "<<currentVideo<<endl;
	cout<<"indexToUsedVideos[currentVideo] "<<indexToUsedVideos[currentVideo]<<endl;
	cout<<"showing now video "<<usedVideoNames[indexToUsedVideos[currentVideo]]<<endl;
	cout<<"kiss frame = "<<kissFrameNum[indexToUsedVideos[currentVideo]]<<endl;
	
	if(actionMode == 1){
		videos[currentVideo].setFrame(kissFrameNum[indexToUsedVideos[currentVideo]]);
	}
	if(actionMode == 2){
		videos[currentVideo].setFrame(1);
		
	}
	videos[currentVideo].setSpeed(1);
	
	bLoadNewVideo = true;
	bLoadNewVideoDone = false;
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofSetColor(255,255,255);
	
	if(initDone == false){
		
		//ofDrawBitmapString("loading "+ofToString(ofGetElapsedTimeMillis()),20,ofGetHeight()/2);
		
		ofSetHexColor(0xFFFFFF);
		string temp_s = "loading ";
		float temp_sWidth = franklinBookSmall.stringWidth(temp_s);
		float temp_sHeight = franklinBookSmall.stringHeight(temp_s);
		
		float textX = ofGetWidth()/2 - temp_sWidth/2;
		float lineY = ofGetHeight()/2 + temp_sHeight;
		
		franklinBookSmall.drawString(temp_s, textX,ofGetHeight()/2);
		
		if(textX2 < textX ) textX2 = textX;
		ofLine(textX, lineY, textX2, lineY);
		textX2 = textX2 + 1;
		if(textX2-textX > temp_sWidth) textX2 = textX;
		
		
	}else{
		
		
		videos[currentVideo].draw(0, 0, ofGetWidth(), ofGetHeight());
		
		
		//at end of movie
		if(actionMode == 1){
			if(videos[currentVideo].getCurrentFrame() == 0){
				
				cout<<ofGetElapsedTimeMillis()<<" "<<currentVideo<<endl;
				//after first loop set new speed 50% of time speed = 1
				//50% of time new speed between 0.2 and 1.0
				
				float newSpeed = 1.0;
				float firstRandom = ofRandom(0,1);
				//cout<<" 1 "<<firstRandom<<endl;
				
				if(firstRandom >= 0.5){
					float secondRandom = ofRandom(0.2, 1);
					newSpeed = secondRandom;
					//cout<<" 2 "<<secondRandom<<endl;
				}
				
				cout<<"newSpeed "<<newSpeed<<endl;
				
				videos[currentVideo].setSpeed(newSpeed);
			}
		}
        
		if(bShowGui){
			
			//playhead lines
			ofSetLineWidth(2);
			int x = ofMap(videos[currentVideo].getCurrentFrame(), 0, videos[currentVideo].getTotalNumFrames(), 0, ofGetWidth());
			ofSetColor(0, 0, 255);
			ofLine(x, ofGetHeight()/4*3, x, ofGetHeight());
			
			int x1 = ofMap(kissFrameNum[indexToUsedVideos[currentVideo]], 0, videos[currentVideo].getTotalNumFrames(), 0, ofGetWidth());
			ofSetColor(255, 255, 255);
			ofLine(x1, ofGetHeight()/4*3, x1, ofGetHeight());
			
			ofSetLineWidth(1);
			
			
			ofPushMatrix();
			ofTranslate(20, 500, 0);
			ofDrawBitmapString("frame: " + ofToString(videos[currentVideo].getCurrentFrame()) + "/"+ofToString(videos[currentVideo].getTotalNumFrames()),20,20);
			ofDrawBitmapString("kiss frame: " + ofToString(kissFrameNum[currentVideo]),20,40);
			ofDrawBitmapString("duration: " + ofToString(videos[currentVideo].getPosition()*videos[currentVideo].getDuration(),2) + "/"+ofToString(videos[currentVideo].getDuration(),2),20,60);
			ofDrawBitmapString("speed: " + ofToString(videos[currentVideo].getSpeed()),20,80);
			ofDrawBitmapString("movie name: "+usedVideoNames[currentVideo], 20, 100);
			ofDrawBitmapString("movie counter "+ofToString(playOrderCnt)+"/"+ofToString(usedVideoAmount),20,120);
			ofDrawBitmapString("reaction mode "+ ofToString(actionMode) + ", "+ofToString(actionMode2JumpTriggered),20,140);
			
			ofPopMatrix();
            
            gui_main.draw();
            gui_kisses.draw();
            gui_detection.draw();
            gui_distribution.draw();
		}
		
		
	
        
		if(showSensitivity == true){
			ofSetHexColor(0xFFFFFF);
			//soundVolumeMax
			int temp_volume = ofMap(soundVolume,0,soundVolumeMax,0,100); //int(soundVolume * 100.0);
			string temp_s = "Sensitivity "+ofToString(temp_volume, 0) + "%";
			int stringWidth = franklinBook.stringWidth(temp_s);
			franklinBook.drawString(temp_s, ofGetWidth()/2 - stringWidth/2,ofGetHeight() - 200);
			
		}
		
		if(showActionMode){
			ofSetHexColor(0xFFFFFF);
			string temp_s = "Reaction Mode "+ofToString(actionMode, 0);
			int stringWidth = franklinBook.stringWidth(temp_s);
			franklinBook.drawString(temp_s, ofGetWidth()/2 - stringWidth/2,ofGetHeight() - 160);
		}
		
	}
	
}

void ofApp::checkGui(){
    if(x_keyTriggered == true){
        x_keyTriggered = false;
        jumpToNext = true;
        autoTriggerTimer = ofGetElapsedTimef();
    }
}
void ofApp::loaded() { //string & path) {
    ofLogVerbose() << "Finsished loading:"; // << path;
	//remove the event listeners
	
	//videos[currentlyLoadingVideo].setSpeed(0);
	videos[currentlyLoadingVideo].setLoopState(OF_LOOP_PALINDROME);
	
	int frameNum = kissFrameNum[indexToUsedVideos[currentlyLoadingVideo]];
	//videos[currentlyLoadingVideo].setFrame(frameNum);
	//    ofRemoveListener(ofEvents().update, this, &eventsObject::update);
    bCheckLoading = false;
//    ofRemoveListener(videos[currentlyLoadingVideo].isLoaded(), this, &ofApp::loaded);
//    ofRemoveListener(videos[currentlyLoadingVideo].error, this, &ofApp::error);
	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
	numberOfVideosLoaded++;
	bLoadNewVideoDone = false;
}

//void ofApp::error(goVideoError & err) {
//    ofLogVerbose() << "Error loading:" << err;
//    //remove the event listeners
//    ofRemoveListener(videos[currentlyLoadingVideo].success, this, &ofApp::loaded);
//    ofRemoveListener(videos[currentlyLoadingVideo].error, this, &ofApp::error);
//    // increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
//    numberOfVideosLoaded++;
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	//keyIsDown[key] = true;
	
	/*
	 if(key == ' '){
	 
	 triggerNewVideo();
	 
	 }
	 */
	switch(key){
        
                 
      
            
        case'n':
            jumpToNext = true;
            break;

            
		case OF_KEY_UP:
			showSensitivity = true;
			soundVolume = soundVolume + 0.01;
			if(soundVolume > soundVolumeMax) soundVolume = soundVolumeMax;
			showSensitivityTimer = ofGetElapsedTimef();
			break;
			
		case OF_KEY_DOWN:
			showSensitivity = true;
			soundVolume = soundVolume - 0.01;
			if(soundVolume <= 0) soundVolume = 0;
			showSensitivityTimer = ofGetElapsedTimef();
			break;
			
		case OF_KEY_RIGHT:
			showSensitivity = true;
			soundVolume = soundVolume + 0.01;
			if(soundVolume > soundVolumeMax) soundVolume = soundVolumeMax;
			showSensitivityTimer = ofGetElapsedTimef();
			break;
			
		case OF_KEY_LEFT:
			showSensitivity = true;
			soundVolume = soundVolume - 0.01;
			if(soundVolume <= 0) soundVolume = 0;
			showSensitivityTimer = ofGetElapsedTimef();
			break;
			
		case '1':
			actionMode = 1;
			showActionMode = true;
			showActionModeTimer = ofGetElapsedTimef();
			break;
		case '2':
			actionMode = 2;
			showActionMode = true;
			showActionModeTimer = ofGetElapsedTimef();
			videos[currentVideo].setFrame(1);
			actionMode2JumpTriggered = false;
			break;
            
            
	}
	
//        x_keyTriggered = false;
	
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//keyIsDown[key] = false;

    if(key =='g'){
        bShowGui = !bShowGui;
        cout<<"hide gui"<<endl;
        if(bShowGui == false){ 
            ofHideCursor();
            
            gui_main.saveToFile("gui_main.xml");
            gui_kisses.saveToFile("gui_kisses.xml");
            gui_detection.saveToFile("gui_detection.xml");
            gui_distribution.saveToFile("gui_distribution.xml");
        }else{
            ofShowCursor();
        }
    }
    
    if(key == 'x'){
//        x_keyTriggered = true;
        jumpToNext = true;
        autoTriggerTimer = ofGetElapsedTimef();
    }
    if(key == 'f') ofToggleFullscreen();

    

    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
	unsigned long curTap = ofGetElapsedTimeMillis();
	if(lastTap != 0 && curTap - lastTap < 500){
		if(x < 100 && y < 100){
			//cout<<"mousePressed"<<endl;
			if(bShowGui == false){
                bShowGui = true;
				ofShowCursor();
			}else{
                bShowGui = false;
				ofHideCursor();
			}
		}
		mouseDoublePressed = true;
	}
	lastTap = curTap;
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
	if(bShowGui == true){
		if(gayAmount != old_gayAmount){
			int rest = 100 - gayAmount;
			
			if(lesbianAmount <=0 ) lesbianAmount = 1;
			if(straightAmount <=0 ) straightAmount = 1;
			
			float full = lesbianAmount + straightAmount;
			float perclesbianAmount = lesbianAmount/full *1.0;
			float percstraightAmount = straightAmount/full *1.0;
			
			lesbianAmount = rest * perclesbianAmount;
			straightAmount = rest * percstraightAmount;
			
		}else if(lesbianAmount != old_lesbianAmount){
			int rest = 100 - lesbianAmount;
			
			if(gayAmount <=0 ) gayAmount = 1;
			if(straightAmount <=0 ) straightAmount = 1;
			
			float full = gayAmount + straightAmount;
			float percgayAmount = gayAmount/full *1.0;
			float percstraightAmount = straightAmount/full *1.0;
			
			gayAmount = rest * percgayAmount;
			straightAmount = rest * percstraightAmount;
			
		}else if(straightAmount != old_straightAmount){
			int rest = 100 - straightAmount;
			
			if(gayAmount <=0 ) gayAmount = 1;
			if(lesbianAmount <=0 ) lesbianAmount = 1;
			
			float full = gayAmount + lesbianAmount;
			float percgayAmount = gayAmount/full *1.0;
			float perclesbianAmount = lesbianAmount/full *1.0;
			
			gayAmount = rest * percgayAmount;
			lesbianAmount = rest * perclesbianAmount;
		}
		
		old_gayAmount = gayAmount;
		old_lesbianAmount = lesbianAmount;
		old_straightAmount = straightAmount;
	}
	
	ofSetFullscreen(showFullscreen);
	
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}

