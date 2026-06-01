//********************************************
// Student Name			: Yin-Chi Chiu
// Student ID			: 113652039
// Student Email Address: andrew950324andrew@gmail.com
//********************************************
//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//
#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>

using namespace std;

namespace SYS_CONSTANTS {
     int max_int = INT_MAX;
     double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;

GRAPH_SYSTEM::GRAPH_SYSTEM( )
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION_ADD_EDGE;
    //mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    //mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
    //
    // modify and add your code heres
    //
}

void GRAPH_SYSTEM::initMemoryPool( )
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for ( int i = 0; i < mMaxNumNodes; ++i ) {
        mNodeArr_Pool[ i ].id = i;
    }
    for ( int i = 0; i < mMaxNumEdges; ++i ) {
        mEdgeArr_Pool[ i ].id = i;
    }
    //
    reset( );
}

void GRAPH_SYSTEM::reset( )
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for ( int i = 0; i < mCurNumOfFreeNodes; ++i ) {
        mFreeNodeArr[ i ] = i;
    }
    for ( int i = 0; i < mCurNumOfFreeEdges; ++i ) {
        mFreeEdgeArr[ i ] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}

GRAPH_NODE *GRAPH_SYSTEM::getFreeNode( ) 
{
    if ( mCurNumOfFreeNodes == 0 ) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[ mCurNumOfFreeNodes ];
    GRAPH_NODE *n = &mNodeArr_Pool[ id ];
    mActiveNodeArr[ mCurNumOfActiveNodes ] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}

GRAPH_EDGE *GRAPH_SYSTEM::getFreeEdge( )
{
    if ( mCurNumOfFreeEdges == 0 ) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[ mCurNumOfFreeEdges ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ id ];
    mActiveEdgeArr[ mCurNumOfActiveEdges ] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}

void GRAPH_SYSTEM::createDefaultGraph( )
{
    cout << "here"<< endl;
    reset( );

    float offset_x = 90.;
    float offset_z = 15.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0 );
	int n_1 = addNode(offset_x + 5.0, 0.0, offset_z + 0.0);
	int n_2 = addNode(offset_x + 0.0, 0.0, offset_z + 5.0);
    cout << "n_0:"<< n_0 << endl;

    //
    // modify and add your code heres
    //

    addEdge( n_0, n_1 );
    addEdge( n_1, n_2 );
}

void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset();

    if (n <= 0) return;

    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    const float PI = 3.14159265358979323846f;

    float r_inner = 15.0f;
    float r_outer = 25.0f;

    float offset_x = 90.0f;
    float offset_z = 15.0f;

    std::vector<int> inner_nodes;
    std::vector<int> outer_nodes;

    float angle_step = 2.0f * PI / (float)n;

    auto rand01 = []() -> float {
        return (float)rand() / (float)RAND_MAX;
        };

    // «Ø¥ß¤º°é»P¥~°é¸`ÂI
    for (int i = 0; i < n; ++i) {
        float angle_base = i * angle_step;

        float jitter_inner = (rand01() - 0.5f) * angle_step * 0.7f;
        float jitter_outer = (rand01() - 0.5f) * angle_step * 0.7f;

        float angle_inner = angle_base + jitter_inner;
        float angle_outer = angle_base + jitter_outer;

        float px_i = offset_x + r_inner * cos(angle_inner);
        float pz_i = offset_z + r_inner * sin(angle_inner);

        float px_o = offset_x + r_outer * cos(angle_outer);
        float pz_o = offset_z + r_outer * sin(angle_outer);

        inner_nodes.push_back(addNode(px_i, 0.0f, pz_i));
        outer_nodes.push_back(addNode(px_o, 0.0f, pz_o));
    }

    // §PÂ_ inner node ¨ì outer node ªº½u¬q¬O§_¤£¬ï¹L inner circle
    auto isCloseOuterNode = [&](const vector3& p_in, const vector3& p_out) -> bool {
        float ax = p_in.x - offset_x;
        float az = p_in.z - offset_z;

        float bx = p_out.x - offset_x;
        float bz = p_out.z - offset_z;

        // ÃöÁä§PÂ_¡G
        // ­Y (inner¦V¶q) dot (outer¦V¶q) >= r_inner^2
        // ¥Nªí½u¬q±q inner circle ©¹¥~¨«¡A¤£·|¬ï¹L inner circle ¤º³¡
        float dot = ax * bx + az * bz;

        return dot >= r_inner * r_inner - 0.001f;
        };

    // ¨C­Ó inner node ÀH¾÷³s¨ì¤@­Ó close outer node
    for (int i = 0; i < n; ++i) {
        // inner_nodes / outer_nodes store node IDs, not active-array indices.
        // Therefore do not call getNodeInfo(...) here; getNodeInfo expects 0..N-1.
        vector3 p_in = mNodeArr_Pool[inner_nodes[i]].p;

        std::vector<int> candidates;

        for (int j = 0; j < n; ++j) {
            vector3 p_out = mNodeArr_Pool[outer_nodes[j]].p;

            if (isCloseOuterNode(p_in, p_out)) {
                candidates.push_back(j);
            }
        }

        if (!candidates.empty()) {
            int chosen_index = rand() % (int)candidates.size();
            int chosen_outer = candidates[chosen_index];

            addEdge(inner_nodes[i], outer_nodes[chosen_outer]);
        }
    }
}


void GRAPH_SYSTEM::createNet_Circular( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 90.;
    float offset_z = 30.;

    std::vector<int> prev_layer_nodes;

    for (int layer = 1; layer <= num_layers; ++layer) {
        std::vector<int> curr_layer_nodes;
        float layer_radius = r + (layer - 1) * d;
        float angle_step = 2.0f * 3.14159f / n;

        for (int i = 0; i < n; ++i) {
            float angle = i * angle_step;
            float px = offset_x + layer_radius * cos(angle);
            float pz = offset_z + layer_radius * sin(angle);
            int node_id = addNode(px, 0.0, pz);
            curr_layer_nodes.push_back(node_id);
        }

        if (layer == 1) {
        } else {
            for (int i = 0; i < n; ++i) {
                addEdge(curr_layer_nodes[i], prev_layer_nodes[i]);
            }
        }

        // ³Ì¥~°é¤£¥Îµe (layer < num_layers) ¤~³s¥~Àô
        if (layer < num_layers) {
            for (int i = 0; i < n; ++i) {
                int next_i = (i + 1) % n;
                addEdge(curr_layer_nodes[i], curr_layer_nodes[next_i]);
            }
        }

        prev_layer_nodes = curr_layer_nodes;
    }
}
void GRAPH_SYSTEM::createNet_Square( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 5.;
    float offset_z = 5.;

    int W = n + 2 * (num_layers - 1);
    std::vector<std::vector<int>> grid(W, std::vector<int>(W, -1));

    for (int i = 0; i < W; ++i) {
        for (int j = 0; j < W; ++j) {
            bool in_hole = false;

            int inner_min = num_layers - 1;
            int inner_max = W - num_layers;

            if (i > inner_min && i < inner_max && j > inner_min && j < inner_max) {
                in_hole = true;
            }

            if (!in_hole) {
                float px = offset_x + i * dx;
                float pz = offset_z + j * dz;
                grid[i][j] = addNode(px, 0.0, pz);
            }
        }
    }

    for (int i = 0; i < W; ++i) {
        for (int j = 0; j < W; ++j) {
            if (grid[i][j] != -1) {
                if (i + 1 < W && grid[i+1][j] != -1) {
                    addEdge(grid[i][j], grid[i+1][j]);
                }
                if (j + 1 < W && grid[i][j+1] != -1) {
                    addEdge(grid[i][j], grid[i][j+1]);
                }
            }
        }
    }
}
void GRAPH_SYSTEM::createNet_RadicalCircular( int n ) {

    reset( );

    float offset_x = 90.0;
    float offset_z = 15.0;

    float r = 15; // radius

    int center_id = addNode(offset_x, 0.0, offset_z);

    float angle_step = 2.0f * 3.14159f / n;

    for (int i = 0; i < n; ++i) {
        float angle = i * angle_step;
        float px = offset_x + r * cos(angle);
        float pz = offset_z + r * sin(angle);

        int node_id = addNode(px, 0.0, pz);

        addEdge(node_id, center_id);
    }
}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode( float x, float y, float z, float r )
{
    GRAPH_NODE *g;
	g = getFreeNode();

	if (g == nullptr) return -1;
	g->p.x = x;
	g->p.y = y;
	g->p.z = z;
	g->r = r;

    g->edgeID.clear();
	g->visited = false;
    g->depth = -1;
    //g = getFreeNode( );
    //
    // modify and add your code heres
    //
    return g->id;
}

//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge( int nodeID_0, int nodeID_1 )
{
    GRAPH_EDGE *e;
    e = getFreeEdge( );
	if (e == nullptr) return -1;
    
	e->nodeID[0] = nodeID_0;
	e->nodeID[1] = nodeID_1;

	mNodeArr_Pool[nodeID_0].edgeID.push_back(e->id);
	mNodeArr_Pool[nodeID_1].edgeID.push_back(e->id);

    return e->id;
    //
    // modify and add your code heres
    //
}

void GRAPH_SYSTEM::askForInput( )
{
    //
    // modify and add your code heres
    //
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create a default graph" << endl;
    cout << "2: create a graph ..." << endl;
    cout << "3: create a graph ..." << endl;
    cout << "4: create a graph ..." << endl;
    cout << "5: create a graph ..." << endl;
    cout << "Delete: delete a selected node and all the edges incident to it" << endl;
    cout << "Spacebar: Change operation" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes and add edges" << endl;
    cout << "Click the left button to select/unselect or create an edge" << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;

}


GRAPH_NODE *GRAPH_SYSTEM::findNearestNode( double x, double z, double &cur_distance2 ) const
{
    GRAPH_NODE *n = nullptr;
    cur_distance2 = -1.0;

    for (int i = 0; i < mCurNumOfActiveNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE *curr_node = &mNodeArr_Pool[nodeID];

        double dx = curr_node->p.x - x;
        double dz = curr_node->p.z - z;
        double dist2 = dx * dx + dz * dz;

        if (n == nullptr || dist2 < cur_distance2) {
            n = curr_node;
            cur_distance2 = dist2;
        }
    }

    return n;
}

void GRAPH_SYSTEM::moveTo(double x, double y )
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}

//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE *nearest_node = findNearestNode( x, z, cur_d2 );
    if (nearest_node == 0 ) {
        mSelectedNode = 0;
        return;
    }
    if ( cur_d2 > nearest_node->r* nearest_node->r ) {
        mSelectedNode = 0;
        return;

    }
    if ( mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}

void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}

void GRAPH_SYSTEM::deleteEdge( int edgeID )
{
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];

    // ¦pªG³o­Ó edge ¤w¸g¤£¦b active ¤¤(¨¾¤î­«½Æ§R°£µ¥±¡ªp)
    if (e->id != edgeID) return; 

    // Retrieve active index
    int dynamicID = e->dynamicID;

    // ±N³o­Ó Edge ±q¸`ÂIªº½t±µ¦ê¦C¤¤²¾°£
    removeEdgeFromNode(e, e->nodeID[0]);
    removeEdgeFromNode(e, e->nodeID[1]);

    // ±N³o­Ó Edge ¦^¦¬¨ì free pool¡A±q active °}¦C¤¤²¾°£
    if (dynamicID >= 0 && dynamicID < mCurNumOfActiveEdges) {
        // Swap with the last active edge to keep array continuous
        int lastEdgeID = mActiveEdgeArr[mCurNumOfActiveEdges - 1];
        mActiveEdgeArr[dynamicID] = lastEdgeID;
        mEdgeArr_Pool[lastEdgeID].dynamicID = dynamicID; // §ó·s³Ì«á¨º­Óªº dynamicID

        // ©ñ¦^ free array
        mFreeEdgeArr[mCurNumOfFreeEdges] = edgeID;
        ++mCurNumOfFreeEdges;
        --mCurNumOfActiveEdges;
    }
}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];

    // ±q node ¤º³¡ºûÅ@ªºÃä ID list ¤¤§R°£¶Ç¤JªºÃä ID
    for (auto it = n->edgeID.begin(); it != n->edgeID.end(); ) {
        if (*it == e->id) {
            it = n->edgeID.erase(it);
        } else {
            ++it;
        }
    }
}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];

    // ¦]¬° deleteEdge ·|±q n->edgeID ·í¤¤²¾°£¤¸¯À¡A
    // ±q«á­±¤@ª½²¾°£ª½¨ìªÅ¬O³Ì¦w¥þ¡B¦³®Ä²vªº°µªk
    while (!n->edgeID.empty()) {
        deleteEdge(n->edgeID.back());
    }
}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    if ( mCurNumOfActiveNodes <= 0 ) return;
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];

    // 1. ¥ý±N³o­Ó¸`ÂIªg¤Wªº©Ò¦³Ãä³£§R°£
    deleteEdgesOfNode(nodeID);

    // 2. ±N¦¹¸`ÂI±q¬¡ÅD¨t²Î¤¤²¾°£¡B¦^¦¬¦Ü Free pool ¤¤
    int dynamicID = n->dynamicID;
    if (dynamicID >= 0 && dynamicID < mCurNumOfActiveNodes) {
        int lastNodeID = mActiveNodeArr[mCurNumOfActiveNodes - 1];
        mActiveNodeArr[dynamicID] = lastNodeID;
        mNodeArr_Pool[lastNodeID].dynamicID = dynamicID;

        mFreeNodeArr[mCurNumOfFreeNodes] = nodeID;
        ++mCurNumOfFreeNodes;
        --mCurNumOfActiveNodes;
    }
}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if ( mSelectedNode == 0 ) return;

    deleteNode(mSelectedNode->id);
    mSelectedNode = 0;
}

bool GRAPH_SYSTEM::isSelectedNode( ) const
{
    return mSelectedNode != 0;
}

void GRAPH_SYSTEM::getInfoOfSelectedPoint( double &r, vector3 &p ) const
{
    if ( isSelectedNode( ) == false ) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}

void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}

void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        n->depth = SYS_CONSTANTS::max_int;
        n->visited = false;
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
	if n is null, return
	set n’s depth to depth
	for each adjacent node m of n				; note m should not be n
		if (m’s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
	for each node n, set its depth to FLT_MAX
	k(selected_node, 0)
	update mMaxNodeDepth
}
*/

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    if (depth >= node->depth) return;

    node->depth = depth;
    node->visited = true;

    if (depth > mMaxNodeDepth) {
        mMaxNodeDepth = (double)depth;
    }

    for (size_t i = 0; i < node->edgeID.size(); ++i) {
        int edgeID = node->edgeID[i];
        if (edgeID < 0 || edgeID >= mMaxNumEdges) continue;

        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        if (e->dynamicID < 0 || e->dynamicID >= mCurNumOfActiveEdges) continue;
        if (mActiveEdgeArr[e->dynamicID] != edgeID) continue;

        int nextNodeID = -1;
        if (e->nodeID[0] == node->id) {
            nextNodeID = e->nodeID[1];
        }
        else if (e->nodeID[1] == node->id) {
            nextNodeID = e->nodeID[0];
        }

        if (nextNodeID < 0 || nextNodeID >= mMaxNumNodes) continue;

        computeDepthOfAllNodesFromSelectedNode(&mNodeArr_Pool[nextNodeID], depth + 1);
    }
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    resetDepthOfAllNodes();
    mMaxNodeDepth = 0;

    if (mSelectedNode == nullptr) return;

    int selectedDynamicID = mSelectedNode->dynamicID;
    if (selectedDynamicID < 0 || selectedDynamicID >= mCurNumOfActiveNodes) return;
    if (mActiveNodeArr[selectedDynamicID] != mSelectedNode->id) return;

    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0);

    // Compute the maximum reachable depth.  Unreachable nodes are set to -1 so
    // the color mapping does not treat INT_MAX as a real depth.
    int numNodes = getNumOfNodes();
    mMaxNodeDepth = 0;
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        if (n->depth == SYS_CONSTANTS::max_int) {
            n->depth = -1;
            n->visited = false;
        }
        else if (n->depth > mMaxNodeDepth) {
            mMaxNodeDepth = (double)n->depth;
        }
    }
}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    if (nodeIndex < 0 || nodeIndex >= mCurNumOfActiveNodes) return -1.0f;

    int nodeID = mActiveNodeArr[nodeIndex];
    const GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

    if (n->depth == SYS_CONSTANTS::max_int) return -1.0f;
    return (float)n->depth;
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        n->path_cost = SYS_CONSTANTS::max_double;
        n->path_parent = nullptr;
        n->visited = false;
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE* node)
{
    if (node == 0) return;
    if (node->path_cost >= SYS_CONSTANTS::max_double * 0.5) return;

    for (size_t i = 0; i < node->edgeID.size(); ++i) {
        int edgeID = node->edgeID[i];
        if (edgeID < 0 || edgeID >= mMaxNumEdges) continue;

        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        if (e->dynamicID < 0 || e->dynamicID >= mCurNumOfActiveEdges) continue;
        if (mActiveEdgeArr[e->dynamicID] != edgeID) continue;

        int nextNodeID = -1;
        if (e->nodeID[0] == node->id) {
            nextNodeID = e->nodeID[1];
        }
        else if (e->nodeID[1] == node->id) {
            nextNodeID = e->nodeID[0];
        }

        if (nextNodeID < 0 || nextNodeID >= mMaxNumNodes) continue;

        GRAPH_NODE* next = &mNodeArr_Pool[nextNodeID];

        double dx = node->p.x - next->p.x;
        double dy = node->p.y - next->p.y;
        double dz = node->p.z - next->p.z;
        double d = sqrt(dx * dx + dy * dy + dz * dz);
        double newCost = node->path_cost + d;

        if (newCost + 1.0e-8 < next->path_cost) {
            next->path_cost = newCost;
            next->path_parent = node;
            computeShortestPath(next);
        }
    }
}

void GRAPH_SYSTEM::computeShortestPath()
{
    resetPathInformationOfAllNodes();

    if (mStartNode == nullptr || mDestinationNode == nullptr) return;

    int startDynamicID = mStartNode->dynamicID;
    int destDynamicID = mDestinationNode->dynamicID;
    if (startDynamicID < 0 || startDynamicID >= mCurNumOfActiveNodes) return;
    if (destDynamicID < 0 || destDynamicID >= mCurNumOfActiveNodes) return;
    if (mActiveNodeArr[startDynamicID] != mStartNode->id) return;
    if (mActiveNodeArr[destDynamicID] != mDestinationNode->id) return;

    mStartNode->path_cost = 0.0;
    mStartNode->path_parent = nullptr;

    computeShortestPath(mStartNode);
}

void GRAPH_SYSTEM::handleKeyPressedEvent( unsigned char key )
{
    
    switch( key ) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode( );
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph( );
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(12, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 5);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles<3) 
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles> 36) 
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}

void GRAPH_SYSTEM::handlePassiveMouseEvent( double x, double z )
{
    double cur_d2;
    GRAPH_NODE *n = findNearestNode( x, z, cur_d2 );
    if ( n == 0 ) return;
    if ( cur_d2 > n->r*n->r ) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}

int GRAPH_SYSTEM::getNumOfNodes( ) const
{
    return mCurNumOfActiveNodes;
}

void GRAPH_SYSTEM::getNodeInfo( int nodeIndex, double &r, vector3 &p ) const
{
    int nodeID = mActiveNodeArr[ nodeIndex ];
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    r = n->r;
    p = n->p;
}

void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}

int GRAPH_SYSTEM::getNumOfEdges( ) const
{
    return mCurNumOfActiveEdges;
}

vector3 GRAPH_SYSTEM::getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const
{
    int edgeID = mActiveEdgeArr[ edgeIndex ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    return mNodeArr_Pool[ e->nodeID[ nodeIndex ] ].p;
}

void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}

//
// For every frame, update( ) function is called.
//
// 
void GRAPH_SYSTEM::update()
{
    if (!mFlgAutoNodeDeletion) {
        return;
    }
    if (mCurNumOfActiveNodes <= 0) {
        mFlgAutoNodeDeletion = false;
        return;
    }

    Sleep(250);

    int nodeIDToDelete = -1;

    // If a selected node is still active, delete it first.  Otherwise delete one
    // active node per frame until the graph becomes empty.
    if (mSelectedNode != 0) {
        int dynamicID = mSelectedNode->dynamicID;
        if (dynamicID >= 0 && dynamicID < mCurNumOfActiveNodes &&
            mActiveNodeArr[dynamicID] == mSelectedNode->id) {
            nodeIDToDelete = mSelectedNode->id;
        }
    }

    if (nodeIDToDelete < 0) {
        nodeIDToDelete = mActiveNodeArr[mCurNumOfActiveNodes - 1];
    }

    deleteNode(nodeIDToDelete);

    mSelectedNode = 0;
    mPassiveSelectedNode = 0;

    if (mCurNumOfActiveNodes <= 0) {
        mFlgAutoNodeDeletion = false;
    }
}