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
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int,int> state;

// Each string in edge_label must be one of the following:
const string actions[] = {"Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A"};

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
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

// GENERIC
void print_path(state s, state t)
{
    if (s != t) {
        print_path(s, pred[t]);
        cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
    } else {
        cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
    }
}

void build_graph(void)
{
    //Implement this function
    int a = 0,
        b = 0;


    for(int itA = 0; itA <= 3; itA++) {   //iterate thru all possible states of A
        int a = itA;
        
        for(int itB = 0; itB <= 4; itB++) { //iterate thru all possible states of B
            int b = itB;
            vector<state> nbrsVect;

            for(int act = 0; act < 6; act++) {    //iterate thru all actions from state
                switch (act) {
                    case 0:     //fill A
                        a = 3;
                        break;
                    case 1:     //fill B
                        b = 4;
                        break;
                    case 2:     //empty A
                        a = 0;
                        break;
                    case 3:     //empty B
                        b = 0;
                        break;
                    case 4:     //pour A into B
                        if(a + b > 4) {
                            a = a - (4 - b);
                            b = 4;
                        } else {
                            b = b + a;
                            a = 0;
                        }
                        break;
                    case 5:     //pour B into A
                        if(a + b > 3) {
                            b = b - (3 - a);
                            a = 3;
                        } else {
                            a = a + b;
                            b = 0;
                        }
                        break;
                    default:
                        break;
                }
                nbrsVect.push_back(make_pair(a, b));

                edge_label.insert({ make_pair(make_pair(itA, itB), make_pair(a, b)), actions[act] });
            }
            nbrs.insert({ make_pair(itA, itB), nbrsVect});
        }
    }

}

int main(void)
{
    build_graph();

    state start = make_pair(0,0);
  
    for (int i=0; i<5; i++)
        nbrs[make_pair(i,5-i)].push_back(make_pair(-1,-1));
    search (start);
    if (visited[make_pair(-1,-1)]) 
        print_path (start, pred[make_pair(-1,-1)]);
    else
        cout << "No path!\n";
  
    return 0;
}
