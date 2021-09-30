/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSamples
 *
 *
 * @file src/AppTest.h
 * @author Olivier Falconnet
 * @date 20210929 - File creation
 */

#pragma once

#include "App.h"


namespace nxn {


    class AppSamples : public App {

    public:
        AppSamples();
        ~AppSamples();

    protected:
        virtual void    preInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    postInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    handleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSamples

} // namespace nxn
