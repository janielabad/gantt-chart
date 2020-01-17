/// @author Janiel Venice C. Abad
/// @brief Implementation file for class ganttUtils
/// @file ganttUtils.cpp
/// @date 2018-11-29

#include "ganttUtils.h"
#include "linkedQueue.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iomanip>

ganttUtils::ganttUtils() {
    adjList = nullptr;
    milestoneCount = 0;
    taskCount = 0;
    sourceNode = 0;
    
    tasksMSRatio = 0;
    tasksDensity = 0;
    
    indeg = nullptr;
    outdeg = nullptr;
    keyMS = 0;
    nodePoint = 0;
    independentMS = nullptr;
    iMSCount = 0;
    
    aps = nullptr;
    
    topoNodes = nullptr;
    
    dist = nullptr;
    crPath = nullptr;
    crPathCount = 0;
    finalNode = 0;
    duration = 0;
    
    highID = 0;
    highIDCount = 0;
    lowID = 0;
    lowIDCount = 0;
    
    highOD = 0;
    highODCount = 0;
    lowOD = 0;
    lowODCount = 0;

    slackTimes = nullptr;
}

/// ----------------------------------------------------------------------------

ganttUtils::~ganttUtils() {
    if(adjList != nullptr) {
        listNode *curr = nullptr;
        listNode *temp = nullptr;

        for(int i = 0; i < milestoneCount; i++) {
            curr = adjList[i].head;
            
            while(curr != nullptr) {
                temp = curr;
                curr = curr->link;
                delete temp;
            }
            
            adjList[i].head = nullptr;
        }
        
        delete [] adjList;
        adjList = nullptr;
        curr = nullptr;
        temp = nullptr;
    }
    
    if(indeg != nullptr) {
        delete [] indeg;
        indeg = nullptr;
    }
    
    if(outdeg != nullptr) {
        delete [] outdeg;
        outdeg = nullptr;
    }
    
    if(independentMS != nullptr) {
        delete [] independentMS;
        independentMS = nullptr;
    }
    
    if(aps != nullptr) {
        delete [] aps;
        aps = nullptr;
    }
    
    if(topoNodes != nullptr) {
        delete [] topoNodes;
        topoNodes = nullptr;
    }

    if(dist != nullptr) {
        delete [] dist;
        dist = nullptr;
    }
    
    if(crPath != nullptr) {
        delete [] crPath;
        crPath = nullptr;
    }
    
    if(slackTimes != nullptr) {
        delete [] slackTimes;
        slackTimes = nullptr;
    }
}

/// ----------------------------------------------------------------------------

bool ganttUtils::getArguments(int argc, char *argv[], 
                              std::string& fileName, bool& printFlag) {
                                  
    using namespace std;
    
    if(argc == 1) {
        cout << "Usage: ./projectInfo -f <filename> [-p]\n";
        return false;
    }

    if(argc != 3 && argc != 4) {
        cout << "Error, invalid command line options.\n";
        return false;
    }
    
    string current;         // current argument
    bool fnFlag = false;    // flag for filename specifier
    printFlag = false;
    
    for(int i = 0; i < argc; i++) {
        current = string(argv[i]);
        
        // filename specifier
        if(current == "-f") {
            fnFlag = true;
            
            // filename argument
            if(i < argc - 1) {
                fileName = string(argv[i + 1]);
            }
            
            // check if file exists
            ifstream inFile;
            inFile.open(fileName);
            
            if(inFile) {
                inFile.close();
            } else {
                cout << "Error, can not find project file.\n";
                return false;
            }
        }
        
        // print flag
        if(current == "-p") {
            printFlag = true;
        }

    }
        
    if(!fnFlag) {
        cout << "Error, invalid project file name specifier.\n";
        return false;
    }
    
    if(argc == 4 && !printFlag) {
        cout << "Error, invalid print specifier.\n";
        return false;
    }

    return true;
}

/// ----------------------------------------------------------------------------

bool ganttUtils::readGraph(const std::string fileName) {
    using namespace std;
    
    bool valid = false;
    string ignore;
    int from = 0;          // source vertex
    int to = 0;            // destination vertex
    int edge = 0;          // edge weight
    
    ifstream inFile;
    inFile.open(fileName);
    
    if(inFile) {
        getline(inFile, title, '\n');
        
        //? where is the extra "title:" coming from
        if(title.substr(0, 6) == "title:") {
            title.erase(0, 6);
        }
        
        inFile >> ignore >> milestoneCount;
        inFile >> ignore >> sourceNode;
        
        if(milestoneCount > 0) {
            // create list
            adjList = new headNode[milestoneCount];
            
            for(int i = 0; i < milestoneCount; i++) {
                adjList[i].head = nullptr;
            }
            
            // add edges
            while(inFile >> from >> to >> edge) { 
                addEdge(from, to, edge);
            }
            
            valid = true;
        }
        
        inFile.close();
    }
    
    return valid;
}

/// ----------------------------------------------------------------------------

void ganttUtils::addEdge(int from, int to, int edge) {
    if(from < milestoneCount && to < milestoneCount 
       && from >= 0 && to >= 0 && from != to) {
           
        listNode *temp = new listNode;
        temp->dest = to;
        temp->weight = edge;
        temp->link = nullptr;
        
        // first connection
        if(adjList[from].head == nullptr) {
            adjList[from].head = temp;
            taskCount++;
        } else {
            listNode *curr = adjList[from].head;
            
            // existing connection
            if(curr->dest == to) {
                curr->weight = edge;
                delete temp;
                temp = nullptr;
                
            // new connection
            } else {
                temp->link = adjList[from].head;
                adjList[from].head = temp;
                taskCount++;
            }
        }
    }
}

/// ----------------------------------------------------------------------------

bool ganttUtils::isValidProject() {
    bool *visited = new bool[milestoneCount]{};
    bool *marked = new bool[milestoneCount]{};
    
    for(int i = 0; i < milestoneCount; i++) {
        visited[i] = false;
        marked[i] = false;
    }

    // check each node for an edge to itself or to one of its ancestors
    for(int v = 0; v < milestoneCount; v++) {
        if(!visited[v]) {
            if(isCycle(v, visited, marked)) {
                
                delete [] visited;
                visited = nullptr;
                
                delete [] marked;
                marked = nullptr;
                
                return false;
            }
        } 
    }
    
    delete [] visited;
    visited = nullptr;
    
    delete [] marked;
    marked = nullptr;

    return true;
}

/// ----------------------------------------------------------------------------

bool ganttUtils::isCycle(int v, bool *visited, bool *marked) {
    if(!visited[v]) {
        visited[v] = true;
        marked[v] = true;
        
        listNode *curr = adjList[v].head;
        int u = 0;
        
        while(curr != nullptr) {            
            u = curr->dest;
            
            if(isCycle(u, visited, marked)) {
                return true;
            } else if(marked[u]) {
                return true;
            }
            
            curr = curr->link;
        }
        
        marked[v] = false;
        return false;
    }

    return false;
}

/// ----------------------------------------------------------------------------

int ganttUtils::getTaskCount() const {
    return taskCount;
}

/// ----------------------------------------------------------------------------

void ganttUtils::findGraphInformation() {
    double t = static_cast<double>(taskCount);
    double m = static_cast<double>(milestoneCount);
    
    tasksMSRatio = t / m;
    tasksDensity = (2 * t) / (m * (m - 1));
}

/// ----------------------------------------------------------------------------

void ganttUtils::findKeyMilestone() {
    // find in-degrees and keep track of key milestone
    // keyMS: latest milestone with the maximum in-degree
    
    indeg = new int[milestoneCount]{};
    listNode *curr = nullptr;
    
    for(int i = 0; i < milestoneCount; i++) {
        indeg[i] = 0;
    }
    
    for(int v = 0; v < milestoneCount; v++) {
        curr = adjList[v].head;
        
        while(curr != nullptr) {
            indeg[curr->dest]++;
            curr = curr->link;
        }
        
        if(indeg[v] >= indeg[keyMS]) {
            keyMS = v;
        }
    }    
}

/// ----------------------------------------------------------------------------

void ganttUtils::findNodePoint() {
    // find out-degrees and keep track of node point
    // nodePoint: first milestone with the maximum out-degree

    outdeg = new int[milestoneCount]{};
    listNode *curr = nullptr;
    
    for(int i = 0; i < milestoneCount; i++) {
        outdeg[i] = 0;
    }
    
    for(int v = 0; v < milestoneCount; v++) {
        curr = adjList[v].head;
        
        while(curr != nullptr) {
            outdeg[v]++;
            curr = curr->link;
        }
        
        if(outdeg[v] > outdeg[nodePoint]) {
            nodePoint = v;
        }
    }
}

/// ----------------------------------------------------------------------------

void ganttUtils::findIndependentMilestones() {
    independentMS = new int[milestoneCount]{};
    
    for(int i = 0; i < milestoneCount; i++) {
        independentMS[i] = 0;
    }
    
    for(int v = 0; v < milestoneCount; v++) {
        if(outdeg[v] == 0) {
            independentMS[iMSCount] = v;
            iMSCount++;
        }
    }
}

/// ----------------------------------------------------------------------------

void ganttUtils::printGraphInformation() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << fixed
         << "Graph Information\n"
         << "   Project title: " << title << endl
         << "   Milestone Count: " << milestoneCount << endl
         << "   Task Count: " << taskCount << endl
         << "   Source Milestone: " << sourceNode << endl
         << "   Tasks/Milestones Ratio: " << tasksMSRatio << endl
         << "   Project Tasks Density: " << tasksDensity << endl << endl
         << "   Key Milestone: " << keyMS 
         << ", in-degree: " << indeg[keyMS] << " tasks\n"
         << "   Node Point: " << nodePoint
         << ", out-degree: " << outdeg[nodePoint] << " tasks\n" 
         << "   Independent Milestones" << endl;
         
    for(int i = 0; i < iMSCount; i++) {
        cout << ' ' << independentMS[i];
    }
    
    cout << "\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::findAPs() {
    aps = new bool[milestoneCount]{};
    
    bool *visited = new bool[milestoneCount]{};
    int *parent = new int[milestoneCount]{};
    int *low = new int[milestoneCount]{};
    int *discovered = new int[milestoneCount]{};

    
    for(int i = 0; i < milestoneCount; i++) {
        aps[i] = false;
        visited[i] = false;
        parent[i] = -1;
        low[i] = 0;
        discovered[i] = 0;
    }
    
    for(int v = 0; v < milestoneCount; v++) {
        if(!visited[v]) {
            findAPsHelper(v, visited, discovered, low, parent, aps);
        }
    }
    
    delete [] visited;
    visited = nullptr;
    
    delete [] parent;
    parent = nullptr;
    
    delete [] low;
    low = nullptr;
    
    delete [] discovered;
    discovered = nullptr;
    
}

/// ----------------------------------------------------------------------------

void ganttUtils::findAPsHelper(int v, bool *visited, int *disc, 
                               int *low, int *parent, bool *aps) {
    int children = 0;
    int time = 0;
    
    visited[v] = true;
    
    disc[v] = low[v] = ++time;
    
    listNode *curr = adjList[v].head;
    int u = 0;
        
    while(curr != nullptr) {            
        u = curr->dest;
        
        if(!visited[u]) {
            children++;
            parent[u] = v;
            
            findAPsHelper(u, visited, disc, low, parent, aps);
            
            low[v] = std::min(low[v], low[u]);
            
            if(parent[v] == -1 && children > 1) {
                aps[v] = true;
            }
            
            if(parent[v] != -1 && low[u] >= disc[v]) {
                aps[v] = true;
            }
        } else if(v != parent[v]) {
            low[v] = std::min(low[v], disc[u]);
        }
        
        curr = curr->link;
    }
}

/// ----------------------------------------------------------------------------

void ganttUtils::printAPs() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << "Articulation Points:\n";
    
    for(int v = 0; v < milestoneCount; v++) {
        if(aps[v]) {
            cout << ' ' << v;
        }
    }
    
    cout << "\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::topoSort() {
    topoNodes = new int[milestoneCount]{};
    int topoCount = 0;
    linkedQueue<int> topoQ;
    int *indegCopy = new int[milestoneCount]{};
    int v = 0;
    int u = 0;
    listNode *curr = nullptr;
    
    // add vertices with in-degree of 0 to queue
    for(int v = 0; v < milestoneCount; v++) {
        topoNodes[v] = 0;
        indegCopy[v] = indeg[v];
        
        if(indegCopy[v] == 0) {
            topoQ.addItem(v);
        }
    }
    
    while(!topoQ.isEmptyQueue()) {
        v = topoQ.front();
        topoQ.deleteItem();
        
        if(indegCopy[v] == 0) {
            topoNodes[topoCount] = v;
            topoCount++;
        }
        
        curr = adjList[v].head;
        
        // decrease in-degree by 1 for all of vertex's neighbors
        while(curr != nullptr) {
            u = curr->dest;
            indegCopy[u]--;
            
            // if in-degree of neighbor is set to 0, add it to queue
            if(indegCopy[u] == 0) {
                topoQ.addItem(u);
            }
            
            curr = curr->link;
        }
    }
    
    delete [] indegCopy;
    indegCopy = nullptr;
}

/// ----------------------------------------------------------------------------

void ganttUtils::printTopoSort() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << "Topological Sort: \n";
    
    for(int i = 0; i < milestoneCount; i++) {
        cout << ' ' << topoNodes[i];
    }
    
    cout << "\n\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::criticalPath() {
    dist = new int[milestoneCount]{};
    int *indegCopy = new int[milestoneCount]{};
    linkedQueue<int> critQ;
    int v = 0;
    int u = 0;
    listNode *curr = nullptr;
    
    crPath = new int[milestoneCount]{};
    
    for(int v = 0; v < milestoneCount; v++) {
        dist[v] = 0;
        indegCopy[v] = indeg[v];
        crPath[v] = -1;
        
        if(indegCopy[v] == 0) {
            critQ.addItem(v);
        }
    }

    while(!critQ.isEmptyQueue()) {
        v = critQ.front();
        critQ.deleteItem();
        
        curr = adjList[v].head;
        
        while(curr != nullptr) {
            u = curr->dest;
            
            dist[u] = std::max(dist[u], dist[v] + curr->weight);
            indegCopy[u]--;
            
            if(indegCopy[u] == 0) {
                critQ.addItem(u);
            }
            
            curr = curr->link;
        }
    }

    // find final task and project duration
    finalNode = sourceNode;
    
    for(v = 0; v < milestoneCount; v++) {
        if(dist[v] > duration) {
            duration = dist[v];
            finalNode = v;
        }
    }

    // reconstruct critical path
    crPath[crPathCount] = finalNode;
    crPathCount++;

    while(finalNode != sourceNode) {
        for(v = 0, u = 0; v < milestoneCount; v++) {
            curr = adjList[v].head;
            
            while(curr != nullptr) {
                u = curr->dest;
                
                if(finalNode == u) {
                    if(dist[finalNode] == dist[v] + curr->weight) {
                        crPath[crPathCount] = v;
                        crPathCount++;
                        finalNode = v;
                    }
                }
                
                curr = curr->link;
            }
        }
    }
    
    finalNode = crPath[0];

    delete [] indegCopy;
    indegCopy = nullptr;
}

/// ----------------------------------------------------------------------------

void ganttUtils::printCriticalPath() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << "Critical Path:\n"
         << "   Source Node: " << sourceNode << endl
         << "   Final Task: " << finalNode << endl
         << "   Total Duration: " << duration << endl << endl
         << "Critical Path: \n";
    
    for(int v = crPathCount - 1; v >= 0; v--) {
        cout << ' ' << crPath[v];
    }
    
    cout << "\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::findDependencyStats() {
    highID = indeg[1];
    lowID = indeg[1];
    highOD = outdeg[0];
    lowOD = outdeg[0];
    
    // find highest and lowest ID and OD
    for(int v = 0; v < milestoneCount; v++) {
        if(v != sourceNode) {
            if(indeg[v] > highID) {
                highID = indeg[v];
            } else if(indeg[v] < lowID) {
                lowID = indeg[v];
            }
        }
        
        if(v != finalNode) {
            if(outdeg[v] > highOD) {
                highOD = outdeg[v];
            } else if(outdeg[v] < lowOD) {
                lowOD = outdeg[v];
            }
        }
    }
    
    // find # of MS with highest and lowest ID and OD
    for(int v = 0; v < milestoneCount; v++) {
        if(v != sourceNode) {
            if(indeg[v] == highID) {
                highIDCount++;
            } else if(indeg[v] == lowID) {
                lowIDCount++;
            }
        }
        
        if(v != finalNode) {
            if(outdeg[v] == highOD) {
                highODCount++;
            } else if(outdeg[v] == lowOD) {
                lowODCount++;
            }
        }
    }
}

/// ----------------------------------------------------------------------------

void ganttUtils::printDependencyStats() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << "Dependency Statistics (in-degree):\n"
         << "   Highest In-Degree: " << highID << endl
         << "   Lowest In-Degree: " << lowID << endl
         << "   Count of Highest Degree: " << highIDCount << endl
         << "   Count of Lowest Degree: " << lowIDCount << endl << endl
         << "Dependency Statistics (out-degree):\n"
         << "   Highest Out-Degree: " << highOD << endl
         << "   Lowest Out-Degree: " << lowOD << endl
         << "   Count of Highest Degree: " << highODCount << endl
         << "   Count of Lowest Degree: " << lowODCount << "\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::findSlackTimes() {
    slackTimes = new int[milestoneCount]{};
    
    for(int i = 0; i < milestoneCount; i++) {
        slackTimes[i] = -1;
    }
    
    for(int i = 0; i < crPathCount; i++) {
        slackTimes[crPath[i]] = 0;
    }
    
    listNode *curr = nullptr;
    
    for(int u = 0; u < milestoneCount; u++) {
        if(slackTimes[u] == -1) {
            curr = adjList[u].head;
            
            while(curr != nullptr) {
                slackTimes[u] = dist[curr->dest] - (dist[u] + curr->weight);
                
                curr = curr->link;
            }
        }
    }
}

/// ----------------------------------------------------------------------------

void ganttUtils::printSlackTimes() {
    using namespace std;
    
    cout << "------------------------------------------------------------\n"
         << "Slack Times (task-slacktime):\n";
    
    for(int i = 0; i < milestoneCount && slackTimes[i] != -1; i++) {
        if(slackTimes[i] != 0) {
            cout << ' ' << i << '-' << slackTimes[i];
        }
    }
    
    cout << "\n\n";
}

/// ----------------------------------------------------------------------------

void ganttUtils::printGraph() {
    using namespace std;
    
    listNode *curr = nullptr;
    
    cout << "------------------------------------------------------------\n"
         << "Graph Adjacency List:\n   Title: " << title << endl << endl
         << "Vertex    vrt /weight | vrt /weight | vrt /weight | ...\n"
         << "------    ----------------------------------------------\n";
         
    for(int v = 0; v < milestoneCount; v++) {
        curr = adjList[v].head;
        
        cout << setw(6) << v << " -> ";
        
        if(curr == nullptr) { 
            cout << "   None";
        } else {
            while(curr != nullptr) {
                cout << setw(4) << curr->dest 
                     << '/' << setw(6) << curr->weight 
                     << " | ";
                
                curr = curr->link;
            }
        }
        
        cout << endl;
    }
    
    cout << endl;
}

/// ----------------------------------------------------------------------------
