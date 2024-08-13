#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include <cxy.h>

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

struct spile
{
    std::vector<cxy> vnode;    // initial node locations
    std::vector<cxy> vcurrent; // current node locations
    std::vector<std::pair<int, int>> vedge; // edges between nodes
    std::vector<sdeform> vdeform;   // define deformations 

    void clear()
    {
        vnode.clear();
        vcurrent.clear();
        vedge.clear();
        vdeform.clear();
    }
    /// @brief  Perform deformation
    void deform()
    {
        if (!vcurrent.size())
        {
            vcurrent = vnode;
            return;
        }

        // TODO: implement more than one node moving
        vcurrent[vdeform[0].node].x += vdeform[0].xv;
        vcurrent[vdeform[0].node].y += vdeform[0].yv;
    }

    // Reverse the last deformation
    void deformReverse()
    {
        if (!vcurrent.size())
        {
            vcurrent = vnode;
            return;
        }
        vcurrent[vdeform[0].node].x -= vdeform[0].xv;
        vcurrent[vdeform[0].node].y -= vdeform[0].yv;
    }
    /// @brief identify the edges that are moving
    /// @return vector of indices of moving edges
    std::vector<int>
    moving()
    {
        std::vector<int> ret;
        for (int iedge = 0; iedge < vedge.size(); iedge++)
        {
            // check if edge includes a moving node
            // TODO: implement more then one moving node
            if (vedge[iedge].first == vdeform[0].node ||
                vedge[iedge].second == vdeform[0].node)
                ret.push_back(iedge);
        }
        return ret;
    }
    bool isIntersection()
    {
        // maximum distance squared that will be ignored
        const double delta = 0.01;

        cxy p;

        // loop over moving edges
        for (int iedgemoving : moving())
        {
            auto n1me = vcurrent[vedge[iedgemoving].first];
            auto n2me = vcurrent[vedge[iedgemoving].second];

            // loop over fixed edges
            for (int iedgefixed = 0; iedgefixed < vedge.size(); iedgefixed++)
            {
                if (iedgefixed == iedgemoving)
                    continue;
                auto n1fe = vcurrent[vedge[iedgefixed].first];
                auto n2fe = vcurrent[vedge[iedgefixed].second];

                // check for intersection
                if (cxy::isIntersection( p,
                        n1me, n2me,
                        n1fe,n2fe))
                {
                    // check that intesection NOT at a shared node
                    if( ! ( n1me.dist2(p) < delta || n2me.dist2(p) < delta ) )
                        return true;
                }
            }
        }
        // no edges intersect
        return false;
    }

    void text()
    {
        std::cout << "starting position\n";
        for( auto& n : vnode )
            std::cout << n.x <<" "<< n.y << "\n";
        std::cout << "final position\n";
        for( auto& n : vcurrent )
         std::cout << n.x <<" "<< n.y << "\n";
    }
};

spile thePile; // the pile of string

std::vector<std::string>
ParseSpaceDelimited(
    const std::string &l)
{
    std::vector<std::string> token;
    std::stringstream sst(l);
    std::string a;
    while (getline(sst, a, ' '))
        token.push_back(a);

    // remove empty tokens
    token.erase(
        remove_if(
            token.begin(),
            token.end(),
            [](std::string t)
            {
                return (t.empty());
            }),
        token.end());

    return token;
}
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
void read(const std::string fname)
{
    thePile.clear();
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error("Cannot open input file");
    std::string line;
    while (getline(ifs, line))
    {
        auto vtoken = ParseSpaceDelimited(line);
        switch (line[0])
        {
        case 'n':
            thePile.vnode.emplace_back(atof(vtoken[1].c_str()), atof(vtoken[2].c_str()));
            break;
        case 'e':
            thePile.vedge.emplace_back(atoi(vtoken[1].c_str()), atoi(vtoken[2].c_str()));
            break;
        case 'd':
            thePile.vdeform.emplace_back(
                atoi(vtoken[1].c_str()),
                atof(vtoken[2].c_str()),
                atof(vtoken[3].c_str()),
                atof(vtoken[4].c_str()));
            break;
        }
    }
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

main()
{
    //cGUI theGUI;

    // read input
    read( "../dat/data1.txt");

    // repeatedly apply deformation
    for( ; ; ) {

        thePile.deform();

        if( thePile.isIntersection() ) {
            // two edges have intersected, back up and finish
            thePile.deformReverse();
            break;
        }
    }

    // display initial and final node positions
    thePile.text();
    return 0;
}
