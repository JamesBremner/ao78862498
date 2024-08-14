
#include <string>
#include <vector>
#include <wex.h>
#include <cxy.h>
#include "cGUI.h"
#include "cPile.h"

extern cPile thePile;

void cGUI::scale()
    {
        double xmin, xmax, ymin, ymax;
        xmax = ymax = 0;
        xmin = ymin = INT_MAX;
        for (auto &p : thePile.vnode)
        {
            if (p.x < xmin)
                xmin = p.x;
            if (p.y < ymin)
                ymin = p.y;
            if (p.x > xmax)
                xmax = p.x;
            if (p.y > ymax)
                ymax = p.y;
        }

        myXoff = xmin;
        myYoff = ymin;

        double xscale = 400 / (xmax - xmin);
        double yscale = 400 / (ymax - ymin);
        myScale = xscale;
        if (yscale < xscale)
            myScale = yscale;
    }
    void cGUI::visual(wex::shapes S)
    {
        for (int ie = 0; ie < thePile.edgeCount(); ie++)
        {
            std::vector<int> pxline;
            for (auto &v : thePile.edgeline(ie))
                pxline.push_back((int)myScale * (v - myXoff) + 10);
            S.line(pxline);
            S.circle(pxline[0], pxline[1], 5);
            S.circle(pxline[2], pxline[3], 5);
            pxline.clear();
            for (auto &v : thePile.edgelineCurrent(ie))
                pxline.push_back((int)myScale * (v - myXoff) + 10);
            pxline[0] += 500;
            pxline[2] += 500;
            S.line(pxline);
            S.circle(pxline[0], pxline[1], 5);
            S.circle(pxline[2], pxline[3], 5);
        }
        S.text("Start",{50,425});
        S.text("Finish",{600,425});
    }
