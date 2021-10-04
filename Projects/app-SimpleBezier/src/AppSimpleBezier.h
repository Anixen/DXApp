/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleBezier
 *
 *
 * @file src/AppSimpleBezier.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_SIMPLE_BEZIER
#define NXN_APP_SIMPLE_BEZIER


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleBezier : public App {

    public:
        AppSimpleBezier                 ();
        ~AppSimpleBezier                ();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleBezier

} // namespace nxn


#endif // !NXN_APP_SIMPLE_BEZIER
