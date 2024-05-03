#include <map>
#include "../SplayTree.cpp"
#include "../libs/BigFloat.cpp"

/**
 * @brief 
 * DSU operations:
 *  find(u)
 *  union(u,v)
 *  make_set(u)
 * 
 * Retroactive operations:
 *  INSERT(t,dsu_operation) inserts at time t a dsu_operation
 *  DELETE(t) deletes the retroactive operation at time t
 * 
 * 
 * Simplifying assumptions:
 *  > union(u,v) is set only once for each pair (u,v) in the union-find.
 *  > You can only do one operation per time. (at t=1 you can only do 1 retroactive operation)
 *  > The timeline of operations will be ordered by BigFloat class.
 *  
 */


enum dsuOP_t{
    FIND=0,
    UNION=1,
    MAKESET=2
};


struct retroOP_t {
    dsuOP_t op;
    int f_node, s_node;
    void * position; // reference pointer to the link-cut node that corresponds to the operation.
};

class RetroDSU{

    private:
    map<BigFloat,retroOP_t> operations;
    // link_cut Tree; 
    

    public:
    RetroDSU();
    void Insert(BigFloat time, retroOP_t dsuOP_t);
    void Delete(BigFloat time); 
    void Query(BigFloat time, retroOP_t dsuOP_t);

    void DisplayTimeline(); // Displays the timeline of operations.

    

};