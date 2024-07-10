/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 16 Aug 2023 12:27:39pm
    Author:  celli

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"



//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    /*
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    */

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // create columns for each component 
    tableComponent.getHeader().addColumn("Track title", 1, 300);
    tableComponent.getHeader().addColumn("Tick-Tock", 2, 200);
    tableComponent.getHeader().addColumn("DGUI 1 :)", 3, 150); // DGUI1 = deck gui1
    tableComponent.getHeader().addColumn("DGUI 2 :)", 4, 150);
    tableComponent.getHeader().addColumn("Delete :(", 5, 100);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(libraryLoadButton);
    addAndMakeVisible(lookForSong);

    libraryLoadButton.addListener(this);
    formatManager.registerBasicFormats();
    lookForSong.addListener(this);

    // produce the songList.txt add it into placeholder 0 (location of the deckGUI)
    // import the song to deckGUI1 or deck GUI2
    std::ofstream myfile("songList.txt");
    myfile << stdstring << std::endl << "0" << std::endl;
    myfile.close();

    // make a txt file called songListHistory.txt 
    // store the current song file
    std::ifstream file("songListHistory.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }

    // if text file contains audio file URLs, add each line into an existing files array
    for (int i = 0; i < lines.size(); ++i)
    {
        existFile.push_back(lines[i]);
    }

    // if there are existing files before
    if (existFile.size() != 0)
    {
        // add the existing files into my file
        for (int i = 0; i < existFile.size(); ++i)
        {
            myFiles.add(File{ existFile[i] });
        }

        for (int i = 0; i < myFiles.size(); ++i)
        {
            fileName = URL::removeEscapeChars(URL{ myFiles[i] }.getFileName());
            fileNames.add(fileName);
            AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]);
            time = reader->lengthInSamples / reader->sampleRate;
            myFilesTime.add(time);
        }
    }

    tableComponent.updateContent();

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colour::fromRGB(202, 161, 193));
    //g.setColour(juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


    /* g.fillAll(Colour::fromRGB(200, 161, 193));*/
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 7;
    libraryLoadButton.setBounds(0, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), getHeight());
    lookForSong.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    
    tableComponent.getHeader().setColumnWidth(1, (getWidth() / 8) * 3);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 8);
    tableComponent.getHeader().setColumnWidth(3, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(4, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(5, (getWidth() / 8));

    tableComponent.setColour(ListBox::backgroundColourId, Colours::deeppink);

    lookForSong.setTextToShowWhenEmpty("Seach for Songs", Colours::whitesmoke);
    lookForSong.setFont(15.0f);
}

int PlaylistComponent::getNumRows() {
    return fileNames.size();
}
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(Colours::paleturquoise); 
    }
    else {
        g.fillAll(Colours::lightpink); 
    }
}
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1) // show the song name
    {
        g.drawText(fileNames[rowNumber], 2, 0,
            width - 4, height, Justification::centredLeft, true);
    }
    if (columnId == 2) // show the time for the songs
    {
        std::time_t seconds(myFilesTime[rowNumber]);
        tm* p = gmtime(&seconds);
        std::string hhmmss;
        std::string hrs;
        std::string mins;
        std::string secs;

        if (p->tm_hour < 10)
        {
            hrs = "0" + std::to_string(p->tm_hour);
        }
        else
        {
            hrs = std::to_string(p->tm_hour);
        }

        if (p->tm_min < 10)
        {
            mins = "0" + std::to_string(p->tm_min);
        }
        else
        {
            mins = std::to_string(p->tm_min);
        }

        if (p->tm_sec < 10)
        {
            secs = "0" + std::to_string(p->tm_sec);
        }
        else
        {
            secs = std::to_string(p->tm_sec);
        }

        hhmmss = hrs + ":" + mins + ":" + secs;
        g.drawText(hhmmss, 2, 0, width - 4, height,
            Justification::centredLeft, true);

    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 3) // load the song to deckGUI1
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "DeckGUI1" }; // button created named DeckGUI1
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id); // determine which button was click. Have an id assigned to the button
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {uploadToDeckGUI1(); }; // when this button is clicked, the song will be loaded to deckGUI1

        }
    }

    if (columnId == 4) // load the song to deckGUI2
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "DeckGUI2" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {uploadToDeckGUI2(); };

        }
    }

    if (columnId == 5) // delete the song
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "x_x" }; // create a button for delete
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteSong(); }; // the song is deleted when clicked

        }
    }

    return existingComponentToUpdate;

}
void PlaylistComponent::buttonClicked(Button* button) 
{

    if (button == &libraryLoadButton)
    {
        FileChooser chooser{ "Select song -> library", File(), "*.mp3;*.wav;*.aiff" };
        if (chooser.browseForMultipleFilesToOpen())
        {
            myFiles = chooser.getResults();
            
            for (int i = 0; i < myFiles.size(); ++i)
            {
                if (existFile.size() != 0 || i == 0) 
                {
                    for (int j = 0; j < existFile.size(); ++j)
                    {
                        if (myFiles[i].getFullPathName().toStdString() == existFile[j])
                        {
                            DBG("There's Duplicates");

                            AlertWindow alertCopy("Extra copy file added", "Duplicated file " +
                                URL::removeEscapeChars(juce::URL{ myFiles[i] }.getFileName()) + " has not been added",
                                MessageBoxIconType::InfoIcon);
                            alertCopy.addButton("Continue", 0);
                            alertCopy.setUsingNativeTitleBar(true);

                            int result = alertCopy.runModalLoop();

                            repeatedIndex = i;
                            break;
                        }
                    }

                    if (i != repeatedIndex)
                    {
                        fileName = URL::removeEscapeChars(URL{ myFiles[i] }.getFileName());
                        
                        // Use an AudioFormatReader pointer to read the audio file
                        std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(myFiles[i]));

                        if (reader)
                        {
                            // Successfully created reader, proceed with processing
                            fileNames.add(fileName);
                            time = reader->lengthInSamples / reader->sampleRate;
                            myFilesTime.add(time);
                            DBG(fileNames[i]);
                            DBG(time);
                            existFile.push_back(myFiles[i].getFullPathName().toStdString());
                        }
                        else
                        {   // Failed to create reader, handle the error
                            DBG("Failed to create reader for: " + myFiles[i].getFullPathName());
                        }
                        
                        //fileNames.add(fileName);
                        //AudioFormatManager* reader = formatManager.createReaderFor(myFiles[i]);
                        //time = reader->lengthInSamples / reader->sampleRate;
                        //myFilesTime.add(time);
                        //DBG(fileNames[i]);
                        //DBG(time);

                        //existFile.push_back(myFiles[i].getFullPathName().toStdString());

                    }
                }
            }

            std::ofstream history("songListHistory.txt");

            for (int i = 0; i < existFile.size(); ++i)
            {
                history << existFile[i] << std::endl;
            }
            history.close();

            tableComponent.updateContent();
        }
                

            
    }

    rowclicked = button->getComponentID().getDoubleValue();
    //int id = std::stoi(button->getComponentID().toStdString());
    //std::cout << "PlaylistComponent::buttonClicked trackTitles id: " << trackTitles[id] << std::endl;
}

void PlaylistComponent::textEditorTextChanged(TextEditor& editor)
{
    searchInput = lookForSong.getText();

    for (int i = 0; i < fileNames.size(); ++i)
    {
        if (fileNames[i].containsWholeWord(searchInput) && searchInput != "")
        {
            tableComponent.selectRow(i, false, true);
        }
    }
}

// This deleteSong function will only operate when the user decide to delete the song
void PlaylistComponent::deleteSong()
{
    DBG(rowclicked);

    for (int i = 0; i < myFiles.size(); ++i)
    {
        if (rowclicked == i)
        {
            // when the user press delete, a pop up will show to ask the user if the user is sure that they want to delete the song
            AlertWindow confirmDelete("Delete Song", "Are you sure?", MessageBoxIconType::QuestionIcon);
            confirmDelete.setUsingNativeTitleBar(true);
            // option given
            confirmDelete.addButton("Yes", true);
            confirmDelete.addButton("No", false);

            bool result = confirmDelete.runModalLoop();

            if (result == true) // if the result is true which is 'yes' the song will be deleted from the playlist table component
            {
                DBG("Song Deleted :<");
                fileNames.remove(i);
                myFilesTime.remove(i);
                existFile.erase(existFile.begin() + i);
            }
            else
            {
                // nothing happen to the current playlist as the user click on the 'No' button
                confirmDelete.exitModalState(true);
            }
        }
    }

    // update the current songs
    std::ofstream history("songListHistory.txt");

    for (int i = 0; i < existFile.size(); ++i)
    {
        history << existFile[i] << std::endl;
    }

    history.close();

    tableComponent.updateContent();
}

void PlaylistComponent::uploadToDeckGUI1() { //only runs when user clicks on "Deck 1" button on the file
    for (int i = 0; i < myFiles.size(); ++i) { //Loops through myFiles
        juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]); //creates reader for the file
        if (rowclicked == i) { //Matches the row(file) which the user clicked to be added into Deck 1
            DBG("Load into deckGUI1");
            DBG(myFiles[i].getFileName());
            DBG(reader->getFormatName());
            fileToDeckGUI = myFiles[i].getFullPathName();//Gets the path of the song you chose
            stdstring = fileToDeckGUI.toStdString();//Converts juce string into std string of the path of the song
            std::replace(stdstring.begin(), stdstring.end(), '\\', '/');// change the \\ to /. only / is allowed to be read as the path of the file
            DBG(stdstring);

            std::ofstream myfile("songList.txt");// songlist.txt open
            myfile << stdstring << std::endl << "1" << std::endl; //
            myfile.close(); //closes the text file
        }
    }
}
void PlaylistComponent::uploadToDeckGUI2() { //only runs when user clicks on "Deck 2" button on the file
    for (int i = 0; i < myFiles.size(); ++i) {
        juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]);
        if (rowclicked == i) {
            DBG("Load into deckGUI2");
            DBG(myFiles[i].getFileName());
            DBG(reader->getFormatName());
            fileToDeckGUI = myFiles[i].getFullPathName();
            stdstring = fileToDeckGUI.toStdString();
            std::replace(stdstring.begin(), stdstring.end(), '\\', '/');
            DBG(stdstring);
            std::ofstream myfile("songList.txt");
            myfile << stdstring << std::endl << "2" << std::endl; 
            myfile.close();


        }
    }
}


