/*
 * Name:
 * Date Submitted:
 * Lab Section:
 * Assignment Name:
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
using namespace std;

// Reflects what each node represents...
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x>>i) & 1; }
const int wolf=0, goat=1, cabbage=2, me=3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
void search(state source_node)
{
    queue<state> to_visit;
    to_visit.push(source_node);
    visited[source_node] = true;
    dist[source_node] = 0;

    while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
        if (!visited[n]) {
	        pred[n] = curnode;
	        dist[n] = 1 + dist[curnode];
	        visited[n] = true;
	        to_visit.push(n);
        }
    }
}

string state_string(state s)
{
    string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
    for (int i=0; i<4; i++)
        if (!bit(s, i)) result += items[i] + " ";
    result += " |river| ";
    for (int i=0; i<4; i++)
        if (bit(s, i)) result += items[i] + " ";
    return result;
}

// GENERIC
void print_path(state s, state t)
{
    if (s != t) {
        print_path(s, pred[t]);
        cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
    } 
    else {
        cout << "Initial state: " << state_string(s) << "\n";
    }
}

string neighbor_label(int s, int t)
{
    string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
    if (bit(s,me) == bit(t,me)) return "";  // must cross river
    int cross_with = 0;
    for (int i=0; i<3; i++) {
        if (bit(s,i) != bit(t,i) && bit(s,i)==bit(s,me)) { cross_with++; which_cross = items[i]; }
        if (bit(s,i) != bit(t,i) && bit(s,i)!=bit(s,me)) return "";
    }
    if (cross_with > 1) return "";
    if (cross_with == 0) return "Cross alone";
    else return "Cross with " + which_cross;
}

bool checkValid(bitset<4> currBits, bitset<4> newBits) {

    //if wolf and goat are on same side without person
    if(newBits[wolf] == newBits[goat] && newBits[me] != newBits[wolf]) {
        return false;
    }

    //if goat and cabbage are on same side without person
    if(newBits[goat] == newBits[cabbage] && newBits[me] != newBits[goat]) {
        return false;
    }

    int bitCounter = 0;

    for(int i = 0; i < 3; i++) {
        if(currBits[i] != newBits[i]) {
            bitCounter++;
            if(currBits[me] == newBits[me]) return false;
        }
    }
    if(bitCounter > 1) return false;    //if more than 2 things moved
    if(bitCounter == 0) return false;   //if nothing is moved

    //otherwise
    return true;
}

void build_graph(void)
{
    //Implement this function

    bitset<4> currState;    //0000
    bitset<4> newState;     //0000

    for(int c = 0; c < 16; c++) {   //iterate thru current states
        currState = c;  //initialize currState
        vector<state> nbrsVect; //create new vector for neighboring states
        
        for (int n = 0; n < 16; n++) {  //iterate thru all possible new states
            
            newState = n;   //initialize newState

            if(checkValid(currState, newState)) {
                //create pair
                pair<state, state> currToNew((state)currState.to_ulong(), (state)newState.to_ulong());
                
                //insert to edge_label
                edge_label.insert({ currToNew, neighbor_label(currToNew.first, currToNew.second) });

                //add to vector of neighbors
                nbrsVect.push_back((state)newState.to_ulong());

            cout << currState<< " --> " << newState << endl;
            }   
        }
        nbrs.insert({ (state)currState.to_ulong(), nbrsVect });
    }

}

int main(void)
{
  build_graph();

  state start = 0, end = 15;
  
  search(start);
  if (visited[end])
    print_path (start, end);
  else
    cout << "No path!\n";
  
  return 0;
}
