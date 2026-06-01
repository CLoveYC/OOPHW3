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

    float r = 15.0f;
    float d = 10.0f;
    float r_outer = r + d; // r_inner + layer distance

    float offset_x = 90.0f;
    float offset_z = 15.0f;

    std::vector<int> inner_nodes;
    std::vector<int> outer_nodes;

    float angle_step = 2.0f * PI / (float)n;

    auto rand01 = []() -> float {
        return (float)rand() / (float)RAND_MAX;
        };

    // ²£¥Í¤º°é»P¥~°é¸`ÂI
    for (int i = 0; i < n; ++i) {
        // Åý¨C­ÓÂI¦b¦Û¤vªº°Ï¶¡ªþªñÀH¾÷°¾²¾¡A³o¼Ë¨C¦¸«ö 5 ·|¤£¦P
        float jitter_inner = (rand01() - 0.5f) * angle_step * 0.7f;
        float jitter_outer = (rand01() - 0.5f) * angle_step * 0.7f;

        float angle_inner = i * angle_step + jitter_inner;
        float angle_outer = i * angle_step + jitter_outer;

        float px_i = offset_x + r * cos(angle_inner);
        float pz_i = offset_z + r * sin(angle_inner);

        float px_o = offset_x + r_outer * cos(angle_outer);
        float pz_o = offset_z + r_outer * sin(angle_outer);

        inner_nodes.push_back(addNode(px_i, 0.0f, pz_i));
        outer_nodes.push_back(addNode(px_o, 0.0f, pz_o));
    }

    // ¤£­n¥[³o¨â¬q¡A§_«h·|ÅÜ¦¨¨â­Ó¶êÀô¹Ï¡A¤£¬OÃD¥Ø¹Ï
    /*
    for (int i = 0; i < n; ++i) {
        addEdge(inner_nodes[i], inner_nodes[(i + 1) % n]);
        addEdge(outer_nodes[i], outer_nodes[(i + 1) % n]);
    }
    */

    auto segmentGoesInsideInnerCircle = [&](const vector3& a, const vector3& b) -> bool {
        float vx = b.x - a.x;
        float vz = b.z - a.z;

        float len2 = vx * vx + vz * vz;
        if (len2 < 0.000001f) return false;

        // §ä¶ê¤ß§ë¼v¨ì½u¬q ab ªº¦ì¸m t
        float cx = offset_x - a.x;
        float cz = offset_z - a.z;

        float t = (cx * vx + cz * vz) / len2;

        // t <= 0 ªí¥Ü³ÌªñÂI¦b inner node ºÝÂIªþªñ
        // ¦]¬° inner node ¥»¨Ó´N¦b inner circle ¤W¡A©Ò¥H¤£ºâ¬ï¹L
        if (t <= 0.0001f || t >= 1.0f) {
            return false;
        }

        float proj_x = a.x + t * vx;
        float proj_z = a.z + t * vz;

        float dx = proj_x - offset_x;
        float dz = proj_z - offset_z;

        float dist2 = dx * dx + dz * dz;

        // ¤p©ó¤º°é¥b®|¥­¤è¡A¥Nªí½u¬q¯uªº¶i¤J inner circle ¤º³¡
        return dist2 < r_inner * r_inner - 0.01f;
        };

    // ¨C­Ó inner node ³s¨ì¤@­Ó close outer node
    for (int i = 0; i < n; ++i) {
        vector3 p_in;
        double dummy_r;
        getNodeInfo(inner_nodes[i], dummy_r, p_in);

        std::vector<int> close_outer_nodes;

        for (int j = 0; j < n; ++j) {
            vector3 p_out;
            getNodeInfo(outer_nodes[j], dummy_r, p_out);

            if (!segmentGoesInsideInnerCircle(p_in, p_out)) {
                close_outer_nodes.push_back(j);
            }
        }

        if (!close_outer_nodes.empty()) {
            int pick = rand() % close_outer_nodes.size();
            int chosen_outer = close_outer_nodes[pick];

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
    //GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    //int dynamicID = e->dynamicID;

    //
    // modify and add your code heres
    //

}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{
    //GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //

}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{
   // GRAPH_NODE *n  = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //

}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    //if ( mCurNumOfActiveNodes <= 0 ) return;
    //GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //

}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if ( mSelectedNode == 0 ) return;
    //
    // modify and add your code heres
    //
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
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        //int nodeID = mActiveNodeArr[i];
        //GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        //
        // modify and add your code heres
        // 
        // set node's depth
        // and others if necessary
        //
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
    node->depth = depth;
    node->visited = true;
    //
    // modify and add your code heres
    //
    // for all edges incident to the node: node
        // get edge ID
        // get e = &mEdgeArr_Pool[edgeID];
        // n0 = &mNodeArr_Pool[e->nodeID[0]];
        // n1 = &mNodeArr_Pool[e->nodeID[1]];
        // determine the next node, which is set as the current node
        // update the current node's depth
        // recursively update all the nodes connected to the current node
        //     
    //for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // modify and add your code heres
        // 
        
    //}
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    // reset the depth of all nodes
    // if mSelectedNode is nullpute, return
    // set  mMaxNodeDepth = 0;
    // set mSelectedNode->depth as zero
    // invoke computeDepthOfAllNodesFromSelectedNode
    // finally, determine the mMaxNodeDepth
    // 
    
    //
    // modify and add your code heres
    //


    // Determine the mMaxNodeDepth
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        //
        // modify and add your code heres
        //
    }
}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;
    //
    // modify and add your code heres
    //
    return d;
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        //
        // modify and add your code heres
        //
        // set path cost of node
        // set path_parent of node
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE *node)
{
    if (node == 0) return;
    //
    // modify and add your code heres
    //    


    // for each edge incident to node, node 
    // DO
    for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // modify and add your code heres
        //
        // get an edge
        // determine the next node. Set it as the current node.
        // compute distance d: node->p.distance(next->p);
        // if new path cost is not better, check for the other edges
        // if new path cost is better, update the node's path cost and path_parent
        // Also, invokte computeShortestPath for the current node.
        //int edgeID = node->edgeID[i];
        //GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];

        //GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        //GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];
        //
        // modify and add your code heres
        //
    }
}

void GRAPH_SYSTEM::computeShortestPath()
{
    //
    // modify and add your code heres
    //
    // reset path information of all nodes
    // if mStartNode == nullptr || mDestinationNode == nullptr, return
    // invokte computeShortestPath with mStartNode
    //
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
        createNet_Square(15, 4);
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
void GRAPH_SYSTEM::update( )
{
    if (!mFlgAutoNodeDeletion) {
     
        return;
    }
    if (mCurNumOfActiveNodes<=0) {
     mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);

    //
    // modify and add your code
    // 
    // delete the selected node?
    // delete all the edges incident to the selected node?
    //

    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}