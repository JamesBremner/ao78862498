#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "cxy.h"
#include "cPile.h"

extern cPile thePile;

void cPile::process(const std::string &fname)
{
    // read input
    read(fname);

    // repeatedly apply deformation
    for (;;)
    {

        deform();

        if (isIntersection())
        {
            // two edges have intersected, back up and finish
            deformReverse();
            break;
        }
    }

    // display initial and final node positions
    text();
}

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

void cPile::read(const std::string &fname)
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

void cPile::deform()
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
void cPile::deformReverse()
{
    if (!vcurrent.size())
    {
        vcurrent = vnode;
        return;
    }
    vcurrent[vdeform[0].node].x -= vdeform[0].xv;
    vcurrent[vdeform[0].node].y -= vdeform[0].yv;
}
std::vector<int>
cPile::moving()
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
bool cPile::isIntersection()
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
            if (cxy::isIntersection(p,
                                    n1me, n2me,
                                    n1fe, n2fe))
            {
                // check that intesection NOT at a shared node
                if (!(n1me.dist2(p) < delta || n2me.dist2(p) < delta))
                    return true;
            }
        }
    }
    // no edges intersect
    return false;
}
std::vector<double> cPile::edgeline(int ie)
{
    std::vector<double> ret;
    ret.push_back(vnode[vedge[ie].first].x);
    ret.push_back(vnode[vedge[ie].first].y);
    ret.push_back(vnode[vedge[ie].second].x);
    ret.push_back(vnode[vedge[ie].second].y);
    return ret;
}
std::vector<double> cPile::edgelineCurrent(int ie)
{
    std::vector<double> ret;
    ret.push_back(vcurrent[vedge[ie].first].x);
    ret.push_back(vcurrent[vedge[ie].first].y);
    ret.push_back(vcurrent[vedge[ie].second].x);
    ret.push_back(vcurrent[vedge[ie].second].y);
    return ret;
}
void cPile::text()
{
    std::cout << "starting position\n";
    for (auto &n : vnode)
        std::cout << n.x << " " << n.y << "\n";
    std::cout << "final position\n";
    for (auto &n : vcurrent)
        std::cout << n.x << " " << n.y << "\n";
}