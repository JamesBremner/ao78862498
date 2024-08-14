struct sdeform
{
    int node;  // index of moving node
    double xv; // x direction
    double yv; // y direction
    double v;  // velocity

    sdeform(int n, double x, double y, double vel)
        : node(n), xv(x), yv(y), v(vel)
    {
    }
};

/// @brief  A pile of string

struct cPile
{
    std::vector<cxy> vnode;                 // initial node locations
    std::vector<cxy> vcurrent;              // current node locations
    std::vector<std::pair<int, int>> vedge; // edges between nodes
    std::vector<sdeform> vdeform;           // define deformations

    void process( const std::string& fname);

    void clear()
    {
        vnode.clear();
        vcurrent.clear();
        vedge.clear();
        vdeform.clear();
    }
    int edgeCount() const
    {
        return vedge.size();
    }
    /// @brief  Perform deformation
    void deform();

    // Reverse the last deformation
    void deformReverse();

    /// @brief identify the edges that are moving
    /// @return vector of indices of moving edges
    std::vector<int>
    moving();

    bool isIntersection();

    std::vector<double> edgeline(int ie);

    std::vector<double> edgelineCurrent(int ie);

    void text();

    private:

    /**
 * @brief read input
 *
 * @param fname input file path
 *
 * one line per node, giving its initial location
 * n x y
 *
 * one line per edge connecting nodes
 * e node_index1 node_index2
 *
 * one line per moving node
 * d node_index x_direction y_direction velocity
 *
 * Note only one moving node implemented
 */
void read(const std::string& fname);

};
