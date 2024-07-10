/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    // adding the image
    auto playPic = ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);
    auto stopPic = ImageCache::getFromMemory(BinaryData::stopbutton_png, BinaryData::stopbutton_pngSize);
    auto loadPic = ImageCache::getFromMemory(BinaryData::uploading_png, BinaryData::uploading_pngSize);
   // auto vinylPic = ImageCache::getFromMemory(BinaryData::vinyldisc_jpg, BinaryData::vinyldisc_jpgSize);

    // make the GUI visible to the screen
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);
   // addAndMakeVisible(blackVinyl);
    addAndMakeVisible(songName);

    addAndMakeVisible(roomsizeSlider);
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(wetGainSlider);
    addAndMakeVisible(dryGainSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    roomsizeSlider.addListener(this);
    dampingSlider.addListener(this);
    wetGainSlider.addListener(this);
    dryGainSlider.addListener(this);

    // instead of button, change to image button
    playButton.setImages(true, true, true, playPic, 0.5f, Colours::transparentBlack,
                        playPic, 1.0f, juce::Colours::transparentBlack,
                        playPic, 0.5f, juce::Colours::transparentBlack, 0.0f);

    stopButton.setImages(true, true, true, stopPic, 0.5f, Colours::transparentBlack,
                        stopPic, 1.0f, juce::Colours::transparentBlack,
                        stopPic, 0.5f, juce::Colours::transparentBlack, 0.0f);

    loadButton.setImages(true, true, true, loadPic, 0.5f, Colours::transparentBlack,
                        loadPic, 1.0f, juce::Colours::transparentBlack,
                        loadPic, 0.5f, juce::Colours::transparentBlack, 0.0f);

    //blackVinyl.setToggleState(true, NotificationType::dontSendNotification);
    //blackVinyl.setImages(true, true, true, vinylPic, 1.0, Colours::transparentBlack,
                        //vinylPic, 1.0, juce::Colours::transparentBlack,
                        //vinylPic, 1.0, juce::Colours::transparentBlack);

    // make the slider in a rotary slider instead of a straight line
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20); // show the range of the slider
    volSlider.setColour(Slider::rotarySliderFillColourId, Colours:: peachpuff); // colour of the slider changed when moved

    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
    speedSlider.setColour(Slider::rotarySliderFillColourId, Colours::darkred);

    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    posSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
    posSlider.setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);

    // slider in horizontal line
    roomsizeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    roomsizeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // no text box to show the range of the slider
    roomsizeSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(131, 238, 255)); // change the bg colour of the slider
    roomsizeSlider.setColour(Slider::thumbColourId, Colour::fromRGB(57, 255, 20)); // change the colour of the slider handle 
    
    dampingSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dampingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    dampingSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(131, 238, 255));
    dampingSlider.setColour(Slider::thumbColourId, Colour::fromRGB(57, 255, 20));

    wetGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal); 
    wetGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    wetGainSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(131, 238, 255));
    wetGainSlider.setColour(Slider::thumbColourId, Colour::fromRGB(57, 255, 20));

    dryGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dryGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    dryGainSlider.setValue(0.5); // set the value in the middle of the slider = the normal sound
    dryGainSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(131, 238, 255));
    dryGainSlider.setColour(Slider::thumbColourId, Colour::fromRGB(57, 255, 20));

    // text for the sliders
    volSliderName.setText("Volume", NotificationType::dontSendNotification);
    volSliderName.attachToComponent(&volSlider, false);
    speedSlidername.setText("Speed", NotificationType::dontSendNotification);
    speedSlidername.attachToComponent(&speedSlider, false);
    posSliderName.setText("Position", NotificationType::dontSendNotification);
    posSliderName.attachToComponent(&posSlider, false);


    // song name showed when a song from the playlist is selected
    songName.setReadOnly(true);
    songName.setTextToShowWhenEmpty("Song? :(", Colours::black);

    // Range of the sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);
    roomsizeSlider.setRange(0.0, 1.0);
    dampingSlider.setRange(0.0, 1.0);
    wetGainSlider.setRange(0.0, 1.0);
    dryGainSlider.setRange(0.0, 1.0);

    // timer loops every 100ms
    startTimer(100);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    juce::Colour pastelPink(251, 111, 146);
    g.fillAll(pastelPink);

    g.setColour (Colours::mediumvioletred);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont (14.0f);
    //g.drawText ("DeckGUI", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
    g.drawText("", getLocalBounds(),
        Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 12; 

    int gliderWidth = getWidth() / 3; // how many glider can fit in a row 
    int gliderHeight = getHeight() / 3.3; // how big are the gliders

    playButton.setBounds(0, 10, getWidth()/6, rowH);
    loopButton.setBounds((getWidth() / 8) * 4, 10, getWidth() / 7, rowH);
    stopButton.setBounds(getWidth() / 6, 10, getWidth() / 7, rowH);
    loadButton.setBounds((getWidth() / 8) * 2.5, 10, getWidth() / 7, rowH);

    int songNameWidth = getWidth() / 2; // Adjust the width as needed
    songName.setBounds(10, 45, songNameWidth, rowH);

    waveformDisplay.setBounds(0, rowH * 2.3, getWidth(), rowH * 2.3);

    volSlider.setBounds(0, rowH * 7.5, gliderWidth, gliderHeight);
    speedSlider.setBounds(gliderWidth, rowH * 7.5, gliderWidth, gliderHeight);
    posSlider.setBounds(gliderWidth * 2, rowH * 7.5, gliderWidth, gliderHeight);

    // reverb slider
    roomsizeSlider.setBounds(0, rowH * 3.5, gliderWidth, rowH * 3);
    dampingSlider.setBounds(gliderWidth, rowH * 3.5, gliderWidth, rowH * 3);
    wetGainSlider.setBounds(gliderWidth/2, rowH * 5, gliderWidth, rowH * 3);
    dryGainSlider.setBounds((gliderWidth / 2) * 3, rowH * 5, gliderWidth, rowH * 3);

    //blackVinyl.setBounds(gliderWidth * 2, rowH * 5.5, gliderWidth, gliderHeight);

    // position of the slider text
    // adjust the position of the text to be in the centre of the slider
    volSliderName.setCentrePosition(volSlider.getX() + volSlider.getWidth() / 2 + 75,
    volSlider.getY() + volSlider.getHeight() / 2 + 10);
    speedSlidername.setCentrePosition(speedSlider.getX() + speedSlider.getWidth() / 2 + 79,
    speedSlider.getY() + speedSlider.getHeight() / 2 + 10);
    posSliderName.setCentrePosition(posSlider.getX() + posSlider.getWidth() / 2 + 76,
    posSlider.getY() + posSlider.getHeight() / 2 + 10);


    // colour of the songName
    songName.setColour(TextEditor::backgroundColourId, Colours::transparentWhite);
    songName.setColour(TextEditor::outlineColourId, Colours::transparentWhite);
}







void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
     
     
   /*    if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()});
            DBG(URL{ chooser.getResult() }.getFileName());

            songName.setText(URL{ chooser.getResult() }.getFileName(),
                NotificationType::dontSendNotification);
        });
    }*/
     if (button == &loadButton)
     {
         FileChooser chooser{"Select a file..."};
         if (chooser.browseForFileToOpen())
         {
             player->loadURL(URL{chooser.getResult()});
             waveformDisplay.loadURL(URL{chooser.getResult()});
             DBG(URL{ chooser.getResult() }.getFileName());

             songName.setText(URL{ chooser.getResult() }.getFileName(), NotificationType::dontSendNotification);
            
         }


     }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &roomsizeSlider)
    {
        player->setRevRoomsize(slider->getValue());
    }

    if (slider == &dampingSlider)
    {
        player->setRevDamping(slider->getValue());
    }

    if (slider == &wetGainSlider)
    {
        player->setRevWetGain(slider->getValue());
    }

    if (slider == &dryGainSlider)
    {
        player->setRevDryGain(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{   // looping every 100ms
    if (std::to_string(loopButton.getToggleState()) == "1") //  1 = ON
    {
        if (player->getPositionRelative() >= 1) // >1 = the end of the song 
        {
            player->setPositionRelative(0); // sets it back to the beginning of the song
            player->start(); // start the song
        }
    }
    
    if (std::to_string(loopButton.getToggleState()) == "0") //  0 = OFF
    {
         if (player->getPositionRelative() >= 1) // >1 = the end of the song 
         {
                player->setPositionRelative(0); // sets it back to the beginning of the song
                player->stop(); // stop the song
         }

    }

    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

void DeckGUI::playlistToDeckGUI()
{
    std::ifstream file("songList.txt"); // open the songlist.txt which contains the songs
    std::string str;
    std::getline(file, str);
    std::string URL = "file:///" + str;
    DBG(URL);
    juce::URL audioURL{ URL };
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    DBG(audioURL.getFileName());

    mtrackName = audioURL.getFileName();

    songName.setText(mtrackName, NotificationType::dontSendNotification);
}
    

