/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleLighting
 *
 *
 * @file src/AppSimpleLighting.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_SIMPLE_LIGHTING
#define NXN_APP_SIMPLE_LIGHTING


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleLighting : public App {

    public:
        AppSimpleLighting               ();
        ~AppSimpleLighting              ();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleLighting

} // namespace nxn


#endif // !NXN_APP_SIMPLE_LIGHTING
