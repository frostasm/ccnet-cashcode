#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "cashcodeprotocol.h"

using namespace std;

int main(int /*argc*/, char */*argv*/[])
{

    CashCodeProtocol CCValidator;
    CCValidator.SetCashCodeTable({
                                     {0x00, 50},
                                     {0x01, 100},
                                     {0x02, 200},
                                     {0x03, 500},
                                     {0x04, 1000},
                                     {0x05, 5000}
                                 });

    CCValidator.ConnectValidator();
    CCValidator.PowerUpValidator();

    for(int i = 0; i < 1; i++)
    {
        cout << "-----------[ TRY " << i << " ]------------\n";
        CCValidator.EnableSequence();
        std::this_thread::sleep_for(std::chrono::seconds(5));
        CCValidator.StartListening();


        std::this_thread::sleep_for(std::chrono::seconds(10));
        cout << "Total Cash: " << dec << CCValidator.CashReceived() << endl;

        CCValidator.StopListening();
        CCValidator.DisableSequence();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "Done\n";

    return 0;
}
