/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();

    startTimer(500);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    juce::Colour pastelPink(251, 111, 146);
    g.fillAll(pastelPink);

   
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight()/1.5 );
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/1.5);
    playlistComponent.setBounds(0, 1.5 * getHeight() / 2.5, getWidth(), getHeight() / 2.5);
}

void MainComponent::buttonClicked(juce::Button* button) {

}

void MainComponent::sliderValueChanged(juce::Slider* slider) {

}

void MainComponent::timerCallback() { // check if the song is loaded into the deckGUI1/2

    std::ifstream file("songList.txt"); // open the file
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str)) //read the line in the songlist file
    {
        lines.push_back(str);
    }
    // if a song is added into the songlist file, it will show the song file URL and the number indicate which deckGUI the user has clicked on
    if (std::stoi(lines[1]) == 1) { 
        deckGUI1.playlistToDeckGUI(); //play the song in deckGUI1
        std::ofstream myfile("songList.txt");
        myfile << "" << std::endl << "0" << std::endl; //reser the songlist and stopping the time
        myfile.close(); //file close
    }
    else if (std::stoi(lines[1]) == 2) { 
        deckGUI2.playlistToDeckGUI(); 
        std::ofstream myfile("playlist.txt");
        myfile << "" << std::endl << "0" << std::endl; 
        myfile.close(); 
    }

}