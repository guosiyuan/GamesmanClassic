
# 
#  the actions to be performed when the toolbar buttons are pressed
#

proc TBaction1 {} {

    .middle.f1.cMLeft raise detVal
    
    # solve the game, 
    global kGameName varObjective gPosition gInitialPosition
    global gGameSolved
    
    # Send initial game-specific options to C procs.
    if { $gGameSolved == "false"} {
	. config -cursor watch
	set theValue [C_DetermineValue $gPosition]
	set gGameSolved true
	. config -cursor {}
	.middle.f1.cMLeft raise iIMB
	.middle.f3.cMRight raise play
	.cStatus lower base
	pack forget .middle.f2.fPlayOptions.fBot
	SetupPlayOptions

	global gLeftName gRightName
	.middle.f1.cMLeft itemconfig LeftName \
	    -text [format "Player1:\n%s" $gLeftName]
	.middle.f3.cMRight itemconfig RightName \
	    -text [format "Player2:\n%s" $gRightName]
	update
    } else {
	.middle.f3.cMRight lower play
	.middle.f1.cMLeft lower detVal
	.cToolbar raise iATB
	.cStatus lower base
	update idletasks
	NewGame
	set gamestarted true 
	
	.cToolbar bind iOTB1 <Any-Leave> \
		".cToolbar raise iATB1"
    }
}

# Rules button
proc TBaction2 {} {
    .cToolbar raise iITB
    pack forget .middle.f2.cMain   
    pack .middle.f2.fRules -side bottom -fill both -expand 1
}

# Help button
proc TBaction3 {} {
    .cToolbar raise iITB
    pack forget .middle.f2.cMain   
    pack .middle.f2.fHelp -side bottom
}

# About button
proc TBaction4 {} {
    .cToolbar raise iITB
    pack forget .middle.f2.cMain
    pack .middle.f2.fAbout -side bottom
}

# Unmapped
proc TBaction5 {} {
}

# Unmapped
proc TBaction6 {} {
}

# Unmapped
proc TBaction7 {} {
}

proc SetupPlayOptions {} {

    global varMoves varPredictions varObjective

    global gSlotsX gSlotsY gSlotSize kLabelFont kLabelColor

    global kGameName

    global kToMove kToWinStandard kToWinMisere

    global kMovesOnAllTheTime

    global gWaitingForHuman
    set gWaitingForHuman true

    global gMoveType gPosition

    GS_HideMoves .middle.f2.cMain $gMoveType $gPosition [C_GetValueMoves $gPosition]

    pack forget .middle.f2.cMain   
    pack .middle.f2.fPlayOptions -side bottom
    .cStatus raise base

    global gamestarted

    set gamestarted true

}
    
proc InitWindow { kRootDir } {


    global gWindowWidth gWindowHeight
    global gSkinsLibName
    global gamestarted
    global kGameName
    global kLabelFont
    global tcl_platform
    global kToMove kToWin gPredString gWhoseTurn
    global gLeftName gRightName
    global gLeftColor gRightColor
    # jesse: move delay and game delay added fall '03
    global gMoveDelay gGameDelay

    #
    # Initialize the constants
    #
    set gWhoseTurn "Jesse"
    set gPredString ""
    set gWindowHeight 600
    set gWindowWidth 800
    wm aspect . 800 600 1600 1200
    set gamestarted false 
    set gSkinsLibName "$kRootDir/../tcl/skins/BasicSkin/BasicSkin"
    if { $tcl_platform(platform) == "macintosh" || \
         $tcl_platform(platform) == "windows" } {
        console hide
    }
    SetupGamePieces

    #
    # create the Toolbar - this is out of order so that we can automatically create a lot of images
    #
    
    canvas .cToolbar -highlightthickness 0 \
	-bd 0 \
	-width $gWindowWidth \
	-height [expr $gWindowHeight / 30] \
	-background green

    #
    # Load all the button images
    #

    foreach mode {A I O} {
	foreach file {1 2 3 4 5 6 7 8} {
	    set name [format i%sTB%s $mode $file]
	    image create photo $name -file [format %s_%s_1_%s.GIF $gSkinsLibName $mode $file]
	    set type [format i%sTB $mode]
	    .cToolbar create image [expr ($gWindowWidth / 16) + ($file - 1) * $gWindowWidth / 8] [expr $gWindowHeight / 60] \
		-image $name -tags [list tbb $type $name]
	}
    } 
    image create photo iIMB1 -file [format %s_I_2_1.GIF $gSkinsLibName]
    image create photo iIMB2 -file [format %s_I_2_2.GIF $gSkinsLibName]
    image create photo iIMB3 -file [format %s_I_2_3.GIF $gSkinsLibName]
    image create photo iIMB4 -file [format %s_I_2_4.GIF $gSkinsLibName]
    image create photo iIMB5 -file [format %s_I_2_5.GIF $gSkinsLibName]
    image create photo iIMB6 -file [format %s_I_2_6.GIF $gSkinsLibName]
    image create photo iAMB5 -file [format %s_A_2_5.GIF $gSkinsLibName]
    image create photo iSMB7 -file [format %s_A_2_7.GIF $gSkinsLibName]
    image create photo iAMB7 -file [format %s_A_7_1.GIF $gSkinsLibName]
    image create photo iAMB8 -file [format %s_A_8_1.GIF $gSkinsLibName]
    image create photo iAMM1 -file [format %s_A_4_1.GIF $gSkinsLibName]
    image create photo iBBB1 -file [format %s_A_3_1.GIF $gSkinsLibName]
    image create photo iABB2 -file [format %s_A_3_2.GIF $gSkinsLibName]
    image create photo iIBB2 -file [format %s_I_3_2.GIF $gSkinsLibName]
    image create photo iABB3 -file [format %s_A_3_3.GIF $gSkinsLibName]
    image create photo iIBB3 -file [format %s_I_3_3.GIF $gSkinsLibName]
    image create photo iABB4 -file [format %s_A_3_4.GIF $gSkinsLibName]
    image create photo iIBB4 -file [format %s_I_3_4.GIF $gSkinsLibName]
    image create photo iABB5 -file [format %s_A_3_5.GIF $gSkinsLibName]    
    image create photo iIBB5 -file [format %s_I_3_5.GIF $gSkinsLibName]
    image create photo iABB6 -file [format %s_A_3_6.GIF $gSkinsLibName]
    image create photo iIBB6 -file [format %s_I_3_6.GIF $gSkinsLibName]
    image create photo iIBB7 -file [format %s_I_3_7.GIF $gSkinsLibName]
    image create photo iABB7 -file [format %s_A_3_7.GIF $gSkinsLibName]
    image create photo iIBB8 -file [format %s_I_3_8.GIF $gSkinsLibName]
    image create photo iABB8 -file [format %s_A_3_8.GIF $gSkinsLibName]
    image create photo iIBB9 -file [format %s_I_3_9.GIF $gSkinsLibName]
    image create photo iABB9 -file [format %s_A_3_9.GIF $gSkinsLibName]

    #
    # Deal with everything in the top toolbar
    #

    #
    # Now Bind all the buttons
    #
    
    # set the active action of each button
    set mode A
    foreach file {1 2 3 4 5 6 7 8} {
	set name [format i%sTB%s $mode $file]
	set type [format i%sTB $mode]
	.cToolbar bind $name <Any-Enter> \
	    ".cToolbar raise {iOTB$file}; update idletasks" 
    }
    
    # bind the action of the mouse-Over images
    set mode O
    foreach file {1 2 3 4 5 6 7} {
	set name [format i%sTB%s $mode $file]
	set type [format i%sTB $mode]
	.cToolbar bind $name <ButtonRelease-1> \
	    ".cStatus raise base; \
             update idletasks; \
             TBaction$file"
	.cToolbar bind $name <Any-Leave> \
	    ".cToolbar raise iATB$file; update idletasks"
    }

    # Deal with exit button separately
    .cToolbar dtag iITB8 iITB
    .cToolbar bind iOTB8 <ButtonRelease-1> {
	exit
    }
    .cToolbar bind iOTB8  <Any-Leave> {
	.cToolbar raise iATB8
    }

    # Set up starting display with the active images on top
    .cToolbar raise iATB




    #
    # Main Area
    #

    # create the frames needed to divide up the middle into 3 parts
    frame .middle

    # set the size of the frames and force them to stay that way
    frame .middle.f1 \
	-width [expr $gWindowWidth * 3 / 16] \
	-height [expr $gWindowHeight * 25 / 30]
    frame .middle.f2 \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 25 / 30] 
    frame .middle.f3 \
	-width [expr $gWindowWidth * 3 / 16] \
	-height [expr $gWindowHeight * 25 / 30]

    pack propagate .middle.f1 0
    pack propagate .middle.f2 0
    pack propagate .middle.f3 0

    # set up the necessary canvases in each frame
    canvas .middle.f1.cMLeft -highlightthickness 0 \
	-bd 0 \
	-width [expr $gWindowWidth * 3 / 16] \
	-height [expr $gWindowHeight * 25 / 30] \
	-background black
    
    canvas .middle.f2.cMain -highlightthickness 0 \
	-bd 0 \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 25 / 30] \
	-background white
    
    # 
    # PLAY OPTIONS FRAME
    #
	    
    frame .middle.f2.fPlayOptions \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 25 / 30]
    pack propagate .middle.f2.fPlayOptions 0
    frame .middle.f2.fPlayOptions.fBot \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 5 / 30]
    pack propagate .middle.f2.fPlayOptions.fBot 0

    # this is not packed now <- you cannot cancel
    button .middle.f2.fPlayOptions.fBot.bCancel -text "Cancel" \
	-command {
            .cToolbar bind iOTB2 <Any-Leave> \
		    ".cToolbar raise iATB2"
	    pack forget .middle.f2.fPlayOptions   
	    pack .middle.f2.cMain
	    .cToolbar raise iATB
            .cStatus lower base
	    update
	}
    # clicking OK in the in game play options
    button .middle.f2.fPlayOptions.fBot.bOk -text "OK" \
	-command {
            .cToolbar bind iOTB2 <Any-Leave> \
		    ".cToolbar raise iATB2"
	    pack forget .middle.f2.fPlayOptions   
	    pack .middle.f2.cMain
            .cStatus lower base
	    .cToolbar raise iATB
            global gLeftName gRightName
            .middle.f1.cMLeft itemconfigure LeftName \
		    -text [format "Player1:\n%s" $gLeftName]
            .middle.f3.cMRight itemconfigure RightName \
		    -text [format "Player2:\n%s" $gRightName]
	    update
	    DriverLoop
        }
    frame .middle.f2.fPlayOptions.fTop \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 20 / 30] \
	-bd 2
	pack propagate .middle.f2.fPlayOptions.fTop 0
    frame .middle.f2.fPlayOptions.fTop.fLeft \
	-width [expr $gWindowWidth * 5 / 16] \
	-height [expr $gWindowHeight * 20 / 30] \
	-bd 2
    frame .middle.f2.fPlayOptions.fTop.fRight \
	-width [expr $gWindowWidth * 5 / 16] \
	-height [expr $gWindowHeight * 20 / 30] \
	-bd 2
    # the contents of the play options frame	    
    radiobutton .middle.f2.fPlayOptions.fTop.fLeft.rHuman \
	    -text "Player1 Human" \
	    -font $kLabelFont \
	    -variable gLeftHumanOrComputer \
	    -value Human \
	-command { 
	    .middle.f2.fPlayOptions.fTop.fLeft.moveDelay configure -state disabled -fg gray -troughcolor gray
	    .middle.f2.fPlayOptions.fTop.fRight.gameDelay configure -state disabled -fg gray -troughcolor gray
	}
    radiobutton .middle.f2.fPlayOptions.fTop.fLeft.rComputer \
	    -text "Player1 Computer" \
	    -font $kLabelFont \
	    -variable gLeftHumanOrComputer \
	    -value Computer \
	-command {
	    if { $gRightHumanOrComputer == "Computer" } {
		.middle.f2.fPlayOptions.fTop.fLeft.moveDelay configure -state active -fg black -troughcolor gold
		.middle.f2.fPlayOptions.fTop.fRight.gameDelay configure -state active -fg black -troughcolor gold
	    }
	}
    label .middle.f2.fPlayOptions.fTop.fLeft.lName \
	    -text "Player1 Name:" \
	    -font $kLabelFont
    entry .middle.f2.fPlayOptions.fTop.fLeft.eName \
	    -text "Player1 Name" \
	    -font $kLabelFont \
	    -textvariable gLeftName \
	    -width 20
    radiobutton .middle.f2.fPlayOptions.fTop.fRight.rHuman \
	    -text "Player2 Human" \
	    -font $kLabelFont \
	    -variable gRightHumanOrComputer \
	    -value Human \
	-command { 
	    .middle.f2.fPlayOptions.fTop.fLeft.moveDelay configure -state disabled -fg gray -troughcolor gray
	    .middle.f2.fPlayOptions.fTop.fRight.gameDelay configure -state disabled -fg gray -troughcolor gray
	}
    radiobutton .middle.f2.fPlayOptions.fTop.fRight.rComputer \
	    -text "Player2 Computer" \
	    -font $kLabelFont \
	    -variable gRightHumanOrComputer \
	    -value Computer \
	-command {
	    if { $gLeftHumanOrComputer == "Computer" } {
		.middle.f2.fPlayOptions.fTop.fLeft.moveDelay configure -state active -fg black -troughcolor gold
		.middle.f2.fPlayOptions.fTop.fRight.gameDelay configure -state active -fg black -troughcolor gold
	    }
	}
    label .middle.f2.fPlayOptions.fTop.fRight.lName \
	    -text "Player2 Name:" \
	    -font $kLabelFont
    entry .middle.f2.fPlayOptions.fTop.fRight.eName \
	    -text "Player2 Name" \
	    -font $kLabelFont \
	    -textvariable gRightName \
	    -width 20

    ## JESSE: adding scale bars for the speed of computer to computer play
    scale .middle.f2.fPlayOptions.fTop.fLeft.moveDelay -label "Move Delay" \
	-from -50 \
	-to 50 \
	-length 8c \
	-state disabled \
	-orient horizontal \
	-activebackground blue \
	-foreground gray \
	-variable gMoveDelay
    scale .middle.f2.fPlayOptions.fTop.fRight.gameDelay -label "Game Delay" \
	-from -50 \
	-to 50 \
	-length 8c \
	-state disabled \
	-orient horizontal \
	-activebackground blue \
	-foreground gray \
	-variable gGameDelay

    # pack in the contents in the correct order
    pack propagate .middle.f2.fPlayOptions.fTop.fLeft 0	
    pack propagate .middle.f2.fPlayOptions.fTop.fRight 0
    ## JESSE: packing scale bars
    #pack .moveDelay 
    #pack .gameDelay
    ## end Jesse
    pack .middle.f2.fPlayOptions.fTop -side top
    pack .middle.f2.fPlayOptions.fBot -side bottom   
    pack .middle.f2.fPlayOptions.fTop.fLeft -side left
    pack .middle.f2.fPlayOptions.fTop.fRight -side right
    ## moveDelay scale bar
    pack .middle.f2.fPlayOptions.fTop.fLeft.moveDelay -side bottom
    pack .middle.f2.fPlayOptions.fTop.fLeft.rHuman -side bottom -fill both -expand 1
    pack .middle.f2.fPlayOptions.fTop.fLeft.rComputer -side bottom -fill both -expand 1
    pack .middle.f2.fPlayOptions.fTop.fLeft.eName -side bottom -expand 1
    pack .middle.f2.fPlayOptions.fTop.fLeft.lName -side bottom  -expand 1
    ## gameDelay scale bar
    pack .middle.f2.fPlayOptions.fTop.fRight.gameDelay -side bottom
    pack .middle.f2.fPlayOptions.fTop.fRight.rHuman -side bottom -fill both -expand 1
    pack .middle.f2.fPlayOptions.fTop.fRight.rComputer -side bottom -fill both -expand 1
    pack .middle.f2.fPlayOptions.fTop.fRight.eName -side bottom -expand 1
    pack .middle.f2.fPlayOptions.fTop.fRight.lName -side bottom  -expand 1
    pack .middle.f2.fPlayOptions.fBot.bOk -side right -fill both -expand 1


    ##########
    ##
    ## Rules Frame 
    ##            
    ##########
	
    frame .middle.f2.fRules \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 2 / 30]
    
    set rulesFrame .middle.f2.fRules
    frame $rulesFrame.buttons
    
    pack propagate $rulesFrame 0
    
    button $rulesFrame.buttons.bCancel -text "Cancel" \
	-command {
	    pack forget .middle.f2.fRules
	    pack .middle.f2.cMain
	    .cToolbar raise iATB
            RaiseStatusBarIfGameStarted
	    update
	}
    button $rulesFrame.buttons.bOk -text "Start new game with above rule settings" \
        -command {
	    pack forget .middle.f2.fRules
	    pack .middle.f2.cMain
	    .cToolbar raise iATB
	    update
	    eval [concat C_SetGameSpecificOptions [GetAllGameOptions]]
	    C_InitializeGame
	    C_InitializeDatabases
	    set theValue [C_DetermineValue $gPosition]
	    TBaction1
	}

    pack $rulesFrame.buttons.bCancel -side left -fill both -expand 1
    pack $rulesFrame.buttons.bOk -side right -fill both -expand 1
    pack $rulesFrame.buttons -side bottom -fill x

    global gRuleset
    set ruleNum 0
    foreach rule $gRuleset {
	frame $rulesFrame.rule$ruleNum -borderwidth 2 -relief raised
	pack $rulesFrame.rule$ruleNum  -fill both -expand 1
	message $rulesFrame.rule$ruleNum.label -text [lindex $rule 0]
	pack $rulesFrame.rule$ruleNum.label -side left
	set rulePartNum 0
	global gRule$ruleNum
	set gRule$ruleNum 0
	foreach rulePart [lrange $rule 1 end] {
	    radiobutton $rulesFrame.rule$ruleNum.p$rulePartNum -text $rulePart -variable gRule$ruleNum -value $rulePartNum -highlightthickness 0
	    pack $rulesFrame.rule$ruleNum.p$rulePartNum -side left -expand 1 -fill both
	    incr rulePartNum
	}
	incr ruleNum
    }    

    #
    # Help Frame
    #

    frame .middle.f2.fHelp \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 2 / 30] 
    pack propagate .middle.f2.fHelp 0
    
    #    
    # About Frame
    #

    frame .middle.f2.fAbout \
	-width [expr $gWindowWidth * 10 / 16] \
	-height [expr $gWindowHeight * 2 / 30] 
    pack propagate .middle.f2.fAbout 0   

    # Help Button and about buttons
    button .middle.f2.fHelp.bReturn -text "Return" \
	-command {
	    pack forget .middle.f2.fHelp   
	    pack .middle.f2.cMain
	    .cToolbar raise iATB
	    RaiseStatusBarIfGameStarted
	    update
	}
    
    button .middle.f2.fAbout.bReturn -text "Return" \
	-command {
	    pack forget .middle.f2.fAbout   
	    pack .middle.f2.cMain
	    .cToolbar raise iATB
	    RaiseStatusBarIfGameStarted
	    update
	}
    pack .middle.f2.fHelp.bReturn -side right -fill both -expand 1
    pack .middle.f2.fAbout.bReturn -side right -fill both -expand 1

    # create the right hand frame
    canvas .middle.f3.cMRight -highlightthickness 0 \
	-bd 0 \
	-width [expr $gWindowWidth * 3 / 16] \
	-height [expr $gWindowHeight * 25 / 30] \
	-background red

    .middle.f1.cMLeft create image [expr $gWindowWidth * 3/32] 100 -image iIMB1 -tags [list  iIMB iIMB1]
    .middle.f1.cMLeft create image [expr $gWindowWidth * 3/32] 300 -image iIMB2 -tags [list  iIMB iIMB2]
    .middle.f1.cMLeft create image [expr $gWindowWidth * 3/32] 450 -image iIMB3 -tags [list  iIMB iIMB3]
    .middle.f1.cMLeft create image [expr $gWindowWidth * 3/32] 250 -image iAMB7 -tags [list detVal]
    .middle.f1.cMLeft create image [expr $gWindowWidth * 3/32] 250 -image iSMB7 -tags [list startupPic]
	    	
    .middle.f3.cMRight create image [expr $gWindowWidth * 3/32] 300 -image iAMB5 -tags [list  iAMB iAMB5]
    .middle.f3.cMRight create image [expr $gWindowWidth * 3/32] 100 -image iIMB4 -tags [list  iIMB iIMB4]
    .middle.f3.cMRight create image [expr $gWindowWidth * 3/32] 300 -image iIMB5 -tags [list  iIMB iIMB5]
    .middle.f3.cMRight create image [expr $gWindowWidth * 3/32] 450 -image iIMB6 -tags [list  iIMB iIMB6]
    .middle.f3.cMRight create image [expr $gWindowWidth * 3/32] 250 -image iAMB8 -tags [list play]

    .middle.f1.cMLeft create text 75 100 \
	    -text [format "To Win: %s" $kToWin] \
	    -width 140 \
	    -justify center \
	    -font $kLabelFont \
	    -anchor center \
	    -tags [list ToWin textitem]

    .middle.f1.cMLeft create text 75 300 \
	    -text [format "To Move: %s" $kToMove] \
	    -width 140 \
	    -justify center \
	    -font $kLabelFont \
	    -anchor center \
	    -tags [list ToMove textitem]

    .middle.f1.cMLeft create text 75 450 \
	    -text [format "Player1:\n%s" $gLeftName] \
	    -width 140 \
	    -justify center \
	    -font { Helvetica 18 bold } \
	    -anchor center \
	-tags [list LeftName Names textitem] \
	-fill $gLeftColor

    .middle.f3.cMRight create text 75 450 \
	    -text [format "Player2:\n%s" $gRightName] \
	    -width 140 \
	    -justify center \
	    -font { Helvetica 18 bold } \
	    -anchor center \
	    -tags [list RightName Names textitem] \
	-fill $gRightColor

    .middle.f3.cMRight create text 75 100 \
	    -text [format "Predictions: %s" $gPredString] \
	    -width 140 \
	    -justify center \
	    -font $kLabelFont \
	    -anchor center \
	    -tags [list Predictions textitem]

    .middle.f3.cMRight create text 75 90 \
	-text [format "It's %s's Turn" $gWhoseTurn] \
	-width 50 \
	-justify center \
	-font $kLabelFont \
	-anchor center \
	-tags [list WhoseTurn textitem]

    # this is the play button
    .middle.f3.cMRight bind play <1> {	
	pack forget .middle.f2.fPlayOptions
	pack .middle.f2.cMain -expand 1
	pack .middle.f2.fPlayOptions.fBot -side bottom
	.middle.f3.cMRight lower play
	.middle.f1.cMLeft lower detVal
	.middle.f1.cMLeft lower startupPic
	.middle.f2.cMain lower base
	.middle.f1.cMLeft lower detVal

	.cToolbar raise iATB

	pack .middle.f2.fPlayOptions.fBot -side bottom
	.cToolbar bind iOTB1 <Any-Leave> \
		".cToolbar raise iATB1"

	.cStatus lower base
	NewGame
	set gamestarted true
 
    }	
    .middle.f1.cMLeft lower detVal
    .middle.f3.cMRight lower play

    pack .middle.f1.cMLeft -expand 1
    pack .middle.f2.cMain  -expand 1
    pack .middle.f3.cMRight -expand 1

    pack .middle.f1 -side left -expand 1
    pack .middle.f2 -side left -expand 1
    pack .middle.f3 -expand 1

    #
    # Status Area
    #
    canvas .cStatus -highlightthickness 0 \
	-bd 0 \
	-width $gWindowWidth \
	-height [expr $gWindowHeight * 4 / 30] \
	-background yellow

    .cStatus create image 400 40 -image iBBB1 -tags [list iABB iABB1 base]
    .cStatus create image 120 40 -image iABB2 -tags [list sbb iABB iABB2 playA def]
    .cStatus create image 120 40 -image iIBB2 -tags [list sbb iIBB iIBB2 playI]
    .cStatus create image 290 22.5 -image iABB3 -tags [list sbb iABB iABB3 winA]
    .cStatus create image 290 22.5 -image iIBB3 -tags [list sbb iIBB iIBB3 winI def]
    .cStatus create image 290 57.5 -image iABB4 -tags [list sbb iABB iABB4 moveA]
    .cStatus create image 290 57.5 -image iIBB4 -tags [list sbb iIBB iIBB4 moveI def]
    .cStatus create image 410 22.5 -image iABB5 -tags [list sbb iABB iABB5 noneA]
    .cStatus create image 410 22.5 -image iIBB5 -tags [list sbb iIBB iIBB5 noneI def]
    .cStatus create image 470 22.5 -image iABB6 -tags [list sbb iABB iABB6 allA def]
    .cStatus create image 470 22.5 -image iIBB6 -tags [list sbb iIBB iIBB6 allI]
    .cStatus create image 530 22.5 -image iABB7 -tags [list sbb iABB iABB7 valueA]
    .cStatus create image 530 22.5 -image iIBB7 -tags [list sbb iIBB iIBB7 valueI def]
    .cStatus create image 470 57.5 -image iABB8 -tags [list sbb iABB iABB8 predA]
    .cStatus create image 470 57.5 -image iIBB8 -tags [list sbb iIBB iIBB8 predI def]
    .cStatus create image 680 40 -image iABB9 -tags [list sbb iABB iABB9 undoA def]
    .cStatus create image 680 40 -image iIBB9 -tags [list sbb iIBB iIBB9 undoI]    
    
    .middle.f2.cMain create image 250 250 -image iAMM1 -tags [list base iAMM iAMM1]

    .cStatus bind playA <ButtonRelease-1> {
	.cToolbar raise iITB
	.cStatus raise base
	SetupPlayOptions

    }

    .cStatus bind playI <ButtonRelease-1> {

	.cStatus raise iABB2
    }

    .cStatus bind winA <ButtonRelease-1> {
	.middle.f1.cMLeft raise iIMB1
	.cStatus raise iIBB3
	update
    }

    .cStatus bind winI <ButtonRelease-1> {
	.middle.f1.cMLeft raise ToWin
	.cStatus raise iABB3
	update
    }

    .cStatus bind moveA <ButtonRelease-1> {
	.middle.f1.cMLeft raise iIMB2
	.cStatus raise moveI
	update
    }

    .cStatus bind moveI <ButtonRelease-1> {
	.middle.f1.cMLeft raise ToMove
	.cStatus raise iABB4
	update
    }

    .cStatus bind noneA <ButtonRelease-1> {
    }

    .cStatus bind noneI <ButtonRelease-1> {
	.middle.f3.cMRight raise iIMB5
	.cStatus raise noneA
	.cStatus raise allI
	.cStatus raise valueI
    }
        
    .cStatus bind allA <ButtonRelease-1> {
    }

    .cStatus bind allI <ButtonRelease-1> {
	global gMoveType
	set gMoveType all
	ToggleMoves all
	.middle.f3.cMRight raise iIMB5
	.cStatus raise valueI
	.cStatus raise allA
	.cStatus raise noneI
    }

    .cStatus bind valueA <ButtonRelease-1> {
    }

    .cStatus bind valueI <ButtonRelease-1> {
	global gMoveType
	set gMoveType value
	ToggleMoves value
	.middle.f3.cMRight raise iAMB5
	.cStatus raise valueA
	.cStatus raise allI
	.cStatus raise noneI
    }

    .cStatus bind iABB8 <ButtonRelease-1> {
	set gPredictions false
	.middle.f3.cMRight raise iIMB4
	.middle.f3.cMRight lower Predictions
	.cStatus raise iIBB8
    }

    .cStatus bind iIBB8 <ButtonRelease-1> {
	.middle.f3.cMRight raise iIMB4
	.middle.f3.cMRight raise Predictions
	.cStatus raise iABB8
    }

    # Undo Button
    .cStatus bind iABB9 <ButtonRelease-1> {
	Undo
    }
    .cStatus raise sbb
    .cStatus raise def
    .cStatus raise base

    .middle.f1.cMLeft raise iIMB
    .middle.f1.cMLeft raise startupPic

    .middle.f3.cMRight lower textitem

    grid config .cToolbar -column 0 -row 1 \
        -columnspan 1 -rowspan 1 
    grid config .middle    -column 0 -row 2 \
        -columnspan 1 -rowspan 1 
    grid config .cStatus  -column 0 -row 3 \
        -columnspan 1 -rowspan 1

    GS_Initialize .middle.f2.cMain

}

proc RaiseStatusBarIfGameStarted {} {
    global gGameSolved
    if { $gGameSolved == "true" } {
	.cStatus lower base
    }
}
