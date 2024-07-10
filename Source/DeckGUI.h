/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 
    void playlistToDeckGUI();

private:

    //TextButton playButton{"PLAY"};
    //TextButton stopButton{ "STOP" };
    //TextButton loadButton{ "LOAD" };
    
    // the button in Image
    ImageButton playButton{ "PLAY" };
    ImageButton stopButton{"STOP"};
    ImageButton loadButton{"LOAD"};

    ToggleButton loopButton{ "LOOP" };

    //ImageButton blackVinyl{ "VINYL" };
  
    // create slider
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    // create a label for the name of the rotary slider
    Label volSliderName;
    Label speedSlidername;
    Label posSliderName;

    // create slider for the reverb
    Slider roomsizeSlider;
    Slider dampingSlider;
    Slider wetGainSlider;
    Slider dryGainSlider;

    TextEditor songName;
    String mtrackName;

    // FileChooser fChooser{"Select a file..."};


    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
