/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleTriangle
 *
 *
 * @file src/AppTest.h
 * @author Olivier Falconnet
 * @date 20210929 - File creation
 */

#pragma once

#include "../../lib-DXApp/src/App.h"


namespace nxn {


    class AppSimpleTriangle : public App {

    public:
        AppSimpleTriangle();
        ~AppSimpleTriangle();

    protected:
        virtual void    preInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    postInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    handleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleTriangle

} // namespace nxn
