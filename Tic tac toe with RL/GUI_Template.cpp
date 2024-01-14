// GUI_Template.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GUI_Template.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
// for rand()
#include <cstdlib>
// for time() to seed the rng
#include <ctime>  
#include <cstdlib>
//used for sleep functions
#include <chrono>
#include <thread>
//file IO
#include <fstream>
#include <sstream>
#include <set>
#include <cstring>

using namespace std;

#define MAX_LOADSTRING 100


//holds the current state of board
string boardState = "         ";
//starting variables determined at start
//token X is first move
char playerToken = ' ';
char botToken = ' ';
//flag used to know when valid input occurs!
int keyInput = -1;
bool validInput = false;

//helps determine which oponent plays their turn, and selects the different end game options.
char UpdateSection = 'M';

bool BotTrain = false;
int trainingTime = 0;

int winCount = 0;
int lossCount = 0;
int tieCount = 0;

int playerCondition = 0;
int botCondition = 0;
int endFlag = 0;
// Assuming standard ASCII characters
const int KEY_COUNT = 256; 
//using an array of bools to define the switchs for the ascii chars pressed on keybooard.
bool keys[KEY_COUNT] = { false }; // Initialize all keys as released

int NodeCount = 0;



/*
handles the input of simulation.
*/
class Menu
{


public:
    //changing input variables : 
    int input1 = 0;
    int input2 = 0;
    bool toggleLabel = true;
    //function used to handle keyboard inputs!
    void InputOpt(char input) {

        static bool keyState[KEY_COUNT] = { false }; // State of keys to prevent multiple detections

        // Check if the key corresponding to the input character is pressed

        /*single keypress*/
        if (keys[input])
        /**/
        {
            // Check if the key is not previously pressed
            if (!keyState[input])
            {
                keyState[input] = true; // Mark key as pressed

                {
                    /*PLAYER INPUT UPDATING*/
                   
                    //letters and numbers can be handled here!
                    if (keys['W'] || keys['w']) input = 'w';
                    else if (keys['S'] || keys['s']) input = 's';
                    else if (keys['A'] || keys['a']) input = 'a';
                    else if (keys['D'] || keys['d']) input = 'd';
                    else if (keys['^']) input = '^';
                    else if (keys['Q'] || keys['q']) input = 'q';
                    else if (keys['E'] || keys['e']) input = 'e';


                    //input handled with switch
                    switch (input)
                    {
                        //input 1
                    case '1'://go forward
                    {
                        keyInput = 0;
                        break;
                    }
                    case '2': //go back
                    {
                        keyInput = 1;
                        break;
                    }//turn left
                    case '3':
                    {
                        
                        keyInput = 2;
                        break;
                    }
                    //turn right
                    case '4':
                    {
                        keyInput = 3;
                        break;
                    }                    
                    //menu options
                    case '5':
                    {
                        keyInput = 4;
                        break;
                    }
                    case '6': 
                    {
                        keyInput = 5;
                        break;
                    }
                    case '7':
                    {
                        keyInput = 6;
                        break;
                    }
                    case '8':
                    {
                        keyInput = 7;
                        break;
                    }
                    case '9':
                    {
                        keyInput = 8;
                        break;
                    }
                    default:
                    {
                        keyInput = -1;
                    }
                    }

                }
            }
        }
        /*single keypress*/
        else
        {
            // Key is released, reset keyState for the released key
            keyState[input] = false;
        }
        /**/
    }
}; Menu m;//global menu




// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUITEMPLATE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUITEMPLATE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //KEYBOARD INPUTS
        m.InputOpt('0');
        m.InputOpt('1');
        m.InputOpt('2');
        m.InputOpt('3');
        m.InputOpt('4');
        m.InputOpt('5');
        m.InputOpt('6');
        m.InputOpt('7');
        m.InputOpt('8');
        m.InputOpt('9');
        
        m.InputOpt('w');
        m.InputOpt('s');
        m.InputOpt('a');
        m.InputOpt('d');

        m.InputOpt('W');
        m.InputOpt('S');
        m.InputOpt('A');
        m.InputOpt('D');



    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUITEMPLATE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUITEMPLATE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/*
function used to draw line from point A to B and assigns a color to it.

*/
void DrawLine(HDC hdc, int x0, int y0, int x1, int y1, COLORREF color) {
    // Create a pen with the specified color
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    SelectObject(hdc, hPen);

    // Draw the line
    MoveToEx(hdc, x0, y0, NULL);
    LineTo(hdc, x1, y1);

    // Clean up
    DeleteObject(hPen);
}

/*
selects image according to its number placement.
*/
LPCWSTR getImage(int imageNumber) {
    LPCWSTR imagePath = nullptr; // Define the image path pointer

    
    switch (imageNumber) {
    case 0:
        imagePath = L"Images//board.bmp"; 
        break;
    case 1:
        imagePath = L"Images//X.bmp"; 
        break;
    case 2:
        imagePath = L"Images//O.bmp"; 
        break;
       
    default:
        
        break;
    }

    return imagePath;
}
/*
takes an image path and sets it to a frame buffer.
*/
void DrawImage(HDC hdc, int x, int y, int width, int height, LPCWSTR imagePath) {
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap != NULL) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

        BITMAP bm;
        GetObject(hBitmap, sizeof(bm), &bm);
        //scaling is handled with stretch function.
        StretchBlt(hdc, x, y, width, height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        DeleteObject(hBitmap);
    }
}
// function for displaying text at a certain location!
void DrawText(HDC hdc, int x, int y, const std::string& text)
{
    // Select a font
    HFONT hFont = CreateFont(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"
    );
    HFONT hOldFont = static_cast<HFONT>(SelectObject(hdc, hFont));

    // Set text color
    SetTextColor(hdc, RGB(0, 0, 0)); // Black color

    // Draw the text on the HDC buffer
    TextOutA(hdc, x, y, text.c_str(), static_cast<int>(text.length()));

    // Clean up
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}
//ADD PLAYER AND BOT CLASSES HERE!!!



class ticTacToe
{

public:
   
    
   /*
   * first check if all moves are taken.
   checks every three in a row possible for a given token input.


   3 = win
   2 = tie
   1 = loss
   0 = NA
   */
    static int  endGame(char token)
    {

        //checking for win
        if(
            //check rows
            boardState[0] == token && boardState[1] == token && boardState[2] == token ||
            boardState[3] == token && boardState[4] == token && boardState[5] == token ||
            boardState[6] == token && boardState[7] == token && boardState[8] == token ||
            //check columns
            boardState[0] == token && boardState[3] == token && boardState[6] == token ||
            boardState[1] == token && boardState[4] == token && boardState[7] == token ||
            boardState[2] == token && boardState[5] == token && boardState[8] == token ||
            //check diagonals
            boardState[0] == token && boardState[4] == token && boardState[8] == token ||
            boardState[2] == token && boardState[4] == token && boardState[6] == token 
            )
        {
           
            //win
            UpdateSection = 'E';
            return 3;
        }
        //check for tie
        /**/
        for (int i = 0; i < boardState.size(); i++)
        {
            if (boardState[i] == ' ')
            {

                return 0;//not done
            }
        }
            
            //if no blanks games a tie.
            UpdateSection = 'E';
            return 2;
        
        /**/
        UpdateSection = 'E';
        return 1;//loss
    }


    /*
       player struct is used to handle a single player and what piece thy play
       while also making sure all moves are legal!
    */
    struct Player
    {
        /*
            function checks if player input is valid.
        */
        bool legalMove()
        {
            if (keyInput >= 0 && boardState[keyInput] == ' ')
            {            
                 return true;
            }
            return false;
        }
        /*
        will visually set the token in the game, but will also update the global state string!

        also will check to make sure moves are valid!

        step 1 check if player lost. if so end game

        step 2 test player input to make sure that the input is legal, in this case if it sets into a blank. if not just send a message illegal move try other spot.

        step 3 update stateString with player input token
        */
        void setToken()
        {
            //check for legal move in paint method as the keyboard input is handled by window. 
            playerCondition = endGame(botToken);          
            {
                //checks if game is over else, keep playing.
                if (playerCondition == 0)
                {                  
                    boardState[keyInput] = playerToken;
                   
                }
                playerCondition = endGame(playerToken);


            }
       
        }
    }; Player player;

    /*
a bot struct which only uses rng to play game.

step 1 check if bot lost, if so end game

step 2 use a vector to store blank spots, this is done for rng part

step 3 use rng to select a move.
*/
    //function to help improve learning of good moves!
    static int stategy(string state,char token)
    {

        
            int placement = 0;
            string Temp = state;

            for (int i = 0; i < state.size(); i++)
            {
                if (state[i] == ' ')
                {
                    string Temp = state;

                    Temp[i] = token;
                    //checking for possible loses, use blanks to figure it out.
                    if (
                        //check rows
                        Temp[0] == token && Temp[1] == token && Temp[2] == token ||
                        Temp[3] == token && Temp[4] == token && Temp[5] == token ||
                        Temp[6] == token && Temp[7] == token && Temp[8] == token ||
                        //check columns
                        Temp[0] == token && Temp[3] == token && Temp[6] == token ||
                        Temp[1] == token && Temp[4] == token && Temp[7] == token ||
                        Temp[2] == token && Temp[5] == token && Temp[8] == token ||
                        //check diagonals
                        Temp[0] == token && Temp[4] == token && Temp[8] == token ||
                        Temp[2] == token && Temp[4] == token && Temp[6] == token
                        )
                    {//if finds win situation, return the future token placement.
                        return i;
                    }
                }
            }
      
        return -1;
    }
    struct BotRNG
    {
              
        void setToken()
        {
            playerCondition = endGame(botToken);
         
            if (playerCondition == 0)
            {
                //strategy is used outside the RL algo as a way to have basic knowlegde.
                int block = stategy(boardState, botToken);
                int   win = stategy(boardState, playerToken);
                int strat = win != -1 ? win : block;

                if (strat == -1)
                {
                    //creates a vector which pushes the indexs of all the open spots on board.
                    vector<int> openSpots;
                    for (int i = 0; i < boardState.size(); i++)
                    {
                        if (boardState[i] == ' ')
                            openSpots.push_back(i);
                    }
                    //next using openSpot's size, we can generate a random choice of the openSpots index.
                    std::srand(static_cast<unsigned int>(std::time(nullptr)) + static_cast<unsigned int>(std::clock()));
                    int move = rand() % openSpots.size();


                    //input done with rng! using move to point to the openSpot index which points to the board position.
                    //places token finally.
                    boardState[openSpots[move]] = playerToken;
                }
                else
                {
                    boardState[strat] = playerToken;
                }
                //check win
                playerCondition = endGame(playerToken);

              
            }
           
        }
    }; BotRNG botTrainer;

    class botAI
    {

        //all components are hidden
    private:
        /*
        carries all the parts used to run the reinforcement learning algorithm!

        Using functions which change over time such as Policy and Reward
        and functions of memory management such as SearchGraph and
        */
        struct RLComponents
        {

            /*
            keeps track of states abd weights.

            */
            struct Node
            {

            public:
                //holds the current state before it is changed, used with GraphSearch
                string nodeState;
                //holds the data which relates to Policy function.
                vector<int> Weights;
                //used during reward function to point to certain weights used during a game!
                int  ActiveWeight = -1;

                /*
               constructor builds the optimal amount of weights for weightVector
               */
                Node(string state)
                {

                    //labels that state for future search
                    nodeState = state;
                    //variable used to create weight vector optimally
                    int blankCount = 0;
                    for (int i = 0; i < state.size(); i++)
                    {
                        blankCount += state[i] == ' ' ? 1 : 0;
                    }
                    //now the weight vector gets intialized with random weight.
                    for (int i = 0; i < blankCount; i++)
                    {
                        std::srand(static_cast<unsigned int>(std::time(nullptr)) + static_cast<unsigned int>(std::clock() * (i * i)));
                        //generates values between 1 and 200, this step doesn't matter much but makes intial training unique!
                        Weights.push_back(1/*rand() % 100 + 1*/);
                    }

                }

                //does the thinking using weighted RNG , returns the index of state.
                int Policy()
                {
                    cout << "weight vector size" << Weights.size() << endl;
                    //used to choose the index of weight which activated
                    int decisionPointer = 0;
                    //used to flag an activation
                    bool hit = false;
                    //count used for RNG
                    int count = 0;
                    //will continue to loop if a weight doesn't activate.
                    while (!hit)
                    {
                        //used to store a potential Activation
                        int potentialWeight = 0;
                        count++;
                        //may add a multiplier to make rng better.
                        std::srand(static_cast<unsigned int>(std::time(nullptr)) + static_cast<unsigned int>(std::clock() * count * count));
                        //first we select one of the weights at random
                        decisionPointer = rand() % Weights.size();
                        //set pW to that weight
                        potentialWeight = Weights[decisionPointer];
                        //may add a multiplier to make rng better.                                                                   multiplier
                        std::srand(static_cast<unsigned int>(std::time(nullptr)) + static_cast<unsigned int>(std::clock()) * (potentialWeight * decisionPointer * count));
                        //try to spike a hit, if it fails, it retries from beginning.
                        hit = rand() % potentialWeight == 0 ? true : false;
                    }
                    //used to save move made for reward function.
                    ActiveWeight = decisionPointer;
                    //used to point to the state which coresponds with the weight data chosen.
                    int stateDecision;

                    for (stateDecision = 0; stateDecision < nodeState.size(); stateDecision++)
                    {

                        //if blank, decrement dp
                        decisionPointer -= nodeState[stateDecision] == ' ' ? 1 : 0;
                        //when dp goes past zero, break loop to get the current state decision.
                        if (decisionPointer < 0) { break; }
                    }
                    //return move of Policy!
                    cout << "Policy Out : " << stateDecision << endl;

                    cout << "Policy Active Weight : " << ActiveWeight << endl;
                    return stateDecision;
                }

                //changes the state string by setting the token
                //this type of function changes depending on the application. for tic tac toe, this is good enough. 
                void UpdateState(int policy, string& state, char botToken)
                {
                    cout << "stateUpdate" << endl;
                    state[policy] = botToken;
                }


            };
            //the graph of the ai DATA
            vector<Node> NodeList;


            /*
           return Node index of matching state!
           */
            int searchGraph(string state)
            {
                for (int i = 0; i < NodeList.size(); i++)
                {
                    if (NodeList[i].nodeState == state)
                    {
                        return i;
                    }
                }
                //-1 flags not found!
                return -1;
            }

            /*
           creates a new node in memory.
           */
            void createNode(string state) //WORKS
            {
                //get node list working here!
                if (searchGraph(state) == -1)
                {
                    cout << "Node Created!" << endl;
                    NodeList.push_back(Node(state));
                }
            }


            /*
            takes in the Graph and updates the weights of all nodes used in the environment.

            the reward function has a way of scaling rewards per weight!

            //score is given by environment!
            */
            void Reward(int score)
            {

                /*
                change the weight of active nodes based on the score and its relation to the change is over time.


                step 1 : check nodes from recently active to lastest active. use a function to distribute the rewards a certain way!

                step 2 : change the weights value and then chnage active to false.

                repeat till all nodes are updated.
                */

                int totalActive = 0;
                //checks to make sure the list is greater than 0.
                cout << "nodeList Size : " << NodeList.size() << endl;

                if (NodeList.size() > 0)
                {
                    for (int i = NodeList.size() - 1; i >= 0; i--)
                    {
                        cout << "Reward - NI : " << i << endl;
                        //cout << "Policy current weight val : " << NodeList[i].Weights[NodeList[i].ActiveWeight] << endl;

                        //the active weight is a way to check if that node had an activation.
                        if (NodeList[i].ActiveWeight != -1)
                        {
                            cout << "reward active 1" << endl;
                            totalActive += 1;
                            //reward reduces the closer the state is to beginning state, this promotes better learning curve!
                            //weight simply adds the score with some scaling!



                            if (score > 0)//punish -  distributes score with a reduction based on how early the node was activated. So last move is punished more than first move.
                            {
                                NodeList[i].Weights[NodeList[i].ActiveWeight] += score / totalActive;
                                cout << "reward active 2" << endl;
                            }
                            else //reward - distributes score evenly, good situations have very little downsides.
                            {
                                NodeList[i].Weights[NodeList[i].ActiveWeight] += score;
                                cout << "reward active 3" << endl;
                                //if weight value is below 1, set it to one. this prevents / 0 and other errors with rng. a 1/1 probability is the best activation so its the cap.
                                NodeList[i].Weights[NodeList[i].ActiveWeight] = NodeList[i].Weights[NodeList[i].ActiveWeight] < 1 ? 1 : NodeList[i].Weights[NodeList[i].ActiveWeight];
                                cout << "reward active 4" << endl;
                            }

                            //set active to false!
                            NodeList[i].ActiveWeight = -1;
                            cout << "reward active 5" << endl;
                        }
                    }
                }
            }

            int getNodeListSize()
            {
                return NodeList.size();
            }

        };
    public:
        RLComponents RLfunction; //this object will be used when updating the model based on the flow chart provided in file!
        //if environment reward is 0 , no updates to weights, else the weights update!
        int EnvReward = 0;

        //control all bot actions here with simple functions!


       //this is the function that allows the bot to make a change to the state.
       /*
       step 1 : check if state has been seen, if not create a new node of that state
       step 2 : get a value from the policy for a given state input
       step 3 : update the state with a given policy num and token
       step 4 : reward function if environment output reward is true.

       */
        void AgentUpdate(string& state , char tokenP,char tokenB)
        {
            cout << "agentUp ER : " << EnvReward << endl;

            botCondition = endGame(tokenP);
            if (botCondition == 0)
            {
                //creates new node if state isn't seen before
                RLfunction.createNode(state);
                //get the index of the node
                int nodePtr = RLfunction.searchGraph(state);
                //get the policy output
                
                //strategy is used outside the RL algo as a way to have basic knowlegde.
                int block = stategy(boardState, playerToken);
                int   win = stategy(boardState, botToken);
                int strat = win != -1 ? win : block;
                if (strat == -1)
                {
                    int policy = RLfunction.NodeList[nodePtr].Policy();
                    //update state with policy,token
                    RLfunction.NodeList[nodePtr].UpdateState(policy, state, tokenB);
                }
                else
                {
                    //update state with policy,token
                    RLfunction.NodeList[nodePtr].UpdateState(strat, state, tokenB);
                }
                botCondition = endGame(tokenB);
            }


        }

        //for later implementation.
        void SaveDataToFile()
        {

        }
        //takes data from file and sets it in ai
        void LoadDataToAI()
        {

        }

    }; botAI bot;

}; ticTacToe game;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN: // Key pressed
        keys[wParam] = true; // Set the corresponding key as pressed
        break;
    case WM_KEYUP: // Key released
        keys[wParam] = false; // Set the corresponding key as released
        break;

        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        // Create a memory device context for double buffering

        HDC hdc = GetDC(hWnd);
        HDC hdcBuffer = CreateCompatibleDC(hdc);
        HBITMAP hBitmapBuffer = CreateCompatibleBitmap(hdc, 640, 480); // setWindow size
        SelectObject(hdcBuffer, hBitmapBuffer);
        int OffsetX = 500, OffsetY = 500;
        // Define a COLORREF variable
        COLORREF graphColor = RGB(100, 100, 100); // GRAY
        COLORREF pathColor = RGB(255, 255, 0); // YELLOW


        //UPDATE VARIABLES HERE! using simulation class!
        {
            //APPROXIMATE UPDATE RATE defualt 10 frame per sec
          //  this_thread::sleep_for(chrono::milliseconds(1000 / 1000));

            //UPDATE HANDLER!
            switch (UpdateSection)
            {
                /*
                Menu options at beginning of game
                current options:
                1 - choose who goes first
                               
                */
            case 'M':
            {
                DrawText(hdcBuffer, 0, 100, "please select who goes first : 1 - Player , 2 - Bot , 3 -bot Trainer vs RL bot 5000 games -> applies to bot moving second!");
                //handles first option here
                if (keyInput == 0)
                {
                    playerToken = 'X'; botToken = 'O';
                    UpdateSection = 'P';
                }
                else if(keyInput == 1)
                {
                    playerToken = 'O'; botToken = 'X';
                    UpdateSection = 'B';
                }
                else if (keyInput == 2)
                {
                    playerToken = 'X'; botToken = 'O';
                    BotTrain = true;
                    UpdateSection = 'T';
                }
              
                break;
            }
            /*
            players move block
            */
            case 'P':
            {
                DrawImage(hdcBuffer, 0, 0, 640, 480, getImage(0));
                DrawText(hdcBuffer, 0, 0, "Players Turn ");
                //handle player input here!
                if (game.player.legalMove())
                {
                    
                    game.player.setToken();

                    //reset player input
                    if (UpdateSection != 'E')
                    UpdateSection = 'B';
                }
                break;
            }
            /*
           players move block
           */
            case 'T':
            {
                
                DrawImage(hdcBuffer, 0, 0, 640, 480, getImage(0));
             //   DrawText(hdcBuffer, 0, 0, "Players Turn ");
                //handle player input here!
               
                    game.botTrainer.setToken();

                    //reset player input
                    if (UpdateSection != 'E')
                        UpdateSection = 'B';
                
                break;
                /*
                //  this_thread::sleep_for(chrono::milliseconds(1000/1));
                DrawImage(hdcBuffer, 0, 0, 640, 480, getImage(0));
                // DrawText(hdcBuffer, 0, 0, "Bots Turn ");
                 //handle bot input here!

                game.bot.AgentUpdate(boardState, botToken, playerToken);

                //reset player input
                if (UpdateSection != 'E')
                    UpdateSection = 'B';


                break;
                */
            }
            /*
            bot move block
            */
            case 'B':
            {
              //  this_thread::sleep_for(chrono::milliseconds(1000/1));
                DrawImage(hdcBuffer, 0, 0, 640, 480, getImage(0));
               // DrawText(hdcBuffer, 0, 0, "Bots Turn ");
                //handle bot input here!

                game.bot.AgentUpdate(boardState,playerToken,botToken);

                if (UpdateSection != 'E')
                {
                    if (BotTrain)
                    {
                        UpdateSection = 'T';
                    }
                    else
                    {
                        UpdateSection = 'P';
                    }
                }
               

                break;
            }          
            /*
            end game
            */
            case 'E':
            {
               
               
                trainingTime += 1;
                DrawImage(hdcBuffer, 0, 0, 640, 480, getImage(0));
               
                    //reward distribute       
                    if (playerCondition == 3) //loss
                    {
                        game.bot.EnvReward = 1000;
                        lossCount += 1;
                    }
                    if (botCondition == 2 || playerCondition == 2) //tie
                    {
                        game.bot.EnvReward = -200;
                        tieCount += 1;
                    }
                    if (botCondition == 3) //win
                    {
                        game.bot.EnvReward = -700;
                        winCount += 1;
                    }
                    botCondition = 0;
                  
                


                //REWARD FUNCTION HERE
                //reward update, has to be done by environment!
                if (game.bot.EnvReward != 0)
                {
                    //  cout << "reward value trigger" << endl;
                    game.bot.RLfunction.Reward(game.bot.EnvReward); game.bot.EnvReward = 0;
                }


                if (trainingTime > 5000 && BotTrain)
                {
                    UpdateSection = 'M'; BotTrain = false;
                }else
                if (!BotTrain) //trainer vs bot
                 //player vs bot
                {
                    //RESET game
                    this_thread::sleep_for(chrono::milliseconds(2000));
                    if (botToken == 'X')
                    {
                        UpdateSection = 'B';
                    }
                    else
                    {
                        UpdateSection = 'P';
                    }
                }
                else
                {
                    UpdateSection = 'T';
                }
                boardState = "         ";
              //  this_thread::sleep_for(chrono::milliseconds(1000 / 1));
                break;

            }
 
            }
            keyInput = -1;
        }
        //DRAW IMAGES HERE!
        {
            //token draw loop
            for (int i = 0; i < boardState.size(); i++)
            {
                //selects the token being read.
                int token = -1;
                if (boardState[i] != ' ')
                {
                    if (boardState[i] == 'X')
                    {
                        token = 1;
                    }
                    else
                    {
                        token = 2;
                    }
                }

                //simply draws the tokens from board to screen!
                switch (i)
                {
                    //top row coordinates
                case 0:
                {
                    DrawImage(hdcBuffer, 100, 20, 100, 100, getImage(token)); break;
                }
                case 1:
                {
                    DrawImage(hdcBuffer, 280, 20, 100, 100, getImage(token)); break;
                }
                case 2:
                {
                    DrawImage(hdcBuffer, 450, 20, 100, 100, getImage(token)); break;
                }
                     //middle row
                case 3:
                {
                    DrawImage(hdcBuffer, 100, 160, 100, 100, getImage(token)); break;
                }
                case 4:
                {
                    DrawImage(hdcBuffer, 280, 160, 100, 100, getImage(token)); break;
                }
                case 5:
                {
                    DrawImage(hdcBuffer, 450, 160, 100, 100, getImage(token)); break;
                }
                     //bottom row
                case 6:
                {
                    DrawImage(hdcBuffer, 100, 300, 100, 100, getImage(token)); break;
                }
                case 7:
                {
                    DrawImage(hdcBuffer, 280, 300, 100, 100, getImage(token)); break;
                }
                case 8:
                {
                    DrawImage(hdcBuffer, 450, 300, 100, 100, getImage(token)); break;
                }
                }

                DrawText(hdcBuffer,0,0,"total Nodes Made : " + to_string(game.bot.RLfunction.getNodeListSize()));
                DrawText(hdcBuffer, 0, 20, "total games Played : " + to_string(trainingTime));
                DrawText(hdcBuffer, 100, 460, " BOT STATS -> Loss : " + to_string(lossCount) + " Win : " + to_string(winCount) + " tie : " + to_string(tieCount));
             
               // gameDraw = false; gameWon = false;
            }
        }
        // Clean up
             //DETERMINES SIZE OF FRAME TO COPY
        BitBlt(hdc, 0, 0, 640, 480, hdcBuffer, 0, 0, SRCCOPY);
        DeleteObject(hBitmapBuffer);
        DeleteDC(hdcBuffer);
        ReleaseDC(hWnd, hdc);
        // Restore the original pen


    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
/*
Yay we made it to the end again! I hope you enjoyed the commentary :-)

*/

