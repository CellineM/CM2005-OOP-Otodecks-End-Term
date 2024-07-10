/*
  ==============================================================================

    PlaylistComponent.h
    Created: 16 Aug 2023 12:27:39pm
    Author:  celli

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
//#include "DeckGUI.h"
// #include "DJAudioPlayer.h"
//#include "WaveformDisplay.h"



//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public TextEditor::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    void textEditorTextChanged(TextEditor& editor) override;
    void deleteSong();
    void uploadToDeckGUI1();
    void uploadToDeckGUI2();
    String fileToDeckGUI;
    std::string stdstring;
    

private:
    TextButton libraryLoadButton{ "Load into library" };
    TextButton libDelete; // library delete
    TextButton intoDeckGUI1;
    TextButton intoDeckGUI2;

    AudioFormatManager formatManager;
    Array<File> myFiles;
    std::vector<std::string> existFile;
    double time;
    String fileName;
    Array<double> myFilesTime;
    Array<String> fileNames;
    TableListBox tableComponent;
    
    bool historyClicked = false;
    int repeatedIndex = -1;

    TextEditor lookForSong{ "Seach for Songs" };
    String searchInput;

    double rowclicked;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};