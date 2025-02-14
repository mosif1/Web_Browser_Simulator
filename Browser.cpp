/*****************************************
** File: Browser.cpp
** Author: Mosi Farquharson
** Date: 12/1/24
** E-mail: mosif1@umbc.edu
*
** This file contains the main logic for the browser class for browser Simulatoras well as main logic
** for the overall project
**
**
***********************************************/


#include "Browser.h"

//Browser
//overloaded constrcutor for the browser
Browser::Browser(string filename){
    m_fileName = filename;
    m_currentPage = nullptr;
}

//~Browser
//Destructor for browser
Browser::~Browser(){
    delete m_currentPage;

    //iterates backstack until its empty and deletes
    while (m_backStack.GetSize() != 0){
        NavigationEntry* delPtr = m_backStack.Pop();
        delete delPtr;
    }

    //iterates forwardstack until empty and deletes
    while (m_forwardStack.GetSize() != 0){
        NavigationEntry* delPtr = m_forwardStack.Pop();
        delete delPtr;
    }
}

//GetCurrentPage
//returns the currentpage pointer as a value
NavigationEntry Browser::GetCurrentPage() const
{
    return *m_currentPage;
}


//Loadfile
//Loads the main file of the program
void Browser::LoadFile(){
    string line;
    fstream inpustream;

    //opens the text file
    inpustream.open(m_fileName);

    string URL;
    string UnixTimestamp;
    //checks if the file doesnt open
    if (!inpustream.is_open()){
        cout << "file not opened" << endl;
    }

    //reads the line until it gets to the delimiter
    while(getline(inpustream, URL, DELIMITER)){
        //uses inputstream and extraction operator to read the rest of the line
        if (inpustream >> UnixTimestamp){
            int timestamp = stoi(UnixTimestamp);
            //creates new site
            Visit(URL, timestamp);
        }
    }

    inpustream.close();

}

//StartBrowser
//Starts the program
void Browser::StartBrowser(){
    LoadFile();

    Menu();
}

//Menu
//Main meny of the program
void Browser::Menu(){
    int pick = 0;

    cout << "Welcome to the browser History Simulator" << endl;
    //main menu of the site, only moves forward abd back once
    do{
        
        cout << "Current Website:  " << endl;
        cout << *m_currentPage << endl;
        cout << "What would you like to do? " << endl;
        cout << "1. Display Browser History" << endl;
        cout << "2. Go Back" << endl;
        cout << "3. Go forward" << endl;
        cout << "4. Visit site" << endl;
        cout << "5. Quit" << endl;
        cin >> pick;

        if (pick == 1){
            Display();
        }else if (pick == 2){
            Back(1);
        }else if (pick == 3){
            Forward(1);
        }else if (pick == 4){
            NewVisit();
        }else if (pick == 5){
            cout << "Thank you for browsing" << endl;
        }
    }while(pick != 5);
}


//Visit
//creates new a website either manually or by text file
void Browser::Visit(const string& url, int timestamp){
    //creates new navigation entry
   NavigationEntry* NE = new NavigationEntry(url, timestamp);
   if (m_currentPage == nullptr){
    m_currentPage = NE;
   }else{
    //if the currentpage is occupied it adds it to the backstack and the new site becomse the current page
    m_backStack.Push(m_currentPage);
    m_currentPage = NE;
   }
}


//NewVisit
//creates a new site with a real-time timestamp
void Browser::NewVisit(){
    string URL;
    cout << "Enter the URL of the new site: " << endl;
    cin >> URL;
    auto now = chrono::system_clock::now();
    time_t nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    int64_t timestamp = static_cast<int64_t>(nowAsTimeT);
    //creates a new site using visit
    Visit(URL, timestamp);
}

//Display
//Displays each stack
void Browser::Display(){
    
    cout << "**Back Stack**" << endl;
    //checks if backstack is empty
    if (m_backStack.IsEmpty()){
        cout << "Empty" << endl;
    }else{
    //counter to use as iterator
    size_t counter = 0;
    //iterates over the backstack and uses .At to display
    while (counter < m_backStack.GetSize()){
        cout << counter + 1 << ". "<< *(m_backStack.At(counter)) << endl;
        counter++;
    }
    }

    cout << endl;

    cout << "**Forward Stack**" << endl;
    //checks if the forwardstack is empty
    if (m_forwardStack.IsEmpty()){
        cout << "Empty" << endl;
    }else{
    size_t counter = 0;
    //same logic as the backstack
    while (counter < m_forwardStack.GetSize()){
        cout << counter + 1 << ". " << *(m_forwardStack.At(counter)) << endl;
        counter++;
    }
}
}

//Back
//moves item from backstack to the current page
NavigationEntry Browser::Back(int steps){
    int counter = 0;
    //of the backstack is empty it does nothing
    if (m_backStack.IsEmpty()){
        return GetCurrentPage();
    }

    //loop to add make the backstack the current page, uses a counter to do it as many times as the steps
    while (counter < steps){
    //creates a new page that is the same as the current page and adds to the stack
    NavigationEntry* tempPage = new NavigationEntry(m_currentPage->GetURL(), m_currentPage->GetTimeStamp());
    m_forwardStack.Push(tempPage);
    delete m_currentPage;
    //takes the data from the backstack and makes it the new current page
    NavigationEntry* data = m_backStack.Pop();
    m_currentPage = data;
    data = nullptr;
    delete data;
    tempPage = nullptr;
    delete tempPage;
    counter++;
    }

    //after loop updates and returns the page
    return GetCurrentPage();
}

//Forward
//moves item from forwardstack to current page
NavigationEntry Browser::Forward(int steps){
    int counter = 0;
    if (m_forwardStack.IsEmpty()){
        return GetCurrentPage();
    }
    while (counter < steps){
        //makes copy of the current page
        NavigationEntry* tempPage = new NavigationEntry(m_currentPage->GetURL(), m_currentPage->GetTimeStamp());
        m_backStack.Push(tempPage);
        delete m_currentPage;
        //takes the data from the top of the forwardstack and makes the current page that data
        NavigationEntry* data = m_forwardStack.Pop();
        m_currentPage = data;
        data = nullptr;
        delete data;
        tempPage = nullptr;
        delete tempPage;
        counter++;
    }
    //returns updated page
    return GetCurrentPage();
}

