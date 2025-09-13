#include "application.h"

int main() {
    // Initiate the app with the user given dimension data and name. 
    App app(SCR_WIDTH, SCR_HEIGHT, APP_NAME);

    // Run the application.
    app.run();

    return 0;
}