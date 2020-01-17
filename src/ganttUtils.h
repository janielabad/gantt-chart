/// @author Janiel Venice C. Abad
/// @brief Header file for class ganttUtils
/// @file ganttUtils.h
/// @date 2018-11-29

#ifndef H_GANTTUTILS
#define H_GANTTUTILS

#include <string>

struct listNode {
    int dest;          /// destination vertex
    int weight;        /// edge weight
    listNode *link;    /// link to next node
};

struct headNode {
    listNode *head;    /// pointer to head of list
};

class ganttUtils {
private:
    headNode *adjList;      /// adjacency list graph representation
    std::string title;      /// project title
    int milestoneCount;     /// # of milestones (vertices)
    int taskCount;          /// # of tasks (edges)
    int sourceNode;         /// source node
    
    double tasksMSRatio;    /// tasks/milestones ration
    double tasksDensity;    /// task density

    int *indeg;             /// indegrees
    int *outdeg;            /// outdegrees
    int keyMS;              /// key milestone
    int nodePoint;          /// node point
    int *independentMS;     /// independent milestones
    int iMSCount;           /// # of independent milestones
    
    bool *aps;              /// articulation points
    
    int *topoNodes;         /// topologically sorted milestones
    
    int *dist;              /// distances
    int *crPath;            /// critical path
    int crPathCount;        /// # of milestones in critical path
    int finalNode;          /// final milestone
    int duration;           /// project duration
    
    int highID;             /// highest in-degree
    int highIDCount;        /// # of milestones with in-degree of highID
    int lowID;              /// lowest in-degree
    int lowIDCount;         /// # of milestones with in-degree of lowID
    
    int highOD;             /// highest in-degree
    int highODCount;        /// # of milestones with in-degree of highID
    int lowOD;              /// lowest in-degree
    int lowODCount;         /// # of milestones with in-degree of lowID
    
    int *slackTimes;        /// slack times

public:
    /// -----------------------------------------------------------------------
    /// @brief (Constructor) Initializes class member variables as appropriate.
    ganttUtils();
    
    /// -----------------------------------------------------------------------
    /// @brief (Destructor) Deallocates memory for dynamic arrays.
    ~ganttUtils();
    
    /// -----------------------------------------------------------------------
    /// @brief Accepts and validates command line arguments.  The arguments 
    /// include the project tasks file ("-f <filename>") and an optional print 
    /// adjacency list flag ("-p").
    ///
    /// @param argc Argument count
    /// @param argv Arguments
    /// @param fileName Project tasks file
    /// @param printFlag Print adjacency list
    ///
    /// @return True if all arguments are valid, false if not
    bool getArguments(int argc, char *argv[],
                      std::string& fileName, bool& printFlag);
    
    /// -----------------------------------------------------------------------
    /// @brief Reads a formatted graph file containing a task list.  The file
    /// content is formatted as follows: 1) project title, 2) milestone count,
    /// 3) source node, 4-eof) source vertex, destination vertex, and edge
    /// weight in one line.  Verifies that milestone count and source node
    /// are valid.
    ///
    /// @param fileName Project tasks file
    ///
    /// @return True if file read was successful, false if not
    bool readGraph(const std::string fileName);
    
    /// -----------------------------------------------------------------------
    /// @brief Checks for cycles in the graph.  Calls isCycle().
    ///
    /// @return False if there is an edge from a node to itself or to one
    /// of its ancestors, true if there are none
    bool isValidProject();
    
    /// -----------------------------------------------------------------------
    /// @return Task count
    int getTaskCount() const;
    
    /// -----------------------------------------------------------------------
    /// @brief Calculates tasks/milestones ratio and the task density using
    /// provided formulas.
    void findGraphInformation();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds the latest milestone with the maximum in-degree.
    void findKeyMilestone();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds the first milestone with the maximum out-degree.
    void findNodePoint();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds all the milestones with an out-degree of 0.
    void findIndependentMilestones();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays the project title, milestone count, task count,
    /// source milestone, tasks/milestones ratio, project tasks density,
    /// key milestone, node point, and independent milestones.
    void printGraphInformation();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds the articulation points in the graph which are vertices
    /// that when removed (and the edges through them) disconnect the graph.
    /// Calls findAPsHelper().
    void findAPs();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays the articulation points of the graph.
    void printAPs();
    
    /// -----------------------------------------------------------------------
    /// @brief Implements Kahn's Topological Sort Algorithm to find a 
    /// topological ordering of the vertices.
    void topoSort();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays the topological sort for the vertices in the graph.
    void printTopoSort();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds the critical path in the directed acyclic graph.  For each
    /// vertex, it will find the minimum possible starting time of each
    /// activity.
    void criticalPath();
    
    /// -----------------------------------------------------------------------
    /// @brief Prints the source node, final task, total duration, and the
    /// vertices found in the critical path.
    void printCriticalPath();
    
    /// -----------------------------------------------------------------------
    /// @brief Finds the dependency statistics for the in-degrees and the
    /// out-degrees (i.e. value of lowest, highest, and counts for each).  
    /// In-degree stats exclude the source node and out-degree stats exclude
    /// the final node.
    void findDependencyStats();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays dependency statistics.
    void printDependencyStats();
    
    /// -----------------------------------------------------------------------
    /// @brief Computes the amount of time between the start of the next task
    /// and the end time of the current task.
    void findSlackTimes();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays slack times.
    void printSlackTimes();
    
    /// -----------------------------------------------------------------------
    /// @brief Displays the project title and a formatted adjacency list.
    void printGraph();

private:
    /// -----------------------------------------------------------------------
    /// @brief Adds an edge of passed weight from a source vertex to a 
    /// destination vertex.
    ///
    /// @param from Source vertex
    /// @param to Destination vertex
    /// @param edge Edge weight
    void addEdge(int from, int to, int edge);
    
    /// -----------------------------------------------------------------------
    /// @brief Recursively checks if a vertex has an edge from itself to itself
    /// or to one of its ancestors.
    ///
    /// @param visited
    /// @param marked
    ///
    /// @return True if there is a cycle, false if not
    bool isCycle(int v, bool *visited, bool *marked);
    
    /// -----------------------------------------------------------------------
    /// @brief Recursively finds the articulation points in the graph.
    ///
    /// @param v Vertex
    /// @param visited
    /// @param marked
    /// @param low
    /// @param parent
    /// @param aps
    void findAPsHelper(int v, bool *visited, int *disc, int *low, 
                       int *parent, bool *aps);
};

#endif /* H_GANTTUTILS */
