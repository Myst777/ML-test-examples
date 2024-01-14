#pragma once
#ifndef RL_MODEL_H
#define RL_MODEL_H



/*
  Program: models basic reinforcment learning principles.
  Author: James MacDermot
  Date:1/10/24
  Description:

  Models a fundimental reinforcement learning algorithm where information about the environment is given as a state and some reward value, thats it.

  With very little information, it is possible to do complex sequences of inputs over many trials of "learning".
*/
//includes :
#include <string>
#include <iostream>
#include <Windows.h>
// for rand()
#include <cstdlib>
//file I/O
#include <fstream>
//includes for getting nums from a string 
#include <sstream>
//vectors
#include <vector>
//usings : 
using namespace std;
// structs go here//

class AI_Agent_RL
{

    //all important steps are hidden from user.
private:
    /*
    simply represents some game or grid simulation etc.
    */
    struct Environment
    {
        //the state string is the environment output.
        string state = "";
        int reward = 0;
        //constructor 
        Environment()
        {

        }
        /*
        changes state and reward
        */
        void Update() //this function handles everything for the environment state and its reward value!
        {
            //if creating environment elsewhere, this will simply not update the state and reward as it is being done elsewhere.

        }

    };
    /*
    controls evertyhing relating to the graph structure used.
    */
    struct Components
    {
        /*
        sub struct used to create a node. Nodes represent every state seen and its decision for each one.
        */
        struct Node
        {
        public:
            //member data
            string NodeState = "";
            int Life = 1000;
            //a variable useed to reset life.
            CONST int MaxLife = 1000;
            vector<int> weights;
            //used during Reward and Policy
            int activeWeight = -1;



            /*
            when inputing a state, make sure the first few indexs are for the Agents decisions -> ' ' and the rest as environment data -> some char.
            */
            Node(string state)
            {
                NodeState = state;
                //generate weight vector
                for (int i = 0; i < state.size(); i++)
                {
                    if (state[i] == ' ')
                    {
                        //initial weight is probability 1 out of 1, but will change over time!
                        weights.push_back(1);
                    }
                }
            }



            //when creating a node, it can have file data assigned to it with this function.
            void SetNodeData(string fileData)
            {
                //clean previous state just in case
                NodeState = "";
                weights.clear();

                //set new information
                bool stateData = false;
                string info = "";
                for (int i = 0; i < fileData.size(); i++)
                {
                    info += fileData[i];
                    //use commas to figure out how data is managed.
                    if (fileData[i + 1] == ',' || i == fileData.size() - 1)
                    {
                        if (stateData == false)
                        {//push state data to node.

                            NodeState = info; info = "";
                            stateData = true;
                        }
                        else
                        {//push weight data to node.
                            weights.push_back(stod(info)); info = "";
                        }
                        i += 1;
                    }

                }
            }



            //gets nodeState and Weights and pushes them to file data for txt file storage.
            string GetNodeData()
            {
                string nodeData = "";
                nodeData += NodeState;
                for (int i = 0; i < weights.size(); i++)
                {
                    //each piece of data separated by comma
                    nodeData += ',' + to_string(weights[i]);
                }
                return nodeData;
            }



        }; vector<Node*> NodeList; //graph of nodes.



        /*
        search function that also can generate new nodes.
        */
        Node* GetNode(string state)
        {


            //search for Node of matching state.
            for (int i = 0; i < NodeList.size(); i++)
            {
                if (NodeList[i] != nullptr && NodeList[i]->NodeState == state)
                {
                    return NodeList[i];
                }
            }

            // Create new node if not found.
            Node* n = new Node(state);
            NodeList.push_back(n);


            return n;
        }



        /*
        will delete Nodes which have a life value of zero. For board games this is similar to prunning branches that suck.

        best used at the end of a training session.
        */
        void ForgetNodes()
        {
            //size changes during node deletion, important to decrement "i" each time that happens.
            for (int i = 0; i < NodeList.size(); i++)
            {
                //when no life in node, its dead and is removed.
                if (NodeList[i]->Life < 1)
                {
                    delete NodeList[i]; // Assuming Node pointers need to be deleted
                    NodeList.erase(NodeList.begin() + i);
                    //decrement i to compensate for missing node.
                    i -= 1;
                }
            }
        }



        /*
        will update all node data such as the weights and its life. Input is a reward.
        */
        void UpdateNodes(int& reward)
        {
            int stateCount = 1;
            //loops and does two things, rewards and updates life variable.
            //we iterate through NodeList from back to front as new nodes are pushed to the back, meaning the last nodes activated are towards the back not front.
            for (int i = NodeList.size() - 1; i >= 0; i--)
            {
                //if not used, decrement life
                if (reward != 0 && NodeList[i]->activeWeight == -1)
                {
                    //if not rewarded, decrement life.
                    NodeList[i]->Life -= 1;
                }

                //for reward, it only touches the active Nodes.
                if (reward != 0 && NodeList[i]->activeWeight != -1)
                {

                    //specific weight selected for reward.
                    if (reward < 0)
                    {
                        //used a simple function to curve reward distribution for bad reward : r/sC
                        //could also use a linear function as well, depends on preference or environment learning.
                        NodeList[i]->weights[NodeList[i]->activeWeight] += reward / stateCount;
                        NodeList[i]->weights[NodeList[i]->activeWeight] = NodeList[i]->weights[NodeList[i]->activeWeight] < 1 ? 1 : NodeList[i]->weights[NodeList[i]->activeWeight];
                    }
                    else
                    {
                        //for good reward, the r is distributed evenly as all good moves have low downsides.
                        NodeList[i]->weights[NodeList[i]->activeWeight] += reward;
                    }

                    //reset activity to null
                    NodeList[i]->activeWeight = -1;

                    //give life to node.
                    NodeList[i]->Life = NodeList[i]->MaxLife;

                    //increment stateCount
                    stateCount += 1;
                }
            }
            //after weights are tweaked, reward must reset to 0 to start new trial
            reward = 0;
        }



        /*
        function determines whether a node edge weight spikes or not.
        */
        bool SpikeNode(Node* n)
        {
            return (n->weights.size() > n->activeWeight && n->activeWeight >= 0 && rand() % n->weights[n->activeWeight] == 0) ? true : false;
        }



        /*
        does the decision making based on weight values and rng.
        */
        int Policy(Node* n)
        {

            srand(time(NULL));
            //while no spike, probe random weight.
            while (!SpikeNode(n))
            {

                n->activeWeight = rand() % n->weights.size();
            }//when loop ends active weight is saved for reward.

            //loop here is used to map weight index to state index. The state index is returned.
            int p = n->activeWeight;
            for (int i = 0; i < n->NodeState.size(); i++)
            {
                if (n->NodeState[i] == ' ')
                {
                    p -= 1;//decrements every time a blank occurs, this is done to propery map the weight index to that state index.
                    if (p < 0)
                    {
                        cout << "Policy Output : " << i << endl;
                        return i;
                    }
                }
            }
            return 1;
        }
    };




    //create objects used in Update Agent & file handling

    Components component;
public:
    char token = '#';
    Environment environment;
    //simply allows agent to take an action. Does the env -> mem -> pol cycle.
    void UpdateAgent()
    {
        //basically updates environment, could be user input or just environment on its own.
        environment.Update();

        //Updates nodes after changes made in environment.
        component.UpdateNodes(environment.reward);

        //deletes old Nodes which are not active. Can be toggled in future to optimize training time!
        component.ForgetNodes();

        //get environement state -> get Node -> get Policy -> set decision token in state
        environment.state[component.Policy(component.GetNode(environment.state))] = token;



    }



    //deletes old graph replaces it with cleaned graph.
    void ResetModel()
    {
        //cleans model
        component.NodeList.clear();
    }



    //gets data from txt file and builds the graph using txt file info.
    void LoadData()
    {
        //makes sure list is clean before setting new data.
        component.NodeList.clear();

        //taking in file input 
        ifstream inputFile("RLdata.txt");

        //checking if file is opened.
        if (!inputFile.is_open())
        {
            cerr << "Failed to open data file!" << endl;
            return;
        }

        //line is used to get each line from txt file.
        string line;

        //loop used to read all the lines from txt and inserts data in new nodes.
        while (getline(inputFile, line))
        {
            istringstream iss(line);

            //will create the node if not exist. This is the case at beginning as the NodeList will be cleared first.
            component.GetNode("");
            //rewrites node data to have data from file.
            component.NodeList.back()->SetNodeData(line);
        }

    }



    //sets data into txt file to store the current model.
    void SaveData()
    {
        //taking in file input 
        ofstream outputFile("RLdata.txt");

        //checking if file is opened.
        if (!outputFile.is_open())
        {
            cerr << "Failed to open data file!" << endl;
            return;
        }

        //loop through NodeList and extract the data!
        for (int i = 0; i < component.NodeList.size(); i++)
        {
            outputFile << component.NodeList[i]->GetNodeData() << endl;
        }
    }

    void PrintData()
    {
        cout << "List Size : " << component.NodeList.size() << endl;
        //loop through NodeList and extract the data!
        for (int i = 0; i < component.NodeList.size(); i++)
        {
            cout << "Node : " << i + 1 << " state , weights[n] : " << component.NodeList[i]->GetNodeData() << endl;
        }
    }


};


#endif // RL_MODEL_H